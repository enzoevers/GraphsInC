
#include "../src/AllInclude.h"
#include "unity.h"

// I rather dislike keeping line numbers updated, so I made my own macro to ditch the line number
#define MY_RUN_TEST(func) RUN_TEST(func, 0)

GRAPH graph;

int nrVertices = 0;
int nrEdges = 0;
char* vertices = NULL;
int* w = NULL; 
int** edges = NULL; 
double** adjMatrix = NULL;  
double** aspMatrix = NULL;
char** predMatrix = NULL;

void setUp(void)
{
	graph = {nrVertices, nrEdges, vertices, w, edges, adjMatrix, aspMatrix, predMatrix};
}

void tearDown(void)
{
    graph = NULL;
}

void test_if_all_pairs_shortest_path_is_calculated(void)
{
	edges = makeMatrixNxN(2, 2);
	edges[0] = {0, 3};
	edges[1] = {1, 2};

	graph = {4, 2, {'0', '1', '2', '3'}, w, edges, adjMatrix, aspMatrix, predMatrix};

	fillAdjacencyMatrix(graph);
	TEST_ASSERT_EQUAL(graph.adjMatrix[0][3], 1);
	TEST_ASSERT_EQUAL(graph.adjMatrix[1][2], 1);
	TEST_ASSERT_EQUAL(graph.adjMatrix[0][0], INFINITY);
}


int main (int argc, char * argv[])
{
    UnityBegin();
    
    MY_RUN_TEST(test1);
    
    return UnityEnd();
}
