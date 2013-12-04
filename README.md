etConnect-Demo
==============

Demo App for iOS and OSX, demonstrating the basic usage of Distributed Objects using the etConnect Objective-C++ framework
(Distributed Objects on iOS and OSX). 

The project contains the binaries of the etConnect framework (ver. alpha 0.9) and the sources to build an iOS version and it's OSX counterpart.

The iOS Version
===============
The iOS version provides a screen monitoring the local domain for available hosts. Available hosts
are listed in a table view as they show up and disappear.


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





