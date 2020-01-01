//
//  DrawingPenController.m
//  PPImager
//
//  Created by 邱开禄 on 2019/11/27.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#import "DrawingPenController.h"
#import "DrawView.h"
#include "GLProgramManager.h"
#include "GLTools.h"
#include "antrace.h"
#import "ImageHelper.h"


@interface DrawingPenController ()
    @property(nonatomic, assign)    DrawView        *penView;
    @property(nonatomic)            EAGLContext     *context;
    @property(nonatomic)            GLuint          program;

@end

@implementation DrawingPenController

- (void) viewDidLoad {
    [super viewDidLoad];
    // 创建GL环境
    [self createGLContext];
    
    // Do any additional setup after loading the view.
    _penView = (DrawView *)self.view;
}

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

- (UIImage *) potraceAndSvgEffect:(UIImage *)image
{
    unsigned char *srcData = [ImageHelper convertUIImageToBitmapRGBA8:image];
    int srcWidth = image.size.width;
    int srcHeight = image.size.height;

    // 文件1，使用c标准库进行创建
    const char *filePath = [[[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0] stringByAppendingPathComponent:@"Potrace_file.data"] cStringUsingEncoding:NSUTF8StringEncoding];

    antrace *potraceObject = new antrace();
    potraceObject->traceImage(srcData, srcWidth, srcHeight, filePath);
    int dstWidth;
    int dstHeight;
    unsigned char* dstData = potraceObject->readBufferFromFile(filePath, dstWidth, dstHeight);
    UIImage * resultImage = [ImageHelper convertBitmapRGBA8ToUIImage:dstData withWidth:dstWidth withHeight:dstHeight];
    SAFE_DELETE(potraceObject);
    return resultImage;
}
@end
