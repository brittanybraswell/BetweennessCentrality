#include <igraph.h>
#include <stdio.h>

#include "graph.h"
#include "print.h"
#include "util.h"

/*
 * Prints a graph to a GML file. The graph should be provided as graph and
 * the path to write to should be provided as out_path. This function will
 * return a non-zero integer on error.
 */
int print_graph(igraph_t *graph, char *out_path) {
    FILE *out_file = fopen(out_path, "w");
    if (out_file == NULL) {
        return 1;
    }

    int retval = igraph_write_graph_gml(graph, out_file, NULL, 0);

    // close the file stream before returning
    fclose(out_file);
    return retval;
}

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
        fprintf(stderr, "Usage: print <in-file> <out-file>\n");
        return EXIT_FAILURE;
    }

    char *fi = argv[1];
    char *fo = argv[2];

    igraph_t g = create_graph(fi);

    if (print_graph(&g, fo) != 0) {
        fprintf(stderr, "ERROR: Could not write graph to out-file\n");
        return EXIT_FAILURE;
    }

    igraph_destroy(&g);

    return EXIT_SUCCESS;
}

