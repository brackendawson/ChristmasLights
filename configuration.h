/* Array for all patterns to use, change its size if you
need to. Be carefull if you make it smaller. */
unsigned long buffer[6];

//Patterns to include
#include "fadeall.h"

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
