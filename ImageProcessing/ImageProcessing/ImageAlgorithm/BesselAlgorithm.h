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

class BesselAlgorithm {
public:
    double InvertL(float t, float l, float A, float B, float C);
private:
    double S(float t, float A, float B, float C);
    double L(float t, float A, float B, float C);
}


#endif // BesselAlgorithm_H
