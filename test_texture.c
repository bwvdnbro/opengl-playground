#include "error.h"
#include "texture.h"

#include <GL/freeglut.h>
#include <stdlib.h>
#include <time.h>

float get_time() {
  struct timespec ts;
  if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
    return 0.f;
  return ts.tv_sec + 1.e-9f * ts.tv_nsec;
}

float global_t0 = 0.f;
struct Texture *global_texture = NULL;

void test_texture(struct Texture *const texture) {
  size_t dimensions[2];
  const size_t num_textures = texture_get_size(texture, dimensions);
  ASSERT(num_textures == 64u, "Number of textures is wrong");
  ASSERT(dimensions[0] == 43u, "Width is wrong");
  ASSERT(dimensions[1] == 47u, "Height is wrong");
  unsigned char *img =
      (unsigned char *)calloc(dimensions[0] * dimensions[1] * 4, 1);
  texture_get(texture, 0u, img);
  size_t ipixel = 0;
  for (size_t ix = 0; ix < dimensions[0]; ++ix) {
    for (size_t iy = 0; iy < dimensions[1]; ++iy) {
      printf(" (%u %u %u %u)", img[4 * ipixel], img[4 * ipixel + 1],
             img[4 * ipixel + 2], img[4 * ipixel + 3]);
      ++ipixel;
    }
    printf("\n");
  }
  free(img);
}

void display() {
  const float time = get_time() - global_t0;
  const size_t index = ((size_t)(time * 10.f)) % 64;

  glClear(GL_COLOR_BUFFER_BIT);
  size_t dimensions[2];
  texture_get_size(global_texture, dimensions);
  unsigned char *img =
      (unsigned char *)calloc(dimensions[0] * dimensions[1] * 4, 1);
  texture_get(global_texture, index, img);

  float position[2] = {70.f, 50.f};
  glRasterPos2fv(position);
  glPixelZoom(-1.f, -1.f);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER, 0.5);
  glDrawPixels(dimensions[0], dimensions[1], GL_RGBA, GL_UNSIGNED_BYTE, img);

  glutSwapBuffers();
  free(img);
  glutPostRedisplay();
}

int main(int argc, char **argv) {
  global_t0 = get_time();

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // | GLUT_DEPTH);
  glutInitWindowSize(1360, 768);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("test_texture");

  glClearColor(1.f, 1.f, 1.f, 1.f);
  glPointSize(20.f);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0., 100., 0., 50.);

  global_texture = texture_init("texture.dat", 64u);
  //  test_texture(global_texture);

  glutDisplayFunc(display);
  glutMainLoop();

  texture_destroy(global_texture);

  return 0;
}
