#include "Datastruct.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

			// -=+ PART 1 : QUESTION 1 : +=- //

list_of_node* init(list_of_node* head){ // This function read the CSV and return the Adjacency List
	float value;
	FILE* fichier = fopen("Map2.csv", "r");
	char* c = (char*)malloc(40 * sizeof(char));
	if(fichier == NULL){
		printf("Erreur : le fichier n'existe pas! \n");
	}
	else {
		while (fgets(c, 2, fichier) != NULL) { // Loop to read to CSV and to call method to create the graph
			char* station1 = (char*)malloc(40 * sizeof(char));
			char* station2 = (char*)malloc(40 * sizeof(char));
			fseek(fichier, -1, SEEK_CUR);
			fscanf(fichier,"%[^';'];%[^';'];%f\n", station1, station2, &value); // Pattern to read "string;string;float"
			//printf("%s, %s, %.1f,\n", station1, station2, value);
			head=Process(head, station1, station2, value); // Now we add all of this informations in our graph
		}
		fclose(fichier);
	}
	return head;
}
// Method to create our AdjacencyList with the informations 
list_of_node* Process(list_of_node* head, char* station1, char* station2, float value){
	node* node1=FindNode(head, station1); // FindNode return the node if it already exists or it creates a new one so we don't have multiple node with the same name
	node* node2=FindNode(head, station2);
	if(head == NULL){ // Initialization
		head=create_list_node(node1);
	}
	else{
		AddInList(head, node1); // If already creater, we had node 1 in the list of node
	}
	AddInList(head, node2); // Add node 2 in the list of node
	AddInNodeList(node1, node2, value); // We add the node 2 in the list of connection of node 1
	AddInNodeList(node2, node1, value); // We add the node 1 in the list of connection of node 2
	return head;
}

void AddInNodeList(node* node1, node* node2, float value){ // Function to add a node in the connections list of an other
	if(node1->connection == NULL){
		node1->connection = create_adj_node(node2, value);
	}
	else{
		AddInAdjList(node1->connection, node2, value);
	}
}

void AddInAdjList(adj_list_node* list, node* node, float value){ // Complementary function of the one before (AddInNodeList)
	while(list->next != NULL){
		list=list->next;
	}
	list->next = create_adj_node(node, value);
}


node* FindNode(list_of_node* list, char* name){ // Find a node in the list of node if it exists, or create a new one if it doesn't
	while(list != NULL){
		if(strcmp(list->node->value, name)==0){
			return list->node;
		}
		list=list->next;
	}
	return create_node(name);
}

void AddInList(list_of_node* list, node* _node){ // Add a node in the list of node
	if(!IsInList(list, _node)){
		if(list!=NULL){
			while(list->next != NULL){
				list=list->next;
			}
			list->next = create_list_node(_node);
		}
	}
}

bool IsInList(list_of_node* AdjacencyList, node* node){ // Return true if the node is in the list, otherwise, it's false
	while(AdjacencyList != NULL){
		if(AdjacencyList->node == node){
			return true;
		}
		AdjacencyList=AdjacencyList->next;
	}
	return false;
}

list_of_node* create_list_node(node* n){ // Create a list_of_node structure
	list_of_node* node_list = (list_of_node*)malloc(sizeof(list_of_node));
	node_list->node=n;
	node_list->next=NULL;
	return node_list;
}

node* create_node(char* value){ // Create a node structure
	node* n = (node*)malloc(sizeof(node));
	n->value=value;
	n->connection=NULL;
	n->alreadyVisited=false; // Usefull in Prim's Algorihm
	return n;
}

adj_list_node* create_adj_node(node* n, float value){ // Create a adj_list_node structure
	adj_list_node* aln = (adj_list_node*)malloc(sizeof(adj_list_node));
	aln->node=n;
	aln->weight=value;
	aln->next=NULL;
	aln->usedInPrim=false; // Usefull in Prim's Algorihm
	return aln;
}

void printGraph(list_of_node* list){ // Print the graph
	printf("\n\t-=+ Graph Representation +=-\n\n");
	while(list != NULL){
		printf("%s\n", list->node->value);
		printConnection(list->node->connection);
		list=list->next;
	}
}

void printConnection(adj_list_node* adjlist){ // Complementary method to print the list of connections of each node
	while(adjlist != NULL){
		printf("\t%s -> %.0f\n", adjlist->node->value, adjlist->weight);
		adjlist=adjlist->next;
	}
}

			// -=+ PART 1 : QUESTIONS 2 AND 3 : +=- //

// Main function for the Prim's Algorithm 

list_of_node* PrimAlgorithm (list_of_node* AdjacencyList){
	list_of_node* MST=create_list_node(AdjacencyList->node); // We initialize our list of node with the first node in the AdjacencyList
	AdjacencyList->node->alreadyVisited=true; // It means we have visited it
	while( !FullyVisited(AdjacencyList)){ // While all the node aren't fully visited
		node* node = minDistance(MST); 
		AddInList(MST, node); // We add the node in our new list of node
	}
	return MST;
}

node* minDistance(list_of_node* list){ // Return the node having the connection with the smallest distance
	float smallestDist = 10000;
	float* PsD = &smallestDist; // Pointer to modify the smallestDist in an other method
	adj_list_node* connection = (adj_list_node*)malloc(sizeof(adj_list_node)); // Will store the connection with the smallest distance 
	while(list != NULL){ // Browse the list of node
		connection = minDistanceInAdjList(list->node->connection, PsD, connection); // Call an other function to browser the list of connection for each node
		list = list->next;
	}
	connection->node->alreadyVisited = true; // Now, we have visited our node so we set it as true
	connection->usedInPrim = true; // And we used this connection in our MST so it's now true
	return connection->node;
} 

adj_list_node* minDistanceInAdjList(adj_list_node* adjlist, float* smallestDist, adj_list_node* connection){ // Return the connection with the smallest distance
	while(adjlist != NULL){
		if(adjlist->node != NULL){
			if(!adjlist->node->alreadyVisited && adjlist->weight < *smallestDist){
				*smallestDist = adjlist->weight; // If the distance is smaller, we store this connection
				connection = adjlist;
			}
		}
		adjlist = adjlist->next;
	}
	return connection;
}

bool FullyVisited(list_of_node* adjlist){ // Function that return true if all the nodes have been visited by the Prim's Algorithm
	bool test=true;
	while(adjlist->next != NULL){
		if(adjlist->node->alreadyVisited == false){
			test=false;
		}
		adjlist=adjlist->next;
	}
	return test;
}

void printGraphPrim(list_of_node* list){ // Method to print our MinimumSpanningTree
	printf("\n\n\t-=+ Prim's Algorithm +=-\n\n");
	while(list != NULL){
		printf("%s\n", list->node->value);
		printConnectionPrim(list->node->connection); 
		list=list->next;
	}
}
void printConnectionPrim(adj_list_node* adjlist){
	while(adjlist != NULL){
		if(adjlist->usedInPrim){ // We print only the connections that have been used in Prim's Algorithm
				printf("\t%s -> %.0f\n", adjlist->node->value, adjlist->weight);
		}
		adjlist=adjlist->next;
	}
}