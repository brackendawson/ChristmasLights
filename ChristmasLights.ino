#include <FastSPI_LED.h>

// Sometimes chipsets wire in a backwards sort of way
struct CRGB { unsigned char b; unsigned char r; unsigned char g; };
// struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
struct CRGB *leds;

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
  FastSPI_LED.setLeds(NUM_LEDS);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_WS2801);

  //FastSPI_LED.setPin(PIN);
  FastSPI_LED.setDataRate(1);
  FastSPI_LED.init();
  FastSPI_LED.start();

  leds = (struct CRGB*)FastSPI_LED.getRGBData();
}

void loop() {
  //every ~1ms
  ++div1;
  
  if (div1 >= DIV1_WRAP) {
    //every ~40ms ~25FPS
    div1=0;
    ++div2;
    frame();
    stringsend();
    
    if (div2 >= DIV2_WRAP) {
      //every ~600s
      div2=0;
      cyclepattern();
    }
  }
}

/* The function to send SPI data to the string, it
 will transmit the whole string and then return. */
void stringsend(void) {
  memset(leds, 255, NUM_LEDS * 3);
  for(int i = 0 ; i < NUM_LEDS; i++ ) {
    unsigned long current_led = getled(i);
    //transmit red
    leds[i].r = current_led >> 16;
    //transmit green
    leds[i].g = current_led >>8;
    //transmit blue
    leds[i].b = current_led;
  }
  FastSPI_LED.show();
}

/* The function to move the pattens up 1 and enable/disable
cycle mode at the end. */
void rotate(void) {
  if (cycle) {
    /* we are in cycle mode, stop that
    and go to static. */
    cycle = 0;
    current_pattern = 0;
    pattern_init();
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
