/* This header defines all user configurable stuff.
Of course you're free to change anything outside
this file too... :-) */

/* Remember to set your MCU in the Makefile */

/* Number of LEDs in the string, up to 255 */
#define NUM_LEDS	50

//Patterns to include
#include "static.h"	//removing static breaks cycle mode logic
#include "fade.h"
#include "run.h"
#include "chameleon.h"
#include "wave.h"
#include "twinkle.h"
#include "pacman.h"
#include "firework.h"

typedef struct pattern_t {
  void (*init)();
  void (*frame)();
  unsigned long (*get)(unsigned char);
} pattern;

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
  };

#define NUM_PATTERNS (sizeof(patterns)/sizeof(pattern)-1)
unsigned char current_pattern = 1;	//default pattern, 0 is static
#define CYCLE_TIME 300/0.04		//frames to display a pattern for in cycle mode, must be integer, for convinience say "seconds/0.04".
bool cycle = 1;			//0 means default to not cycling, 1 means default to cycling from the default pattern


//add the patterns you desire and have #included to these three switches
void pattern_init(void) {
  patterns[current_pattern].init();
}

void frame(void) {
  patterns[current_pattern].frame();
}

unsigned long getled(unsigned char index) {
  return patterns[current_pattern].get(index);
}
