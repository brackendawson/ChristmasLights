//Definitions for this pattern
unsigned long static_buffer[6];

//functions for this pattern

/* init function */
void static_init(void) {
  static_buffer[0] = colour(RED,99);
  static_buffer[1] = colour(ORANGE,99);
  static_buffer[2] = colour(YELLOW,99);
  static_buffer[3] = colour(GREEN,99);
  static_buffer[4] = colour(BLUE,99);
  static_buffer[5] = colour(INDIGO,99);
  return;
}

/* frame function */
void static_frame(void) {
  return;
}

/* getled function */
unsigned long static_getled(unsigned char led) {
  //TODO: put this in the right place
  return static_buffer[led%6];
}
