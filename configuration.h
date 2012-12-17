/* Timer calibration, if the patterns are too fast, make
this larger. Not all MSP430s were created equal and few
come with larger than a 1MHz calibration. */
#define DCO_CAL_DIV     16

//Patterns to include
#include "patterns/static.h"  //removing static breaks cycle logic
#include "patterns/fadeall.h"
#include "patterns/run.h"
#include "patterns/chameleon.h"
#include "patterns/wave.h"

#define NUM_PATTERNS 4 //not including static
unsigned char current_pattern = 0;	//default pattern, 0 is static
#define CYCLE_TIME 60/0.04		//frames to display a pattern for (seconds/0.04 MUST BE INTEGER!)
_Bool cycle = 0;			//0 means default to not cycling


//add the patterns you deside and have #included to these three switches
void init(void) {
  switch (current_pattern) {
    case 0:
      static_init();
      break;
    case 1:
      fadeall_init();
      break;
    case 2:
      run_init();
      break;
    case 3:
      chameleon_init();
      break;
    case 4:
      wave_init();
      break;
  }
  return;
}

void frame(void) {
  switch (current_pattern) {
    case 0:
      static_frame();
      break;
    case 1:
      fadeall_frame();
      break;
    case 2:
      run_frame();
      break;
    case 3:
      chameleon_frame();
      break;
    case 4:
      wave_frame();
      break;
  }
  return;
}

unsigned long getled(unsigned char brightness) {
  switch (current_pattern) {
    case 0:
      return static_getled(brightness);
    case 1:
      return fadeall_getled(brightness);
    case 2:
      return run_getled(brightness);
    case 3:
      return chameleon_getled(brightness);
    case 4:
      return wave_getled(brightness);
  }
  return(0);
}
