xennigan
========

`xennigan` is a shell to run on a xen dom0 to give domu admins the ability
to issue some `xl` commands for their domu.

Example

    ssh xennigan@dom0
    nameofdomu> help
    Available commands:
     status       shows status of domu
     shutdown     sends shutdown signal to domu
     reboot       sends reboot signal to domu
     console      opens console to domu
     destroy      immediate shutdown of domu
     create       starts domu if not running
     exit         exits shell
    nameofdomu> status
    Name                                        ID   Mem VCPUs  State   Time(s)
    nameofdomu                                  37  2048     8     -b----  497606.7
    nameofdomu> reboot

Installation
------------

First build and install.

    mkdir build && cd build
    cmake ..
    make
    sudo make install

By default, `xennigan` assumes your xen domu configuration files are stored
as

    /etc/xen/nameofdomu.cfg

Put the SSH public keys allowed to control the domu `nameofdomu` in

    /etc/xen/nameofdomu.keys

Then run

    update-xennigan

This will (after confirmation) create a `xennigan` user (and group) with
home directory `/var/lib/xennigan`.  It will fill the `authorized_keys` file
for `xennigan` appropriately.

An admin can now control their domu by SSHing as the `xennigan`
user to the dom0.

TODO
----

- Write man page.
- Packaging.
