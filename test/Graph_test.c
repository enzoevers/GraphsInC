#include "unity.h"
#include "../src/GraphPathFinding/GraphPathFinding.h"
#include "../src/MakeGraph/MakeGraph.h"
#include "../src/GraphStructure/GraphStructure.h"

// I rather dislike keeping line numbers updated, so I made my own macro to ditch the line number
#define MY_RUN_TEST(func) RUN_TEST(func, 0)

GRAPH graph;

double** matrix_double = NULL;
char** matrix_char = NULL;

void setUp(void)
{
	clearGraph(&graph);
}

void tearDown(void)
{
	matrix_double = NULL;
	matrix_char = NULL;
}

void testUnity(void)
{
	TEST_ASSERT_EQUAL(0, 0);
}

//===================================//
//            MakeGraph.c            //
//===================================//

//===================================makeGraph===================================//



int main (int argc, char * argv[])
{
    UnityBegin();
    
    MY_RUN_TEST(testUnity);
    
    return UnityEnd();
}
