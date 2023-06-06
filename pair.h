#ifndef _PAIR_H
#define _PAIR_H

/* Type use for defining a pair	*/

typedef struct pair_t* Pair;

typedef void* Element;
typedef Element(*ElemCopyFunc)(Element);
typedef void(*ElemFreeFunc)(Element);
typedef int(*ElemCompareFunc)(Element, Element);

/** Type used for returning error codes from pair functions */

typedef enum {
	PAIR_NULL_ARG,
	PAIR_MEMORY_ERROR,
	PAIR_SUCCESS
}PairResult;

/*
pairCreate: Creates a new pair of elements.

@param first_element - The first element in the pair.
@param second_element - The second element in the pair.
@param first_copy_func - The first element's copying function.
@param second_copy_func - The second element's copying function.
@param first_free_func - The first element's deallocating function.
@param second_free_func - The second element's deallocating function.

@return NULL if one of the arguments is NULL, or if the memory allocation failed.
		Else, it will return a pair that stores a copy of 2 elements.
*/
Pair pairCreate(Element first_element, Element second_element,
	ElemCopyFunc first_copy_func, ElemCopyFunc second_copy_func,
	ElemFreeFunc first_free_func, ElemFreeFunc second_free_func);

/*
pairDestroy: Deallocates the pair and its elements.

@param pair - The pair to deallocate.
*/
void pairDestroy(Pair pair);

/*
pairSetFirst: Changes the first element of the pair.

@param pair - The pair to change.
@param element - The element to be stored as first.

@return PAIR_NULL_ARG - If the supplied arguments are NULL.
		PAIR_MEMORY_ERROR - If the element copy function failed.
		PAIR_SUCCESS - If the replacment is successfull.
*/
PairResult pairSetFirst(Pair pair, Element element);

/*
pairSetSecond: Changes the second element of the pair.

@param pair - The pair to change.
@param element - The element to be stored as second.

@return PAIR_NULL_ARG - If the supplied arguments are NULL.
		PAIR_MEMORY_ERROR - If the element copy function failed.
		PAIR_SUCCESS - If the replacment is successfull.
*/
PairResult pairSetSecond(Pair pair, Element element);

/*
pairGetFirst: Returns the first element in the pair.

@param pair - The pair to extract the first element from.

@return NULL if the supplied pair is NULL.
		Else it will return a copy of the first element in the pair.
*/
Element pairGetFirst(Pair pair);

/*
pairGetSecond: Returns the second element in the pair.

@param pair - The pair to extract the second element from.


@return NULL if the supplied pair is NULL.
		Else it will return a copy of the second element in the pair.
*/
Element pairGetSecond(Pair pair);

/*
pairCopy: Returns a copy of a given pair.

@param pair - The pair to copy.

@return NULL if the supplied pair is NULL or if the creationg of the copy failed.
		Else, it will return a copy of the pair, which stores copies of both of the original elements.
*/
Pair pairCopy(Pair pair);

/*
pairFirst: Returns the first element stored in the pair
			The returned element is not copied, and therefore it is the user's
			responsibility not to modify it in any way.

@param pair - The pair to extract the element from.

@return NULL if the given pair is NULL.
		Else, returns the element stored in the pair (Not a copy)
*/

Element pairFirst(Pair pair);
/*
pairSecond: Returns the second element stored in the pair
			The returned element is not copied, and therefore it is the user's
			responsibility not to modify it in any way.

@param pair - The pair to extract the element from.

@return NULL if the given pair is NULL.
		Else, returns the element stored in the pair (Not a copy)
*/
Element pairSecond(Pair pair);

#endif /* _PAIR_H */