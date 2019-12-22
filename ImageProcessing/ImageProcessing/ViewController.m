//
//  ViewController.m
//  ImageProcessing
//
//  Created by 邱开禄 on 2019/11/27.
//  Copyright © 2019 邱开禄. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self InitButton];
    // Do any additional setup after loading the view.
}


#pragma mark 初始化按钮控件
- (void) InitButton {
 
    _pPenButton = [UIButton buttonWithType:UIButtonTypeSystem];
    _pPenButton.frame = CGRectMake(SCREENWIDTH_D40 * 30, SCREENHEIGHT_D40 * 26, SCREENWIDTH_D40 * 10, SCREENHEIGHT_D40 * 2);
    [_pPenButton setTitle:@"绘画笔" forState:UIControlStateNormal];
    [_pPenButton setTitleColor:[UIColor colorWithRed:1 green:1 blue:1 alpha:1] forState:(UIControlState)UIControlStateNormal];
    _pPenButton.contentMode = UIViewContentModeCenter;
    _pPenButton.backgroundColor = [UIColor colorWithRed:0 green:1 blue:0 alpha:1];
    [_pPenButton addTarget:self action:@selector(clickPenButton:) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:_pPenButton];

}

- (void)clickPenButton:(id)sender {
    if(self.pPenButton == nil) {
        return;
    }
    ViewController *penController = [self.storyboard instantiateViewControllerWithIdentifier:@"DrawingPen"];
    [self presentViewController:penController animated:YES completion:nil];
    
}

@end
