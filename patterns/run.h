//Definitions for this pattern
  unsigned char run_index;
  unsigned char run_colour;

//functions for this pattern

//init function
void run_init(void) {
  run_index = 0;
  run_colour = RED;
  return;
}

//frame function
void run_frame(void) {
  run_index++;
  if (run_index >= NUM_LEDS) {
    run_index = 0;
    run_colour++;
    if (run_colour > INDIGO) {
      run_colour = RED;
    }
  }
  return;
}

//getled function
unsigned long run_getled(unsigned char led) {
  if (led == run_index) {
    return(colour(run_colour,99));
  } else {
    return(colour(run_colour,0));
  }
}
