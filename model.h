#ifndef MODEL_H
#define MODEL_H

#include "stddef.h"

struct Model {
  /* data */
  struct Map *map;
  struct Character *characters;

  /* model state */
  float model_time;
  float last_program_time;

  /* bookkeeping */
  struct Character *last_character;
  size_t num_characters;
};

/* basics */
struct Model *model_create();
void model_destroy(struct Model *model);

/* character management */
void model_add_character(struct Model *const model,
                         struct Character *character);

/* model update */
void model_start(struct Model *const model, float program_time);
void model_update(struct Model *const model, float program_time,
                  const float *box_size);

/* model querying */
size_t model_get_positions(const struct Model *const model, float **positions,
                           float program_time, const float *box_size);

#endif
