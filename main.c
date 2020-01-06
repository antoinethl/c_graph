#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DataStruct.h"

int main(int argc, char **argv)
{
	// GRAPH CREATION
	list_of_node* AdjacencyList = init(NULL);
	printGraph(AdjacencyList);
	// PRIM'S ALGORITHM
	list_of_node* Prim = PrimAlgorithm(AdjacencyList);
	printGraphPrim(Prim);
	getch();
}
