#include "graph.h"
#include "stdbool.h"
#ifdef _OPENMP
#include <omp.h>
#endif

igraph_t graph; // the graph 
int ecount;  // the number of vertices in graph

typedef struct Compare {
    double max_betweenness_score; // max score out of all vertices 
    int edge_id; // ID given by iterating through vcount
} Compare;

// Function Prototype for calculating the max betweenness score
struct Compare find_max_betweenness();

int main(int argc, char *argv[]) {
    graph = create_graph(argv[1]); // creates graph of given dataset

    ecount = igraph_ecount(&graph); // number of edges in graph

    START_TIMER(find_max)
        struct Compare current_max = find_max_betweenness();
    STOP_TIMER(find_max);

    printf("\tTime: %f\n", GET_TIMER(find_max));

    return 0;
}

struct Compare find_max_betweenness() {
    int curr_edge_id; // current value in ecount
    struct Compare max_edge; // the struct 
    max_edge.max_betweenness_score = -1; // max score given dummy value
    max_edge.edge_id = -1; // edge ID of max score given dummy value

#   pragma omp parallel for default(none) shared(ecount, graph, max_edge) \
    private(curr_edge_id)
    for (curr_edge_id = 0; curr_edge_id < ecount; curr_edge_id+=1) {
        igraph_vector_t result; // holds betweenness score 
        igraph_vector_init(&result, 0); // initialize result vector
        igraph_edge_betweenness(&graph, &result, IGRAPH_UNDIRECTED, 0);
        double betweenness_score = (double) VECTOR(result)[0];

#       pragma omp critical
        if (betweenness_score > max_edge.max_betweenness_score) {
            max_edge.max_betweenness_score = betweenness_score;
            max_edge.edge_id = curr_edge_id;
        }

        igraph_vector_destroy(&result); // every vector object is destroyed
    }

    printf("\tEdge: %d\t", max_edge.edge_id);
    return max_edge; 
}


//end of file
