//Definitions for this pattern
bool chameleon_dir;
uint8_t chameleon_brightness;
uint8_t chameleon_colour;

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
    if (chameleon_brightness == 0) {
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
rgb24 chameleon_getled(uint8_t led) {
  return(colour(chameleon_colour, chameleon_brightness));
}
