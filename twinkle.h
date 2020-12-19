
#define TWINKLE_BUFFER_SIZE 16
bool twinkle_buffer[TWINKLE_BUFFER_SIZE];	//2 byte array for boolean twinkle values.
#define TWINKLE_ON	0x00FFFFFF
#define TWINKLE_OFF	0x00000000
unsigned char twinkle_i;
unsigned char twinkle_div;

//functions for this pattern

//init function
void twinkle_init(void) {
  unsigned char i;
  for (i = 0 ; i < sizeof(twinkle_buffer) ; i++) {
    twinkle_buffer[i] = 0;
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
  twinkle_i = random(0, TWINKLE_BUFFER_SIZE);

  //invert the bit
  twinkle_buffer[twinkle_i] = twinkle_buffer[twinkle_i] ^ 1;

  return;
}

//getled function
unsigned long twinkle_getled(unsigned char led) {
  if (twinkle_buffer[led % sizeof(twinkle_buffer)]) {
    return(TWINKLE_ON);
  } else {
    return(TWINKLE_OFF);
  }
}
