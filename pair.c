#include <stdlib.h>
#include "pair.h"


struct pair_t
{
	Element first;
	Element second;
	ElemCopyFunc firstCopyFunc;
	ElemCopyFunc secondCopyFunc;
	ElemFreeFunc firstFreeFunc;
	ElemFreeFunc secondFreeFunc;
};

/* =---------------------------------------------------------------------------=

								Pair Functions

   =---------------------------------------------------------------------------=
*/

Pair pairCreate(Element first_element, Element second_element,
	ElemCopyFunc first_copy_func, ElemCopyFunc second_copy_func,
	ElemFreeFunc first_free_func, ElemFreeFunc second_free_func)
{
	if (first_element == NULL || second_element == NULL ||
		first_copy_func == NULL || second_copy_func == NULL ||
		first_free_func == NULL || second_free_func == NULL) {
		return NULL;
	}

	Pair pair = malloc(sizeof(*pair));
	if (pair == NULL) {
		return NULL;
	}
	pair->first = first_copy_func(first_element);
	if (pair->first == NULL) {
		free(pair);
		return NULL;
	}
	pair->second = second_copy_func(second_element);
	if (pair->second == NULL) {
		first_free_func(pair->first);
		free(pair);
		return NULL;
	}

	pair->firstCopyFunc = first_copy_func;
	pair->secondCopyFunc = second_copy_func;
	pair->firstFreeFunc = first_free_func;
	pair->secondFreeFunc = second_free_func;

	return pair;
}


void pairDestroy(Pair pair) //Generic function passed to node_t.
{
	if (pair == NULL) {
		return;
	}
	pair->firstFreeFunc(pair->first);
	pair->secondFreeFunc(pair->second);
	free(pair);
}





Element pairGetFirst(Pair pair)
{
	if (pair == NULL) {
		return NULL;
	}
	return pair->firstCopyFunc(pair->first);
}


Element pairGetSecond(Pair pair)
{
	if (pair == NULL) {
		return NULL;
	}
	return pair->secondCopyFunc(pair->second);
}

Pair pairCopy(Pair pair)
{
	if (pair == NULL) {
		return NULL;
	}


	Pair pair_copy = pairCreate(pair->first, pair->second, pair->firstCopyFunc,
		pair->secondCopyFunc, pair->firstFreeFunc, pair->secondFreeFunc);

	if (pair_copy == NULL) {
		return NULL;
	}

	return pair_copy;
}

Element pairFirst(Pair pair)
{
	if (pair == NULL) {
		return NULL;
	}
	return pair->first;
}

Element pairSecond(Pair pair)
{
	if (pair == NULL) {
		return NULL;
	}
	return pair->second;
}