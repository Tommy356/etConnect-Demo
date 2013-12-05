//
//  EIPortCoder.h
//  Framework-EIConnect
//
//  Created by Thomas Krautter on 26.10.13.
//  Copyright (c) 2013 Thomas Krautter. All rights reserved.
//

#import <Foundation/Foundation.h>
#if TARGET_OS_IPHONE==1
#import "NSPortCoderOSX.h"
#define NSPortCoder NSPortCoderOSX
#else
#import "NSPortCoderOSX.h"
#endif //#if TARGET_OS_IPHONE==1
#import "EIConnection.h"

#if TARGET_OS_IPHONE==1
@interface EIPortCoder : NSPortCoderOSX
#else
@interface EIPortCoder : NSPortCoderOSX
#endif //#if TARGET_OS_IPHONE==1

@property (nonatomic, retain) EIConnection* xconn;

/** APPLE comments:
 *
 *  The following methods are deprecated.
 *  Instead of using these methods, NSPort subclasses should look up
 *  an NSConnection object using
 *
 *      +connectionWithReceivePort:sendPort:
 *
 *  and then ask that object to dispatch recieved component data using
 *
 *      -dispatchWithComponents:.
 *
 *  The NSConnection object will take care of creating the right kind of NSPortCoder
 *  object and giving it the data to decode and dispatch.
 */

/**
 *  creates an EIPortCoder for a connection (using the connection's ports),
 *  optionally with the components supplied.
 */
+ (EIPortCoder*)portCoderForConnection:(EIConnection*)connection withComponents:(NSArray*)comp;

/**
 * Returns the <code>EIConnection</code> using this instance.
 */
- (EIConnection*)coderConnection;

/**
 * Processes and acts upon the initial message the receiver was initialized
 * with..
 */
- (void)dispatch;

/**
 * Initialize a new instance for communications over send and recv, and send an
 * initial message through send as specified by comp.
 */
- (id)initWithReceivePortExt:(NSPort*)recv
                 sendPort:(NSPort*)send
               components:(NSArray*)comp;

@end

/**
 * This informal protocol allows an object to control the details of how an
 * object is sent over the wire in distributed objects Port communications.
 */
@interface NSObject (NSPortCoder)
/**
 *	Must return the class that will be created on the remote side
 *	of the connection.  If the class to be created is not the same
 *	as that of the object returned by replacementObjectForPortCoder:
 *	then the class must be capable of recognising the object it
 *	actually gets in its initWithCoder: method.
 *	The default operation is to return EIDistantObject unless the
 *	object is being sent bycopy, in which case the objects actual
 *	class is returned.  To force bycopy operation the object should
 *	return its own class.
 */
- (Class)classForPortCoder;

/**
 *	This message is sent to an object about to be encoded for sending
 *	over the wire.  The default action is to return an EIDistantObject
 *	which is a local proxy for the object unless the object is being
 *	sent bycopy, in which case the actual object is returned.
 *	To force bycopy, an object should return itself.
 */
- (id)replacementObjectForPortCoder:(EIPortCoder*)aCoder;

@end


