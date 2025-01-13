struct NetworkGraph;
struct NetworkGraphNode;
struct NetworkGraphEdge;

struct Grid;

struct NetworkGraph *network_graph_create( struct Grid *const grid);
void network_graph_destroy(struct NetworkGraph *const network_graph);

void network_graph_export(const struct NetworkGraph* const network_graph, const char* filename);
