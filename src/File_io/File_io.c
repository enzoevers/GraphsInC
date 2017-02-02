#include "File_io.h"
#include <string.h>

#include <stdio.h>

static long fileSizeFromFilePointer(FILE* filePtr)
{
    fseek(filePtr, 0, SEEK_END);
    long filePtrSize = ftell(filePtr);
    fseek(filePtr, 0, SEEK_SET);

    return filePtrSize;
}

static void writeTempFileBeforeProperty(char* fileToWriteTo, char* tempFileBeforeProperty_name, FILE* tempFileBeforeProperty)
{
    if(tempFileBeforeProperty != NULL)
    {
        long tempFileSize = fileSizeFromFilePointer(tempFileBeforeProperty);
        appendFile(fileToWriteTo, tempFileBeforeProperty, 0, tempFileSize-1, 1);

        fclose(tempFileBeforeProperty);
    }
    remove(tempFileBeforeProperty_name);
}

static void writeTempFileAfterProperty(char* fileToWriteTo, char* tempFileAfterProperty_name, FILE* tempFileAfterProperty)
{
    if(tempFileAfterProperty != NULL)
    {
        long tempFileSize = fileSizeFromFilePointer(tempFileAfterProperty);
        appendFile(fileToWriteTo, tempFileAfterProperty, 0, tempFileSize-1, 0);

        fclose(tempFileAfterProperty);
    }
    remove(tempFileAfterProperty_name);
}

static int makeTempFiles(int startIndex, int endingIndex, int propertyIndex, char* filename, int fileLength,
    FILE** tempFileBeforeProperty, char* tempFileBeforeProperty_name, FILE** tempFileAfterProperty, char* tempFileAfterProperty_name)
{
    if(startIndex >= 0)
    {
        endingIndex = (endingIndex == 0) ? 1 : 0; // Take the extra \n after "Name: <name>;\n\n" into account.
        endingIndex += findInFile(filename, ";", 1, startIndex); // Assign the index.
    }

    if(propertyIndex > 0)
    {   
        *tempFileBeforeProperty = fopen(tempFileBeforeProperty_name, "w+");
        if(*tempFileBeforeProperty == NULL)
        {
            return -1;
        }

        // Copy the file into a temporary file made with tmpfile().
        // -1 is there to copy everyting up to the index of the first characters of nrEdges but not cupy the 'n'.
        copyFile(filename, *tempFileBeforeProperty, 0, propertyIndex-1);
    }

    if(endingIndex+1 != fileLength-1)
    {
        *tempFileAfterProperty = fopen(tempFileAfterProperty_name, "w+");
        if(*tempFileAfterProperty == NULL)
        {
            return -1;
        }

        // Copy the file into a temporary file made with tmpfile().
        // +2 is there to not copy the ; and \n.
        // -1 is because fileLength is the number of bytes but the index must be given.
        copyFile(filename, *tempFileAfterProperty, endingIndex+2, fileLength-1);
    }   

    return 0;
}

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
long findInFile(char* filename, char* stringToFind, int stringLength, long startIndex)
{
	if(filename == NULL || stringToFind == NULL || stringLength < 1 || startIndex < 0)
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
	long index = startIndex;
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

    fclose(filePtr);
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

	char charToCopy = ' ';
	int readItems = 0;
	while(ftell(originalFile) <= cpyTo && feof(originalFile) <= 0 && charToCopy != '\0')
	{
		readItems += fread(&charToCopy, sizeof(char), 1, originalFile);
	    fwrite(&charToCopy, sizeof(char), 1, tempFile);
	}

	clearerr(originalFile);

	fclose(originalFile);

	return readItems;
}
		
int appendFile(char* originalFilename, FILE* tempFile, long pstFrom, long pstTo, int clean)
{
	if(originalFilename == NULL || tempFile == NULL)
	{
		return -1;
	}

	FILE* originalFile = (clean == 0) ? fopen(originalFilename, "a") : fopen(originalFilename, "w");
	if(originalFile == NULL)
	{
		return -1;
	}

    long tempFileSize = fileSizeFromFilePointer(tempFile);

	if(pstFrom < 0 || pstFrom > tempFileSize || pstTo < 0 || pstTo > tempFileSize)
	{
		return -1;
	}

    fseek(tempFile, pstFrom, SEEK_SET);

	char charToAppend = ' ';
	int appendedItems = 0;
	while(ftell(tempFile) <= pstTo && feof(tempFile) <= 0 && charToAppend != '\0')
	{	
		appendedItems += fread(&charToAppend, sizeof(char), 1, tempFile);
	    fwrite(&charToAppend, sizeof(char), 1, originalFile);     
	}

	clearerr(tempFile);

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
        result = write_nrEdges(filename, graph);
    }

    if(result == 0)
    {
        result = write_vertices(filename, graph);
    }

	if(result == 0)
	{
		result = write_edges(filename, graph);
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

	long fileLength = fileSize(filename);

    FILE* fileToWrite = NULL;
	FILE* tempFile = NULL;
    long startIndexOfName = 0;
    long endingOfNameInformation = -3;

	// Make a back up from the original file (filename).
	if(fileLength > 0)
	{
		tempFile = tmpfile();
		if(tempFile == NULL)
		{
			return -1;
		}

        startIndexOfName = findInFile(filename, "Name:", 5, 0);
        if(startIndexOfName >= 0)
        {
            endingOfNameInformation = findInFile(filename, ";", 1, startIndexOfName);
        }

        // Copy the file into a temporary file made with tmpfile().
        // +3 is there to not copy the ; and \n\n. Als of name isn't isn't in the file +3 will set the start index to 0.
        // -1 is because fileLength is the number of bytes but the index must be given.
	    copyFile(filename, tempFile, endingOfNameInformation+3, fileLength-1);
	}


    // Clear the file and write the (new) name.
    fileToWrite = fopen(filename, "w");
	fwrite("Name: ", sizeof(char), 6, fileToWrite);
	fwrite(graph->name, sizeof(char), graph->nameLength, fileToWrite);
	fwrite(";\n\n", sizeof(char), 3, fileToWrite);
	fclose(fileToWrite);

	// Was a tempFile made?
	if(tempFile != NULL)
	{
        long tempFileSize = fileSizeFromFilePointer(tempFile);

		appendFile(filename, tempFile, 0, tempFileSize-1, 0);
		fclose(tempFile);
	}

	return 0;
}

int write_nrVertices(char* filename, GRAPH* graph)
{
    if(filename == NULL || graph == NULL)
    {
        return -1;
    }

    long fileLength = fileSize(filename);

    FILE* fileToWrite = NULL;

    char* tempFileBeforeProperty_name = "tempFileBeforeProperty.txt";
    char* tempFileAfterProperty_name = "tempFileAfterProperty.txt";
    FILE* tempFileBeforeProperty = NULL;
    FILE* tempFileAfterProperty = NULL;
    long propertyIndex = -1;
    long startIndex = -1;
    long endingIndex = -2;

    if(fileLength > 0)
    {
        startIndex = findInFile(filename, "nrVertices:", 11, 0);
        propertyIndex = startIndex;
        if(startIndex < 0)
        {
            startIndex = findInFile(filename, "Name:", 5, 0);
            endingIndex = 0; //Flag for next if statement. 
        }
        
        int makeTempFiles_result = makeTempFiles(startIndex, endingIndex, propertyIndex, filename, fileLength,
        &tempFileBeforeProperty, tempFileBeforeProperty_name, &tempFileAfterProperty, tempFileAfterProperty_name);
        if(makeTempFiles_result == -1)
        {
            return -1;
        }
    }

    writeTempFileBeforeProperty(filename, tempFileBeforeProperty_name, tempFileBeforeProperty);

    // Write the nrEdges file.
    fileToWrite = fopen(filename, "a");
    if(fileToWrite == NULL)
    {
        return -1;
    }

    char nrVerticesString[5]; // Max number of vertices is 9999.
    int nrVerticesStringLength = snprintf(nrVerticesString, sizeof(nrVerticesString), "%d", graph->nrVertices);

    // If there would be more than 9999 vertices MAX< is written instead.
    if(nrVerticesStringLength >= sizeof(nrVerticesString))
    {
        nrVerticesStringLength = snprintf(nrVerticesString, sizeof(nrVerticesString), "MAX<");
    }

    fwrite("nrVertices: ", sizeof(char), 12, fileToWrite);
    fwrite(nrVerticesString, sizeof(char), nrVerticesStringLength, fileToWrite);
    fwrite(";\n", sizeof(char), 2, fileToWrite);

    fclose(fileToWrite);

    writeTempFileAfterProperty(filename, tempFileAfterProperty_name, tempFileAfterProperty);

    return 0;
}
// Pre: -
// Post: The nrVertices of the graph are written to the file.
//       - If the filename doesn't exist the file is made and the nrVertices are written. 
//       - If filename does exist the nrVertices are written to the correct place. (see format above).
//         
// Return: -1 if file couldn't be made or opened or filename or graph is NULL.
//          0 in succes.

int write_nrEdges(char* filename, GRAPH* graph)
{
    if(filename == NULL || graph == NULL)
    {
        return -1;
    }

    long fileLength = fileSize(filename);

    FILE* fileToWrite = NULL;

    char* tempFileBeforeProperty_name = "tempFileBeforeProperty.txt";
    char* tempFileAfterProperty_name = "tempFileAfterProperty.txt";
    FILE* tempFileBeforeProperty = NULL;
    FILE* tempFileAfterProperty = NULL;
    long propertyIndex = -1;
    long startIndex = -1;
    long endingIndex = -2;

    if(fileLength > 0)
    {
        startIndex = findInFile(filename, "nrEdges:", 8, 0);
        propertyIndex = startIndex;
        if(startIndex < 0)
        {
            startIndex = findInFile(filename, "nrVertices:", 11, 0);
            if(startIndex < 0)
            {
                startIndex = findInFile(filename, "Name:", 5, 0);
                endingIndex = 0; //Flag for next if statement. 
            }
        }
        
        int makeTempFiles_result = makeTempFiles(startIndex, endingIndex, propertyIndex, filename, fileLength,
        &tempFileBeforeProperty, tempFileBeforeProperty_name, &tempFileAfterProperty, tempFileAfterProperty_name);
        if(makeTempFiles_result == -1)
        {
            return -1;
        }
    }

    writeTempFileBeforeProperty(filename, tempFileBeforeProperty_name, tempFileBeforeProperty);

    // Write the nrEdges file.
    fileToWrite = fopen(filename, "a");
    if(fileToWrite == NULL)
    {
        return -1;
    }

    char nrEdgesString[5]; // Max number of edges is 9999.
    int nrEdgesStringLength = snprintf(nrEdgesString, sizeof(nrEdgesString), "%d", graph->nrEdges);

    // If there would be more than 9999 edges MAX< is written instead.
    if(nrEdgesStringLength >= sizeof(nrEdgesString))
    {
        nrEdgesStringLength = snprintf(nrEdgesString, sizeof(nrEdgesString), "MAX<");
    }

    fwrite("nrEdges: ", sizeof(char), 9, fileToWrite);
    fwrite(nrEdgesString, sizeof(char), nrEdgesStringLength, fileToWrite);
    fwrite(";\n", sizeof(char), 2, fileToWrite);

    fclose(fileToWrite);

    writeTempFileAfterProperty(filename, tempFileAfterProperty_name, tempFileAfterProperty);

    return 0;
}
// Pre: -
// Post: The nrEdges of the graph are written to the file.
//       - If the filename doesn't exist the file is made and the nrEdges are written. 
//       - If filename does exist the nrEdges are written to the correct place. (see format above).
//         
// Return: -1 if file couldn't be made or opened or filename or graph is NULL.
//          0 in succes.

int write_vertices(char* filename, GRAPH* graph)
{
	return 0;
}
// Pre: -
// Post: The vertices of the graph are written to the file.
//       - If the filename doesn't exist the file is made and the vertices are written. 
//       - If filename does exist the vertices are written to the correct place. (see format above).
//         
// Return: -1 if file couldn't be made or opened or filename or graph is NULL.
//          0 in succes.

int write_edges(char* filename, GRAPH* graph)
{
    return 0;
}
// Pre: -
// Post: The edges of the graph are written to the file.
//       - If the filename doesn't exist the file is made and the edges are written. 
//       - If filename does exist the edges are written to the correct place. (see format above).
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