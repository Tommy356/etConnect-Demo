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
 *  The protocol for the ETConnectServer delegate. Thge delegate will be notified
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

/** ETConnectServer
 *  Originally based on NSPortNameServer, which is not available
 *  on iOS.
 *  FIXME: remove the references to NSPortNameServer, as we use
 *  streamed connections now.
 */
//@interface ETConnectServer : NSPortNameServer
@interface ETConnectServer : NSObject

#pragma mark - CLASS METHODS -

+ (id)sharedInstance;
+ (ETServerPlatformType) platformType;
+ (void) dumpAll;
- (void) dumpAll;

#pragma mark - SERVICE MANAGEMENT -
- (BOOL) startWithOptions:(ETServerOptions)options;

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

/** returns an array of known hosts */
- (NSArray*) hosts;
/** returns the EINetService for a named host or 'nil', if the host is not found */
- (EINetService*) eiNetServiceForHost:(NSString*)hostName;
/** returns the EINetService machting the socket data provided */
- (EINetService*) eiNetServiceForAddress:(NSData*)sockAddrData;
/** checks, whether the service named is available on the host passed */
- (BOOL) host:(NSString*)hostName providesService:(NSString*)serviceName;
/** returns the TXTRecordDictionary for a given host, or nil if the host could not be found */
- (NSDictionary*) serviceDictForHost:(NSString*)hostName;
/** returns the connection type for a host */
- (ETHostConnectionTypes) hostConnectionType:(NSString*)hostName;

#pragma mark - SERVICE SEARCH CONFIGURATION
/** TRUE (default), is sevices need to be suspended in background mode (iOS only) */
@property (nonatomic, assign) BOOL suspendInBackground;

/** The name we'll publish (and search) services for */
@property (nonatomic, retain) NSString* protocolName;

/** The name we'll publish (and search) services for */
@property (nonatomic, retain) NSString* publishName;

/** the service types to search for - aUtomatically configured */
@property (nonatomic, retain) NSString* searchServiceType;

/** our (optional) delegate ... */
@property (nonatomic, retain) id<EIConnectionServerDelegate> delegate;

/** holds an array of CANetService objects */
@property (nonatomic, retain) NSMutableArray *registeredServices;

/** our own published service name, if published, 'nil' otherwise */
@property (nonatomic, readonly) NSString* publishedName;

/** our own service, if we're published */
@property (nonatomic, retain) NSNetService* service;

@end
