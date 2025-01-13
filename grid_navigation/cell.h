struct Cell;

struct Cell *cell_create_cells(const unsigned int num_cell);
void cell_destroy_cells(struct Cell *const cells);

struct Cell *cell_get_cell(struct Cell *const cells, const unsigned int offset);

void cell_print(const struct Cell *const cell);

enum CellType { CELL_TYPE_EMPTY = 0, CELL_TYPE_ROAD, NUM_CELL_TYPE };
int cell_get_type(const struct Cell *const cell);
void *cell_get_network_link(const struct Cell *const cell);
void cell_set_network_link(struct Cell *const cell, void *const link);

void cell_init_cells(struct Cell *const cells, const unsigned int num_cell,
                     const int *const values);
void cell_export_cells(const struct Cell *const cells,
                       const unsigned int num_cell, int *const values);
