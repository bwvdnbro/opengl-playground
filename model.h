#ifndef MODEL_H
#define MODEL_H

#include "stddef.h"

struct Model {
  /* data */
  struct Map *map;
  struct Character *characters;

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
void model_update(struct Model *const model, float time, const float *box_size);

/* model querying */
size_t model_get_positions(const struct Model *const model, float **positions,
                           float time);

#endif
