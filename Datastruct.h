#ifndef Datastruct
#define Datastruct

#include <stdbool.h>

typedef struct _list_of_node {
	struct _node* node;
	struct _list_of_node* next;
} list_of_node;

typedef struct _node {
	char* value;
	struct _adj_list_node* connection;
	bool alreadyVisited; // Used for Prim's Algorithm
} node;

typedef struct _adj_list_node{
	struct _node* node;
	float weight;
	struct _adj_list_node* next;
	bool usedInPrim; // Used for Prim's Algorithm
} adj_list_node;

list_of_node* init(list_of_node* head);
list_of_node* Process(list_of_node* head, char* station1, char* station2, float value);
bool IsInList(list_of_node* AdjacencyList, node* node);
node* FindNode(list_of_node* list, char* name);

void AddInNodeList(node* node1, node* node2, float value);
void AddInAdjList(adj_list_node* list, node* node, float value);
void AddInList(list_of_node* list, node* _node);

void printGraph(list_of_node* list);
void printConnection(adj_list_node* adjlist);
void printGraphPrim(list_of_node* list);
void printConnectionPrim(adj_list_node* adjlist);

node* create_node(char* value);
list_of_node* create_list_node(node* n);
adj_list_node* create_adj_node(node* n, float value);

list_of_node* PrimAlgorithm (list_of_node* AdjacencyList);
bool FullyVisited(list_of_node* adjlist);
node* minDistance(list_of_node* list);
adj_list_node* minDistanceInAdjList(adj_list_node* adjlist, float* smallestDist, adj_list_node* connection);

void getch();

#endif


