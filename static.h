//Definitions for this pattern
uint32_t (*static_buffer)[6] = (uint32_t(*)[6]) &common_buffer;
static_assert(sizeof(*static_buffer) <= sizeof(common_buffer), "static_buffer must fit in common_buffer");

//functions for this pattern

/* init function */
void static_init(void) {
  (*static_buffer)[0] = colour(RED,99);
  (*static_buffer)[1] = colour(ORANGE,99);
  (*static_buffer)[2] = colour(YELLOW,99);
  (*static_buffer)[3] = colour(GREEN,99);
  (*static_buffer)[4] = colour(BLUE,99);
  (*static_buffer)[5] = colour(INDIGO,99);
  return;
}

/* frame function */
void static_frame(void) {
  return;
}

/* getled function */
uint32_t static_getled(uint8_t led) {
  return (*static_buffer)[led%6];
}
