/**
 * Vertex betweenness calculates the maximum betweenness score, which is
 * the vertex that has the greatest number of shortest paths passing
 * through it. This is accomplished by calculating the betweenness score
 * of each vertex then comparing the current max score of some vertex 
 * with the score just calculated. 
 **/

// Global Variables
igraph_t graph; // the graph 
int vcount;  // the number of vertices in graph

// This struct will hold the maximum betweenness score and the vertex
// that has that maximum betweenness score. The maximum betweenness
// score is the vertex that has the most shortest paths passing through
// it. The max score will be compared to the score calculated for each
// vertex and replaced if that vertex has a larger score. The vertex
// id will also be changed to the id of that vertex. The ID is given
// by iterating through the number of vertices in the graph (vcount)
// and assigning the current vertex number to the vertex ID variable.
typedef struct Compare {
    double max_betweenness_score; // max score out of all vertices 
    int vertex_id; // ID given by iterating through vcount
} Compare;

// Function Prototype for calculating the max betweenness score
struct Compare find_max_betweenness();

/**
 * Calculates the vertex with the maximum betweenness score by calling
 * find_max_betweenness() and prints the execution time.
 **/
int main(int argc, char *argv[]) {
    graph = create_graph(argv[1]); // creates graph of given dataset

    vcount = igraph_vcount(&graph); // number of vertices in graph

    START_TIMER(find_max)
        struct Compare current_max = find_max_betweenness();
    STOP_TIMER(find_max);

    printf("Time: %f\n", GET_TIMER(find_max));

    return 0;
}

/**
 * Calculates the maximum betweenness score of the graph.
 **/
struct Compare find_max_betweenness() {
    int curr_vertex_id; // current value in vcount
    // the max betweenness score and vertex id that has that max score
    // will both have dummy values of negative one since no vertex from
    // the graph has had its betweenness score calculated yet.
    struct Compare max_vertex; // the struct 
    max_vertex.betweenness = -1; // max score given dummy value
    max_vertex.vid = -1; // vertex ID of max score given dummy value

    // Iterate over the number of vertices in the graph. Each iteration
    // will represent a new vertex.
    for (vertex_id = 0; vertex_id < vcount; vertex_id+=1) {
        igraph_vs_t curr_vertex; // holds the current vertex
        igraph_vector_t result; // holds betweenness score 
        igraph_vector_init(&result, 0); // initialize result vector
        igraph_vs_1(&curr_vertex, vertex_id); // get current vertex
        igraph_betweenness(&graph, &result, curr_vertex, IGRAPH_UNDIRECTED, 0, 1);
        double betweenness_score = (double) VECTOR(result)[0];

	// If the betweenness score of the current vertex is greater
	// than the maximum betweenness score than replace with the
	// current vertex.
        if (betweenness_score > max_vertex.max_betweenness_score) {
            max_vertex.max_betweenness_score = betweenness_score;
            max_vertex.vertex_id = vertex_id;
        }

        igraph_vector_destroy(&result); // every vector object is destroyed
        igraph_vs_destroy(&curr_vertex); // every vertex selector is destroyed
    }

    printf ("Vertex: %d\t", max_vertex.vertex_id);
    return max_vertex; 
}


//end of file
