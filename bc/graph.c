#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

/*
 * graph.c Contains helper methods 
 */

igraph_vector_t v;  //global variable

/*
 * Checks if create_graph is correct.
 *
 */
void print_vector(igraph_vector_t *v, FILE *f) {
  long int i;
  for (i=0; i<igraph_vector_size(v); i++) {
    fprintf(f, "%.5f\n", (double) VECTOR(*v)[i]);
  }
  fprintf(f, "\n");
}

/*
 * Creates a graph given a text file with edges
 */
igraph_t create_graph(char* file_name) {
    igraph_t g;
    int size, f, index = 0;

    /* Open a file */ 
    FILE *file = fopen (file_name, "r");
    fscanf(file, "%d", &size);  //size of vectors needed, edges x 2

    /* Initialize a vector: IGraph's internal struct */
    igraph_vector_init(&v, size);

    /* Read in a node and assign it to 1 vector at a time */
    while(fscanf(file, "%d", &f) > 0) {
        VECTOR(v)[index++] = f-1;
    }

    fclose(file);
    igraph_create(&g, &v, 0, IGRAPH_UNDIRECTED);

    return g;  
}

/* 
 * Returns a list of edges given a graph. 
 */
struct edge* get_edges(igraph_t g) {
    int i, j = 0;
    int edge_count = igraph_ecount(&g);
    int vector_size = igraph_vector_size(&v);
    struct edge *edges = (struct edge*)malloc(sizeof(struct edge) * edge_count);;
    for (i = 0 ; i < vector_size- 1 ; i+=2) {
        edges[j].from = VECTOR(v)[i];
        edges[j].to = VECTOR(v)[i+1];
        j++;
    }
    return edges;
}
