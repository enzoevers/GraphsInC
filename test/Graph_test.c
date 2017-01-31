#include "unity.h"
#include "../src/GraphStructure/GraphStructure.h"
#include "../src/GraphPathFinding/GraphPathFinding.h"
#include "../src/MakeGraph/MakeGraph.h"
#include "../src/File_io/File_io.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>

//#define printMatrix // Uncomment to print the matrices used in the test cases.

// I rather dislike keeping line numbers updated, so I made my own macro to ditch the line number
#define MY_RUN_TEST(func) RUN_TEST(func, 0)

GRAPH graph;
GRAPH weightedGraph;

char graphName[] = "TestGraph";
int weights[] = {8, 1, 1, 4, 2, 9};
char vertices[4] = {'0', '1', '2', '3'};
char edges[][2] = {{'0', '1'}, {'0', '3'}, 
                   {'1', '2'}, 
                   {'2', '0'}, 
                   {'3', '1'}, {'3', '2'}};

int nrEdges = sizeof(edges)/sizeof(edges[0]);
int nrVertices = sizeof(vertices)/sizeof(vertices[0]);

char* fileToWriteGraph = "graphFile.txt";

char* testFile_Filled = "testFile_Filled.txt";
FILE* testFile_Filled_Ptr = NULL;
char testSentence[] = "What a nice day for some testing isn't it.";

void setUp(void)
{
	makeGraph(&graph, graphName, strlen(graphName), edges, nrEdges, vertices, nrVertices);
	makeWeightedGraph(&weightedGraph, graphName, strlen(graphName), edges, weights, nrEdges, vertices, nrVertices);

	testFile_Filled_Ptr = fopen(testFile_Filled, "w");
	if(testFile_Filled_Ptr != NULL)
	{
		fwrite(testSentence, sizeof(char), sizeof(testSentence), testFile_Filled_Ptr);
	}
	fclose(testFile_Filled_Ptr);
}

void tearDown(void)
{
	remove(testFile_Filled);
	//remove(fileToWriteGraph);
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

//===================================//
//              File_io.c            //
//===================================//

//===================================fileSize===================================//
static void test_parameters_fileSize(void)
{
	TEST_ASSERT_EQUAL(-1, fileSize(NULL));
}

static void test_if_fileSize_is_returned(void)
{
	TEST_ASSERT_EQUAL(sizeof(testSentence), fileSize(testFile_Filled));
}

//===================================findInFile===================================//
static void test_parameters_findInFile(void)
{
	TEST_ASSERT_EQUAL(-1, findInFile(NULL, "day", 3));
	TEST_ASSERT_EQUAL(-1, findInFile(testFile_Filled, NULL, 3));
	TEST_ASSERT_EQUAL(-1, findInFile(testFile_Filled, "day", 0));
}

static void test_if_word_can_be_found_in_file(void)
{
	TEST_ASSERT_EQUAL(12, findInFile(testFile_Filled, "day", 3));
}

static void test_if_minus1_is_returned_if_word_does_not_occurre(void)
{
	TEST_ASSERT_EQUAL(-1, findInFile(testFile_Filled, "nope", 4));
}

//===================================write_name===================================//
static void test_parameters_write_name(void)
{
	TEST_ASSERT_EQUAL(-1, write_name(NULL, &graph));
	TEST_ASSERT_EQUAL(-1, write_name(fileToWriteGraph, NULL));
}

static void test_if_name_is_written_in_graph_file(void)
{
	TEST_ASSERT_EQUAL(0, write_name(fileToWriteGraph, &graph));

	//Read the actual name
	char readName[sizeof(graphName)];
	readName[sizeof(graphName)-1] = '\0';

	FILE* checkForName = fopen(fileToWriteGraph, "r");
	if(checkForName != NULL)
	{
		TEST_ASSERT_EQUAL(sizeof(readName)-1, fread(readName, sizeof(char), sizeof(readName)-1, checkForName));
	}
	fclose(checkForName);

	TEST_ASSERT_EQUAL_STRING(readName, graphName);
}


int main (int argc, char * argv[])
{
    UnityBegin();

    //printInfo();
    
    printf("\n//=====makeGraph=====//\n");
    MY_RUN_TEST(test_adjMatrix_of_made_graph);
    MY_RUN_TEST(test_aspMatrix_of_made_graph);
    MY_RUN_TEST(test_predMatrix_of_made_graph);

    printf("\n//=====makeWeightedGraph=====//\n");
    MY_RUN_TEST(test_adjMatrix_of_made_graph_weighted);
    MY_RUN_TEST(test_aspMatrix_of_made_graph_weighted);
    MY_RUN_TEST(test_predMatrix_of_made_graph_weighted);
    
    printf("\n//=====getShortestPath=====//\n");
    MY_RUN_TEST(test_if_shorted_path_is_returned);

    printf("\n//=====fileSize=====//\n");
    MY_RUN_TEST(test_parameters_fileSize);
    MY_RUN_TEST(test_if_fileSize_is_returned);

    printf("\n//=====findInFile=====//\n");
    MY_RUN_TEST(test_parameters_findInFile);
    MY_RUN_TEST(test_if_word_can_be_found_in_file);
    MY_RUN_TEST(test_if_minus1_is_returned_if_word_does_not_occurre);

    printf("\n//=====write_name=====//\n");
    MY_RUN_TEST(test_parameters_write_name);
    MY_RUN_TEST(test_if_name_is_written_in_graph_file);

    return UnityEnd();
}
