//Constants for this pattern
  unsigned char run_index;
  unsigned char run_colour;

//functions for this pattern

void run_init(void) {
  run_index = 0;
  run_colour = RED;
  return;
}

void run_frame(void) {
  run_index++;
  if (run_index > 49) { //todo: 100, not 50
    run_index = 0;
    run_colour++;
    if (run_colour > INDIGO) {
      run_colour = RED;
    }
  }
  return;
}

unsigned long run_getled(unsigned char led) {
  if (led == run_index) {
    return(colour(run_colour,99));
  } else {
    return(colour(run_colour,0));
  }
}
