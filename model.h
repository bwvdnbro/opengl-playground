#ifndef MODEL_H
#define MODEL_H

#include "stddef.h"

struct Character;
struct Model;

/* basics */
struct Model *model_create();
void model_destroy(struct Model *const model);

/* setup */
void model_init(struct Model *const model, const float *const box_size,
                const unsigned int seed);

/* character management */
void model_add_character(struct Model *const model,
                         struct Character *const character,
                         const float *const position);

/* model update */
void model_start(struct Model *const model, const float program_time);
void model_update(struct Model *const model, const float program_time);

/* model querying */
size_t model_get_positions(const struct Model *const model,
                           float **const positions, const float program_time);

#endif
