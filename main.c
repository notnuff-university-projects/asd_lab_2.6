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
#include "src/prime_algorithm.h"

int n;

int main() {
    int n1 = 2, n2 = 1, n3 = 2, n4 = 5;
    srand(n1 * 1000 + n2 * 100 + n3 * 10 + n4);

    //you can choose size of the graph here
    n = 10 + n3;

    //you can choose number of sides of the graph here
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

    graph_t *new_graph = graph_create(n, sides);
    graph_t *visited_graph = graph_create(0, 1);

    init_x();//window created

    int isP = 0;
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
                            graph_clear_full(&new_graph);
                            graph_clear(&visited_graph);
                            close_x();
                            return 0;
                            break;

                        case 'p':
                            if (isP == 0) {
                                graph_clear_full(&new_graph);
                                graph_clear(&visited_graph);
                                new_graph = graph_create(n, sides);

                                graph_set_weights(new_graph, rel_mat, weight_mat);
                                isP = 1;

                                visited_graph = graph_create(0, 1);
                                int start_v = new_graph->size * (double) rand() / RAND_MAX + 1;
                                printf("starting vertex = %d\n", start_v);
                                vertex_t *first_ver = graph_get_vertex(new_graph, start_v);
                                vertex_add(visited_graph, first_ver);
                                draw_graph_weighted(new_graph);
                                break;
                            }
                            if (visited_graph) {
                                if (new_graph->size <= visited_graph->size) {
                                    int min_weight = 0;
                                    edges_list_t *edges_list = visited_graph->minimal_edges_list;
                                    while (edges_list) {
                                        min_weight += edges_list->this_edge->weight;
                                        edges_list = edges_list->next_edge_p;
                                    }
                                    char string[70];
                                    sprintf(string, "It`s done. Weight of the minimal backbone of the graph is: %d", min_weight);
                                    XSetForeground(dis, gc, 0xFFFFFF);
                                    XDrawString(dis, win, gc, 20, 20, string, (int) strlen(string));
                                    break;
                                }
                            }

                            redraw_x();
                            pr_algorithm(&visited_graph, new_graph);
                            draw_graph_weighted(new_graph);
                            break;
                        default:
                            redraw_x();
                            isP = 0;
                            graph_clear_full(&new_graph);
                            new_graph = graph_create(n, sides);
                            graph_set_weights(new_graph, rel_mat, weight_mat);
                            draw_graph_weighted(new_graph);
                            break;
                    }
                    break;
                }
        }
    }
}
