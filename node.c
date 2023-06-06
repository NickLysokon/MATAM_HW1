#include <stdlib.h>
#include <assert.h>
#include "node.h"

struct node_t {
	Element data;
	ElemCopyFunc copyFunc;
	ElemFreeFunc freeFunc;
	Node next;
};

/* =---------------------------------------------------------------------------=

								Node Functions

   =---------------------------------------------------------------------------=
*/

Node nodeCreate(Element data, ElemCopyFunc copy_func, ElemFreeFunc free_func)
{
	if (data == NULL || copy_func == NULL || free_func == NULL) {
		return NULL;
	}

	Node node = malloc(sizeof(*node));
	if (node == NULL) {
		return NULL;
	}

	node->data = copy_func(data);
	if (node->data == NULL) {
		free(node);
		return NULL;
	}

	node->copyFunc = copy_func;
	node->freeFunc = free_func;
	node->next = NULL;
	return node;
}




void nodeRemove(Node node)
{
	if (node == NULL) {
		return;
	}
	node->freeFunc(node->data);
	free(node);
}


void nodeDestroy(Node list)
{
	while (list != NULL) {
		Node next = list->next;
		nodeRemove(list);
		list = next;
	}
}


Node nodeCopy(Node list)
{
	if (list == NULL){
		return NULL;
	}
	Node copy = nodeCreate(list->data, list->copyFunc, list->freeFunc);
	assert(copy != NULL);
	return copy;
}


Node nodeListCopy(Node list)
{
	if (list == NULL){
		return NULL;
	}
	Node copy = nodeCopy(list);
	assert(copy != NULL);
	Node head = copy;
	while (list != NULL){
		copy->next = nodeCopy(list->next);
		if (copy->next == NULL && list->next != NULL){
			nodeDestroy(head);
			return NULL;
		}
		list = list->next;
		copy = copy->next;
	}
	return head;
}





Node nodeGetNext(Node node)
{
	if (node == NULL) {
		return NULL;
	}

	return node->next;
}





void nodeSetNext(Node node, Node next_node)
{
	if (node == NULL) {
		return;
	}
	node->next = next_node;
}




Element nodeGet(Node node)
{
	if (node == NULL) {
		return NULL;
	}
	return node->data;
}