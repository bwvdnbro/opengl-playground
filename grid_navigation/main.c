#include "grid.h"
#include "network_graph.h"

#include <stdio.h>

int main() {
  struct Grid *const grid = grid_create(100, 100);
  grid_init(grid, "grid.pbm");

  grid_export(grid, "grid_export.pbm");
  
  struct NetworkGraph* const network_graph = network_graph_create(grid);
  network_graph_export(network_graph, "grid_export.pgm");
  network_graph_destroy(network_graph);
  
  grid_destroy(grid);

  return 0;
}
