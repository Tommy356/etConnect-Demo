//
//  ETHelpers.h
//  eiLance XC4 SV1
//
//  Created by Thomas Krautter on 26.01.12.
//  Copyright (c) 2012 TK Consulting, Thomas Krautter. All rights reserved.
//

//#import <UIKit/UIKit.h>

@interface ETHelpers : NSObject

+ (BOOL) isIPhone4;
+ (BOOL) isIPadMini;
+ (NSString *) modelName;
+ (NSString *) machineName;
+ (NSString *) uniqueGlobalDeviceIdentifier;
+ (NSString *) deviceIPAddress; 

@end
