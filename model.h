#ifndef MODEL_H
#define MODEL_H

#include "stddef.h"

struct Model {
  /* data */
  struct Map *map;
  struct Character *characters;

  /* model settings */
  float box_size[2];

  /* model state */
  float model_time;
  float last_program_time;
  float next_model_time;

  /* bookkeeping */
  struct Character *last_character;
  size_t num_characters;

  /* auxiliary variables */
  unsigned int seed;
};

/* basics */
struct Model *model_create();
void model_destroy(struct Model *model);

/* setup */
void model_init(struct Model *const model, const float *box_size,
                unsigned int seed);

/* character management */
void model_add_character(struct Model *const model, struct Character *character,
                         const float *position);

/* model update */
void model_start(struct Model *const model, float program_time);
void model_update(struct Model *const model, float program_time);

/* model querying */
size_t model_get_positions(const struct Model *const model, float **positions,
                           float program_time);

#endif
