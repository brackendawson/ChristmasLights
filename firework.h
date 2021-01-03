/* Global definitions for this pattern, must be unique, it
is a good idea to prefix the pattern name. */
uint8_t firework_pos;
uint8_t firework_exp_pos;
uint8_t firework_col;
rgb24 firework_exp_col;
uint8_t firework_size;
enum firework_states {
  LAUNCHED = 1,
  EXPLODED = 2
};
enum firework_states firework_state;

/* Private functions for this pattern, if you need any.
Make the names unique. */

/* init function for this pattern, called when beginning
this pattern before the first call of x_frame(). Prefixed
with the pattern name. */
void firework_init(void) {
  firework_pos = 0;
  firework_state = LAUNCHED;
  firework_size = random(10,20);
  firework_exp_pos = random((NUM_LEDS>>1), NUM_LEDS-(firework_size>>1));
  return;
}

uint8_t percent(uint16_t n, uint8_t d) {
  uint8_t r = 0;
  n *= 100;
  while (n >= d) {
    n -= d;
    r++;
  }
  return r;
}

/* frame function for this pattern, called 25 times per
second while displaying this pattern. It should be fairly
efficient, if it takes too long P1.0 output will be set
to 1 (red LED on launchpad). Prefixed with pattern name. */
void firework_frame(void) {
  if (firework_state == LAUNCHED) {
    if (firework_pos >= firework_exp_pos) {
      firework_state = EXPLODED;
      firework_pos = 0; // size of explosion
      firework_col = random(1,NUM_COLOURS); //max argument on random is exclusive, min is inclusive
      firework_exp_col =
        colour(firework_col, percent(firework_size-firework_pos,firework_size));
    } else {
      firework_pos++;
    }
  } else {
    if (firework_pos == firework_size) {
      firework_init();
    } else {
      firework_pos++;
      firework_exp_col =
        colour(firework_col, percent(firework_size-firework_pos,firework_size));
    }
  }
  return;
}

rgb24 firework_getled(uint8_t led) {
  if (firework_state == LAUNCHED) {
    if (firework_pos == led) {
      return (rgb24) {0xff,0x7f,0x7f};
    }
  } else {
    for (uint8_t i = 0; i < 3; i++) {
      uint8_t offset = firework_pos >> i;
      if (led == firework_exp_pos+offset || led+offset == firework_exp_pos) {
        return firework_exp_col;
      }
    }
  }
  return (rgb24) {0x00,0x00,0x00};
}

/* An alternate firework pattern without the rocket visible. */
void firework_b_init(void) {
  firework_pos = 0;
  firework_state = EXPLODED;
  firework_col = random(1,NUM_COLOURS); //max argument on random is exclusive, min is inclusive
  firework_size = random(10,20);
  firework_exp_col =
    colour(firework_col, percent(firework_size-firework_pos,firework_size));
  firework_exp_pos = random(firework_size/2, NUM_LEDS-(firework_size/2));
  return;
}

void firework_b_frame(void) {
  if (firework_pos == firework_size) {
    firework_b_init();
  } else {
    firework_pos++;
    firework_exp_col =
      colour(firework_col, percent(firework_size-firework_pos,firework_size));
  }
}
