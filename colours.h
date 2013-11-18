/*This header defines the colors, sub-pixel tables and the colour() function. */

#define BLACK	0	//  0   0   0
#define WHITE	1	//255 255 255
#define RED	2	//255   0   0
#define ORANGE	3	//255  32   0
#define YELLOW	4	//255 122   0
#define GREEN	5	//  0 255   0
#define BLUE	6	//  0   0 255
#define INDIGO	7	//255   0  80
#define NUM_COLOURS 8

/* These colours need 4 sub-pixel tables.
 I fancy 100 vlaue per table */

/* Generated by fade_table.pl 100 255 2.5 */
static const unsigned char SUBP_255[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  2,  2,  3,  3,  4,
    4,  5,  5,  6,  7,  8,  9,  9, 10, 11, 12, 13, 15, 16, 17, 18, 20, 21, 23, 24,
   26, 28, 29, 31, 33, 35, 37, 39, 41, 43, 46, 48, 50, 53, 56, 58, 61, 64, 66, 69,
   72, 75, 79, 82, 85, 89, 92, 96, 99,103,107,111,115,119,123,127,131,136,140,145,
  149,154,159,164,169,174,179,184,189,195,200,206,212,218,224,230,236,242,248,255};
/* Generated by fade_table.pl 100 112 2.5 */
static const unsigned char SUBP_112[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  //manually move 0-1 boundary to prevent flash of other colour before off.
    2,  2,  2,  2,  3,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8,  9, 10, 10,
   11, 12, 13, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 28, 29, 30,
   32, 33, 34, 36, 37, 39, 40, 42, 43, 45, 47, 48, 50, 52, 54, 55, 57, 59, 61, 63,
   65, 67, 69, 72, 74, 76, 78, 81, 83, 85, 88, 90, 93, 95, 98,101,103,106,109,112};
/* Generated by fade_table.pl 100 80 2.5 */
static const unsigned char SUBP_80[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  //manually move 0-1 boundary to prevent flash of other colour before off.
    1,  1,  1,  2,  2,  2,  2,  3,  3,  3,  4,  4,  4,  5,  5,  5,  6,  6,  7,  7,
    8,  8,  9,  9, 10, 11, 11, 12, 13, 13, 14, 15, 15, 16, 17, 18, 19, 20, 21, 21,
   22, 23, 24, 25, 26, 27, 29, 30, 31, 32, 33, 34, 36, 37, 38, 39, 41, 42, 44, 45,
   46, 48, 49, 51, 53, 54, 56, 57, 59, 61, 63, 64, 66, 68, 70, 72, 74, 76, 77, 80};
/* Generated by fade_table.pl 100 32 2.5 */
static const unsigned char SUBP_32[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  //manually move 0-1 boundary to prevent flash of other colour before off.
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  3,
    3,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,  6,  6,  6,  7,  7,  7,  8,  8,  8,
    9,  9,  9, 10, 10, 11, 11, 12, 12, 12, 13, 13, 14, 14, 15, 15, 16, 17, 17, 18,
   18, 19, 19, 20, 21, 21, 22, 23, 23, 24, 25, 25, 26, 27, 28, 28, 29, 30, 31, 32};

/* unsigned long colour(unsigned char colour, unsigned char brightness);

This function takes a colour defined in the table at the top of this file and a
brightness in the range of 0 to 99 where 0 is off and 99 (or higher)
is fully bright, the ramp is liner if the tables were generated with a good power.
It returns an nsigned long contining a 24-bit RGB value from LSB to bit 23, bits
24 to 31 are reserved. */
unsigned long colour(unsigned char colour, unsigned char brightness) {
  if (brightness > 99) {
    brightness = 99;
  }
  unsigned long value = 0;
  switch (colour) {
    case BLACK:
      break;
    case WHITE:
      value = SUBP_255[brightness];
      value = value << 8;
      value = value | SUBP_255[brightness];
      value = value << 8;
      value = value | SUBP_255[brightness];
      break;
    case RED:
      value = SUBP_255[brightness];
      value = value << 16;
      break;
    case ORANGE:
      value = SUBP_255[brightness];
      value = value << 8;
      value = value | SUBP_32[brightness];
      value = value << 8;
      break;
    case YELLOW:
      value = SUBP_255[brightness];
      value = value << 8;
      value = value | SUBP_112[brightness];
      value = value << 8;
      break;
    case GREEN:
      value = SUBP_255[brightness];
      value = value << 8;
      break;
    case BLUE:
      value = SUBP_255[brightness];
      break;
    case INDIGO:
      value = SUBP_255[brightness];
      value = value << 16;
      value = value | SUBP_80[brightness];
      break;
  }
  return(value);
}

