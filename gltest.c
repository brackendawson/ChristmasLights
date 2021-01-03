/*
 * export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/opt/X11/lib/pkgconfig # if on mac
 * gcc -c -o gltest.o  gltest.c -std=c99 -Wall `pkg-config --cflags gl` -I.
 * gcc -o gltest gltest.o `pkg-config --libs gl` -lglut
 */

#ifndef ARDUINO
#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

typedef uint8_t bool;
#define static_assert _Static_assert

/* Macro to match arduino function. */
#define random(MIN,MAX) ((MIN)+(rand()%((MAX)-(MIN))))

#include <colours.h>
#include <configuration.h>

void loop();

/* The function to move the pattens up 1 and enable/disable
cycle mode at the end. */
void rotate();
/* The function to cycle patterns in cycle mode */
void cyclepattern();
/* The function to send SPI data to the string, it
 will transmit the whole string and then return. */
void stringsend();

void keyboard(unsigned char key, int x, int y);

/*Dividers for the timing*/
#define DIV1_WRAP 500         //1ms to 40ms
unsigned int div1 = 0;
unsigned int div2 = 0;

void init() {
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glColor3f(1.0, 1.0, 0.0);

  // Set up the viewing volume: 500 x 50 x 1 window with origin lower left.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 500.0, 0.0, 50.0, 0.0, 1.0);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int timelow = 0;
int timehigh = 0;

int main(int argc, char** argv) {
  pattern_init();
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 50);
  glutInitWindowPosition(40, 40);
  glutCreateWindow("Christmas Lights Test");
  glutDisplayFunc(display);
  //glutReshapeFunc(reshape);
  glutIdleFunc(loop);
  glutKeyboardFunc((*keyboard));
  init();
  glutMainLoop();
}

void loop() {
  //every ~1ms
  ++div1;
  
  if (div1 >= DIV1_WRAP) {
    //every ~40ms ~25FPS
    div1=0;
    ++div2;
    frame();
    stringsend();
    
    if (div2 >= CYCLE_TIME) {
      //every ~600s
      div2=0;
      cyclepattern();
    }
  }
}

/* The function to send SPI data to the string, it
 will transmit the whole string and then return. */
void stringsend() {
  timelow = glutGet(GLUT_ELAPSED_TIME);
  glClear(GL_COLOR_BUFFER_BIT);

  float s = 500.0/NUM_LEDS;
  for(int i = 0 ; i < NUM_LEDS; i++ ) {
    rgb24 current_led = getled(i);
    float r = current_led.r/255.0;
    float g = current_led.g/255.0;
    float b = current_led.b/255.0;
    
    //printf("%d %06x = [%f, %f, %f]\n", i, (unsigned int)current_led, r, g, b);
    glBegin(GL_POLYGON);
      glColor3f(r, g, b);
      glVertex3f(i*s, 50, 0);
      glVertex3f((i+1)*s-1, 50, 0);
      glVertex3f((i+1)*s-1, 0, 0);
      glVertex3f(i*s, 0, 0);
    glEnd();
  }
  glFlush();
  // printf("loop\n");
  while (timehigh - timelow < 40) {
    timehigh = glutGet(GLUT_ELAPSED_TIME);
  }
  timehigh = glutGet(GLUT_ELAPSED_TIME);
}

/* The function to move the pattens up 1 and enable/disable
cycle mode at the end. */
void rotate(void) {
  if (cycle) {
    /* we are in cycle mode, stop that
    and go to static. */
    cycle = 0;
    current_pattern = 0;
  } else {
    /* we are not in cycle mode,
    incriment mode or enable cycle
    from mode 1. */
    if (NUM_PATTERNS <= current_pattern) {
      /* we are at the last pattern, enable
      cycle from pattern 1. */
      cycle = 1;
      current_pattern = 1;
    } else {
      current_pattern++;
    }
  }

  pattern_init();
  return;
}

/* The function to cycle patterns in cycle mode */
void cyclepattern(void) {
  if (!cycle) {
    return;
  }
#ifdef CYCLE_RANDOMLY
  uint8_t old = current_pattern;
  while (old == current_pattern) {
    current_pattern = random(1,NUM_PATTERNS+1);
  }
#else
  if (NUM_PATTERNS <= current_pattern) {
    current_pattern = 1;
  } else {
    current_pattern++;
  }
#endif
  pattern_init();
  printf("Changed to pattern %d\n", current_pattern);
  return;
}

void keyboard(unsigned char key, int x, int y) {
  if (key == ' ') {
    rotate();
  }
}

#endif
