//
//  EIConnection.h
//
//  etConnect Framework
//
//  Created by Thomas Krautter on 06.10.13.
//  Copyright (c) 2013 Thomas Krautter. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ETConnectTypes.h"

@class NSMutableData, EIDistantObject, NSException, NSData;
@class NSPort, NSRunLoop, NSDictionary, NSArray;
@class NSDistantObjectRequest;
@class GSLazyRecursiveLock;
@class EIPortCoder;
@class ETConnectServer;
@class EINetMessage;
@class EINetConnection;

@protocol EIConnectionDelegate;

/*
 *	Keys for the NSDictionary returned by [EIConnection -statistics]
 */
extern NSString* const NSConnectionRepliesReceived;
extern NSString* const NSConnectionRepliesSent;
extern NSString* const NSConnectionRequestsReceived;
extern NSString* const NSConnectionRequestsSent;
extern NSString* const NSConnectionLocalCount;
extern NSString* const NSConnectionProxyCount;

/*
 *  ETConnectionTypes - define the nature of a connection:
 */
typedef NS_ENUM(UInt32,ETConnectionTypes)
{
    /* undefined */
    ETConnectionTypeUndefined = 0,
    /* Root connection for locally published objects */
    ETConnectionTypeRoot,
    /* Service connection */
    ETConnectionTypeService,
    /* Remote connection */
    ETConnectionTypeProxy
};


#pragma mark - EIConnection -
/** The main class to vend objects or to subscribe to objects vended by other hosts.
 *
 *  This class provides functionality similiar to the <b>NSConnection</b> class provided on OSX, however
 *  due to architectural differences the API and implementation differs from the original class implementation.
 *  You'll use this class to 'vend' services and to subscribe to services vended by other hosts.
 *
 *  Publishing Services:
 *  ---------------------
 *  To publish a service, make sure, the shared instance of ETConnectServer has been started by your App correctly.
 *  Once the ETConnectServer has been started, you'll need to:
 *
 *  - Create an instance of the object you want to make available for remote calls
 *  - Call the startVending:rootObject: method to create a new EIConnection to hanbdle remote calls to the instance of this object
 *
 *  Subscribing to Services:
 *  ------------------------
 *  To subscribe to a service published by another App or another deivice, make sure, the shared instance of ETConnectServer has 
 *  been started by your App correctly. You'll need to specify the ETServerOptionSearch option, when starting the service.
 *  Once the ETConnectServer is running, you'll need to:
 *
 *  - Pick up an available host name from ETConnectServer's hosts array
 *  - Call the subscribeTo:onHost subscribeTo:onHost:usingProtocol method to receive a proxy to the remote object
 *  - Call any method published by the remote object
 *
 *  Unsubscribing from a service:
 *  ------------------------
 *  To unsubscribe fromn a service, just release the proxy obtained in the step described above. This will release the proxy and,
 *  if it's not retained otherwise, close all communication streams to the remote host and release the underlying EIConnection instance.
 *
 *  Unpublishing Services:
 *  ----------------------
 *  To stop publishing a service, release the EIConnection instance returned by the call to startVending:rootObject described previously.
 *
 */
@interface EIConnection : NSObject
#pragma mark Transport Layer
#pragma mark - VENDING SERVICES
/** @name Vending Objects */

/** Starts vending an object
 *
 *  To call this successful, the shared instance of ETConnectServer must have been started
 *  with at least poublishing mode set.
 *  
 @param name: a unique name, which will be used to announce the vending service oin the current domain
 @param root: an instance of an NSObject-based class, which will serve as the root object for the service
 @returns a new EIConnection instance, responsible to handle & dispatch incoming connection requests and route them to the root object specified
 */
+ (id)startVending:(NSString *)name rootObject:(id)root;

/** Stops vending an object. 
 *
 *  Calling this method will also immediately close all connections to remote clients and cancel all
 *  possibly outstanding requests.
 *  This method needs to be called <b>obligatory</b>, before a service connection is released.
 *
 *  @returns: TRUE, if the vending has been stopped, FALSE, if an error occured.
 */
- (BOOL)stopVending;

#pragma mark - SUBSCRIBING TO SERVICES
/** @name Subscribing to Services */
/**
 * Returns a connection initialised using -initWithReceivePort:sendPort:<br />
 * Both ports must be of the same type.
 * DEPRECATED - Do not use any more.
 */
+ (EIConnection*)connectionWithReceivePort:(NSPort*)r sendPort:(NSPort*)s;

/**
 * Returns an EIConnection object whose send port is that of the
 * EIConnection registered under the name n on the host h
 * <p>This method calls +connectionWithRegisteredName:host:usingNameServer:
 * using the default system name server.
 */
+ (EIConnection*)connectionWithRegisteredName:(NSString*)n host:(NSString*)h;

/**
 * This method calls
 * +rootProxyForConnectionWithRegisteredName:host:usingNameServer:
 * to return a proxy for a root object on the remote connection with
 * the send port registered under name n on host h.
 */
+ (EIDistantObject*)rootProxyForConnectionWithRegisteredName:(NSString*)n
                                                        host:(NSString*)h;

/**
 * This method calls
 * +connectionWithRegisteredName:host:usingNameServer:
 * to get a connection, then sends it a -rootProxy message to get
 * a proxy for the root object being vended by the remote connection.
 * Returns the proxy or nil if it couldn't find a connection or if
 * the root object for the connection has not been set.<br />
 * Use [ETConnectServer+sharedInstance]
 * for connections to remote hosts.
 */
+ (EIDistantObject*)rootProxyForConnectionWithRegisteredName:(NSString*)n
                                                        host:(NSString*)h
                                             usingNameServer:(ETConnectServer*)s;

/** Creates a service connection with a specified root object and a specified name.
 *
 * Deprecated: you should use 'startVending:root' instead.
 *
 @param name: a unique name, which will be used to announce the vending service oin the current domain
 @param root: an instance of an NSObject-based class, which will serve as the root object for the service
 @returns a new EIConnection instance, responsible to handle & dispatch incoming connection requests and route them to the root object specified
 */
+ (id)serviceConnectionWithName:(NSString *)name
                     rootObject:(id)root;

/** Creates a service connection with a specified root object and a specified name using a
 *  specified instance of ETConnectServer.
 *
 * Deprecated: you should use 'startVending:root' instead.
 *
 @param name: a unique name, which will be used to announce the vending service oin the current domain
 @param root: an instance of an NSObject-based class, which will serve as the root object for the service
 @param usingNameServer: an instance of ETConnectServer
 @returns a new EIConnection instance, responsible to handle & dispatch incoming connection requests and route them to the root object specified
 */
+ (id)serviceConnectionWithName:(NSString *)name
                     rootObject:(id)root
                usingNameServer:(ETConnectServer *)server;

/** Initializes a new child connection for a vended object.
 *
 * This routine is used internally, to instantiate new EIConnection objects for remote
 * client requests. There is rarely any need to call this from within an application.
 *
 @param parent: the parent connection (must be a service connection) hosting a vended root object.
 @param netConnection: the net connection responsible to handle incoming requests and outgoing responses
 @returns a new EIConnection instance responsible to handle incoming requests. 
 */
- (id) initWithParent:(EIConnection*)parent andNetConnection:(EINetConnection*)netConnection;

/**
 *  called by EINetConnection, if a new port connect message has been received. 
 *
 *  The EINetConnection expects a child connection to be created, which will act as message handler for
 *  incoming messages. The netConnection passed will be responsible toi handle all traffic
 *  for this EIConnection. This routine is called from the net transport layer for parent connections,
 *  which are vending services.
 *  This routine will call initWithParent:andNetConnection
 *
 @param netConnection: the net connection responsible to handle incoming requests and outgoing responses
 @returns a new EIConnection instance responsible to handle incoming requests. 
 */
- (EIConnection*) connectRequest:(EINetConnection*)netConnection;

/*  An EIConnection may be of several types:
 *
 *  (1) a parent connection, holding a 'vended' root proxy and waiting for service calls,
 *  (2) a connection serving incoming calls, or
 *  (3) a connection holding a remote root proxy and all derviced objects,
 *  (4) a connection created by a local rootProy request.
 *
 *  In any case, it references either a service or a connection or both.
 */
@property (nonatomic, retain) EINetConnection* netConnection;
@property (nonatomic, retain) NSString* serviceName;
@property (nonatomic, retain) id connectionRootObject;
@property (nonatomic, retain) NSMutableArray* childConnections;


#pragma mark UNSORTED ENTRIES:
/** Returns a dictionary with some statistical information about an EIConnection instance.
 */
- (NSDictionary*)statistics;

#pragma mark NOTIFICATION HANDLING:

#pragma mark INIT & DEALLOC
/** Invalidates an existing EIConnection instance.
 *
 *  Internal management only; there should be no reason, to call this routine externally. To
 *  Release a service connection, call 'stopVending' and release the reference to the EIConnection.
 */
- (void)invalidate;

/** Returns TRUE, if an EIConnection is valid, FALSE, if a connection is shutting down due to local
 *  or remote requests.
 *
 */
- (BOOL)isValid;

#pragma mark INSTANCE PROPERTIES
/** The root object associated to this connection. If the connection refewrs to a locally
 *  published service, the local object is returned. If the coinnection refers to a remote service,
 *  a proxy to the remote service is returned.
 */
@property (nonatomic, retain) id rootObject;

/** The delegate for this connection. */
@property (nonatomic, retain) id<EIConnectionDelegate> delegate;
/** TRUE, if the connection is enabled to serve multple threads. */
@property (nonatomic, assign) BOOL multipleThreadsEnabled;
@property (nonatomic, assign) ETServerPlatformType platformType;
/** An (optional) name for a connection. Clients may use this property to identify connections. */
@property (nonatomic, retain) NSString* connName;

/** The type of the connection.
 *
 *  The following types are defined:
 *
 *  - <i>ETConnectionTypeUndefined</i>: No type assigned for this connection
 *  - <i>ETConnectionTypeRoot</i>: A root connection vending a local root object
 *  - <i>ETConnectionTypeService</i>: A connection to a remote root object
 *  - <i>ETConnectionTypeProxy</i>: A proxy connection established through a remote call to a vended service
 */
@property (nonatomic, assign) ETConnectionTypes connType;

#pragma mark PROXY STUFF
/**
 * Returns the proxy for the root object of the remote EIConnection.<br />
 * Generally you will wish to call [EIDistantObject-setProtocolForProxy:]
 * immediately after obtaining such a root proxy.
 */
- (EIDistantObject*)rootProxy;

#pragma mark CLASS METHODS

/**
 * Returns an array containing all EIConnection objects known to
 * the system. These connections will be valid at the time that the
 * array was created, but may be invalidated by other threads
 * before you get to examine the array.
 */
+ (NSArray*)allConnections;

+ (void) dumpAll;

/**
 *   Returns an EIConnection object which is registered under <em>name</em> on <em>host</em>.
 *
 *   The connect server <em>server</em> is used to look up the host to be used for the connection.
 *   Use [ETConnectServer sharedInstance] for connections to remote hosts.
 *
 *   If <em>host</em> is <code>nil</code> or an empty string, the host is taken to be the local machine.<br />
 *   If it is an asterisk ('*') then the nameserver checks all hosts on the local subnet (unless the nameserver is one
 *   that only manages local ports).
 */
+ (EIConnection*)connectionWithRegisteredName:(NSString*)n
                                         host:(NSString*)h
                              usingNameServer:(ETConnectServer*)s;

/**
 * Sets the EIConnection configuration so that multiple threads may
 * use the connection to send requests to the remote connection. This option is inherited by child connections.
 * A connection with multiple threads enabled will run slower than a normal connection.
 */
- (void)enableMultipleThreads;

/*
 * Backward compatibiulity - not implemented any more.
 */
+ (id)currentConversation;

/**
 *  called, once an EINetConnections becomes invalid
 */
- (void)connectionIsInvalid:(EINetConnection*)netConnection;

/** Debug only:
 *  sets the common debug level for all connection objects...
 */
+ (int) setDebug:(int)val;

/** Debug only:
 *  returns the common debug level for all connection objects...
 */
+ (int) getDebug;

@end

@interface EIConnection (MessageHandling)
/*
 *  An incoming net message is dispatched from an EINetConnection
 */
- (void)handleNetMessage:(EINetMessage*)msg;
@end


#pragma mark - EIConnectionDelegate
/** This protocol may be implemented by classes using EIConnection objects and to control
 *  the creation and authentication of connections and assiciated communication channels.
 *
 *  Note:
 *  -----
 *  This protocol is still subject to be changed and you should not rely on these routines until
 *  the first version of etConnect has been released.
 */
@protocol EIConnectionDelegate <NSObject>
@optional

//Use the NSConnectionDidInitializeNotification notification instead
// of this delegate method if possible.
- (BOOL)makeNewConnection:(EIConnection *)conn sender:(EIConnection *)ancestor;

// Use the NSConnectionDidInitializeNotification notification instead
// of this delegate method if possible.
- (BOOL)connection:(EIConnection *)ancestor shouldMakeNewConnection:(EIConnection *)conn;

- (NSData *)authenticationDataForComponents:(NSArray *)components;
- (BOOL)authenticateComponents:(NSArray *)components withData:(NSData *)signature;

- (id)createConversationForConnection:(EIConnection *)conn;

- (BOOL)connection:(EIConnection *)connection handleRequest:(NSDistantObjectRequest *)doreq;

@end

FOUNDATION_EXPORT NSString * const NSFailedAuthenticationException;
FOUNDATION_EXPORT NSString * const NSConnectionDidInitializeNotification;

#pragma mark - NSDistantObjectRequest
#if TARGET_OS_IPHONE==1
@interface NSDistantObjectRequest : NSObject
- (NSInvocation *)invocation;
- (EIConnection *)connection;
- (id)conversation;
- (void)replyWithException:(NSException *)exception;
@end
#endif //#if TARGET_OS_IPHONE==0


