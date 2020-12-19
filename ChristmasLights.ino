#include <FastLED.h>

#include "colours.h"
#include "configuration.h"

CRGB leds[NUM_LEDS];

/* The function to move the pattens up 1 and enable/disable
cycle mode at the end. */
void rotate(void);
/* The function to cycle patterns in cycle mode */
void cyclepattern(void);
/* The function to send SPI data to the string, it
 will transmit the whole string and then return. */
void stringsend(void);

/*Dividers for the timing*/
#define DIV1_WRAP 160         //1ms to 40ms
unsigned char div1 = 0;
unsigned int div2 = 0;

//button
int button_state = 1;

void setup() {                
  //start the SPI for the WS2801 string
  FastLED.addLeds<WS2801, 11, 13, RGB>(leds, NUM_LEDS);
  
  //Mode button
  pinMode(BUTTON_PIN, INPUT_PULLUP);
 
  pattern_init();
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
    
    if (div2 >= CYCLE_TIME) {
      //every ~600s
      div2=0;
      cyclepattern();
    }
  }
  
  //button
  int button_now = digitalRead(BUTTON_PIN);
  if (button_now < button_state) {
    rotate();
    button_state = button_now;
  } else {
    button_state = button_now;
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
    leds[i].g = current_led >> 8;
    //transmit blue
    leds[i].b = current_led;
  }
  FastLED.show();
  delay(40); // without this lights are "random"
}

/* The function to move the pattens up 1 and enable/disable
cycle mode at the end. */
void rotate(void) {
  if (cycle) {
    /* we are in cycle mode, stop that
    and go to static. */
    cycle = 0;
    current_pattern = 0;
  } else {
    /* we are not in cycle mode,
    incriment mode or enable cycle
    from mode 1. */
    if (NUM_PATTERNS <= current_pattern) {
      /* we are at the last pattern, enable
      cycle from pattern 1. */
      cycle = 1;
      current_pattern = 1;
    } else {
      current_pattern++;
    }
  }

  pattern_init();
  return;
}

/* The function to cycle patterns in cycle mode */
void cyclepattern(void) {
  if (!cycle) {
    return;
  }
#ifdef CYCLE_RANDOMLY
  unsigned char old = current_pattern;
  while (old == current_pattern) {
    current_pattern = random(1,NUM_PATTERNS+1);
  }
#else
  if (NUM_PATTERNS <= current_pattern) {
    current_pattern = 1;
  } else {
    current_pattern++;
  }
#endif
  pattern_init();
  return;
}
