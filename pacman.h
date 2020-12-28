/* Global definitions for this pattern, must be unique, it
is a good idea to prefix the pattern name. */
uint8_t pacman_pos;
bool pacman_dir;
uint8_t pacman_slow;

/* Private functions for this pattern, if you need any.
Make the names unique. */

/* init function for this pattern, called when beginning
this pattern before the first call of x_frame(). Prefixed
with the pattern name. */
void pacman_init(void) {
  pacman_pos = 0;
  pacman_dir = 1;
  pacman_slow = 0;
  return;
}

/* frame function for this pattern, called 25 times per
second while displaying this pattern. It should be fairly
efficient, if it takes too long P1.0 output will be set
to 1 (red LED on launchpad). Prefixed with pattern name. */
void pacman_frame(void) {
  /* colour(colour, brightness) is a useful helper function,
    it will retrun the 24 bit RGB value as uint32_t for
    a colour defined in ../colours.h and brightness from 0
    to 99 as unsigned char. */
  if (pacman_slow++ < 8) {
    return;
  }
  pacman_slow = 0;
  
  if (pacman_dir) {
    if (pacman_pos >= NUM_LEDS) {
      pacman_dir = 0;
    } else {
      pacman_pos++;
    }
  } else {
    if (pacman_pos == 0) {
      pacman_dir = 1;
    } else {
      pacman_pos--;
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
uint32_t pacman_getled(uint8_t led) {
  if (pacman_pos == led) {
    return 0xff7a00; // pacman
  }

  // orange ghost
  if (led+6 == pacman_pos) return pacman_dir ? 0xff2000 : 0x00007f;

  // cyan ghost
  if (led+12 == pacman_pos) return pacman_dir ? 0x00ffff : 0x00007f;

  // pink ghost
  if (led+16 == pacman_pos) return pacman_dir ? 0xff0050 : 0x00007f;

  if (pacman_dir) {
    // TOFIX: underflows when pacman_pos < 10 and NUM_LEDS is close to 255
    if (led+1 == NUM_LEDS) return 0xff0000; // red cherry
    if ( (led % 2 == 0) & (led > pacman_pos) ) return 0xffffff;
  }
  return 0x000000;
}
