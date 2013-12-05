//
//  EIConnectionServer.h
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
#if TARGET_OS_IPHONE == 1
#import "NSPortNameServer.h"
#endif //#if TARGET_OS_IPHONE == 1

#define EIApplicationProtocol   @"eiconnect-remc"
#define EIServiceDictKey        @"serviceDict"

@class EIConnectionServer, NSPortNameServer;
@protocol TCPServerDelegate;

#define START_OPTIONS

#ifdef START_OPTIONS
/** EIConnectionServer flags */
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
#endif //#ifdef START_OPTIONS


/**
 *  The protocol for the EIConnectionServer delegate
 */

@protocol EIConnectionServerDelegate <NSObject>
@optional
/** notifications, when new hosts are detected or dropped */
- (void) eiConnectionServer:(EIConnectionServer*)server didAddHost:(NSString*)hostName;
- (void) eiConnectionServer:(EIConnectionServer*)server didRemoveHost:(NSString*)hostName;
/** notifications, when hosts publish or remove services */
- (void) eiConnectionServer:(EIConnectionServer*)server didUpdateHost:(NSString*)hostName;
/** notifications when own service has been published */
- (void) eiConnectionServer:(EIConnectionServer*)server didPublish:(NSString*)hostName;
/** notifications when own service has been published */
- (void) eiConnectionServer:(EIConnectionServer*)server didNotPublish:(NSString*)hostName;
@end

@interface EIConnectionServer : NSPortNameServer
<EINetServiceDelegate, TCPServerDelegate, NSNetServiceBrowserDelegate>
{
    /** base bonjour server to manage connections */
    TCPServer*      _server;
    /** TRUE, if we normally should discover services  */
    BOOL            _shouldSearch;
    /** TRUE, if we're currently searching for services. */
    BOOL            _isSearching;
    /** TRUE, if we should locate other services after own service is published */
    BOOL            _searchOnPublish;
    /** TRUE, if bluetooth should be enabled */
    BOOL            _btEnabled;
    
    /** port ID counter ... */
    NSUInteger      _nextPortID;
    
    NSMapTable      *_portMap; /* Registered ports information.	*/
    NSMapTable      *_nameMap; /* Registered names information.	*/
}

#pragma mark - SERVICE MANAGEMENT -
#ifdef START_OPTIONS
- (BOOL) startWithOptions:(ETServerOptions)options;
#endif //#ifdef START_OPTIONS

#pragma mark - required NSPortNameServer overwrites:

#pragma mark - PORT MANAGEMENT -
- (NSUInteger) nextPortID;
- (NSPort *) portForName:(NSString *)name host:(NSString *)host;
- (NSPort *) portForName:(NSString *)name;
- (BOOL) registerPort:(NSPort *)port name:(NSString *)name host:(NSString *)host;

/** register & remove ports for the local service */
- (BOOL) registerPort:(NSPort*)port name:(NSString*)name;
- (BOOL) removePortForName:(NSString *)name host:(NSString *)host;
- (BOOL) removePortForName:(NSString *)name;

#pragma mark - PUBLISHING -
- (void) setup;
- (void) setBlutoothEnabled:(BOOL)blutoothEnabled;
- (BOOL) blutoothEnabled;

#pragma mark - EIConnectionServer additions:

#pragma mark - PUBLIC SERVICE METHODS
- (NSArray*) connectionNamesForService:(EINetService*)eiSvc;
- (NSArray*) connectionNamesForHost:(NSString*)hostName;

/** returns an array of known hosts */
- (NSArray*) hosts;
/** returns the EINetService for a named host or 'nil', if the host is not found */
- (EINetService*) eiNetServiceForHost:(NSString*)hostName;
/** returns the EINetService machting the socket data provided */
- (EINetService*) eiNetServiceForAddress:(NSData*)sockAddrData;
/** checks, whether  the service named is available on the host passed */
- (BOOL) host:(NSString*)hostName providesService:(NSString*)serviceName;

+ (id)sharedInstance;
+ (ETServerPlatformType) platformType;

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

/** the service browser - up all time */
@property (nonatomic, retain) NSNetServiceBrowser *netServiceBrowser;

/** the service dictionary - holds all published connections */
@property (nonatomic, retain) NSMutableDictionary *serviceDict;

/** our own published service name, if published, 'nil' otherwise */
@property (nonatomic, readonly) NSString* publishedName;

/** our own service, if we're published */
@property (nonatomic, retain) NSNetService* service;

@end
