etConnect-Demo
==============

Demo App for iOS and OSX, demonstrating the basic usage of Distributed Objects using the etConnect Objective-C++ framework
(Distributed Objects on iOS and OSX). 

The project contains the binaries of the etConnect framework (ver. alpha 0.9) and the sources to build an iOS version and it's OSX counterpart.

History:
----------------
2013/12/06  Added the etConnect OSX demo App and the frameworks required. 


About etConnect:
----------------
The etConnect framework is (loosely) based on Distributed Objects, a mechanism taking advantage of Objective-C++'s built-in capability
for remote calls to objects 'vended' by a remote service.

In short, rather than dealing with sockets or network protocols, you may design and implement an NSObject-based class as you
would do on a local instance, and then 'vend' the object to other devices on the network, which, in turn, with little overhead
may call the methods of the 'vended' object.

While OSX provided the required functionality since a while, Distributed Objects have not been available for iOS devices. The etConnect
framework makes this functionality available and allows to implement DO's on both platforms, allowing you to design classes, that
interact seamlessly on all platforms. 

Other than the original approach using the NSConnection/NSDistantObject classes found on OSX, etConnect provides similiar
functionality on both platforms in a more straigfoward approach and a redesigned architecture, which allows customization and
the addition of several security layers to protect the interchange of secure information through connected clients.

The base code of etConnect is highly platform independent and offers (with minor internal changes) the same functionality and API
on all platforms (iOS, Simulator and OSX). For iOS devices you'll require the iOS framework (ETConnect), which is a universal framework
for iOS devices and iOS simulator, while the OSX counterpart (ETConnect-OSX) offers the same functionality in an OSX environment. 


About the Demo Version:
-----------------------
This public demo version utilizies the compiled binaries of the etConnect framework in a little demo application.
The Demo-App monitors all etConnect devices available within a network domain and shows how to 'vend' objects as a service as well as to subscribe to 
objects vended by other devices and to remotely call their public methods.  

The current project shows this for iOS; the OSX counterpart will be added to the project shortly.  

Compile & Run:
--------------
Just check out the complete project and compile it. To make most use of it, you'll require at least two devices (one of
may be simulated in iOS simulator). I tried to keep the project as simple as possible and the sources are well documented; 
however, at a first glance you might walk through the following classes:

<b>ESAppDelegate:</b></br>

Upon startup time, we'll start up a shared instance of the ETConnectServer class, which will

    (1) publish the device on the current domain, so other clients apps can find it, and
    (2) start monitoring the current domain for other devices running the etConnect Demo App.    

<b>ESViewController:</b></br>

The main view controller: this view controller monitors other client on the domain and provides
controls to publish/unpublish a demo service and to enable/disable peer-to-peer functionality (iOS 7.x only). 
 
It provides an UITableView which is updated every time, a new device is found or the services of a
device have been changed. When tapping an entry of a table view, the demo app will switch to a detail view
demonstrating additional functionality (see 'ESDetailViewController', below).


<b>ESDetailViewController:</b></br>

The detail view controller demonstrates how to retrieve additional information of a remote device, how
to create a proxy and how to remotely call methods on the other device. It provides two buttons:

    (1) Tap To Connect: Tap this, to make the remote device display an alert message.
    (2) Tap for Ping-Pong: Tap this, to make the remote device connecting back. You won't get visual feedback, just follow the log output in XCode and check out the code ;-)  


<b>ESRemoteObject:</b></br>

This is a sample implementation of an object 'vended' over etConnect. Currently, it provides the 
Ping-Pong and Alert methods; however it's a good starting point to discover, what etConnect can do for you.
Check out the code, it's documented.

<b>One Word about Protocols:</b></br>

In general, it's a good practice to define a protocol for all methods you intend to access with remote calls.
Having (and assigning) a protocol has two benefits:

(1) It reduces the network traffic, as for unknown methods distant objects will always query the remote side to match and code the required data when calling a method,

(2) Once doing cross platform calls (32 -> 64 bit or vice versa) the coding of some Cocoa values (i.e. NSInteger, NSUInteger) are defined differently.
To assure correct value coding, <b>etConnect</b> will refuse remote calls on cross-platform calls without protocols. Just to illustrate:

This will work (without protocol), when called from 64-bit OSX to 32-bit iOS:

    NSNumber* someNumber = [NSNumber numberWithInt:4711];
    [someRemoteProxy setNumber:someNumber]; 

This will not work (without protocol), when called from 64-bit OSX to 32-bit iOS:

    NSInteger someNumber = 4711;
    [someRemoteProxy setNumber:someNumber]; 

Reason for this is found in the coding protocol, which is an integral part of the compiler, and the different #define statements for NSInteger (and anything else,
relying on this). The etConnect framework can compensate this, but it requires a protocol on both sides to be able to deal with it. So, it will just throw an
exception, whenever you try to pass an NSInteger/NSUInteger or any structure containing such a value. The workaround is to define a protocol and to assign it to the proxy
as soon as you acquired it (see comments in the Demo App).  

Notes:
------
This is currently under development - the functionality descibed covers only a basic part. However, I'll add
some more possible applications to the Demo App over time - you're well invited, to contribute your ideas! 


Next Steps:
------------
I'll enable the bluetooth options for both platforms.


Notes for Building & Compiling:
------------------------------

Make sure, you add the 'ObjC' flag to 'Other Linker Options' in order to link the framework correctly. If this
flag is NOT set, the etConnect framework will not link correctly (because some extended categories are not
recognized) and you'll get runtime errors after starting your App.

This version of etConnect has not been built for automatic reference counting (ARC). This will lead to compile 
time warnings and errors, if you attempt to compile modules imnporting etConnect with the ARC flag set.





