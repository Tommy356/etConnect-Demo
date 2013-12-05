//
//  ESMainView.m
//  etConnect-Demo
//
//  Created by Thomas Krautter on 05.12.13.
//  Copyright (c) 2013 eitap Technologies GmbH. All rights reserved.
//

#import "ESMainView.h"

@implementation ESMainView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect
{
	[super drawRect:dirtyRect];
	
    // Drawing code here.
}


/** switch server on/off */
- (IBAction) btnPublishClicked:(id)sender
{
    NSButton*       btn = sender;
    ESRemoteObject* testObject = nil;
    
    NSLog(@"Publish clicked for %@!",btn);
    NSLog(@"Publish clicked for %@!",_btnPublish);
    NSLog(@"Publish state: %ld",(long)btn.state);
    
    /*
     testObject = [[DODTestObject alloc] init];
     testObject.window = self.window;
     [testObject showAlert:@"Service published!"];
     testObject = nil;
     return;
     */
    
    if ( btn.state && _remoteService == nil )
    {
        NSLog(@"Switch remote service on!");
        NSLog(@"OLD Connections: %@",[EIConnection allConnections]);
        
        testObject = [[ESRemoteObject alloc] init];
        testObject.window = self.window;
        
        [EIConnection setDebug:5];
        
        EIConnection* rs = [EIConnection serviceConnectionWithName:DEMO_SERVICE_NAME
                                                        rootObject:testObject];
        
        if ( rs != nil )
        {
            NSLog(@"Remote Service has been published!");
            self.remoteService = rs;
            self.testObject = testObject;
            NSLog(@"NEW Connections: %@",[EIConnection allConnections]);
            
        } else {
            NSLog(@"Remote Service could not be published!");
            self.remoteService = nil;
            btn.state = 0;
        }
        
        [testObject release];
        
    } else if ( !btn.state && _remoteService != nil )
    {
        
        NSLog(@"Switch remote service off!");
        [self.remoteService stopVending];
        NSLog(@"Invalidated!");
        //[self.remoteService release];
        self.remoteService = nil;
        NSLog(@"Released.... ");
        NSLog(@"Connections: %@",[EIConnection allConnections]);
        
        //[self.testObject release];
        self.testObject = nil;
        
    } else {
        
        NSLog(@"Strange!");
    }
}

@end
