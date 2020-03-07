//
//  CircularQueue.h
//  PPImager
//  Created by 邱开禄 on 2020/03/07.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#ifndef CircularQueue_H
#define CircularQueue_H
#include <stdio.h>
#include <vector>
#include "ImageCommon.h"

NS_IMAGE_BEGIN
class CircularQueue {
public:
    CircularQueue();
    ~CircularQueue();
    
    void setQueueSize(int size);
    
    void next();
    
    void back();
    
    int getCurNode();
    
    int getLastNode();
private:
    int         queueSize;
    int         front;
    int         rear;
    int*        node;

};

NS_IMAGE_END

#endif // CircularQueue_H
