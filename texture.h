#ifndef TEXTURE_H
#define TEXTURE_H

#include <stddef.h>

struct Texture;

struct Texture *texture_init(const char *const filename,
                             const size_t num_textures);
void texture_destroy(struct Texture *const texture);

size_t texture_get_size(const struct Texture *const texture,
                        size_t *const dimensions);
void texture_get(const struct Texture *const texture, const size_t index,
                 unsigned char *const buffer);

unsigned char *texture_get_ptr(const struct Texture *const texture,
                               const size_t index, size_t *const dimensions);

#endif
