/************************************************************************
 * ETDebug.h
 *
 * Definitions for ETDebugOutput class
 ************************************************************************/

#import <Foundation/Foundation.h>

#ifndef ETBase_ETDebug_included
#define ETBase_ETDebug_included

#define DEBUG_SHOW_FULLPATH NO
#define DEBUG_SHOW_THREAD   YES

#if DEBUG
#define debug(format,...) [[ETDebugOutput sharedDebug] output:__FILE__ lineNumber:__LINE__ input:(format), ##__VA_ARGS__]
#else
#define debug(format,...)
#endif

#define NSLog(format,...) [[ETDebugOutput sharedDebug] output:__FILE__ lineNumber:__LINE__ input:(format), ##__VA_ARGS__]

#if DEBUG
#define ETSAssert(cond,format,...)  [[ETDebugOutput sharedDebug] assert:(cond) file:__FILE__ lineNumber:__LINE__ input:(format), ##__VA_ARGS__]
#else
#define ETSAssert(format,...)
#endif

// Super definitions for the special cases below:
#define LOGIT(format,...)                   [[ETDebugOutput sharedDebug] output:__FILE__ lineNumber:__LINE__ input:(format), ##__VA_ARGS__]
#define NOLOG(format,...)
#define LOGIF(condition,format,...)         if((condition)){[[ETDebugOutput sharedDebug] output:__FILE__ lineNumber:__LINE__ input:(format), ##__VA_ARGS__]}

/* general log statements */
#define NSLogWarning    LOGIT
#define NSLogError      LOGIT

/* GNUstep overrides */
#define NSDebugLog      LOGIT
#define NSDebugMLog     LOGIT
#define NSDebugFLog     LOGIT

#define NSWarnLog       NSLogWarning
#define NSWarnMLog      NSLogWarning
#define NSWarnFLog      NSLogWarning

#ifdef OBSOLETE
@interface DebugLog : NSObject
{
    BOOL bRunningInDebugger;
}

@property (nonatomic, retain) NSMutableString* strLog;

-(id) init;
-(void) show;
-(void)reset;
-(BOOL)removeLastBlock;
-(BOOL)shrink;
-(void)log:(char*)fileName lineNumber:(int)lineNumber input:(NSString*)input, ...;
@end
#endif //#ifdef OBSOLETE

@interface ETDebugOutput : NSObject
{
    BOOL bRunningInDebugger;
    NSMutableDictionary* dbgPrefix;
}

+(BOOL)isRunningInDebugger;
+ (ETDebugOutput *) sharedDebug;
-(void)output:(char*)fileName lineNumber:(int)lineNumber input:(NSString*)input, ...;
-(void)assert:(bool)condition file:(char*)fileName lineNumber:(int)lineNumber input:(NSString*)input, ...;

@end

@interface ETDebugOutput (Prefix)
/* enables and disables prefix-debugging:
 * if prefixes are enabled, format strings are checked for a '$:{PREFIX}' pattern. If a
 * prefix is found, it must be enabled, to generate debug output, otherwise it will be
 * supressed.
 */
+(void)enablePrefix:(NSString*)strPrefix;
+(void)disablePrefix:(NSString*)strPrefix;
@end

#endif //#define ETBase_ETDebug_included
