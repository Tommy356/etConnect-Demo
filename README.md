etConnect-Demo
==============

Demo App for iOS and OSX, demonstrating the basic usage of Distributed Objects using the etConnect Objective-C++ framework
(Distributed Objects on iOS and OSX). 

The project contains the binaries of the etConnect framework (ver. alpha 0.9) and the sources to build an iOS version and it's OSX counterpart.

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
About etConnect:
----------------

Other than the original approach using the NSConnection/NSDistantObject classes found on OSX, etConnect provides similiar
functionality on both platforms in a mre straigfoward approach and a redesigned archtitecture, which allows customization and
the addition of several security layers to protect the interchange of secure information through connected clients.

The base code of etConnect is highly platform independent and offers (with minor internal changes) the same functionality and API
on all platforms (iOS, Simulator and OSX). For iOS devices you'll require the iOS framework (ETConnect), which is a universal framework
for iOS devices and iOS simulator, while the OSX counterpart (ETConnect-OSX) offers the same functionality in an OSX environment. 


About the Demo Version:
-----------------------
The public demo version utilizies the compiled binaries of the etConnect framework in a little demo application.
The Demo-App monitors all etConnect devices available within a network domain and shows how to 'vend' objects as a service as well as to subscribe to 
objects vended by other devices and to remotely call their public methods.  

The current project shows this for iOS; the OSX counterpart will be added to the project shortly.  

Compile & Run:
--------------
Just check out the complete project and compile it. To make most use of it, you'll require at least two devices (one of
may be simulated in iOS simulator). I tried to keep the project as simple as possible and the sourcesd are well documented; 
however, at a first glance you might walk through the following classes:

<b>ESAppDelegate:</b></br>
Upon startup time, we'll start up a shared instance of the ETConnectServer class, which will
    (1) publish the device on the current domain, so other clients apps can find it, and
    (2) start monitoring the current domain for other devices running the etConnect Demo App.    





Notes:
------
This is currently under development - no real contents are added yet. The project is
due to be published as a first Beta soon.

General Comments:
-----------------

This project is an XCode5 workspace, holding several targets:

- etConnect-Demo is the demo app for iOS devices (iOS 6.0 or higher)
- etConnect-Demo-OSX implements a version for OSX


Notes for Building & Compiling:
------------------------------_
Make sure, you add the 'ObjC' flag to 'Other Linker Options' in order to link the framework correctly. If this
flag is NOT set, the etConnect framework will not link correctly (because some extended categories are not
recognized) and you'll get runtime errors after starting your App.

This version of etConnect has not been built for automatic reference counting (ARC). This will lead to compile 
time warnings and errors, if you attempt to compile modules imnporting etConnect with the ARC flag set.





