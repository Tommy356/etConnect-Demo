//
//  EINetConnection.h
//  EIConnectionIOS
//
//  Created by Thomas Krautter on 13.10.13.
//  Copyright (c) 2013 Thomas Krautter. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "EINetMessage.h"
#import "ETConnectTypes.h"

@class EIConnection, EINetService, EINetMessage, EISocketPort;

/** defines a progress handler block */
typedef void (^EINetResponseHandler)(EINetMessage* responseMsg);

/** the timeout value for document processes, initialized by default to 60 sec,
 if set to 0, timeouts will not be detected */
#define _defRequestTimeoutInterval 60
#define _defRequestTimeoutDelay 15

/** a single service may hold several connections.
 this interface is created any time, a (registered) service receives
 a request for a connection from a new device. The connection will
 handle all communication requests.
 */

#define ConnectionLockClass NSLock

#ifdef OBSOLETE
/**
 *  a protocol which allows PCB clients to respond to port connect
 *  requests...
 */
@protocol EINetConnectionDelegate <NSObject>
- (BOOL) portConnectRequest:(EINetConnection*)connection recv:(EISocketPort*)recvPort send:(EISocketPort*)sendPort;
@end
#endif // OBSOLETE

@protocol EINetConnectionInStreamDelegate <NSObject>
/* passes up a rtange where length holds the number of bytes expected, and location
 * indicates the number of bytes actually read */
- (void) connectionDidRead:(NSRange)dataRange;
@end

@interface EINetConnection : NSNetService
<NSStreamDelegate>
{
@public
    // Locking ...
    ConnectionLockClass*    _bufLock;
    
    // Stream handling
	NSInputStream*      _inStream;
	NSOutputStream*     _outStream;
    
    NSRunLoop*          _streamRunLoop;
    
    
    BOOL                _open;
	BOOL				_inReady;
	BOOL				_outReady;
    
    /** this is set TO YES, if we own a client connection, which was created
     *  through a remote client request. If - and only if - this is set to
     *  TRUE, the client connection is locked by this thread.
     */
    BOOL                _clientOwner;
    /** set to TRUE, if our streams are opened and we're ready to
     *  send & receive
     */
    BOOL                _isValid;
    
    /** array with all object to be sent. If the send buffer is running out of space,
     it's reloaded with the data of the next message object */
    NSMutableArray*     arSendBuffer;
    
    /** NSData object with the current object data and the current send status */
    NSData*             _currentData;
    /** # of bytes already send from the current buffer */
    NSUInteger          _nBytesSent;
    /** # of bytes left in the current send buffer */
    NSUInteger          _nBytesLeft;
    
    /** response handler, in case we've sent a message which requires a response */
    EINetResponseHandler  _reponseHandler;
    
    /** if a response is expected, we'll start a timer for the case of no response... */
    NSTimer*            _rspTimer;
}

- (void) dumpAll;

/* The etConnection responsible to handle incoming messages. This entry
 * will be validated after receiving a handshake message requesting
 * a specific service.
 * TK, 21.11.2013:
 * make the reference to EIClientConnection weak to reduce the chance
 * of cross-locks. Actually, the EIConnection owns this, so we don't
 * need to lock it twice.
 */
@property (nonatomic, assign) EIConnection* clientConnection;

/* called, when the parent service invalidates */
- (void)netServiceInvalidates:(EINetService*)serice;
/* called, when a client connection invalidates */
- (void)clientConnectionInvalidates:(EIConnection*)clientConnection;

/** a delegate to receive notifications on lengthy messages */
@property (nonatomic, retain) id <EINetConnectionInStreamDelegate> inStreamDelegate;


/** TRUE, if the connection was established by another client */
@property (nonatomic, assign) BOOL isPortConnection;
/** TRUE, if the connection was established by another client */
@property (nonatomic, assign) BOOL isIncoming;
/** TRUE, if the connection should close after all bytes had been sent */
@property (nonatomic, assign) BOOL autoclose;

/** the service, which had established the connection */
@property (nonatomic, assign) EINetService* service;

/** the service, responsible to serve incoming requests */
@property (nonatomic, assign) EINetService* localService;

/** a message object to receive input data */
@property (nonatomic, retain) EINetMessage* inMsg;

/** a message object to receive input data */
@property (nonatomic, assign) ETServerPlatformType platformType;

/** served ports dictionary */
//@property (nonatomic, retain) NSMutableArray* connectedPorts;


- (void) closeStreams;
- (void) openStreams;
- (BOOL) isValid;
- (id) initWithStreams:(NSInputStream *)istr outputStream:(NSOutputStream *)ostr;
- (void) sendRequestMessage:(EINetMessage*)msg withResponseHandler:(EINetResponseHandler)responseHandler;

- (void) sendMessage:(EINetMessage*)msg;
- (void) sendMessage:(EINetMessage*)msg autoclose:(BOOL)autoclose;
- (void) sendObject:(id)obj;

@end

#pragma mark - PORT BASED COMMUNICATION MANAGEMENT (PBC)
@interface EINetConnection (PortBasedCommunication)
//- (void) addConnectedPort:(EISocketPort*)port;
- (void) handlePortMessage:(EINetMessage*)msg;
- (void) handlePortConnectResponse:(EINetMessage*)msg;
- (void) handlePortConnectRequest:(EINetMessage*)msg;
@end
