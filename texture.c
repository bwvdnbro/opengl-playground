#include "texture.h"
#include "error.h"

#include <stdlib.h>
#include <string.h>

struct Texture *texture_init(const char *filename, size_t num_textures,
                             size_t width, size_t height) {
  struct Texture *texture = (struct Texture *)malloc(sizeof(struct Texture));

  texture->num_textures = num_textures;
  texture->width = width;
  texture->height = height;

  texture->offset_factor = width * height * 3;

  const size_t data_size = num_textures * width * height * 3;
  texture->data = (char *)calloc(data_size, 1);
  FILE *file = fopen(filename, "rb");
  ASSERT(file, "Unable to open texture file %s", filename);
  const size_t size_read = fread(texture->data, 1, data_size, file);
  ASSERT(size_read == data_size, "Unable to read texture file %s", filename);

  return texture;
}

void texture_destroy(struct Texture *const texture) {
  free(texture->data);
  free(texture);
}

size_t texture_get_size(const struct Texture *const texture,
                        size_t *dimensions) {
  dimensions[0] = texture->width;
  dimensions[1] = texture->height;
  return texture->num_textures;
}

void texture_get(const struct Texture *const texture, size_t index,
                 char *buffer) {
  memcpy(texture->data + index * texture->offset_factor, buffer,
         texture->offset_factor);
}
