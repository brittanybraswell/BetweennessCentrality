#include <stdbool.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include <string.h>
#include "graph.h"
#include "util.h"

// global variables
igraph_t graph;
int ecount;
bool igraph = false;

// function prototypes
compare_t igraph_find_max_betweenness();
compare_t other_find_max_betweenness();
igraph_vector_ptr_t get_paths(long int);

/*
 * This main function is the equivalent of ONE iteration of the Girvan-Newman
 * algorthim for creating digraphs.  The betweenness of all edges is calculated,
 * the edge with the max betweenness is removed.  If this were to be done in a
 * loop checking that the number of non-connected networks in the graph was
 * greater than one but less than "n" (the target number of divisions) then it
 * would be a full implementation.
 */
int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: srun ./girvan-newman <file-path> <igraph OR custom>\n");
        return EXIT_FAILURE;
    }

    if (strncmp(argv[2], "igraph", 6) == 0) {
        igraph = true;
    } else if (strncmp(argv[2], "custom", 6) == 0) {
        igraph = false;
    } else {
        fprintf(stderr, "Usage: srun ./girvan-newman <file-path> <igraph OR custom>\n");
        fprintf(stderr, "Please choose 'custom' or 'igraph'\n");
        return EXIT_FAILURE;
    }

    graph = create_graph(argv[1]);           // creates graph of given dataset
    igraph_to_undirected(&graph, IGRAPH_TO_UNDIRECTED_EACH, 0);
    ecount = igraph_ecount(&graph);          // number of edges in graph

    compare_t max;
    igraph_es_t edge_selector;
    int start_edge_count;

    start_edge_count = igraph_ecount(&graph);

    /***************************************************************/
    /********* Below here is 1 iteration of Girvan-Newman **********/
    /************ We would loop this in a full version *************/
    /***************************************************************/

    fprintf(stderr, "Girvan-Newman Algorithm\n");
    fprintf(stderr, "Betweenness Score Values:\n\n");

    START_TIMER(find_max)

    // get the edge id of the edge with highest betweenness
    if (igraph) {
        max = igraph_find_max_betweenness();
    } else {
        max = other_find_max_betweenness();
    }

    // delete the edge from the graph
    igraph_es_1(&edge_selector, max.id);
    igraph_delete_edges(&graph, edge_selector);

    STOP_TIMER(find_max);

    // print some correctness output
    if (igraph) {
        fprintf(stderr, "\nSelected:          igraph\n");
    } else {
        fprintf(stderr, "\nSelected:          custom\n");
    }

    fprintf(stderr, "Edge Count Before: %d\n", start_edge_count);
    fprintf(stderr, "Edge Selected:     %d\n", max.id);
    fprintf(stderr, "Edge Count After:  %d\n", igraph_ecount(&graph));

    // clean up the memory used
    igraph_es_destroy(&edge_selector);
    igraph_destroy(&graph);

    fprintf(stderr,"Time:              %f\n", GET_TIMER(find_max));

    return EXIT_SUCCESS;
}

/*
 * This function is to find the edge with the greatest betweenness
 * for a graph, which will be removed.  This is the implementation that uses
 * the igraph_edge_betweenness() function.
 */
compare_t igraph_find_max_betweenness() {
    igraph_vector_t edges;           

    igraph_vector_init(&edges, ecount);

    // fill the vector with the edge betweenness values
    igraph_edge_betweenness(&graph, &edges, IGRAPH_DIRECTED, 0);

    // get the edge id of the edge with the greatest value
    int curr_edge;
    compare_t max_edge;
    max_edge.max_betweenness_score = -1;
    max_edge.id = -1;

    for (curr_edge = 0; curr_edge < ecount; curr_edge++) {
        double score = (double) VECTOR(edges)[curr_edge];

        // this print statment is here to check the edge betweenness values
        // will be removed later
        fprintf(stderr,"%f\n", score);

        if (score > max_edge.max_betweenness_score) {
            max_edge.max_betweenness_score = score;
            max_edge.id = curr_edge;
        }
    }

    // clean up and destroy
    igraph_vector_destroy(&edges);

    return max_edge;
}

/*
 * This function also finds the edge with the highest betweenness value
 * for a graph, which will be removed in main.  This version does not us
 * the igraph_edge_betweenness() function and instead uses a helper method to get
 * all edgeids in all shortest paths, then iteratively determines how many times
 * each edge is used in all paths (the betweenness).  Then it returns the id of
 * the edge with the highest betweenness.
 *
 *
 * There is a bug in this preventing further inspection of the implementation,
 * the error warnings in the messages is due to not reaching all of the nodes
 * in a graph when we were using an igraph function to get all of the shortest
 * paths to all nodes from a chosen node.  The problem comes from the graphs
 * being directed or undirected and not being complete, even though all of ours
 * at least a have a path from each node from every node because they are social
 * networks.  We attempted to change the graphs manually through igraph functions
 * but we could not reach the sweet spot of full functionality.  This complication
 * while seeming terrible actually heavily supports the notion of this algorthim
 * being difficult to parallelize as it would rapidly become worse as edges were
 * removed.
 *
 */
compare_t other_find_max_betweenness() {
    compare_t max_edge;
    igraph_vector_ptr_t path;
    igraph_vector_t used, unused, betweenness;
    int i, j, k;

    // igraph requires special memory initializers much like POSIX does
    igraph_vector_init(&used, igraph_ecount(&graph));
    igraph_vector_init(&unused, igraph_ecount(&graph));
    igraph_vector_init(&betweenness, igraph_ecount(&graph));

    // this loop is for ensuring we use all root nodes
    // the bug is here as not all edges are being accounted for in all iterations
    for (i = igraph_vcount(&graph) / 2; i < igraph_vcount(&graph); i++) {
        path = get_paths(i);

        // this loop is to get the counts of each edge from each vector if it is used, or not
        for (j = 0; j < igraph_vector_ptr_size(&path); j++) {
            for (k = 0; k < ecount; k++) {
                if (igraph_vector_contains(VECTOR(path)[j], k)) {
                    VECTOR(used)[k] = VECTOR(used)[k] + 1;
                } else {
                    VECTOR(unused)[k] = VECTOR(unused)[k] + 1;
                }
                VECTOR(betweenness)[k] += (double) VECTOR(used)[k] / (double) VECTOR(unused)[k];
            }
        }
    }

    // get the id of the most traveled edge
    max_edge.max_betweenness_score = -1;
    max_edge.id = -1;

    // this loop is to determine which edge has the highest betweenness score
    for (i = 0; i < ecount; i++) {
        double score = VECTOR(betweenness)[i];

        // print the betweenness scores for each edge to check correctness
        fprintf(stderr,"%f\n", score);

        if (score > max_edge.max_betweenness_score) {
            max_edge.max_betweenness_score = score;
            max_edge.id = i;
        }
    }

    // clean up and destroy the vectors
    igraph_vector_destroy(&used);
    igraph_vector_destroy(&unused);
    igraph_vector_destroy(&betweenness);
    igraph_vector_ptr_destroy(&path);

    return max_edge;
}

/*
 * This is a helper to generate all of the edges along all paths from a given
 * vertex.  It returns an igraph_vector_ptr that is a vector of vectors where
 * each inner vector is a list of the edges used in each path.
 */
igraph_vector_ptr_t get_paths(long int start_v) {
    igraph_vector_ptr_t evecs;                 //pointer to a vector of vectors
    igraph_vector_long_t pred;                 //stored predicates for calculation
    igraph_vector_long_t inbound;              //stored inbounds for calculation
    long int i;

    igraph_vector_ptr_init(&evecs, igraph_vcount(&graph));
    igraph_vector_long_init(&pred, 0);
    igraph_vector_long_init(&inbound, 0);

    // loop through the vector pointer to allocate memory for the inner vectors
    for (i = 0; i < igraph_vector_ptr_size(&evecs); i++) {
        VECTOR(evecs)[i] = calloc(1, sizeof(igraph_vector_t));
        igraph_vector_init(VECTOR(evecs)[i], 0);
    }
    
    // generate shortest paths from start node to evey node, saving the edge ids
    // IGRAPH_OUT determines that only outgoing edges from each node are used
    // IGRAPH_IN would only use incoming edges
    // IGRAPH_ALL would use every edge, incoming or outgoing
    // as we are following paths in a directed graph it should be IGRAPH_OUT
    igraph_get_shortest_paths(&graph, 0, &evecs, start_v, igraph_vss_all(), IGRAPH_OUT, &pred, &inbound);

    // clean up the igraph data types
    igraph_vector_long_destroy(&pred);
    igraph_vector_long_destroy(&inbound);

    // return the vector pointer
    return evecs;
}

// end of file
