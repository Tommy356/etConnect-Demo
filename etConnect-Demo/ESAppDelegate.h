//
//  ESAppDelegate.h
//  etConnect-Demo
//
//  Created by Thomas Krautter on 30.11.13.
//  Copyright (c) 2013 eitap Technologies GmbH. All rights reserved.
//

#import <UIKit/UIKit.h>

/* import ETCopnnect.h for all declarations required to use the framework */
#import <ETConnect/ETConnect.h>

@interface ESAppDelegate : UIResponder
<UIApplicationDelegate, EIConnectionServerDelegate>

@property (strong, nonatomic) UIWindow *window;

@end
