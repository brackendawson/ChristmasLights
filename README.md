This is a C program compilable with arduino and the
gcc-msp430 toolchain (msp430 launchpad).
It drives a WS2801 based string of RGB LEDs to create
lights for a Christmas tree.

Copyright 2012 - Bracken Dawson - Licensed under GPLv3.

# Arduino
## Installing
Edit configuration.h, it is commented. Settings
include number of LEDs, selection of patterns and
defaults. If you like, you can define your own
patterns, see fade.h for a documented example.

Install the FastSPI library (not version 2) from:
https://github.com/FastLED/FastLED
By clicking Sketch > Include Library > Add .ZIP Library...

open ChristmasLights.ino in the Arduino IDE,
select your board and serial port, then click
upload.

# MSP430
## Installing
Edit configuration.h, it is commented. Settings
include number of LEDs, selection of patterns and
defaults. If you can't compile in the space on your
MSP430, you might try taking out some patterns. If
you like, you can define your own patterns, see
fade.h for a documented example.

You require the mspgcc toolchain, version
mspgcc-20120406 works, as does the gcc-msp430 package
in ubuntu from 12.10 and up). YOu also need a tool to
program the chip such as mspdebug or msp430-gdbproxy.

If you are using mspdebug; "make prog" will compile
and program the chip for you.

If you are using some other programmer just use
"make" and load lights.elf onto your chip.

## Minimum requirements
 * 4KB program memory (8KB recommended).
 * 128B RAM (256B recommended).
 * DCO.
 * TimerA0.
 * USI.

I use the MSP430G2452 that comes with more recently
shipped launchpad development boards.

