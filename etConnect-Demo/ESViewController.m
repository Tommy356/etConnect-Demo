//
//  ESViewController.m
//  etConnect-Demo
//
//  Created by Thomas Krautter on 30.11.13.
//  Copyright (c) 2013 eitap Technologies GmbH. All rights reserved.
//
//  This view controller:
//
//  - monitors all ETConnect hosts on the current network and - optionally - bluetooth,
//  - allows to switch bluetotth support on and off,
//  - allows to publish and unpublish a demo service
//  - switches to a detailed view fore active devices (see ESDetailViewController)
//
//  History:    Added bluetooth switch functionality,
//              Added a bluetooth icon, if a BT peer has been found

#import "ESViewController.h"


@interface ESViewController ()

@end

@implementation ESViewController

    
- (void)dealloc 
{
    /*
     *  Remove ourself from the list of server notificatrion objects
     */
    [ETConnectServerNotifications removeNotificationObserver:self];
    
    [_tvHosts release];
    [_swPublish release];
    [_swBluetooth release];
    
    [super dealloc];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    
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
    
    /*
     *  Set the status of the bluetooth switch. If bluetooth has been anabled by supplying the
     *  peerToPeer flag on startup, it's on by default, off otherwise.
     */
    _swBluetooth.on = [[ETConnectServer sharedInstance] bluetoothEnabled];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
 *  This sections handles the GUI interaction on the main view
 */
#pragma mark - GUI Interaction

/*
 *  Depending on the state of tghe switch, we'll either publish ('vend') a remote object
 *  or stop publishing it.
 */
- (IBAction)publish:(UISwitch *)sender 
{
    if ( sender.on )
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
 *  Depending on the state of the switch, we'll enabvle bluetooth (peer-to-peer) services or
 *  disable them (currently with iOS7 only).
 *
 *  With Bluetooth switched ON, the current device will be visible for other devices, that
 *  have switched Bluetooth on, AND it will report all Bluetooth hosts found.
 *
 *  Note, that with this button enabled, you're able to discover devices without a local 
 *  WiFi network. You just need to make sure, that Bluetooth is enabled on all devices
 *  you wish to connect to. Also, notification over Bluetooth will come in significantly 
 *  slower, than with Wifi only.
 *
 *  After selecting this button, the device will shortly disappear 
 */
- (IBAction)enableBluetooth:(UISwitch *)sender 
{
    /*
     *  For the current release of the framework, this is disabled, as there is still
     *  optimization work going on.
     */
    [[ETConnectServer sharedInstance] setBluetoothEnabled:sender.on];
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
  
/*
 *  This section provides the TableView Delegate & Datasource code...
 */
#pragma mark - TableView Delegate & Data Source
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _hosts.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Cell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
    }
    
    /*
     *  Set the title to the host name, add some additional information,
     *  set an icon, if this host currently publishes a remote object and set
     *  a disclosure which will display details for this host.
     */
    NSString* strHost = [_hosts objectAtIndex:indexPath.row];
    
    cell.textLabel.text = strHost;

    UIImage* image  = [[ETConnectServer sharedInstance] host:strHost providesService:DEMO_SERVICE_NAME] ?
        [UIImage imageNamed:@"etDemo Icon_0120 transparent_on.png"] :
        [UIImage imageNamed:@"etDemo Icon_0120 transparent_off.png"];
    
    /*
     *  Check the connection type to this device; if it's a bluetooth connection, add
     *  a bluetooth icon ...
     */
    ETHostConnectionTypes ct = [[ETConnectServer sharedInstance] hostConnectionType:strHost];
    if ( ct & ETHostConnectionBluetooth ){
        UIImage* iconBT = [UIImage imageNamed:@"icon_bluetooth_0256.png"];
        UIGraphicsBeginImageContext(image.size);
        [image  drawInRect:CGRectMake(0, 0, image.size.width, image.size.height)];
        [iconBT drawInRect:CGRectMake(0, image.size.height/4, image.size.width/2, image.size.height/2)];
        image = UIGraphicsGetImageFromCurrentImageContext();
        UIGraphicsEndImageContext();
    }
    
    cell.imageView.image = image;
    
    return cell;
}
    
/*
 *  Once a cell on nthe table view is selected, bring up the detail view controller for the
 *  selected host.
 */
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell* cell = [tableView cellForRowAtIndexPath:indexPath];
    cell.selected = FALSE;
    
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
    ESDetailViewController *vc = [storyboard instantiateViewControllerWithIdentifier:@"DetailDeviceView"];
    
    /*
     *  Before presenting the detail view controller, set the host name selected:
     */
    vc.hostName = [_hosts objectAtIndex:indexPath.row];
    
    [self presentViewController:vc animated:YES completion:nil];
}

@end
