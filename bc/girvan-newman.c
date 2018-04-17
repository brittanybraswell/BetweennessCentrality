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

    igraph_real_t max;                       // the edge id of the edge with highest ebt
    igraph_es_t edge_selector;               // an iterator for to select the edge to remove
    int ret;                                 // used to check that igraph functions ran correctly

    graph = create_graph(argv[1]);           // creates graph of given dataset
    ecount = igraph_ecount(&graph);          // number of edges in graph

    // print the number of edges before removal
    fprintf(stderr,"Number of edges is %d\n", ecount);

    // calculate the time to find and remove the edge with the highest ebt
    START_TIMER(find_max)

    // use the function that uses igraphs edge betweenness function to get an edge id
    max = find_max_betweenness();

    // make an edge selector for just the edge with the selected edge id
    igraph_es_1(&edge_selector, max);

    // delete the edge from the graph
    ret = igraph_delete_edges(&graph, edge_selector);
    STOP_TIMER(find_max);

    // determine the number of edges after the deletion
    ecount = igraph_ecount(&graph);

    // print the new number of edges to verify only one was removed
    fprintf(stderr,"Number of edges is %d\n", ecount);

    // clean up the memory used
    igraph_es_destroy(&edge_selector);
    igraph_destroy(&graph);

    // print the run time
    fprintf(stderr,"Time: %f\n", GET_TIMER(find_max));

    return EXIT_SUCCESS;
}

/*
 * This function is to find the edge with the greatest betweenness
 * for a graph, which will be removed.  This is the implementation that uses
 * the igraph_edge_betweenness() function.
 */
igraph_real_t find_max_betweenness() {
    igraph_real_t max_edge;          // the id of the edge with the highest ebt
    igraph_vector_t edges;           // this vector will store the edge betweenness values for each edge id

    // create a vector to hold all of the edge betweenness values
    igraph_vector_init(&edges, ecount);

    // fill the vector with the ebt values
    igraph_edge_betweenness(&graph, &edges, IGRAPH_UNDIRECTED, 0);

    // get the edge id of the edge with the greatest value
    max_edge = igraph_vector_max(&edges);

    // print the edge id of the selected edge
    fprintf(stderr, "Edge Selected: %li\n", max_edge);

    // clean up the vector we used
    igraph_vector_destroy(&edges);

    // return the id of the edge with the highest ebt value
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
    // just a temp value for compiling the stub
    igraph_real_t max_edge = 0;


    return max_edge;
}

// end of file
