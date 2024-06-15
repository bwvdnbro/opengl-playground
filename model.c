#include "model.h"
#include "character.h"
#include "error.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct CharacterLink {
  struct Character *character;
  struct CharacterLink *next;
  struct CharacterLink *previous;
};

static struct CharacterLink *character_link_create() {
  struct CharacterLink *const character_link =
      (struct CharacterLink *)malloc(sizeof(struct CharacterLink));
  character_link->character = NULL;
  character_link->next = NULL;
  character_link->previous = NULL;
  return character_link;
}

static void character_link_destroy(struct CharacterLink *const character_link) {
  if (character_link->next)
    character_link_destroy(character_link->next);
  if (character_link->character)
    character_destroy(character_link->character);
  free(character_link);
}

struct CharacterList {
  struct CharacterLink *first_link;
  struct CharacterLink *last_link;
  size_t size;
};

static struct CharacterList *character_list_create() {
  struct CharacterList *const character_list =
      (struct CharacterList *)malloc(sizeof(struct CharacterList));
  character_list->first_link = NULL;
  character_list->last_link = NULL;
  character_list->size = 0u;
  return character_list;
}

static void character_list_destroy(struct CharacterList *const character_list) {
  ASSERT(character_list->first_link || character_list->size > 0u,
         "Invalid list!");
  if (character_list->first_link)
    character_link_destroy(character_list->first_link);
  free(character_list);
}

static void character_list_add(struct CharacterList *const character_list,
                               struct Character *const character) {
  struct CharacterLink *const new_link = character_link_create();
  new_link->character = character;
  if (!character_list->first_link) {
    ASSERT(!character_list->last_link && character_list->size == 0u,
           "Inconsistent list!");
    character_list->first_link = new_link;
    character_list->last_link = new_link;
  } else {
    ASSERT(character_list->last_link && character_list->size > 0u,
           "Inconsistent list!");
    character_list->last_link->next = new_link;
    new_link->previous = character_list->last_link;
    character_list->last_link = new_link;
  }
  ++character_list->size;
}

struct Model {
  /* data */
  struct CharacterList *characters;

  /* model settings */
  float box_size[2];

  /* model state */
  float model_time;
  float last_program_time;
  float next_model_time;

  /* auxiliary variables */
  unsigned int seed;
};

struct Model *model_create() {
  struct Model *const model = (struct Model *)malloc(sizeof(struct Model));
  model->characters = NULL;

  model->box_size[0] = 0.f;
  model->box_size[1] = 0.f;

  model->model_time = 0.f;
  model->last_program_time = 0.f;
  model->next_model_time = 0.f;

  model->seed = 0u;

  return model;
}

void model_destroy(struct Model *const model) {
  if (model->characters)
    character_list_destroy(model->characters);
  free(model);
}

void model_init(struct Model *const model, const float *const box_size,
                const unsigned int seed) {
  model->box_size[0] = box_size[0];
  model->box_size[1] = box_size[1];

  model->seed = seed;
}

static void generate_position(struct Model *const model,
                              float *const position) {
  const float u0 = ((float)rand_r(&model->seed)) / ((float)RAND_MAX + 1);
  const float u1 = ((float)rand_r(&model->seed)) / ((float)RAND_MAX + 1);
  position[0] = model->box_size[0] * u0;
  position[1] = model->box_size[1] * u1;
}

void model_add_character(struct Model *const model,
                         struct Character *const character,
                         const float *const position) {
  if (position)
    character_set_position(character, position);
  else {
    float random_position[2];
    generate_position(model, random_position);
    character_set_position(character, random_position);
  }
  character_set_time(character, model->model_time);
  if (!model->characters)
    model->characters = character_list_create();
  character_list_add(model->characters, character);
}

void model_start(struct Model *const model, const float program_time) {
  model->last_program_time = program_time;
  model_update(model, program_time);
}

void model_update(struct Model *const model, const float program_time) {
  model->model_time += program_time - model->last_program_time;
  model->last_program_time = program_time;
  if (model->model_time < model->next_model_time)
    return;
  float min_next_time = FLT_MAX;
  struct CharacterLink *current = model->characters->first_link;
  while (current != NULL) {
    const float next_time = character_update(
        current->character, model->model_time, model->box_size);
    ASSERT(next_time >= model->model_time,
           "Next time is in the past - next_time: %g, model_time: %g",
           next_time, model->model_time);
    min_next_time = fminf(min_next_time, next_time);
    current = current->next;
  }
  model->next_model_time = min_next_time;
}

size_t model_get_positions(const struct Model *const model,
                           float **const positions, const float program_time) {
  const float time =
      model->model_time + program_time - model->last_program_time;
  const size_t num_positions = 2 * model->characters->size;
  float *const pos = (float *)calloc(num_positions, sizeof(float));
  size_t ichar = 0u;
  struct CharacterLink *current = model->characters->first_link;
  while (current != NULL) {
    ASSERT(ichar < model->characters->size,
           "Character bookkeeping is messed up!");
    if (ichar == model->characters->size)
      break;
    ASSERT(2u * ichar + 1u < num_positions,
           "More characters than expected: %lu!", ichar);
    character_get_position(current->character, pos + 2u * ichar, time,
                           model->box_size);
    current = current->next;
    ++ichar;
  }
  *positions = pos;
  return 2u * model->characters->size;
}
