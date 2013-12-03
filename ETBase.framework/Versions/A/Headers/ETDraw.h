//
//  ETDraw.h
//  ETBase framework
//
//  Created by Thomas Krautter on 14.09.11.
//  Copyright (c) 2011 TK Consulting, Thomas Krautter. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "UIColor-ETBase.h"

@interface NSDraw : NSObject
{
    CGContextRef    ctx;
}

- (void) initWithContext:(CGContextRef)context;

+ (CGPathRef)createRoundRectPath:(CGRect)rect radius:(CGFloat)radius;
+ (CGPathRef)createRoundRectPath:(CGRect)rect; 
+ (void) drawRoundRect:(CGContextRef) ctx rect:(CGRect)rect;
+ (void) drawRoundRect:(CGContextRef) ctx rect:(CGRect)rect radius:(CGFloat)radius;

+ (CGContextRef) createBitmapContext:(int)pixelsWide height:(int)pixelsHigh;
+ (void) deleteBitmapContext:(CGContextRef)ctx;

+ (UIImage*) buttonImage:(CGSize)size withTitle:(NSString*)strTitle andColor:(UIColor*) color;
+ (void) setButtonImage:(UIButton*)btn withColor:(UIColor*)color forState:(UIControlState)state;
+ (void) setButtonBackgroundImage:(UIButton*)btn withColor:(UIColor*)color forState:(UIControlState)state;

@end
