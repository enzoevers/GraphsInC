#include "unity.h"
#include "../src/GraphPathFinding/GraphPathFinding.h"
#include "../src/MakeGraph/MakeGraph.h"
#include "../src/GraphStructure/GraphStructure.h"
#include <limits.h>

// I rather dislike keeping line numbers updated, so I made my own macro to ditch the line number
#define MY_RUN_TEST(func) RUN_TEST(func, 0)

GRAPH graph;

char vertices[4] = {'0', '1', '2', '3'};
char edges[][2] = {{'0', '1'}, {'2', '3'}};
int nrEdges = sizeof(edges)/sizeof(edges[0]);
int nrVertices = sizeof(vertices)/sizeof(vertices[0]);

void setUp(void)
{
	clearGraph(&graph);
}

void tearDown(void)
{
}

//===================================//
//            MakeGraph.c            //
//===================================//

//===================================makeGraph===================================//
static void printInfo(void)
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
}

static void test_first_row_of_made_graph(void)
{
	TEST_ASSERT_EQUAL(0, makeGraph(&graph, edges, nrEdges, vertices, nrVertices));

	TEST_ASSERT_EQUAL(0, graph.adjMatrix[0][0]);
	TEST_ASSERT_EQUAL(1, graph.adjMatrix[0][1]);
	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[0][2]);
	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[0][3]);
}

static void test_second_row_of_made_graph(void)
{
	TEST_ASSERT_EQUAL(0, makeGraph(&graph, edges, nrEdges, vertices, nrVertices));

	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[1][0]);
	TEST_ASSERT_EQUAL(0, graph.adjMatrix[1][1]);
	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[1][2]);
	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[1][3]);
}

static void test_third_row_of_made_graph(void)
{
	TEST_ASSERT_EQUAL(0, makeGraph(&graph, edges, nrEdges, vertices, nrVertices));

	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[2][0]);
	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[2][1]);
	TEST_ASSERT_EQUAL(0, graph.adjMatrix[2][2]);
	TEST_ASSERT_EQUAL(1, graph.adjMatrix[2][3]);
}

static void test_fourth_row_of_made_graph(void)
{
	TEST_ASSERT_EQUAL(0, makeGraph(&graph, edges, nrEdges, vertices, nrVertices));

	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[3][0]);
	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[3][1]);
	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[3][2]);
	TEST_ASSERT_EQUAL(0, graph.adjMatrix[3][3]);
}



int main (int argc, char * argv[])
{
    UnityBegin();

    printInfo();
    
    //===================================makeGraph===================================//
    MY_RUN_TEST(test_first_row_of_made_graph);
    MY_RUN_TEST(test_second_row_of_made_graph);
    MY_RUN_TEST(test_third_row_of_made_graph);
    MY_RUN_TEST(test_fourth_row_of_made_graph);
    
    return UnityEnd();
}
