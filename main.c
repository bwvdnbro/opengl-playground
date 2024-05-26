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

struct leg {
  union {
    struct {
      float foot[2];
      float knee[2];
    };
    float v[4];
  };
};

void interpolate(float f, const float *a, const float *b, float *result,
                 int size) {
  for (int i = 0; i < 2 * size; ++i) {
    result[i] = (1.0f - f) * a[i] + f * b[i];
  }
}

void draw_figure(double x, double y, double body_position) {
  const struct leg left_leg1 = {-20, 0, -10, 50};
  const struct leg right_leg1 = {20, 0, 10, 50};

  const struct leg left_leg2 = {-40, 0, -10, 50};
  const struct leg right_leg2 = {30, 20, 30, 50};

  const struct leg left_leg3 = {-50, 20, -20, 50};
  const struct leg right_leg3 = {0, 0, 0, 50};

  const int nframe = 4;
  const struct leg left_legs[4] = {left_leg2, left_leg3, right_leg2, right_leg3};
  const struct leg right_legs[4] = {right_leg2, right_leg3, left_leg2, left_leg3};

  const float body_fraction = body_position - (int)body_position;

  struct leg left_leg, right_leg;
  const int index = (int)(nframe * body_fraction);
  const float f = nframe * body_fraction - index;
  const int index1 = (index + 1) % nframe;
  interpolate(f, left_legs[index].v, left_legs[index1].v, left_leg.v, 2);
  interpolate(f, right_legs[index].v, right_legs[index1].v, right_leg.v, 2);

  glColor3f(0., 0., 0.);
  glBegin(GL_LINE_STRIP);
  glVertex2i(x, y + 100);
  glVertex2i(x + left_leg.knee[0], y + left_leg.knee[1]);
  glVertex2i(x + left_leg.foot[0], y + left_leg.foot[1]);
  glVertex2i(x + left_leg.foot[0] + 20, y + left_leg.foot[1]);
  glEnd();
  glBegin(GL_LINE_STRIP);
  glVertex2i(x, y + 100);
  glVertex2i(x + right_leg.knee[0], y + right_leg.knee[1]);
  glVertex2i(x + right_leg.foot[0], y + right_leg.foot[1]);
  glVertex2i(x + right_leg.foot[0] + 20, y + right_leg.foot[1]);
  glEnd();
  glBegin(GL_LINES);
  /* torso */
  glVertex2i(x, y + 100);
  glVertex2i(x, y + 200);
  /* arm 1 */
  glVertex2i(x, y + 200);
  glVertex2i(x - 20, y + 150);
  /* arm 2 */
  glVertex2i(x, y + 200);
  glVertex2i(x + 20, y + 150);
  glEnd();
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < 1000; ++i) {
    glVertex2i(x + 20 * cos(0.002 * M_PI * i),
               y + 220 + 20 * sin(0.002 * M_PI * i));
  }
  glEnd();
}

void draw_frame(const float time) {
  const float ground_speed = 160.f;
  const float body_speed = 1.5;
  float position = -750 + ground_speed * time;
  while (position > 750)
    position -= 1500.f;
  const float body_position = body_speed * time;

  draw_figure(position - 50, 0, body_position);
  draw_figure(position + 50, 0, body_position+0.3);
}

// Function to display animation
void display(void) {
  const float target_frame_time = 1.f / 70.f;

  prevt = t;
  t = fmaxf(0.001f, get_time() - t0);
  fps = 1.f / (t - prevt + 0.001f);
  prevfps = 0.99f * prevfps + 0.01f * fps;
  printf("prevt: %g, t: %g, fps: %g, prevfps: %g\n", prevt, t, fps, prevfps);
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
