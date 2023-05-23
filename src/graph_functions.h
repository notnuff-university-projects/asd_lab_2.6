#ifndef INC_2_6_GRAPH_FUNCTIONS_H
#define INC_2_6_GRAPH_FUNCTIONS_H
#include "consts_n_structures.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
graph_t *graph_create(int size, int sides);
void vertex_add(graph_t *tar_graph, vertex_t *vertex);
void edges_add(edges_list_t **edges, edge_t *this_edge);
void print_graph(graph_t *graph);
vertex_t *graph_get_vertex (graph_t *graph, int index);
void graph_set_weights(graph_t *graph, int **rel_mat, int **weight_mat);
void graph_print_weights(graph_t *graph);
void graph_print_ver_weights(graph_t *graph);
void graph_clear(graph_t **graph);
void graph_clear_full(graph_t **graph);



#endif //INC_2_6_GRAPH_FUNCTIONS_H
