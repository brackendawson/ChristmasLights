/* Global definitions for this pattern, must be unique, it
is a good idea to prefix the pattern name. */
uint8_t fade_pat_brt;
bool fade_pat_dir;

/* this buffer is a pointer directed to the memory address of common_buffer
(they can be differnet types, see static.sh) to save memory. Do check it fits.
*/
static uint8_t (*fade_buffer)[6] = (uint8_t(*)[6]) &common_buffer;
static_assert(sizeof(*fade_buffer) <= sizeof(common_buffer), "fade_buffer must fit in common_buffer");

/* Private functions for this pattern, if you need any.
Make the names unique. */
void shuffle(uint8_t a[]) {
  for (uint8_t i = 0; i < 6 - 1; i++) {
    uint8_t j = random(i, 6);
    uint8_t c = a[i];
    a[i] = a[j];
    a[j] = c;
  }
  return;
}

/* init function for this pattern, called when beginning
this pattern before the first call of x_frame(). Prefixed
with the pattern name. */
void fade_init(void) {
  for (uint8_t i = 0; i < 6; i++) {
    (*fade_buffer)[i] = i+RED;
  }
  fade_pat_brt = 0;
  fade_pat_dir = 1;
  return;
}

/* frame function for this pattern, called 25 times per
second while displaying this pattern. It should be fairly
efficient, if it takes too long P1.0 output will be set
to 1 (red LED on launchpad). Prefixed with pattern name. */
void fade_frame(void) {
  if (fade_pat_dir) {
    if (fade_pat_brt >= 99) {
      fade_pat_dir = 0;
    } else {
      fade_pat_brt++;
    }
  } else {
    if (fade_pat_brt == 0) {
      fade_pat_dir = 1;
      shuffle(*fade_buffer);
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
  /* colour(colour, brightness) is a useful helper function,
    it will retrun the 24 bit RGB value as uint32_t for
    a colour defined in ../colours.h and brightness from 0
    to 99 as unsigned char. */
  return colour((*fade_buffer)[led%6], fade_pat_brt);
}
