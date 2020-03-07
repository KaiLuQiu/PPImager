//
//  DrawView.m
//  PPImager
//
//  Created by 邱开禄 on 2019/11/27.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#import "DrawView.h"

@interface DrawView ()
    @property (nonatomic, strong) NSMutableArray *PointArr;
    @property(nonatomic)            EAGLContext     *context;
    @property(strong,nonatomic)     CAEAGLLayer     *eglLayer;
    @property(nonatomic)            GLuint          program;

@end


@implementation DrawView
{
    CGPoint point;
}

- (void) layoutSubviews {
    // 创建GL环境
    [self createGLLayer];
    [self createGLContext];
}


+(Class) layerClass {
    return [CAEAGLLayer class];
}

- (void) createGLLayer {
    self.eglLayer = (CAEAGLLayer *)self.layer;
    self.eglLayer.opaque = YES;
    [self.eglLayer setContentsScale:[[UIScreen mainScreen] scale]];
    self.eglLayer.drawableProperties = @{kEAGLDrawablePropertyRetainedBacking: [NSNumber numberWithBool:NO],
    kEAGLDrawablePropertyColorFormat:kEAGLColorFormatRGBA8};
};

- (void) createGLContext
{
    // 设置OpenGLES的版本为2.0 当然还可以选择1.0和最新的3.0的版本，以后我们会讲到2.0与3.0的差异，目前为了兼容性选择2.0的版本
    EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
    _context = [[EAGLContext alloc] initWithAPI:api];
    if (nil == _context) {
        NSLog(@"Failed to initialize OpenGLES 2.0 context");
        exit(1);
    }
    
    // 将当前上下文设置为我们创建的上下文
    if (![EAGLContext setCurrentContext:_context]) {
        NSLog(@"Failed to set current OpenGL context");
        exit(1);
    }
}

- (void) cancleGLContext
{
    [EAGLContext setCurrentContext:nil];
    _context = nil;
}

// 绘制东西
- (void)drawRect:(CGRect)rect
{

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

@end
