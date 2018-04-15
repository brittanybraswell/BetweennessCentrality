#ifndef _UTIL_H
#define _UTIL_H

// STRUCTS

// This struct will hold the maximum betweenness score and the vertex
// that has that maximum betweenness score. The maximum betweenness
// score is the vertex that has the most shortest paths passing through
// it. The max score will be compared to the score calculated for each
// vertex and replaced if that vertex has a larger score. The vertex
// id will also be changed to the id of that vertex. The ID is given
// by iterating through the number of vertices in the graph (vcount)
// and assigning the current vertex number to the vertex ID variable.
typedef struct Compare {
    double max_betweenness_score; // max score found so far
    int id; // id of the high-scoring edge
} compare_t;


// FUNCTION PROTOTYPES

compare_t find_max_betweenness(igraph_t*, int);

#endif
