//
//  DrawingPenController.m
//  ImageProcessing
//
//  Created by 邱开禄 on 2019/11/27.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#import "DrawingPenController.h"
#import "DrawView.h"

@interface DrawingPenController ()
@property (strong, nonatomic)   DrawView *penView;
@end

@implementation DrawingPenController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // Do any additional setup after loading the view.
    _penView = (DrawView *)self.view;

}


@end
