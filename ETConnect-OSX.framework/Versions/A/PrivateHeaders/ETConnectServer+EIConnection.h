//
//  ETConnectServer+EIConnection.h
//  EIConnectionIOS
//
//  Created by Thomas Krautter on 14.10.13.
//  Copyright (c) 2013 Thomas Krautter. All rights reserved.
//

#import "ETConnectServer.h"

@class EIConnection;

@interface ETConnectServer (EIConnection)

- (BOOL) publish:(EIConnection*)connection;

@end
