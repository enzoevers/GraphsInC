#include "MakeGraph.h"
#include <math.h> // To use INFINITY
#include <stdlib.h>

static double** makeMatrixNxN_double(int rows, int columns);
static char** makeMatrixNxN_char(int rows, int columns);
static int fillAdjacencyMatrix(GRAPH graph);

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

int makeGraph(GRAPH* graph, char** edges, int nrEdges, char* vertices, int nrVertices)
{
	if(edges == NULL || vertices == NULL || nrEdges < 1 || nrVertices < 1)
	{
		return -1;
	}

	clearGraph(graph);
    graph->nrVertices = nrVertices;
    graph->nrEdges = nrEdges;
    graph->vertices = vertices;
    graph->edges = edges;

	double** matrixSizeVertices_double = makeMatrixNxN_double(nrVertices, nrVertices);
    char** matrixSizeVertices_char = makeMatrixNxN_char(nrVertices, nrVertices);

	graph->adjMatrix = matrixSizeVertices_double;
	graph->aspMatrix = matrixSizeVertices_double;
	graph->predMatrix = matrixSizeVertices_char;

    fillAdjacencyMatrix(graph);

	return 0;
}

static double** makeMatrixNxN_double(int rows, int columns)
{
    if (rows < 1 || columns < 1)
    {
        return NULL;
    }
	double** matrix = (double**)malloc(rows*sizeof(double*));

    int i = 0;
	for (i = 0; i < rows; i++)
	{
		matrix[i] = (double*)malloc(columns*sizeof(double));
	}

    return matrix;
}

static char** makeMatrixNxN_char(int rows, int columns)
{
    if (rows < 1 || columns < 1)
    {
        return NULL;
    }
    char** matrix = (char**)malloc(rows*sizeof(char*));

    int i = 0;
    for (i = 0; i < rows; i++)
    {
        matrix[i] = (char*)malloc(columns*sizeof(char));
    }

    return matrix;
}

static int fillAdjacencyMatrix(GRAPH* graph)
{
	double weight = INFINITY;
	int state = 0;
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
    		}
    		else
    		{
                // graph->edges is an 2D array with nrEdges rows and 2 columns (column 1 = start; 2 = destination).
                // For each edge check if i is the start vertex and j is the destination vertex.
    		 	int edgeFound = 0;
    			for (k = 0; k < graph->nrEdges && edgeFound == 0; k++)
    			{
    				if(graph->edges[k][0] == i && graph->edges[k][1] == j)
    				{
                        // Determine is the graph has a weights array.
                        // If not the default weights of an edge will be 1.
    					weight = (graph->weights == NULL) ? 1 : graph->weights[i];
    					edgeFound = 1; // Set the flag to break out of the for-loop.
    				}
    			}

                // If no edge {i, j} is found set the weight to INFINITY.
                if(edgeFound == 0)
                {
                    weight = INFINITY;
                }
    		}

            // Assign the weight to the right place in the adjacency matrix.
    		graph->adjMatrix[i][j] = weight;
    	}
    }

    return state;
}