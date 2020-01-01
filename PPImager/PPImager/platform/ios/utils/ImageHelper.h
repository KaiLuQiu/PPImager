//
//  ImageHepler.h
//  PPImager
//
//  Created by 邱开禄 on 2019/11/27.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface ImageHelper : NSObject {
    
}

/*
 * UIImage 转 unsigned char *
 */
+ (unsigned char *) convertUIImageToBitmapRGBA8:(UIImage *)image;

/*
 *
 */
+ (CGContextRef) newBitmapRGBA8ContextFromImage:(CGImageRef)image;


/*
 * unsigned char * 转UIimage
 */
+ (UIImage *) convertBitmapRGBA8ToUIImage:(unsigned char *)buffer
                                withWidth:(int)width
                               withHeight:(int)height;

@end

