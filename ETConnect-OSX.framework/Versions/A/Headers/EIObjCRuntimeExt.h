//
//  EIObjCRuntimeExt.h
//  Framework-EIConnect
//
//  Created by Thomas Krautter on 25.10.13.
//  Copyright (c) 2013 Thomas Krautter. All rights reserved.
//

#ifndef __C_IMPORT__
#import <Foundation/Foundation.h>
#endif //#ifndef __C_IMPORT__

#import "EIConnectInternal.h"

#include <stdio.h>
#include <objc/objc.h>
#include <objc/objc-api.h>
#include <objc/runtime.h>
#include <stdarg.h>

//#ifndef _C_ATOM
//#define _C_ATOM '%'
//#endif

/*
 * Redefined encoding qualifiers: the original definitions don't match
 * the values returned from 'objc_get_type_qualifiers' in the runtime
 */

#define _F_CONST        0x0001
#define _F_IN           0x0002
#define _F_OUT          0x0004
#define _F_INOUT        0x0008
#define _F_BYCOPY       0x0010
#define _F_BYREF        0x0100
// FIXME: mismatch to Apple's definitions!
#define _F_ONEWAY       0x0400
#define _F_GCINVISIBLE  0x0200

#define _C_CONST    'r'
#define _C_IN       'n'
#define _C_INOUT    'N'
#define _C_OUT      'o'
#define _C_BYCOPY   'O'
#define _C_BYREF    'R'
#define _C_ONEWAY   'V'

BOOL EIObjCIsInstance(id obj);
BOOL EIObjCIsClass(Class cls);

Method EIGetMethod(Class cls, SEL sel, BOOL searchInstanceMethods, BOOL searchSuperClasses);
struct objc_method_description EIProtocolGetMethodDescriptionRecursive(Protocol *aProtocol, SEL aSel, BOOL isRequired, BOOL isInstance);

#ifdef __C_IMPORT__

#endif //#ifdef __C_IMPORT__

GS_EXPORT BOOL EISelectorTypesMatch(const char *types1, const char *types2);
GS_EXPORT BOOL EISelectorTypesMatchExt(const char *types1, const char *types2);

void* EIAutoreleasedBuffer(size_t size);

GS_EXPORT const char *objc_skip_type_qualifiers (const char *type);
GS_EXPORT const char *objc_skip_typespec(const char *type);
GS_EXPORT const char *objc_skip_argspec(const char *type);
GS_EXPORT size_t objc_sizeof_type(const char *type);
GS_EXPORT size_t objc_alignof_type(const char *type);
GS_EXPORT size_t objc_aligned_size(const char *type);
GS_EXPORT size_t objc_promoted_size(const char *type);

// -> GSObjcRuntimeExtras:
GS_EXPORT unsigned objc_get_type_qualifiers (const char *type);
