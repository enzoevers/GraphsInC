#include "GraphPathFinding.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include <stdio.h>

static int indexOf(GRAPH* graph, char value)
{
    int index = 0;
    char foundIndexOfDestinationVertex = 'n';
    for(int i = 0; i < graph->nrVertices && foundIndexOfDestinationVertex == 'n'; i++)
    {
        if(graph->vertices[i] == value)
        {
            index = i;
            foundIndexOfDestinationVertex = 'y';
        }
    }

    return index;
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

    for (k = 0; k < graph->nrVertices; k++) // Set the inital relax vertex to the first vertex.
    {
        for (i = 0; i < graph->nrVertices; i++) // For each vertex check if the shorted path from i to j
                                                // is shorter in the last aspMatrix using K as a relax vertex.
        {
            for (j = 0; j < graph->nrVertices; j++) // Take every vertex as a destination vertex.
            {
                if(graph->aspMatrix[i][k] != INT_MAX && graph->aspMatrix[k][j] != INT_MAX)
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
    }
    return 0;
}


char original_dest = '-';
char vertexDevider = '-';
int indexOfStartVertexInArray = 0;
int indexOfDestinationVertexInArray = 0;
char recurcionStarted = 'n';
char foundShortestPath = 'n';
char* path;

char* getShortestPath(GRAPH* graph, char startVertex, char destinationVertex, int* nrVerticesInPath)
{
    if(graph == NULL || nrVerticesInPath == NULL)
    {
        return NULL;
    }

    // Get the index of the current destination vertex in the graph.vertices.
    indexOfDestinationVertexInArray = indexOf(graph, destinationVertex);

    // Assign the variables that are always the same.
    if(recurcionStarted == 'n')
    {
        *nrVerticesInPath = 1;
        original_dest = destinationVertex;
        indexOfStartVertexInArray = indexOf(graph, startVertex);

        recurcionStarted = 'y';
    }

    if(startVertex == destinationVertex)
    {
        // To add '-' between vertices pathLength-1 '-' are needed.
        // The '\0' needs to be added cancalling out the -1 in pathLength-1.
        if(foundShortestPath == 'n')
        {
            path = (char*)calloc(2*(*nrVerticesInPath), sizeof(char));
            foundShortestPath = 'y';
        }

        strncat(path, &destinationVertex, 1);
        if(destinationVertex != original_dest)
        {
            strncat(path, &vertexDevider, 1);
        }
    }
    else if(graph->predMatrix[indexOfStartVertexInArray][indexOfDestinationVertexInArray] != '-')
    {
        (*nrVerticesInPath)++;
        getShortestPath(graph, startVertex, graph->predMatrix[indexOfStartVertexInArray][indexOfDestinationVertexInArray], nrVerticesInPath);

        strncat(path, &destinationVertex, 1);
        if(destinationVertex != original_dest)
        {
            strncat(path, &vertexDevider, 1);
        }

        recurcionStarted = 'n';
        foundShortestPath = 'n';

        return path;
    }

    return NULL;
}