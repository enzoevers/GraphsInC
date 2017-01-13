#include "MakeGraph.h"
#include <math.h> // To use INFINITY
#include <stdlib.h>

GRAPH* makeGraph(char** edges, int nrEdges, char* vertices, int nrVertices)
{
	if(edges == NULL | vertices == NULL | nrEdges < 1 | nrVertices < 1)
	{
		return NULL;
	}

	GRAPH graph = {nrVertices, nrEdges, vertices, NULL, edges, NULL, NULL, NULL};

	int** matrixSizeVertices = makeMatrixNxN(nrVertices, nrVertices);

	graph.adjMatrix = matrixSizeVertices;
	graph.aspMatrix = matrixSizeVertices;
	graph.predMatrix = matrixSizeVertices;

	return &graph;
}

int** makeMatrixNxN(int rows, int columns)
{
	int** matrix = (int**)malloc(rows*sizeof(int*));
	for (int i = 0; i < n; i++)
	{
		matrix[i] = (int*)malloc(columns*sizeof(int));
	}
}

int fillAdjacencyMatrix(GRAPH graph)
{
	int weight = INFINITY;
	int state = 0;

    for(int i = 0; i < graph.nrVertices; i++)
    {
    	for (int j = 0; j < graph.nrVertices; j++)
    	{
    		if(i == j)
    		{
    			weight = INFINITY; 
    		}
    		else
    		{
    		 	uint8_t edgeFound = 0;
    			for (int i = 0; i < graph.nrEdges && edgeFound == 0; i++)
    			{
    				if(graph.edges[i][0] == i && graph.edges[i][1] == j)
    				{
    					weight = (graph.w == NULL) ? 1 : graph.w[i];
    					edgeFound = 1;
    				}
    			}
    		}

    		graph.adjMatrix[i][j] = weight;
    	}
    }

    return state;
}