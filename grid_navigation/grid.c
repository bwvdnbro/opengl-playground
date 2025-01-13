#include "grid.h"
#include "cell.h"

#include <stdio.h>
#include <stdlib.h>

struct Grid {
  unsigned int nrow;
  unsigned int ncol;

  struct Cell *cells;
};

struct Grid *grid_create(const unsigned int nrow, const unsigned int ncol) {
  struct Grid *const grid = (struct Grid *)malloc(sizeof(struct Grid));
  grid->nrow = nrow;
  grid->ncol = ncol;
  grid->cells = cell_create_cells(nrow * ncol);
  return grid;
}

void grid_destroy(struct Grid *const grid) {
  cell_destroy_cells(grid->cells);
  free(grid);
}

void grid_get_dimensions(const struct Grid* const grid, unsigned int * const nrow, unsigned int * const ncol)
{
  *nrow = grid->nrow;
  *ncol = grid->ncol;
}

struct GridIterator {
  struct Grid *grid;

  unsigned int irow;
  unsigned int icol;
};

struct GridIterator *grid_begin(struct Grid *const grid) {
  struct GridIterator *const grid_iterator =
      (struct GridIterator *)malloc(sizeof(struct GridIterator));
  grid_iterator->grid = grid;
  grid_iterator->irow = 0;
  grid_iterator->icol = 0;
  return grid_iterator;
}

void grid_iterator_destroy(struct GridIterator *const grid_iterator) {
  free(grid_iterator);
}

struct GridIterator *
grid_iterator_next(struct GridIterator *const grid_iterator) {
  ++grid_iterator->icol;
  if (grid_iterator->icol < grid_iterator->grid->ncol)
    return grid_iterator;
  grid_iterator->icol = 0;
  ++grid_iterator->irow;
  if (grid_iterator->irow < grid_iterator->grid->nrow)
    return grid_iterator;
  free(grid_iterator);
  return NULL;
}

void grid_iterator_print(const struct GridIterator *const grid_iterator) {
  printf("Grid %ux%u: %u %u, ", grid_iterator->grid->nrow,
         grid_iterator->grid->ncol, grid_iterator->irow, grid_iterator->icol);
  cell_print(grid_iterator_get_cell(grid_iterator));
}

struct Cell *
grid_iterator_get_cell(const struct GridIterator *const grid_iterator) {
  return cell_get_cell(grid_iterator->grid->cells,
                       grid_iterator->irow * grid_iterator->grid->ncol +
                           grid_iterator->icol);
}

struct GridIterator *
grid_iterator_neighbour(const struct GridIterator *const grid_iterator,
                        const int direction) {
  unsigned int icol = grid_iterator->icol;
  unsigned int irow = grid_iterator->irow;
  switch (direction) {
  case GRID_DIRECTION_NORTH:
  case GRID_DIRECTION_NE:
  case GRID_DIRECTION_NW: {
    if (irow == 0)
      return NULL;
    --irow;
    break;
  }
  case GRID_DIRECTION_SOUTH:
  case GRID_DIRECTION_SE:
  case GRID_DIRECTION_SW: {
    ++irow;
    if (irow == grid_iterator->grid->nrow)
      return NULL;
    break;
  }
  }

  switch (direction) {
  case GRID_DIRECTION_EAST:
  case GRID_DIRECTION_NE:
  case GRID_DIRECTION_SE: {
    ++icol;
    if (icol == grid_iterator->grid->ncol)
      return NULL;
    break;
  }
  case GRID_DIRECTION_WEST:
  case GRID_DIRECTION_NW:
  case GRID_DIRECTION_SW: {
    if (icol == 0)
      return NULL;
    --icol;
    break;
  }
  }

  struct GridIterator *const neighbour_iterator =
      (struct GridIterator *)malloc(sizeof(struct GridIterator));
  neighbour_iterator->grid = grid_iterator->grid;
  neighbour_iterator->icol = icol;
  neighbour_iterator->irow = irow;
  return neighbour_iterator;
}

void grid_init(struct Grid *const grid, const char *filename) {
  FILE *const file = fopen(filename, "r");
  if (!file)
    return;

  unsigned int nrow, ncol;
  fscanf(file, "%*[^\n]\n%*[^\n]\n%u %u\n", &nrow, &ncol);
  printf("Size of grid in %s: %u %u\n", filename, nrow, ncol);

  if (grid->nrow != nrow || grid->ncol != ncol)
    return;

  int *const pixels = (int *)calloc(nrow * ncol, sizeof(int));
  for (unsigned int icell = 0; icell < nrow * ncol; ++icell) {
    char c = fgetc(file);
    if (c == '\n')
      c = fgetc(file);
    pixels[icell] = c - '0';
  }

  cell_init_cells(grid->cells, nrow * ncol, pixels);
  free(pixels);

  fclose(file);
}

void grid_export(const struct Grid *const grid, const char *filename) {
  FILE *const file = fopen(filename, "w");
  fprintf(file, "P1\n# Created for debugging\n%u %u\n", grid->nrow, grid->ncol);

  int *const pixels = (int *)calloc(grid->nrow * grid->ncol, sizeof(int));
  cell_export_cells(grid->cells, grid->nrow * grid->ncol, pixels);
  unsigned int linesize = 0;
  for (unsigned int icell = 0; icell < grid->nrow * grid->ncol; ++icell) {
    fprintf(file, "%i", pixels[icell]);
    ++linesize;
    if (linesize == 70) {
      fprintf(file, "\n");
      linesize = 0;
    }
  }
  free(pixels);

  fclose(file);
}
