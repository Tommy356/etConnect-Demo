//
//  NSData+EIConnection.h
//  Recipes
//
//  Created by Thomas Krautter on 12.01.13.
//
//

#import <Foundation/Foundation.h>
#import <arpa/inet.h>

@interface NSData (EIConnection)
+ (NSData*)dataWithSocket:(const struct sockaddr*)addr;
- (int)family;
- (int)port;
- (NSString *)host;
- (BOOL)isIP4Addr;
@end
