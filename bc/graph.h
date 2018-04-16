#ifndef GRAPH_H
#define GRAPH_H

#include <igraph.h>

void print_vector(igraph_vector_t *, FILE *);
igraph_t create_directed_graph(char *file_name);
igraph_t create_graph(char*);

#ifdef _OPENMP
#   include <omp.h>
#   define START_TIMER(X) double _start_##X = omp_get_wtime();
#   define STOP_TIMER(X)  double _stop_##X = omp_get_wtime();
#   define GET_TIMER(X)   (_stop_##X - _start_##X)
#else
#   include <time.h>
#   define START_TIMER(X) clock_t _start_##X = clock();
#   define STOP_TIMER(X)  clock_t _stop_##X = clock();
#   define GET_TIMER(X)   ((double)(_stop_##X - _start_##X) / (double)CLOCKS_PER_SEC)
#endif


#endif
