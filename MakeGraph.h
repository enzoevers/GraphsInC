#ifndef MAKE_GRAPH_H
#define MAKE_GRAPH_H

#include "GraphStructure.h"

GRAPH makeGraph(int* u, int* v, int nrEdges, int* vertices, int nrVertices);
// Varaibles:
//      u: a pointer to the 
// Pre:
// Post: A new graph is made and represented int de way of an adjacency matrix.
//       Also the all-pairs shortest path and predecessor matrix is made. 
//       The default weigth of the edges is 1.
//       The structure of GRAPH can be found in GraphStructure.h.
// Return: A graph with 


#endif