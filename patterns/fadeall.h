/* Global definitions for this pattern, must be unique, it
is a good idea to prefix the pattern name. */
unsigned char fadeall_pat_brt;
_Bool fadeall_pat_dir;
unsigned long fadeall_buffer[6];

/* Private functions for this pattern, if you need any.
Make the names unique. */

/* init function for this pattern, called when beginning
this pattern before the first call of x_frame(). Prefixed
with the pattern name. */
void fadeall_init(void) {
  fadeall_pat_brt = 0;
  fadeall_pat_dir = 1;
  return;
}

/* frame function for this pattern, called 25 times per
second while displaying this pattern. It should be fairly
efficient, if it takes too long P1.0 output will be set
to 1 (red LED on launchpad). Prefixed with pattern name. */
void fadeall_frame(void) {
  /* colour(colour, brightness) is a useful helper function,
    it will retrun the 24 bit RGB value as unsigned long for
    a colour defined in ../colours.h and brightness from 0
    to 99 as unsigned char. */
  fadeall_buffer[0] = colour(RED,fadeall_pat_brt);
  fadeall_buffer[1] = colour(ORANGE,fadeall_pat_brt);
  fadeall_buffer[2] = colour(YELLOW,fadeall_pat_brt);
  fadeall_buffer[3] = colour(GREEN,fadeall_pat_brt);
  fadeall_buffer[4] = colour(BLUE,fadeall_pat_brt);
  fadeall_buffer[5] = colour(INDIGO,fadeall_pat_brt);
  if (fadeall_pat_dir) {
    if (fadeall_pat_brt >= 99) {
      fadeall_pat_dir = 0;
    } else {
      fadeall_pat_brt++;
    }
  } else {
    if (fadeall_pat_brt <= 0) {
      fadeall_pat_dir = 1;
    } else {
      fadeall_pat_brt--;
    }
  }
  return;
}

/* getled function for this pattern called many times in no
guarented order, but always after the frame function has
finished.
Will be passed and unsigned char representing an LED index,
you must return the 24bit RGB value as unsigned long for that
LED. Your pattern should handle as many LEDs as are defined by
NUM_LEDS, that's up to 255.
Prefixed with pattern name. */
unsigned long fadeall_getled(unsigned char led) {
  return fadeall_buffer[led%6];
}
