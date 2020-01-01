//
//  KmeansCore.cpp
//  PPImager
//
//  Created by 邱开禄 on 2019/12/17.
//  Copyright © 2019 邱开禄. All rights reserved.
//


#include "Kmeans.h"
#include "math.h"
#include "string.h"
#include "stdlib.h"
NS_IMAGE_BEGIN

/*
 * 计算两点直接的欧拉距离
 */
double KmeansCore::getDistance4RGB(ColorElement x1, ColorElement x2) {
    return sqrt((float)(pow((x1.red - x2.red), 2) + pow((x1.green - x2.green), 2) + pow((x1.blue - x2.blue), 2)));
}

/*
 * 计算当前ColorElement所属族
 */
int KmeansCore::clusterofElementRGB(ColorElement means[], ColorElement element) {
    float dist = getDistance4RGB(means[0], element);
    int label = 0;
    float tempDist;
    for (int i = 1; i < CLUSTER_NUM; i++) {
        tempDist = getDistance4RGB(means[i], element);
        if (tempDist < dist) {
            dist = tempDist;
            label = i;
        }
    }
    return label;
}

/*
 * 计算所有族的平方误差
 */
float KmeansCore::getAverage(std::vector<ColorElement> clusters[], ColorElement *means) {
    float avg = 0;
    for (int i = 0; i < CLUSTER_NUM; i++) {
        std::vector<ColorElement> k = clusters[i];
        for (int j = 0; j < k.size(); j ++) {
            avg += getDistance4RGB(k[j], means[i]);
        }
    }
    return avg;
}

/*
 * 获得当前族的质心点
 */
ColorElement KmeansCore::getMeans(std::vector<ColorElement> cluster){
    int num = cluster.size();
    double avgRed = 0;
    double avgGreen = 0;
    double avgBlue = 0;
    ColorElement element;
    for (int i = 0; i < num; i++) {
        avgRed += cluster[i].red;
        avgGreen += cluster[i].green;
        avgBlue += cluster[i].blue;

    }
    element.red = avgRed / num;
    element.green = avgGreen / num;
    element.blue = avgBlue / num;
    return element;
}

/*
 * 获取初始化的质心点
 */
void KmeansCore::chooseSeeds()
{

    int number = (int)Node.size();
    int cnt = 0;
    srand((unsigned int) time(NULL));
    int index = rand() % number;
    Means[cnt++] = Node[index];
    
    double* dis = (double*)malloc(number * sizeof(double));
    int count = 0;

    memset(dis,0x3f,sizeof(dis));
    while(cnt < CLUSTER_NUM)//求出每个点与距离它最近的均值中心的距离
    {
        double sum=0;
        for(int i=0;i<number;i++)
        {
            for(int j=0;j<cnt;j++)
            {
                if(i==j) continue;
                dis[i] = fmin(dis[i],getDistance4RGB(Node[i], Means[j]));
            }
            sum+=dis[i];
        }
        for(int i=0;i<number;i++)//归一化，其后可以对应到概率
        {
            dis[i]/=sum;
        }
        double *cumprobs = (double *)malloc(number * sizeof(double));
        cumprobs[0] = dis[0];
        for(int i=1;i<number;i++)//求出概率的前缀和
        {
            cumprobs[i]=dis[i]+cumprobs[i-1];
        }
        bool *used = (bool *)malloc(number * sizeof(bool));
        memset(used, true, number * sizeof(used));
        used[index]=false;
    next:
        double r= (rand()%1000)*0.001;
        bool flg=true;
        for(int i = 0; i < number; i++)
        {
            if((r < cumprobs[i] && used[i]) || count >= 1)//选择满足概率的点作为簇中心
            {
                index=i;
                flg=false;
                used[i]=false;
                count = 0;
                break;
            }
        }
        if(flg) {
            count++;
            goto next; //如果没有找到，重新产生随机数r继续找
        }
        Means[cnt++] = Node[index];
        SAFE_FREE(cumprobs);
        SAFE_FREE(used);
    }
    SAFE_FREE(dis);
}

/*
 * kmeans聚类的核心
 */
void KmeansCore::Core(std::vector<ColorElement> elements)
{
    // 选择初始聚类中心
    chooseSeeds();
    int lable=0;
    // 将元组根据初始聚类中心分类,lable表示均值中心标号
    for(int i=0 ; i != elements.size() ;i++)
    {
        lable= clusterofElementRGB(Means, elements[i]);
        clusters[lable].push_back(Node[i]);
    }
    
    double oldAvg = -1;
    double newAvg = getAverage(clusters, Means);
    // 当两次迭代误差相差不到1时迭代终止
    while(fabs(newAvg - oldAvg) >= 1e-9)
    {
        // 更新每个簇的均值中心
        for(int i = 0;i < CLUSTER_NUM; i++)
        {
            Means[i] = getMeans(clusters[i]);
        }
        oldAvg = newAvg;
        // 计算新的误差
        newAvg = getAverage(clusters, Means);
        // 清空每个簇，因为之后要重新分配
        for(int i = 0;i < CLUSTER_NUM; i++)
        {
            clusters[i].clear();
        }
        // 根据新的均值中心获得新的簇
        for(int i = 0;i != elements.size(); i++)
        {
            lable = clusterofElementRGB(Means, elements[i]);
            clusters[lable].push_back(elements[i]);
        }
    }
}

NS_IMAGE_END

