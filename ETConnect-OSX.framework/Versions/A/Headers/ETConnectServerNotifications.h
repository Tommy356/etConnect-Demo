//
//  ETConnectServerNotifications.h
//  ETConnect Framework
//
//  Created by Thomas Krautter on 14.11.13.
//  Copyright (c) 2013 eitap Technologies GmbH. All rights reserved.
//

#if TARGET_OS_IPHONE==1
#import <ETBase/ETNotificationManager.h>
#else
#import <ETBase-OSX/ETNotificationManager.h>
#endif

@interface ETConnectServerNotifications : ETNotifications

+ (void) send_ETConnectServerDidAddHost:(NSString*)name;
+ (void) send_ETConnectServerDidRemoveHost:(NSString*)name;
+ (void) send_ETConnectServerDidUpdateHost:(NSString*)name;

@end

/// --------------------------------------------------------------------
/// ETDemoNotificationsDelegate
/// --------------------------------------------------------------------
@protocol ETConnectServerNotificationsDelegate <NSObject>
@optional
/** the server has added a new host */
- (void) ETConnectServerDidAddHost:(NSNotification*)note;
/** the server has removed a host, which was added previously */
- (void) ETConnectServerDidRemoveHost:(NSNotification*)note;
/** the server has updated the services offered by a host */
- (void) ETConnectServerDidUpdateHost:(NSNotification*)note;
@end
