#ifndef FILE_IO_H
#define FILE_IO_H

#include "../GraphStructure/GraphStructure.h"
#include <stdio.h>

// STRUCTURE OF GRAPH-FILE
// This programs features two possible ways to write a graph to a file:
//     1) Human readable. This also means you can write a text file and give it to the 
//        program to make a usable graph of it. This also contains formatted matrices of the graph.
//        Each file may only contain ONE graph.
//     2) Computer readable. This will just write the GRAPH struct as is. Doing this enables an easy
//        fast way of storing mutiple graphs in one file to later be read back in the program.
//      
//
// The structure of the file (1) is similar to the structure of the graph:
//=========================================================================
// Name: someName;
//
// *nrVertices: (the number of vertices the graph has);                       
// *nrEdges: (the number of edges the graph has);
// *vertices: a,b,c,d,etc.;
// *edges: a-b
//        a-c
//        c-b
//        etc.;
//
// ADJACENCY_MATRIX
// ====================
//    |V0 |V1 |V2 |etc.
// ___|___|___|___|___
// V0 |w00|w01|w02|etc.
// ___|___|___|___|___
// V1 |w10|w11|w12|etc.
// ___|___|___|___|___
// V2 |w20|w21|w22|etc.
// ___|___|___|___|___
// etc|etc|etc|etc|etc.;
//
// FLOYD_WARSHALL
// ====================
//    |V0 |V1 |V2 |etc.
// ___|___|___|___|___
// V0 |w00|w01|w02|etc.
// ___|___|___|___|___
// V1 |w10|w11|w12|etc.
// ___|___|___|___|___
// V2 |w20|w21|w22|etc.
// ___|___|___|___|___
// etc|etc|etc|etc|etc.;
//
// PREDECESSOR_MATRIX
// ====================
//    |V0 |V1 |V2 |etc.
// ___|___|___|___|___
// V0 |p00|p01|p02|etc.
// ___|___|___|___|___
// V1 |p10|p11|p12|etc.
// ___|___|___|___|___
// V2 |p20|p21|p22|etc.
// ___|___|___|___|___
// etc|etc|etc|etc|etc.;
//=========================================================================
//
// properties with * are required to make a graph in the program.
// 
// NOTE: that each column in the matrices have the width of the longest character array in the column.
// Also note that all properties of the graph end with ';'. This is necessary for reading the file in.

int writeCompleteGraphStruct(char* filename, GRAPH* graphArray, int nrGraphInArray);
// Pre: -
// Post: nrGraphInArray graphs of graphArray are appended in filename.
// Return: -1 if filename or graph is NULL or nrGraphInArray < 1.
//          0 in succes. 

int writeCompleteGraphHumanReadable(char* filename, GRAPH* graph);
// Pre: -
// Post: all attributes of graph (see GraphStructure) are written to filename.
//       Whatever was in filename will be overwritten.
// Return: -1 if filename or graph is NULL.
//          0 in succes. 

int write_name(char* filename, GRAPH* graph);
// Pre: -
// Post: The name of the graph is written to the file.
//       - If the filename doesn't exist the file is made and the name is written. 
//       - If filename does exist the name is written to the correct place. (see format above).
//         
// Return: -1 if file couldn't be made or opened or filename or graph is NULL.
//          0 in succes.

int write_vericesAndEdges(char* filename, GRAPH* graph);
// Pre: -
// Post: The vertices and edges of the graph are written to the file.
//       - If the filename doesn't exist the file is made and the vertices and edges are written. 
//       - If filename does exist the vertices and edges are written to the correct place. (see format above).
//         
// Return: -1 if file couldn't be made or opened or filename or graph is NULL.
//          0 in succes.

int write_adjacencyMatrix(char* filename, GRAPH* graph);
// Pre: -
// Post: The adjacency matrix of the graph is written to the file.
//       - If the filename doesn't exist the file is made and the matrix is written. 
//       - If filename does exist the adjacency matrix is written to the correct place. (see format above).
//         
// Return: -1 if file couldn't be made or opened or filename or graph is NULL.
//          0 in succes.

int write_FloydWarshall(char* filename, GRAPH* graph);
// Pre: -
// Post: The Floyd-Warshall matrix of the graph is written to the file.
//       - If the filename doesn't exist the file is made and the matrix is written. 
//       - If filename does exist the Floyd-Warshall matrix is written to the correct place. (see format above).
//         
// Return: -1 if file couldn't be made or opened or filename or graph is NULL.
//          0 in succes.

int write_predecessorMatrix(char* filename, GRAPH* graph);
// Pre: -
// Post: The predecessor matrix of the graph is written to the file.
//       - If the filename doesn't exist the file is made and the matrix is written. 
//       - If filename does exist the predecessor matrix is written to the correct place. (see format above).
//         
// Return: -1 if file couldn't be made or opened or filename or graph is NULL.
//          0 in succes.

int readGraphFromStructFile(char* filename, GRAPH* graphArray, int graphArraySize, int nrGraphsToRead); 
// Pre: -
// Post: nrGraphsToRead are read from filename into graphArray. 
//       If nrGraphsToRead > graphArraySize only nrGraphsToRead graphs are read.
//       if nrGraphsToRead > number-of-graphs-in-file only number-of-graphs-in-file graphs are read. 
// Return: -1 if filename or graphArray is NULL, or graphArraySize or nrGraphsToRead < 1.
//         Otherwise return number or graphs read.

int readGraphFromHumanReadableFile(char* filename, GRAPH* graph);
// Pre: -
// Post: The graph properties described in the file formated as above are read 
//       and translated into graph.
// Return: -1 if filename or graph is NULL or filename doens't containthe required (*) properties.
//          0 on succes.

long fileSize(char* filename);
// Pre: -
// Post: The number of bytes in the file is determined and returned.
// Return: -1 if filePtr is NULL.
//          Otherwise the number of bytes in filePtr.

long findInFile(char* filename, char* stringToFind, int stringLength);
// Pre: -
// Post: The index of the first occurrence of stringToFind in filePtr is determined.
// Return: -1 if filePtr or stringToFind is NULL or stringLength < 1.
//          The index if the first occurrence of stringToFind is returned otherwise.

int copyFile(char* originalFilename, FILE* tempFile, long cpyFrom, long cpyTo);
// Pre: -
// Post:
// Return:

#endif