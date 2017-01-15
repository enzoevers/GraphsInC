#include "MakeGraph.h"
#include "../GraphPathFinding/GraphPathFinding.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

static void fillAdjacencyMatrix(GRAPH* graph);

void clearGraph(GRAPH* graph)
{
    graph->nrVertices = 0;
    graph->nrEdges = 0;
    graph->vertices = NULL;
    graph->weights = NULL;
    graph->edges = NULL;
    graph->adjMatrix = NULL;
    graph->aspMatrix = NULL;
    graph->predMatrix = NULL;
}

int makeGraph(GRAPH* graph, char (*edges)[2], int nrEdges, char* vertices, int nrVertices)
{
	if(graph == NULL || edges == NULL || vertices == NULL || nrEdges < 1 || nrVertices < 1)
	{
		return -1;
	}

	clearGraph(graph);
    graph->nrVertices = nrVertices;
    graph->nrEdges = nrEdges;
    graph->vertices = vertices;
    graph->edges = edges;

	int** matrixSizeVertices_int = makeMatrixNxN_double(nrVertices, nrVertices);
    char** matrixSizeVertices_char = makeMatrixNxN_char(nrVertices, nrVertices);

	graph->adjMatrix = matrixSizeVertices_int;
	graph->aspMatrix = matrixSizeVertices_int;
	graph->predMatrix = matrixSizeVertices_char;

    fillAdjacencyMatrix(graph);
    //floyd_warshall(graph);

	return 0;
}

int** makeMatrixNxN_double(int rows, int columns)
{
    if (rows < 1 || columns < 1)
    {
        return NULL;
    }

	int** matrix = (int**)malloc(rows*sizeof(int*));

	for (int i = 0; i < rows; i++)
	{
		matrix[i] = (int*)malloc(columns*sizeof(int));
        for(int k = 0; k < columns; k++)
        {
            matrix[i][k] = INT_MAX;
        }
	}

    return matrix;
}

char** makeMatrixNxN_char(int rows, int columns)
{
    if (rows < 1 || columns < 1)
    {
        return NULL;
    }
    char** matrix = (char**)malloc(rows*sizeof(char*));

    for (int i = 0; i < rows; i++)
    {
        matrix[i] = (char*)malloc(columns*sizeof(char));
        for(int k = 0; k < columns; k++)
        {
            matrix[i][k] = '\0';
        }
    }

    return matrix;
}

static void fillAdjacencyMatrix(GRAPH* graph)
{
	int weight = INT_MAX;
    char predVertex = '-';
    int i = 0;
    int j = 0;
    int k = 0;

    for(i = 0; i < graph->nrVertices; i++)
    {
        //printf("i = %d\n", i);
    	for (j = 0; j < graph->nrVertices; j++)
    	{
            //printf("j = %d\n", j);
    		if(i == j)
    		{
    			weight = 0; 
    		}
    		else
    		{
                // graph->edges is an 2D array with nrEdges rows and 2 columns (column 1 = start; 2 = destination).
                // For each edge check if i is the start vertex and j is the destination vertex.
    		 	int edgeFound = 0;
    			for (k = 0; k < graph->nrEdges && edgeFound == 0; k++)
    			{
                    //printf("graph->edges[%d][0] = %c graph->edges[%d][1] = %c\n", k, graph->edges[k][0], k, graph->edges[k][1]);
    				if(graph->edges[k][0] == graph->vertices[i] && graph->edges[k][1] == graph->vertices[j])
    				{
                        predVertex = graph->vertices[i];
                        // Determine is the graph has a weights array.
                        // If not the default weights of an edge will be 1.
    					weight = (graph->weights == NULL) ? 1 : graph->weights[i];
                        //printf("Weight[%d][%d] = %d\n", i, j, weight);
    					edgeFound = 1; // Set the flag to break out of the for-loop.
    				}
    			}

                // If no edge {i, j} is found set the weight to INT_MAX.
                if(edgeFound == 0)
                {
                    weight = INT_MAX;
                    predVertex = '-';
                }
    		}

            // Assign the weight to the right place in the adjacency and all-pairs shortest path matrix
            // And assing prevevious vertex to the right place in the predecessor matrix.
            graph->predMatrix[i][j] = predVertex;
    		graph->adjMatrix[i][j] = weight;
            graph->aspMatrix[i][j] = weight;
    	}
    }
}