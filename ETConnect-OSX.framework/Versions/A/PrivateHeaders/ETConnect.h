//
//  ETConnect.h
//  Framework-EIConnect
//
//  Created by Thomas Krautter on 23.10.13.
//  Copyright (c) 2013 Thomas Krautter. All rights reserved.
//
//  Main Framework Header File - Includes all public classes required

#ifndef Framework_EIConnect_EIConnect_h
#define Framework_EIConnect_EIConnect_h

#if TARGET_OS_IPHONE==0
#import <ETConnect-OSX/ETConnectServer.h>
#import <ETConnect-OSX/EINetService.h>
#import <ETConnect-OSX/EIConnection.h>
#import <ETConnect-OSX/EIDistantObject.h>
#import <ETConnect-OSX/ETHelpers.h>
#else
#import <ETConnect/ETConnectServer.h>
//#import <ETConnect/EINetService.h>
#import <ETConnect/EIConnection.h>
#import <ETConnect/EIDistantObject.h>
//#import <ETConnect/ETHelpers.h>
#endif
#endif
