#include "File_io.h"
#include <stdio.h>

int writeCompleteGraphStruct(char* filename, GRAPH* graphArray, int nrGraphInArray)
{
	if(filename == NULL || graphArray == NULL || nrGraphInArray < 1)
	{
		return -1;
	}

	// Check if filename can be opened in append mode.
	FILE* fileToWrite = fopen(filename, "a");
	if(fileToWrite == NULL)
	{
		return -1;
	} 

	// Return the amount of graph written to the file.
	return fwrite(graphArray, sizeof(GRAPH), nrGraphInArray, fileToWrite);
}

int writeCompleteGraphHumanReadable(char* filename, GRAPH* graph)
{
	if(filename == NULL || graph == NULL)
	{
		return -1;
	}

	// Only go further with writing to the file if no error occurred.

	int result = -1;

	result = write_name(filename, graph);

	if(result == 0)
	{
		result = write_vericesAndEdges(filename, graph);
	}

	if(result == 0)
	{
		result = write_adjacencyMatrix(filename, graph);
	}

	if(result == 0)
	{
		result = write_FloydWarshall(filename, graph);
	}

	if(result == 0)
	{
		result = write_predecessorMatrix(filename, graph);
	}

	return result;
}

int write_name(char* filename, GRAPH* graph)
{
	if(filename == NULL || graph == NULL)
	{
		return -1;
	}

	FILE* fileToWrite = fopen(filename, "r+")
}
// Pre: -
// Post: The name of the graph is written to the file.
//       - If the filename doesn't exist the file is made and the name is written. 
//       - If filename does exist the name is written to the correct place. (see format above).
//         
// Return: -1 if file couldn't be made or opened or filename or graph is NULL.
//          0 in succes. 

int write_vericesAndEdges(char* filename, GRAPH* graph)
{

}
// Pre: -
// Post: The vertices and edges of the graph are written to the file.
//       - If the filename doesn't exist the file is made and the vertices and edges are written. 
//       - If filename does exist the vertices and edges are written to the correct place. (see format above).
//         
// Return: -1 if file couldn't be made or opened or filename or graph is NULL.
//          0 in succes.

int write_adjacencyMatrix(char* filename, GRAPH* graph)
{

}
// Pre: -
// Post: The adjacency matrix of the graph is written to the file.
//       - If the filename doesn't exist the file is made and the matrix is written. 
//       - If filename does exist the adjacency matrix is written to the correct place. (see format above).
//         
// Return: -1 if file couldn't be made or opened or filename or graph is NULL.
//          0 in succes.

int write_FloydWarshall(char* filename, GRAPH* graph)
{

}
// Pre: -
// Post: The Floyd-Warshall matrix of the graph is written to the file.
//       - If the filename doesn't exist the file is made and the matrix is written. 
//       - If filename does exist the Floyd-Warshall matrix is written to the correct place. (see format above).
//         
// Return: -1 if file couldn't be made or opened or filename or graph is NULL.
//          0 in succes.

int write_predecessorMatrix(char* filename, GRAPH* graph)
{

}
// Pre: -
// Post: The predecessor matrix of the graph is written to the file.
//       - If the filename doesn't exist the file is made and the matrix is written. 
//       - If filename does exist the predecessor matrix is written to the correct place. (see format above).
//         
// Return: -1 if file couldn't be made or opened or filename or graph is NULL.
//          0 in succes.

int readGraphFromStructFile(char* filename, GRAPH* graphArray, int graphArraySize, int nrGraphsToRead)
{

}
// Pre: -
// Post: nrGraphsToRead are read from filename into graphArray. 
//       If nrGraphsToRead > graphArraySize only nrGraphsToRead graphs are read.
//       if nrGraphsToRead > number-of-graphs-in-file only number-of-graphs-in-file graphs are read. 
// Return: -1 if filename or graphArray is NULL, or graphArraySize or nrGraphsToRead < 1.
//         Otherwise return number or graphs read.

int readGraphFromHumanReadableFile(char* filename, GRAPH* graph)
{

}
// Pre: -
// Post: The graph properties described in the file formated as above are read 
//       and translated into graph.
// Return: -1 if filename or graph is NULL or filename doens't containthe required (*) properties.
//          0 on succes.