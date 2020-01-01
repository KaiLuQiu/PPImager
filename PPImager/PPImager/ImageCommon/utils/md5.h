//
//  md5.h
//  PPImager
//  计算md5工具类,参考:https://blog.csdn.net/fareast_mzh/article/details/99238268
//  Created by 邱开禄 on 2019/12/17.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#ifndef MD5_H
#define MD5_H
#include <stdio.h>
#include <vector>
#include "ImageCommon.h"
#include <iostream>

NS_IMAGE_BEGIN

 
// 右移的时候，高位一定要补零，而不是补充符号位
#define shift(x, n) (((x) << (n)) | ((x) >> (32-(n))))
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/**
 * MD5运算要用到一个128位的MD5缓存器,用来保存中间变量和最终结果.该缓存器又可看成是4个32位的寄存器A、B、C、D,初始化为:
 * A: 01 23 45 67
 * B：89 ab cd ef
 * C：fe dc ba 98
 * D：76 54 32 10
 */
#define A 0x67452301
#define B 0xefcdab89
#define C 0x98badcfe
#define D 0x10325476

class md5{
public:
    md5();
    
    ~md5();
    
    // 懒汉模式
    static md5* getInstance();
    
    std::string getMD5(std::string source);

private:
    void mainLoop(unsigned int M[]);
    
    unsigned int* add(std::string str);
    
    std::string changeHex(int a);
    
    //strBaye的长度
    unsigned long strlength;
    
    //A,B,C,D的临时变量
    unsigned int atemp;
    unsigned int btemp;
    unsigned int ctemp;
    unsigned int dtemp;
    
};
 
NS_IMAGE_END

#endif // MD5_H

