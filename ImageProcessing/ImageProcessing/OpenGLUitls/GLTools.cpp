//
//  GLTools.cpp
//  ImageProcessing
//
//  Created by 邱开禄 on 2019/12/17.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#include "GLTools.h"
NS_IMAGE_BEGIN

/*
 * 创建GLProgram from file
 */
GLuint GLTools::createGLProgramFromFile(const char *vertextPath, const char *fragPath)
{
    char vBuffer[2048] = {0};
    char fBuffer[2048] = {0};
    
    if (getFileContent(vBuffer, sizeof(vBuffer), vertextPath) < 0) {
        return 0;
    }
    
    if (getFileContent(fBuffer, sizeof(fBuffer), fragPath) < 0) {
        return 0;
    }
    
    return createGLProgram(vBuffer, fBuffer);
}

/*
 * 创建GLProgram
 */
GLuint GLTools::createGLProgram(const char *VertexText, const char *FragmentText)
{
    GLuint program = glCreateProgram();
    
    GLuint vertexShader = createGLShader(VertexText, GL_VERTEX_SHADER);
    GLuint fragShader = createGLShader(FragmentText, GL_FRAGMENT_SHADER);
    
    if (vertexShader == 0 || fragShader == 0) {
        printf("GLTools: createGLShader error!!!\n");
        return 0;
    }
    
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragShader);
    
    glLinkProgram(program);
    
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint infoLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            GLchar *infoText = (GLchar *)malloc(sizeof(GLchar) * infoLen + 1);
            if (infoText) {
                memset(infoText, 0, sizeof(GLchar) * infoLen + 1);
                glGetProgramInfoLog(program, infoLen, NULL, infoText);
                printf("Error link program : %s\n", infoText);
                free(infoText);
            }
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragShader);
        glDeleteProgram(program);
        return 0;
    }
    
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    return program;
}

/*
 * 加载一个空的纹理
 */
GLuint GLTools::loadTexture(int width, int height, GLenum format)
{
    return loadTexture(NULL, width, height, format);
}

/*
 * 传入data，创建一份纹理
 */
GLuint GLTools::loadTexture(unsigned char *data, int width, int height, GLenum format)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

/*
 * 创建GLshader
 */
GLuint GLTools::createGLShader(const char *shaderText, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderText, NULL);
    glCompileShader(shader);
    
    int complited = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &complited);
    if (!complited) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char *infoLog = (char *)malloc(sizeof(char) * infoLen);
            if (infoLog) {
                glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
                printf("Error compliting shader: %s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

/*
* 从file中去读字节
*/
long GLTools::getFileContent(char *buffer, long len, const char *filePath)
{
    FILE *file = fopen(filePath, "rb");
    if (file == NULL) {
        return -1;
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    
    if (len < size) {
        printf("file is large than the size(%ld) you give\n", len);
        return -1;
    }
    
    fread(buffer, 1, size, file);
    buffer[size] = '\0';
    
    fclose(file);
    
    return size;
}

NS_IMAGE_END



