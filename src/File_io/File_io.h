#ifndef FILE_IO_H
#define FILE_IO_H

#include "../GraphStructure/GraphStructure.h"

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

int writeCompleteGraph(char* filename, GRAPH* graph, char mode);
// Pre: -
// Post: graph is written to filename using mode.
//       modes: 'h' (human readable).
//              'c' (computer readable).

int write_FloydWarshall(char* filename, GRAPH* graph)
// Pre: -
// Post: The Floyd-Warshall matrix if the graph is written to the file.
//       - If the filename doesn't exist the file is made and the matrix is written. 
//       - If filename does exist the Floyd-Warshall matrix is written to the correct place. (see format above).
//         
// Return: -1 if file couldn't be made or opened, filename is NULL or graph is NULL.
//          0 in succes.

int 
#endif