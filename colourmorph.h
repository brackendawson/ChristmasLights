uint8_t (*colourmorph_buffer)[REPEAT_LENGTH] = (uint8_t(*)[REPEAT_LENGTH]) &common_buffer;
static_assert(sizeof(colourmorph_buffer) <= sizeof(common_buffer), "colourmorph_buffer must fit in common_buffer");
uint8_t colourmorph_div;
uint8_t colourmorph_cur;
uint8_t colourmorph_morphed;

//functions for this pattern

//init function
void colourmorph_init(void) {
  colourmorph_div = 0;
  colourmorph_cur = RED;
  colourmorph_morphed = 0;
  for (uint8_t i = 0; i < REPEAT_LENGTH; i++) {
    (*colourmorph_buffer)[i] = RED;
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
  if (colourmorph_morphed >= REPEAT_LENGTH) {
    colourmorph_cur++;
    if (colourmorph_cur > INDIGO) {
      colourmorph_cur = RED;
    }
    colourmorph_morphed = 0;
  }

  //choose a random current color led and set it to the next colour
  uint8_t i = random(0, REPEAT_LENGTH);
  while (colourmorph_cur != (*colourmorph_buffer)[i]) {
    i = (i+1)%REPEAT_LENGTH;
  }
  (*colourmorph_buffer)[i] = (colourmorph_cur + 1);
  if ((*colourmorph_buffer)[i] > INDIGO) {
    (*colourmorph_buffer)[i] = RED;
  }
  colourmorph_morphed++;

  return;
}

//getled function
uint32_t colourmorph_getled(uint8_t led) {
  return colour((*colourmorph_buffer)[led % REPEAT_LENGTH], 99);
}
