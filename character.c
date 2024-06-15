#include "character.h"
#include "character_sprite.h"
#include "error.h"

#include <math.h>
#include <stdlib.h>

static const float max_dt = 100.f;

struct Character {
  struct CharacterSprite *sprite;

  /* character data */
  float x, y;
  float vx, vy;
  float step, vstep;
  float current_time;
  float next_time;
  int direction;

  /* auxiliary variables */
  unsigned int seed;
};

void character_log(struct Character *const character, char *const string) {
  sprintf(string,
          "Character (%p):\n"
          " sprite: %p\n"
          " x: %.2f, y: %.2f\n"
          " vx: %.2f, vy: %.2f\n"
          " step: %.2f, vstep: %.2f'n"
          " current_time: %.2f, next_time: %.2f\n"
          " seed: %u",
          (void *)character, (void *)character->sprite, character->x,
          character->y, character->vx, character->vy, character->step,
          character->vstep, character->current_time, character->next_time,
          character->seed);
}

static float get_max_dt_for_direction(const float position,
                                      const float velocity,
                                      const float box_size) {
  if (fabs(velocity) < 1.e-5)
    return max_dt;
  if (velocity < 0.f)
    return fminf(position / fabs(velocity), max_dt);
  else
    return fminf((box_size - position) / velocity, max_dt);
}

static float get_max_dt_for_box(const struct Character *const character,
                                const float *const box_size) {
  return fminf(
      get_max_dt_for_direction(character->x, character->vx, box_size[0]),
      get_max_dt_for_direction(character->y, character->vy, box_size[1]));
}

struct Character *character_create() {
  struct Character *const character =
      (struct Character *)malloc(sizeof(struct Character));
  ASSERT(character, "Unable to allocate Character!");

  character->sprite = character_sprite_create();

  character->x = 0.f;
  character->y = 0.f;
  character->vx = 0.f;
  character->vy = 0.f;
  character->step = 0.f;
  character->vstep = 0.f;
  character->current_time = 0.f;
  character->next_time = 0.f;
  character->direction = -1;

  character->seed = 0u;

  return character;
}

void character_destroy(struct Character *const character) {
  ASSERT(character, "Character was already destroyed!");
  character_sprite_destroy(character->sprite);
  free(character);
}

static void get_velocity(const int direction, float *const velocity) {
  ASSERT(direction >= 0 && direction < 8, "Invalid direction: %i", direction);
  switch (direction) {
  case 0:
    velocity[0] = -1.f;
    velocity[1] = 0.f;
    return;
  case 1:
    velocity[0] = -1.f;
    velocity[1] = -1.f;
    return;
  case 2:
    velocity[0] = 0.f;
    velocity[1] = -1.f;
    return;
  case 3:
    velocity[0] = 1.f;
    velocity[1] = -1.f;
    return;
  case 4:
    velocity[0] = 1.f;
    velocity[1] = 0.f;
    return;
  case 5:
    velocity[0] = 1.f;
    velocity[1] = 1.f;
    return;
  case 6:
    velocity[0] = 0.f;
    velocity[1] = 1.f;
    return;
  case 7:
    velocity[0] = -1.f;
    velocity[1] = 1.f;
    return;
  default:
    velocity[0] = 0.f;
    velocity[1] = 0.f;
    return;
  }
}

static void set_new_direction_and_time(struct Character *const character,
                                       const float *const box_size) {
  float next_dt;
  do {
    character->direction = rand_r(&character->seed) % 8u;
    float velocity[2];
    get_velocity(character->direction, velocity);
    character->vx = velocity[0];
    character->vy = velocity[1];
    const float u0 = ((float)rand_r(&character->seed)) / ((float)RAND_MAX + 1);
    next_dt = fminf(10.f * u0, get_max_dt_for_box(character, box_size));
  } while (next_dt < 1.e-5);
  character->next_time = character->current_time + next_dt;
}

void character_init(struct Character *const character, const unsigned int seed,
                    const float *const box_size, const float time) {
  ASSERT(character, "Character is NULL!");
  character->seed = seed;
  character->current_time = time;
  character->step = 0.f;
  character->vstep = 0.5f;
  set_new_direction_and_time(character, box_size);
}

void character_set_position(struct Character *const character,
                            const float *const position) {
  character->x = position[0];
  character->y = position[1];
}

void character_set_time(struct Character *const character, const float time) {
  character->current_time = time;
}

float character_update(struct Character *const character, const float time,
                       const float *const box_size) {
  ASSERT(character, "Character is NULL!");
  while (time > character->next_time) {
    const float dt = character->next_time - character->current_time;
    character->x =
        fmaxf(0.f, fminf(character->x + character->vx * dt, box_size[0]));
    character->y =
        fmaxf(0.f, fminf(character->y + character->vy * dt, box_size[1]));
    character->step += character->vstep * dt;
    while (character->step > 1.f)
      character->step -= 1.f;
    character->current_time = character->next_time;
    set_new_direction_and_time(character, box_size);
  }
  const float dt = time - character->current_time;
  ASSERT(dt >= 0.f, "Negative time step!");
  character->x += character->vx * dt;
  character->y += character->vy * dt;
  character->step += character->vstep * dt;
  while (character->step > 1.f)
    character->step -= 1.f;
  character->current_time = time;
  LOG_CHARACTER(character);
  return character->next_time;
}

void character_get_position(const struct Character *const character,
                            float *const position, const float time,
                            const float *const box_size) {
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

void *character_get_sprite(struct Character *const character, const float time,
                           const float *const box_size) {
  ASSERT(character, "Character is NULL!");
  const float dt = time - character->current_time;
  LOG("character: %p, dt: %.2f", (void *)character, dt);
  ASSERT(dt >= -1.e-5, "Negative time step - time: %.2f, current_time: %.2f!",
         time, character->current_time);
  const float x =
      fmaxf(0.f, fminf(character->x + character->vx * dt, box_size[0]));
  const float y =
      fmaxf(0.f, fminf(character->y + character->vy * dt, box_size[1]));
  float step = character->step + character->vstep * dt;
  while (step > 1.f)
    step -= 1.f;
  character_sprite_update(character->sprite, x, y, step, character->direction);
  return (void *)character->sprite;
}
