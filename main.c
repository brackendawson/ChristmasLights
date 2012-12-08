#include <msp430.h>
#include <signal.h>
#include "colours.h"

//buffer for patterns to use, change its size if you need to
unsigned int buffer[6];

//stuff for USI
#define NUM_LEDS	100	//Number of LEDs on the string
unsigned char led_index;
//USI states
#define USI_IDLE	0	//USI not transmitting, USIIE is off
#define USI_TXRED	1	//USI is/was transmitting RED sub-pixel for LED[index]
#define USI_TXGREEN	2       //USI is/was transmitting GREEN sub-pixel for LED[index]
#define USI_TXBLUE	3	//USI is/was transmitting BLUE sub-pixel for LED[index]
unsigned char usi_state = USI_IDLE;

//TODO: move this to the right place
unsigned char pat_brt = 0;
_Bool pat_dir = 1;

int main(void) {

WDTCTL = WDTPW | WDTHOLD;     // Stop WDT
USICTL0 = USISWRST;           // Stop spamming the USI

/*setup clock, DCO to calibrated 1Mhz (apparently
calibration data for 8Mhz is not present on my
chip :-( ).*/
DCOCTL = CALDCO_1MHZ;
BCSCTL1 = CALBC1_1MHZ;

//setup GPIO
P1DIR = 255;
P1OUT = 0;

//setup USI
//        edges     interrupt still disabled, USIIFG cleared
USICTL1 = USICKPH;
//         SMCLK      no div
USICKCTL = USISSEL_2;
//        Output and clk             output enable   master    this reanables the USI
USICTL0 = USIPE7 | USIPE6 | USIPE5 | USIOE         | USIMST; //TODO trywithout USIPE7

//setup timers
//      SMCLK      /0     Up mode   interrupt en
TACTL = TASSEL_2 | ID_0 | MC_1    | TAIE;
//enable global interrupts
WRITE_SR(GIE);
// starts timer, going for 40ms 
TACCR0 = 0x9C40;

/* Go into Low power mode , main stops here.
CPU, MCLK are disabled, SMCLK, ACLK  and DCO
are active. */
_BIS_SR(LPM1_bits);

}

unsigned int get_led_val(unsigned char led) {
  //TODO: put this in the right place
  return buffer[led%6];
}

/*The USI state machine function, can be
called by code to get out of IDLE or by
USIServerRoutine() when a sub-pixel
transmission finishes. */
void send(void) {
  switch (usi_state) {
    case USI_IDLE:
      led_index = 0;
      USISRL = get_led_val(led_index) >> 16;	//load red byte into SPI buffer
      USICTL1 = USICTL1 + USIIE;	//enable interrupt on tx completion
      USICNT = 8;			//Start send of 8 bits
      usi_state = USI_TXRED;
      return;
    case USI_TXRED:
      USISRL = get_led_val(led_index) >> 8;	//load green byte into SPI buffer
      USICNT = 8;
      usi_state = USI_TXGREEN;
      return;
    case USI_TXGREEN:
      USISRL = get_led_val(led_index);	//load blue byte into SPI buffer
      USICNT = 8;
      usi_state = USI_TXBLUE;
      return;
    case USI_TXBLUE:
      led_index++;
      if (led_index >= NUM_LEDS) {
        USICTL1 = USICTL1 - USIIE;	//disable interrupt
        usi_state = USI_IDLE;
        return;
      } else {
        USISRL = get_led_val(led_index) >> 16;    //load red byte into SPI buffer
        USICNT = 8;                       //Start send of 8 bits
        usi_state = USI_TXRED;
        return;
      }
  }
}

/* This function shall be called 25 times per second. */
interrupt (TIMERA1_VECTOR) TimerA1ServerRoutine(void) {
  TACTL = TACTL - TAIFG; //reset Timer A interrupt flag
  if (usi_state != USI_IDLE) {
    /*set red LED if this happens. It means the pattern function
     is too slow. The string might also look wrong. */
    P1OUT = 1;
  }

  //TODO: move this pattern function to the right place
  buffer[0] = colour(RED,pat_brt);
  buffer[1] = colour(ORANGE,pat_brt);
  buffer[2] = colour(YELLOW,pat_brt);
  buffer[3] = colour(GREEN,pat_brt);
  buffer[4] = colour(BLUE,pat_brt);
  buffer[5] = colour(INDIGO,pat_brt);
  if (pat_dir) {
    if (pat_brt >= 99) {
      pat_dir = 0;
    } else {
      pat_brt++;
    }
  } else {
    if (pat_brt <= 0) {
      pat_dir = 1;
    } else {
      pat_brt--;
    }
  }
  
  send();  //MUST BE AFTER ALL EDITS OF BUFFER! MUST FINISH SENDING BEFORE NEXT CALL OF FUNCTION;
}

/* This function shall be called when the USI has
finished tranmising the data in it's buffer. */
interrupt (USI_VECTOR) USIServiceRoutine(void) {
  USICTL1 = USICTL1 - USIIFG;
  send();
}
