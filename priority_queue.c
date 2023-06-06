#include <stdlib.h>
#include "priority_queue.h"
#include "node.h"
#include "pair.h"

#define NO_SIZE -1

struct PriorityQueue_t {
	Node elements;
	Node iterator;
	CopyPQElement copyElement;
	FreePQElement freeElement;
	EqualPQElements equalElements;
	CopyPQElementPriority copyPriorityElement;
	FreePQElementPriority freePriorityElement;
	ComparePQElementPriorities comparePriorities;
};

/* =---------------------------------------------------------------------------=

							Static Functions Declarations

   =---------------------------------------------------------------------------=
*/


/*
getQueueSpot: Returns the last node that has a higher priority than the current node.

@param queue - The queue to iterate over.
@param node - The element to find its right place in the queue.

@return NULL if one of the arguments is NULL.
		Else will return the last node that
		has a higher priority than the current node.
*/
static Node getQueueSpot(PriorityQueue queue, Node node);

/*
removeElement: Removes a specific element with a specific priority from the queue.

@param queue - The queue to iterate over.
@param element - The specific element to remove.
@param priority - The specific priority to remove.

@return PQ_ELEMENT_DOESNT_EXIST if there isnt a node with the specific element and priority.
		PQ_SUCCESS if the latest node with the specific element and priority was removed.
*/
static PriorityQueueResult removeElement(PriorityQueue queue, PQElement element, PQElementPriority priority);

/* =---------------------------------------------------------------------------=

							Priority Queue Functions

   =---------------------------------------------------------------------------=
*/


PriorityQueue pqCreate(CopyPQElement copy_element, FreePQElement free_element,
						EqualPQElements equal_elements, CopyPQElementPriority copy_priority,
						FreePQElementPriority free_priority, ComparePQElementPriorities compare_priorities)
{
	if (copy_element == NULL || free_element == NULL || equal_elements == NULL ||
		copy_priority == NULL || free_priority == NULL || compare_priorities == NULL) {
		return NULL;
	}

	PriorityQueue queue = malloc(sizeof(*queue));
	if (queue == NULL) {
		return NULL;
	}

	queue->elements = NULL;
	queue->iterator = NULL;
	queue->copyElement = copy_element;
	queue->freeElement = free_element;
	queue->equalElements = equal_elements;
	queue->copyPriorityElement = copy_priority;
	queue->freePriorityElement = free_priority;
	queue->comparePriorities = compare_priorities;

	return queue;
}


void pqDestroy(PriorityQueue queue)
{
	if (queue == NULL) {
		return;
	}
	nodeDestroy(queue->elements);
	free(queue);
}


PriorityQueue pqCopy(PriorityQueue queue)
{
	if (queue == NULL) {
		return NULL;
	}
	PriorityQueue queue_copy = pqCreate(queue->copyElement, queue->freeElement,
										queue->equalElements, queue->copyPriorityElement,
										queue->freePriorityElement, queue->comparePriorities);
	if (queue_copy == NULL) {
		return NULL;
	}

	queue_copy->elements = nodeListCopy(queue->elements);
	if (queue_copy->elements == NULL) {
		free(queue_copy);
		return NULL;
	}
	queue->iterator = NULL;
	queue_copy->iterator = NULL;

	return queue_copy;
}


int pqGetSize(PriorityQueue queue)
{
	if (queue == NULL) {
		return NO_SIZE;
	}

	int count = 0;
	Node ptr = queue->elements;
	while (ptr != NULL) {
		count++;
		ptr = nodeGetNext(ptr);
	}
	return count;
}


bool pqContains(PriorityQueue queue, PQElement element)
{
	if (queue == NULL || element == NULL) {
		return false;
	}

	NODE_FOREACH(Node, ptr, queue->elements) {
		Pair data = nodeGet(ptr);
		if (queue->equalElements(pairFirst(data), element)) {
			return true;
		}
	}

	return false;
}


PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
	if (queue == NULL || element == NULL || priority == NULL) {
		return PQ_NULL_ARGUMENT;
	}

	Pair pair = pairCreate(element, priority, queue->copyElement, queue->copyPriorityElement,
							queue->freeElement, queue->freePriorityElement);
	if (pair == NULL) {
		return PQ_OUT_OF_MEMORY;
	}
	Node node = nodeCreate(pair, (ElemCopyFunc)pairCopy, (ElemFreeFunc)pairDestroy);
	if (node == NULL) {
		pairDestroy(pair);
		return PQ_OUT_OF_MEMORY;
	}
	pairDestroy(pair); //nodeCreate makes a copy of the pair.

	if (queue->elements == NULL) { //Check if this the first time an element is added.
		queue->elements = node;
		return PQ_SUCCESS;
	}
	Node ptr = getQueueSpot(queue, node);
	if (ptr != node) {
	nodeSetNext(node, nodeGetNext(ptr));
	nodeSetNext(ptr, node);
	}

	queue->iterator = NULL;
	return PQ_SUCCESS;
}


PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element,
	PQElementPriority old_priority, PQElementPriority new_priority)
{
	if (queue == NULL || element == NULL || old_priority == NULL || new_priority == NULL) {
		return PQ_NULL_ARGUMENT;
	}
	Node ptr = queue->elements;
	if (ptr == NULL) {
		return PQ_ELEMENT_DOES_NOT_EXISTS;
	}

	queue->iterator = NULL;
	int res = removeElement(queue, element, old_priority);
	if (res != PQ_SUCCESS) {
		return res;
	}
	return pqInsert(queue, element, new_priority);
}


PriorityQueueResult pqRemove(PriorityQueue queue)
{
	if (queue == NULL) {
		return PQ_NULL_ARGUMENT;
	}

	queue->iterator = NULL;
	Node ptr = queue->elements;
	if (ptr == NULL) {
		return PQ_SUCCESS;
	}
	
	queue->elements = nodeGetNext(ptr);
	nodeRemove(ptr);
	return PQ_SUCCESS;
}


PriorityQueueResult pqRemoveElement(PriorityQueue queue, PQElement element)
{
	if (queue == NULL || element == NULL) {
		return PQ_NULL_ARGUMENT;
	}

	queue->iterator = NULL;
	Node ptr = queue->elements;
	if (ptr == NULL) {
		return PQ_ELEMENT_DOES_NOT_EXISTS;
	}

	NODE_FOREACH(Node, ptr, queue->elements) {
		Pair data = nodeGet(ptr);
		if (queue->equalElements(pairFirst(data), element)) {
			return removeElement(queue, element, pairSecond(data));
		}
	}
	return PQ_ELEMENT_DOES_NOT_EXISTS;
}


PQElement pqGetFirst(PriorityQueue queue)
{
	if (queue == NULL) {
		return NULL;
	}
	queue->iterator = queue->elements;
	return pairFirst(nodeGet(queue->iterator));
}

PQElement pqGetNext(PriorityQueue queue)
{
	if (queue == NULL) {
		return NULL;
	}
	queue->iterator = nodeGetNext(queue->iterator);
	return pairFirst(nodeGet(queue->iterator));
}


PriorityQueueResult pqClear(PriorityQueue queue)
{
	if (queue == NULL) {
		return PQ_NULL_ARGUMENT;
	}
	nodeDestroy(queue->elements);
	queue->elements = NULL;
	queue->iterator = NULL;
	return PQ_SUCCESS;
}


/* =---------------------------------------------------------------------------=

								Static Functions

   =---------------------------------------------------------------------------=
*/

static Node getQueueSpot(PriorityQueue queue, Node node)
{
	Node ptr = queue->elements;
	PQElementPriority priority = pairSecond(nodeGet(ptr));

	if (queue->comparePriorities(priority, pairSecond(nodeGet(node))) < 0) {
		nodeSetNext(node, ptr);
		queue->elements = node;
		return node;
	}

	Node prev = ptr;
	ptr = nodeGetNext(ptr);
	while (ptr != NULL) {
		PQElementPriority priority = pairSecond(nodeGet(ptr));
		if (queue->comparePriorities(priority, pairSecond(nodeGet(node))) < 0) {
			return prev;
		}
		prev = ptr;
		ptr = nodeGetNext(ptr);
	}
	return prev; //Returns the last element in the list.
}


static PriorityQueueResult removeElement(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
	Node ptr = queue->elements;
	PQElement curr_element = pairFirst(nodeGet(ptr));
	PQElementPriority curr_priority = pairSecond(nodeGet(ptr));

	if (queue->equalElements(curr_element, element) && !queue->comparePriorities(curr_priority, priority)) {
		queue->elements = nodeGetNext(ptr);
		nodeRemove(ptr);
		return PQ_SUCCESS;
	}

	Node ptr_next = nodeGetNext(ptr);
	while (ptr_next != NULL) {
		PQElement curr_element = pairFirst(nodeGet(ptr_next));
		PQElementPriority curr_priority = pairSecond(nodeGet(ptr_next));

		if (queue->equalElements(curr_element, element) && !queue->comparePriorities(curr_priority, priority)) {
			nodeSetNext(ptr, nodeGetNext(ptr_next));
			nodeRemove(ptr_next);
			return PQ_SUCCESS;
		}
		ptr = ptr_next;
		ptr_next = nodeGetNext(ptr_next);
	}
	return PQ_ELEMENT_DOES_NOT_EXISTS;
}