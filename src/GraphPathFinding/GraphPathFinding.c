#include "GraphPathFinding.h"
#include <stdlib.h>

int test(void)
{
    return 123456;
}

int floyd_warshall(GRAPH* graph)
{
    if(graph == NULL)
    {
        return -1;
    }

    int k = 0;
    int i = 0;
    int j = 0;

    for (k = 1; k < graph->nrVertices; k++) // Set the inital relax vertex to the first vertex.
    {
        for (i = 0; i < graph->nrVertices; i++) // For each vertex check if the shorted path from i to j
                                                // is shorter in the last aspMatrix using K as a relax vertex.
        {
            for (j = 0; i < graph->nrVertices; j++) // Take every vertex as a destination vertex.
            {
                int currentWeight = graph->aspMatrix[i][j];
                int pathWeigthUsingK = graph->aspMatrix[i][k] + graph->aspMatrix[k][j];
                if(currentWeight > pathWeigthUsingK)
                {
                    graph->aspMatrix[i][j] = pathWeigthUsingK;
                    graph->predMatrix[i][j] = graph->predMatrix[k][j];
                }
            }
        }
    }
    return 0;
}