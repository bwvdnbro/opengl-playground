#include "texture.h"
#include "error.h"
#include "log.h"

#include <stdlib.h>
#include <string.h>

struct Texture {
  unsigned char *data;

  size_t num_textures;
  size_t width;
  size_t height;

  size_t offset_factor;
};

struct Texture *texture_init(const char *const filename,
                             const size_t num_textures) {
  struct Texture *const texture =
      (struct Texture *)malloc(sizeof(struct Texture));

  FILE *const file = fopen(filename, "rb");
  ASSERT(file, "Unable to open texture file %s", filename);
  size_t sizes[4];
  size_t size_read = fread(sizes, sizeof(size_t), 4u, file);
  ASSERT(size_read == 4u, "Unable to read texture file %s", filename);
  LOG_ALWAYS("Texture sizes: %lu %lu %lu %lu", sizes[0], sizes[1], sizes[2],
             sizes[3]);
  ASSERT(sizes[0] == num_textures, "Wrong number of textures");
  ASSERT(sizes[3] == 4u, "Wrong number of color channels");

  texture->num_textures = num_textures;
  texture->width = sizes[2];
  texture->height = sizes[1];

  texture->offset_factor = sizes[1] * sizes[2] * 4u;

  const size_t data_size = num_textures * sizes[1] * sizes[2] * 4u;
  texture->data = (unsigned char *)calloc(data_size, 1u);
  size_read = fread(texture->data, 1u, data_size, file);
  ASSERT(size_read == data_size, "Unable to read texture file %s", filename);

  return texture;
}

void texture_destroy(struct Texture *const texture) {
  free(texture->data);
  free(texture);
}

size_t texture_get_size(const struct Texture *const texture,
                        size_t *const dimensions) {
  dimensions[0] = texture->width;
  dimensions[1] = texture->height;
  return texture->num_textures;
}

void texture_get(const struct Texture *const texture, const size_t index,
                 unsigned char *const buffer) {
  memcpy(buffer, texture->data + index * texture->offset_factor,
         texture->offset_factor);
}

unsigned char *texture_get_ptr(const struct Texture *const texture,
                               const size_t index, size_t *const dimensions) {
  dimensions[0] = texture->width;
  dimensions[1] = texture->height;
  return texture->data + index * texture->offset_factor;
}
