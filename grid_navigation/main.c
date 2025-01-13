#include "grid.h"

#include <stdio.h>

int main() {
  struct Grid *const grid = grid_create(100, 100);
  grid_init(grid, "grid.pbm");

  /*  for(struct GridIterator* grid_iterator = grid_begin(grid); grid_iterator;
   * grid_iterator = grid_iterator_next(grid_iterator))*/
  /*  {*/
  /*    printf("====\n");*/
  /*    grid_iterator_print(grid_iterator);*/
  /*    printf("====\n");*/
  /*    for (int direction = 0; direction < NUM_GRID_DIRECTION_ALL;
   * ++direction)*/
  /*    {*/
  /*      struct GridIterator* neighbour_iterator =
   * grid_iterator_neighbour(grid_iterator, direction);*/
  /*      printf("  neighbour %i: ", direction);*/
  /*      if (neighbour_iterator)*/
  /*      {*/
  /*        grid_iterator_print(neighbour_iterator);*/
  /*        grid_iterator_destroy(neighbour_iterator);*/
  /*      }*/
  /*      else*/
  /*        printf("None\n");*/
  /*    }*/
  /*    printf("====\n\n");*/
  /*  }*/

  grid_export(grid, "grid_export.pbm");

  grid_destroy(grid);

  return 0;
}
