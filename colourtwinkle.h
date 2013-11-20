#define COLOURTWINKLE_ELEMENTS 16
unsigned char colourtwinkle_buffer[COLOURTWINKLE_ELEMENTS];
unsigned char colourtwinkle_div;

//functions for this pattern

//init function
void colourtwinkle_init(void) {
  colourtwinkle_div = 0;
  for (unsigned char i = 0; i < COLOURTWINKLE_ELEMENTS; i++) {
    colourtwinkle_buffer[i] = (unsigned char) random(1,NUM_COLOURS);
  }
  return;
}

//frame function
void colourtwinkle_frame(void) {
  //only run twice a second
  if (colourtwinkle_div++ < 12) {
    return;
  }
  colourtwinkle_div = 0;

  //set a random LED to a random colour
  colourtwinkle_buffer[random(0,COLOURTWINKLE_ELEMENTS)] = (unsigned char) random(1,NUM_COLOURS);

  return;
}

//getled function
unsigned long colourtwinkle_getled(unsigned char led) {
  return colour(colourtwinkle_buffer[led % COLOURTWINKLE_ELEMENTS], 99);
}
