//
//  IOSHelperClasses.h
//  EIConnectionIOS
//
//  Created by Thomas Krautter on 06.10.13.
//  Copyright (c) 2013 Thomas Krautter. All rights reserved.
//

#import <Foundation/Foundation.h>

extern NSRecursiveLock *gnustep_global_lock;

@interface	GSLazyRecursiveLock : NSRecursiveLock
{
    int	counter;
}
- (void) _becomeThreaded: (NSNotification*)n;
@end


@interface	GSLazyLock : NSLock
{
    int	locked;
}
- (void) _becomeThreaded: (NSNotification*)n;
@end
