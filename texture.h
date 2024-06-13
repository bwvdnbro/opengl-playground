#ifndef TEXTURE_H
#define TEXTURE_H

#include <stddef.h>

struct Texture {
  char *data;

  size_t num_textures;
  size_t width;
  size_t height;

  size_t offset_factor;
};

struct Texture *texture_init(const char *filename, size_t num_textures,
                             size_t width, size_t height);
void texture_destroy(struct Texture *const texture);

size_t texture_get_size(const struct Texture *const texture,
                        size_t *dimensions);
void texture_get(const struct Texture *const texture, size_t index,
                 char *buffer);

#endif
