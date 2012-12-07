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

unsigned char buffer[] = {RED,ORANGE,YELLOW,GREEN,BLUE,INDIGO};
unsigned char buffer_byte = 0;
unsigned char buffer_loops_left = 0;
#define BUFFER_LOOPS	17  // 100/6

unsigned char rotate = 0;

void send(unsigned char r, unsigned char g, unsigned char b);

int main(void) {

WDTCTL = WDTPW | WDTHOLD;     // Stop WDT
USICTL0 = USISWRST;           // Stop spamming the USI

/*setup clock, DCO to calibrated 1Mhz (apparently
calibration data for 8Mhz is not present on my
chip :-( ).*/
DCOCTL = CALDCO_1MHZ;
BCSCTL1 = CALBC1_1MHZ;

//setup USI
//        edges     interrupt enabe (this row clear USIIFG)
USICTL1 = USICKPH;// | USIIE;
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

/* Go into Low power mode , main stops here.
CPU, MCLK are disabled, SMCLK, ACLK  and DCO
are active. */
_BIS_SR(LPM1_bits);

}

/*sends the first byte down the string. Interrupt
driven code handles the rest. */
void startsend(void) {
  //enable USI interrput
  USICTL1 = USICTL1 + USIIE;
  //handle counters
  buffer_loops_left = BUFFER_LOOPS;
  //load the first byte
  USISRL = buffer[buffer_byte = 0];
  USICNT = 8;
}

/* This function should be called 25 times per second. */
interrupt (TIMERA1_VECTOR) TimerA1ServerRoutine(void) {
  TACTL = TACTL - TAIFG; //reset Timer A interrupt flag
  
  rotate++;
  //rotate the array by one every 25th pass
  if (12 == rotate) {
    rotate = 0;
    unsigned char storr = buffer[0];
    unsigned char storg = buffer[1];
    unsigned char storb = buffer[2];
    unsigned char i;
    for (i = 0 ; i < sizeof(buffer) - 3 ; i+=3) {
      buffer[i] = buffer[i+3];
      buffer[i+1] = buffer[i+4];
      buffer[i+2] = buffer[i+5];    
    }
    buffer[sizeof(buffer) - 3] = storr;
    buffer[sizeof(buffer) - 2] = storg;
    buffer[sizeof(buffer) - 1] = storb;
  }

  startsend();  //MUST BE AFTER ALL EDITS OF BUFFER!
}

/* This function should be called when the USI has
finished tranmising the data in it's buffer. */
interrupt (USI_VECTOR) USIServiceRoutine(void) {
  USICTL1 = USICTL1 - USIIFG;
  //handle counters
  if (buffer_byte >= sizeof(buffer) - 1) {
    buffer_byte = 0;
    buffer_loops_left--;
    //end condition
    if (0 == buffer_loops_left) {
      //disable interrupt
      USICTL1 = USICTL1 - USIIE;
      return;
    }
  } else {
    buffer_byte++;
  }
  //load a byte
  USISRL = buffer[buffer_byte];
  USICNT = 8;
}
