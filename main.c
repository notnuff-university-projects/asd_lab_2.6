// other libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "src/consts_n_structures.h"
#include "src/matrix_math.h"
#include "src/vec_math_lib.h"
#include "src/X_routines.h"
#include "src/graph_draw.h"
#include "src/graph_functions.h"

int n;

int main() {
    int n1 = 2, n2 = 1, n3 = 2, n4 = 5;
    srand(n1 * 1000 + n2 * 100 + n3 * 10 + n4);

    n = 10 + n3;
    int sides = 3;
    double c = 1.0 - n3 * 0.01 - n4 * 0.005 - 0.05;
    double **service_mat;
    service_mat = randm(n, n);
    int **rel_mat;
    rel_mat = mat_create(n, n);
    rel_mat = mulmr(c, service_mat, rel_mat, n, n, 0);

    mat_by_coefficient(service_mat, 100, n);

    int **weight_mat = roundm(service_mat, n);

    mat_elem_by_elem(weight_mat, rel_mat, n);

    int **B_mat = B_mat_create(weight_mat, n);
    int **C_mat = C_mat_create(B_mat, n);
    int **D_mat = D_mat_create(B_mat, n);
    weight_mat_final(weight_mat, C_mat, D_mat, n);


    printf("\n");
    printf("1. Printing relation matrix:\n");
    print_mat(rel_mat, n, n);

    printf("\n");
    printf("2. Printing weight matrix:\n");
    print_mat(weight_mat, n, n);

    graph_t *new_graph = NULL;
    new_graph = graph_create(n, sides);
    graph_set_weights(new_graph, rel_mat, weight_mat);
    graph_print_ver_weights(new_graph);
    init_x();//window created

    while (1) {
        XNextEvent(dis, &event);
        switch (event.type) {
            case Expose:
                if (event.xexpose.count == 0) redraw_x();
                break;
            case KeyPress:
                if (XLookupString(&event.xkey, text, 255, &key, 0) == 1) {
                    switch (text[0]) {
                        case 'q':
                            free_mat(service_mat, n);
                            free_mat(rel_mat, n);
                            free_mat(B_mat, n);
                            free_mat(C_mat, n);
                            free_mat(D_mat, n);
                            free_mat(weight_mat, n);
                            close_x();
                            return 0;
                            break;
                        case 'u':
                            redraw_x();
                            draw_graph_old(new_graph, rel_mat, n, 0);
                            draw_graph_vertices(new_graph);
                            break;

                        default:
                            redraw_x();
                            draw_graph_weighted(new_graph);
                            break;
                    }
                    break;
                }
        }
    }
}