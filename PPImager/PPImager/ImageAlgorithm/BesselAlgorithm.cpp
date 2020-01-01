//
//  BesselAlgorithm.cpp
//  PPImager
//
//  Created by 邱开禄 on 2019/12/17.
//  Copyright © 2019 邱开禄. All rights reserved.
//


#include "BesselAlgorithm.h"
#include <string.h>

#include <stdio.h>
#include <cmath>
NS_IMAGE_BEGIN

/*
 * s(t_) = Sqrt[A*t*t+B*t+C]
 */
double BesselAlgorithm::S(float t, float A, float B, float C)
{
    return sqrt((float)(A * t * t + B * t + C));
}


/*
 * 长度函数
 */
double BesselAlgorithm::L(float t, float A, float B, float C)
{
    float temp1 = sqrt(C + t * (B + A * t));
    float temp2 = (2 * A * t * temp1 + B * (temp1 - sqrt(C)));
    float temp3 = log(B + 2 * sqrt(A) * sqrt(C));
    float temp4 = log(B + 2 * A * t + 2 * sqrt(A) * temp1);
    float temp5 = 2 * sqrt(A) * temp2;
    float temp6 = (B * B - 4 * A * C) * (temp3 - temp4);
    return (temp5 + temp6) / (8 * pow(A, 1.5));
}

/*
 * X(n+1) = Xn - F(Xn)/F'(Xn) 长度函数反函数，使用牛顿切线法求解
 */
double BesselAlgorithm::InvertL(float t, float l, float A, float B, float C)
{
    float t1 = t, t2;
    do {
        t2 = t1 - (L(t1, A, B, C) - l) / S(t1, A, B, C);
        if(abs(t1 - t2) < 0.0001)
            break;
        t1 = t2;
    }while(true);
    return t2;
}

NS_IMAGE_END



