//
//  EINetConnection.h
//
//  etConnect Framework
//
//  Created by Thomas Krautter on 06.10.13.
//  Copyright (c) 2013 Thomas Krautter. All rights reserved.
//
//   Created: Juli 2013
//
//   This file is part of the ETConnect framework.
//
//   This library is free software; you can redistribute it and/or
//   modify it under the terms of the GNU Lesser General Public
//   License as published by the Free Software Foundation; either
//   version 2 of the License, or (at your option) any later version.
//
//   This library is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//   Library General Public License for more details.
//
//   You should have received a copy of the GNU Lesser General Public
//   License along with this library; if not, write to the Free
//   Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
//   Boston, MA 02111 USA.
//

#import <Foundation/Foundation.h>
#import "EINetMessage.h"
#import "ETConnectTypes.h"

@class EIConnection, EINetService, EINetMessage;

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

/** The EINetConnectionInStreamDelegate protocol allows the monitoring of network transport
 *  layer. You'll use this to get notifications on the network transport progress while calling remote
 *  methods that generate a large amount of data (i.e. transferring images or sound files).
 *  Typically you would set a delegate before calling such a method on the client side and implement
 *  the connectionDidRead: method to get notified about the data to be transmitted.
 *  It is NOT recommended to generally set a delegate, as this methods adds additional overhead
 *  to the time-critical network tranbsport.
 */
@protocol EINetConnectionInStreamDelegate <NSObject>
/** Passes up a range where length holds the number of bytes expected, and location
 *  indicates the number of bytes actually read.
 *  You may use this to implement a progress indicator while transferring large data objects
 *  from a remote host.
 @param dataRange: the length value of the range holds the total number of bytes expected to be received; the location member holds the amount of bytes actually received. 
 */
- (void) connectionDidRead:(NSRange)dataRange;
@end

/** This class provides the main functionality communicate with remote hosts.
 *
 *  There's no need to create instances of this class on your own. Instances of this class are generated automatically
 *  for you whenever a remote procedure is called or a remote hosts calls methods on a vended proxy. EINetConnections
 *  are tightly connected to EIConnection and EINetService, handling all communication requirements automatically. 
 *  Thus, once using vended objects over EIConnection, there's no need at all to take care of EINetConnections.
 *
 *  However, beside the etConnect services, you may use the functionality provided by EINetService/EINetConnection to set
 *  up your own messaging scheme beside the default Objective-C++ messaging. Directly using an EINetConnection
 *  allwos you to directly transfer data from a client to a remote host. But, again, there should be absolutely no need 
 *  to create this overhead work, because there's hardly any speed- or memory advantage compared to 
 *  implementing any protocol by using appropriate remote objects.
 *
 *  An EINetConnection replaces the send/receive port architecture found on other DO implementations by using
 *  NSStreams to read and write data and to monitor the status of a remote host. 
 *
 */
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

/** The EIConnection instance responsible to handle incoming messages. This entry
 * will be validated after receiving a handshake message requesting a specific service.
 */
@property (nonatomic, assign) EIConnection* clientConnection;

/** called, when the parent service invalidates */
- (void)netServiceInvalidates:(EINetService*)serice;

/** called, when a client connection invalidates */
- (void)clientConnectionInvalidates:(EIConnection*)clientConnection;

/** A delegate to receive notifications on lengthy messages.
 *
 *  You may (temporarily) set a delegate to monitor the transmission status of
 *  lengths messages. See also: EINetConnectionInStreamDelegate protocol description.
 */
@property (nonatomic, retain) id <EINetConnectionInStreamDelegate> inStreamDelegate;

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


- (void) closeStreams;
- (void) openStreams;
- (BOOL) isValid;
- (id) initWithStreams:(NSInputStream *)istr outputStream:(NSOutputStream *)ostr;

/** Sends an EINetMessage to the remote host and calls the rsponse handler block after
 *  the remote hosts has reponded.
 *
 *  You may use the EINetMessage interface to compose a message and pass it to a remote
 *  hosts.
 */
- (void) sendRequestMessage:(EINetMessage*)msg withResponseHandler:(EINetResponseHandler)responseHandler;

/** Sends an EINetMessage to the remote host without waiting for a reply.
 *
 *  You may use the EINetMessage interface to compose a message and pass it to a remote
 *  hosts.
 */
- (void) sendMessage:(EINetMessage*)msg;

/** Sends an EINetMessage to the remote host without waiting for a reply and closes the
 *  connection automatically, after all message data have been transferred.
 *
 *  You may use the EINetMessage interface to compose a message and pass it to a remote
 *  hosts.
 */
- (void) sendMessage:(EINetMessage*)msg autoclose:(BOOL)autoclose;

/** Low level routine to send an object of type NSString, NSDictionary, or NSData to a
 *  remote host.
 */
- (void) sendObject:(id)obj;

@end

