#include "character.h"
#include "error.h"

#include <math.h>
#include <stdlib.h>

static const float max_dt = 100.f;

static float get_max_dt_for_direction(float position, float velocity,
                                      float box_size) {
  if (fabs(velocity) < 1.e-5)
    return max_dt;
  if (velocity < 0.f)
    return fminf(position / fabs(velocity), max_dt);
  else
    return fminf((box_size - position) / velocity, max_dt);
}

static float get_max_dt_for_box(const struct Character *const character,
                                const float *box_size) {
  return fminf(
      get_max_dt_for_direction(character->x, character->vx, box_size[0]),
      get_max_dt_for_direction(character->y, character->vy, box_size[1]));
}

struct Character *character_create() {
  struct Character *character =
      (struct Character *)malloc(sizeof(struct Character));
  ASSERT(character, "Unable to allocate Character!");
  character->next = NULL;

  character->x = 0.f;
  character->y = 0.f;
  character->vx = 0.f;
  character->vy = 0.f;
  character->current_time = 0.f;
  character->next_time = 0.f;
  character->direction = 0;

  character->seed = 0;

  return character;
}

void character_destroy(struct Character *character) {
  ASSERT(character, "Character was already destroyed!");
  if (character->next)
    character_destroy(character->next);
  free(character);
  character = NULL;
}

static void get_velocity(int direction, float *velocity) {
  switch (direction) {
  case 0:
    velocity[0] = -1.f;
    velocity[1] = 0.f;
    break;
  case 1:
    velocity[0] = -1.f;
    ;
    velocity[1] = -1.f;
    break;
  case 2:
    velocity[0] = 0.f;
    velocity[1] = -1.f;
    break;
  case 3:
    velocity[0] = 1.f;
    velocity[1] = -1.f;
    break;
  case 4:
    velocity[0] = 1.f;
    velocity[1] = 0.f;
    break;
  case 5:
    velocity[0] = 1.f;
    velocity[1] = 1.f;
    break;
  case 6:
    velocity[0] = 0.f;
    velocity[1] = 1.f;
    break;
  case 7:
    velocity[0] = -1.f;
    velocity[1] = 1.f;
    break;
  default:
    velocity[0] = 0.f;
    velocity[1] = 0.f;
    break;
  }
  return;
}

static void set_new_direction_and_time(struct Character *const character,
                                       const float *box_size) {
  float next_dt;
  do {
    character->direction = rand_r(&character->seed) % 8;
    float velocity[2];
    get_velocity(character->direction, velocity);
    character->vx = velocity[0];
    character->vy = velocity[1];
    const float u0 = ((float)rand_r(&character->seed)) / ((float)RAND_MAX + 1);
    next_dt = fminf(10. * u0, get_max_dt_for_box(character, box_size));
  } while (next_dt < 1.e-5);
  character->next_time = character->current_time + next_dt;
}

void character_init(struct Character *const character, unsigned int seed,
                    const float *box_size, float time) {
  ASSERT(character, "Character is NULL!");
  character->seed = seed;
  character->current_time = time;
  set_new_direction_and_time(character, box_size);
}

void character_set_position(struct Character *const character,
                            const float *position) {
  character->x = position[0];
  character->y = position[1];
}

float character_update(struct Character *const character, float time,
                       const float *box_size) {
  ASSERT(character, "Character is NULL!");
  while (time > character->next_time) {
    const float dt = character->next_time - character->current_time;
    character->x =
        fmaxf(0.f, fminf(character->x + character->vx * dt, box_size[0]));
    character->y =
        fmaxf(0.f, fminf(character->y + character->vy * dt, box_size[1]));
    character->current_time = character->next_time;
    set_new_direction_and_time(character, box_size);
  }
  const float dt = time - character->current_time;
  ASSERT(dt >= 0.f, "Negative time step!");
  character->x += character->vx * dt;
  character->y += character->vy * dt;
  character->current_time = time;
  LOG_CHARACTER(character);
  return character->next_time;
}

void character_get_position(const struct Character *const character,
                            float *const position, float time,
                            const float *box_size) {
  ASSERT(character, "Character is NULL!");
  const float dt = time - character->current_time;
  LOG("character: %p, dt: %.2f", (void *)character, dt);
  ASSERT(dt >= -1.e-5, "Negative time step - time: %.2f, current_time: %.2f!",
         time, character->current_time);
  position[0] =
      fmaxf(0.f, fminf(character->x + character->vx * dt, box_size[0]));
  position[1] =
      fmaxf(0.f, fminf(character->y + character->vy * dt, box_size[1]));
}
