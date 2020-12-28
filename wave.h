//Definitions for this pattern
uint32_t (*wave_buffer)[12] = (uint32_t(*)[12]) &common_buffer;
static_assert(sizeof(wave_buffer) <= sizeof(common_buffer), "wave_buffer must fit in common_buffer");
uint8_t wave_fade_a;
uint8_t wave_fade_b;
uint8_t wave_col_a;
uint8_t wave_col_b;
bool wave_dir;

//functions for this pattern
uint32_t wave_colour(uint8_t col, uint8_t brt) {
  if (col > INDIGO) {
    col = (RED - 1) + (col - INDIGO);
  }
  return colour(col, brt);
}

//init function
void wave_init(void) {
  wave_dir = 1;
  wave_fade_a = 99;
  wave_fade_b = 19;
  wave_col_a = INDIGO;
  wave_col_b = INDIGO;
  return;
}

//frame function
void wave_frame(void) {
  if (wave_dir) {
    wave_fade_a -= 10;
    wave_fade_b += 10;
    if (wave_fade_a <= 19) {
      wave_dir = 0;
      wave_col_a--;
      if (wave_col_a < RED) {
        wave_col_a = INDIGO;
      }
    }
  } else {
    wave_fade_a += 10;
    wave_fade_b -= 10;
    if (wave_fade_a >= 99) {
      wave_dir = 1;
      wave_col_b--;
      if (wave_col_b < RED) {
        wave_col_b = INDIGO;
      }
    }
  }
  (*wave_buffer)[0] = wave_colour(wave_col_a, wave_fade_a);
  (*wave_buffer)[1] = wave_colour(wave_col_b, wave_fade_b);
  (*wave_buffer)[2] = wave_colour(wave_col_a+1, wave_fade_a);
  (*wave_buffer)[3] = wave_colour(wave_col_b+1, wave_fade_b);
  (*wave_buffer)[4] = wave_colour(wave_col_a+2, wave_fade_a);
  (*wave_buffer)[5] = wave_colour(wave_col_b+2, wave_fade_b);
  (*wave_buffer)[6] = wave_colour(wave_col_a+3, wave_fade_a);
  (*wave_buffer)[7] = wave_colour(wave_col_b+3, wave_fade_b);
  (*wave_buffer)[8] = wave_colour(wave_col_a+4, wave_fade_a);
  (*wave_buffer)[9] = wave_colour(wave_col_b+4, wave_fade_b);
  (*wave_buffer)[10] = wave_colour(wave_col_a+5, wave_fade_a);
  (*wave_buffer)[11] = wave_colour(wave_col_b+5, wave_fade_b);
  return;
}

//getled function
uint32_t wave_getled(uint8_t led) {
  return(*wave_buffer)[led%12];
}
