//Constants for this pattern

//functions for this pattern

/* init function for this pattern, called when beginning
this pattern */
void static_init(void) {
  buffer[0] = colour(RED,99);
  buffer[1] = colour(ORANGE,99);
  buffer[2] = colour(YELLOW,99);
  buffer[3] = colour(GREEN,99);
  buffer[4] = colour(BLUE,99);
  buffer[5] = colour(INDIGO,99);
  return;
}

/* frame function for this pattern, called 25 times per
second while displaying this pattern. It should be fairly
efficient, if it takes too long P1.0 output will be set
to 1 (red LED on launchpad). */
void static_frame(void) {
  return;
}

/* led function for this pattern called many times in no
guarented order, but always after the frame function has
finished.
Will be passed and unsigned char between 0 and 99, must
return the 24-bit colour value for the correspinding
LED as the right three bytes of an unsigned long. The
left byte is reserverved and should be returned zero. */
unsigned long static_getled(unsigned char led) {
  //TODO: put this in the right place
  return buffer[led%6];
}
