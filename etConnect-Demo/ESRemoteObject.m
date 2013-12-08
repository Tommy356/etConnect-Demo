//
//  ESRemoteObject.m
//  etConnect-Demo
//
//  Created by Thomas Krautter on 03.12.13.
//  Copyright (c) 2013 eitap Technologies GmbH. All rights reserved.
//
//  This is a basic class designed for iOS and OSX, providing some test routines
//  and serving as an example for basic connections using the etConnect framework.
//
//  Please follow the comments in the sources.

#import "ESRemoteObject.h"

@implementation ESRemoteObject

#pragma mark - Remote Methods defined in ESRemoteObjectProtocol

/*
 *  This is a very basic example for a remote call. This method will be called
 *  remotely, if the 'TaptoConnect' button is pressed on a remote device.
 *
 *  It will just display an alert (and set up a timer to cancel it, in case nobody
 *  presses a button).
 */
- (void) sayHelloMT:(NSString*)strMsg
{
#if TARGET_OS_IPHONE==1
    /* if we're currently displaying an alert, cancel it ... */
    if ( self.alert ){
        [timer invalidate];
        timer = nil;
        [self.alert dismissWithClickedButtonIndex:0 animated:YES];
    }
    /** Show the alert ... */
    self.alert = [[UIAlertView alloc] initWithTitle:@"etConnect-Demo"
                                            message:strMsg 
                                           delegate:self
                                  cancelButtonTitle:@"OK"
                                  otherButtonTitles:nil, nil];
    
    [self.alert show];
    timer = [NSTimer scheduledTimerWithTimeInterval:3.0 target:self selector:@selector(timerProc:) userInfo:nil repeats:NO];
#else
    /** Show the alert ... */
    NSAlert *alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:@"OK"];
    [alert addButtonWithTitle:@"Cancel"];
    [alert setMessageText:strMsg];
    [alert setInformativeText:@"etConnect-Demo"];
    [alert setAlertStyle:NSWarningAlertStyle];
    [alert beginSheetModalForWindow:[self window]
                      modalDelegate:self
                     didEndSelector:@selector(alertDidEnd:returnCode:contextInfo:)
                        contextInfo:nil];
    timer = [NSTimer scheduledTimerWithTimeInterval:3.0 target:self selector:@selector(timerProc:) userInfo:nil repeats:NO];
    
    self.alert = alert;
    [alert release];
#endif
}

- (void) sayHello:(NSString*)strMsg
{
    [self performSelectorOnMainThread:@selector(sayHelloMT:) withObject:strMsg waitUntilDone:NO];
}

/*
 *  Time & Alert close routines for both, iOS and OSX
 *
 *  The alert will close automatically after a couple of seconds...
 */
#if TARGET_OS_IPHONE==1
- (void)alertView:(UIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex
{
    self.alert = nil;
    if ( timer ) {
        [timer invalidate];
        timer = nil;
    }
}

- (void) timerProc:(NSTimer*)t
{
    if ( self.alert ) {
        [timer invalidate];
        timer = nil;
    }
    [self.alert dismissWithClickedButtonIndex:0 animated:YES];
}
#else
- (void) alertDidEnd:(NSAlert*)alert returnCode:(NSInteger)returnCode contextInfo:(void*)contextInfo
{
    NSLog(@"Alert did end!");
    self.alert = nil;
    if ( timer ) {
        [timer invalidate];
        timer = nil;
    }
}

- (void) timerProc:(NSTimer*)t
{
    NSLog(@"Timer close alert!");
    if ( self.alert ) 
    {
        [NSApp endSheet:_alert.window];        
    }
}
#endif // TARGET_OS_IPHONE==1

#pragma mark - iPhone Ping Pong
    
/*
 *  This one is a bit more complicated.
 *
 *  We have two methods:
 *
 *  ping:hostName 
 *
 *  is called from a host, which also publishes a remote object. Upon reception of a 'ping'
 *  request, we'll establish a connection to the calling host, call the 'pong' method and return
 *  the result.
 *
 *  pong:hostname
 *
 *  therefore is only called from hosts, we have sent a 'ping' before. 'pong' will simply
 *  compare the host name parameter passed on to the local host name and (normnally) return TRUE,
 *  if there's a match.
 *
 *  Once this is done, all connections and proxies are released automatically. So now for the
 *  code, which is pretty simple and straightforward as the most overhead is managed by the
 *  ETConnect framework:
 */
    
- (out NSString*) ping:(NSString*)hostName
{
    id <ESRemoteObjectProtocol> proxy = nil;
    NSString* ret = [NSString stringWithFormat:@"%@ not published on %@!",DEMO_SERVICE_NAME,hostName];
    /*
     *  Create the proxy for the sender side and call back ...
     */
    proxy = (id <ESRemoteObjectProtocol>)[EIConnection rootProxyForConnectionWithRegisteredName:DEMO_SERVICE_NAME host:hostName];
    if ( proxy != nil ){
        ret = [proxy pong:[[ETConnectServer sharedInstance] publishedName]];
    }
    return(ret);
}                       

/*
 *  This will be called from the remote host's 'ping' routine...
 */
- (out NSString*) pong:(NSString*)hostName
{
    NSString* ret = [NSString stringWithFormat:@"pong response from %@ to %@",[[ETConnectServer sharedInstance] publishedName],hostName];
    return(ret);
}
    
    
    
@end
