#ifndef ARDUINO
#include <msp430.h>
#include <stdint.h>
#include "msp430/arduino2msp430.h"
#include "colours.h"
#include "configuration.h"

//stuff for USI
uint8_t led_index;
//USI states
#define USI_IDLE	0	//USI not transmitting, USIIE is off
#define USI_TXRED	1	//USI is/was transmitting RED sub-pixel for LED[index]
#define USI_TXGREEN	2       //USI is/was transmitting GREEN sub-pixel for LED[index]
#define USI_TXBLUE	3	//USI is/was transmitting BLUE sub-pixel for LED[index]
uint8_t usi_state = USI_IDLE;

//Timer devider
uint8_t timera_div = 0; //to DCO_CAL_DIV

//Pattern cycle divider
uint16_t cycle_div = 0; //to CYCLE_TIME

void main(void) {

BCSCTL3 = LFXT1S_2; // ACLK from VLO
#ifdef DISABLE_WATCHDOG
WDTCTL = WDTPW | WDTHOLD; // Stop WDT
#else
WDTCTL = WDT_ARST_1000;   // Pet WDT
#endif

USICTL0 = USISWRST;           // Stop spamming the USI during init

/*setup clock, DCO to calibrated 16MHz */
DCOCTL = CALDCO_16MHZ;
BCSCTL1 = CALBC1_16MHZ;

//setup GPIO
P1DIR = ~(1 << BUTTON_PIN); //Set S2 (P1.3) as input 
P1REN = 1 << BUTTON_PIN;    //Set pullup
P1OUT = 1 << BUTTON_PIN;    //Set pullup
P1IE  = 1 << BUTTON_PIN;    //Intettupt only for S2 (P1.3)
P1IES = 1 << BUTTON_PIN;    //interrupt on falling edge
P1IFG = 0;                  //Clear P1 interrupts

//setup USI
//        edges     interrupt disabled and USIIFG cleared by this
USICTL1 = USICKPH;
//         SMCLK       /8 (2MHz assuming a 16MHz SMCLK)
USICKCTL = USISSEL_2 | USIDIV_3;
//        Output and clk    output enable   master    this reanables the USI by clearning USISWRST
USICTL0 = USIPE6 | USIPE5 | USIOE         | USIMST;

//setup timers
//       SMCLK      /8     Up mode   interrupt enabled
TA0CTL = TASSEL_2 | ID_3 | MC_1    | TAIE;
//         higher priority interrupt enabled too
TA0CCTL0 = CCIE;

//initialise default pattern, defined in configuration.h
pattern_init();

//enable maskable interrupts globaly
WRITE_SR(GIE);
// starts timer, aiming for 250µs assuming a 16Mhz SMCLK 
TA0CCR0 = 0x01f4;

/* Go into Low power mode , main stops here.
CPU, MCLK are disabled, SMCLK, ACLK  and DCO
are active. */
_BIS_SR(LPM1_bits);

}

/* The function to move the pattens up 1 and enable/disable
cycle mode at the end. */
void rotate(void) {
  /* Disable Timer interrupt for duration
  of this function so we don't get a
  frame before the pattern is ready. */
  TA0CTL = TA0CTL - TAIE;

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
  // Timer interrupt back on
  TA0CTL = TA0CTL + TAIE;
  return;
}

/* The function to cycle patterns in cycle mode */
void cyclepattern(void) {
  if (!cycle) {
    return;
  }
#ifdef CYCLE_RANDOMLY
  uint8_t old = current_pattern;
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

/*The USI state machine function, can be
called by code to get out of IDLE and start
a transmission, or by USIServerRoutine()
when a sub-pixel transmission finishes and
it's time for another 8 bytes. */
void send(void) {
  static rgb24 pixel;

  switch (usi_state) {
    case USI_IDLE:
      led_index = 0;
      pixel = getled(led_index);
      USISRL = pixel.r;	//load red byte into SPI buffer
      USICTL1 = USICTL1 + USIIE;	//enable interrupt on tx completion
      USICNT = 8;			//Start send of 8 bits
      usi_state = USI_TXRED;
      return;
    case USI_TXRED:
      USISRL = pixel.g;	//load green byte into SPI buffer
      USICNT = 8;
      usi_state = USI_TXGREEN;
      return;
    case USI_TXGREEN:
      USISRL = pixel.b;	//load blue byte into SPI buffer
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
        pixel = getled(led_index);
        USISRL = pixel.r;	//load red byte into SPI buffer
        USICNT = 8;				//Start send of 8 bits
        usi_state = USI_TXRED;
        return;
      }
  }
}

/* This function shall be called very roughly every
250µs, one cycle before TimerA1ServerRoutine
is called. It shall also be called while that service
routine is running. This means I can use it to
increment timera_div. */
__attribute__((interrupt(TIMER0_A0_VECTOR))) void TimerA0ServiceRoutine(void) {
  //No need to clear CCIFG, it's done automatically.
  if (timera_div > DCO_CAL_DIV) {
    /* set red LED is this happens. It means the pattern
    frame function is too slow, it was still running when
    it was time for the next frame plus one timer wrap. */
    P1OUT = 1;
  }
  timera_div++;
  return;
}

/* This function shall be called very roughly every
250µs. Using this every somethingth iteration
gives 25 times per second. Calibrate with DCO_CAL_DIV*/
__attribute__((interrupt(TIMER0_A1_VECTOR))) void TimerA1ServerRoutine(void) {
  TA0CTL = TA0CTL - TAIFG; //reset Timer A interrupt flag
  if (timera_div < DCO_CAL_DIV) {
    return;
  }
  timera_div = 0;  

  if (usi_state != USI_IDLE) {
    /*set red LED if this happens. It means the pattern frame
    function is too slow, the USI is still transmitting when
    we call the frame function again. The string might look
    wrong. */
    P1OUT = 1;
  }

  if (CYCLE_TIME <= cycle_div++) { 
    cycle_div = 0;
    //time to move the pattern up if we are cycling
    cyclepattern();
  }

  /* Pet the watchdog
    __      _
  o'')}____//
   `_/      )
   (_(_/-(_/
  */
#ifndef DISABLE_WATCHDOG
  WDTCTL = WDT_ARST_1000;
#endif

  frame();   //triggers the current pattern to generate the next frame

  send();  //MUST BE AFTER FRAME FUNCTION IN PATTERN! MUST FINISH SENDING BEFORE NEXT CALL OF FRAME FUNCTION;
}

/* This function shall be called when the USI has
finished tranmising the data in it's buffer. */
__attribute__((interrupt(USI_VECTOR))) void USIServiceRoutine(void) {
  USICTL1 = USICTL1 - USIIFG;
  send();
}

/* This function shall be called when switch P1.3 (S2) is pressed down. */
__attribute__((interrupt(PORT1_VECTOR))) void Port1ServiceRoutine(void) {
  P1IFG = 0;
  rotate();
}
#endif
