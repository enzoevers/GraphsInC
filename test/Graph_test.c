#include "unity.h"
#include "../src/GraphPathFinding/GraphPathFinding.h"
#include "../src/MakeGraph/MakeGraph.h"
#include "../src/GraphStructure/GraphStructure.h"
#include <limits.h>

//#define printMatrix

// I rather dislike keeping line numbers updated, so I made my own macro to ditch the line number
#define MY_RUN_TEST(func) RUN_TEST(func, 0)

GRAPH graph;
GRAPH weightedGraph;

int weights[] = {8, 1, 1, 4, 2, 9};
char vertices[4] = {'0', '1', '2', '3'};
char edges[][2] = {{'0', '1'}, {'0', '3'}, 
                   {'1', '2'}, 
                   {'2', '0'}, 
                   {'3', '1'}, {'3', '2'}};

int nrEdges = sizeof(edges)/sizeof(edges[0]);
int nrVertices = sizeof(vertices)/sizeof(vertices[0]);

void setUp(void)
{
	makeGraph(&graph, edges, nrEdges, vertices, nrVertices);
	makeWeightedGraph(&weightedGraph, edges, weights, nrEdges, vertices, nrVertices);
}

void tearDown(void)
{
}

//===================================//
//            MakeGraph.c            //
//===================================//

//===================================makeGraph===================================//
/*static void printInfo(void)
{
	printf("nrEdges = %d\n", nrEdges);
	printf("nrVertices = %d\n", nrVertices);
	printf("sizeof(edges) = %d\n", sizeof(edges));
	printf("sizeof(edges[0]) = %d\n", sizeof(edges[0]));
	printf("sizeof(edges[0][0]) = %d\n", sizeof(edges[0][0]));
	printf("edges %p\n", (void*)edges);
	printf("(edges+1) %p\n", (void*)(edges+1));
	printf("*edges %p\n", *edges);
	printf("(*edges+1) %p\n", (*edges+1));
	printf("**(edges) %c\n", **(edges));
	printf("**(edges+1) %c\n", **(edges+1))	;
}*/

static void test_adjMatrix_of_made_graph(void)
{
	#ifdef printMatrix
	for(int i = 0; i < nrVertices; i++)
	{
		for (int j = 0; j < nrVertices; j++)
		{
			printf("%d ", graph.adjMatrix[i][j]);
		}
		printf("\n");
	}
	#endif

	TEST_ASSERT_EQUAL(0, graph.adjMatrix[0][0]);
	TEST_ASSERT_EQUAL(1, graph.adjMatrix[0][1]);
	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[0][2]);
	TEST_ASSERT_EQUAL(1, graph.adjMatrix[0][3]);

	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[1][0]);
	TEST_ASSERT_EQUAL(0, graph.adjMatrix[1][1]);
	TEST_ASSERT_EQUAL(1, graph.adjMatrix[1][2]);
	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[1][3]);

	TEST_ASSERT_EQUAL(1, graph.adjMatrix[2][0]);
	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[2][1]);
	TEST_ASSERT_EQUAL(0, graph.adjMatrix[2][2]);
	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[2][3]);

	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[3][0]);
	TEST_ASSERT_EQUAL(1, graph.adjMatrix[3][1]);
	TEST_ASSERT_EQUAL(1, graph.adjMatrix[3][2]);
	TEST_ASSERT_EQUAL(0, graph.adjMatrix[3][3]);
}

static void test_aspMatrix_of_made_graph(void)
{
	#ifdef printMatrix
	for(int i = 0; i < nrVertices; i++)
	{
		for (int j = 0; j < nrVertices; j++)
		{
			printf("%d ", graph.aspMatrix[i][j]);
		}
		printf("\n");
	}
	#endif

	TEST_ASSERT_EQUAL(0, graph.aspMatrix[0][0]);
	TEST_ASSERT_EQUAL(1, graph.aspMatrix[0][1]);
	TEST_ASSERT_EQUAL(2, graph.aspMatrix[0][2]);
	TEST_ASSERT_EQUAL(1, graph.aspMatrix[0][3]);

	TEST_ASSERT_EQUAL(2, graph.aspMatrix[1][0]);
	TEST_ASSERT_EQUAL(0, graph.aspMatrix[1][1]);
	TEST_ASSERT_EQUAL(1, graph.aspMatrix[1][2]);
	TEST_ASSERT_EQUAL(3, graph.aspMatrix[1][3]);

	TEST_ASSERT_EQUAL(1, graph.aspMatrix[2][0]);
	TEST_ASSERT_EQUAL(2, graph.aspMatrix[2][1]);
	TEST_ASSERT_EQUAL(0, graph.aspMatrix[2][2]);
	TEST_ASSERT_EQUAL(2, graph.aspMatrix[2][3]);

	TEST_ASSERT_EQUAL(2, graph.aspMatrix[3][0]);
	TEST_ASSERT_EQUAL(1, graph.aspMatrix[3][1]);
	TEST_ASSERT_EQUAL(1, graph.aspMatrix[3][2]);
	TEST_ASSERT_EQUAL(0, graph.aspMatrix[3][3]);
}

static void test_predMatrix_of_made_graph(void)
{
	#ifdef printMatrix
	for(int i = 0; i < nrVertices; i++)
	{
		for (int j = 0; j < nrVertices; j++)
		{
			printf("%c ", graph.predMatrix[i][j]);
		}
		printf("\n");
	}
	#endif

	TEST_ASSERT_EQUAL('-', graph.predMatrix[0][0]);
	TEST_ASSERT_EQUAL('0', graph.predMatrix[0][1]);
	TEST_ASSERT_EQUAL('1', graph.predMatrix[0][2]);
	TEST_ASSERT_EQUAL('0', graph.predMatrix[0][3]);

	TEST_ASSERT_EQUAL('2', graph.predMatrix[1][0]);
	TEST_ASSERT_EQUAL('-', graph.predMatrix[1][1]);
	TEST_ASSERT_EQUAL('1', graph.predMatrix[1][2]);
	TEST_ASSERT_EQUAL('0', graph.predMatrix[1][3]);

	TEST_ASSERT_EQUAL('2', graph.predMatrix[2][0]);
	TEST_ASSERT_EQUAL('0', graph.predMatrix[2][1]);
	TEST_ASSERT_EQUAL('-', graph.predMatrix[2][2]);
	TEST_ASSERT_EQUAL('0', graph.predMatrix[2][3]);

	TEST_ASSERT_EQUAL('2', graph.predMatrix[3][0]);
	TEST_ASSERT_EQUAL('3', graph.predMatrix[3][1]);
	TEST_ASSERT_EQUAL('3', graph.predMatrix[3][2]);
	TEST_ASSERT_EQUAL('-', graph.predMatrix[3][3]);
}

//===================================makeWeightedGraph===================================//
static void test_adjMatrix_of_made_graph_weighted(void)
{
	#ifdef printMatrix
	for(int i = 0; i < nrVertices; i++)
	{
		for (int j = 0; j < nrVertices; j++)
		{
			printf("%d ", weightedGraph.adjMatrix[i][j]);
		}
		printf("\n");
	}
	#endif

	TEST_ASSERT_EQUAL(0, weightedGraph.adjMatrix[0][0]);
	TEST_ASSERT_EQUAL(8, weightedGraph.adjMatrix[0][1]);
	TEST_ASSERT_EQUAL(INT_MAX, weightedGraph.adjMatrix[0][2]);
	TEST_ASSERT_EQUAL(1, weightedGraph.adjMatrix[0][3]);

	TEST_ASSERT_EQUAL(INT_MAX, weightedGraph.adjMatrix[1][0]);
	TEST_ASSERT_EQUAL(0, weightedGraph.adjMatrix[1][1]);
	TEST_ASSERT_EQUAL(1, weightedGraph.adjMatrix[1][2]);
	TEST_ASSERT_EQUAL(INT_MAX, weightedGraph.adjMatrix[1][3]);

	TEST_ASSERT_EQUAL(4, weightedGraph.adjMatrix[2][0]);
	TEST_ASSERT_EQUAL(INT_MAX, weightedGraph.adjMatrix[2][1]);
	TEST_ASSERT_EQUAL(0, weightedGraph.adjMatrix[2][2]);
	TEST_ASSERT_EQUAL(INT_MAX, weightedGraph.adjMatrix[2][3]);


	TEST_ASSERT_EQUAL(INT_MAX, weightedGraph.adjMatrix[3][0]);
	TEST_ASSERT_EQUAL(2, weightedGraph.adjMatrix[3][1]);
	TEST_ASSERT_EQUAL(9, weightedGraph.adjMatrix[3][2]);
	TEST_ASSERT_EQUAL(0, weightedGraph.adjMatrix[3][3]);
}

static void test_aspMatrix_of_made_graph_weighted(void)
{
	#ifdef printMatrix
	for(int i = 0; i < nrVertices; i++)
	{
		for (int j = 0; j < nrVertices; j++)
		{
			printf("%d ", weightedGraph.aspMatrix[i][j]);
		}
		printf("\n");
	}
	#endif

	TEST_ASSERT_EQUAL(0, weightedGraph.aspMatrix[0][0]);
	TEST_ASSERT_EQUAL(3, weightedGraph.aspMatrix[0][1]);
	TEST_ASSERT_EQUAL(4, weightedGraph.aspMatrix[0][2]);
	TEST_ASSERT_EQUAL(1, weightedGraph.aspMatrix[0][3]);

	TEST_ASSERT_EQUAL(5, weightedGraph.aspMatrix[1][0]);
	TEST_ASSERT_EQUAL(0, weightedGraph.aspMatrix[1][1]);
	TEST_ASSERT_EQUAL(1, weightedGraph.aspMatrix[1][2]);
	TEST_ASSERT_EQUAL(6, weightedGraph.aspMatrix[1][3]);

	TEST_ASSERT_EQUAL(4, weightedGraph.aspMatrix[2][0]);
	TEST_ASSERT_EQUAL(7, weightedGraph.aspMatrix[2][1]);
	TEST_ASSERT_EQUAL(0, weightedGraph.aspMatrix[2][2]);
	TEST_ASSERT_EQUAL(5, weightedGraph.aspMatrix[2][3]);

	TEST_ASSERT_EQUAL(7, weightedGraph.aspMatrix[3][0]);
	TEST_ASSERT_EQUAL(2, weightedGraph.aspMatrix[3][1]);
	TEST_ASSERT_EQUAL(3, weightedGraph.aspMatrix[3][2]);
	TEST_ASSERT_EQUAL(0, weightedGraph.aspMatrix[3][3]);
}

static void test_predMatrix_of_made_graph_weighted(void)
{

	#ifdef printMatrix
	for(int i = 0; i < nrVertices; i++)
	{
		for (int j = 0; j < nrVertices; j++)
		{
			printf("%c ", weightedGraph.predMatrix[i][j]);
		}
		printf("\n");
	}
	#endif

	TEST_ASSERT_EQUAL('-', weightedGraph.predMatrix[0][0]);
	TEST_ASSERT_EQUAL('3', weightedGraph.predMatrix[0][1]);
	TEST_ASSERT_EQUAL('1', weightedGraph.predMatrix[0][2]);
	TEST_ASSERT_EQUAL('0', weightedGraph.predMatrix[0][3]);

	TEST_ASSERT_EQUAL('2', weightedGraph.predMatrix[1][0]);
	TEST_ASSERT_EQUAL('-', weightedGraph.predMatrix[1][1]);
	TEST_ASSERT_EQUAL('1', weightedGraph.predMatrix[1][2]);
	TEST_ASSERT_EQUAL('0', weightedGraph.predMatrix[1][3]);

	TEST_ASSERT_EQUAL('2', weightedGraph.predMatrix[2][0]);
	TEST_ASSERT_EQUAL('3', weightedGraph.predMatrix[2][1]);
	TEST_ASSERT_EQUAL('-', weightedGraph.predMatrix[2][2]);
	TEST_ASSERT_EQUAL('0', weightedGraph.predMatrix[2][3]);

	TEST_ASSERT_EQUAL('2', weightedGraph.predMatrix[3][0]);
	TEST_ASSERT_EQUAL('3', weightedGraph.predMatrix[3][1]);
	TEST_ASSERT_EQUAL('1', weightedGraph.predMatrix[3][2]);
	TEST_ASSERT_EQUAL('-', weightedGraph.predMatrix[3][3]);
}

//===================================//
//        GraphPathFinding.c         //
//===================================//

//===================================getShortestPath===================================//
static void test_if_shorted_path_is_returned(void)
{
	int nrVerticesInShortestPath = 0;
	char* shortestPathString_Ptr = getShortestPath(&weightedGraph, '0', '1', &nrVerticesInShortestPath);

	TEST_ASSERT_EQUAL(3, nrVerticesInShortestPath);
	TEST_ASSERT_NOT_NULL(shortestPathString_Ptr);
	TEST_ASSERT_EQUAL_STRING("0-3-1", shortestPathString_Ptr);
}

int main (int argc, char * argv[])
{
    UnityBegin();

    //printInfo();
    
    //===================================makeGraph===================================//
    MY_RUN_TEST(test_adjMatrix_of_made_graph);
    MY_RUN_TEST(test_aspMatrix_of_made_graph);
    MY_RUN_TEST(test_predMatrix_of_made_graph);

    //===================================makeWeightedGraph===================================//
    MY_RUN_TEST(test_adjMatrix_of_made_graph_weighted);
    MY_RUN_TEST(test_aspMatrix_of_made_graph_weighted);
    MY_RUN_TEST(test_predMatrix_of_made_graph_weighted);
    
    //===================================getShortestPath===================================//
    MY_RUN_TEST(test_if_shorted_path_is_returned);

    return UnityEnd();
}
