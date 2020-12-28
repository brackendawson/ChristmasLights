/*This file impliments some language used for
 arduino in MSP430, as best we can. */
#define bool _Bool
#define static_assert _Static_assert

/* Generated by noise_table.pl 24 256 */
static const uint8_t msp430_noise[] = {
     46,113,236, 44, 77,116,251,243,115,236,123, 17,208, 30, 87,208,176, 88,  0, 36,
     163, 84, 36,229};
uint8_t msp430_noise_i = 0;

/* Stand in for arduino random(), as we can't do
 function overloading we're only implimenting
 the two argument version. */
long random(long min, long max) {
  return min + (msp430_noise[msp430_noise_i++ % sizeof(msp430_noise)] % (max - min));
}
