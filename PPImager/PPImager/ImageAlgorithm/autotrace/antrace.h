//
//  antrace.h
//  PPImager
//  potrace, 参考:https://github.com/jiangpeng79/antrace
//  参考文献:https://blog.csdn.net/linxinboy/article/details/19020917
//  Created by 邱开禄 on 2019/12/25.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#ifndef ANTRACE_H
#define ANTRACE_H
#include "antraceInfo.h"


class antrace {
public:
    antrace();
    ~antrace();
   /*
    * 进行potrace（路径追踪）算法的解析
    * 1.输入黑白图，根据对应的算法记录一个闭环路径
    * 2.寻找最有多边形
    * 3.优化处理多边形
    * 4.通过贝塞尔曲线优化曲线轮廓
    * 5.将数据写入对应的路径文件中
    */
    bool traceImage(unsigned char *data, int width, int height, const char * path);
    /*
     * 从对应的文件中读取数据
     */
    unsigned char* readBufferFromFile(const char* path, int& width, int& height);
private:
    void calc_dimensions(imginfo_t *imginfo, potrace_path_t *plist);
    void initInfo(char const* filetype);
    bool saveToFile(const char* path, int width, int height, const char* filetype);
    int backend_lookup(char *name, backend_t **bp);

    double double_of_dim(dim_t d, double def) {
        if (d.d) {
            return d.x * d.d;
        } else {
            return d.x * def;
        }
    }
    potrace_state_t* s_state;
};


#endif


