#ifndef _EVENT_H
#define _EVENT_H

#include "date.h"
#include "node.h"

/** Type for defining the event */
typedef struct event_t* Event;

/** Type used for returning error codes from event functions */
typedef enum {
	EVENT_MEMORY_FAIL,
	EVENT_NULL_ARG,
	EVENT_STUDENT_ALREDY_LINKED,
	EVENT_STUDENT_NOT_LINKED,
	EVENT_SUCCESS
}EventResult;


/*
eventCreate: Creates a new event with a name, a unique id and a date.

@param event_name - The event's name
@param event_id - The event's Id.
@param event_date - The event's date.

@return NULL if a memory allocation fails or if the paramaters are NULL.
		Else, returns a new event.
*/
Event eventCreate(char* event_name, int event_id, Date event_date);

/*
eventDestroy: Deallocates a given event.

@param event - The event to deallocate.
*/
void eventDestroy(Event event);

/*
eventCopy: Creates a copy of a given event.

@param event - The event to copy.

@return NULL if a memory allocation fails or if the event is NULL.
		Else, returns a copy of the event.
*/
Event eventCopy(Event event);

/*
eventGetDate: Returns a copy of the date of a given event.

@param event - The event to extract the date from.

@return NULL if a memory allocation fails or if the event is NULL.
		Else, returns a copy of the date of the event.
*/
Date eventGetDate(Event event);

/*
eventGetIdList: Returns a copy of the student id list of the event.

@param event - The event to extract the id list from.

@return NULL if a memory allocation fails or if the event is NULL.
		Else, returns a copy of the id list from the event.
*/
Node eventGetIdList(Event event);

/*
eventGetId: Returns the id number of the event.

@param event - The event to extract the id from.

@return NO_ID if the event is NULL.
		Else, returns the id of the event.
*/
int eventGetId(Event event);

/*
eventGetName: Returns a copy of the name of a given event.

@param event - The event to extract the name from.

@return NULL if the event is NULL, or the copy function fails.
		Else, returns a copy of the name of the event.
*/
char* eventGetName(Event event);

/*
eventGetNamePtr: Returns a pointer to the name of a given event.
				 Only used in the emGetNextEvent function.

@param event - The event to extract the name from.

@return NULL if the event is NULL.
		Else, returns a pointer to the name of the event.
*/
char* eventGetNamePtr(Event event);

/*
eventSetDate: Changes the date of a given event.

@param event - The event to change its date.
@param new_event_date - The new date to change to.

@return EVENT_NULL_ARG if the function arguments are NULL.
		EVENT_MEMORY_FAIL if the date copy function fails.
		EVENT_SUCCESS if the date has been changed successfully.
		
*/
EventResult eventSetDate(Event event, Date new_event_date);

/*
eventAddStudentId: Adds a new student id to the event's student id list.

@param event - The event to add the student id to.
@param student_id - The student id to add to the list.

@return EVENT_NULL_ARG if the function arguments are NULL.
		EVENT_MEMORY_FAIL if a memory allocation fails.
		EVENT_STUDENT_ALREDY_LINKED if the student id alredy exists in the list.
		EVENT_SUCCESS if the student id has been added successfully.
*/
EventResult eventAddStudentId(Event event, int student_id);

/*
eventRemoveStudentId: Removes a given student id from the event's student id list.

@param event - The event to remove the id from its list.
@param student_id - The student id to remove from the list.

@return EVENT_NULL_ARG if the function arguments are NULL.
		EVENT_MEMORY_FAIL if a memory allocation fails.
		EVENT_STUDENT_NOT_LINKED if the student id doesn't exist in the list.
		EVENT_SUCCESS if the student id has been added successfully.
*/
EventResult eventRemoveStudentId(Event event, int student_id);

/*
eventEquals: Compares between two events.

@param event1 - The event to compare with the other event.
@param student_id - The event to compare with the other event.

@return True if the events have the same name, same date and same id.
		Else, returns False.
*/
bool eventEquals(Event event1, Event event2);

#endif /* _EVENT_H */