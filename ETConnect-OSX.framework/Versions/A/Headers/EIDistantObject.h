/* Interface for GNU Objective-C version of EIDistantObject
   Copyright (C) 1997 Free Software Foundation, Inc.

   Written by:             Thomas Krautter <t.krautter@eitap.de>
   Inspired from code by:  Andrew Kachites McCallum <mccallum@gnu.ai.mit.edu>
                           Richard Frith-Macdonald <richard@brainstorm.co.uk>
 
   Created: October 2013

   This file is part of the ETConnect framework.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02111 USA.
 */

#ifndef __EIDistantObject_h_INCLUDE
#define __EIDistantObject_h_INCLUDE

#import <Foundation/NSProxy.h>

#if defined(__cplusplus)
extern "C" {
#endif

@class EIConnection;

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

/**
 * Creates and returns a proxy associated with aConnection
 * which will hold a reference to the local object anObject.
 */
+ (EIDistantObject*)proxyWithLocal:(id)anObject connection:(EIConnection*)aConnection;

/*
 *	NB. Departure from the OpenStep/MacOS spec - the type of a target
 *	is an integer, not an id, since we can't safely pass id's
 *	between address spaces on machines with different pointer sizes.
 */
+ (EIDistantObject*)proxyWithTarget:(unsigned)anObject connection:(EIConnection*)aConnection;

/** returns the EIConnection the proxy is connected to */
- (EIConnection*)connectionForProxy;

- (id)initWithLocal:(id)anObject connection:(EIConnection*)aConnection;
- (id)initWithTarget:(unsigned)target connection:(EIConnection*)aConnection;
- (NSMethodSignature*)methodSignatureForSelector:(SEL)aSelector;
- (void)setProtocolForProxy:(Protocol*)aProtocol;

/* platform encoding: returns a combination of the values defined above */
@property (nonatomic, readonly) ETServerPlatformType platformType;

@property (nonatomic, retain) NSString* proxyName;


#pragma mark iOS Additions (start)
- (id) awakeAfterUsingCoder:(NSCoder*)coder;
#pragma mark iOS Additions (end)

@end

@interface EIDistantObject (Debug)
+ (int)setDebug:(int)val;
+ (int)getDebug;
@end

#if defined(__cplusplus)
}
#endif

#endif /* __EIDistantObject_h_INCLUDE */
