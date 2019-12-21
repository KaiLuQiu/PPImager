//
//  DrawView.m
//  ImageProcessing
//
//  Created by 邱开禄 on 2019/11/27.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#import "DrawView.h"

@interface DrawView ()
    @property (nonatomic, strong) NSMutableArray *PointArr;

@end


@implementation DrawView
{
    CGPoint point;
}

// 撤销
- (void)undo {
}

- (void)clearAll {
}

- (void)clearBezierPoint {
    [self.PointArr removeAllObjects];
}

- (void)CalcuBezierPoint {
    
}

- (NSMutableArray *)PointArr {
    if (_PointArr == nil) {
        _PointArr = [NSMutableArray array];
    }
    return _PointArr;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    // 清除当前的贝塞尔点
    [self clearBezierPoint];
    UITouch *touch = [touches anyObject];
    point = [touch locationInView:self];
    [self.PointArr addObject:[NSValue valueWithCGPoint:point]];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    UITouch *touch = [touches anyObject];
    point = [touch locationInView:self];
    [self.PointArr addObject:[NSValue valueWithCGPoint:point]];
    
    [self CalcuBezierPoint];
}

-(void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    UITouch *touch = [touches anyObject];
    point = [touch locationInView:self];
    [self.PointArr addObject:[NSValue valueWithCGPoint:point]];
}

// 绘制东西
- (void)drawRect:(CGRect)rect
{

}



@end
