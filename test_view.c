#include "character.h"
#include "error.h"
#include "model.h"

#include <GL/freeglut.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>

float get_time() {
  struct timespec ts;
  if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
    return 0.f;
  return ts.tv_sec + 1.e-9f * ts.tv_nsec;
}

float global_t0 = 0.f;
struct Model *global_model = NULL;

// Function to display animation
void display() {
  const float time = 10.f * (get_time() - global_t0);
  model_update(global_model, time);
  float *pos;
  size_t npos = model_get_positions(global_model, &pos, time);

  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0.f, 0.f, 0.f);
  glBegin(GL_POINTS);
  for (size_t ipos = 0; ipos < npos; ipos += 2) {
    LOG("Position: %g %g", pos[ipos], pos[ipos + 1]);
    glVertex2f(pos[ipos], pos[ipos + 1]);
  }
  glEnd();

  free(pos);

  glFlush();
  glutSwapBuffers();
  glutPostRedisplay();
}

void setup_model(struct Model *const model) {
  float box_size[2] = {100.f, 50.f};
  unsigned int seed = 42u;
  unsigned int r;

  model_init(model, box_size);

  for (int i = 0; i < 20; ++i) {
    struct Character *character = character_create();
    model_add_character(model, character);
    r = rand_r(&seed);
    character_init(character, r, box_size, 0.f);
  }
}

void setup_glut() {
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(1360, 768);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("test_view");

  glClearColor(1.f, 1.f, 1.f, 1.f);
  glPointSize(20.f);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0., 100., 0., 50.);

  glutDisplayFunc(display);
}

int main(int argc, char **argv) {
  global_t0 = get_time();
  LOG("Global t0: %g", global_t0);

  LOG("Initializing OpenGL");
  glutInit(&argc, argv);
  setup_glut();

  LOG("Creating model");
  global_model = model_create();
  setup_model(global_model);

  model_start(global_model, get_time() - global_t0);
  LOG("Main loop");
  glutMainLoop();

  LOG("Destroying model");
  model_destroy(global_model);
  return 0;
}
