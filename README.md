avr
===
Repository of AVR projects.

Dependencies
------------
Install the following software packages on your system:
- avr-binutils
- avr-gcc
- avr-gdb
- avr-libc
- avrdude
- scons

Quick start
-----------
1. Plug in your AVR flash device; note the device port in `/dev`.
2. Build the hex and flash your device; for example:

    scons --port=/dev/ttyACM0 blinky flash

This will prompt for your sudo password.
Type `scons --help` for more options; pass additional options in necessary.
3. Enjoy!
