#define COLOURMORPH_ELEMENTS 16
unsigned char colourmorph_buffer[COLOURMORPH_ELEMENTS];
unsigned char colourmorph_div;
unsigned char colourmorph_cur;
unsigned char colourmorph_morphed;

//functions for this pattern

//init function
void colourmorph_init(void) {
  colourmorph_div = 0;
  colourmorph_cur = RED;
  colourmorph_morphed = 0;
  for (unsigned char i = 0; i < COLOURMORPH_ELEMENTS; i++) {
    colourmorph_buffer[i] = RED;
  }
  return;
}

//frame function
void colourmorph_frame(void) {
  //only run twice a second
  if (colourmorph_div++ < 12) {
    return;
  }
  colourmorph_div = 0;

  //if all the lights are morphed, move the colour
  if (colourmorph_morphed >= COLOURMORPH_ELEMENTS) {
    colourmorph_cur++;
    if (colourmorph_cur > INDIGO) {
      colourmorph_cur = RED;
    }
    colourmorph_morphed = 0;
  }

  //choose a random current color led and set it to the next colour
  unsigned char i;
  do {
    i = random(0, COLOURMORPH_ELEMENTS);
  } while (colourmorph_cur != colourmorph_buffer[i]);
  colourmorph_buffer[i] = (colourmorph_cur + 1);
  if (colourmorph_buffer[i] > INDIGO) {
    colourmorph_buffer[i] = RED;
  }
  colourmorph_morphed++;

  return;
}

//getled function
unsigned long colourmorph_getled(unsigned char led) {
  return colour(colourmorph_buffer[led % COLOURMORPH_ELEMENTS], 99);
}
