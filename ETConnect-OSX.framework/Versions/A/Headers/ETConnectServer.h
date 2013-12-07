//
//  ETConnectServer.h
//  EIConnectionIOS
//
//  Created by Thomas Krautter on 10.10.13.
//  Copyright (c) 2013 Thomas Krautter. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ETConnectTypes.h"

#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#endif //#if TARGET_OS_IPHONE

#import "EINetService.h"

#ifdef USE_DNSSD
#import "DNSSDBrowser.h"
#endif //#ifdef USE_DNSSD

#define EIApplicationProtocol   @"eiconnect-remc"
#define EIServiceDictKey        @"serviceDict"
#define EIDeviceDictKey         @"deviceDict"

#import "ETConnectServerNotifications.h"

@class ETConnectServer;
@protocol TCPServerDelegate;

/** ETConnectServer flags */
typedef NS_OPTIONS(UInt16, ETServerOptions)
{
    /** server will start searching for services on the local network */
    ETServerOptionSearch = (1 << 0),
    /** server will publish the general ETConnect service */
    ETServerOptionPublish = (1 << 1),
    /** server will publish its management service (requires ETServerOptionPublish)  */
    ETServerOptionPublishService = (1 << 2),
    /** enables bluetooth communication (IOS7) */
    ETServerOptionPeerToPeer = (1 << 3),
    /** enables logging to ETConnect.log */
    ETServerOptionLog = (1 << 4),
};

typedef NS_OPTIONS(UInt16, ETHostConnectionTypes)
{
    ETHostConnectionUnknown = (0 << 0),
    ETHostConnectionWifi = (1 << 0),
    ETHostConnectionBluetooth = (1 << 1),
};

/**
 *  The protocol for the ETConnectServer delegate. The delegate will be notified
 *  if new hosts are added, removed or updated, and after publishing services on the
 *  local machine.
 */

@protocol EIConnectionServerDelegate <NSObject>
@optional
/** notifications, when new hosts are detected or dropped */
- (void) eiConnectionServer:(ETConnectServer*)server didAddHost:(NSString*)hostName;
- (void) eiConnectionServer:(ETConnectServer*)server didRemoveHost:(NSString*)hostName;
/** notifications, when hosts publish or remove services */
- (void) eiConnectionServer:(ETConnectServer*)server didUpdateHost:(NSString*)hostName;
/** notifications when own service has been published */
- (void) eiConnectionServer:(ETConnectServer*)server didPublish:(NSString*)hostName;
/** notifications when own service has been published */
- (void) eiConnectionServer:(ETConnectServer*)server didNotPublish:(NSString*)hostName;
@end

/** ETConnectServer provides a functionality similiar to NSPortNameServer. 
 *  However, as the NSPortNameServer class is not available on iOS and etConnect
 *  relies on NSStream classes rather than ports, implementation and API differes in
 *  several parts from what NSPortNameServer has been designed for.
 *
 *  There's a single shared in stance of ETConnectServer for every app - no need to create
 *  more than one instance.
 */
@interface ETConnectServer : NSObject

#pragma mark - CLASS METHODS -
/**---------------------------------------------------------------------------------------
 * @name Creating and starting the server
 *  ---------------------------------------------------------------------------------------
 */

/** Returns the shared instance of the <b>ETConnectServer</b>. If no instance has been
 *  created yet, a call to this method will automatically create it.
 @param none
 @return the shared instance of the ETConnectServer
 */
+ (id)sharedInstance;

/** Starts the server with the options supplied.

 The following options may be supplied, when starting the server:
 
 - <b>ETServerOptionSearch</b>: The server will also start to discover other hosts on the network
 - <b>ETServerOptionPublish</b>: The server will publish the general ETConnect service
 - <b>ETServerOptionPublishService</b>: The server will publish its management service (requires ETServerOptionPublish)
 - <b>ETServerOptionPeerToPeer</b>: Enables bluetooth communication (IOS7 only)
 
 @param options: a combination of one or more ETServerOptions values.
 @return TRUE, if the server is starting, FALSE if an error occured.
 */
- (BOOL) startWithOptions:(ETServerOptions)options;


+ (ETServerPlatformType) platformType;
+ (void) dumpAll;
- (void) dumpAll;

#pragma mark - SERVICE MANAGEMENT -

#pragma mark - required NSPortNameServer overwrites:

#pragma mark - NEW STYLE - CONNECTION MANAGEMENT -
- (BOOL) addRoot:(EIConnection*)conn withName:(NSString*)serviceName;
- (BOOL) removeRoot:(EIConnection*)conn withName:(NSString*)serviceName;
- (EINetConnection*)connectionForService:(NSString *)name host:(NSString *)host;

#pragma mark - PUBLISHING -
- (void) setup;
- (void) setBlutoothEnabled:(BOOL)blutoothEnabled;
- (BOOL) blutoothEnabled;

#pragma mark - ETConnectServer additions:

#pragma mark - PUBLIC SERVICE METHODS
- (NSArray*) connectionNamesForService:(EINetService*)eiSvc;
- (NSArray*) connectionNamesForHost:(NSString*)hostName;

/** @name Retrieving service information */

/** Returns an array of strings with all hosts currently available. The
 *  hosts may vary depending on whether new hosts are discovered or not.
 *  Clients may set a delegate to get a notification, whenever new hosts
 *  have been found (or removed).
 */
- (NSArray*) hosts;

/** returns the EINetService for a named host or 'nil', if the host is not found */
- (EINetService*) eiNetServiceForHost:(NSString*)hostName;
/** returns the EINetService machting the socket data provided */
- (EINetService*) eiNetServiceForAddress:(NSData*)sockAddrData;

/** checks, whether the service named is available on the host passed 
 
 @param hostName: a valid host name for a host in the current domain.
 @param serviceName: the name of the service to be queried
 @return TRUE, if the host provides the service, FALSE if the host is not found or the host does not provice the service specified.
 */
- (BOOL) host:(NSString*)hostName providesService:(NSString*)serviceName;

/** returns the TXTRecordDictionary for a given host, or nil if the host could not be found */
- (NSDictionary*) serviceDictForHost:(NSString*)hostName;
/** returns the connection type for a host */
- (ETHostConnectionTypes) hostConnectionType:(NSString*)hostName;

#pragma mark - PROPERTIES
/** @name Properties */

/** TRUE (default), is sevices need to be suspended in background mode (iOS only) */
@property (nonatomic, assign) BOOL suspendInBackground;

/** The name we'll publish (and search) services for */
@property (nonatomic, retain) NSString* protocolName;

/** The name we'll publish (and search) services for */
@property (nonatomic, retain) NSString* publishName;

/** the service types to search for - aUtomatically configured */
@property (nonatomic, retain) NSString* searchServiceType;

/** The ETConnectionServer delegate to receive notification
 *  on host changes (must implement the EIConnectionServerDelegate protocol). */
@property (nonatomic, retain) id<EIConnectionServerDelegate> delegate;

/** Holds an array of EINetService objects for every service found in the currewnt domain */
@property (nonatomic, retain) NSMutableArray *registeredServices;

/** Our own published service name, if published, 'nil' otherwise */
@property (nonatomic, readonly) NSString* publishedName;

/** Our own service, if we're published */
@property (nonatomic, retain) NSNetService* service;

@end
