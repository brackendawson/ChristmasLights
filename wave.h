uint8_t wave_pos;
uint8_t wave_brt;

void wave_init(void) {
  wave_pos = 0;
  wave_brt = 0;
  return;
}

void wave_frame(void) {
  if (wave_brt < 99) {
    wave_brt += 10;
  } else {
    wave_brt = 0;
    wave_pos = (wave_pos+1)%((INDIGO-RED+1)*2);
  }
  return;
}

rgb24 wave_getled(uint8_t led) {
  led+=INDIGO*2;
  if ((led-wave_pos)%2==0) {
    return(colour(((led-wave_pos)%((INDIGO-RED+1)*2))/2+RED,100-wave_brt));
  }
  return(colour(((led-wave_pos)%((INDIGO-RED+1)*2))/2+RED,wave_brt));
}
