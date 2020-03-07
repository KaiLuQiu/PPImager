//
//  CircularQueue.cpp
//  PPImager
//
//  Created by 邱开禄 on 2019/12/17.
//  Copyright © 2019 邱开禄. All rights reserved.
//


#include "CircularQueue.h"
#include <string.h>

#include <stdio.h>
#include <cmath>
NS_IMAGE_BEGIN

CircularQueue::CircularQueue()
{
    queueSize = 0;
    front = 0;
    rear = 0;
    node = NULL;
}

CircularQueue::~CircularQueue()
{
    SAFE_DELETE_ARRAY(node);
}

void CircularQueue::setQueueSize(int size)
{
    node = new int[size];
    for(int i = 0; i < size; i++) {
        node[i] = i;
    }
    queueSize = size;
}

void CircularQueue::next()
{
    rear = (rear + 1) % queueSize;
    if (rear == front) {
        front = (front + 1) % queueSize;
    }
}

void CircularQueue::back()
{
    rear = (rear - 1 + queueSize) % queueSize;
}

int CircularQueue::getCurNode()
{
    return node[rear];
}

int CircularQueue::getLastNode()
{
    return node[(rear - 1 + queueSize) % queueSize];
}

NS_IMAGE_END



