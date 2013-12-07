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
<NSTableViewDataSource, NSTableViewDelegate>

- (IBAction) btnPublishClicked:(NSButton*)sender;

- (IBAction) connect:(NSButton *)sender;
- (IBAction) pingPong:(NSButton *)sender;

@property (nonatomic, retain) IBOutlet NSButton* btnPublish;
@property (nonatomic, retain) IBOutlet NSButton* enableBluetooth;

@property (nonatomic, retain) IBOutlet NSTableView* tvHosts;

@property (nonatomic, retain) EIConnection* remoteService;
@property (nonatomic, retain) ESRemoteObject* testObject;
@property (nonatomic, retain) NSArray* hosts;
@property (nonatomic, retain) NSString* selectedHost;

/*
 *  Selected host detail properties:
 */
@property (nonatomic, retain) IBOutlet NSView* detailView;
@property (nonatomic, retain) IBOutlet NSTextField* detailTitle;
@property (nonatomic, retain) IBOutlet NSTextField* deviceDetails;

@property (nonatomic, retain) IBOutlet NSButton* btnConnect;
@property (nonatomic, retain) IBOutlet NSButton* btnPingPong;

@end
