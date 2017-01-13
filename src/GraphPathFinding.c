#include "PathFinding.h"

int floyd_warshall(GRAPH *graph)
{
    int k = 0;
    int i = 0;
    int j = 0;

    for (k = 1; k < graph.nrVertices; k++) // Set the inital relax vertex to the first vertex.
    {
        for (i = 0; i < graph.nrVertices; i++)
        {
            for (j = 0; i < graph.nrVertices; j++)
            {
                int currentLength = graph.aspMatrix[i][j];
                int pathWeigthUsingK = graph.aspMatrix[i][k] + graph.aspMatrix[k][j];
                if(currentLength > pathWeigthUsingK)
                {
                    graph.aspMatrix[i][j] = pathWeigthUsingK;
                    graph.predMatrix[i][j] = graph.vertices[k];
                }
            }
        }
    }
}