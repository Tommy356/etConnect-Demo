//
//  EIConnectionServer+EIConnection.h
//  EIConnectionIOS
//
//  Created by Thomas Krautter on 14.10.13.
//  Copyright (c) 2013 Thomas Krautter. All rights reserved.
//

#import "EIConnectionServer.h"

@class EIConnection;

@interface EIConnectionServer (EIConnection)

- (BOOL) publish:(EIConnection*)connection;

@end
