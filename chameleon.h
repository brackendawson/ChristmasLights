//Definitions for this pattern
bool chameleon_dir;
unsigned char chameleon_brightness;
unsigned char chameleon_colour;

//functions for this pattern

//init function
void chameleon_init(void) {
  chameleon_dir = 1;	//up
  chameleon_colour = RED;
  chameleon_brightness = 0;
  return;
}

//frame function
void chameleon_frame(void) {
  if (chameleon_dir) {
    chameleon_brightness++;
    if (chameleon_brightness >= 99) {
      chameleon_dir = 0;
    }
  } else {
    chameleon_brightness--;
    if (chameleon_brightness <= 0) {
      chameleon_dir = 1;
      chameleon_colour++;
      if (chameleon_colour > INDIGO) {
        chameleon_colour = RED;
      }
    }
  }
  return;
}

//getled function
unsigned long chameleon_getled(unsigned char led) {
  unsigned long col = colour(chameleon_colour, chameleon_brightness);
  return(col);
}
