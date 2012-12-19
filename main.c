#include <msp430.h>
#include "colours.h"
#include "configuration.h"

//stuff for USI
unsigned char led_index;
//USI states
#define USI_IDLE	0	//USI not transmitting, USIIE is off
#define USI_TXRED	1	//USI is/was transmitting RED sub-pixel for LED[index]
#define USI_TXGREEN	2       //USI is/was transmitting GREEN sub-pixel for LED[index]
#define USI_TXBLUE	3	//USI is/was transmitting BLUE sub-pixel for LED[index]
unsigned char usi_state = USI_IDLE;

//Timer devider
unsigned char timera_div = 0; //to DCO_CAL_DIV

//Pattern cycle divider
unsigned int cycle_div = 0; //to CYCLE_TIME

void main(void) {

WDTCTL = WDTPW | WDTHOLD;     // Stop WDT
USICTL0 = USISWRST;           // Stop spamming the USI during init

/*setup clock, DCO to max DCO, about 25MHz */
//       DCOx         MODx
DCOCTL = 0b11100000 | 0b00011111;
//        no xtal  RSELx
BCSCTL1 = XT2OFF | 0b00001111;

//setup GPIO
P1DIR = 0b11110111;  //S2 (P1.3 is input)
P1IFG = 0;           //Clear interrupts
P1IES = 255;         //interrupt on falling edge
P1IE  = 0b00001000;  //Intettupt only for S2 (P1.3)
P1OUT = 0;

//setup USI
//        edges     interrupt disabled and USIIFG cleared by this
USICTL1 = USICKPH;
//         SMCLK      no div
USICKCTL = USISSEL_2 | USIDIV_3;
//        Output and clk    output enable   master    this reanables the USI by clearning USISWRST
USICTL0 = USIPE6 | USIPE5 | USIOE         | USIMST;

//setup timers
//       SMCLK      /8     Up mode   interrupt enabled
TA0CTL = TASSEL_2 | ID_3 | MC_1    | TAIE;
//         higher priority interrupt enabled too
TA0CCTL0 = CCIE;

//initialise default pattern, defined in configuration.h
init();

//enable maskable interrupts globaly
WRITE_SR(GIE);
// starts timer, aiming for 250us 
TA0CCR0 = 0x186A;

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

  // Timer interrupt back on
  TA0CTL = TA0CTL + TAIE;
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

/*The USI state machine function, can be
called by code to get out of IDLE and start
a transmission, or by USIServerRoutine()
when a sub-pixel transmission finishes and
it's time for another 8 bytes. */
void send(void) {
  switch (usi_state) {
    case USI_IDLE:
      led_index = 0;
      USISRL = getled(led_index) >> 16;	//load red byte into SPI buffer
      USICTL1 = USICTL1 + USIIE;	//enable interrupt on tx completion
      USICNT = 8;			//Start send of 8 bits
      usi_state = USI_TXRED;
      return;
    case USI_TXRED:
      USISRL = getled(led_index) >> 8;	//load green byte into SPI buffer
      USICNT = 8;
      usi_state = USI_TXGREEN;
      return;
    case USI_TXGREEN:
      USISRL = getled(led_index);	//load blue byte into SPI buffer
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
        USISRL = getled(led_index) >> 16;	//load red byte into SPI buffer
        USICNT = 8;				//Start send of 8 bits
        usi_state = USI_TXRED;
        return;
      }
  }
}

/* This function shall be called very roughly every
1 milisecond, one cycle before TimerA1ServerRoutine
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
1 milisecond. Using this every somethingth iteration
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

  frame();   //triggers the current pattern to generate the next frame

  send();  //MUST BE AFTER FRAME FUNCTION IN PATTERN! MUST FINISH SENDING BEFORE NEXT CALL OF FRAME FUNCTION;
}

/* This function shall be called when the USI has
finished tranmising the data in it's buffer. */
__attribute__((interrupt(USI_VECTOR))) void USIServiceRoutine(void) {
  USICTL1 = USICTL1 - USIIFG;
  send();
}

/* This function shall be called when switch P1.3 (S1) is pressed down. */
__attribute__((interrupt(PORT1_VECTOR))) void Port1ServiceRoutine(void) {
  P1IFG = 0;
  rotate();
}
