#ifndef MAKE_GRAPH_H
#define MAKE_GRAPH_H

#include "GraphStructure.h"

GRAPH makeGraph(int** edges, int nrEdges, char* vertices, int nrVertices);
// Varaibles:
//      edges: A pointer to the a 2D array/matrix were column 0 is the begin vertex and
//             column 1 is destination vertex.
//      nrEdges: The amount of edges / rows in the edges matrix.
//      vertices: A pointer to the an array with all the vertices names.
//      nrVertices: The amount of vertices.
// Pre: Each index in the edges array must have a array of 2 indexes.
// Post: A new graph is made and represented int de way of an adjacency matrix.
//       Also the all-pairs shortest path and predecessor matrix is made. 
//       The default weigth of the edges is 1.
//       The structure of GRAPH can be found in GraphStructure.h.
// Return: A graph with attributes found in the GRAPH struct in GraphStructure.h

GRAPH makeWeightedGraph(int** edges, int* weigths, int nrEdges, char* vertices, int nrVertices);
// Varaibles:
//      edges: A pointer to the a 2D array/matrix were column 0 is the begin vertex and
//             column 1 is destination vertex.
//      nrEdges: The amount of edges / rows in the edges matrix.
//      weigths: The indexes of in the array correspond to the rows of the edges matrix.
//      vertices: A pointer to the an array with all the vertices names.
//      nrVertices: The amount of vertices.
// Pre: Each index in the edges array must have a array of 2 indexes. 
//      The weights array must have the same length and de number of rows in the edges matrix (this being nrEdges).
// Post: A new weighted graph is made and represented int de way of an adjacency matrix.
//       Also the all-pairs shortest path and predecessor matrix is made. 
//       The structure of GRAPH can be found in GraphStructure.h.
// Return: A weighted graph with attributes found in the GRAPH struct in GraphStructure.h


#endif