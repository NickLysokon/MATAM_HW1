#ifndef _NODE_H
#define _NODE_H

#include "pair.h"

/** Type for defining a node */

typedef struct node_t* Node;

/** Type used for returning error codes from node functions */

typedef enum {
    NODE_SUCCESS,
    NODE_OUT_OF_MEMORY,
    NODE_NULL_ARGUMENT,
    NODE_ERROR
} NodeResult;

/*
nodeCreate: Creates a new single node.

@param data - The element to store in the node.
@param copy_func - Function for copying elements.
@param free_func - Function for deallocating elements.

@return NULL if one of the supplied arguments is NULL or if a memory allocation failed.
        Else, it will return a node which stores a copy of data, and the next node points to NULL.
*/
Node nodeCreate(Element data, ElemCopyFunc copy_func, ElemFreeFunc free_func);

/*
nodeAdd: Adds a new node at the end of a node list.

@param list - The node list to add the new node.
@param data - The element to store in the new node.

@return NODE_NULL_ARGUMENT - If one of the supplied arguments is NULL
        NODE_OUT_OF_MEMORY - If the node creation failed.
        NODE_SUCCESS - If the new node is successfully added to the end of the list.
*/
NodeResult nodeAdd(Node list, Element data);

/*
nodeRemove: Deallocates a single node.

@param node - The node to deallocate.
*/
void nodeRemove(Node node);

/*
nodeDestroy: Deallocates the node list and all of its elements.

@param list - The node list to deallocate.
*/
void nodeDestroy(Node node);

/*
nodeCopy: Returns a copy of a single node.

@param list - The Node to copy.

@return NULL if the Node is NULL or if copying the Node fails.
        Else, a copy of the Node.
*/
Node nodeCopy(Node list);

/*
nodeListCopy: Returns a copy of a node list.

@param list - The Node list to copy.

@return NULL if the list is NULL or 
*/
Node nodeListCopy(Node list);

/*
nodeGetDate: Returns the data stored in the node.

@param node - The node that stores the data.

@return NULL if the node is NULL or if the copy function of the element fails.
        Else, a copy of the data stored in the node.
*/
Element nodeGetData(Node node);

/*
nodeGetNext: Returns the next node.

@param node - The node that stores the pointer to the next node.

@return NULL if the node is NULL.
        Else, the next node the current node points to.
*/
Node nodeGetNext(Node node);

/*
nodeGetCopyFunc: Returns the copy function for elements stored in the node.

@param node - The node that stores the element copy function.

@return NULL if the node is null.
        Else, a pointer to the element copy function.
*/
ElemCopyFunc nodeGetCopyFunc(Node node);

/*
nodeGetFreeFunc: Returns the free function of elements stored in the node.

@param node - The node that stores the element free function.

@return NULL if the node is null.
        Else, a pointer to the element free function
*/
ElemFreeFunc nodeGetFreeFunc(Node node);

/*
nodeSetData: Changes the node's data.

@param node - The node to change its data.
@param new_data - The element to store in the node.

@return NODE_NULL_ARGUMENT if one of the arguments is NULL.
        NODE_OUT_OF_MEMORY if a memory allocation fails.
        NODE_SUCCESS if the data has been changed successfully.
*/
NodeResult nodeSetData(Node node, Element new_data);

/*
nodeSetNext: Changes the next node.

@param node - The node to change its next node.
@param next_node - The node that will be pointed to next.
*/
void nodeSetNext(Node node, Node next_node);

/*
nodeGet: Returns the data stored in the node.
         The returned element is not copied, and therefore it is the user's
         responsibility not to modify it in any way.

@param node - The node to extract the data from.

@return NULL if the node is NULL.
        Else, returns the data stored in the node (as a pointer, and not a copy)
*/
Element nodeGet(Node node);


/*
* Macro for iterating over a node list.
* Declares a new iterator for the loop.
* Iterates over the nodes, not the data.
*/
#define NODE_FOREACH(type, iterator, node) \
    for(type iterator = (type) node ; \
        iterator != NULL ;\
        iterator = nodeGetNext(iterator))


#endif /* _NODE_H */