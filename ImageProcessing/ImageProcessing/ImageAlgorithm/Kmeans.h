//
//  KmeansCore.h
//  ImageProcessing
//
//  Created by 邱开禄 on 2019/12/17.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#ifndef KMEANS_H
#define KMEANS_H

#include <stdio.h>
#include <vector>
#include "ImageCommon.h"

NS_IMAGE_BEGIN

// Kmeans聚类中的族数量
#define CLUSTER_NUM 10

// 聚类算法中每一个数据成员的结构信息
typedef struct {
    int red;
    int green;
    int blue;
    int alpha;
}ColorElement;

class KmeansCore {
public:
    /*
     * kmeans 聚类算法的核心
     */
    void Core(std::vector<ColorElement> elements);
private:
    /*
     * 节点信息
     */
    std::vector<ColorElement> Node;
    
    /*
     * 聚类中每一个族的质心
     */
    ColorElement Means[CLUSTER_NUM];
    
    /*
     * 当前族的所有成员
     */
    std::vector<ColorElement> clusters[CLUSTER_NUM];
    
    /*
     * 计算两点直接的欧拉距离
     */
    double getDistance4RGB(ColorElement x1, ColorElement x2);
    
    /*
     * 计算当前ColorElement所属族
     */
    int clusterofElementRGB(ColorElement means[], ColorElement elements);
    
    /*
     * 计算所有族的平方误差
     */
    float getAverage(std::vector<ColorElement> clusters[], ColorElement means[]);
    
    /*
     * 获得质心点
     */
    ColorElement getMeans(std::vector<ColorElement> cluster);
    
    /*
     * 获取随机初始质心
     */
    void chooseSeeds();
    
    /*
     * 获取固定初始质心
     */
    void chooseFixedSeeds(unsigned char *data);
};

NS_IMAGE_END
#endif // KMEANS_H

