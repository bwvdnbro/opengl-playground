#ifndef CHARACTER_H
#define CHARACTER_H

#include "log.h"

struct Character;

void character_log(struct Character *const character, char *const string);
#define LOG_CHARACTER(character)                                               \
  {                                                                            \
    char string[200];                                                          \
    character_log(character, string);                                          \
    LOG("%s", string);                                                         \
  }

/* basics */
struct Character *character_create();
void character_destroy(struct Character *const character);

/* initialization */
void character_init(struct Character *const character, const unsigned int seed,
                    const float *box_size, const float time);
void character_set_position(struct Character *const character,
                            const float *const position);
void character_set_time(struct Character *const character, const float time);

/* modeling */
float character_update(struct Character *const character, const float time,
                       const float *const box_size);

/* querying */
void character_get_position(const struct Character *const character,
                            float *const position, const float time,
                            const float *const box_size);

void *character_get_sprite(struct Character *const character, const float time,
                           const float *const box_size);

#endif
