#ifndef GRAPH_H
#define GRAPH_H

#include <igraph.h>

typedef struct edge {
    int from;
    int to;
    double from_degree;
    double to_degree;
} edge;

void print_vector(igraph_vector_t *, FILE *);
igraph_t create_graph(char*);
struct edge* get_edges(igraph_t);
