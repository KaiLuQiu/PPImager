
//
//  antrace.cpp
//  ImageProcessing
//
//  Created by 邱开禄 on 2019/12/17.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#include "antrace.h"

antrace::antrace() {
    s_state = NULL;
}

antrace::~antrace() {
    if (NULL != s_state) {
        potrace_state_free(s_state);
        s_state = NULL;
    }
}

int antrace::backend_lookup(char *name, backend_t **bp) {
  int i;
  int m=0;  /* prefix matches */
  backend_t *b = NULL;

  for (i=0; backend[i].name; i++) {
    if (strcasecmp(backend[i].name, name)==0) {
      *bp = &backend[i];
      return 0;
    } else if (strncasecmp(backend[i].name, name, strlen(name))==0) {
      m++;
      b = &backend[i];
    }
  }
  /* if there was no exact match, and exactly one prefix match, use that */
  if (m==1) {
    *bp = b;
    return 0;
  } else if (m) {
    return 2;
  } else {
    return 1;
  }
}
 
void antrace::calc_dimensions(imginfo_t *imginfo, potrace_path_t *plist)
{
  double dim_def;
  double maxwidth, maxheight, sc;
  int default_scaling = 0;

  /* we take care of a special case: if one of the image dimensions is
     0, we change it to 1. Such an image is empty anyway, so there
     will be 0 paths in it. Changing the dimensions avoids division by
     0 error in calculating scaling factors, bounding boxes and
     such. This doesn't quite do the right thing in all cases, but it
     is better than causing overflow errors or "nan" output in
     backends.  Human users don't tend to process images of size 0
     anyway; they might occur in some pipelines. */
  if (imginfo->pixwidth == 0) {
    imginfo->pixwidth = 1;
  }
  if (imginfo->pixheight == 0) {
    imginfo->pixheight = 1;
  }

  /* set the default dimension for width, height, margins */
  if (antrace_info.backend->pixel) {
    dim_def = DIM_PT;
  } else {
    dim_def = DEFAULT_DIM;
  }

  /* apply default dimension to width, height, margins */
  imginfo->width = antrace_info.width_d.x == UNDEF ? UNDEF : double_of_dim(antrace_info.width_d, dim_def);
  imginfo->height = antrace_info.height_d.x == UNDEF ? UNDEF : double_of_dim(antrace_info.height_d, dim_def);
  imginfo->lmar = antrace_info.lmar_d.x == UNDEF ? UNDEF : double_of_dim(antrace_info.lmar_d, dim_def);
  imginfo->rmar = antrace_info.rmar_d.x == UNDEF ? UNDEF : double_of_dim(antrace_info.rmar_d, dim_def);
  imginfo->tmar = antrace_info.tmar_d.x == UNDEF ? UNDEF : double_of_dim(antrace_info.tmar_d, dim_def);
  imginfo->bmar = antrace_info.bmar_d.x == UNDEF ? UNDEF : double_of_dim(antrace_info.bmar_d, dim_def);

  /* start with a standard rectangle */
  trans_from_rect(&imginfo->trans, imginfo->pixwidth, imginfo->pixheight);

  /* if info.tight is set, tighten the bounding box */
  if (antrace_info.tight) {
    trans_tighten(&imginfo->trans, plist);
  }

  /* sx/rx is just an alternate way to specify width; sy/ry is just an
     alternate way to specify height. */
  if (antrace_info.backend->pixel) {
    if (imginfo->width == UNDEF && antrace_info.sx != UNDEF) {
      imginfo->width = imginfo->trans.bb[0] * antrace_info.sx;
    }
    if (imginfo->height == UNDEF && antrace_info.sy != UNDEF) {
      imginfo->height = imginfo->trans.bb[1] * antrace_info.sy;
    }
  } else {
    if (imginfo->width == UNDEF && antrace_info.rx != UNDEF) {
      imginfo->width = imginfo->trans.bb[0] / antrace_info.rx * 72;
    }
    if (imginfo->height == UNDEF && antrace_info.ry != UNDEF) {
      imginfo->height = imginfo->trans.bb[1] / antrace_info.ry * 72;
    }
  }

  /* if one of width/height is specified, use stretch to determine the
     other */
  if (imginfo->width == UNDEF && imginfo->height != UNDEF) {
    imginfo->width = imginfo->height / imginfo->trans.bb[1] * imginfo->trans.bb[0] / antrace_info.stretch;
  } else if (imginfo->width != UNDEF && imginfo->height == UNDEF) {
    imginfo->height = imginfo->width / imginfo->trans.bb[0] * imginfo->trans.bb[1] * antrace_info.stretch;
  }

  /* if width and height are still variable, tenatively use the
     default scaling factor of 72dpi (for dimension-based backends) or
     1 (for pixel-based backends). For fixed-size backends, this will
     be adjusted later to fit the page. */
  if (imginfo->width == UNDEF && imginfo->height == UNDEF) {
    imginfo->width = imginfo->trans.bb[0];
    imginfo->height = imginfo->trans.bb[1] * antrace_info.stretch;
    default_scaling = 1;
  }

  /* apply scaling */
  trans_scale_to_size(&imginfo->trans, imginfo->width, imginfo->height);

  /* apply rotation, and tighten the bounding box again, if necessary */
  if (antrace_info.angle != 0.0) {
    trans_rotate(&imginfo->trans, antrace_info.angle);
    if (antrace_info.tight) {
      trans_tighten(&imginfo->trans, plist);
    }
  }

  /* for fixed-size backends, if default scaling was in effect,
     further adjust the scaling to be the "best fit" for the given
     page size and margins. */
  if (default_scaling && antrace_info.backend->fixed) {

    /* try to squeeze it between margins */
    maxwidth = UNDEF;
    maxheight = UNDEF;

    if (imginfo->lmar != UNDEF && imginfo->rmar != UNDEF) {
      maxwidth = antrace_info.paperwidth - imginfo->lmar - imginfo->rmar;
    }
    if (imginfo->bmar != UNDEF && imginfo->tmar != UNDEF) {
      maxheight = antrace_info.paperheight - imginfo->bmar - imginfo->tmar;
    }
    if (maxwidth == UNDEF && maxheight == UNDEF) {
      maxwidth = max(antrace_info.paperwidth - 144, antrace_info.paperwidth * 0.75);
      maxheight = max(antrace_info.paperheight - 144, antrace_info.paperheight * 0.75);
    }

    if (maxwidth == UNDEF) {
      sc = maxheight / imginfo->trans.bb[1];
    } else if (maxheight == UNDEF) {
      sc = maxwidth / imginfo->trans.bb[0];
    } else {
      sc = min(maxwidth / imginfo->trans.bb[0], maxheight / imginfo->trans.bb[1]);
    }

    /* re-scale coordinate system */
    imginfo->width *= sc;
    imginfo->height *= sc;
    trans_rescale(&imginfo->trans, sc);
  }

  /* adjust margins */
  if (antrace_info.backend->fixed) {
    if (imginfo->lmar == UNDEF && imginfo->rmar == UNDEF) {
      imginfo->lmar = (antrace_info.paperwidth-imginfo->trans.bb[0])/2;
    } else if (imginfo->lmar == UNDEF) {
      imginfo->lmar = (antrace_info.paperwidth-imginfo->trans.bb[0]-imginfo->rmar);
    } else if (imginfo->lmar != UNDEF && imginfo->rmar != UNDEF) {
      imginfo->lmar += (antrace_info.paperwidth-imginfo->trans.bb[0]-imginfo->lmar-imginfo->rmar)/2;
    }
    if (imginfo->bmar == UNDEF && imginfo->tmar == UNDEF) {
      imginfo->bmar = (antrace_info.paperheight-imginfo->trans.bb[1])/2;
    } else if (imginfo->bmar == UNDEF) {
      imginfo->bmar = (antrace_info.paperheight-imginfo->trans.bb[1]-imginfo->tmar);
    } else if (imginfo->bmar != UNDEF && imginfo->tmar != UNDEF) {
      imginfo->bmar += (antrace_info.paperheight-imginfo->trans.bb[1]-imginfo->bmar-imginfo->tmar)/2;
    }
  } else {
    if (imginfo->lmar == UNDEF) {
      imginfo->lmar = 0;
    }
    if (imginfo->rmar == UNDEF) {
      imginfo->rmar = 0;
    }
    if (imginfo->bmar == UNDEF) {
      imginfo->bmar = 0;
    }
    if (imginfo->tmar == UNDEF) {
      imginfo->tmar = 0;
    }
  }
}

void antrace::initInfo(char const* filetype)
{
    backend_lookup((char*)filetype, &antrace_info.backend);
    antrace_info.debug = 0;
    antrace_info.width_d.x = UNDEF;
    antrace_info.height_d.x = UNDEF;
    antrace_info.rx = UNDEF;
    antrace_info.ry = UNDEF;
    antrace_info.sx = UNDEF;
    antrace_info.sy = UNDEF;
    antrace_info.stretch = 1;
    antrace_info.lmar_d.x = UNDEF;
    antrace_info.rmar_d.x = UNDEF;
    antrace_info.tmar_d.x = UNDEF;
    antrace_info.bmar_d.x = UNDEF;
    antrace_info.angle = 0;
    antrace_info.paperwidth = DEFAULT_PAPERWIDTH;
    antrace_info.paperheight = DEFAULT_PAPERHEIGHT;
    antrace_info.tight = 0;
    antrace_info.unit = 10;
    antrace_info.compress = 1;
    antrace_info.pslevel = 2;
    antrace_info.color = 0xffffff;
    antrace_info.gamma = 2.2;
    /*
    info.param = potrace_param_default();
    if (!info.param) {
        return JNI_FALSE;
    }
    */
    antrace_info.longcoding = 0;
    antrace_info.outfile = NULL;
    antrace_info.blacklevel = 0.5;
    antrace_info.invert = 0;
    antrace_info.opaque = 1;
    antrace_info.grouping = 1;
    antrace_info.fillcolor = 0x000000;
    antrace_info.progress = 0;
    antrace_info.progress_bar = DEFAULT_PROGRESS_BAR;
}


bool antrace::traceImage(unsigned char *data, int width, int height, char* path)
{
    potrace_param_t* param_t = potrace_param_default();
    param_t->turdsize = 15;
    param_t->opttolerance = 0.8;
    potrace_bitmap_t* bmp_t = bm_new(width, height);
    const int kShiftBits = 20;
    const int32_t kRedRatio = static_cast<int32_t>((1 << kShiftBits) * 0.21f);
    const int32_t kGreenRatio = static_cast<int32_t>((1 << kShiftBits) * 0.71f);
    const int32_t kBlueRatio = static_cast<int32_t>((1 << kShiftBits) * 0.07f);
    
    unsigned char *pdata = data;
    for (int i = 0; i < height; i++) {
       for (int j = 0; j < width; j++) {
            int32_t src_red = pdata[0];
            int32_t src_green = pdata[1];
            int32_t src_blue = pdata[2];
            int32_t src_alpha = pdata[3];

            int32_t dst_color = (kRedRatio * src_red + kGreenRatio * src_green +
                    kBlueRatio * src_blue) >> kShiftBits;
            if (dst_color > 128) {
              BM_PUT(bmp_t, j, height - 1 - i, 1);
            }
            else
            {
              BM_PUT(bmp_t, j, height - 1 - i, 0);
            }
           pdata += 4;
        }
    }
    
    if(s_state != NULL) {
        potrace_state_free(s_state);
        s_state = NULL;
    }
    s_state = potrace_trace(param_t, bmp_t);
    potrace_param_free(param_t);
    
    saveToFile(path, width, height, "svg");
 
    bm_free(bmp_t);
    
    if (!s_state || s_state->status != POTRACE_STATUS_OK) {
        return false;
    }
    return true;
}

bool antrace::saveToFile(char* path, int width, int height, const char* filetype)
{
    imginfo_t imginfo;
    imginfo.pixwidth = width;
    imginfo.pixheight = height;
    initInfo(filetype);
    calc_dimensions(&imginfo, s_state->plist);
    FILE *f = fopen(path, "w+");
    if (f) {
         struct backend_s *b = antrace_info.backend;
         if (b->init_f) {
             if(0 != b->init_f(f)) {
                 fclose(f);
                 return false;
             }
         }
         b->page_f(f, s_state->plist, &imginfo);
         if (b->term_f) {
             if(0 != b->term_f(f)) {
                 fclose(f);
                 return false;
             }
         }
         fclose(f);
     }
    return true;
}
