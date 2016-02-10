xennigan
========

`xennigan` is a shell to run on a xen dom0 to give domu admins the ability
to issue some `xl` commands for their domu.

**`xennigan` is not finished yet.**


`/etc/xennigan.conf`
--------------------

    # These are the default values.
    xl-path = /usr/sbin/xl
    domu-cfg-path = /etc/xen/%1%.cfg


TODO
----

- Write man page.
- Packaging.


`xennigan-shell` return codes
-----------------------------

*  0: everything went fine
* -1: Invalid commandline arguments
* -3: Invalid character in domu name
* -4: Configuration file for domu not found
* -5: stdin, stdout or stderr not available
* -6: Could not clear environment
* -7: Could not load configuration file `/etc/xennigan.conf`
* -8: Could not find xl binary
* -9: Invalid domu-cfg-path in `/etc/xennigan.conf`

Build
-----

    mkdir build
    cd build
    cmake ..
    make
