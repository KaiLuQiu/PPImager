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

@end

@implementation DrawingPenController

- (void) viewDidLoad {
    [super viewDidLoad];    
    // Do any additional setup after loading the view.
    _penView = (DrawView *)self.view;
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
