#ifndef GRAPH_STRUCTURE_H
#define GRAPH_STRUCTURE_H

typedef struct
{
    char* name; // A name for the graph to recognize it.
    int nameLength;

    int nrVertices;
    int nrEdges;

    char* vertices; // An  array with all names of the vertices.

    int* weights; // An array with weigths of edges. The index of the weigths
                  // correspond to the index of the rows in edges.

    char (*edges)[2]; // A 2D array with 2 columns. Each row is a edge where
                      // column 0 is the start vertex and column 1 is the destination vertex.

    int** adjMatrix; // A pointer to the the adjacency matrix.
                     // The matrix can be used by adjMatrix[row][column] e.i. adjMatrix[from][to]
    
    int** aspMatrix; // A pointer to the the all-pairs shortest path matrix.
                    // Each value of aspMatrix[i][j] gives the shortest path weigth from i to j.
                    // The largest value in the matrix if the value isn't NIL is the diameter of the graph.

    char** predMatrix; // A pointer to the the predecessor matrix.
                      // Each value in the matrix can be read as the previous vertex on the shortest path
                      // from i to j where predMatrix[i][j]
                      //    To get the shortest path using this matrix you have to look at the value (lets say k) in [i][j].
                      //    while k isn't i:
                      //        Look at the value of [i][k] and assing this value to k.
                      //        Append k to an array.
                      //                    ***!TO IMPLEMENT MAKE RECURSIVE!***
                      //By doing this and reversing the array you have the shortest path from i to j.
}GRAPH;

#endif