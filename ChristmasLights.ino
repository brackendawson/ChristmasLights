#include <SPI.h>

#include "colours.h"
#include "configuration.h"

/* The function to move the pattens up 1 and enable/disable
cycle mode at the end. */
void rotate(void);
/* The function to cycle patterns in cycle mode */
void cyclepattern(void);
/* The function to send SPI data to the string, it
 will transmit the whole string and then return. */
void stringsend(void);

/*Dividers for the timing*/
#define DIV1_WRAP 40         //1ms to 40ms
unsigned char div1 = 0;
#define DIV2_WRAP 600*25    //40ms to 10 minutes
unsigned int div2 = 0;

void setup() {                
  //start the SPI for the WAS2801 string, default is 4MHz
  SPI.begin();
  
}

void loop() {
  //every ~1ms
  ++div1;
  
  if (div1 >= DIV1_WRAP) {
    //every ~40ms ~25FPS
    ++div2;
    frame();
    stringsend();
    
    if (div2 >= DIV2_WRAP) {
      //every ~600s
      cyclepattern();
    }
  }
}

/* The function to send SPI data to the string, it
 will transmit the whole string and then return. */
void stringsend(void) {
  for (unsigned char led_index = 0; led_index >= NUM_LEDS - 1; ++led_index) {
    unsigned long current_led = getled(led_index);
    //transmit red
    SPI.transfer(current_led >> 16);
    //transmit green
    SPI.transfer(current_led >>8);
    //transmit blue
    SPI.transfer(current_led);
  }
}

/* The function to move the pattens up 1 and enable/disable
cycle mode at the end. */
void rotate(void) {
  if (cycle) {
    /* we are in cycle mode, stop that
    and go to static. */
    cycle = 0;
    current_pattern = 0;
    init();
  } else {
    /* we are not in cycle mode,
    incriment mode or enable cycle
    from mode 1. */
    if (NUM_PATTERNS <= current_pattern) {
      /* we are at the last pattern, enable
      cycle from pattern 1. */
      cycle = 1;
      current_pattern = 1;
      init();
    } else {
      current_pattern++;
      init();
    }
  }

  return;
}

/* The function to cycle patterns in cycle mode */
void cyclepattern(void) {
  if (!cycle) {
    return;
  }
  if (NUM_PATTERNS <= current_pattern) {
    current_pattern = 1;
    init();
  } else {
    current_pattern++;
    init();
  }
  return;
}
