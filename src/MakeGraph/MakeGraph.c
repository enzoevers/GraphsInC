#include "MakeGraph.h"
#include "../GraphPathFinding/GraphPathFinding.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

static void fillAdjacencyMatrix(GRAPH* graph);
static void assignMatrices(GRAPH* graph, int** (*makeMatrix_int)(int, int), char** (*makeMatrix_char)(int, int));

void clearGraph(GRAPH* graph)
{
    free(graph->adjMatrix);
    free(graph->aspMatrix);
    free(graph->predMatrix);

    graph->name = NULL;
    graph->nameLength = 0;
    graph->nrVertices = 0;
    graph->nrEdges = 0;
    graph->vertices = NULL;
    graph->weights = NULL;
    graph->edges = NULL;
    graph->adjMatrix = NULL;
    graph->aspMatrix = NULL;
    graph->predMatrix = NULL;
}

int makeGraph(GRAPH* graph, char* name, int nameLength, char (*edges)[2], int nrEdges, char* vertices, int nrVertices)
{
	return makeWeightedGraph(graph, name, nameLength, edges, NULL, nrEdges, vertices, nrVertices);
}

int makeWeightedGraph(GRAPH* graph, char* name, int nameLength, char (*edges)[2], int* weights, int nrEdges, char* vertices, int nrVertices)
{
    if(graph == NULL || edges == NULL || vertices == NULL || nrEdges < 1 || nrVertices < 1)
    {
        return -1;
    }

    clearGraph(graph);
    graph->name = name;
    graph->nameLength = nameLength;
    graph->nrVertices = nrVertices;
    graph->nrEdges = nrEdges;
    graph->vertices = vertices;
    graph->edges = edges;
    graph->weights = weights;
    
    assignMatrices(graph, makeMatrixNxN_int, makeMatrixNxN_char);

    fillAdjacencyMatrix(graph);
    floyd_warshall(graph);

    return 0;
}

static void assignMatrices(GRAPH* graph, int** (*makeMatrix_int)(int, int), char** (*makeMatrix_char)(int, int))
{
    graph->adjMatrix = makeMatrix_int(graph->nrVertices, graph->nrVertices);
    graph->aspMatrix = makeMatrix_int(graph->nrVertices, graph->nrVertices);
    graph->predMatrix = makeMatrix_char(graph->nrVertices, graph->nrVertices);
}

int** makeMatrixNxN_int(int rows, int columns)
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
    	for (j = 0; j < graph->nrVertices; j++)
    	{
    		if(i == j)
    		{
    			weight = 0; 
                predVertex = '-';
    		}
    		else
    		{
                // graph->edges is an 2D array with nrEdges rows and 2 columns (column 1 = start; 2 = destination).
                // For each edge check if i is the start vertex and j is the destination vertex.
    		 	int edgeFound = 0;
    			for (k = 0; k < graph->nrEdges && edgeFound == 0; k++)
    			{
    				if(graph->edges[k][0] == graph->vertices[i] && graph->edges[k][1] == graph->vertices[j])
    				{
                        // Determine is the graph has a weights array.
                        // If not the default weights of an edge will be 1.
    					weight = (graph->weights == NULL) ? 1 : graph->weights[k];
                        predVertex = graph->vertices[i];

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
    		graph->adjMatrix[i][j] = weight;
            graph->aspMatrix[i][j] = weight;
            graph->predMatrix[i][j] = predVertex;
    	}
    }
}