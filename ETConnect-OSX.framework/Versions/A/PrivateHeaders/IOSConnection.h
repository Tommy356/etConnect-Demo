//
//  IOSConnection.h
//  EIConnectionIOS
//
//  Created by Thomas Krautter on 06.10.13.
//  Copyright (c) 2013 Thomas Krautter. All rights reserved.
//

#ifndef NSConnectionIOS_IOSConnection_h
#define NSConnectionIOS_IOSConnection_h

#pragma mark - UTILITY DEFINITIONS -
#define M_LOCK(X)   {NSLogConnLock(@"EIConnection Lock   %@",X); [X lock]; }
#define M_UNLOCK(X) {NSLogConnLock(@"EIConnection Unlock %@",X); [X unlock]; }

#ifndef	RETAIN
/**
 *	Basic retain operation ... calls [NSObject-retain]
 */
#define	RETAIN(object)		[(object) retain]
#endif

#ifndef	RELEASE
/**
 *	Basic release operation ... calls [NSObject-release]
 */
#define	RELEASE(object)		[(object) release]
#endif

#ifndef	AUTORELEASE
/**
 *	Basic autorelease operation ... calls [NSObject-autorelease]
 */
#define	AUTORELEASE(object)	[(object) autorelease]
#endif

#ifndef	ASSIGN
/**
 *	ASSIGN(object,value) assigns the value to the object with
 *	appropriate retain and release operations.
 */
#define	ASSIGN(object,value)	({\
id __object = object; \
object = [(value) retain]; \
[__object release]; \
})
#endif

#ifndef	TEST_RETAIN
/**
 *	Tested retain - only invoke the
 *	objective-c method if the receiver is not nil.
 */
#define	TEST_RETAIN(object)	({\
id __object = (object); (__object != nil) ? [__object retain] : nil; })
#endif
#ifndef	TEST_RELEASE
/**
 *	Tested release - only invoke the
 *	objective-c method if the receiver is not nil.
 */
#define	TEST_RELEASE(object)	({\
id __object = (object); if (__object != nil) [__object release]; })
#endif
#ifndef	TEST_AUTORELEASE
/**
 *	Tested autorelease - only invoke the
 *	objective-c method if the receiver is not nil.
 */
#define	TEST_AUTORELEASE(object)	({\
id __object = (object); (__object != nil) ? [__object autorelease] : nil; })
#endif

#ifndef	ASSIGN
/**
 *	ASSIGN(object,value) assigns the value to the object with
 *	appropriate retain and release operations.
 */
#define	ASSIGN(object,value)	({\
id __object = object; \
object = [(value) retain]; \
[__object release]; \
})
#endif

#ifndef	ASSIGNCOPY
/**
 *	ASSIGNCOPY(object,value) assigns a copy of the value to the object
 *	with release of the original.
 */
#define	ASSIGNCOPY(object,value)	({\
id __object = object; \
object = [(value) copy];\
[__object release]; \
})
#endif

#ifndef	DESTROY
/**
 *	DESTROY() is a release operation which also sets the variable to be
 *	a nil pointer for tidiness - we can't accidentally use a DESTROYED
 *	object later.  It also makes sure to set the variable to nil before
 *	releasing the object - to avoid side-effects of the release trying
 *	to reference the object being released through the variable.
 */
#define	DESTROY(object) 	({ \
id __o = object; \
object = nil; \
[__o release]; \
})
#endif



#endif
