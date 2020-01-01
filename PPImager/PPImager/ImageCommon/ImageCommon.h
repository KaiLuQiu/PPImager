//
//  ImageCommon.h
//  PPlayer
//
//  Created by 邱开禄 on 2019/11/14.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#ifndef ImageCommon_H
#define ImageCommon_H

#include <list>
#include <vector>
#include <string.h>
#include <stdlib.h>

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#define NANOSVGRAST_IMPLEMENTATION
#define NANOSVG_IMPLEMENTATION

#define RED     0
#define GREEN   1
#define BLUE    2
#define ALPHA   3



#define NS_IMAGE_BEGIN namespace image {
#define NS_IMAGE_END  }

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) { if (x) delete (x); (x) = NULL; }    //定义安全释放函数
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) { if (x) delete [] (x); (x) = NULL; }    //定义安全释放函数
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(p) if(p != NULL) {free(p); p = NULL;}
#endif

#ifndef SAFE_DELETE_TEXTURE
#define SAFE_DELETE_TEXTURE(p) if(p != 0) {glDeleteTextures(1, &p); p = 0;}
#endif

#ifndef SAFE_DELETE_FBO
#define SAFE_DELETE_FBO(p) if(p != 0) {glDeleteFramebuffers(1, &p); p = 0;}
#endif

#ifndef SAFE_DELETE_PROGRAM
#define SAFE_DELETE_PROGRAM(p) if(p != 0) {glDeleteProgram(p); p = 0;}
#endif


#endif

