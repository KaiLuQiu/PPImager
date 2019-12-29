//
//  BesselAlgorithm.h
//  ImageProcessing
//  匀速贝塞尔曲线, 参考https://blog.csdn.net/linuxheik/article/details/79454663

//  Created by 邱开禄 on 2019/12/17.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#ifndef BesselAlgorithm_H
#define BesselAlgorithm_H
#include <stdio.h>
#include <vector>
#include "ImageCommon.h"

NS_IMAGE_BEGIN
class BesselAlgorithm {
public:
    /*
     * X(n+1) = Xn - F(Xn)/F'(Xn) 长度函数反函数，使用牛顿切线法求解
     */
    double InvertL(float t, float l, float A, float B, float C);
private:
   /*
    * s(t_) = Sqrt[A*t*t+B*t+C]
    */
    double S(float t, float A, float B, float C);
   /*
    * 长度函数
    */
    double L(float t, float A, float B, float C);
};

NS_IMAGE_END

#endif // BesselAlgorithm_H
