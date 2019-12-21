//
//  DrawView.h
//  ImageProcessing
//
//  Created by 邱开禄 on 2019/11/27.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#import <UIKit/UIKit.h>
@interface DrawView : UIView


//@property (nonatomic, assign) CGFloat lineWidth;
//
//// 线段的颜色
//@property (nonatomic, strong) UIColor *lineColor;

// 清屏
- (void)clear;

// 撤销
- (void)undo;

@end


