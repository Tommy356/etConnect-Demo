//
//  ETConnectTypes.h
//  ETConnect
//
//  Created by Thomas Krautter on 09.11.13.
//  Copyright (c) 2013 eitap Technologies GmbH. All rights reserved.
//

#ifndef ETConnect_ETConnectTypes_h
#define ETConnect_ETConnectTypes_h

typedef NS_OPTIONS(UInt32, ETServerPlatformType)
{
    ETServerPlatformType_16Bit    = ( 1 <<  0),
    ETServerPlatformType_32Bit    = ( 1 <<  1),
    ETServerPlatformType_64Bit    = ( 1 <<  2),

    ETServerPlatformType_iOS      = ( 1 <<  4),
    ETServerPlatformType_OSX      = ( 1 <<  5),
};

#define ETSameCoding(pt)    ( (pt & 0x0F) == ([ETConnectServer platformType] & 0x0F) )

#endif /* ETConnect_ETConnectTypes_h */
