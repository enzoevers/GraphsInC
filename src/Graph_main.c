#include <stdio.h>
#include "GraphPathFinding/GraphPathFinding.h"
#include "File_io/File_io.h"
#include "MakeGraph/MakeGraph.h"

int weights[] = {8, 1, 1, 4, 2, 9};
char vertices[4] = {'0', '1', '2', '3'};
char edges[][2] = {{'0', '1'}, {'0', '3'}, 
                   {'1', '2'}, 
                   {'2', '0'}, 
                   {'3', '1'}, {'3', '2'}};

int nrEdges = sizeof(edges)/sizeof(edges[0]);
int nrVertices = sizeof(vertices)/sizeof(vertices[0]);

GRAPH graph;

int main(int argc, char* argv[])
{
    makeGraph(&graph, "someName", 6, edges, nrEdges, vertices, nrVertices);
    printf("write_name Result = %d\n", write_name("something.txt", &graph));
    return 0;
}