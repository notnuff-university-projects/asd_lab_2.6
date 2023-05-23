#include "../prime`s_algorithm.h"

void pr_algorithm(graph_t **visited_graph_p, graph_t *target_graph) {
    graph_t *v_graph = *visited_graph_p;

    vertices_list_t *this_ver_list = v_graph->vertices_list_start;
    edge_t *min_edge = NULL;
    vertex_t *vertex_to_add = NULL;
    while (this_ver_list) {
        vertex_t *this_ver = this_ver_list->this_vertex;
        edges_list_t *this_ver_edges = this_ver->edges;
        while(this_ver_edges) {
            edge_t *this_edge = this_ver_edges->this_edge;
            if (graph_get_vertex(v_graph, this_edge->v1) == NULL) {
                if (min_edge == NULL || this_edge->weight < min_edge->weight) {
                    min_edge = this_edge;
                    vertex_to_add = graph_get_vertex(target_graph, this_edge->v1);
                }
            }
            if (graph_get_vertex(v_graph, this_edge->v2) == NULL) {
                if (min_edge == NULL || this_edge->weight < min_edge->weight) {
                    min_edge = this_edge;
                    vertex_to_add = graph_get_vertex(target_graph, this_edge->v2);
                }
            }
            this_ver_edges = this_ver_edges->next_edge_p;
        }
        this_ver_list = this_ver_list->next_vertex_p;
    }
    if (min_edge) {
        min_edge->color = 0xFF0000;
        min_edge->thickness = 5;
        edges_add(&(v_graph->minimal_edges_list), min_edge);
    }
    if (vertex_to_add) vertex_add(v_graph, vertex_to_add);
    *visited_graph_p = v_graph;
}

