//
//  GLProgramManager.cpp
//  ImageProcessing
//  Programg的管理类
//  Created by 邱开禄 on 2019/12/17.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#include "GLProgramManager.h"
#include "md5.h"

NS_IMAGE_BEGIN
// 类的静态指针需要在此初始化
std::mutex Mutex;
GLProgramManager* GLProgramManager::p_GLprogram = nullptr;

GLProgramManager::GLProgramManager()
{
    p_GLprogramsMap.clear();
}

GLProgramManager::~GLProgramManager()
{
    clearAllGLProgramFromMap();
}

/*
 * 通过key值，在对应的map中找到对应的program值，如果不存在则创建一个新的
 */
GLuint GLProgramManager::createGLProgram(const std::string VertexText, const std::string FragmentText)
{
    std::string md5Key = VertexText + FragmentText;
    std::string key = md5::getInstance()->getMD5(md5Key);
    auto it = p_GLprogramsMap.find(key);
    int curProgram = 0;
    if (it != p_GLprogramsMap.end()) {
        // second对应program的值
        curProgram = it->second;
    }
    // 如果curProgram = 0说明当前的map中不存在，则根据vertex和Fragment创建一个新的program
    if (curProgram == 0) {
        curProgram = GLTools::createGLProgram(VertexText.c_str(), FragmentText.c_str());
        p_GLprogramsMap.insert(std::pair<std::string, int>(md5Key, curProgram));
    }
    return curProgram;
}

/*
 * 通过key值，在对应的map中找到对应的program值，如果不存在则创建一个新的
 */
GLuint GLProgramManager::createGLProgramByPath(const std::string VertexPath, const std::string FragmentPath)
{
    
    int vBufferLen = sizeof(char) * 2048;
    char *vBuffer = (char *)malloc(vBufferLen);
    if (NULL == vBuffer) {
        printf("GLProgramManager: vBuffer malloc fail!!!\n");
        return -1;
    }
    int fBufferLen = sizeof(char) * 2048;
    char *fBuffer = (char *)malloc(fBufferLen);
    if (NULL == fBuffer) {
        printf("GLProgramManager: fBuffer malloc fail!!!\n");
        return -1;
    }
    image::GLTools::getFileContent(vBuffer, vBufferLen, VertexPath.c_str());
    image::GLTools::getFileContent(fBuffer, fBufferLen, FragmentPath.c_str());
    
    std::string vertText = vBuffer;
    std::string fragText = fBuffer;
    
    std::string md5Key = vertText + fragText;
    std::string key = md5::getInstance()->getMD5(md5Key);
    auto it = p_GLprogramsMap.find(key);
    int curProgram = 0;
    if (it != p_GLprogramsMap.end()) {
        // second对应program的值
        curProgram = it->second;
    }
    // 如果curProgram = 0说明当前的map中不存在，则根据vertex和Fragment创建一个新的program
    if (curProgram == 0) {
        curProgram = GLTools::createGLProgram(vertText.c_str(), fragText.c_str());
        p_GLprogramsMap.insert(std::pair<std::string, int>(key, curProgram));
    }
    SAFE_FREE(vBuffer);
    SAFE_FREE(fBuffer);
    return curProgram;
}

/*
 * 清除所有的map值
 */
void GLProgramManager::clearAllGLProgramFromMap()
{
    auto it = p_GLprogramsMap.begin();
    for(; it != p_GLprogramsMap.end();)
    {
        SAFE_DELETE_PROGRAM(it->second);
        // 这边先把it传递给item_e,然后把it迭代器指向下一个，否则直接erase(it)会有问题的
        auto item_e = it++;
        p_GLprogramsMap.erase(item_e);
    }
}

NS_IMAGE_END



