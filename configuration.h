/* Array for all patterns to use, change its size if you
need to. Be carefull if you make it smaller. */
unsigned long buffer[6];

/* Timer calibration, if the patterns are too fast, make
this larger. Not all MSP430s were created equal and few
come with larger than a 1MHz calibration. */
#define DCO_CAL_DIV     16

//Patterns to include
#include "patterns/fadeall.h"

void init(void) {
  return;
}

void frame(void) {
  fadeall_frame();
  return;
}

unsigned long getled(unsigned char brightness) {
  return fadeall_getled(brightness);
}
