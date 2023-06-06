#include <stdlib.h>
#include "student.h"
#include <string.h>

#define NO_ID -1

struct student_t {
	char* name;
	int id;
	int event_count;
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


/* =---------------------------------------------------------------------------=

								Student Functions

   =---------------------------------------------------------------------------=
*/

Student stCreate(char* member_name, int member_id)
{
	if (member_name == NULL || member_id < 0) {
		return NULL;
	}

	Student member = malloc(sizeof(*member));
	if (member == NULL) {
		return NULL;
	}
	member->name = stringCopy(member_name);
	if (member->name == NULL) {
		free(member);
		return NULL;
	}
	member->id = member_id;
	member->event_count = 0;
	return member;
}

void stDestroy(Student student)
{
	if (student == NULL) {
		return;
	}
	free(student->name);
	free(student);
}

Student stCopy(Student student)
{
	if (student == NULL) {
		return NULL;
	}

	Student student_copy = stCreate(((Student)student)->name,((Student)student)->id);
	if (student_copy == NULL) {
		return NULL;
	}
	student_copy->event_count = ((Student)student)->event_count;
	return student_copy;
}

int stGetId(Student student)
{
	if (student == NULL) {
		return NO_ID;
	}
	return student->id;
}

char* stGetName(Student student)
{
	if (student == NULL) {
		return NULL;
	}
	return stringCopy(student->name);
}
void stSetEventCount(Student student, int increment)
{
	if (student == NULL) {
		return;
	}
	if ((student->event_count + increment) < 0) {
		student->event_count = 0;
		return;
	}
	student->event_count += increment;
}

bool stEquals(Student student1, Student student2)
{
	if (student1 == NULL || student2 == NULL){
		return false;
	}
	if ((student1->id == student2->id) && !strcmp(student1->name, student2->name)){
		return true;
	}
	return false;
}


int stGetEventCount(Student student)
{
	if (student == NULL) {
		return NO_ID;
	}
	return student->event_count;
}




/*=--------------------------------------------------------------=
*						Static functions
* =--------------------------------------------------------------=
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