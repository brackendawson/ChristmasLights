/* This header defines all user configurable stuff.
Of course you're free to change anything outside
this file too... :-) */

/* MSP430 only, remember to set your MCU in the Makefile */

/* Number of LEDs in the string, up to 255 */
#define NUM_LEDS	50

/* Pin for mode button on MSP430 */
#define BUTTON_PIN      3       //P1.3 (S2 on launchpad)
/* Pin for mode button on arduino */
#ifdef ARDUINO
#define BUTTON_PIN      7
#endif

#ifndef ARDUINO
/* MSP430 only, Timer calibration, if the patterns are
 too fast, make this larger. Not all MSP430s were
 created equal and few come with larger than a 1MHz
 calibration data. The Cycle time defined by
 CYCLE_TIME is a good thing to test against. */
#define DCO_CAL_DIV	16
#endif

//Patterns to include when compiling
#include "static.h"	//removing static breaks cycle mode logic
#include "fade.h"
#include "run.h"
#include "chameleon.h"
#include "wave.h"
#include "twinkle.h"
#include "pacman.h"
#include "firework.h"
#include "colourtwinkle.h"

typedef struct pattern_t {
  void (*init)();
  void (*frame)();
  unsigned long (*get)(unsigned char);
} pattern;

/* Add the patterns you wish to display into this structure
 in the order you want to see them static must be the first
 element. */
pattern patterns[] =
  {
    { &static_init, &static_frame, &static_getled }, // keep static first
    { &fade_init, &fade_frame, &fade_getled },
    { &run_init, &run_frame, &run_getled },
    { &chameleon_init, &chameleon_frame, &chameleon_getled },
    { &wave_init, &wave_frame, &wave_getled },
    { &twinkle_init, &twinkle_frame, &twinkle_getled },
    { &pacman_init, &pacman_frame, &pacman_getled },
    { &firework_init, &firework_frame, &firework_getled },
    { &colourtwinkle_init, &colourtwinkle_frame, &colourtwinkle_getled },
  };

unsigned char current_pattern = 1;	//default pattern, 0 is static
#define CYCLE_TIME 300/0.04		//frames to display a pattern for in cycle mode, must be integer, for convinience say "seconds/0.04".
bool cycle = 1;	        		//0 means default to not cycling, 1 means default to cycling from the default pattern
#define CYCLE_RANDOMLY  //uncomment to cycle randomly through the patterns in cycle mode

//No need to edit below this line
//-------------------------------
#define NUM_PATTERNS (sizeof(patterns)/sizeof(pattern)-1)

void pattern_init(void) {
  patterns[current_pattern].init();
}

void frame(void) {
  patterns[current_pattern].frame();
}

unsigned long getled(unsigned char index) {
  return patterns[current_pattern].get(index);
}
