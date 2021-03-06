/* This header defines all user configurable stuff.
Of course you're free to change anything outside
this file too... :-) */

/* MSP430 only, remember to set your MCU in the Makefile */

/* Number of LEDs in the string, up to 255. You will need to increase the size
of common_buffer if you increase NUM_LEDS, depending on the patterns you have
enabled. */
#define NUM_LEDS	50

/* Pin for mode button on arduino */
#ifdef ARDUINO
#define BUTTON_PIN      7
#else
/* Pin for mode button on MSP430 */
#define BUTTON_PIN      3       //P1.3 (S2 on launchpad)
#endif

/* On MSP430 there is a ~1 second wartdog serviced between frames, if a
pattern takes longer than this to render a frame, the CPU will reset. Unless
you uncomment this. */
//#define DISABLE_WATCHDOG

#ifndef ARDUINO
/* MSP430 only, Timer calibration.
This sets the frame time to 40ms (25fps) assuming the intarnal timers tick
every 250µs and the clock is 16MHz. If that's not the case, then this divider
can be used to adjust the speed of the program. */
#define DCO_CAL_DIV	160
#endif

/* General purpose memory for patterns to share.
You can reduce the size of this memory segment if you disable some of the
larger patterns. Compilation will fail if this is set too small. */
uint8_t common_buffer[24];

/* If an arbitrary length pattern repeats, repeat after this many LEDs
If this number is set too small, then the repitition will look obvious. If it
is set too large, then the tree will have little activity and look boring. */
#define REPEAT_LENGTH 16

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
#include "colourmorph.h"
#include "tetris.h"
#include "candle.h"

typedef struct pattern_t {
  void (*init)();
  void (*frame)();
  rgb24 (*get)(uint8_t);
} pattern;

/* Add the patterns you wish to display into this structure
 in the order you want to see them static must be the first
 element. */
static const pattern patterns[] =
  {
    { &static_init, &static_frame, &static_getled }, // keep static first
    { &fade_init, &fade_frame, &fade_getled },
    { &run_init, &run_frame, &run_getled },
    { &chameleon_init, &chameleon_frame, &chameleon_getled },
    { &wave_init, &wave_frame, &wave_getled },
    { &twinkle_init, &twinkle_frame, &twinkle_getled },
    { &pacman_init, &pacman_frame, &pacman_getled },
    { &firework_init, &firework_frame, &firework_getled },
    // { &firework_b_init, &firework_b_frame, &firework_getled }, // An alternate firework with no rocket
    { &colourtwinkle_init, &colourtwinkle_frame, &colourtwinkle_getled },
    { &colourmorph_init, &colourmorph_frame, &colourmorph_getled },
    { &tetris_init, &tetris_frame, &tetris_getled },
    { &candle_init, &candle_frame, &candle_getled },
  };

uint8_t current_pattern = 1;	//default pattern, 0 is static
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

rgb24 getled(uint8_t index) {
  return patterns[current_pattern].get(index);
}
