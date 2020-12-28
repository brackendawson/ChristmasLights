This is a C program compilable with Arduino and TI MSP430 Launchpad. It drives
a WS2801 based string of RGB LEDs to create lights for a Christmas tree.

The MSP430 version uses the msp430-gcc toolchain and only works with WS2801
pixels. The Arduino version can be used with any LED type supported by the
FastLED library.

# Arduino
## Installing
Edit configuration.h, it is commented. Settings
include number of LEDs, selection of patterns and
defaults. If you like, you can define your own
patterns, see fade.h for a documented example.

Install the FastSPI library from:
https://github.com/FastLED/FastLED
By clicking Sketch > Include Library > Add .ZIP Library...

open ChristmasLights.ino in the Arduino IDE,
select your board and serial port, then click
upload.

## Wiring
* Clock -> pin 13
* Data -> pin 11
* Button -> pin 7 (optional)

# MSP430
## Installing
Edit configuration.h, it is commented. Settings
include number of LEDs, selection of patterns and
defaults. If you can't compile in the space on your
MSP430, you might try taking out some patterns. If
you like, you can define your own patterns, see
fade.h for a documented example.

You require the msp430-gcc toolchain and mspdebug, both available on Ubuntu.
Connect the launchpad to your computer, make sure you are in the `tty` and
`dialout` groups and run `make prog` to program the chip.

## Wiring
* Clock -> P1.5
* Data -> P1.6 (LED2, remove J5 P1.6 if you have issues with the lights)
* Button -> P1.3 (S2, optional)
* Error LED -> P1.0 (LED1, optional)

## Minimum requirements
Works out of the box on MSP430G2452. With modification can run on other chips:

Recommended:
 * 8KB program memory
 * 256B RAM
 * DCO
 * TimerA
 * USI
 * WDT

 Minimum (will require some patterns to be disabled):
 * 2KB program memory
 * 128B RAM
 * DCO
 * TimerA
 * USI
