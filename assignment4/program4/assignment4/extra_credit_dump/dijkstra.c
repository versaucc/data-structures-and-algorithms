/*
 * In this file, you'll implement Dijkstra's algorithm to find the least
 * expensive paths in the graph defined in `airports.dat`.  Don't forget to
 * include your name and @oregonstate.edu email address below.
 *
 * Name: Oliver Grenon
 * Email: grenono@oregonstate.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pq.h"


#define DATA_FILE "airports.dat"

//Assume infinity is just very large
#define MAX 999999

typedef struct edge {
	int dest; //Each edge has at most two destinations, some have 1 - Keep note of this 
	int weight; 
	struct edge* next; 
}edge_;

typedef struct {
	int n_nodes;
	int n_edges; 
	struct edge** adjList; //Adjacency list - array of pointers pointing to head of linked list of edges
}graph_;

//Each priority queue holds the current vertext and its total weight
typedef struct {
	int vertex;
	int sum_weight;
} pq_;

//Create the graph 
graph_* create_graph(int n_nodes) {
	graph_* graph = malloc(sizeof(graph_));
	graph->n_nodes = n_nodes;
	graph->adjList = malloc(n_nodes * sizeof(edge_*));

	// For each node, we need an adjacency list to track edge traversal
	for (int i = 0; i < n_nodes; i++) {
		graph->adjList[i] = NULL;
	}
	return graph;
}

//Each edge will have AT LEAST 1 destination, AT MOST 2 
void add_edge(graph_* graph, int point, int dest, int weight) {
	edge_* new_edge = malloc(sizeof(edge_));
	
	new_edge->dest = dest;
	new_edge->weight = weight;
	new_edge->next = graph->adjList[point];
	graph->adjList[point] = new_edge;
}

//Free the allocated dynamic memory
void free_graph(graph_* graph) {
	for (int i = 0; i < graph->n_nodes; i++) {
		edge_* curr = graph->adjList[i];
		while (curr != NULL) {
			edge_* temp = curr;
			curr = curr->next;
			free(temp);
		}
	}
	free(graph->adjList); // Needed, else floating pointers. 
	free(graph);
	
}

//Dijkstras:
void dijkstra(graph_* graph, int point, const char* airport) {
	int n = graph->n_nodes;
	int* dist = malloc(n * sizeof(int));
	int* prev = malloc(n * sizeof(int));

	for (int i = 0; i < n; i++) {
		dist[i] = MAX;
		prev[i] = -1;
	}
	// Dijkstra's 'base case' at source, we haven't travelled anywhere
	dist[point] = 0;

	// Create a priority queue
	struct pq* pq = pq_create();
	pq_* entry = malloc(sizeof(pq_));

	//Enter source point into priority queue
	entry->vertex = point;
	entry->sum_weight = 0;
	pq_insert(pq, entry, 0);

	// Find vertex with smallest distance
	while (!pq_isempty(pq)) {
		pq_* current = (pq_*)pq_remove_first(pq);
		int u = current->vertex;
		int d = current->sum_weight;
		free(current);

		// Skip if distance is larger
		if (d > dist[u])
			continue;

		// Relax edges
		for (edge_* edge = graph->adjList[u]; edge != NULL; edge = edge->next) {
			int v = edge->dest;
			int weight = edge->weight;
			if (dist[u] != MAX && dist[u] + weight < dist[v]) {
				dist[v] = dist[u] + weight;
				prev[v] = u;
				pq_* new_entry = malloc(sizeof(pq_));

				new_entry->vertex = v;
				new_entry->sum_weight = dist[v];
				pq_insert(pq, new_entry, dist[v]);
			}
		}
	}

	// Print results in clean format
	printf("Dijkstra's from %4s:\n", airport);
	printf("End point       ||  Lowest Cost Path \n");
	for (int i = 0; i < n; i++) {
		printf("%d		||		%d\n", i, dist[i]);
	}

	free(dist);
	free(prev);
	pq_free(pq);
}

//Use many ifs
int airport_to_num(const char* airport) {

	if (!strcmp(airport, "PDX")) {
		return 0;
	}
	else if (!strcmp(airport, "SEA")) {
		return 1; 
	}
	else if (!strcmp(airport, "SFO")) {
		return 2; 
	}
	else if (!strcmp(airport, "LAX")) {
		return 3;
	}
	else if (!strcmp(airport, "MSP")) {
		// printf('MSP'); //USE MSP FOR TEST 
		return 4; 
	}
	else if (!strcmp(airport, "ORD")) {
		return 5; 
	}
	else if (!strcmp(airport, "STL")) {
		return 6; 
	}
	else if (!strcmp(airport, "BOS")) {
		return 7; 
	}
	else if (!strcmp(airport, "PHL")) {
		return 8; 
	}
	else if (!strcmp(airport, "ATL")) {
		return 9; 
	}
	else {
		return 0; //Assume they are flying out of Portland. 
	}


	
	
}

int main(int argc, char const *argv[]) {
	/*
	 * Open file and read the first two int: num of nodes, num of edges
	 */
	int n_nodes, n_edges;
	FILE* file = fopen(DATA_FILE, "r");
	fscanf(file, " %d %d ", &n_nodes, &n_edges);

	//Create blank graph with n nodes initialized
	graph_* graph = create_graph(n_nodes);

	/*
	I hard coded all of the cities, works very well. But read too late
	printf("Enter starting airport:");
	char airport[5];
	scanf("%4s", &airport);
	int point = airport_to_num(airport); //Testing the output of 
	printf("%d", point); //Testing the output of airport to num ...
	*/

	//No hard coded
	int point;
	printf("Enter starting node:");
	scanf("%d", point);

	// Read all edges
	for (int i = 0; i < n_edges; i++) {
		int point, dest, weight;
		if (fscanf(file, " %d %d %d ", &point, &dest, &weight) == 3) {
			add_edge(graph, point, dest, weight);
		}
		// Since the graph is directed, we only add an edge from src to dest.
		// If the edge is bidirectional, the input should include an edge from dest to src.
	}

	// Run Dijkstra's algorithm starting from vertex 0.
	dijkstra(graph, point, airport);

	// Free
	free_graph(graph);

	
	//Close
	fclose(file);

	 
  return 0;
}


/* Dijkstra's Algorithm: 
	Finding the most efficient path of a graph with weighted (non-neg) edges.
		(Also refer to MTH 231 notes for visual examples)
	1. Distance to start node is zero
		-Initialize all other nodes as univisted with (NULL) or infinite distance. 
		-Place all other nodes into a priority queue
		PRIORITY QUEUE:	
			-Each element has a priority (key) value 
			-Lower keys have priority *** we use this 
			-Max heap; higher keys have priority
			
*/