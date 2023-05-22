#include "../graph_functions.h"

void vertex_add(graph_t *tar_graph, int number, int x, int y, edges_list_t *edges) {
    vertex_t *ver = malloc(sizeof(vertex_t));
    ver->number = number;
    ver->x = x;
    ver->y = y;
    ver->next_vertex = NULL;
    ver->edges = edges;
    vertex_t *last = tar_graph->vertices_list_end;
    tar_graph->vertices_list_end = ver;

    if (tar_graph->size == 0) {
        tar_graph->vertices_list_start = ver;
        ver->prev_vertex = NULL;
        tar_graph->size++;
        return;
    }
    ver->prev_vertex = last;
    last->next_vertex = ver;
    tar_graph->size++;
}

vertex_t *graph_get_vertex(graph_t *graph, int index) {
    int size = graph->size;
    vertex_t *this_v = graph->vertices_list_start;
    for (int i = 1; i <= size; i++) {
        if (this_v->number == index) return this_v;
        this_v = this_v->next_vertex;
    }
    printf("no vertex with such index\n");
    return NULL;
}

void print_vertices(graph_t *graph) {
    vertex_t *this = graph->vertices_list_start;
    while (this) {
        printf("i = %i, coords: x = %d, y = %d\n", this->number, this->x, this->y);
        printf("this->next_vertex = %p\n", this->next_vertex);
        this = this->next_vertex;
    }
}

void print_graph(graph_t *graph) {
    printf("size = %d, vertices:\n", graph->size);
    print_vertices(graph);
}

graph_t *graph_create(int size, int sides) {
    double as_ratio = (double) SCREEN_HEIGHT / SCREEN_WIDTH;
    graph_t *graph = malloc(sizeof(graph_t) * size);
    graph->size = 0;
    graph->sides = sides;
    graph->edges_list = NULL;
    double indent_x = 0.95;
    double indent_y = 0.95;
    int points[sides + 1];
    serv_point_t serv_points[size + 1];
    double angle = M_PI / 2;
    for (int i = 0; i < sides; i++) {
        points[i] = i * size / sides;
    }
    points[sides] = size;

    for (int i = 0; i < sides; i++) {
        serv_points[points[i]] = (serv_point_t) {as_ratio * indent_x * cos(angle), indent_y * sin(angle)};
        angle -= 2 * M_PI / sides;
    }
    serv_points[points[sides]] = serv_points[points[0]];

    for (int k = 0; k < sides; k++) {
        //interpolation (I guess?) for non-anchor points of triangle
        for (int i = points[k] + 1, j = 1; i < points[k + 1]; i++, j++) {
            int this = points[k];
            int next = points[k + 1];
            serv_points[i].x = serv_points[this].x + j * (serv_points[next].x - serv_points[this].x) / (next - this);
            serv_points[i].y = serv_points[this].y + j * (serv_points[next].y - serv_points[this].y) / (next - this);
        }
    }
    //translating into the screen space
    for (int i = 0; i < size; i++) {
        int x = SCREEN_WIDTH + (int) (serv_points[i].x * SCREEN_WIDTH - SCREEN_WIDTH) / 2;
        int y = SCREEN_HEIGHT - (int) (serv_points[i].y * SCREEN_HEIGHT + SCREEN_HEIGHT) / 2;
        vertex_add(graph, i + 1, x, y, NULL);
        sprintf(graph->vertices_list_end->name, "%d", i + 1);
    }
    return graph;
}

void edges_add(edges_list_t **edges, edge_t *this_edge) {
    edges_list_t *this_node = malloc(sizeof (edges_list_t));
    this_node->this_edge = this_edge;
    if (*edges == NULL) {
        *edges = this_node;
        this_node->next_edge = NULL;
        return;
    }
    this_node->next_edge = *edges;
    *edges = this_node;
}
void graph_set_weights(graph_t *graph, int **rel_mat, int **weight_mat) {
    int size = graph->size;
    for (int i = 0; i < size; i++) {
        for (int j = i; j < size; j++) {
            if (rel_mat[i][j]) {
                edge_t *this_edge = malloc(sizeof(edge_t));
                int color = 0xFFFFFF * (double) rand() / (double) RAND_MAX;
                *this_edge = (edge_t) {i + 1, j + 1, weight_mat[i][j], color};
                vertex_t *ver1 = graph_get_vertex(graph, i + 1);
                vertex_t *ver2 = graph_get_vertex(graph, j + 1);
                if (ver1) edges_add(&(ver1->edges), this_edge);
                if (ver2) edges_add(&(ver2->edges), this_edge);
                edges_add(&(graph->edges_list), this_edge);
            }
        }
    }
}

void graph_print_weights(graph_t *graph) {
    edges_list_t *edges_list = graph->edges_list;
    if (edges_list == NULL) {
        printf("no edges list\n");
        return;
    }
    while (edges_list) {
        printf("v1 = %i, v2 = %i, this_edge->weight = %i, this_edge = %p, next_edge = %p\n",
               edges_list->this_edge->v1,
               edges_list->this_edge->v2,
               edges_list->this_edge->weight,
               edges_list,
               edges_list->next_edge);
        edges_list = edges_list->next_edge;
    }
}

void graph_print_ver_weights(graph_t *graph) {
    int size = graph->size;
    for (int i = 1; i <= size; i++) {
        vertex_t *ver = graph_get_vertex(graph, i);
        edges_list_t *edges_list = ver->edges;
        while (edges_list) {
            printf("v1 = %i, v2 = %i, weight = %i",
                   edges_list->this_edge->v1,
                   edges_list->this_edge->v2,
                   edges_list->this_edge->weight);
            edges_list = edges_list->next_edge;
            printf("\n");

        }
        printf("\n");
    }
}