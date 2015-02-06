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
The above packages can usually be found through your system's package manager.

You also need to install the Arduino libraries;
to do this, please follow the instructions in the next section.

Arduino Libraries
-----------------
1. `git clone https://github.com/Earlz/make-wiring`
2. `cd make-wiring`
3. Run the following commands:
    - `./build.rb build arduino`
    - `./build.rb build Wire`
    - `./build.rb build SPI`
    - `./build.rb build SoftwareSerial`

   This will create the directory `output` in the current directory.
4. `sudo mv output /usr/share/arduino`

   The build script will look for the arduino libraries in this folder.

Quick start
-----------
1. Plug in your AVR flash device; note the device port in `/dev`.
2. Build the hex and flash your device; for example:

       scons --port=/dev/ttyACM0 serial flash

   This will prompt for your sudo password.
   Type `scons --help` for more options; pass additional options in necessary.
3. Enjoy!
