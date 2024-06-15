#include "character_sprite.h"
#include "error.h"
#include "log.h"
#include "texture.h"

#include <GL/freeglut.h>
#include <stdlib.h>

struct CharacterSprite {
  float x, y;
  float step;
  int direction;
};

struct CharacterSprite *character_sprite_create() {
  struct CharacterSprite *const character_sprite =
      (struct CharacterSprite *)malloc(sizeof(struct CharacterSprite));
  ASSERT(character_sprite, "Unable to allocate character sprite");
  character_sprite->x = 0.f;
  character_sprite->y = 0.f;
  character_sprite->step = 0.f;
  character_sprite->direction = -1;
  return character_sprite;
}

void character_sprite_destroy(struct CharacterSprite *const character_sprite) {
  free(character_sprite);
}

void character_sprite_update(struct CharacterSprite *const character_sprite,
                             const float x, const float y, const float step,
                             const int direction) {
  character_sprite->x = x;
  character_sprite->y = y;
  character_sprite->step = step;
  character_sprite->direction = direction;
}

static size_t get_direction_offset(const int direction) {
  ASSERT(direction >= 0 && direction < 8, "Invalid direction: %i", direction);
  switch (direction) {
  case 0:
    // west
    return 0u;
  case 1:
    // southwest
    return 56u;
  case 2:
    // south
    return 48u;
  case 3:
    // southeast
    return 40u;
  case 4:
    // east
    return 32u;
  case 5:
    // northeast
    return 25u;
  case 6:
    // north
    return 16u;
  case 7:
    // northwest
    return 8u;
  default:
    return 0;
  }
}

void character_sprite_draw(const struct CharacterSprite *const character_sprite,
                           const struct Texture *const texture) {
  LOG("Drawing sprite %p", (void *)character_sprite);
  const size_t offset = get_direction_offset(character_sprite->direction);
  const size_t step = ((size_t)(character_sprite->step * 8u)) % 8u;
  size_t dimensions[2];
  unsigned char *img = texture_get_ptr(texture, offset + step, dimensions);

  float position[2] = {character_sprite->x, character_sprite->y};
  glRasterPos2fv(position);
  glPixelZoom(1.f, -1.f);
  glDrawPixels(dimensions[0], dimensions[1], GL_RGBA, GL_UNSIGNED_BYTE, img);
}
