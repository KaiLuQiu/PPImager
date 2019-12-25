
//
//  antrace.h
//  ImageProcessing
//  potrace, 参考:https://github.com/jiangpeng79/antrace
//  参考文献:https://blog.csdn.net/linxinboy/article/details/19020917
//  Created by 邱开禄 on 2019/12/25.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#ifndef ANTRACE_H
#define ANTRACE_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <math.h>

#include "potracelib.h"
#include "backend_pdf.h"
#include "backend_eps.h"
#include "backend_pgm.h"
#include "backend_svg.h"
#include "backend_xfig.h"
#include "backend_dxf.h"
#include "backend_geojson.h"
#include "potracelib.h"
#include "bitmap.h"
#include "platform.h"
#include "auxiliary.h"
#include "progress_bar.h"
#include "trans.h"
#include "curve.h"
// 定义全局变量
//struct info_s info;
struct info_s antrace_info;

#define UNDEF ((double)(1e30))

struct backend_s {
  char *name;       /* name of this backend */
  char *ext;        /* file extension */
  int fixed;        /* fixed page size backend? */
  int pixel;        /* pixel-based backend? */
  int multi;        /* multi-page backend? */
  int (*init_f)(FILE *fout);                 /* initialization function */
  int (*page_f)(FILE *fout, potrace_path_t *plist, imginfo_t *imginfo);
                                             /* per-bitmap function */
  int (*term_f)(FILE *fout);                 /* finalization function */
  int opticurve;    /* opticurve capable (true Bezier curves?) */
};
typedef struct backend_s backend_t;

static backend_t backend[] = {
  { "eps",        ".eps", 0, 0, 0,   NULL,     page_eps,     NULL,     1 },
  { "postscript", ".ps",  1, 0, 1,   init_ps,  page_ps,      term_ps,  1 },
  { "ps",         ".ps",  1, 0, 1,   init_ps,  page_ps,      term_ps,  1 },
  { "pdf",        ".pdf", 0, 0, 1,   init_pdf, page_pdf,     term_pdf, 1 },
  { "pdfpage",    ".pdf", 1, 0, 1,   init_pdf, page_pdfpage, term_pdf, 1 },
  { "svg",        ".svg", 0, 0, 0,   NULL,     page_svg,     NULL,     1 },
  { "dxf",        ".dxf", 0, 1, 0,   NULL,     page_dxf,     NULL,     1 },
  { "geojson",    ".json",0, 1, 0,   NULL,     page_geojson, NULL,     1 },
  { "pgm",        ".pgm", 0, 1, 1,   NULL,     page_pgm,     NULL,     1 },
  { "gimppath",   ".svg", 0, 1, 0,   NULL,     page_gimp,    NULL,     1 },
  { "xfig",       ".fig", 1, 0, 0,   NULL,     page_xfig,    NULL,     0 },
  { NULL, NULL, 0, 0, 0, NULL, NULL, NULL, 0 },
};


class antrace {
public:
    antrace();
    ~antrace();
    bool traceImage(unsigned char *data, int width, int height, char * path);
    int backend_lookup(char *name, backend_t **bp);
private:
    void calc_dimensions(imginfo_t *imginfo, potrace_path_t *plist);
    void initInfo(char const* filetype);
    bool saveToFile(char* path, int width, int height, const char* filetype);
    
    double double_of_dim(dim_t d, double def) {
        if (d.d) {
            return d.x * d.d;
        } else {
            return d.x * def;
        }
    }
//    info_s antrace_info;

    potrace_state_t* s_state;
};


#endif

