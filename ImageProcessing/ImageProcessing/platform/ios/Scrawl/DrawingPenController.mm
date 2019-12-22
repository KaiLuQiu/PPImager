//
//  DrawingPenController.m
//  ImageProcessing
//
//  Created by 邱开禄 on 2019/11/27.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#import "DrawingPenController.h"
#import "DrawView.h"
#include "GLProgramManager.h"
#include "GLTools.h"

@interface DrawingPenController ()
    @property(nonatomic, assign)    DrawView        *penView;
    @property(nonatomic)            EAGLContext     *context;
    @property(nonatomic)            GLuint          program;

@end

@implementation DrawingPenController

- (void)viewDidLoad {
    [super viewDidLoad];
    // 创建GL环境
    [self createGLContext];
    
    // Do any additional setup after loading the view.
    _penView = (DrawView *)self.view;

    NSString *vertFile = [[NSBundle mainBundle] pathForResource:@"vert.glsl" ofType:nil];
    NSString *fragFile = [[NSBundle mainBundle] pathForResource:@"frag.glsl" ofType:nil];

    GLuint program = image::GLProgramManager::getIntanse()->createGLProgramByPath([vertFile UTF8String], [fragFile UTF8String]);
    GLuint program2 = image::GLProgramManager::getIntanse()->createGLProgramByPath([vertFile UTF8String], [fragFile UTF8String]);
    GLuint program3 = image::GLProgramManager::getIntanse()->createGLProgramByPath([vertFile UTF8String], [fragFile UTF8String]);

}

- (void)createGLContext
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

- (void)cancleGLContext
{
    [EAGLContext setCurrentContext:nil];
    _context = nil;
}

@end
