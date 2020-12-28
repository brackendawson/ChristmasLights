uint8_t (*colourtwinkle_buffer)[REPEAT_LENGTH] = (uint8_t(*)[REPEAT_LENGTH]) &common_buffer;
static_assert(sizeof(*colourtwinkle_buffer) <= sizeof(common_buffer), "colourtwinkle_buffer must fit in common_buffer");
uint8_t colourtwinkle_div;

//functions for this pattern

//init function
void colourtwinkle_init(void) {
  colourtwinkle_div = 0;
  for (uint8_t i = 0; i < REPEAT_LENGTH; i++) {
    (*colourtwinkle_buffer)[i] = (uint8_t) random(RED,INDIGO+1); //random()'s max argument is exclusive
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

  //choose a random led and set it to a random *new* colour 
  uint8_t i = random(0,REPEAT_LENGTH);
  uint8_t old = (*colourtwinkle_buffer)[i];
  while (old == (*colourtwinkle_buffer)[i]) {
    (*colourtwinkle_buffer)[i] = (uint8_t) random(RED,INDIGO+1);
  }
  return;
}

//getled function
uint32_t colourtwinkle_getled(uint8_t led) {
  return colour((*colourtwinkle_buffer)[led % REPEAT_LENGTH], 99);
}
