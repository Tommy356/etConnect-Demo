//
//  EIDistantObject.h
//
//  etConnect Framework
//
//  Created by Thomas Krautter on 06.10.13.
//  Copyright (c) 2013 Thomas Krautter. All rights reserved.
//
//
//   Written by:             Thomas Krautter <t.krautter@eitap.de>
//   Inspired from code by:  Andrew Kachites McCallum <mccallum@gnu.ai.mit.edu>
//                           Richard Frith-Macdonald <richard@brainstorm.co.uk>
// 
//   Created: October 2013
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

#ifndef __EIDistantObject_h_INCLUDE
#define __EIDistantObject_h_INCLUDE

#import <Foundation/NSProxy.h>

#if defined(__cplusplus)
extern "C" {
#endif

@class EIConnection;
    
/** A Distant Object is a placeholder for a Proxy provided by a remote host. Typically, you won't create
 *  instances of this class directly, but use the routines provided by the EIConnection class to either publish
 *  local objects or connect to remote objects published by other hosts.
 */
@interface EIDistantObject : NSProxy <NSCoding>
{
@public
    EIConnection  *_connection;
    id _object;
    unsigned _handle;
    unsigned _counter;
}

+ (id)allocWithZone:(NSZone*)z;
+ (id)allocWithZoneExt:(NSZone*)z;

/** Creates and returns a proxy associated with aConnection which will hold a reference to the local object anObject.
 *
 *  This routine is used internally, to create proxies for objects returned upon remote requests.
 */
+ (EIDistantObject*)proxyWithLocal:(id)anObject connection:(EIConnection*)aConnection;

/** Creates and returns a proxy for a target object.
 *
 *  This routine is used internally, to create proxies for objects returned upon remote requests.
 *	Departure from the OpenStep/MacOS spec - the type of a target is an integer, not an id, since we can't safely pass id's
 *	between address spaces on machines with different pointer sizes.
 */
+ (EIDistantObject*)proxyWithTarget:(unsigned)anObject connection:(EIConnection*)aConnection;

/** returns the EIConnection the proxy is connected to */
- (EIConnection*)connectionForProxy;

/* etConnect internal useage */
- (id)initWithLocal:(id)anObject connection:(EIConnection*)aConnection;
- (id)initWithTarget:(unsigned)target connection:(EIConnection*)aConnection;
- (NSMethodSignature*)methodSignatureForSelector:(SEL)aSelector;

/** Sets a protocol for a proxy retrieved from a remote host.
 *
 *  The usage of protocols is highly recommended to speed up network communication and to enable safe coding
 *  of data structures and NSInteger/NSUInteger parameters between 32-bit and 64-bit platforms.
 *  NOTE, that cross-platform calls using such parameters will require a protocol to be set; otherwise
 *  an exception will be raised.
 *
 *  For a more intense dicussion on this topic please refer to the etConnect framework documentation.
 */
- (void)setProtocolForProxy:(Protocol*)aProtocol;

/** Platform encoding: returns a combination of the values defined above 
 *  The platform types are used to differentiate between the coding of values (i.e. NSInteger/NSUInteger)
 *  which have different sizes on 32/64 bit platforms. The following definitions are available:
 *
 * - ETServerPlatformType_16Bit    = ( 1 <<  0),
 * - ETServerPlatformType_32Bit    = ( 1 <<  1),
 * - ETServerPlatformType_64Bit    = ( 1 <<  2),
 * - ETServerPlatformType_iOS      = ( 1 <<  4),
 * - ETServerPlatformType_OSX      = ( 1 <<  5),
 *
 *  Remote calls to different platform types will usaually work withoput a protocol, if only objects are passed
 *  as parameters. Structure- and NSInteger/NSUInteger parameters are refused without a defined protocol.
 */
@property (nonatomic, readonly) ETServerPlatformType platformType;

/** Optional property to assign a name to a well known proxy.
 */
@property (nonatomic, retain) NSString* proxyName;


- (id) awakeAfterUsingCoder:(NSCoder*)coder;

@end

@interface EIDistantObject (Debug)
+ (int)setDebug:(int)val;
+ (int)getDebug;
@end

#if defined(__cplusplus)
}
#endif

#endif /* __EIDistantObject_h_INCLUDE */
