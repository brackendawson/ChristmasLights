/* program to make a rainbow on the first 7 LEDs of
   my RGB LED Pixel string. */

#include <msp430.h>

#define	RED	0xFF0000
#define ORANGE	0xFF7F00
#define YELLOW	0xFFFF00
#define	GREEN	0x00FF00
#define BLUE	0x0000FF
#define INDIGO	0x4B0082
#define VIOLET	0x8F00FF

void send(unsigned long c);

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

//Push some data down the line
send(RED);
send(ORANGE);
send(YELLOW);
send(GREEN);
send(BLUE);
send(INDIGO);
send(VIOLET);

P1OUT = 1;

_BIS_SR(LPM4_bits);	//Go into Low power mode 4, main stops here

}

void send(unsigned long c) {
  //R
  //wait for USIIFG to be clear
  while (0 == (USICTL1 & 1)) {}
  USICTL1 = USICKPH | USIIE;
  USISRL = c >> 16; //load the 8 "red" bits into the buffer
  USICNT = 8;       //tell the USI to clock 8 bits out
  //G
  while (0 == (USICTL1 & 1)) {}
  USICTL1 = USICKPH | USIIE;
  USISRL = c >> 8  ;
  USICNT = 8;
  //B
  while (0 == (USICTL1 & 1)) {}
  USICTL1 = USICKPH | USIIE;
  USISRL = c;
  USICNT = 8;
  return;
}
