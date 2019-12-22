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
    
    /*
     * 创建GLProgram
     */
    static GLuint createGLProgram(const char *VertexText, const char *FragmentText);
    
    /*
     * 加载一个空的纹理
     */
    static GLuint loadTexture(int width, int height, GLenum format = GL_RGBA);
    
    /*
     * 传入data，创建一份纹理
     */
    static GLuint loadTexture(unsigned char *data, int width, int height, GLenum format = GL_RGBA);
private:
    
    /*
    * 创建GLshader
    */
    static GLuint createGLShader(const char *shaderText, GLenum shaderType);
};

NS_IMAGE_END

#endif // GLTools_H
