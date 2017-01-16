#ifndef PATH_FINDING_H
#define PATH_FINDING_H

#include "../GraphStructure/GraphStructure.h"

int floyd_warshall(GRAPH* graph);
// Variables:
//     graph: The graph of which the all pairs shorted path and
//            predecessor matrix is requested.adjMatrix
// Pre: -
// Post: aspMatrix and predMatrix of the graph is assigned.
// Retun: 0 on succes.
//       -1 when graph is null or an error occured.

char* getShortestPath(GRAPH* graph, char startVertex, char destinationVertex, int* nrVerticesInPath);

#endif