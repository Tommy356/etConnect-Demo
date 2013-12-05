//
//  EINetMessage.h
//  EIConnectionIOS
//
//  Created by Thomas Krautter on 13.10.13.
//  Copyright (c) 2013 Thomas Krautter. All rights reserved.
//

#import <Foundation/Foundation.h>

@class EINetConnection;

/** net message header signature ('zyxg') */
#define EINMSG_MAGIC                0x7A797867
/** current protocol version; previous versions are not uspported */
#define EINMSG_PROTOCOL_VERSION     0x0200

typedef NS_ENUM(UInt16, EINetMsgType)
{
    EINetMsgTypeUndefined           = 0x0000,
    EINetMsgTypeEcho                = 0x0001,
    EINetMsgTypeRequest             = 0x0002,
    EINetMsgTypeResponse            = 0x0003,
    
    EINetMsgTypePortConnectRequest  = 0x0004,
    EINetMsgTypePortConnectResponse = 0x0005,
    
    EINetMsgTypePortMessage         = 0x0006,

    /* we also need to inform remotres, if a connection is not required any more */
    EINetMsgTypePortReleaseRequest  = 0x0007,
    EINetMsgTypePortReleaseResponse = 0x0008,
};

typedef NS_OPTIONS(UInt16, EINetMsgFlags)
{
    EINetMsgFlagsNone           = 0x0000,
    EINetMsgNeedResponse        = 0x0100,
    
    /** if this flag is set, the private message type entries in
     *  the header will identify the sending/receiving ports
     *  of the net service responsible for the connection
     */
    EINetMsgIsPortMessage       = 0x0200,
};

typedef NS_ENUM(UInt16, EINetMsgDataType)
{
    EINetMsgDataTypeUndefined   = 0,    //  invalid message type
    EINetMsgDataTypeRaw         = 1,    //  just message body
    EINetMsgDataTypeNSData      = 2,    //  NSData appended
    EINetMsgDataTypeNSDict      = 3,    //  NSDictionary appended
    EINetMsgDataTypeNSString    = 4,    //  NSString (UTF8 encoded) appended
    EINetMsgDataTypeComponents  = 5,    //  An array of NSData
};


/** Data header or all messages */
struct _canm_hdr {
    /** 4 bytes header data ("zyxg") */
    UInt32              magic;
    /** 2 Bytes header length == sizeof(EINMHdr) */
    UInt16              hdrLength;
    /** set to EINMSG_PROTOCOL_VERSION */
    uint16_t            protocol_version;
    /** PROTOCOL FLAGS, see above */
    EINetMsgFlags       protocol_flags;
    /** EINetMsgDataType, see above */
    EINetMsgDataType    data_type;
    
    /** length of the data plus a checksum */
    uint32_t            data_length;
    uint16_t            data_checksum;
    
    /** may be set to a private message type - values are app dependent 
     *  when used with port messages, these entries will have special meanings
     */
    EINetMsgType      message_type;
    UInt32            res1;
    UInt32            res2;
};

typedef struct _canm_hdr    EINMHdr;

/*
 *  Message progress block: might be used on lengthy messages
 *  to get feedback ...
 */
typedef void (^NetMsgProgressBlock)(NSRange rProg);


@interface EINetMessage : NSObject //NSNetService
{
@public
    EINMHdr     hdr;
    BOOL        bInputMessage;
    UInt32      _intMsgID;
}

#pragma mark Initialization
-(id) init;
-(id) initWithData:(NSData*)data;
-(id) initWithDictionary:(NSDictionary*)dict;
-(id) initWithString:(NSString*)string;
-(id) initFromStream:(NSInputStream*)_inStream;
-(id) initForInput;

#pragma mark DATA SETTERS
- (BOOL) setData:(NSData*)data;
- (BOOL) setComponents:(NSArray*)components;
- (BOOL) setString:(NSString*)string;
- (BOOL) setDictionary:(NSDictionary*)dict;

#pragma mark Data Retrieval
- (EINetMsgDataType) dataType;

//- (NSData*) getMessageData;
- (NSData*) getMessageData;
- (NSArray*) getMessageComponents;
- (NSDictionary*) getMessageDict;
- (NSString*) getMessageString;

- (BOOL) readFromStream:(NSInputStream*)_inStream;
-(BOOL) readFromStream:(NSInputStream*)_inStream progress:(NetMsgProgressBlock)progBlock;
- (BOOL) isCompleted;

#pragma mark Message Creation
+(EINetMessage*) netMessageWithString:(NSString*)string;
+(EINetMessage*) netMessageWithData:(NSData*)data;
+(EINetMessage*) netMessageWithDictionary:(NSDictionary*)dict;
+(EINetMessage*) netMessageOfType:(EINetMsgType)messageType;
+(EINetMessage*) netMessageOfType:(EINetMsgType)messageType res1:(UInt32)res1;
+(EINetMessage*) netMessageOfType:(EINetMsgType)messageType res1:(UInt32)res1 res2:(UInt32)res2;
+(EINetMessage*) messageFromInputStream:(NSInputStream*)_inStream;
/** creates a new message object, which will be used to read data from an input stream */
+(EINetMessage*) messageForInput;

#pragma mark Properties
/** the connection, that received the message (weak, for input messages only */
@property (nonatomic, assign) EINetConnection* connection;
/** the fully coded message data */
@property (nonatomic, retain)   NSMutableData* msgData;
/** a complete NSData object, holding the message and all associated data */
@property (nonatomic, readonly) NSData* messageData;

@end
