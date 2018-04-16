#include <stdbool.h>
#include <stdio.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "graph.h"
#include "util.h"

int main(int argc, char *argv[]) {
    igraph_t graph = create_graph(argv[1]); // creates graph of given dataset

    fprintf(stderr, "Warning. edge_bt is not parallelized\n");

    int ecount = igraph_ecount(&graph); // number of edges in graph

    START_TIMER(find_max)
    compare_t current_max = find_max_betweenness(&graph, ecount);
    STOP_TIMER(find_max);

    printf("\tTime: %f\n", GET_TIMER(find_max));

    return 0;
}

/*
 * The igraph library finds the edge with the greatest number of shortest
 * paths passing through it.
 */
compare_t find_max_betweenness(igraph_t *graph, int ecount) {

  igraph_vector_t eb;  
  igraph_vector_init(&eb, ecount);
  igraph_edge_betweenness(graph, &eb, IGRAPH_UNDIRECTED, 0);

  int curr_edge_id;
  compare_t max_edge; // the struct 
  max_edge.max_betweenness_score = -1; // max score given dummy value
  max_edge.id = -1; // edge ID of max score given dummy value

  for (curr_edge_id = 0; curr_edge_id < ecount; curr_edge_id+=1) {
        double betweenness_score = (double) VECTOR(eb)[curr_edge_id];
        if (betweenness_score > max_edge.max_betweenness_score) {
            max_edge.max_betweenness_score = betweenness_score;
            max_edge.id = curr_edge_id;
        }
    }

  printf("\tEdge: %d\t", max_edge.id);
  igraph_vector_destroy(&eb);
  igraph_destroy(graph);
  return max_edge;

}

//end of file
