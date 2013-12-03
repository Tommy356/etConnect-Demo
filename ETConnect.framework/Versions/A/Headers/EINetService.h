//
//  EINetService.h
//  EIConnectionIOS
//
//  Created by Thomas Krautter on 11.10.13.
//  Copyright (c) 2013 Thomas Krautter. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "EINetConnection.h"

#define EIDefResponsePortName   @"__defResponsePort"

@class EIConnection, EINetService, EINetConnection, EINetMessage, EISocketPort;

/** this is the main protocol for bound services.
 *  The delegate will be established with a call to
 *  bindDelegate: of the NSNetService instance
 */
@protocol EINetServiceDelegate <NSObject>
@optional
/** a (remote) service did resolve and retrieve the remote dict for the service */
- (void) eiSvc:(EINetService*)sender didResolveWithDict:(NSDictionary*)remoteDict;
- (void) service:(EINetService*)sender hasReceived:(EINetMessage*)msg withConnection:(EINetConnection*)connection;
/** a (remote) service did update its record data */
- (void) eiSvc:(EINetService*)sender didUpdateTXTRecordDict:(NSDictionary*)remoteDict;
@end

@interface EINetService : NSObject
<NSNetServiceDelegate>
{
    BOOL _isOwnService;
    BOOL _isResolving;
    BOOL _isResolved;
    
    // Thread Start parameters
    EINetResponseHandler    __reponseHandler;
    EINetMessage*           __msg;
}

- (void) dumpAll;

#pragma mark - INITIALISATION
- (void) dealloc;
- (id) init;
- (NSString*) description;
/** binds the associate service to the original service */
- (void) bindTo:(NSNetService*)netService;

#pragma mark - CONNECTION MANAGEMENT
- (void) addConnection:(EINetConnection*)connection;
- (void) removeConnection:(EINetConnection*)connection;

-(BOOL) acceptInConnection:(BOOL)isInput inputStream:(NSInputStream *)istr outputStream:(NSOutputStream *)ostr;
-(void) connectionClosed:(EINetConnection*)connection;
-(void) connectionHasHandledResponse:(EINetConnection*)connection message:(EINetMessage*)msg;
-(void) connectionHasReceivedData:(EINetConnection*)connection message:(EINetMessage*)msg;

#pragma mark - GENERAL INFORMATION
- (NSDictionary*) serviceNameDict;

#pragma mark - ROOT CONNECTION MANAGEMENT
- (EIConnection*) rootConnectionForServiceName:(NSString*)serviceName;
- (BOOL) addRootConnection:(EIConnection*)conn withName:(NSString*)serviceName;
- (BOOL) removeRootConnection:(EIConnection*)conn withName:(NSString*)serviceName;
/* establishes & returns a new net connection to a service on a remote host */
- (EINetConnection*)connectionForService:(NSString *)name host:(NSString *)host;

#pragma mark - Thread scheduling
+ (NSThread *)networkThread;

#pragma mark - send & receive - direct sockets
- (BOOL) sendMessage:(EINetMessage*)msg;
- (EINetMessage*) sendRequest:(EINetMessage*)msg;

#pragma mark - send & receive
- (BOOL) sendDictionary:(NSDictionary*)dict;
- (BOOL) sendString:(NSString*)string;
- (BOOL) sendData:(NSData*)data;

- (BOOL) sendMessage:(EINetMessage*)msg withResponseHandler:(void (^)(EINetMessage* rsp))responseHandler;

/** deprecated methods: */
- (BOOL) sendRequest:(EINetMessage*)rqMessage withResponseHandler:(void (^)(EINetMessage* responseMsg))responseHandler;
- (NSTimeInterval) sendTimesyncRequest;

@property (nonatomic, retain) NSTimer* resolveTimer;

/** the port dictionary for this service */
@property (nonatomic, retain) NSMutableDictionary* portDict;

/** current connections of this service */
@property (nonatomic, retain) NSMutableArray* arConnections;

/** for remoter services, this holds the dictionary of the remote service,
 *  for the local service, we store all vended connections and their names
 */
@property (nonatomic, retain) NSDictionary* remoteDict;
@property (nonatomic, retain) NSMutableDictionary* localDict;

/** underlying service and root delegate */
@property (nonatomic, assign) NSNetService* netService;
@property (nonatomic, retain) id<EINetServiceDelegate> delegate;

@end

/** additions to the original NSNetService, mainly used to
 *  bind an instance to an associate EINetService object and
 *  to retrieve this object.
 */
@interface NSNetService (EIAdditions)

- (void) bindDelegate:(id<EINetServiceDelegate>)delegate;
- (NSDictionary*) txtRecordDict;

@property (nonatomic, assign, readonly) EINetService* eiNetService;


#pragma mark ASSOCIATE PASSTHROUGH
-(BOOL) acceptInConnection:(BOOL)isInput inputStream:(NSInputStream *)istr outputStream:(NSOutputStream *)ostr;

@end
