#include "../graph_functions.h"

vertex_t *vertex_create(int number, int x, int y, edges_list_t *edges) {
    vertex_t *vertex = malloc(sizeof (vertex_t));
    vertex->number = number;
    sprintf(vertex->name, "%d", number);
    vertex->x = x; vertex->y = y;
    vertex->edges = edges;
    return vertex;
}

void vertex_add(graph_t *tar_graph, vertex_t *vertex) {
    vertices_list_t *this_node = malloc(sizeof (vertices_list_t));
    this_node->this_vertex = vertex;
    this_node->next_vertex_p = NULL;

    vertices_list_t *last = tar_graph->vertices_list_end;
    tar_graph->vertices_list_end = this_node;

    if (last == NULL) {
        tar_graph->vertices_list_start = this_node;
        tar_graph->size++;
        return;
    }
    last->next_vertex_p = this_node;
    tar_graph->size++;
}

vertex_t *graph_get_vertex(graph_t *graph, int index) {
    vertices_list_t *this_ver_p = graph->vertices_list_start;
    while (this_ver_p) {
        if (this_ver_p->this_vertex->number == index) {
            return this_ver_p->this_vertex;
        }
        this_ver_p = this_ver_p->next_vertex_p;
    }
    return NULL;
}

void print_vertices(graph_t *graph) {
    vertices_list_t *this_ver_p = graph->vertices_list_start;
    while (this_ver_p) {
        vertex_t *this = this_ver_p->this_vertex;
        printf("i = %i, coords: x = %d, y = %d\n", this->number, this->x, this->y);
        this_ver_p = this_ver_p->next_vertex_p;
    }
}

void print_graph(graph_t *graph) {
    printf("size = %d, vertices:\n", graph->size);
    print_vertices(graph);
}

graph_t *graph_create(int size, int sides) {
    double as_ratio = (double) SCREEN_HEIGHT / SCREEN_WIDTH;

    if (size == 0) {
        graph_t *graph = malloc(sizeof(graph_t) * 1);
        graph->size = 0;
        graph->sides = sides;
        graph->minimal_edges_list = NULL;
        graph->vertices_list_start = NULL;
        graph->vertices_list_end = NULL;
        return graph;
    }

    graph_t *graph = malloc(sizeof(graph_t) * size);
    graph->size = 0;
    graph->sides = sides;
    graph->minimal_edges_list = NULL;
    graph->vertices_list_start = NULL;
    graph->vertices_list_end = NULL;

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
        vertex_t *this_ver = vertex_create(i+1, x, y, NULL);
        vertex_add(graph, this_ver);
    }
    return graph;
}

void edges_add(edges_list_t **edges, edge_t *this_edge) {
    edges_list_t *this_node = malloc(sizeof (edges_list_t));
    this_node->this_edge = this_edge;
    if (*edges == NULL) {
        *edges = this_node;
        this_node->next_edge_p = NULL;
        return;
    }
    this_node->next_edge_p = *edges;
    *edges = this_node;
}

void graph_set_weights(graph_t *graph, int **rel_mat, int **weight_mat) {
    int size = graph->size;
    for (int i = 0; i < size; i++) {
        for (int j = i; j < size; j++) {
            if (rel_mat[i][j]) {
                edge_t *this_edge = malloc(sizeof(edge_t));
                int color = 0xFFFFFF * (double) rand() / (double) RAND_MAX;
                *this_edge = (edge_t) {i + 1, j + 1, weight_mat[i][j], color, 1};
                vertex_t *ver1 = graph_get_vertex(graph, i + 1);
                vertex_t *ver2 = graph_get_vertex(graph, j + 1);
                if (ver1) edges_add(&(ver1->edges), this_edge);
                if (ver2) edges_add(&(ver2->edges), this_edge);
                edges_add(&(graph->minimal_edges_list), this_edge);
            }
        }
    }
}

void graph_print_weights(graph_t *graph) {
    edges_list_t *edges_list = graph->minimal_edges_list;
    if (edges_list == NULL) {
        return;
    }
    while (edges_list) {
        printf("v1 = %i, v2 = %i, this_edge->weight = %i, this_edge = %p, next_edge_p = %p\n",
               edges_list->this_edge->v1,
               edges_list->this_edge->v2,
               edges_list->this_edge->weight,
               edges_list,
               edges_list->next_edge_p);
        edges_list = edges_list->next_edge_p;
    }
}

void graph_clear(graph_t **graph) {
    graph_t *this_graph = *graph;
    if (!this_graph) return;
    vertices_list_t *ver_list = this_graph->vertices_list_start;
    while (ver_list) {
        vertices_list_t *next = ver_list->next_vertex_p;
        free(ver_list);
        ver_list = next;
    }
    edges_list_t *all_edges = this_graph->minimal_edges_list;
    while(all_edges) {
        edges_list_t *next_edge = all_edges->next_edge_p;
        free(all_edges);
        all_edges = next_edge;
    }
    free(this_graph);
    *graph = NULL;
}

void graph_clear_full(graph_t **graph) {
    graph_t *this_graph = *graph;
    if (!this_graph) return;
    vertices_list_t *ver_list = this_graph->vertices_list_start;
    while (ver_list) {
        vertex_t *this_vertex = ver_list->this_vertex;
        edges_list_t *this_edges = this_vertex->edges;
        while (this_edges) {
            edges_list_t *next = this_edges->next_edge_p;
            free (this_edges);
            this_edges = next;
        }
        free(this_vertex);
        vertices_list_t *next = ver_list->next_vertex_p;
        free(ver_list);
        ver_list = next;
    }

    edges_list_t *all_edges = this_graph->minimal_edges_list;
    while(all_edges) {
        edges_list_t *next_edge = all_edges->next_edge_p;
        free(all_edges->this_edge);
        free(all_edges);
        all_edges = next_edge;
    }
    free(this_graph);
    *graph = NULL;
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
            edges_list = edges_list->next_edge_p;
            printf("\n");

        }
        printf("\n");
    }
}