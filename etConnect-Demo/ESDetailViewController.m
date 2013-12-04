//
//  ESDetailViewController.m
//  etConnect-Demo
//
//  Created by Thomas Krautter on 03.12.13.
//  Copyright (c) 2013 eitap Technologies GmbH. All rights reserved.
//
//  This view controller will display details for a remote device and
//  provides a simple interface to do some test calls, if the remote device
//  provides the demo service.

#import "ESDetailViewController.h"

@interface ESDetailViewController ()

@end

@implementation ESDetailViewController
    
- (void) dealloc
{
    /*
     *  Remove ourself from the list of server notificatrion objects
     */
    [ETConnectServerNotifications removeNotificationObserver:self];
    
    self.hostName = nil;
    [_lblTitle release];
    [_titleIcon release];
    [_btnConnect release];
    [_btnPingPong release];
    [_lblInfo release];
    [super dealloc];
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
    
    /*
     *  This will add this view as a notification server. The notification routines
     *  (see below) will be called automatically.
     */
    [ETConnectServerNotifications addNotificationObserver:self];
    
    /*
     *  set the title to the host name we'd display and update all other GUI elements...
     */
    _lblTitle.text = _hostName;
    
    [self updateGUI];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
 *  This sections handles the GUI interaction for the detail view
 */
#pragma mark - GUI Interaction
/*
 *  We're done; dismiss this view and return to the main view
 */
- (IBAction)btnDoneSelected:(UIButton *)sender 
{
    [self dismissViewControllerAnimated:YES completion:nil];
}
    
/*
 *  This is the more interesing part:
 *
 *  we'll query the proxy from the host and - once successfull - call some routines
 *  remotely...
 */
- (IBAction)connect:(UIButton *)sender 
{
    /*
     *  Receive a proxy to the remote object published by this host...
     */
    EIDistantObject* proxy = 
    [EIConnection rootProxyForConnectionWithRegisteredName:DEMO_SERVICE_NAME 
                                                      host:_hostName];
    /*
     *  At this poiunt we might receive a nil-value, if the host just stopped publishiung, was shot down,
     *  lost network connection or anything else mean you might think of.
     *  In this case, just stop here ...
     */
    if ( proxy == nil )
    {
        NSLog(@"Can't reach service %@ on host: %@. Closing detail view!",DEMO_SERVICE_NAME,_hostName);
        [self dismissViewControllerAnimated:YES completion:nil];
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
- (IBAction)pingPong:(UIButton *)sender 
{
    /*
     *  Create the remote proxy - see decription for connect:sender, above.
     */
    EIDistantObject* proxy = [EIConnection rootProxyForConnectionWithRegisteredName:DEMO_SERVICE_NAME host:_hostName];
    if ( proxy == nil )
    {
        NSLog(@"Can't reach service %@ on host: %@. Closing detail view!",DEMO_SERVICE_NAME,_hostName);
        [self dismissViewControllerAnimated:YES completion:nil];
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
 *
 *  For this view controller we'll just monitor, whether the services for our own device
 *  have been modified ort the device has stopped publishing...
 */
#pragma mark - ETConnectServer Notifications
/*
 *  Depending on the services offered by the remoote host, update the GUI... 
 *  
 *  This is called, after the view has been loaded and each time, we'll receive change
 *  notifications from the ETConnectServer ...
 */
- (void) updateGUI
{
    BOOL hasService = [[ETConnectServer sharedInstance] host:_hostName providesService:DEMO_SERVICE_NAME];
    
    _titleIcon.image = hasService ?
        [UIImage imageNamed:@"etDemo Icon_0120 transparent_on.png"] :
        [UIImage imageNamed:@"etDemo Icon_0120 transparent_off.png"];
    
    /*
     *  For iOS 7, enabling/disabling would be sufficient; as we also support 6.0, we
     *  need to change the visual appearance...
     */
    _btnConnect.enabled = hasService;
    _btnConnect.titleLabel.textColor = hasService ? [UIColor blueColor] : [UIColor lightGrayColor];
    
    _btnPingPong.enabled = hasService;
    _btnPingPong.titleLabel.textColor = hasService ? [UIColor blueColor] : [UIColor lightGrayColor];
    
    /*
     *  Retrieve some more information about this host:
     *
     *  We'll get the instance of the net service for this host and check out it's published 
     *  dictionary...
     *
     *  First, retrieve the EINetService for this host. 
     */
    EINetService* service = [[ETConnectServer sharedInstance] eiNetServiceForHost:_hostName];
    if ( service ) {
        /*
         *  Each registered service holds a remote dictionary with basic information aout a host.
         *  We don't need to be connected to access this information:
         */
        NSDictionary* remoteDict = [service remoteDict];
        NSMutableString* strInfo = [NSMutableString string];
        if ( remoteDict ) {
            NSDictionary* deviceDict = [remoteDict valueForKey:@"deviceDict"];
            [strInfo appendFormat:@"Host  \t: %@\n",_hostName];
            [strInfo appendFormat:@"Model \t: %@\n",[deviceDict valueForKey:@"model"]];
            [strInfo appendFormat:@"Type  \t: %@\n",[deviceDict valueForKey:@"type"]];

            NSDictionary* serviceDict = [remoteDict valueForKey:@"serviceDict"];
            [strInfo appendFormat:@"Services\t:\n"];
            [serviceDict enumerateKeysAndObjectsUsingBlock:^(NSString* key, id obj, BOOL *stop) {
                [strInfo appendFormat:@"\t\t%@\n",key];
            }];
            
        }
        _lblInfo.text = strInfo;
    }
}
    
/** the server has added a new host */
- (void) ETConnectServerDidAddHost:(NSNotification*)note
{
    NSLog(@"ESViewController::ETConnectServerDidAddHost: %@",note.object);
}

/** the server has removed a host, which was added previously */
- (void) ETConnectServerDidRemoveHost:(NSNotification*)note
{
    NSLog(@"ESViewController::ETConnectServerDidRemoveHost: %@",note.object);
    /*
     *  if this is the host currently displayed, we'll dismiss this view
     */
    if ( [note.object isEqualToString:_hostName] )
    {
        [self dismissViewControllerAnimated:YES completion:nil];
    }
}

/** the server has updated the services offered by a host;
 *  if this is our service, wen need to update our controls ...
 */
- (void) ETConnectServerDidUpdateHost:(NSNotification*)note
{
    NSLog(@"ESViewController::ETConnectServerDidUpdateHost: %@",note.object);
    /*
     *  if this is the host currently displayed, we'll update the GUI...
     */
    if ( [note.object isEqualToString:_hostName] )
    {
        [self updateGUI];
    }
}

    
@end
