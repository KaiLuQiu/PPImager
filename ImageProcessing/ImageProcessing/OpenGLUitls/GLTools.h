//
//  GLTools.h
//  ImageProcessing
//  GL工具类
//  Created by 邱开禄 on 2019/12/17.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#ifndef GLTools_H
#define GLTools_H
#include <stdio.h>
#include <vector>
#include "ImageCommon.h"


NS_IMAGE_BEGIN
class GLTools {
public:
    static GLuint createGLProgram(const char *VertexString, const char *FragmentString);
private:

};

NS_IMAGE_END

#endif // GLTools_H
