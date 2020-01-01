
//
//  antrace.cpp
//  PPImager
//
//  Created by 邱开禄 on 2019/12/17.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#include "antrace.h"
#include "ImageCommon.h"
#include "nanosvg/nanosvg.h"
#include "nanosvg/nanosvgrast.h"

// 定义全局变量
struct info_s potrace_info;

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
  if (potrace_info.backend->pixel) {
    dim_def = DIM_PT;
  } else {
    dim_def = DEFAULT_DIM;
  }

  /* apply default dimension to width, height, margins */
  imginfo->width = potrace_info.width_d.x == UNDEF ? UNDEF : double_of_dim(potrace_info.width_d, dim_def);
  imginfo->height = potrace_info.height_d.x == UNDEF ? UNDEF : double_of_dim(potrace_info.height_d, dim_def);
  imginfo->lmar = potrace_info.lmar_d.x == UNDEF ? UNDEF : double_of_dim(potrace_info.lmar_d, dim_def);
  imginfo->rmar = potrace_info.rmar_d.x == UNDEF ? UNDEF : double_of_dim(potrace_info.rmar_d, dim_def);
  imginfo->tmar = potrace_info.tmar_d.x == UNDEF ? UNDEF : double_of_dim(potrace_info.tmar_d, dim_def);
  imginfo->bmar = potrace_info.bmar_d.x == UNDEF ? UNDEF : double_of_dim(potrace_info.bmar_d, dim_def);

  /* start with a standard rectangle */
  trans_from_rect(&imginfo->trans, imginfo->pixwidth, imginfo->pixheight);

  /* if potrace_info.tight is set, tighten the bounding box */
  if (potrace_info.tight) {
    trans_tighten(&imginfo->trans, plist);
  }

  /* sx/rx is just an alternate way to specify width; sy/ry is just an
     alternate way to specify height. */
  if (potrace_info.backend->pixel) {
    if (imginfo->width == UNDEF && potrace_info.sx != UNDEF) {
      imginfo->width = imginfo->trans.bb[0] * potrace_info.sx;
    }
    if (imginfo->height == UNDEF && potrace_info.sy != UNDEF) {
      imginfo->height = imginfo->trans.bb[1] * potrace_info.sy;
    }
  } else {
    if (imginfo->width == UNDEF && potrace_info.rx != UNDEF) {
      imginfo->width = imginfo->trans.bb[0] / potrace_info.rx * 72;
    }
    if (imginfo->height == UNDEF && potrace_info.ry != UNDEF) {
      imginfo->height = imginfo->trans.bb[1] / potrace_info.ry * 72;
    }
  }

  /* if one of width/height is specified, use stretch to determine the
     other */
  if (imginfo->width == UNDEF && imginfo->height != UNDEF) {
    imginfo->width = imginfo->height / imginfo->trans.bb[1] * imginfo->trans.bb[0] / potrace_info.stretch;
  } else if (imginfo->width != UNDEF && imginfo->height == UNDEF) {
    imginfo->height = imginfo->width / imginfo->trans.bb[0] * imginfo->trans.bb[1] * potrace_info.stretch;
  }

  /* if width and height are still variable, tenatively use the
     default scaling factor of 72dpi (for dimension-based backends) or
     1 (for pixel-based backends). For fixed-size backends, this will
     be adjusted later to fit the page. */
  if (imginfo->width == UNDEF && imginfo->height == UNDEF) {
    imginfo->width = imginfo->trans.bb[0];
    imginfo->height = imginfo->trans.bb[1] * potrace_info.stretch;
    default_scaling = 1;
  }

  /* apply scaling */
  trans_scale_to_size(&imginfo->trans, imginfo->width, imginfo->height);

  /* apply rotation, and tighten the bounding box again, if necessary */
  if (potrace_info.angle != 0.0) {
    trans_rotate(&imginfo->trans, potrace_info.angle);
    if (potrace_info.tight) {
      trans_tighten(&imginfo->trans, plist);
    }
  }

  /* for fixed-size backends, if default scaling was in effect,
     further adjust the scaling to be the "best fit" for the given
     page size and margins. */
  if (default_scaling && potrace_info.backend->fixed) {

    /* try to squeeze it between margins */
    maxwidth = UNDEF;
    maxheight = UNDEF;

    if (imginfo->lmar != UNDEF && imginfo->rmar != UNDEF) {
      maxwidth = potrace_info.paperwidth - imginfo->lmar - imginfo->rmar;
    }
    if (imginfo->bmar != UNDEF && imginfo->tmar != UNDEF) {
      maxheight = potrace_info.paperheight - imginfo->bmar - imginfo->tmar;
    }
    if (maxwidth == UNDEF && maxheight == UNDEF) {
      maxwidth = max(potrace_info.paperwidth - 144, potrace_info.paperwidth * 0.75);
      maxheight = max(potrace_info.paperheight - 144, potrace_info.paperheight * 0.75);
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
  if (potrace_info.backend->fixed) {
    if (imginfo->lmar == UNDEF && imginfo->rmar == UNDEF) {
      imginfo->lmar = (potrace_info.paperwidth-imginfo->trans.bb[0])/2;
    } else if (imginfo->lmar == UNDEF) {
      imginfo->lmar = (potrace_info.paperwidth-imginfo->trans.bb[0]-imginfo->rmar);
    } else if (imginfo->lmar != UNDEF && imginfo->rmar != UNDEF) {
      imginfo->lmar += (potrace_info.paperwidth-imginfo->trans.bb[0]-imginfo->lmar-imginfo->rmar)/2;
    }
    if (imginfo->bmar == UNDEF && imginfo->tmar == UNDEF) {
      imginfo->bmar = (potrace_info.paperheight-imginfo->trans.bb[1])/2;
    } else if (imginfo->bmar == UNDEF) {
      imginfo->bmar = (potrace_info.paperheight-imginfo->trans.bb[1]-imginfo->tmar);
    } else if (imginfo->bmar != UNDEF && imginfo->tmar != UNDEF) {
      imginfo->bmar += (potrace_info.paperheight-imginfo->trans.bb[1]-imginfo->bmar-imginfo->tmar)/2;
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
    backend_lookup((char*)filetype, &potrace_info.backend);
    potrace_info.debug = 0;
    potrace_info.width_d.x = UNDEF;
    potrace_info.height_d.x = UNDEF;
    potrace_info.rx = UNDEF;
    potrace_info.ry = UNDEF;
    potrace_info.sx = UNDEF;
    potrace_info.sy = UNDEF;
    potrace_info.stretch = 1;
    potrace_info.lmar_d.x = UNDEF;
    potrace_info.rmar_d.x = UNDEF;
    potrace_info.tmar_d.x = UNDEF;
    potrace_info.bmar_d.x = UNDEF;
    potrace_info.angle = 0;
    potrace_info.paperwidth = DEFAULT_PAPERWIDTH;
    potrace_info.paperheight = DEFAULT_PAPERHEIGHT;
    potrace_info.tight = 0;
    potrace_info.unit = 10;
    potrace_info.compress = 1;
    potrace_info.pslevel = 2;
    potrace_info.color = 0xffffff;
    potrace_info.gamma = 2.2;
    /*
    potrace_info.param = potrace_param_default();
    if (!potrace_info.param) {
        return JNI_FALSE;
    }
    */
    potrace_info.longcoding = 0;
    potrace_info.outfile = NULL;
    potrace_info.blacklevel = 0.5;
    potrace_info.invert = 0;
    potrace_info.opaque = 1;
    potrace_info.grouping = 1;
    potrace_info.fillcolor = 0x000000;
    potrace_info.progress = 0;
    potrace_info.progress_bar = DEFAULT_PROGRESS_BAR;
}


bool antrace::traceImage(unsigned char *data, int width, int height, const char* path)
{
    bool ret = false;
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
            int32_t src_red = pdata[RED];
            int32_t src_green = pdata[GREEN];
            int32_t src_blue = pdata[BLUE];
            int32_t src_alpha = pdata[ALPHA];

            int32_t dst_color = (kRedRatio * src_red + kGreenRatio * src_green +
                    kBlueRatio * src_blue) >> kShiftBits;
            if (dst_color > 200) {
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
    
    ret = saveToFile(path, width, height, "svg");
 
    bm_free(bmp_t);
    
    if (!s_state || s_state->status != POTRACE_STATUS_OK) {
        ret = false;
    }
    return ret;
}

bool antrace::saveToFile(const char* path, int width, int height, const char* filetype)
{
    imginfo_t imginfo;
    imginfo.pixwidth = width;
    imginfo.pixheight = height;
    initInfo(filetype);
    calc_dimensions(&imginfo, s_state->plist);

    FILE *f = fopen(path, "w+");
    if (f) {
         struct backend_s *b = potrace_info.backend;
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

unsigned char* antrace::readBufferFromFile(const char* path, int& width, int& height)
{
    FILE *f = fopen(path, "rb");
    
    fseek(f,0,SEEK_SET);
    long begin = ftell(f);
    fseek(f,0,SEEK_END);
    long end = ftell(f);
    long filesize = end - begin;
    fseek(f,0,SEEK_SET);
    char* buffer = (char*)malloc(filesize * sizeof(char));
    if (NULL == buffer) {
        printf("antrace: buffer is NULL!!!\n");
        return NULL;
    }
    memset(buffer, 0, filesize * sizeof(char));
    int size = fread(buffer, sizeof(char), filesize, f);
    NSVGimage* nsvgImage = nsvgParse(buffer, "px", 96.0f);
    if (NULL == nsvgImage) {
        printf("antrace: nsvgImage is NULL!!!\n");
        return NULL;
    }
    int nsvgImageWidth = (int)nsvgImage->width;
    int nsvgImageHeight = (int)nsvgImage->height;
    unsigned char* svgBuffer = (unsigned char*)malloc(nsvgImageWidth * nsvgImageHeight * 4);
    if (NULL == svgBuffer) {
        printf("antrace: svgBuffer is NULL!!!\n");
        return NULL;
    }
    NSVGrasterizer *svgRast = nsvgCreateRasterizer();
    if (NULL == svgRast) {
        printf("antrace: create svgRast is fail!!!\n");
        return NULL;
    }
    nsvgRasterize(svgRast, nsvgImage, 0, 0, 1., svgBuffer, nsvgImageWidth, nsvgImageHeight, nsvgImageWidth * 4);
    
    // 像素预乘
    unsigned char* pdata = svgBuffer;
    for (int i = 0; i < nsvgImageWidth; i++) {
        for(int j = 0; j < nsvgImageHeight; j++) {
            if(pdata[ALPHA] != 255) {
                pdata[RED] = pdata[RED] * pdata[ALPHA] / 255;
                pdata[GREEN] = pdata[GREEN] * pdata[ALPHA] / 255;
                pdata[BLUE] = pdata[BLUE] * pdata[ALPHA] / 255;
            }
            pdata[ALPHA] = 255;
            pdata += 4;
        }
    }
    
    width = nsvgImageWidth;
    height = nsvgImageHeight;
    fclose(f);
    SAFE_FREE(buffer);
    return svgBuffer;
}
