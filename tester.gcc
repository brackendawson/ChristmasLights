#include <stdio.h>

typedef unsigned char bool;

#include <colours.h>
#include <configuration.h>

void loop(void);

/* The function to move the pattens up 1 and enable/disable
cycle mode at the end. */
void rotate(void);
/* The function to cycle patterns in cycle mode */
void cyclepattern(void);
/* The function to send SPI data to the string, it
 will transmit the whole string and then return. */
void stringsend(void);

/*Dividers for the timing*/
#define DIV1_WRAP 10         //1ms to 40ms
unsigned char div1 = 0;
#define DIV2_WRAP 6*25    //40ms to 10 minutes
unsigned int div2 = 0;

int main(int c, char** v)
{
  printf("<html><head></head><body><table>\n");
  for (int i = 0; i < 10000; i++) {
    loop();
  }
  printf("<html><head></head><body><table>\n");
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
  printf("<tr>\n");
  for(int i = 0 ; i < NUM_LEDS; i++ ) {
    unsigned long current_led = getled(i);
    printf("<td bgcolor=\"#%1x%1x%1x\">&nbsp;</td>",
           (current_led >> 16)&0xf, (current_led>>8)&0xf, current_led&0xf);
  }
  printf("</tr>\n");
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
      //init();
    } else {
      current_pattern++;
      //init();
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
    pattern_init();
  } else {
    current_pattern++;
    pattern_init();
  }
  printf("<tr><td colspan=\"%d\">Changed to pattern %d</td><tr>\n",
         NUM_LEDS, current_pattern);
  return;
}
