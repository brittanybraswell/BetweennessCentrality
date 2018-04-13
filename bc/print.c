#include <igraph.h>
#include <stdio.h>
#include "graph.h"
#include "util.h"


/*
 *
 * This encapsulation is used to take a file representing a graph and then
 * load it into a graph.  This graph is then used to output the same data in
 * a different format.  The newly formatted data can then be used in with the
 * R programming language to plot the graph to a pdf to give us a visual aid
 * on what a data set actually looks like.
 *
 */
int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Usage: print <in-file> <out-file>\n");
        return EXIT_FAILURE;
    }

    char *fi = argv[1];
    char *fo = argv[2];

    igraph_vector_t v;
    igraph_t g;

    int size, f, index = 0;

    /* Open a file */
    FILE *in_file = fopen (fi, "r");
    fscanf(in_file, "%d", &size);               //size of vectors needed, edges x 2
    /* Initialize a vector: IGraph's internal struct */
    igraph_vector_init(&v, size);

    /* Read in a node and assign it to 1 vector at a time */
    while(fscanf(in_file, "%d", &f) > 0) {
        VECTOR(v)[index++] = f-1;
    }

    fclose(in_file);
    igraph_create(&g, &v, 0, IGRAPH_UNDIRECTED);

    FILE *out_file = fopen(fo, "w");
    if (out_file == 0) {
        printf("ERROR: Could not open out-file\n");
        return EXIT_FAILURE;
    }

    igraph_write_graph_gml(&g, out_file, 0, fo);
    igraph_destroy(&g);

    return EXIT_SUCCESS;
}

