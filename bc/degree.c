#include <stdbool.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "graph.h"
#include "util.h"

/*
 * This is not actually an example of betweenness centrality. It is
 * degree centrality. It is just in this folder to make things easier for us.
 */

struct {
    double score;
    int id;
} winner;

double get_degree(igraph_t*, int);

int main(int argc, char *argv[]) {
    igraph_t graph = create_directed_graph(argv[1]); // creates graph of given dataset

    int vcount = igraph_vcount(&graph); // number of edges in graph

    START_TIMER(find_max)
    /*
     * This doesn't actually scale particularly well. In fact, it appears
     * that in most all cases, the OMP version is significantly worse than
     * the original serial version and then using any OMP_THREAD_COUNT
     * results in approximately the same time regardless of the number of
     * threads.
     *
     * This actually makes some sense. Degree centrality is a very local
     * view of the graph; you're simply seeing how many edges point to each
     * vertex, and each thread gets assigned particular vertices to check.
     * This is a relatively fast operation and without an absolutely
     * enormous number of nodes, the speedup from parallelizing is not
     * sufficient to overcome the penalties of locking/unlocking for the
     * critical section.
     */
#   pragma omp parallel for default(none) shared(winner, graph, vcount)
    for (int vertex = 0; vertex < vcount; vertex++) {
        double result = get_degree(&graph, vertex);
#       pragma omp critical
        if (result > winner.score) {
            winner.score = result;
            winner.id = vertex;
        }
    }
    STOP_TIMER(find_max);

    printf("Vertex: %d\t", winner.id);
    printf("Time: %f\n", GET_TIMER(find_max));

    return 0;
}

double get_degree(igraph_t *graph, int vertex_id) {
    igraph_vector_t degree;
    igraph_vs_t vs = igraph_vss_1(vertex_id);

    igraph_vector_init(&degree, 1);

    igraph_degree(graph, &degree, vs, IGRAPH_ALL, true);

    int result = VECTOR(degree)[0];

    igraph_vector_destroy(&degree);

    return result;
}
