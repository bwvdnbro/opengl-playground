#ifndef CHARACTER_H
#define CHARACTER_H

#include "log.h"

struct Character {
  /* linked list */
  struct Character *next;

  /* character data */
  float x, y;
  float vx, vy;
  float current_time;
  float next_time;

  /* auxiliary variables */
  unsigned int seed;
};

#define LOG_CHARACTER(c)                                                       \
  LOG("Character (%p):\n"                                                      \
      " next: %p\n"                                                            \
      " x: %.2f, y: %.2f\n"                                                    \
      " vx: %.2f, vy: %.2f\n"                                                  \
      " current_time: %.2f, next_time: %.2f\n"                                 \
      " seed: %u",                                                             \
      (void *)c, (void *)c->next, c->x, c->y, c->vx, c->vy, c->current_time,   \
      c->next_time, c->seed);

/* basics */
struct Character *character_create();
void character_destroy(struct Character *character);

/* initialization */
void character_init(struct Character *const character, unsigned int seed,
                    const float *box_size, float time);

/* modeling */
float character_update(struct Character *const character, float time,
                       const float *box_size);

/* querying */
void character_get_position(const struct Character *const character,
                            float *const position, float time,
                            const float *box_size);

#endif
