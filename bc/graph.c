#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

#define MAX_EXT_LEN 3

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
 * Creates a graph given a gml file.
 */
igraph_t gml_create_graph(char *file_name) {
    igraph_t g;
    /* Open a file */
    FILE *file = fopen (file_name, "r");
    igraph_read_graph_gml(&g, file);
    return g;
}

/*
 * Creates a graph given a csv file.
 */
igraph_t csv_create_graph(char *file_name) {
    igraph_t g;
    /* Open a file */
    FILE *file = fopen (file_name, "r");
    igraph_read_graph_edgelist(&g, file, 0, IGRAPH_UNDIRECTED);
    return g;
}

/*
 * Creates a graph given a text file with edges. The graph is directed.
 * Currently, this is meant to be used for testing.
 */
igraph_t create_directed_graph(char *file_name) {
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
    igraph_create(&g, &v, 0, IGRAPH_DIRECTED);

    return g;
}

igraph_t (*get_file_handler(char *filename))(char *) {
    /* Get the last occurance of the . character */
    char *dot = strrchr(filename, '.');
    /*
     * If there isn't one, just use csv_create_graph -- it's the more general
     * function
     */
    if (!dot || dot == filename) return csv_create_graph;
    /* The extension begins one character after dot, yay ptr arithmetic */
    char *extension = (dot + 1);
    bool is_gml = strncmp(extension, "gml", MAX_EXT_LEN);
    /*
     * csv_create_graph is our default handler. This should probably be
     * converted to if/else branches that are switch-like if we ever decide
     * to support more file extensions
     */
    return is_gml ? gml_create_graph : csv_create_graph;
}

/*
 * Creates a graph given a text file with edges
 */
igraph_t create_graph(char* file_name) {
    /*
     * Okay, so this is kinda gross-looking. But get_file_handler needs to
     * take in file_name so it can determine the handler based on the
     * extension. Then file_name needs to be passed to the handler. I'd like
     * to see a cleaner way to do this eventually, but I am hesitant to make
     * get_file_handler call the handler. I also don't want to have to do it
     * elsewhere in the project (like at all the callsites that currently call
     * this function. And we're keeping this function around for legacy reasons
     * anyway, because it's existed since the beginning and everyone calls it.
     */
    return get_file_handler(file_name)(file_name);
}

/* 
 * Returns a list of edges given a graph. 
 */
struct edge* get_edges(igraph_t g) {
    int i, j = 0;
    int edge_count = igraph_ecount(&g);
    int vector_size = igraph_vector_size(&v);
    struct edge *edges = (struct edge*)malloc(sizeof(struct edge) * edge_count);;
    for (i=0, j=0; i < vector_size- 1; i+=2, j++) {
        edges[j].from = VECTOR(v)[i];
        edges[j].to = VECTOR(v)[i+1];
    }
    return edges;
}
