#include "unity.h"
#include "../src/GraphPathFinding/GraphPathFinding.h"
#include "../src/MakeGraph/MakeGraph.h"
#include "../src/GraphStructure/GraphStructure.h"
#include <limits.h>

// I rather dislike keeping line numbers updated, so I made my own macro to ditch the line number
#define MY_RUN_TEST(func) RUN_TEST(func, 0)

GRAPH graph;

char* vertices = NULL; 
char** edges = NULL;

void setUp(void)
{
	char verticesInReal[4] = {'1', '2', '3', '4'};
	char edgesInreal[2][2] = {{'1', '2'}, {'3', '4'}};

	vertices = (char*)verticesInReal;
	edges = (char**)edgesInreal;

	printf("%p\n", edges[0]);
	printf("%p\n", vertices[0]);

	clearGraph(&graph);

	/*int something_matrix[10][20];
	int (*someting)[20] = something_matrix;
	printf("someting %p\n", (void*)someting);
	printf("something_matrix %p\n", (void*)something_matrix);
	printf("someting[0][0] = %p\n", (void*)&someting[1][0]);*/



	/*edges = makeMatrixNxN_char(2, 2);
	printf("pointer to edge = %p\n", (void*)edges);
	printf("pointer to edge[0] = %p\n", edges[0]);
	printf("pointer to edge[0][0] = %p\n", &edges[0][0]);
	printf("pointer to edge[0][1] = %p\n", &edges[0][1]);
	printf("pointer to edge[1] = %p\n", edges[1]);
	printf("pointer to edge[1][0] = %p\n", &edges[1][0]);
	printf("pointer to edge[1][1] = %p\n", &edges[1][1]);
	clearGraph(&graph);
	printf("%c %c\n%c %c\n\n", 
		edges[0][0], edges[0][1], edges[1][0], edges[1][1]);*/
}

void tearDown(void)
{
}

//===================================//
//            MakeGraph.c            //
//===================================//

//===================================makeGraph===================================//
void make_a_graph(void)
{
	TEST_ASSERT_EQUAL(0, makeGraph(&graph, edges, 3, vertices, 4));

	TEST_ASSERT_EQUAL(0, graph.adjMatrix[0][0]);
	TEST_ASSERT_EQUAL(1, graph.adjMatrix[0][1]);
	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[0][2]);
	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[0][3]);

	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[1][0]);
	TEST_ASSERT_EQUAL(0, graph.adjMatrix[1][1]);
	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[1][2]);
	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[1][3]);

	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[2][0]);
	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[2][1]);
	TEST_ASSERT_EQUAL(0, graph.adjMatrix[2][2]);
	TEST_ASSERT_EQUAL(1, graph.adjMatrix[2][3]);

	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[3][0]);
	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[3][1]);
	TEST_ASSERT_EQUAL(INT_MAX, graph.adjMatrix[3][2]);
	TEST_ASSERT_EQUAL(0, graph.adjMatrix[3][3]);
}



int main (int argc, char * argv[])
{
    UnityBegin();
    
    //===================================makeGraph===================================//
    MY_RUN_TEST(make_a_graph);
    
    return UnityEnd();
}
