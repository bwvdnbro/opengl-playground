#include "cell.h"

#include <stdio.h>
#include <stdlib.h>

struct Cell {
  int type;
  void *network_link;
};

struct Cell *cell_create_cells(const unsigned int num_cell) {
  struct Cell *const cells =
      (struct Cell *)malloc(num_cell * sizeof(struct Cell));
  for (unsigned int icell = 0; icell < num_cell; ++icell) {
    cells[icell].type = CELL_TYPE_EMPTY;
    cells[icell].network_link = NULL;
  }
  return cells;
}

void cell_destroy_cells(struct Cell *const cells) { free(cells); }

struct Cell *cell_get_cell(struct Cell *const cells,
                           const unsigned int offset) {
  return &cells[offset];
}

void cell_print(const struct Cell *const cell) {
  printf("Type: %i\n", cell->type);
}

int cell_get_type(const struct Cell *const cell) { return cell->type; }

void *cell_get_network_link(const struct Cell *const cell) {
  return cell->network_link;
}

void cell_set_network_link(struct Cell *const cell, void *const link) {
  cell->network_link = link;
}

void cell_init_cells(struct Cell *const cells, const unsigned int num_cell,
                     const int *const values) {
  for (unsigned int icell = 0; icell < num_cell; ++icell) {
    cells[icell].type = values[icell];
  }
}

void cell_export_cells(const struct Cell *const cells,
                       const unsigned int num_cell, int *const values) {
  for (unsigned int icell = 0; icell < num_cell; ++icell) {
    values[icell] = cells[icell].type;
  }
}
