//
//  ETException.h
//  ETSCore Framework
//
//  Created by Thomas Krautter on 15.09.13.
//  Copyright (c) 2013 eitap Technologies GmbH. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 *  Several macro definitions to raise exceptions with extended error information:
 */
#define RAISE_IF(cond,r,i)      [ETException raiseIf:(cond)  cls:[self class] func:__func__ line:__LINE__ reason:(r) info:(i)]
#define RAISE_IFNOT(cond,r,i)   [ETException raiseIf:!(cond) cls:[self class] func:__func__ line:__LINE__ reason:(r) info:(i)]
#define RAISE(r)                [ETException raiseIf:TRUE    cls:[self class] func:__func__ line:__LINE__ reason:(r) info:nil]

#define IsETException(ex)       [ETException isETSException:ex]
#define ETExceptionReason(ex)   [ETException reasonID:ex]

/** 
 *  Predefined exception definitions for various system components.
 */
typedef NS_ENUM(NSInteger, RGExceptionReason) {
    
    RGXUndefined = 0,               // undefined exception
    RGXIOS,                         // System generated exception, dictionary holds additional exception info
    RGXSeriousError,                // @Any serious error (mostly startup), which can't be recovered
    
    RGXFileFirst,                   // File format & version errors:
    RGXFileUnknown = RGXFileFirst,  // unkown file format
    RGXFileStructure,               // unkown file format
    RGXFileVersionOld,              // app stack version is deprected, no converter available any more
    RGXFileVersionNew,              // app stack version is higher than system version
    
    RGXFileNotFound,                // @"stack file could not be found"
    RGXPathNotFound,                // @"stack path could not be found"
    
    RGXInvalidFileName,             // @"file name is invalid"
    RGXInvalidPathName,             // @"path name is invalid"
    RGXInvalidSystemPathName,       // @"system path name is invalid"
    RGXInvalidLibraryPathName,      // @"library path name is invalid"
    
    RGXFileExists,                  // @"File exists, specify overwrite or delete before creating"
    
    RGXInvalidStackOptions,         // @"Invalid stack options used for loading or creating a stack"

    RGXHomeStackMissing,            // @"Home stack could neither be found nor created, SERIOUS"
    
    RGXFileLast = RGXHomeStackMissing,
    
    RGXGraphFirst, // Errors when creating or manipulating objects
    RGXGraphDuplicate = RGXGraphFirst, // error while adding duplicate object
    RGXGraphLast = RGXGraphDuplicate,
    
    RGXResFirst, // Resource Errors
    RGXResNotFound = RGXResFirst, // a resource has not been found
    RGXResCopyError, // a resource could not be copied to the documents directoy
    RGXResLast = RGXGraphDuplicate,
    
    RGXImgFirst, // Image Management Exceptions
    RGXImgNotFound = RGXImgFirst, // a resource has not been found
    RGXImgDuplicateReference, // an image is added with a duplicate reference
    RGXImgLast = RGXImgDuplicateReference,
    
    RGXParamFirst, // Parameter and call errors
    RGXParamMismatch = RGXParamFirst, // a parameter mismatch has been detected
    RGXParamLast = RGXImgDuplicateReference,
    
    RGXLast
};

@interface ETException : NSException
+ (void) raiseIf:(BOOL)condition
             cls:(Class)class
            func:(const char*)func
            line:(NSInteger)line
          reason:(RGExceptionReason)reason
            info:(id)info;

+ (BOOL) isETSException:(NSException*)ex;
+ (RGExceptionReason) reasonID:(NSException*)ex;

@end
