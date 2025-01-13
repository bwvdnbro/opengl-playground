#include "network_graph.h"
#include "cell.h"
#include "grid.h"

#include <stdio.h>
#include <stdlib.h>

enum NetworkLinkType {
  NETWORK_LINK_NODE = 0,
  NETWORK_LINK_EDGE,
};

struct NetworkLink {
  int type;
  
  void* graph_component;
};

struct NetworkGraph {
  unsigned int size;

  struct NetworkGraphNode *nodes;
  struct NetworkGraphEdge *edges;
  
  struct Grid* source_grid;
};

struct NetworkGraphNode {};

struct NetworkGraphEdge {};

static struct NetworkLink* get_network_link(struct GridIterator* const grid_iterator)
{
  if (!grid_iterator)
    return NULL;
  struct Cell *const cell = grid_iterator_get_cell(grid_iterator);
  if (!cell)
    return NULL;
  struct NetworkLink* const link = (struct NetworkLink*)cell_get_network_link(cell);
  return link;
}

struct NetworkGraph *network_graph_create(struct Grid *const grid) {
  struct NetworkGraph *const graph =
      (struct NetworkGraph *)malloc(sizeof(struct NetworkGraph));

  // step 1: label each (road-like) cell as either a node or an edge
  graph->source_grid = grid;
  graph->size = 0;
  for (struct GridIterator *grid_iterator = grid_begin(grid); grid_iterator;
       grid_iterator = grid_iterator_next(grid_iterator)) {
    ++graph->size;
    struct Cell *const cell = grid_iterator_get_cell(grid_iterator);
    if (cell_get_type(cell) == CELL_TYPE_EMPTY)
      continue;

    unsigned int num_connections = 0;
    for (int direction = 0; direction < NUM_GRID_DIRECTION_PRINCIPAL;
         ++direction) {
      struct GridIterator *neighbour_iterator =
          grid_iterator_neighbour(grid_iterator, direction);

      if (!neighbour_iterator)
        continue;

      if (cell_get_type(grid_iterator_get_cell(neighbour_iterator)) ==
          CELL_TYPE_ROAD)
        ++num_connections;
      grid_iterator_destroy(neighbour_iterator);
    }
    struct NetworkLink *const link =
        (struct NetworkLink *)malloc(sizeof(struct NetworkLink));
    if (num_connections == 2)
      link->type = NETWORK_LINK_EDGE;
    else
      link->type = NETWORK_LINK_NODE;
    link->graph_component = NULL;
    cell_set_network_link(cell, link);
  }
  
  graph->nodes = (struct NetworkGraphNode*)malloc(graph->size*sizeof(struct NetworkGraphNode));
  graph->edges = (struct NetworkGraphEdge*)malloc(graph->size*sizeof(struct NetworkGraphEdge));

  // step 2: create actual nodes and edges by grouping together neighbouring cells of the same
  // type
  unsigned int last_node = 0;
  unsigned int last_edge = 0;
  for (struct GridIterator *grid_iterator = grid_begin(grid); grid_iterator;
       grid_iterator = grid_iterator_next(grid_iterator)) {
      
    struct NetworkLink* const link = get_network_link(grid_iterator);
    if (!link || link->graph_component)
      continue;
      
    if (link->type == NETWORK_LINK_NODE)
    {
      link->graph_component = &graph->nodes[last_node];
      ++last_node;
      continue;
    }
    
    link->graph_component = &graph->edges[last_edge];
    ++last_edge;
    
    // find connections
    // connect them
    struct GridIterator* left_ngb = NULL;
    struct GridIterator* right_ngb = NULL;
    for (int direction = 0; direction < NUM_GRID_DIRECTION_PRINCIPAL;
         ++direction) {
      struct GridIterator *neighbour_iterator =
          grid_iterator_neighbour(grid_iterator, direction);
      if (!neighbour_iterator)
        continue;
      struct NetworkLink* const link = get_network_link(neighbour_iterator);
      if (!link)
        continue;
      if (!left_ngb)
        left_ngb = neighbour_iterator;
      else
        right_ngb = neighbour_iterator;
    }
    
    // ...
    if(left_ngb)
    grid_iterator_destroy(left_ngb);
    if(right_ngb)
    grid_iterator_destroy(right_ngb);
  }

  return graph;
}

void network_graph_destroy(struct NetworkGraph *const network_graph) {

  for (struct GridIterator *grid_iterator = grid_begin(network_graph->source_grid); grid_iterator;
       grid_iterator = grid_iterator_next(grid_iterator)) {

    struct Cell *const cell = grid_iterator_get_cell(grid_iterator);
    struct NetworkLink* const link = (struct NetworkLink*)cell_get_network_link(cell);
    if (!link)
      continue;
    free(link);
    cell_set_network_link(cell, NULL);
  }
  free(network_graph->nodes);
  free(network_graph->edges);
  free(network_graph);
}

void network_graph_export(const struct NetworkGraph* const network_graph, const char* filename)
{
  FILE *const file = fopen(filename, "w");
  int nrow, ncol;
  grid_get_dimensions(network_graph->source_grid, &nrow, &ncol);
  fprintf(file, "P2\n# Created for debugging\n%u %u\n2\n", nrow, ncol);

  int *const pixels = (int *)calloc(nrow * ncol, sizeof(int));
  
  unsigned int icell = 0;
  for (struct GridIterator *grid_iterator = grid_begin(network_graph->source_grid); grid_iterator;
       grid_iterator = grid_iterator_next(grid_iterator)) {

    struct Cell *const cell = grid_iterator_get_cell(grid_iterator);
    struct NetworkLink* const link = (struct NetworkLink*)cell_get_network_link(cell);
    if (!link)
      pixels[icell] = 0;
    else
      pixels[icell] = link->type+1;
    ++icell;
  }
  
  unsigned int linesize = 0;
  for (icell = 0; icell < nrow * ncol; ++icell) {
    fprintf(file, "%i ", pixels[icell]);
    linesize += 2;
    if (linesize == 70) {
      fprintf(file, "\n");
      linesize = 0;
    }
  }
  free(pixels);

  fclose(file);
}
