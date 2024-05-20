// C Program to illustrate
// OpenGL animation for revolution

#include <GL/freeglut.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

float get_time() {
  struct timespec ts;
  if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
    return 0.f;
  return ts.tv_sec + 1.e-9f * ts.tv_nsec;
}

float t0;
float t, prevt;
float prevfps, fps;

// Initialization function
void myInit(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0);

  glPointSize(3.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Set window size in X- and Y- direction
  gluOrtho2D(-780, 780, -420, 420);
}

void draw_frame(const float time) {
  const float v = 1.f + 0.1 * time;
  const float j = v * time;

  glBegin(GL_QUADS);
  for (int ix = -780; ix < 780; ix += 10)
    for (int iy = -420; iy < 420; iy += 10) {
      glColor4f(0.1 + 0.05 * cos(ix / 39.f),
                1.f - 0.1f * cos(ix / 78.f) * sin(iy / 42.f), 0.1, 1.0);
      glVertex2i(ix, iy);
      glVertex2i(ix, iy + 10);
      glVertex2i(ix + 10, iy + 10);
      glVertex2i(ix + 10, iy);
    }
  glEnd();

  glColor3f(0., 0., 0.);
  glBegin(GL_POINTS);

  // Iterate i up to 2*pi, i.e., 360 degree
  // plot point with slight increment in angle,
  // so, it will look like a continuous figure

  // Loop is to draw outer circle
  for (float i = 0; i < 6.29; i += 0.001) {
    int x = 200 * cos(i);
    int y = 200 * sin(i);
    glVertex2i(x, y);
  }

  // Loop to make orbit of revolution
  for (float i = 0; i < 6.29; i += 0.001) {
    int x = 600 * cos(i);
    int y = 100 * sin(i);
    glVertex2i(x, y);
  }
  glEnd();

  glBegin(GL_LINE_STRIP);
  const float x = -600 * cos(j);
  const float y = -100 * sin(j);
  const float w = 100;
  glVertex2i(x - w, y - w);
  glVertex2i(x - w, y + w);
  glVertex2i(x + w, y + w);
  glVertex2i(x + w, y - w);
  glVertex2i(x - w, y - w);
  glEnd();
}

// Function to display animation
void display(void) {
  const float target_frame_time = 1.f / 70.f;

  prevt = t;
  t = fmaxf(0.001f, get_time() - t0);
  fps = 1.f / (t - prevt);
  prevfps = 0.99f * prevfps + 0.01f * fps;
  glClear(GL_COLOR_BUFFER_BIT);
  draw_frame(t);
  glRasterPos2i(-770, -410);
  char status[100];
  sprintf(status, "t: %10.2f fps: %5.2f", t, prevfps);
  glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, status);
  glFlush();
  glutSwapBuffers();
  /*    // sleep approximately until a next frame is due
      const float frame_time = get_time() - t0 - t;
      if (frame_time < target_frame_time)
        usleep(1000000 * (target_frame_time - frame_time));
  */
  glutPostRedisplay();
}

void on_mouse_click(int button, int state, int x, int y) {
  printf("Mouse called!\n");
  fprintf(stderr, "mouse click: %i %i %i %i\n", button, state, x, y);
}

// Driver Program
int main(int argc, char **argv) {
  t0 = get_time();
  t = 0.f;
  prevfps = 60.f;
  glutInit(&argc, argv);

  // Display mode which is of RGB (Red Green Blue) type
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

  // Declares window size
  glutInitWindowSize(1360, 768);

  // Declares window position which is (0, 0)
  // means lower left corner will indicate position (0, 0)
  glutInitWindowPosition(0, 0);

  // Name to window
  glutCreateWindow("Revolution");

  // Call to myInit()
  myInit();
  glutMouseFunc(on_mouse_click);
  glutDisplayFunc(display);
  glutMainLoop();
}
