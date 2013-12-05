//
//  ESAppDelegate.m
//  etConnect-Demo OSX
//
//  Created by Thomas Krautter on 05.12.13.
//  Copyright (c) 2013 eitap Technologies GmbH. All rights reserved.
//

#import "ESAppDelegate.h"

#define HOST_NAME_MAX 255

@implementation ESAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    /** establish a connection to the default ETConnectServer
     *  to receive notifications about new hosts ...
     */
    ETConnectServer* srvr = [ETConnectServer sharedInstance];
    [srvr setDelegate:self];
    
    /** change the service name to differentiate between native and iOS apps ... 
     *  (if you're running the iOS version in simulator, Bonjour, by default picks the
     *   same name for both instances, which leads to conflicts. Adding -OSX to the
     *   published name allows to run in Simulator and native OSX simultaneously).
     */
    char* buf = alloca(HOST_NAME_MAX);
    gethostname(buf,HOST_NAME_MAX);
    NSString* str = [NSString stringWithFormat:@"%@-OSX",[NSString stringWithUTF8String:buf]];
    
    srvr.publishName = str;
    
    [srvr startWithOptions:ETServerOptionPublish|ETServerOptionSearch];
}

@end
