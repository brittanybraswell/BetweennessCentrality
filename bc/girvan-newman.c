#include <stdbool.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "graph.h"

igraph_t graph;
int ecount;

igraph_real_t find_max_betweenness();
igraph_real_t max_betweenness();

/*
 * This main function is the equivalent of ONE iteration of the Girvan-Newman
 * algorthim for creating digraphs.  The betweenness of all edges is calculated,
 * the edge with the max betweenness is removed.  If this were to be done in a
 * loop checking that the number of non-connected networks in the graph was
 * greater than one but less than "n" (the target number of divisions) then it
 * would be a full implementation.
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./edge_bt <file-path>\n");
        return EXIT_FAILURE;
    }

    igraph_real_t max;
    igraph_es_t edge_selector;
    int ret;

    graph = create_graph(argv[1]); // creates graph of given dataset
    ecount = igraph_ecount(&graph); // number of edges in graph

    fprintf(stderr,"Number of edges is %d\n", ecount);

    START_TIMER(find_max)

    max = find_max_betweenness();
    igraph_es_1(&edge_selector, max);
    ret = igraph_delete_edges(&graph, edge_selector);
    STOP_TIMER(find_max);

    ecount = igraph_ecount(&graph);
    fprintf(stderr,"Number of edges is %d\n", ecount);

    igraph_es_destroy(&edge_selector);
    igraph_destroy(&graph);
    fprintf(stderr,"Time: %f\n", GET_TIMER(find_max));

    return EXIT_SUCCESS;
}

/*
 * This function is to find the edge with the greatest betweenness
 * for a graph, which will be removed.  This is the implementation that uses
 * the igraph_edge_betweenness() function.
 */
igraph_real_t find_max_betweenness() {
    igraph_real_t max_edge;
    igraph_vector_t edges;

    igraph_vector_init(&edges, ecount);

    igraph_edge_betweenness(&graph, &edges, IGRAPH_UNDIRECTED, 0);

    max_edge = igraph_vector_max(&edges);

    fprintf(stderr, "Edge Selected: %li\n", max_edge);

    fprintf(stderr, "Edge Selected: %li\n", max_edge);

    igraph_vector_destroy(&edges);

    return max_edge;
}
/*
 * This function also finds the edge with the highest betweenness value
 * for a graph, which will be removed in main.  This version does not us
 * the igraph_edge_betweenness() function and instead attempts to use the
 * calculate_shortest_paths functions instead to give us more control over how
 * the edge betweenness values are being calculated.
 */
igraph_real_t max_betweenness() {
    igraph_real_t max_edge = 0;


    return max_edge;
}

// end of file
