//
//  ESAppDelegate.m
//  etConnect-Demo
//
//  Created by Thomas Krautter on 30.11.13.
//  Copyright (c) 2013 eitap Technologies GmbH. All rights reserved.
//

#import "ESAppDelegate.h"

@implementation ESAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    /*
     *  At application launch, we'll start the ETConnect server with search & publish options and define
     *  the AppDelegate as a delegate of the ETConnectServer (see delegate routines below).
     */
    
    /* get the shared instance of the ETConnectServer ... */
    ETConnectServer* server = [ETConnectServer sharedInstance];
    
    /*  Note:
     *  If you intend to run this App from Simulator and the OSX version at the same time, there
     *  will be a name conflict, as ETConnect/Bonjour will use the same name to publish.
     *
     *  If this happens, you'll need to set a different publish name by uncommenting the code below:
     */
#if 0
    #define HOST_NAME_MAX 255
    char* buf = alloca(HOST_NAME_MAX);
    gethostname(buf,HOST_NAME_MAX);
    NSString* str = [NSString stringWithFormat:@"%@-iOS",[NSString stringWithUTF8String:buf]];
    server.publishName = str;
#endif
    /*
     *  Start the ETConnect server by publishing this device (note, that dis does NOT publish custom
     *  services yet) and starting to search for other devices...
     */
    [server startWithOptions:ETServerOptionPublish|ETServerOptionSearch];
    
    /** before we may publish any services or create remote connections,
     *  we need to await the 'didPublish' connect from the connection server (see below)
     */
    
    return YES;
}
							
- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}
@end
