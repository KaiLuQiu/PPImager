//
//  GLProgramManager.h
//  ImageProcessing
//  Programg的管理类
//  Created by 邱开禄 on 2019/12/17.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#ifndef GLProgramManager_H
#define GLProgramManager_H
#include <stdio.h>
#include "ImageCommon.h"
#include "GLTools.h"
#include <mutex>
#include <map>
#include <string>

NS_IMAGE_BEGIN
class GLProgramManager {
public:
    /*
     * GLProgramManager单例模式：饿汉模式
     */
    static GLProgramManager* getIntanse() {
        std::mutex Mutex;
        if(NULL == p_GLprogram) {
            Mutex.lock();
            if(NULL == p_GLprogram) {
                p_GLprogram = new (std::nothrow)GLProgramManager();
                if(p_GLprogram == NULL) {
                    printf("GLProgramManager getInstance is NULL! \n");
                }
            }
            Mutex.unlock();
        }
        return p_GLprogram;
    }
    
    /*
     * 创建GLProgram通过脚本内容(通过key值，在对应的map中找到对应的program值，如果不存在则创建一个新的)
     */
    GLuint createGLProgram(const std::string VertexText, const std::string FragmentText);
    
    /*
     * 创建GLProgram通过脚本路径(通过key值，在对应的map中找到对应的program值，如果不存在则创建一个新的)
     */
    GLuint createGLProgramByPath(const std::string VertexPath, const std::string FragmentPath);
    
    /*
     * 清除所有的map值
     */
    void clearAllGLProgramFromMap();
    ~GLProgramManager();

private:
    GLProgramManager();
    std::map<std::string, int> p_GLprogramsMap;
    static GLProgramManager* p_GLprogram;
};

NS_IMAGE_END

#endif // GLProgramManager_H
