//
//  ETNotificationManager.h
//  ETBase Framework
//
//  Created by Thomas Krautter on 14.01.12.
//  Copyright (c) 2012 TK Consulting, Thomas Krautter. All rights reserved.
//

#import <Foundation/Foundation.h>

/*
 *
 */
typedef struct __etNotificationStruct
{
    NSString*   name;
} ETNOTIFICATIONSTRUCT;

/** ETNotificationCenter is a shared instance class, which provides the base services to
 *  register client objects to specific notification services.
 */

@interface ETNotificationCenter : NSNotificationCenter
/** returns the instance of the default ETNotification Center */
+ (ETNotificationCenter*) defaultCenter;
    
/** registers an notification oberserver for a group of (optional) notifications */
+ (BOOL) addNotificationObserver:(id)observer withNotifications:(NSArray*)notifications;
    
@end

/// --------------------------------------------------------------------
/// ETNotifications
/// --------------------------------------------------------------------

/** This class is internally used by CloudAgent and CloudAgent classes to
 send notifications to related modules and applications.
 Please refer to the x3NotificationDelegate protocol to check, what notifications
 are available for client Apps.
 @note You don't need to create, subclass or implement anything around this class,
 because it's mostly used internally.
 */
@interface ETNotifications : NSObject

#pragma mark - CLASS METHODS
+ (BOOL) addNotificationObserver:(id)observer;
+ (BOOL) removeNotificationObserver:(id)observer;

+ (void) gaPost:(NSString*)strNote withObject:(id)obj;
+ (void) gaPostWait:(NSString*)strNote withObject:(id)obj;
+ (NSArray*) notifications;
@end

