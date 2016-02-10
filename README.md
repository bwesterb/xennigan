xennigan
========

`xennigan` is a shell to run on a xen dom0 to give domu admins the ability
to issue some `xl` commands for their domu.

**`xennigan` is not finished yet.**


TODO
----

- Write man page.


Return codes
------------

-1 Invalid commandline arguments
-3 Invalid character in domu name
-4 Configuration file for domu not found
-5 stdin, stdout or stderr not available
-6 Could not clear environment
-7 Could not load configuration file /etc/xennigan.conf
-8 Could not find xl binary
-9 Invalid domu-cfg-path in /etc/xennigan.conf

Build
-----

    mkdir build
    cd build
    cmake ..
    make
