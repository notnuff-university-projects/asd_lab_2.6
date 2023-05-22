#ifndef INC_2_3_GRAPH_DRAW_H
#define INC_2_3_GRAPH_DRAW_H

#include <math.h>
#include <stdlib.h>
#include "consts_n_structures.h"
#include "X_routines.h"
#include "vec_math_lib.h"

void draw_graph_vertices(graph_t *graph);

void weight(int weight, int px, int py, int color);

void draw_arc(vec_4_t *vector, int offset_radius, edge_t *edge, weights_list_t **weights_to_draw);

void draw_line(vec_4_t *vector, edge_t *edge, weights_list_t **weights_to_draw);

void draw_graph_old(graph_t *graph, int **relation_matrix, int size, int red);

void draw_graph_weighted(graph_t *graph);


#endif //INC_2_3_GRAPH_DRAW_H
