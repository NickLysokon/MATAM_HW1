#include <stdlib.h>
#include "event.h"
#include <string.h>
#include <assert.h>

#define NO_ID -1;

struct event_t {
	char* name;
	int id;
	Date date;
	Node id_list;
};


/* =---------------------------------------------------------------------------=

							Static Functions Declarations

   =---------------------------------------------------------------------------=
*/


/*
stringCopy: Copies a string and returns the copy.

@param str - The string to copy.

@return NULL if the given string is NULL.
		Else, returns a copy of the string.
*/
static char* stringCopy(char* str);

/*
intCopy: Copies an integer and returns the copy.

@param str - The integer to copy.

@return NULL if the given element is NULL.
		Else, returns a copy of the int.
*/
static void* intCopy(void* element);

/*
intFree: Deallocates an integer.

@param str - The int* to copy.
*/
static void intFree(void* element);

/*
sortId: Adds a new student id into an event's id list,
		while keeping the list sorted.

@param event - The event to add the id to.
@param new_id - The student id to add to the id list.

@return EVENT_SUCCESS if the student id hass been successfully added.
		EVENT_STUDENT_ALREDY_LINKED if the student id already exists in the list.
		EVENT_MEMORY_FAIL if a memory error occurs.
*/
static EventResult sortId(Event event, Node new_id);


/* =---------------------------------------------------------------------------=

								Event Functions

   =---------------------------------------------------------------------------=
*/


Event eventCreate(char* event_name, int event_id, Date event_date)
{
	if (event_name == NULL || event_id < 0 || event_date == NULL) {
		return NULL;
	}

	Event event = malloc(sizeof(*event));
	if (event == NULL) {
		return NULL;
	}
	event->name = stringCopy(event_name);
	assert(event->name != NULL);
	if (event->name == NULL) {
		free(event);
		return NULL;
	}
	event->date = dateCopy(event_date);
	assert(event->date != NULL);
	if (event->date == NULL) {
		free(event->name);
		free(event);
		return NULL;
	}
	event->id_list = NULL;
	event->id = event_id;
	return event;
}

void eventDestroy(Event event)
{
	if (event == NULL) {
		return;
	}
	free(event->name);
	dateDestroy(event->date);
	nodeDestroy(event->id_list);
	free(event);
}

Event eventCopy(Event event)
{
	if (event == NULL) {
		return NULL;
	}

	Event copy_event = eventCreate(event->name, event->id, event->date);
	assert(copy_event != NULL);
	if (copy_event == NULL) {
		return NULL;
	}
	if (event->id_list != NULL){
		copy_event->id_list = nodeListCopy(event->id_list);
		assert(copy_event->id_list != NULL);
		if (copy_event->id_list == NULL) {
			free(copy_event);
			return NULL;
		}
	}
	return copy_event;
}

Date eventGetDate(Event event)
{
	if (event == NULL) {
		return NULL;
	}
	return dateCopy(event->date);
}

Node eventGetIdList(Event event)
{
	if (event == NULL) {
		return NULL;
	}
	return event->id_list;
}





char* eventGetNamePtr(Event event)
{
	if (event == NULL){
		return NULL;
	}
	return event->name;
}

EventResult eventSetDate(Event event, Date new_event_date)
{
	if (event == NULL || new_event_date == NULL) {
		return EVENT_NULL_ARG;
	}

	dateDestroy(event->date);
	event->date = dateCopy(new_event_date);
	if (event->date == NULL) {
		return EVENT_MEMORY_FAIL;
	}
	return EVENT_SUCCESS;
}

EventResult eventAddStudentId(Event event, int student_id)
{
	if (event == NULL || student_id < 0) {
		return EVENT_NULL_ARG;
	}

	Node new_id = nodeCreate(&student_id,intCopy, intFree);
	if (new_id == NULL) {
		return EVENT_MEMORY_FAIL;
	}
	int res = sortId(event, new_id);
	if (res == EVENT_MEMORY_FAIL) {
		nodeDestroy(new_id);
		return EVENT_MEMORY_FAIL;
	}
	else if (res == EVENT_STUDENT_ALREDY_LINKED) {
		nodeDestroy(new_id);
		return EVENT_STUDENT_ALREDY_LINKED;
	}
	return EVENT_SUCCESS;
}

EventResult eventRemoveStudentId(Event event, int student_id)
{
	if (event == NULL || student_id < 0) {
		return EVENT_NULL_ARG;
	}

	Node prev = event->id_list;
	int* data = nodeGet(event->id_list);
	if (data == NULL){
		return EVENT_STUDENT_NOT_LINKED;
	}
	if (*data == student_id) {
		event->id_list = nodeGetNext(event->id_list);
		nodeRemove(prev);
		return EVENT_SUCCESS;
	}

	Node ptr = nodeGetNext(prev);
	while (ptr != NULL) {
		int* data = nodeGet(ptr);
		if (data == NULL) {
			return EVENT_MEMORY_FAIL;
		}

		if (*data == student_id) {
			nodeSetNext(prev, nodeGetNext(ptr));
			nodeRemove(ptr);
			return EVENT_SUCCESS;
		}
		prev = ptr;
		ptr = nodeGetNext(ptr);
	}
	return EVENT_STUDENT_NOT_LINKED;
}


int eventGetId(Event event)
{
	if (event == NULL) {
		return NO_ID;
	}
	return event->id;
}

char* eventGetName(Event event)
{
	if (event == NULL) {
		return NULL;
	}
	return stringCopy(event->name);
}

bool eventEquals(Event event1, Event event2)
{
	if (event1 == NULL || event2 == NULL){
		return false;
	}
	if (!dateCompare(event1->date, event2->date) &&
		!strcmp(event1->name, event2->name) &&
		(event1->id == event2->id)){
		return true;
	}
	return false;
}



/* =---------------------------------------------------------------------------=

								Static Functions

   =---------------------------------------------------------------------------=
*/


static char* stringCopy(char* str) {
	if (str == NULL) {
		return NULL;
	}

	int len = strlen(str);
	char* out = malloc(sizeof(char) * (len + 1));
	if (out == NULL) {
		return NULL;
	}
	for (int i = 0; i <= len; i++)
	{
		out[i] = str[i];
	}

	return out;
}

static void* intCopy(void* element)
{
	if (element == NULL) {
		return NULL;
	}

	int* new_int = malloc(sizeof(int));
	if (new_int == NULL) {
		return NULL;
	}
	*new_int = *(int*)element;
	return new_int;
}

static void intFree(void* element)
{
	free(element);
}

static EventResult sortId(Event event, Node new_id)
{
	assert(event != NULL && new_id != NULL);
	int* data = nodeGet(event->id_list), *student_id = nodeGet(new_id);
	if (data == NULL) {
		event->id_list = new_id;
		return EVENT_SUCCESS;
	}
	else if (*data > *student_id){
		Node next = event->id_list;
		event->id_list = new_id;
		nodeSetNext(new_id, next);
		return EVENT_SUCCESS;
	}
	else if (*data == *student_id){
		return EVENT_STUDENT_ALREDY_LINKED;
	}

	Node prev = event->id_list;
	Node ptr = nodeGetNext(event->id_list);
	while (ptr != NULL) {
		data = nodeGet(ptr);
		if (data == NULL) {
			return EVENT_MEMORY_FAIL;
		}

		if (*data < *student_id) {
			prev = ptr;
			ptr = nodeGetNext(ptr);
			continue;
		}
		else if (*data == *student_id) {
			return EVENT_STUDENT_ALREDY_LINKED;
		}
		else {
			break; //Ptr now points to the first id bigger than student_id.
		}
	}
	nodeSetNext(prev, new_id);
	nodeSetNext(new_id, ptr);
	return EVENT_SUCCESS;
}

