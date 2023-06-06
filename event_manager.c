#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "priority_queue.h"
#include "event_manager.h"
#include "event.h"
#include "student.h"
#include "pair.h"

#define EQUAL_ELEMENTS 0
#define FIRST_ELEMENT_BIGGER 1
#define SECOND_ELEMENT_BIGGER -1
#define NO_SIZE -1
#define REVERSE_PRIORITY -1
#define EVENT_REMOVED 0
#define EVENT_QUEUE_UPDATED 1
#define EVENT_QUEUE_OUT_OF_MEMORY 2

struct EventManager_t {
	Date current_date;
	PriorityQueue events;
	PriorityQueue students;
};

/* =---------------------------------------------------------------------------=

							Static Functions Declarations

   =---------------------------------------------------------------------------=
*/

/*
studentPriorityCompare: Compares between 2 student priorities.

@param pair1 - A pair of the first student's event count and his id.
@param pair2 - A pair of the second student's event count and his id.

@return EQUAL_ELEMENTS if the event counts and id's are equal.
		FIRST_ELEMENT_BIGGER if the first pair is of higher priority.
		SECOND_ELEMENT_BIGGER if the second pair is of higher priority.
*/
static int studentPriorityCompare(Pair pair1, Pair pair2);

/*
dateCompareEarliest: Compares between 2 dates and returns the earliest.

@param date1 - The date to compare with the other date.
@param date2 - The date to compare with the other date.

@return EQUAL_ELEMENTS if the dates are in the same day.
		FIRST_ELEMENT_BIGGER if the first date is earlier
		SECOND_ELEMENT_BIGGER if the second date is earlier.
*/
static int dateCompareEarliest(Date date1, Date date2);

/*
intCompare: Compares between 2 integers and returns the biggest.

@param num1 - The first number to compare.
@param num2 - The second number to compare.

@return EQUAL_ELEMENTS if the numbers are equal.
		FIRST_ELEMENT_BIGGER if the first number is bigger.
		SECOND_ELEMENT_BIGGER if the second number is bigger.
*/
static int intCompare(int num1, int num2);

/*
intCopy: Copies an integer and returns it.

@param element - The integer to copy.


@return NULL if the element is NULL or a memory allocation fails.
		Else returns a copy of the integer.
*/
static void* intCopy(void* num);

/*
intFree: Deallocates an integer.

@param num - The integer to deallocate.
*/
static void intFree(void* num);

/*
checkEventQueue: Checks if there is already an event with the given name and date in the queue.

@param events - The priority queue to iterate over.
@param event_name - The event name to search for.
@param event_date - The event date to search for.

@return EM_OUT_OF_MEMORY if a memory error occurs,
		EM_EVENT_ALREADY_EXISTS if there is an event with the given name and date,
		EM_SUCCESS if there isn't an event with the given paramaters.
*/
static EventManagerResult checkEventQueue(PriorityQueue events, char* event_name, Date event_date);

/*
findEvent: Searches for an event by its id.

@param events - The priority queue to iterate over.
@param event_id - The event id to search for.

@return NULL if the event doesn't exist in the queue.
		Else, returns the event. (NOT A COPY)
*/
static Event findEvent(PriorityQueue events, int event_id);

/*
findStudent: Searches for a student by its id.

@param students - The priority queue to iterate over.
@param student_id - The student id to search for.

@return NULL if the student doesn't exist in the queue.
		Else, returns the student. (NOT A COPY)
*/
static Student findStudent(PriorityQueue students, int student_id);

/*
changeStudentEventCount: Changes a student's event count and updates the priority queue.

@param students - The priority queue to iterate over.
@param student - The student to update its event count and queue priority.
@param increment - The amount to add/subtract from the student's event count.

@return EM_OUT_OF_MEMORY if the change priority function fails.
		EM_SUCCESS if the student's priority was changed successfully.
*/
static EventManagerResult changeStudentEventCount(PriorityQueue students, Student student, int increment);

/*
changeStudentPriority: Updates a student's priority.

@param students - The priority queue to iterate over.
@param student - The student to update its queue priority.

@return EM_OUT_OF_MEMORY if a memory allocation fails.
		Else, returns the student. (NOT A COPY)
*/
static EventManagerResult changeStudentPriority(PriorityQueue students, Student student);

/*
eventPrintStudentList: Prints the names of the students in the id list.

@param students - The priority queue to iterate over.
@param id_list - The id list that stores the students id's
@param stream - The file to print into.
*/
static void eventPrintStudentList(PriorityQueue students, Node id_list, FILE* stream);

/*
updateEventQueue: Removes one of the outdated events while using emTick.

@param em - The event manager the stores the events queue.

@return EVENT_REMOVED if one outdated event has been removed successfully.
		EVENT_QUEUE_UPDATED if all of the events in the queue aren't outdated.
		EVENT_QUEUE_OUT_OF_MEMORY if a memory allocation has failed.
*/
static int updateEventQueue(EventManager em);


/* =---------------------------------------------------------------------------=

								Event Manager Functions

   =---------------------------------------------------------------------------=
*/


EventManager createEventManager(Date date)
{
	if (date == NULL) {
		return NULL;
	}

	EventManager manager = malloc(sizeof(*manager));
	if (manager == NULL) {
		return NULL;
	}

	manager->current_date = dateCopy(date);
	if (manager->current_date == NULL) {
		free(manager);
		return NULL;
	}
	manager->events = pqCreate((ElemCopyFunc)eventCopy, (ElemFreeFunc)eventDestroy,
		(EqualPQElements)eventEquals, (CopyPQElementPriority)dateCopy,
		(FreePQElementPriority)dateDestroy,
		(ComparePQElementPriorities)dateCompareEarliest);
	if (manager->events == NULL) {
		dateDestroy(manager->current_date);
		free(manager);
		return NULL;
	}
	manager->students = pqCreate((ElemCopyFunc)stCopy, (ElemFreeFunc)stDestroy,
		(EqualPQElements)stEquals, (CopyPQElementPriority)pairCopy,
		(FreePQElementPriority)pairDestroy,
		(ComparePQElementPriorities)studentPriorityCompare);
	if (manager->students == NULL) {
		pqDestroy(manager->events);
		dateDestroy(manager->current_date);
		free(manager);
		return NULL;
	}

	return manager;
}

void destroyEventManager(EventManager em)
{
	if (em == NULL) {
		return;
	}

	pqDestroy(em->events);
	pqDestroy(em->students);
	dateDestroy(em->current_date);
	free(em);
}

EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id)
{
	if (em == NULL || event_name == NULL || date == NULL) {
		return EM_NULL_ARGUMENT;
	}
	else if (dateCompareEarliest(em->current_date, date) == SECOND_ELEMENT_BIGGER) {
		return EM_INVALID_DATE;
	}
	else if (event_id < 0) {
		return EM_INVALID_EVENT_ID;
	}

	int res = checkEventQueue(em->events, event_name, date);
	if (res != EM_SUCCESS) {
		return res;
	}
	Event ptr = findEvent(em->events, event_id);
	if (ptr != NULL) {
		return EM_EVENT_ID_ALREADY_EXISTS;
	}
	Event event = eventCreate(event_name, event_id, date);
	assert(event != NULL);
	if (event == NULL) {
		return EM_OUT_OF_MEMORY;
	}

	res = pqInsert(em->events, event, date);
	assert(res != PQ_OUT_OF_MEMORY);
	if (res == PQ_OUT_OF_MEMORY) {
		eventDestroy(event);
		return EM_OUT_OF_MEMORY;
	}

	eventDestroy(event);
	return EM_SUCCESS;
}

EventManagerResult emAddEventByDiff(EventManager em, char* event_name, int days, int event_id)
{
	if (em == NULL) {
		return EM_NULL_ARGUMENT;
	}
	if (days < 0) {
		return EM_INVALID_DATE;
	}

	Date date = dateCopy(em->current_date);
	assert(date != NULL);
	if (date == NULL) {
		return EM_OUT_OF_MEMORY;
	}
	for (int i = 0; i < days; i++)
	{
		dateTick(date);
	}

	int res = emAddEventByDate(em, event_name, date, event_id);
	dateDestroy(date);
	return res;
}

EventManagerResult emRemoveEvent(EventManager em, int event_id)
{
	if (em == NULL) {
		return EM_NULL_ARGUMENT;
	}
	else if (event_id < 0) {
		return EM_INVALID_EVENT_ID;
	}

	Event ptr = findEvent(em->events, event_id);
	if (ptr == NULL) {
		return EM_EVENT_NOT_EXISTS;
	}
	NODE_FOREACH(Node, node, eventGetIdList(ptr)) {
		int* id = nodeGet(node);
		assert(id != NULL);
		Student student = findStudent(em->students, *id);
		assert(student != NULL);
		if (changeStudentEventCount(em->students, student, -1) != EM_SUCCESS) {
			return EM_OUT_OF_MEMORY;
		}
	}
	int res = pqRemoveElement(em->events, ptr);
	if (res != PQ_SUCCESS) {
		return EM_OUT_OF_MEMORY;
	}
	return EM_SUCCESS;
}

EventManagerResult emChangeEventDate(EventManager em, int event_id, Date new_date)
{
	if (em == NULL || new_date == NULL) {
		return EM_NULL_ARGUMENT;
	}
	else if (dateCompare(em->current_date, new_date) > 0) {
		return EM_INVALID_DATE;
	}
	else if (event_id < 0) {
		return EM_INVALID_EVENT_ID;
	}

	Event event = findEvent(em->events, event_id);
	if (event == NULL) {
		return EM_EVENT_ID_NOT_EXISTS;
	}
	int res = checkEventQueue(em->events, eventGetNamePtr(event), new_date);
	if (res != EM_SUCCESS) {
		return res;
	}

	Date event_date = eventGetDate(event);
	if (event_date == NULL) {
		return EM_OUT_OF_MEMORY;
	}
	res = eventSetDate(event, new_date);
	if (res != EVENT_SUCCESS) {
		dateDestroy(event_date);
		return EM_OUT_OF_MEMORY;
	}
	Event event_copy = eventCopy(event); //The queue iterator doesn't return a copy.
	if (event_copy == NULL) {
		dateDestroy(event_date);
		return EM_OUT_OF_MEMORY;
	}

	res = pqChangePriority(em->events, event_copy, event_date, new_date);
	if (res != PQ_SUCCESS) {
		eventDestroy(event_copy);
		dateDestroy(event_date);
		return EM_OUT_OF_MEMORY;
	}

	eventDestroy(event_copy);
	dateDestroy(event_date);
	return EM_SUCCESS;
}

EventManagerResult emAddMember(EventManager em, char* member_name, int member_id)
{
	if (em == NULL || member_name == NULL) {
		return EM_NULL_ARGUMENT;
	}
	else if (member_id < 0) {
		return EM_INVALID_MEMBER_ID;
	}

	Student student = findStudent(em->students, member_id);
	if (student != NULL) {
		return EM_MEMBER_ID_ALREADY_EXISTS;
	}
	student = stCreate(member_name, member_id);
	assert(student != NULL);
	if (student == NULL) {
		return EM_OUT_OF_MEMORY;
	}

	int event_count = 0;
	Pair priority = pairCreate(&event_count, &member_id, intCopy, intCopy, intFree, intFree);
	assert(priority != NULL);
	if (priority == NULL) {
		stDestroy(student);
		return EM_OUT_OF_MEMORY;
	}
	else if (pqInsert(em->students, student, priority) != PQ_SUCCESS) {
		stDestroy(student);
		pairDestroy(priority);
		return EM_OUT_OF_MEMORY;
	}

	stDestroy(student);
	pairDestroy(priority);
	return EM_SUCCESS;
}

EventManagerResult emAddMemberToEvent(EventManager em, int member_id, int event_id)
{
	if (em == NULL) {
		return EM_NULL_ARGUMENT;
	}
	else if (member_id < 0) {
		return EM_INVALID_MEMBER_ID;
	}
	else if (event_id < 0) {
		return EM_INVALID_EVENT_ID;
	}

	Event event = findEvent(em->events, event_id);
	if (event == NULL) {
		return EM_EVENT_ID_NOT_EXISTS;
	}
	Student student = findStudent(em->students, member_id);
	if (student == NULL) {
		return EM_MEMBER_ID_NOT_EXISTS;
	}

	int res = eventAddStudentId(event, member_id);
	if (res == EVENT_STUDENT_ALREDY_LINKED) {
		return EM_EVENT_AND_MEMBER_ALREADY_LINKED;
	}
	else if (res == EVENT_MEMORY_FAIL) {
		return EM_OUT_OF_MEMORY;
	}
	res = changeStudentEventCount(em->students, student, 1);
	if (res == EM_OUT_OF_MEMORY) {
		return EM_OUT_OF_MEMORY;
	}

	return EM_SUCCESS;
}

EventManagerResult emRemoveMemberFromEvent(EventManager em, int member_id, int event_id)
{
	if (em == NULL) {
		return EM_NULL_ARGUMENT;
	}
	else if (event_id < 0) {
		return EM_INVALID_EVENT_ID;
	}
	else if (member_id < 0) {
		return EM_INVALID_MEMBER_ID;
	}

	Event event = findEvent(em->events, event_id);
	if (event == NULL) {
		return EM_EVENT_ID_NOT_EXISTS;
	}
	Student student = findStudent(em->students, member_id);
	if (student == NULL) {
		return EM_MEMBER_ID_NOT_EXISTS;
	}

	int res = eventRemoveStudentId(event, member_id);
	if (res == EVENT_STUDENT_NOT_LINKED) {
		return EM_EVENT_AND_MEMBER_NOT_LINKED;
	}
	else if (res == EVENT_MEMORY_FAIL) {
		return EM_OUT_OF_MEMORY;
	}
	res = changeStudentEventCount(em->students, student, -1);
	if (res == EM_OUT_OF_MEMORY) {
		return EM_OUT_OF_MEMORY;
	}
	return EM_SUCCESS;
}

EventManagerResult emTick(EventManager em, int days)
{
	if (em == NULL) {
		return EM_NULL_ARGUMENT;
	}
	if (days <= 0) {
		return EM_INVALID_DATE;
	}

	for (int i = 0; i < days; i++)
	{
		dateTick(em->current_date);
	}

	int res = updateEventQueue(em);
	while (res == EVENT_REMOVED) {  //We must call the function again because emRemoveEvent changes the iterator.
		res = updateEventQueue(em);
	}

	if (res == EVENT_QUEUE_UPDATED) {
		return EM_SUCCESS;
	}
	else {
		return EM_OUT_OF_MEMORY;
	}
}

int emGetEventsAmount(EventManager em)
{
	if (em == NULL) {
		return NO_SIZE;
	}
	return pqGetSize(em->events);
}

char* emGetNextEvent(EventManager em)
{
	if (em == NULL) {
		return NULL;
	}
	Event event = pqGetFirst(em->events);
	return eventGetNamePtr(event);
}

void emPrintAllEvents(EventManager em, const char* file_name)
{
	if (em == NULL || file_name == NULL) {
		return;
	}

	FILE* fd = fopen(file_name, "w");
	if (fd == NULL) {
		return;
	}
	PQ_FOREACH(Event, event, em->events) {
		char* name = eventGetName(event);
		assert(name != NULL);
		Date date = eventGetDate(event);
		assert(date != NULL);

		int day = 0, month = 0, year = 0;
		dateGet(date, &day, &month, &year);

		fprintf(fd, "%s,%d.%d.%d", name, day, month, year);
		eventPrintStudentList(em->students, eventGetIdList(event), fd);
		fprintf(fd, "\n");

		free(name);
		dateDestroy(date);
	}
	fclose(fd);
}

void emPrintAllResponsibleMembers(EventManager em, const char* file_name)
{
	if (em == NULL || file_name == NULL) {
		return;
	}

	FILE* fd = fopen(file_name, "w");
	if (fd == NULL) {
		return;
	}
	PQ_FOREACH(Student, student, em->students) {
		int event_count = stGetEventCount(student);
		if (event_count == 0) {
			break;
		}
		char* name = stGetName(student);
		assert(name != NULL);

		fprintf(fd, "%s,%d\n", name, event_count);

		free(name);
	}
	fclose(fd);
}

/* =---------------------------------------------------------------------------=

								Static Functions

   =---------------------------------------------------------------------------=
*/


static int studentPriorityCompare(Pair pair1, Pair pair2)
{

	assert(pair1 != NULL && pair2 != NULL);

	int* count1 = pairGetFirst(pair1), * count2 = pairGetFirst(pair2);
	assert(count1 != NULL && count2 != NULL);

	int res = intCompare(*count1, *count2);
	if (res != EQUAL_ELEMENTS) {
		free(count1);
		free(count2);
		return res;
	}

	int* id1 = pairGetSecond(pair1), * id2 = pairGetSecond(pair2);
	assert(id1 != NULL && id2 != NULL);

	res = intCompare(*id2, *id1);
	//Need to free, because the function allocated memory in the heap.
	free(count1);
	free(count2);
	free(id1);
	free(id2);
	return res;

}

static int dateCompareEarliest(Date date1, Date date2)
{
	return (REVERSE_PRIORITY * dateCompare(date1, date2));
}

static int intCompare(int num1, int num2)
{
	if (num1 > num2) {
		return FIRST_ELEMENT_BIGGER;
	}
	else if (num1 < num2) {
		return SECOND_ELEMENT_BIGGER;
	}
	else {
		return EQUAL_ELEMENTS;
	}
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

static void intFree(void* num)
{
	free(num);
}

static EventManagerResult checkEventQueue(PriorityQueue events, char* event_name, Date event_date)
{
	PQ_FOREACH(Event, current_event, events) {
		char* name = eventGetName(current_event);
		if (name == NULL) {
			return EM_OUT_OF_MEMORY;
		}
		Date date = eventGetDate(current_event);
		if (date == NULL) {
			free(name);
			return EM_OUT_OF_MEMORY;
		}

		if (!strcmp(event_name, name) && !dateCompare(event_date, date)) {
			free(name);
			dateDestroy(date);
			return EM_EVENT_ALREADY_EXISTS;
		}

		free(name);
		dateDestroy(date);
	}
	return EM_SUCCESS;
}

static Event findEvent(PriorityQueue events, int event_id)
{
	PQ_FOREACH(Event, ptr, events) {
		if (eventGetId(ptr) == event_id) {
			return ptr;
		}
	}
	return NULL;
}

static Student findStudent(PriorityQueue students, int student_id)
{
	PQ_FOREACH(Student, ptr, students) {
		if (stGetId(ptr) == student_id) {
			return ptr;
		}
	}
	return NULL;
}

static EventManagerResult changeStudentEventCount(PriorityQueue students, Student student, int increment)
{
	stSetEventCount(student, increment);
	if (changeStudentPriority(students, student) != EM_SUCCESS) {
		return EM_OUT_OF_MEMORY;
	}
	return EM_SUCCESS;
}

static EventManagerResult changeStudentPriority(PriorityQueue students, Student student)
{
	int event_count = stGetEventCount(student), student_id = stGetId(student);
	Pair new_priority = pairCreate(&event_count, &student_id, intCopy, intCopy, intFree, intFree);
	if (new_priority == NULL) {
		return EM_OUT_OF_MEMORY;
	}
	Student new_student = stCopy(student);
	if (new_student == NULL) {
		return EM_OUT_OF_MEMORY;
	}

	int res = pqRemoveElement(students, student);
	if (res != PQ_SUCCESS) {
		stDestroy(new_student);
		pairDestroy(new_priority);
		return EM_OUT_OF_MEMORY;
	}
	res = pqInsert(students, new_student, new_priority);
	if (res != PQ_SUCCESS) {
		stDestroy(new_student);
		pairDestroy(new_priority);
		return EM_OUT_OF_MEMORY;
	}

	stDestroy(new_student);
	pairDestroy(new_priority);
	return EM_SUCCESS;
}

static void eventPrintStudentList(PriorityQueue students, Node id_list, FILE* stream)
{
	if (id_list == NULL || stream == NULL) {
		return;
	}
	assert(students != NULL);

	NODE_FOREACH(Node, node, id_list) {
		int* id = nodeGet(node);
		assert(id != NULL);
		Student student = findStudent(students, *id);
		assert(student != NULL);

		char* name = stGetName(student);
		if (name == NULL) {
			return;
		}

		fprintf(stream, ",%s", name);
		free(name);
	}
}

static int updateEventQueue(EventManager em)
{
	PQ_FOREACH(Event, event, em->events) {
		Date date = eventGetDate(event);
		if (date == NULL) {
			return EVENT_QUEUE_OUT_OF_MEMORY;
		}

		if (dateCompareEarliest(em->current_date, date) == SECOND_ELEMENT_BIGGER) {
			int res = emRemoveEvent(em, eventGetId(event));
			if (res == EM_SUCCESS) {
				dateDestroy(date);
				return EVENT_REMOVED;
			}
			else {
				return EVENT_QUEUE_OUT_OF_MEMORY;
			}
		}
		dateDestroy(date);
	}
	return EVENT_QUEUE_UPDATED;
}