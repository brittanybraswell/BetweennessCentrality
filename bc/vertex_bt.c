/**
 * Vertex betweenness calculates the maximum betweenness score, which is
 * the vertex that has the greatest number of shortest paths passing
 * through it. This is accomplished by calculating the betweenness score
 * of each vertex then comparing the current max score of some vertex 
 * with the score just calculated. 
 **/

#ifdef _OPENMP
#include <omp.h>
#endif

#include "graph.h"
#include "util.h"

/**
 * Calculates the vertex with the maximum betweenness score by calling
 * find_max_betweenness() and prints the execution time.
 **/
int main(int argc, char *argv[]) {
    igraph_t graph = create_graph(argv[1]); // creates graph of given dataset

    int vcount = igraph_vcount(&graph); // number of vertices in graph

    START_TIMER(find_max)
    compare_t current_max = find_max_betweenness(&graph, vcount);
    STOP_TIMER(find_max);

    printf("Time: %f\n", GET_TIMER(find_max));

    return 0;
}

/**
 * Calculates the maximum betweenness score of the graph.
 **/
compare_t find_max_betweenness(igraph_t *graph, int vcount) {
    int curr_vertex_id; // current value in vcount
    // the max betweenness score and vertex id that has that max score
    // will both have dummy values of negative one since no vertex from
    // the graph has had its betweenness score calculated yet.
    compare_t max_vertex; // the struct 
    max_vertex.max_betweenness_score = -1; // max score given dummy value
    max_vertex.id = -1; // vertex ID of max score given dummy value

    // Iterate over the number of vertices in the graph. Each iteration
    // will represent a new vertex.
#   pragma omp parallel for default(none) shared(vcount, graph, max_vertex) \
    private(curr_vertex_id)
    for (curr_vertex_id = 0; curr_vertex_id < vcount; curr_vertex_id+=1) {
        igraph_vs_t curr_vertex; // holds the current vertex
        igraph_vector_t result; // holds betweenness score 
        igraph_vector_init(&result, 0); // initialize result vector
        igraph_vs_1(&curr_vertex, curr_vertex_id); // get current vertex
        igraph_betweenness(graph, &result, curr_vertex, IGRAPH_UNDIRECTED, 0, 1);
        double betweenness_score = (double) VECTOR(result)[0];
	// If the betweenness score of the current vertex is greater
	// than the maximum betweenness score than replace with the
	// current vertex.
#       pragma omp critical
        if (betweenness_score > max_vertex.max_betweenness_score) {
            max_vertex.max_betweenness_score = betweenness_score;
            max_vertex.id = curr_vertex_id;
        }

        igraph_vector_destroy(&result); // every vector object is destroyed
        igraph_vs_destroy(&curr_vertex); // every vertex selector is destroyed
    }

    printf("\tVertex: %d\t", max_vertex.id);
    return max_vertex; 
}

//end of file
