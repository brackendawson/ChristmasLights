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

rgb24 pacman_getled(uint8_t led) {
  if (pacman_pos == led) {
    return (rgb24) {0xff,0xa0,0x00};
  }

  // orange ghost
  if (led+6 == pacman_pos) {
    if (pacman_dir) {
      return (rgb24) {0xff,0x20,0x00};
     }
     return (rgb24) {0x00,0x00,0x7f};
  }

  // cyan ghost
  if (led+12 == pacman_pos) {
    if (pacman_dir) {
      return (rgb24) {0x00,0xff,0xff};
    }
    return (rgb24) {0x00,0x00,0x7f};
  }

  // pink ghost
  if (led+16 == pacman_pos) {
    if (pacman_dir) {
      return (rgb24) {0xff,0x00,0x50};
    }
    return (rgb24) {0x00,0x00,0x7f};
  }

  if (pacman_dir) {
    // TOFIX: underflows when pacman_pos < 10 and NUM_LEDS is close to 255
    if (led+1 == NUM_LEDS) return (rgb24) {0xff,0x00,0x00}; // red cherry
    if ((led % 2 == 0) & (led > pacman_pos)) return (rgb24) {0xff,0xff,0xff};
  }
  return (rgb24) {0x00,0x00,0x00};
}
