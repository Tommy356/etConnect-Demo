//
//  NSNetService+EIConnection.h
//  Recipes
//
//  Created by Thomas Krautter on 12.01.13.
//
//

#import <Foundation/Foundation.h>

@interface NSNetService (CANetAgentExt)
- (NSData*)ip4Addr;
- (NSString*)address;
- (NSArray*)ip4Addresses;
@end
