#include <msp430.h>
#include <signal.h>

//colors
#define	RED	0xFF,0x00,0x00
#define ORANGE	0xFF,0x20,0x00
#define YELLOW	0xFF,0x70,0x00
#define	GREEN	0x00,0xFF,0x00
#define BLUE	0x00,0x00,0xFF
#define INDIGO	0xFF,0x00,0x50
#define WHITE   0xFF,0xFF,0xFF

/* This flag is true while the USI is transmitting and is
cleared by interrupt. It must be false before another
transmission can begin. It must be volatile to stop GCC
optimizing it away.*/
_Bool volatile txing = 0;

void send(unsigned char r, unsigned char g, unsigned char b);

int main(void) {

WDTCTL = WDTPW | WDTHOLD;     // Stop WDT
USICTL0 = USISWRST;           // Stop spamming the USI

/*setup clock, DCO to calibrated 1Mhz (apparently
calibration data for 8Mhz is not programmed).*/
DCOCTL = CALDCO_1MHZ;
BCSCTL1 = CALBC1_1MHZ;

//setup USI
//        edges     interrupt enabe (this row clear USIIFG)
USICTL1 = USICKPH | USIIE;
//         SMCLK      no div
USICKCTL = USISSEL_2;
//        Output and clk             output enable   master    this reanables the USI
USICTL0 = USIPE7 | USIPE6 | USIPE5 | USIOE         | USIMST;

//setup timers
//      SMCLK      /0     Up mode   interrupt en
TACTL = TASSEL_2 | ID_0 | MC_1    | TAIE;
//enable global interrupts
WRITE_SR(GIE);
// starts timer, going for 40ms 
TACCR0 = 0x9C40;

unsigned char i;
for ( i = 0 ; i < 50 ; i++ ) {
  send(RED);
  send(ORANGE);
  send(YELLOW);
  send(GREEN);
  send(BLUE);
  send(INDIGO);
}

_BIS_SR(LPM4_bits);	//Go into Low power mode 4, main stops here

}

void send(unsigned char r, unsigned char g, unsigned char b) {
  //R
  //wait for USIIFG to be clear
  while (txing) {}
  USISRL = r;       //load the 8 "red" bits into the buffer
  txing = 1;        //Set the flag to stop thenext transmission
  USICNT = 8;       //tell the USI to clock 8 bits out
  //G
  while (txing) {}
  USISRL = g;
  txing = 1;
  USICNT = 8;
  //B
  while (txing) {}
  USISRL = b;
  txing = 1;
  USICNT = 8;
  return;
}

/* This function should be called 25 times per second. */
interrupt (TIMERA1_VECTOR) TimerA1ServerRoutine(void) {
  TACTL = TACTL - TAIFG; //reset Timer A interrupt flag
}

/* This function should be called when the USI has
finished tranmising the data in it's buffer. */
interrupt (USI_VECTOR) USIServiceRoutine(void) {
  USICTL1 = USICTL1 - USIIFG;
  txing = 0;
}
