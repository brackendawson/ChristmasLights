#define TETRIS_GAP 10
#define TETRIS_PIECES NUM_LEDS/TETRIS_GAP
#define TETRIS_LARGEST 4

typedef struct {
  unsigned char len;
  unsigned char col;
  unsigned char pos;
} tetris_piece;

tetris_piece tetris_pieces[TETRIS_PIECES];
unsigned char tetris_div;
unsigned char tetris_gap;

void tetris_init(void) {
  tetris_pieces[0].col = random(RED,INDIGO+1);
  tetris_pieces[0].len = random(1,TETRIS_LARGEST+1);
  tetris_pieces[0].pos = NUM_LEDS;
  for (unsigned char i = 1; i < TETRIS_PIECES; i++) {
    tetris_pieces[i].pos = 0;
  }
  tetris_div = 0;
  tetris_gap = 0;
  return;
}

void tetris_frame(void) {
  //only run twice a second
  if (tetris_div++ < 12) {
    return;
  }
  tetris_div = 0;

  //move pieces down
  for (unsigned char i = 0; i < TETRIS_PIECES; i++) {
    if (tetris_pieces[i].pos == 0) {
      continue;
    }
    tetris_pieces[i].pos--;
  }

  //spawn a piece if we can
  if (tetris_gap >= TETRIS_GAP) {
    for (unsigned char i = 0; i < TETRIS_PIECES; i++) {
      if (tetris_pieces[i].pos > 0) {
        continue;
      }
      tetris_pieces[i].col = random(RED,INDIGO+1);
      tetris_pieces[i].len = random(1,TETRIS_LARGEST+1);
      tetris_pieces[i].pos = NUM_LEDS;
      tetris_gap = 0;
      break;
    }
  } else {
    tetris_gap++;
  }

  return;
}

unsigned long tetris_getled(unsigned char led) {
  led += 1;
  for (unsigned char i = 0; i < TETRIS_PIECES; i++) {
    if (tetris_pieces[i].pos == 0) {
      continue;
    }
    if (led < tetris_pieces[i].pos) {
      continue;
    }
    if (led > tetris_pieces[i].pos + tetris_pieces[i].len - 1) {
      continue;
    }
    return colour(tetris_pieces[i].col, 99);
  }
  return 0;
}
