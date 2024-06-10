#include "character.h"
#include "error.h"
#include "model.h"

#include "stdio.h"
#include "stdlib.h"

int main() {
  printf("Creating model...\n");
  struct Model *model = model_create();

  float box_size[2] = {100.f, 50.f};
  unsigned int seed = 42u;
  unsigned int r;

  printf("Adding first character...\n");
  struct Character *character = character_create();
  model_add_character(model, character);
  r = rand_r(&seed);
  character_init(character, r, box_size, 0.f);

  float *positions = NULL;
  size_t num_positions = model_get_positions(model, &positions, 0.f, box_size);
  printf("num_positions: %lu\n", num_positions);
  for (size_t i = 0; i < num_positions; i += 2)
    printf("  %.2f %.2f\n", positions[i], positions[i + 1]);
  ASSERT(num_positions == 2u, "Wrong number of positions: %lu", num_positions);
  free(positions);
  positions = NULL;

  printf("Adding second character...\n");
  struct Character *character2 = character_create();
  model_add_character(model, character2);
  r = rand_r(&seed);
  character_init(character2, r, box_size, 0.f);

  num_positions = model_get_positions(model, &positions, 1.f, box_size);
  printf("num_positions: %lu\n", num_positions);
  for (size_t i = 0; i < num_positions; i += 2)
    printf("  %.2f %.2f\n", positions[i], positions[i + 1]);
  ASSERT(num_positions == 4u, "Wrong number of positions: %lu", num_positions);
  free(positions);

  printf("=== BEGIN ===\n");
  float time = 0.f;
  for (int i = 0; i < 100; ++i) {
    model_update(model, time, box_size);
    for (int j = 0; j < 100; ++j) {
      num_positions = model_get_positions(model, &positions, time, box_size);
      printf("%g\t%g\t%g\t%g\n", positions[0], positions[1], positions[2],
             positions[3]);
      free(positions);
      positions = NULL;
      time += 0.1;
    }
  }
  printf("=== END ===\n");

  model_destroy(model);
  return 0;
}
