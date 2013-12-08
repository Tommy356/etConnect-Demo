//
//  ESRemoteObject.h
//  etConnect-Demo
//
//  Created by Thomas Krautter on 03.12.13.
//  Copyright (c) 2013 eitap Technologies GmbH. All rights reserved.
//

#import <Foundation/Foundation.h>

#if TARGET_OS_IPHONE==1
#import <ETConnect/ETConnect.h>
#else
#import <ETConnect-OSX/ETConnect.h>
#endif

#define DEMO_SERVICE_NAME   @"ESRemoteObject"

/** To speed up the network transport, it's a good practice to define
 *  a proxy protocol. This is not required for simple calls, however, when
 *  dealing with structures, EIConnection requires a protocol to map values
 *  correctly from 32-bit to 64-bit platforms.
 *
 *  The following protocol simply defines all functions to be called remotely:
 */

@protocol ESRemoteObjectProtocol <NSObject>
- (void) sayHello:(NSString*)strMsg;
- (out NSString*) ping:(NSString*)hostName;
- (out NSString*) pong:(NSString*)hostName;
@end

@interface ESRemoteObject : NSObject
#if TARGET_OS_IPHONE==1
<ESRemoteObjectProtocol,UIAlertViewDelegate>
{
    NSTimer* timer;
}
@property (nonatomic, retain) UIAlertView* alert;
#else
<ESRemoteObjectProtocol>
{
    NSTimer* timer;
}
/* required to show alerts, must be set by owner */
@property (nonatomic, retain) NSWindow* window;
@property (nonatomic, retain) NSAlert* alert;
#endif
@end
