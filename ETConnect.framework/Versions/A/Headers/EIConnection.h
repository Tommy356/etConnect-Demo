//
//  EIConnection.h
//  EIConnectionIOS
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
/* These in OPENSTEP 4.2 */
/**
 *  Key for dictionary returned by [EIConnection-statistics]: number of
 *  messages replied to so far by the remote connection.
 */
#  define GS_EXPORT  extern

GS_EXPORT NSString* const NSConnectionRepliesReceived;

/**
 *  Key for dictionary returned by [EIConnection-statistics]: number of
 *  messages sent so far to the remote connection.
 */
GS_EXPORT NSString* const NSConnectionRepliesSent;

/**
 *  Key for dictionary returned by [EIConnection-statistics]: number of
 *  messages received so far from the remote connection.
 */
GS_EXPORT NSString* const NSConnectionRequestsReceived;

/**
 *  Key for dictionary returned by [EIConnection-statistics]: number of
 *  messages sent so far to the remote connection.
 */
GS_EXPORT NSString* const NSConnectionRequestsSent;
/* These Are GNUstep extras */

/**
 *  GNUstep-specific key for dictionary returned by [EIConnection-statistics]:
 *  number of local objects currently in use remotely.
 */
GS_EXPORT NSString* const NSConnectionLocalCount;   /* Objects sent out */

/**
 *  GNUstep-specific key for dictionary returned by [EIConnection-statistics]:
 *  number of remote objects currently in use.
 */
GS_EXPORT NSString* const NSConnectionProxyCount;   /* Objects received */

#define USE_DEBUG_NAMES

typedef NS_ENUM(UInt32,ETConnectionTypes)
{
    /* undefined */
    ETConnectionTypeUndefined = 0,
    /* Root connection */
    ETConnectionTypeRoot,
    /* Service connection */
    ETConnectionTypeService,
    /* Remote connection */
    ETConnectionTypeProxy
};


#pragma mark - EIConnection -

@interface EIConnection : NSObject
#pragma mark Transport Layer
/*
 *  we might implemenent a clone protocol, which allows a 'duplicate' root
 *  object once a connection is made - given, this does not work with root class ...
 *  
 */

- (BOOL)stopVending;
+ (id)startVending:(NSString *)name rootObject:(id)root;

/**
 *  initializes a new child connection for a vended object ...
 */
- (id) initWithParent:(EIConnection*)parent andNetConnection:(EINetConnection*)netConnection;

/**
 *  called by EINetConnection, if a new port connect message has been received. The EINetConnection
 *  expects a child connection to be created, which will act as message handler for
 *  incoming messages. The netConnection passed will be responsible toi handle all traffic
 *  for this EIConnection.
 */
- (EIConnection*) connectRequest:(EINetConnection*)netConnection;
/*  An ETConnection may be of several types:
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

#pragma mark Coding Layer

#pragma mark Management Layer


#pragma mark UNSORTED ENTRIES:
- (void)addRequestMode:(NSString*)mode;
- (void)addRunLoop:(NSRunLoop*)loop;
- (NSDictionary*)statistics;

#pragma mark NOTIFICATION HANDLING:

#pragma mark INIT & DEALLOC
- (void)invalidate;
- (BOOL)isValid;

#pragma mark INSTANCE PROPERTIES
@property (nonatomic, retain) id rootObject;
@property (nonatomic, retain) id<EIConnectionDelegate> delegate;

@property (nonatomic, assign) BOOL multipleThreadsEnabled;
@property (nonatomic, assign) ETServerPlatformType platformType;

#ifdef USE_DEBUG_NAMES
@property (nonatomic, retain) NSString* connName;
#endif //#ifdef USE_DEBUG_NAMES
/** DEBUG: type of the connection */
@property (nonatomic, assign) ETConnectionTypes connType;

//@property (nonatomic, assign) NSTimeInterval requestTimeout;
//@property (nonatomic, assign) NSTimeInterval replyTimeout;

#pragma mark PROXY STUFF
- (EIDistantObject*)rootProxy;

#pragma mark CLASS METHODS

/**
 * Returns an array containing all the EIConnection objects known to
 * the system. These connections will be valid at the time that the
 * array was created, but may be invalidated by other threads
 * before you get to examine the array.
 */
+ (NSArray*)allConnections;
+ (void) dumpAll;

/**
 * Returns a connection initialised using -initWithReceivePort:sendPort:<br />
 * Both ports must be of the same type.
 */
+ (EIConnection*)connectionWithReceivePort:(NSPort*)r sendPort:(NSPort*)s;

/**
 * <p>Returns an EIConnection object whose send port is that of the
 * EIConnection registered under the name n on the host h
 * </p>
 * <p>This method calls +connectionWithRegisteredName:host:usingNameServer:
 * using the default system name server.
 * </p>
 * <p>Use [ETConnectServer] for connections to remote hosts.
 * </p>
 */
+ (EIConnection*)connectionWithRegisteredName:(NSString*)n host:(NSString*)h;

/**
 * <p>
 *   Returns an EIConnection object whose send port is that of the
 *   EIConnection registered under <em>name</em> on <em>host</em>.
 * </p>
 * <p>
 *   The nameserver <em>server</em> is used to look up the send
 *   port to be used for the connection.<br />
 *   Use [ETConnectServer+sharedInstance]
 *   for connections to remote hosts.
 * </p>
 * <p>
 *   If <em>host</em> is <code>nil</code> or an empty string,
 *   the host is taken to be the local machine.<br />
 *   If it is an asterisk ('*') then the nameserver checks all
 *   hosts on the local subnet (unless the nameserver is one
 *   that only manages local ports).<br />
 *   In the GNUstep implementation, the local host is searched before
 *   any other hosts.<br />
 *   NB. if the nameserver does not support connections to remote hosts
 *   (the default situation) the host argument should be omitted.
 * </p>
 * <p>
 *   If no EIConnection can be found for <em>name</em> and
 *   <em>host</em>host, the method returns <code>nil</code>.
 * </p>
 * <p>
 *   The returned object has the default EIConnection of the
 *   current thread as its parent (it has the same receive port
 *   as the default connection).
 * </p>
 */
+ (EIConnection*)connectionWithRegisteredName:(NSString*)n
                                         host:(NSString*)h
                              usingNameServer:(ETConnectServer*)s;

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


+ (id)serviceConnectionWithName:(NSString *)name
                     rootObject:(id)root;

+ (id)serviceConnectionWithName:(NSString *)name
                     rootObject:(id)root
                usingNameServer:(ETConnectServer *)server;

/**
 * Sets the EIConnection configuration so that multiple threads may
 * use the connection to send requests to the remote connection.<br />
 * This option is inherited by child connections.<br />
 * NB. A connection with multiple threads enabled will run slower than
 * a normal connection.
 */
- (void)enableMultipleThreads;

/**
 * Return the current conversation ... not implemented in GNUstep
 */
+ (id)currentConversation;

/**
 *  called, once an EINetConnections becomes invalid
 */
- (void)connectionIsInvalid:(EINetConnection*)netConnection;

/** Debug only:
 *  sets/gets the common debug level for all connection objects...
 */
+ (int) setDebug:(int)val;
+ (int) getDebug;

@end

@interface EIConnection (MessageHandling)
/*
 *  an incoming net message is dispatched from an EINetConnection
 */
- (void)handleNetMessage:(EINetMessage*)msg;
@end


#pragma mark - EIConnectionDelegate
@protocol EIConnectionDelegate <NSObject>
@optional

// Use the NSConnectionDidInitializeNotification notification instead
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


