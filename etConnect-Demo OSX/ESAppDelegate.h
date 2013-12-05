//
//  ESAppDelegate.h
//  etConnect-Demo OSX
//
//  Created by Thomas Krautter on 05.12.13.
//  Copyright (c) 2013 eitap Technologies GmbH. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <ETConnect-OSX/ETConnect.h>

@interface ESAppDelegate : NSObject 
<NSApplicationDelegate, EIConnectionServerDelegate>

@property (assign) IBOutlet NSWindow *window;

@end
