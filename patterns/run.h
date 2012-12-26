//Definitions for this pattern
  unsigned int run_index;
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
  if (run_index >= NUM_LEDS + 9) {
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
  } else if (led == run_index - 1) {
    return(colour(run_colour,89));
  } else if (led == run_index - 2) {
    return(colour(run_colour,79));
  } else if (led == run_index - 3) {
    return(colour(run_colour,69));
  } else if (led == run_index - 4) {
    return(colour(run_colour,59));
  } else if (led == run_index - 5) {
    return(colour(run_colour,49));
  } else if (led == run_index - 6) {
    return(colour(run_colour,39));
  } else if (led == run_index - 7) {
    return(colour(run_colour,29));
  } else if (led == run_index - 8) {
    return(colour(run_colour,19));
  } else if (led == run_index - 9) {
    return(colour(run_colour,9));
  } else {
    return(colour(run_colour,0));
  }
}
