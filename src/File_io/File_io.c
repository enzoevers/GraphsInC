#include "File_io.h"
#include <string.h>

#include <stdio.h>

// Returns the number of bytes in a file.
long fileSize(char* filename)
{
	if(filename == NULL)
	{
		return -1;
	}

	FILE* filePtr = fopen(filename, "r");
	if(filePtr == NULL)
	{
		return -1;
	}

	fseek(filePtr, 0, SEEK_END);
	long size = ftell(filePtr);
	fseek(filePtr, 0, SEEK_SET);

	fclose(filePtr);

	return size;
}

// Return the index of the first encounter of stringToFind. 
long findInFile(char* filename, char* stringToFind, int stringLength)
{
	if(filename == NULL || stringToFind == NULL || stringLength < 1)
	{
		return -1;
	}

	FILE* filePtr = fopen(filename, "r");
	if(filePtr == NULL)
	{
		return -1;
	}

	char readString[stringLength+1];
	memset(readString, 0, stringLength);
	readString[stringLength+1] = '\0';

	long fileLength = fileSize(filename);
	long index = 0;
	int freadResult = 0;

	fseek(filePtr, index, SEEK_SET);
	freadResult = fread(readString, sizeof(char), stringLength, filePtr);

	while(strncmp(readString, stringToFind, stringLength) != 0 && index + stringLength <= fileLength && freadResult == stringLength)
	{
		index++;
		fseek(filePtr, index, SEEK_SET);
		memset(readString, 0, stringLength);
		freadResult = fread(readString, sizeof(char), stringLength, filePtr);
	}

	return (index + stringLength > fileLength) ? -1 : index;
}

int copyFile(char* originalFilename, FILE* tempFile, long cpyFrom, long cpyTo)
{
	if(originalFilename == NULL || tempFile == NULL)
	{
		return -1;
	}

	FILE* originalFile = fopen(originalFilename, "r");

	if(originalFile == NULL)
	{
		return -1;
	}

	long originalFile_Size = fileSize(originalFilename);

	if(cpyFrom < 0 || cpyFrom > originalFile_Size || cpyTo < 0 || cpyTo > originalFile_Size)
	{
		return -1;
	}

	// Make sure the file pointers are at the start.
    fseek(originalFile, cpyFrom, SEEK_SET);
    fseek(tempFile, 0, SEEK_SET);

	char charToCopy;
	int readItems = 0;
	while(ftell(originalFile) <= cpyTo)
	{	
        readItems += fread(&charToCopy, sizeof(char), 1, originalFile);
	    fwrite(&charToCopy, sizeof(char), 1, tempFile);
	}

	fclose(originalFile);

	return readItems;
}

int appendFile(char* originalFilename, FILE* tempFile, long pstFrom, long pstTo)
{
	if(originalFilename == NULL || tempFile == NULL)
	{
		return -1;
	}

	FILE* originalFile = fopen(originalFilename, "a");

	if(originalFile == NULL)
	{
		return -1;
	}

	fseek(tempFile, 0, SEEK_END);
	long tempFileSize = ftell(tempFile);
	fseek(tempFile, 0, SEEK_SET);

	if(pstFrom < 0 || pstFrom > tempFileSize || pstTo < 0 || pstTo > tempFileSize)
	{
		return -1;
	}

    fseek(tempFile, pstFrom, SEEK_SET);

	char charToAppend;
	int appendedItems = 0;
	while(ftell(tempFile) <= pstTo)
	{	
        appendedItems += fread(&charToAppend, sizeof(char), 1, tempFile);
	    fwrite(&charToAppend, sizeof(char), 1, originalFile);
	}

	fclose(originalFile);

	return appendedItems;
}

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

	// Try opening the file in r+ mode. This won't truncate the
	// file to length zero.
	FILE* fileToWrite = fopen(filename, "r+");
	if(fileToWrite == NULL)
	{
		printf("r+_ doens't work\n");
		// If this doesn't work it is probably becouse the file
		// doesn't exists. Make The file.
		fileToWrite = fopen(filename, "w");
		if(fileToWrite == NULL)
		{
		    return -1;
		}
		fclose(fileToWrite);

		// Open the file again in r+ mode.
		fileToWrite = fopen(filename, "r+");
		if(fileToWrite == NULL)
	    {
	    	return -1;
	    }
	}

	long fileLength = fileSize(filename);
	FILE* tempFile = NULL;
	// Make a back up from the original file (filename).
	if(fileLength > 0)
	{
		printf("There is something in the file\n");
		tempFile = tmpfile();
	    copyFile(filename, tempFile, 0, fileLength);

	    fclose(fileToWrite);

	    // Reopen the file (filename) so it is truncated to size 0.
	    // Later tempFile is concatenated to this file.
	    fileToWrite = fopen(filename, "w+");
		if(fileToWrite == NULL)
	    {
	    	return -1;
	    }
	}

	printf("%p\n", (void*)fileToWrite);
	fwrite(graph->name, sizeof(char), graph->nameLength, fileToWrite);
	printf("name = %s\n", graph->name);

	if(fileLength > 0)
	{
		printf("Appending the tempFile\n");
		fseek(tempFile, 0, SEEK_END);
		long tempFileSize = ftell(tempFile);
		fseek(tempFile, 0, SEEK_SET);

		appendFile(filename, tempFile, 0, tempFileSize);
	}

	fclose(fileToWrite);
	//fclose(tempFile);
	
	return 0;
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
	return 0;
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
    return 0;
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
    return 0;
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
    return 0;
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
    return 0;
}
// Pre: -
// Post: nrGraphsToRead are read from filename into graphArray. 
//       If nrGraphsToRead > graphArraySize only nrGraphsToRead graphs are read.
//       if nrGraphsToRead > number-of-graphs-in-file only number-of-graphs-in-file graphs are read. 
// Return: -1 if filename or graphArray is NULL, or graphArraySize or nrGraphsToRead < 1.
//         Otherwise return number or graphs read.

int readGraphFromHumanReadableFile(char* filename, GRAPH* graph)
{
    return 0;
}
// Pre: -
// Post: The graph properties described in the file formated as above are read 
//       and translated into graph.
// Return: -1 if filename or graph is NULL or filename doens't containthe required (*) properties.
//          0 on succes.