#ifndef CHARACTER_SPRITE_H
#define CHARACTER_SPRITE_H

struct CharacterSprite;
struct Texture;

struct CharacterSprite *character_sprite_create();
void character_sprite_destroy(struct CharacterSprite *const character_sprite);

void character_sprite_update(struct CharacterSprite *const character_sprite,
                             const float x, const float y, const float step,
                             const int direction);

void character_sprite_draw(const struct CharacterSprite *const character_sprite,
                           const struct Texture *const texture);

#endif
