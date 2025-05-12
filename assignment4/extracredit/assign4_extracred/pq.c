/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Oliver Grenon
 * Email: grenono@oregonstate.edu
 * 
 */

#include <stdlib.h>

#include "pq.h"
#include "dynarray.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq {
	struct dynarray* dyn;
};

//A single node in the queue
typedef struct {
	void *data;
	int priority; 
}node;


/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
	//Give the pq space
	struct pq* the_pq = malloc(sizeof(struct pq));
	
	//We will hold neighboring vertices here 
	the_pq->dyn = dynarray_create(); 

	//May need to prevent double malloc if the function is called twice on the same pq
	return the_pq;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue.  That is the responsibility of the caller.
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
	//First free pq->dyn
	dynarray_free(pq->dyn);

	//Then free pointer to pq 
	free(pq);
	
	return;
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
	//If the dyanmic array inside pq is empty, then pq is empty.
	if (pq == NULL) {
		return 0; 
	}
	return dynarray_size(pq->dyn) == 0;
}

//Helper function for insertion; nescessary for Olog(n) 
void perc_up(struct pq* pq, int size) {
	int index = size - 1;
	while (index >> 0) {
		//Check priority value of the index, compare to 	
		int parent_index = index - 1;
		if (parent_index < 0) { //If parent index is -1, the array has 1 element 
			break;
		}
		node* current = dynarray_get(pq->dyn, index);
		node* parent = dynarray_get(pq->dyn, parent_index);

		if (current->priority < parent->priority) {
			dynarray_set(pq->dyn, index, parent); //swapping the parent and child 
			dynarray_set(pq->dyn, parent_index, current);
			index = parent_index;
		}
		else{
			break;
		}
	}
}

//Helper function for removal; nescessary for Olog(n)
void perc_down(struct pq* pq, int index) {
	
	//get size
	int size = dynarray_size(pq->dyn);
	while (1) { //These are the children of pq->dyn[0]
		int left = 2 * index + 1;
		int right = 2 * index + 2;
		int smallest = index; //Always zero in this implementation 

		// Check left
		if (left < size) {
			node* leftNode = dynarray_get(pq->dyn, left);
			node* smallestNode = dynarray_get(pq->dyn, smallest);
			if (leftNode->priority < smallestNode->priority) {
				smallest = left;
			}
		}

		// Check right
		if (right < size) {
			node* rightNode = dynarray_get(pq->dyn, right);
			node* smallestNode = dynarray_get(pq->dyn, smallest);
			if (rightNode->priority < smallestNode->priority) {
				smallest = right;
			}
		}

		// If the smallest of the two isnt first, swap 
		if (smallest != index) {
			node* temp = dynarray_get(pq->dyn, index);
			dynarray_set(pq->dyn, index, dynarray_get(pq->dyn, smallest));
			dynarray_set(pq->dyn, smallest, temp);
			index = smallest;
		}
		else {
			break;
		}
	}
	
}
/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 *	
 * Helpful functions: 
 * void _dynarray_resize(struct dynarray* da, int new_capacity)
 * int dynarray_size(struct dynarray* da)
 * void dynarray_insert(struct dynarray* da, void* val)
 * void dynarray_set(struct dynarray* da, int idx, void* val)
 */
void pq_insert(struct pq* pq, void* value, int priority) {
	/* Element will be inserted to the end
	* As elements are appeneded, they should be sorted by priority 
	* Method:
	* Traverse dyn, getting priority values 
	* Once we find index dyn[i] and dyn[i-1] such that priorityValue[i-1] << priority << priority[i]
	* Place our void* value at i and shift every element after it right
	* ***May be easier to make new dyn***
	*/
	if (pq == NULL) {
		return;
	}
	node *the_node = malloc(sizeof(node));
	
	the_node->data = value; 
	the_node->priority = priority; 

	dynarray_insert(pq->dyn, the_node); //If empty will be placed at dyn[0];

	int size = dynarray_size(pq->dyn);
	perc_up(pq, size); 

	
	return;
}

//Helper to get first min node
node* get_first(struct pq* pq) {
	return dynarray_get(pq->dyn, 0);
}
/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
	if (pq == NULL || dynarray_size(pq->dyn) == 0) {
		return NULL;
	}
	//Return the first value in dyn (lowest priority value)
	return get_first(pq)->data;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
	return get_first(pq)->priority;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
	if (dynarray_size(pq->dyn) == 0) {
		return NULL; 
	}
	node* first_node = get_first(pq); 
	void* value = first_node->data; 
	int size = dynarray_size(pq->dyn);

	if (dynarray_size(pq->dyn) > 1) { //If larger than 1, swap them
		node* last_node = dynarray_get(pq->dyn, size - 1);
		dynarray_set(pq->dyn, 0, last_node);
	}
	dynarray_remove(pq->dyn, size - 1); //O(1)

	free(first_node);
	perc_down(pq, 0); //Olog(n)
	return value;
}

//Results of ./test_pq:
// 0 bytes in use at exit 
// Total Heap Usage: 2,008 bytes 
//No memory leaks possible
//Some values are not being removed. check dynarray usage 
//"first", "removed", and "priority" should always match "expected"