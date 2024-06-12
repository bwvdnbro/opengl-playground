#include "model.h"
#include "character.h"
#include "error.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct Model *model_create() {
  struct Model *model = (struct Model *)malloc(sizeof(struct Model));
  model->map = NULL;
  model->characters = NULL;

  model->box_size[0] = 0.f;
  model->box_size[1] = 0.f;

  model->model_time = 0.f;
  model->last_program_time = 0.f;
  model->next_model_time = 0.f;

  model->last_character = NULL;
  model->num_characters = 0;

  return model;
}

void model_destroy(struct Model *model) {
  free(model->map);
  if (model->characters)
    character_destroy(model->characters);
  free(model);
  model = NULL;
}

void model_init(struct Model *const model, const float *box_size) {
  model->box_size[0] = box_size[0];
  model->box_size[1] = box_size[1];
}

void model_add_character(struct Model *const model,
                         struct Character *character) {
  character->next = NULL;
  if (!model->characters)
    model->characters = character;
  else {
    ASSERT(model->last_character, "Character bookkeeping is messed up!");
    ASSERT(model->last_character->next == NULL,
           "Character bookkeeping is messed up!");
    model->last_character->next = character;
  }
  model->last_character = character;
  ++model->num_characters;
}

void model_start(struct Model *const model, float program_time) {
  model->last_program_time = program_time;
  model_update(model, program_time);
}

void model_update(struct Model *const model, float program_time) {
  model->model_time += program_time - model->last_program_time;
  model->last_program_time = program_time;
  if (model->model_time < model->next_model_time)
    ;
  float min_next_time = FLT_MAX;
  struct Character *current = model->characters;
  while (current != NULL) {
    const float next_time =
        character_update(current, model->model_time, model->box_size);
    ASSERT(next_time >= model->model_time,
           "Next time is in the past - next_time: %g, model_time: %g",
           next_time, model->model_time);
    min_next_time = fminf(min_next_time, next_time);
    current = current->next;
  }
  model->next_model_time = min_next_time;
}

size_t model_get_positions(const struct Model *const model, float **positions,
                           float program_time) {
  const float time =
      model->model_time + program_time - model->last_program_time;
  size_t num_positions = 2 * model->num_characters;
  float *pos = (float *)calloc(num_positions, sizeof(float));
  size_t ichar = 0;
  struct Character *current = model->characters;
  while (current != NULL) {
    ASSERT(ichar < model->num_characters,
           "Character bookkeeping is messed up!");
    if (ichar == model->num_characters)
      break;
    ASSERT(2 * ichar + 1 < num_positions, "More characters than expected: %lu!",
           ichar);
    character_get_position(current, pos + 2 * ichar, time, model->box_size);
    current = current->next;
    ++ichar;
  }
  *positions = pos;
  return 2 * model->num_characters;
}
