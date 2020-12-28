/* Global definitions for this pattern, must be unique, it
is a good idea to prefix the pattern name. */
uint8_t fade_pat_brt;
bool fade_pat_dir;

// this buffer is a pointer directed to the memory address of common_buffer,
// even though they are different types, to save memory. Do check it fits.
uint32_t (*fade_buffer)[6] = (uint32_t(*)[6]) &common_buffer;
static_assert(sizeof(fade_buffer) <= sizeof(common_buffer), "fade_buffer must fit in common_buffer");

/* Private functions for this pattern, if you need any.
Make the names unique. */

/* init function for this pattern, called when beginning
this pattern before the first call of x_frame(). Prefixed
with the pattern name. */
void fade_init(void) {
  fade_pat_brt = 0;
  fade_pat_dir = 1;
  return;
}

/* frame function for this pattern, called 25 times per
second while displaying this pattern. It should be fairly
efficient, if it takes too long P1.0 output will be set
to 1 (red LED on launchpad). Prefixed with pattern name. */
void fade_frame(void) {
  /* colour(colour, brightness) is a useful helper function,
    it will retrun the 24 bit RGB value as uint32_t for
    a colour defined in ../colours.h and brightness from 0
    to 99 as unsigned char. */
  (*fade_buffer)[0] = colour(RED,fade_pat_brt);
  (*fade_buffer)[1] = colour(ORANGE,fade_pat_brt);
  (*fade_buffer)[2] = colour(YELLOW,fade_pat_brt);
  (*fade_buffer)[3] = colour(GREEN,fade_pat_brt);
  (*fade_buffer)[4] = colour(BLUE,fade_pat_brt);
  (*fade_buffer)[5] = colour(INDIGO,fade_pat_brt);
  if (fade_pat_dir) {
    if (fade_pat_brt >= 99) {
      fade_pat_dir = 0;
    } else {
      fade_pat_brt++;
    }
  } else {
    if (fade_pat_brt == 0) {
      fade_pat_dir = 1;
    } else {
      fade_pat_brt--;
    }
  }
  return;
}

/* getled function for this pattern called many times in no
guarented order, but always after the frame function has
finished.
Will be passed and uint8_t representing an LED index,
you must return the 24bit RGB value as uint32_t for that
LED. Your pattern should handle as many LEDs as are defined by
NUM_LEDS, that's up to 255.
Prefixed with pattern name. */
uint32_t fade_getled(uint8_t led) {
  return (*fade_buffer)[led%6];
}
