struct Grid;
struct GridIterator;

struct Cell;

struct Grid *grid_create(const unsigned int nrow, const unsigned int ncol);
void grid_destroy(struct Grid *const grid);

void grid_get_dimensions(const struct Grid* const grid, unsigned int * const nrow, unsigned int * const ncol);

struct GridIterator *grid_begin(struct Grid *const grid);
void grid_iterator_destroy(struct GridIterator *const grid_iterator);
struct GridIterator *
grid_iterator_next(struct GridIterator *const grid_iterator);
void grid_iterator_print(const struct GridIterator *const grid_iterator);
struct Cell *
grid_iterator_get_cell(const struct GridIterator *const grid_iterator);

enum GridDirectionPrincipal {
  GRID_DIRECTION_NORTH = 0,
  GRID_DIRECTION_EAST,
  GRID_DIRECTION_SOUTH,
  GRID_DIRECTION_WEST,
  NUM_GRID_DIRECTION_PRINCIPAL,
};

enum GridDirectionDiagonal {
  GRID_DIRECTION_NE = NUM_GRID_DIRECTION_PRINCIPAL,
  GRID_DIRECTION_SE,
  GRID_DIRECTION_SW,
  GRID_DIRECTION_NW,
  NUM_GRID_DIRECTION_ALL,
};

struct GridIterator *
grid_iterator_neighbour(const struct GridIterator *const grid_iterator,
                        const int direction);

void grid_init(struct Grid *const grid, const char *filename);
void grid_export(const struct Grid *const grid, const char *filename);
