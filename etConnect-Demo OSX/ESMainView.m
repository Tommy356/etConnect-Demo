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

- (id) initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self) {
        /*
         *  This will add this view as a notification server. The notification routines
         *  (see below) will be called automatically.
         */
        [ETConnectServerNotifications addNotificationObserver:self];
        
        /*
         *  Create a new hosts array. This will be initially empty. As new hosts are detected or
         *  removed, we'll receive automatic notifications (see below) and update the
         *  table view...
         */
        self.hosts = [NSArray array];
    }
    return self;
}

- (void) awakeFromNib
{
    _detailView.hidden = TRUE;
}

/*
- (void)drawRect:(NSRect)dirtyRect
{
	[super drawRect:dirtyRect];
}
*/

/*
 *  Depending on the state of tghe switch, we'll either publish ('vend') a remote object
 *  or stop publishing it.
 */
- (IBAction)btnPublishClicked:(NSButton *)sender 
{
    if ( sender.state )
    {
        NSLog(@"Publishing ...");
        
        /*
         *  Create an instance of an -ESRemoteObject (which will serve as sample root object)
         *  and vend it, by creating en EIConnection with this instance as root.
         */
        ESRemoteObject* localRoot = [[ESRemoteObject alloc] init];
        EIConnection* connection = [EIConnection startVending:DEMO_SERVICE_NAME rootObject:localRoot];
        /* 
         *  once published successfully, keep a (retained) reference to the connection
         *  and the root object
         */
        if ( connection ) {
            localRoot.window = self.window;
            self.publishedConnection = connection;
            self.publishedRoot = localRoot;
        }
        
    } else {
        
        NSLog(@"Stop Publishing ...");
        
        /*
         *  IMPORTANT:
         *  We NEED to inform the connection explicitely, that it should stop vending, 
         *  before we may release it.
         *  So, remember to call 'stopVending' always, before releasing a published 
         *  connection, otherwise you'll produce memory leaks as the connection will never
         *  be released.
         */
        [self.publishedConnection stopVending];
        
        /*
         *  After stopping the 'vend', just release the references and we're done.
         */
        self.publishedRoot = nil;
        self.publishedConnection = nil;
    }
}

/*
 *  This is the more interesing part:
 *
 *  we'll query the proxy from the host and - once successfull - call some routines
 *  remotely...
 */
- (IBAction)connect:(NSButton *)sender 
{
    /*
     *  Receive a proxy to the remote object published by this host...
     */
    EIDistantObject* proxy = 
    [EIConnection rootProxyForConnectionWithRegisteredName:DEMO_SERVICE_NAME host:_selectedHost];
    /*
     *  At this poiunt we might receive a nil-value, if the host just stopped publishiung, was shot down,
     *  lost network connection or anything else mean you might think of.
     *  In this case, just stop here ...
     */
    if ( proxy == nil )
    {
        NSLog(@"Can't reach service %@ on host: %@!",DEMO_SERVICE_NAME,_selectedHost);
        return;
    }
    
    /*
     *  Once we've got the proxy, we may (theorethically) call ANY method implemented 
     *  on the receiver side.
     *
     *  EIConnection however, as the dispatching interface, will refuse calls with
     *  undefined variable sizes, as this might lead to unpredictable results on
     *  both sides. Therefore, as a good practice, you may define a protocol with 
     *  API prototypes supported by your host as shown in ESRemoteObject.h, erspecially, when
     *  dealing with structures such as NSRange.
     *
     *  Setting a protocol will also reduce network traffic significantly, as signatures will
     *  not be queried from the remote side. 
     *
     *  For more information on this subject, please refer to the etConnect Framework reference.
     * 
     *  For this Demo, we'll just cast the proxy and set the protcol. This might be done more elegant,
     *  but for now this will work fine:
     */
    [proxy setProtocolForProxy:@protocol(ESRemoteObjectProtocol)];
    id <ESRemoteObjectProtocol> remoteObject = (id <ESRemoteObjectProtocol>) proxy;
    
    /*
     *  Now, you may call any methoid on the remote object (almost) like you would do on
     *  a local instance.
     *  The following call will just display an alert on the remote device.
     */
    [remoteObject sayHello:@"Hello World!"];
    
    /*
     *  Nothing more to do here ... As 'rootProxyForConnectionWithRegisteredName' returns an
     *  autoreleased copy, all related connection stuff is shut down and released automatically,
     *  after we're done here.
     */
}

/*
 *  This will call the 'ping' on the remote side, resulting in a pong to ourself ... 
 */
- (IBAction)pingPong:(NSButton *)sender 
{
    /*
     *  Create the remote proxy - see decription for connect:sender, above.
     */
    EIDistantObject* proxy = [EIConnection rootProxyForConnectionWithRegisteredName:DEMO_SERVICE_NAME host:_selectedHost];
    if ( proxy == nil )
    {
        NSLog(@"Can't reach service %@ on host: %@. Closing detail view!",DEMO_SERVICE_NAME,_selectedHost);
        return;
    }
    [proxy setProtocolForProxy:@protocol(ESRemoteObjectProtocol)];
    id <ESRemoteObjectProtocol> remoteObject = (id <ESRemoteObjectProtocol>) proxy;
    
    NSString* myHost = [[ETConnectServer sharedInstance] publishedName];
    NSString* result = [remoteObject ping:myHost];
    
    NSLog(@"ping result: %@",result);
}

/*
 *  The following notifications are sent by the ETConnectServer as it discovers new
 *  devices, device removals or changes in the service offerings for a certain device.
 *  Upon reception of these notifications we'll update the table view showing the
 *  active devices on the current network.
 */
#pragma mark - ETConnectServer Notifications
/*
 *  Whenever hosts are added, removed, or change, we need to renew our list of hosts and
 *  refresh the table view ...
 */
- (void) updateHosts
{
    /* get & retain the current list of hosts */
    self.hosts = [[ETConnectServer sharedInstance] hosts];
    
    /* reload the table view data */
    [_tvHosts reloadData];
    
    if ( _selectedHost != nil )
    {
        if ( [_hosts containsObject:_selectedHost] )
            [self updateSelection:[_hosts indexOfObjectIdenticalTo:_selectedHost]];
        else
            [self updateSelection:-1];
    }
}

/** the server has added a new host */
- (void) ETConnectServerDidAddHost:(NSNotification*)note
{
    NSLog(@"ESViewController::ETConnectServerDidAddHost: %@",note.object);
    [self updateHosts];
}

/** the server has removed a host, which was added previously */
- (void) ETConnectServerDidRemoveHost:(NSNotification*)note
{
    NSLog(@"ESViewController::ETConnectServerDidRemoveHost: %@",note.object);
    [self updateHosts];
}

/** the server has updated the services offered by a host;
 *  we need to find the devic e view and make it update its controls...
 */
- (void) ETConnectServerDidUpdateHost:(NSNotification*)note
{
    NSLog(@"ESViewController::ETConnectServerDidUpdateHost: %@",note.object);
    [self updateHosts];
}


#pragma mark - TableView Datasource & Delegate
- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView 
{
    return _hosts.count;
}

- (NSView *)tableView:(NSTableView *)tableView viewForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{
    // The cell is setup in IB. The textField and imageView outlets are properly setup.
    // Special attributes are automatically applied by NSTableView/NSOutlineView for the source list
    NSTableCellView *result = [tableView makeViewWithIdentifier:@"MainCell" owner:self];
    NSString* strHost = [_hosts objectAtIndex:row];
    result.textField.stringValue = strHost;
    result.imageView.image = [[ETConnectServer sharedInstance] host:strHost providesService:DEMO_SERVICE_NAME] ?
        [NSImage imageNamed:@"etDemo Icon_0120 transparent_on.png"] :
        [NSImage imageNamed:@"etDemo Icon_0120 transparent_off.png"];
    
    return(result);
}

/**
 *  selection did change ... check, what we need to display
 */
- (void)tableViewSelectionDidChange:(NSNotification *)notification
{
    NSLog(@"Changed row: %ld",[_tvHosts selectedRow]);
    [self updateSelection:[_tvHosts selectedRow]];
/*    
    if ([_tvHosts selectedRow] != -1)
    {
        NSTableCellView *view = [_tvHosts viewAtColumn:0 row:[_tvHosts selectedRow] makeIfNecessary:NO];
        NSLog(@"Host: %@",view.textField.stringValue);
        _detailView.hidden = FALSE;
        _detailTitle.stringValue = view.textField.stringValue;
    } else {
        _detailView.hidden = TRUE;
    }
 */
}

/*
 *  Depending on the services offered by the remoote host, update the GUI... 
 *  
 *  This is called, after the view has been loaded and each time, we'll receive change
 *  notifications from the ETConnectServer ...
 */
- (void) updateSelection:(NSInteger)selected
{
    if ( selected == -1 ){
        self.selectedHost  = nil;
        _detailView.hidden = TRUE;
        return;
    }
    
    self.selectedHost = [_hosts objectAtIndex:selected];
    BOOL hasService = [[ETConnectServer sharedInstance] host:_selectedHost providesService:DEMO_SERVICE_NAME];
    
    //_titleIcon.image = hasService ?
    //[UIImage imageNamed:@"etDemo Icon_0120 transparent_on.png"] :
    //[UIImage imageNamed:@"etDemo Icon_0120 transparent_off.png"];
    
    /*
     *  For iOS 7, enabling/disabling would be sufficient; as we also support 6.0, we
     *  need to change the visual appearance...
     */
    _btnConnect.enabled = hasService;
    _btnConnect.hidden  = !hasService;
    //_btnConnect.  titleLabel.textColor = hasService ? [NSColor blueColor] : [NSColor lightGrayColor];
    
    _btnPingPong.enabled = hasService;
    _btnPingPong.hidden  = !hasService;
    //_btnPingPong.titleLabel.textColor = hasService ? [NSColor blueColor] : [NSColor lightGrayColor];
    
    /*
     *  Retrieve some more information about this host:
     *
     *  We'll get the instance of the net service for this host and check out it's published 
     *  dictionary...
     *
     *  First, retrieve the EINetService for this host. 
     */
    EINetService* service = [[ETConnectServer sharedInstance] eiNetServiceForHost:_selectedHost];
    if ( service ) {
        /*
         *  Each registered service holds a remote dictionary with basic information aout a host.
         *  We don't need to be connected to access this information:
         */
        NSDictionary* remoteDict = [service remoteDict];
        NSMutableString* strInfo = [NSMutableString string];
        if ( remoteDict ) {
            NSDictionary* deviceDict = [remoteDict valueForKey:@"deviceDict"];
            [strInfo appendFormat:@"Host  \t: %@\n",_selectedHost];
            [strInfo appendFormat:@"Model \t: %@\n",[deviceDict valueForKey:@"model"]];
            [strInfo appendFormat:@"Type  \t: %@\n\nbtnConnect",[deviceDict valueForKey:@"type"]];
            
            NSDictionary* serviceDict = [remoteDict valueForKey:@"serviceDict"];
            [strInfo appendFormat:@"Services\t:\n"];
            [serviceDict enumerateKeysAndObjectsUsingBlock:^(NSString* key, id obj, BOOL *stop) {
                [strInfo appendFormat:@"\t\t%@\n",key];
            }];
            
        }
        _deviceDetails.stringValue = strInfo;
    }
    
    _detailView.hidden = FALSE;
}


@end
