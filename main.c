/* program to make a rainbow on the first 7 LEDs of
   my RGB LED Pixel string. */

#include <msp430.h>

#define	RED	(31<<11)+(0*2<<5)+0
#define ORANGE	(31<<11)+(3*2<<5)+0
#define YELLOW	(31<<11)+(12*2<<5)+0
#define	GREEN	(0<<11)+(31*2<<5)+0
#define BLUE	(0<<11)+(0*2<<5)+31
#define INDIGO	(12<<11)+(0*2<<5)+8
#define WHITE	(31<<11)+(31*2<<5)+31
//               ^r       ^g       ^b  16-bit High Color

void send(unsigned int c);

int main(void) {

WDTCTL = WDTPW + WDTHOLD;     // Stop WDT



/*setup clock, DCO to calibrated 1Mhz (apparently
calibration data for 8Mhz is not programmed).*/
DCOCTL = CALDCO_1MHZ;
BCSCTL1 = CALBC1_1MHZ;

//setup USI
USICTL0 = USISWRST;
//        edges     interrupt enabe (this row clear USIIFG)
USICTL1 = USICKPH | USIIE;
//         SMCLK      no div
USICKCTL = USISSEL_2;
//        Output and clk             output enable   master
USICTL0 = USIPE7 | USIPE6 | USIPE5 | USIOE         | USIMST;

P1DIR = 255;

unsigned char i;
for ( i = 0 ; i < 50 ; i++ ) {
  send(RED);
  send(ORANGE);
  send(YELLOW);
  send(GREEN);
  send(BLUE);
  send(INDIGO);
}

P1OUT = 1;

_BIS_SR(LPM4_bits);	//Go into Low power mode 4, main stops here

}

void send(unsigned int c) {
  //R
  //wait for USIIFG to be clear
  while (0 == (USICTL1 & USIIFG)) {}
  USICTL1 = USICTL1 - USIIFG;
  USISRL = c >> 8 & 0xF8; //load the 8 "red" bits into the buffer
  USICNT = 8;       //tell the USI to clock 8 bits out
  //G
  while (0 == (USICTL1 & USIIFG)) {}
  USICTL1 = USICTL1 - USIIFG;
  USISRL = c >> 3 & 0xFC;
  USICNT = 8;
  //B
  while (0 == (USICTL1 & USIIFG)) {}
  USICTL1 = USICTL1 - USIIFG;
  USISRL = c << 3;
  USICNT = 8;
  return;
}
