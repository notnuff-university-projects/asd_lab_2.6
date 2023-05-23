#include "../graph_draw.h"
#include <stdio.h>
#include "../graph_functions.h"

void weight_add(weights_list_t **tar_p, edge_t *edge, int px, int py) {
    if (tar_p == NULL) return;
    weights_list_t *node_p;
    node_p = malloc(sizeof(weights_list_t));
    node_p->px = px;
    node_p->py = py;
    node_p->weight = edge->weight;
    node_p->color = edge->color;
    if (*tar_p) {
        node_p->next_weight = *tar_p;
    } else {
        node_p->next_weight = NULL;
    }
    *tar_p = node_p;
}

void weight_clear(weights_list_t **tar_p) {
    weights_list_t *tar = *tar_p;
    if (!tar) return;
    weights_list_t *next_p;
    while (tar) {
        next_p = tar->next_weight;
        free(tar);
        tar = next_p;
    }
    *tar_p = NULL;
}

void weight_draw(weights_list_t *tar) {
    weights_list_t *next_p;
    while (tar) {
        next_p = tar->next_weight;
        weight(tar->weight, tar->px, tar->py, tar->color);
        tar = next_p;
    }
}

int count_digits(int number) {
    int count = 0;

    if (number == 0) {
        return 1;
    }

    if (number < 0) {
        number = -number;
    }

    while (number > 0) {
        count++;
        number /= 10;
    }

    return count;
}

void weight(int weight, int px, int py, int color) {
    int str_size = count_digits(weight);
    char str[3];
    sprintf(str, "%d", weight);
    XSetForeground(dis, gc, color);
    XFillRectangle(dis, win, gc, px - 2, py - 10, 7 * str_size + 3, 11);
    XSetForeground(dis, gc, ~0xFFFFFF);
    XDrawString(dis, win, gc, px, py, str, str_size);
}

void draw_graph_vertices(graph_t *graph) {
    int size = graph->size;
    vertices_list_t *this_ver_p = graph->vertices_list_start;
    while (this_ver_p) {
        vertex_t *this_ver = this_ver_p->this_vertex;
        XSetForeground(dis, gc, 0xFFFFFF);
        XFillArc(dis, win, gc,
                 this_ver->x - POINT_RADIUS, this_ver->y - POINT_RADIUS,
                 2 * POINT_RADIUS, 2 * POINT_RADIUS,
                 0, 360 * 64);

        this_ver_p = this_ver_p->next_vertex_p;
    }

    XSetLineAttributes(dis, gc,
                       2,
                       LineSolid,
                       CapButt, JoinMiter);

    this_ver_p = graph->vertices_list_start;
    while (this_ver_p) {
        vertex_t *this_ver = this_ver_p->this_vertex;
        int line_size = this_ver->number / 10 + 2;
        switch (this_ver->number) {
            case 'N':
                XSetForeground(dis, gc, 0x0000FF);
                break;
            case 'V':
                XSetForeground(dis, gc, 0x005500);
                break;
            case 'A':
                XSetForeground(dis, gc, 0xFF0000);
                line_size = 1;
                break;
            default:
                XSetForeground(dis, gc, 0x000000);
                line_size--;
                break;
        }

        XSetLineAttributes(dis, gc,
                           2,
                           LineSolid,
                           CapButt, JoinMiter);
        XDrawString(dis, win, gc, this_ver->x - 2, this_ver->y + 4,
                    this_ver->name, line_size);

        XDrawArc(dis, win, gc, this_ver->x - POINT_RADIUS, this_ver->y - POINT_RADIUS,
                 2 * POINT_RADIUS, 2 * POINT_RADIUS, 0, 360 * 64);

        this_ver_p = this_ver_p->next_vertex_p;

    }
}

void draw_arc(vec_4_t *vector, int offset_radius, edge_t *edge, weights_list_t **weights_to_draw) {
    vec_2_t *vec = malloc(sizeof(vec_2_t));
    vec_2_t *vec_OX = malloc(sizeof(vec_2_t));
    *vec_OX = (vec_2_t) {1, 0};

    vec->x = vector->end_x - vector->start_x;
    vec->y = vector->end_y - vector->start_y;

    int quarter = vec_quarter(vec);

    int midpoint_x = vec->x / 2;
    int midpoint_y = vec->y / 2;
    double cosB = vectors_cos(vec, vec_OX);
    double sinA = cosB;
    double cosA;
    if (quarter == 1 || quarter == 2) cosA = -sqrt(1 - cosB * cosB);
    else cosA = sqrt(1 - cosB * cosB);

    int offset_y = sinA * offset_radius;
    int offset_x = cosA * offset_radius;
    int center_x = vector->start_x + midpoint_x - offset_x;
    int center_y = vector->start_y + midpoint_y + offset_y;
    int rad2 = sqrt((center_x - vector->start_x) * (center_x - vector->start_x) +
                    (center_y - vector->start_y) * (center_y - vector->start_y));

    vec_2_t *center_to_end = malloc(sizeof(vec_2_t));
    center_to_end->x = vector->end_x - center_x;
    center_to_end->y = vector->end_y - center_y;

    vec_2_t *center_to_start = malloc(sizeof(vec_2_t));
    center_to_start->x = vector->start_x - center_x;
    center_to_start->y = vector->start_y - center_y;

    vec_2_t *center_to_mid = malloc(sizeof(vec_2_t));
    center_to_mid->x = offset_x;
    center_to_mid->y = -offset_y;

    double angle = vec_angle(center_to_mid);

    cosA = vectors_cos(center_to_start, vec_OX);
    int S = 64 * acos(cosA) * 180 / M_PI;
    if (vec_quarter(center_to_start) == 4 || vec_quarter(center_to_start) == 3)
        S = 360 * 64 - S;
    cosB = vectors_cos(center_to_end, vec_OX);
    int E = 64 * acos(cosB) * 180 / M_PI;

    if (vec_quarter(center_to_end) == 4 || vec_quarter(center_to_end) == 3)
        E = 360 * 64 - E;

    if (vec_quarter(center_to_end) == 4 && vec_quarter(center_to_start) == 1)
        S = 360 * 64 + S;

    XSetForeground(dis, gc, edge ? edge->color : 0xFFFFFF);
    XSetLineAttributes(dis, gc, edge? edge->thickness : 1, LineSolid, CapButt, JoinMiter);

    XDrawArc(dis, win, gc, center_x - rad2, center_y - rad2,
             2 * rad2, 2 * rad2, E, abs(S - E));


    if (weights_to_draw)
        weight_add(weights_to_draw, edge,
                   center_x + rad2 * cos(angle) - 0.00 * offset_x, center_y - rad2 * sin(angle));
    free(vec);
    free(vec_OX);
    free(center_to_end);
    free(center_to_start);
    free(center_to_mid);
}

void draw_line(vec_4_t *vector, edge_t *edge, weights_list_t **weights_to_draw) {
    int color = edge ? edge->color : 0xFFFFFF;
    XSetForeground(dis, gc, color);
    XSetLineAttributes(dis, gc, edge ? edge->thickness : 1, LineSolid, CapButt, JoinMiter);
    XDrawLine(dis, win, gc, vector->start_x, vector->start_y, vector->end_x, vector->end_y);

    if (edge) {

        r_offset_t *offset = rad_offsetting(vector);

        vector->start_x +=  offset->x_offset;
        vector->start_y +=  offset->y_offset;
        vector->end_x +=  offset->x_offset;
        vector->end_y +=  offset->y_offset;

        free(offset);

        int mid_x = vector->start_x + (vector->end_x - vector->start_x)/2 - 5;
        int mid_y = vector->start_y + (vector->end_y - vector->start_y)/2 + 5;

        weight_add(weights_to_draw, edge, mid_x, mid_y);
    }
}

void draw_graph_old(graph_t *graph, int **relation_matrix, int size, int red) {

    int point_1 = 0;
    int point_2 = size / 3;
    int point_3 = 2 * size / 3;

    int color = 0xFFFFFF;
    int thickness = 1;

    if (red) {
        color = 0xFF0000;
        thickness = 5;
    }

    for (int i = 0; i < size; i++) {
        for (int j = i; j < size; j++) {
            XSetForeground(dis, gc, color);
            XSetLineAttributes(dis, gc, thickness, LineSolid, CapButt, JoinMiter);

            vec_4_t *this_vec = malloc(sizeof(vec_4_t));
            *this_vec = (vec_4_t) {graph_get_vertex(graph, i + 1)->x, graph_get_vertex(graph, i + 1)->y,
                                   graph_get_vertex(graph, j + 1)->x, graph_get_vertex(graph, j + 1)->y};

            r_offset_t *offset = rad_offsetting(this_vec);

            this_vec->start_x += offset->x_offset;
            this_vec->start_y += offset->y_offset;
            this_vec->end_x -= offset->x_offset;
            this_vec->end_y -= offset->y_offset;

            if (relation_matrix[i][j]) {
                int condition = (
                        point_1 <= i && i <= point_2 && point_1 <= j && j <= point_2 ||
                        point_2 <= i && i <= point_3 && point_2 <= j && j <= point_3 ||
                        point_3 <= i && i < size && point_3 <= j && j < size ||
                        point_3 <= i && i < size && j == point_1 ||
                        point_3 <= j && j < size && i == point_1);

                if (condition) { //на одній лінії знаходяться
                    int a = abs(i - j) % (size - 1) + abs(i - j) / (size - 1);
                    switch (a) {
                        case 0:
                            if (relation_matrix[i][i]) {
                                XDrawArc(dis, win, gc, this_vec->start_x + 0.3 * POINT_RADIUS, this_vec->start_y,
                                         2 * POINT_RADIUS, 2 * POINT_RADIUS, 240 * 64, 240 * 64);
                            }
                            break;
                        case 1:
                            if (relation_matrix[i][j]) {
                                draw_line(this_vec, NULL, NULL);
                            }
                            break;
                        default:
                            if (relation_matrix[i][j])
                                draw_arc(this_vec, 500, NULL, NULL);
                    }
                } else { //на різних лініях
                    if (relation_matrix[i][j]) {
                        if (relation_matrix[i][j])
                            draw_line(this_vec, NULL, NULL);
                    }
                }
            }
            free(this_vec);
            free(offset);
        }
    }
}

void draw_edges(graph_t *graph, edges_list_t *edges, weights_list_t **weights_to_draw) {
    int size = graph->size;
    int sides = graph->sides;

    int points[sides];
    for (int i = 0; i < sides; i++) {
        points[i] = i * size / sides + 1;
    }
    points[sides] = size;

    int color = 0xFFFFFF;
    int thickness = 1;


    while (edges) {
        edge_t *edge = edges->this_edge;
        vertex_t *v1 = graph_get_vertex(graph, edge->v1);
        vertex_t *v2 = graph_get_vertex(graph, edge->v2);
        if (!v1 || !v2) return;
        XSetForeground(dis, gc, color);
        XSetLineAttributes(dis, gc, thickness, LineSolid, CapButt, JoinMiter);

        vec_4_t *this_vec = malloc(sizeof(vec_4_t));

        *this_vec = (vec_4_t) {v1->x, v1->y,
                               v2->x, v2->y};

        r_offset_t *offset = rad_offsetting(this_vec);

        this_vec->start_x += offset->x_offset;
        this_vec->start_y += offset->y_offset;
        this_vec->end_x -= offset->x_offset;
        this_vec->end_y -= offset->y_offset;

        int condition = 0;
        if (points[sides - 1] <= v1->number && v1->number < points[sides] && v2->number == points[sides % sides] ||
            points[sides - 1] <= v2->number && v2->number < points[sides] && v1->number == points[sides % sides])
            condition = 1;

        for (int k = 0; k < sides; k++)
            if (points[k] <= v1->number && v1->number <= points[k + 1] &&
                points[k] <= v2->number && v2->number <= points[k + 1])
                condition = 1;

        if (condition) {
            int a = abs(v1->number - v2->number) % size + abs(v1->number - v2->number) / size;
            switch (a) {
                case 0:
                    XDrawArc(dis, win, gc, this_vec->start_x + 0.3 * POINT_RADIUS, this_vec->start_y,
                             2 * POINT_RADIUS, 2 * POINT_RADIUS, 240 * 64, 240 * 64);
                    break;
                case 1:
                    draw_line(this_vec, edge, weights_to_draw);
                    break;
                default:
                    draw_arc(this_vec, 500, edge, weights_to_draw);
                    break;
            }
        } else {
            draw_line(this_vec, edge, weights_to_draw);
        }
        free(this_vec); free(offset);
        edges = edges->next_edge_p;
    }
}

void draw_graph_weighted(graph_t *graph) {
    int size = graph->size;
    weights_list_t *weights_to_draw = NULL;

    vertices_list_t *this_v = graph->vertices_list_start;

    while (this_v) {
        vertex_t *this_vertex = this_v->this_vertex;
        edges_list_t *list_this_edge = this_vertex->edges;
        draw_edges(graph, list_this_edge, &weights_to_draw);
        this_v = this_v->next_vertex_p;
    }

    if(graph->minimal_edges_list) {
        draw_edges(graph, graph->minimal_edges_list, &weights_to_draw);
    }
    weight_draw(weights_to_draw);
    weight_clear(&weights_to_draw);
    draw_graph_vertices(graph);
}
