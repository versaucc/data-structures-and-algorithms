Priority queue implementation of Dijkstra's algorithm: 
	To test: 
		-You can fly out of 10 different airports:
		-PDX, SEA, LAX, SFO, MSP, BOS, ALT, STL, ORD, PHL
	1. Compile the code: 
	bash: 
		gcc pq.c pq.h dynarray.c dynarray.h dijkstras.c
	2. Run the program: 
	bash: 
		a.out
	3. You will receive a prompt to enter the airport to fly out of. 
		-The program is case sensitive and space sensitive. 
		-You must enter the exact (3 character string) of the airport you'd like to fly out of
	4. The results of Dijkstra's algorithm will be displayed 
		- Flying to    ||	Least distance

Details: 
	-Used the priority queue implementation 
	-Incorporated an adjacency matrix to store the possible routes
		-This is an array of pointers that point to the head of a linked list storing routes (edges)
	-The struct pq_ stores the current vertex and weighted sum of the path travelled (so far)
	-Time complexity analysis:
		create_graph: O(V) - V is num of nodes
		add_edge: O(Elog(V)) - E is num edges 
			- this is worst case - each edge insertion with binary heap is Olog(V)
		remove edges: O(V+E) 
			-Each removal takes  Olog(V)
		total edge removal: O(V+E)log(V)