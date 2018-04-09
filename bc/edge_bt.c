#include <stdbool.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "graph.h"
#include "util.h"

int main(int argc, char *argv[]) {
    igraph_t graph = create_graph(argv[1]); // creates graph of given dataset

    int ecount = igraph_ecount(&graph); // number of edges in graph

    START_TIMER(find_max)
    compare_t current_max = find_max_betweenness(&graph, ecount);
    STOP_TIMER(find_max);

    printf("\tTime: %f\n", GET_TIMER(find_max));

    return 0;
}

compare_t find_max_betweenness(igraph_t *graph, int ecount) {
    int curr_edge_id; // current value in ecount
    compare_t max_edge; // the struct 
    max_edge.max_betweenness_score = -1; // max score given dummy value
    max_edge.id = -1; // edge ID of max score given dummy value

#   pragma omp parallel for default(none) shared(ecount, graph, max_edge) \
    private(curr_edge_id)
    for (curr_edge_id = 0; curr_edge_id < ecount; curr_edge_id+=1) {
        igraph_vector_t result; // holds betweenness score 
        igraph_vector_init(&result, 0); // initialize result vector
        igraph_edge_betweenness(graph, &result, IGRAPH_UNDIRECTED, 0);
        double betweenness_score = (double) VECTOR(result)[0];

#       pragma omp critical
        if (betweenness_score > max_edge.max_betweenness_score) {
            max_edge.max_betweenness_score = betweenness_score;
            max_edge.id = curr_edge_id;
        }

        igraph_vector_destroy(&result); // every vector object is destroyed
    }

    printf("\tEdge: %d\t", max_edge.id);
    return max_edge; 
}

//end of file
