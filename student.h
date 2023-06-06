#ifndef _STUDENT_H
#define _STUDENT_H

#include <stdbool.h>

/** Type for defining a student */
typedef struct student_t* Student;


/*
stCreate: Creates a new student with a copy of the given name, 
          id and an event count of 0.

@param member_name - The student's name to copy.
@param member_id - The student's id.

@return NULL if a memory allocation fails or if the member_name is NULL.
		Else, returns a the created student.
*/
Student stCreate(char* member_name, int member_id);

/*
stDestroy: Deallocates a student.

@param student - The student to deallocate.
*/
void stDestroy(Student student);

/*
stCopy: Creates a copy of the given student.

@param student - The student to copy.

@return NULL if a memory allocation fails or if student is NULL.
		Else, returns a copy of the student.
*/
Student stCopy(Student student);

/*
stGetId: Returns the id of the given student.

@param student - The student to extract the id from

@return NULL if the student is NULL.
		Else, returns the student's id.
*/
int stGetId(Student student);

/*
stGetName: Returns a copy of the name of the given student.

@param student - The student to extract the name from

@return NULL if the student is NULL or if a memory allocation fails.
		Else, returns the student's id.
*/
char* stGetName(Student student);

/*
stSetEventCount: Increments the event count of the given student.

@param student - The student to add the increment to.
@param increment - The increment to the student's event count (Can be negative).
*/
void stSetEventCount(Student student, int increment);

/*
stGetEventCount: Returns the value of the student's event count.

@param student - The student to extract the event count value from.

@return NO_ID if the student is NULL.
        Else returns the student's event count.
*/
int stGetEventCount(Student student);

/*
stEquals: Compares between 2 students.

@param student1 - The student to compare with the second student.
@param student2 - The student to compate with the first student.

@return True if both students have the same name and the same id.
        Else, returns False if one of the students is NULL,
        or if they arent the same student.
*/
bool stEquals(Student student1, Student student2);

#endif /* _STUDENT_H */