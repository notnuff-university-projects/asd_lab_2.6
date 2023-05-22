#ifndef INC_2_3_GRAPH_CREATE_H
#define INC_2_3_GRAPH_CREATE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "consts_n_structures.h"

double **randm(int size1, int size2);
int **mulmr(double coefficient, double **double_mat, int **target_mat, int size1, int size2, int oriented);
int **mat_create(int size1, int size2);

point_t *tri_graph_create(point_t *tar_graph, int graph_size);
void free_mat (size_t **target_mat, int size2);
void print_mat(int **target_mat, int size1, int size2);
void mat_elem_by_elem(int **target_mat, int **source_mat, int size);
void mat_by_coefficient(double **target_mat, int coefficient, int size);

int **roundm(double **target_mat, int size);
int **D_mat_create(int **B_mat, int size);
int **C_mat_create(int **B_mat, int size);
int **B_mat_create(int **weight_mat, int size);
void weight_mat_final(int **weight_mat, int **C_mat, int **D_mat, int size);


#endif //INC_2_3_GRAPH_CREATE_H
