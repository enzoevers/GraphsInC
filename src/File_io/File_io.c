#include "File_io.h"
#include <string.h>
#include <limits.h> // For writing the matrices

#include <stdio.h> // !!FOR DEBUG ONLY!!

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

static void writeMatrixRowSeperator(GRAPH* graph, FILE* fileToWrite, int end)
{
    char rowSeparator[] = "_\t";

    for(int i = 0; i < graph->nrVertices; i++)
    {
        fwrite(rowSeparator, sizeof(char), strlen(rowSeparator), fileToWrite);
        if(i == graph->nrVertices-1)
        {
            fwrite(end ? ";" : "\n", sizeof(char), 1, fileToWrite);
        }
    } 
}

// Return new dst length
static int convertIntToString(char* dst, int dstLength, int weight)
{
    int weightStringLength = 0;

    if(weight == INT_MAX)
    {
        dst[0] = '-';
        dst[1] = '\0';
        weightStringLength = 1;
    }
    else
    {
        weightStringLength = snprintf(dst, dstLength, "%d", weight);

        // If there would be more than 9999 vertices '<' is written instead.
        if(weightStringLength >= dstLength)
        {
            weightStringLength = snprintf(dst, dstLength, "<");
        }
    }

    return weightStringLength;
}

static void writeMatrix(GRAPH* graph, FILE* fileToWrite, char mode)
{
    for(int i = -1; i < graph->nrVertices; i++)
    {
        for(int j = 0; j < graph->nrVertices; j++)
        {
            fwrite("|", sizeof(char), 1, fileToWrite);

            if(i == -1)
            {
                fwrite(graph->vertices+j, sizeof(char), 1, fileToWrite);
                fwrite("\t", sizeof(char), 1, fileToWrite);
                if(j == graph->nrVertices-1)
                {
                    fwrite("\n__\t", sizeof(char), 4, fileToWrite);
                    writeMatrixRowSeperator(graph, fileToWrite, 0);
                }
            }
            else
            {
                char weightString[5]; // Max weight is 9999.
                int weightStringLength = 0;

                switch(mode)
                {
                    case '1':
                        weightStringLength = convertIntToString(weightString, sizeof(weightString), graph->adjMatrix[i][j]);
                        break;

                    case '2':
                        weightStringLength = convertIntToString(weightString, sizeof(weightString), graph->aspMatrix[i][j]);
                        break;

                    case '3':
                        weightStringLength = 1;
                        weightString[0] = graph->predMatrix[i][j];
                        break;
                }

                fwrite(weightString, sizeof(char), weightStringLength, fileToWrite);
                fwrite("\t", sizeof(char), 1, fileToWrite);

                if(j == graph->nrVertices-1)
                {
                    fwrite("\n__\t", sizeof(char), 4, fileToWrite);
                    writeMatrixRowSeperator(graph, fileToWrite, (i == graph->nrVertices-1) ? 1 : 0);
                }
            }
        }
        if(i < graph->nrVertices-1)
        {
            fwrite(graph->vertices+(i+1), sizeof(char), 1, fileToWrite);
            fwrite("\t", sizeof(char), 1, fileToWrite);
        }
    }
    
    fwrite("\n\n", sizeof(char), 2, fileToWrite);
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

int write_vertices(char* filename, GRAPH* graph)
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
        startIndex = findInFile(filename, "vertices:", 9, 0);
        propertyIndex = startIndex;
        if(startIndex < 0)
        {
            startIndex = findInFile(filename, "nrEdges:", 8, 0);
            if(startIndex < 0)
            {
                startIndex = findInFile(filename, "nrVertices:", 11, 0);
                if(startIndex < 0)
                {
                    startIndex = findInFile(filename, "Name:", 5, 0);
                    endingIndex = 0; //Flag for next if statement. 
                }
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

    fwrite("vertices: ", sizeof(char), 10, fileToWrite);  

    for(int i = 0; i < graph->nrVertices; i++)
    {
        fwrite(graph->vertices+i, sizeof(char), 1, fileToWrite);
        if(i < graph->nrVertices-1)
        {
            fwrite(", ", sizeof(char), 2, fileToWrite);
        }
    }
    fwrite(";\n", sizeof(char), 2, fileToWrite);

    fclose(fileToWrite);

    writeTempFileAfterProperty(filename, tempFileAfterProperty_name, tempFileAfterProperty);

    return 0;
}

int write_edges(char* filename, GRAPH* graph)
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
        startIndex = findInFile(filename, "edges:", 6, 0);
        propertyIndex = startIndex;
        if(startIndex < 0)
        {
            startIndex = findInFile(filename, "vertices:", 9, 0);
            if(startIndex < 0)
            {
                startIndex = findInFile(filename, "nrEdges:", 8, 0);
                if(startIndex < 0)
                {
                    startIndex = findInFile(filename, "nrVertices:", 11, 0);
                    if(startIndex < 0)
                    {
                        startIndex = findInFile(filename, "Name:", 5, 0);
                        endingIndex = 0; //Flag for next if statement. 
                    }
                }
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

    fwrite("edges: ", sizeof(char), 7, fileToWrite);  

    for(int i = 0; i < graph->nrEdges; i++)
    {
        if(i > 0)
        {
            fwrite("\t\t", sizeof(char), 2, fileToWrite);  
        }
        for(int j = 0; j < 2; j++)
        {
            fwrite(&(graph->edges[i][j]), sizeof(char), 1, fileToWrite); 
            if(j == 0)
            {
                fwrite("-", sizeof(char), 1, fileToWrite);
            } 
            else if(j == 1 && i < graph->nrEdges-1)
            {
              fwrite("\n", sizeof(char), 1, fileToWrite);  
            }
        }
    }
    fwrite(";\n\n", sizeof(char), 3, fileToWrite);

    fclose(fileToWrite);

    writeTempFileAfterProperty(filename, tempFileAfterProperty_name, tempFileAfterProperty);

    return 0;
}

int write_adjacencyMatrix(char* filename, GRAPH* graph)
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
        startIndex = findInFile(filename, "ADJACENCY_MATRIX", 16, 0);
        propertyIndex = startIndex;
        if(startIndex < 0)
        {
            startIndex = findInFile(filename, "edges:", 6, 0);
            if(startIndex < 0)
            {
                startIndex = findInFile(filename, "vertices:", 9, 0);
                if(startIndex < 0)
                {
                    startIndex = findInFile(filename, "nrEdges:", 8, 0);
                    if(startIndex < 0)
                    {
                        startIndex = findInFile(filename, "nrVertices:", 11, 0);
                        if(startIndex < 0)
                        {
                            startIndex = findInFile(filename, "Name:", 5, 0);
                            endingIndex = 0; //Flag for empty line separator.
                        }
                    }
                }
            }
            else
            {
                endingIndex = 0; //Flag for empty line separator.
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

    fwrite("ADJACENCY_MATRIX\n====================\n\t", sizeof(char), 39, fileToWrite);  
    writeMatrix(graph, fileToWrite, '1');
    fclose(fileToWrite);

    writeTempFileAfterProperty(filename, tempFileAfterProperty_name, tempFileAfterProperty);

    return 0;
}

int write_FloydWarshall(char* filename, GRAPH* graph)
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
        startIndex = findInFile(filename, "FLOYD_WARSHALL", 14, 0);
        propertyIndex = startIndex;
        if(startIndex < 0)
        {
            startIndex = findInFile(filename, "ADJACENCY_MATRIX", 16, 0);
            if(startIndex < 0)
            {
                startIndex = findInFile(filename, "edges:", 6, 0);
                if(startIndex < 0)
                {
                    startIndex = findInFile(filename, "vertices:", 9, 0);
                    if(startIndex < 0)
                    {
                        startIndex = findInFile(filename, "nrEdges:", 8, 0);
                        if(startIndex < 0)
                        {
                            startIndex = findInFile(filename, "nrVertices:", 11, 0);
                            if(startIndex < 0)
                            {
                                startIndex = findInFile(filename, "Name:", 5, 0);
                                endingIndex = 0; //Flag for empty line separator.
                            }
                        }
                    }
                }
                else
                {
                    endingIndex = 0; //Flag for empty line separator.
                }
            }
            else
            {
                endingIndex = 0; //Flag for empty line separator.
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

    fwrite("FLOYD_WARSHALL\n====================\n\t", sizeof(char), 37, fileToWrite);  
    writeMatrix(graph, fileToWrite, '2');
    fclose(fileToWrite);

    writeTempFileAfterProperty(filename, tempFileAfterProperty_name, tempFileAfterProperty);

    return 0;
}

int write_predecessorMatrix(char* filename, GRAPH* graph)
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
        startIndex = findInFile(filename, "PREDECESSOR_MATRIX", 18, 0);
        propertyIndex = startIndex;
        if(startIndex < 0)
        {
            startIndex = findInFile(filename, "FLOYD_WARSHALL", 14, 0);
            if(startIndex < 0)
            {
                startIndex = findInFile(filename, "ADJACENCY_MATRIX", 16, 0);
                if(startIndex < 0)
                {
                    startIndex = findInFile(filename, "edges:", 6, 0);
                    if(startIndex < 0)
                    {
                        startIndex = findInFile(filename, "vertices:", 9, 0);
                        if(startIndex < 0)
                        {
                            startIndex = findInFile(filename, "nrEdges:", 8, 0);
                            if(startIndex < 0)
                            {
                                startIndex = findInFile(filename, "nrVertices:", 11, 0);
                                if(startIndex < 0)
                                {
                                    startIndex = findInFile(filename, "Name:", 5, 0);
                                    endingIndex = 0; //Flag for empty line separator.
                                }
                            }
                        }
                    }
                    else
                    {
                        endingIndex = 0; //Flag for empty line separator.
                    }
                }
                else
                {
                    endingIndex = 0; //Flag for empty line separator.
                }
            }
            else
            {
                endingIndex = 0; //Flag for empty line separator.
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

    fwrite("PREDECESSOR_MATRIX\n====================\n\t", sizeof(char), 41, fileToWrite);  
    writeMatrix(graph, fileToWrite, '3');
    fclose(fileToWrite);

    writeTempFileAfterProperty(filename, tempFileAfterProperty_name, tempFileAfterProperty);

    return 0;
}

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