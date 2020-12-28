bool (*twinkle_buffer)[REPEAT_LENGTH] = (bool(*)[16]) &common_buffer;
static_assert(sizeof(*twinkle_buffer) <= sizeof(common_buffer), "twinkle_buffer must fit in common_buffer");
#define TWINKLE_ON	0x00FFFFFF
#define TWINKLE_OFF	0x00000000
uint8_t twinkle_i;
uint8_t twinkle_div;

//functions for this pattern

//init function
void twinkle_init(void) {
  for (uint8_t i = 0 ; i < REPEAT_LENGTH ; i++) {
    (*twinkle_buffer)[i] = 0;
  }
  twinkle_i = 0;
  twinkle_div = 0;

  return;
}

//frame function
void twinkle_frame(void) {
  //only run twice a second
  if (twinkle_div++ < 12) {
    return;
  }
  twinkle_div = 0;
  
  //select the next bit slightly randomly
  twinkle_i = random(0, REPEAT_LENGTH);

  //invert the bit
  (*twinkle_buffer)[twinkle_i] = (*twinkle_buffer)[twinkle_i] ^ 1;

  return;
}

//getled function
uint32_t twinkle_getled(uint8_t led) {
  if ((*twinkle_buffer)[led % REPEAT_LENGTH]) {
    return(TWINKLE_ON);
  } else {
    return(TWINKLE_OFF);
  }
}
