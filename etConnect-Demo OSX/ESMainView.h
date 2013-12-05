//
//  ESMainView.h
//  etConnect-Demo
//
//  Created by Thomas Krautter on 05.12.13.
//  Copyright (c) 2013 eitap Technologies GmbH. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <ETConnect-OSX/ETConnect.h>
#import "ESRemoteObject.h"

@interface ESMainView : NSView

- (IBAction) btnPublishClicked:(id)sender;

@property (nonatomic, retain) IBOutlet NSButton* btnPublish;
@property (nonatomic, retain) IBOutlet NSButton* enableBluetooth;

@property (nonatomic, retain) IBOutlet NSTableView* tv;

@property (nonatomic, retain) EIConnection* remoteService;
@property (nonatomic, retain) ESRemoteObject* testObject;

@end
