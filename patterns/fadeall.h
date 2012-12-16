//Constants for this pattern
unsigned char fadeall_pat_brt;
_Bool fadeall_pat_dir;
unsigned long fadeall_buffer[6];

//functions for this pattern

/* init function for this pattern, called when beginning
this pattern */
void fadeall_init(void) {
  fadeall_pat_brt = 0;
  fadeall_pat_dir = 1;
  return;
}

/* frame function for this pattern, called 25 times per
second while displaying this pattern. It should be fairly
efficient, if it takes too long P1.0 output will be set
to 1 (red LED on launchpad). */
void fadeall_frame(void) {
  fadeall_buffer[0] = colour(RED,fadeall_pat_brt);	/* colour(unsigned char color, unsigned char brightness)
					is a helper function that returns the 24-bit colour value
					for any colour defined in colours.h and a brightess from
					0 (off) to 99 (full brighness). */
  fadeall_buffer[1] = colour(ORANGE,fadeall_pat_brt);
  fadeall_buffer[2] = colour(YELLOW,fadeall_pat_brt);
  fadeall_buffer[3] = colour(GREEN,fadeall_pat_brt);
  fadeall_buffer[4] = colour(BLUE,fadeall_pat_brt);
  fadeall_buffer[5] = colour(INDIGO,fadeall_pat_brt);
  if (fadeall_pat_dir) {
    if (fadeall_pat_brt >= 99) {
      fadeall_pat_dir = 0;
    } else {
      fadeall_pat_brt++;
    }
  } else {
    if (fadeall_pat_brt <= 0) {
      fadeall_pat_dir = 1;
    } else {
      fadeall_pat_brt--;
    }
  }
  return;
}

/* led function for this pattern called many times in no
guarented order, but always after the frame function has
finished.
Will be passed and unsigned char between 0 and 99, must
return the 24-bit colour value for the correspinding
LED as the right three bytes of an unsigned long. The
left byte is reserverved and should be returned zero. */
unsigned long fadeall_getled(unsigned char led) {
  //TODO: put this in the right place
  return fadeall_buffer[led%6];		/* buffer is an array of unsigned long
				which you can use however you like, make
				it larger via the option in configuration.h
				if you need to	and never assume you know
				how long it is.	Defining your own array will
				use more RAM. */
}
