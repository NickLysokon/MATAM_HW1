#include <stdlib.h>
#include "date.h"

#define MAX_DAYS 30
#define MAX_MONTHS 12
#define MIN_DAYS 1
#define MIN_MONTH 1
#define EQUAL_DATES 0
#define FIRST_DATE_EARLIER -1
#define SECOND_DATE_EARLIER 1

struct Date_t {
	int day;
	int month;
	int year;
};

/* =---------------------------------------------------------------------------=

							Static Functions Declarations

   =---------------------------------------------------------------------------=
*/

/*
compareInt: Compares 2 integers.

@param num1 - first integer
@param num2 - second integer

@return FIRST_DATE_EARLIER if num1 is smaller than num2.
		SECOND_DATE_EARLIER if num2 is smaller than num1.
		EQUAL_DATES if both numbers are equal.
*/
static int compareInt(int num1, int num2);

/* =---------------------------------------------------------------------------=

								Date Functions

   =---------------------------------------------------------------------------=
*/

Date dateCreate(int day, int month, int year)
{
	if (day < MIN_DAYS || day > MAX_DAYS || month < MIN_MONTH || month > MAX_MONTHS) { //Years can be negative
		return NULL;
	}

	Date date = malloc(sizeof(*date));
	if (date == NULL) {
		return NULL;
	}

	date->day = day;
	date->month = month;
	date->year = year;
	return date;
}


void dateDestroy(Date date)
{
	free(date);
}


Date dateCopy(Date date)
{
	if (date == NULL) {
		return NULL;
	}
	Date date_copy = dateCreate(date->day, date->month, date->year);
	if (date_copy == NULL) {
		return NULL;
	}
	return date_copy;
}


bool dateGet(Date date, int* day, int* month, int* year)
{
	if (date == NULL || day == NULL || month == NULL || year == NULL) {
		return false;
	}
	*day = date->day;
	*month = date->month;
	*year = date->year;
	return true;
}


int dateCompare(Date date1, Date date2)
{
	if (date1 == NULL || date2 == NULL) {
		return 0;
	}

	int res = compareInt(date1->year, date2->year);
	if (res != 0) {
		return res;
	}
//We know they are in the same year.
	res = compareInt(date1->month, date2->month);
	if (res != 0) {
		return res;
	}
//We know they are in the same month.
	return compareInt(date1->day, date2->day);
}


void dateTick(Date date)
{
	if (date == NULL) {
		return;
	}

	if (date->day == MAX_DAYS) {
		date->day = 1;
		if (date->month == MAX_MONTHS) {
			date->month = 1;
			date->year += 1;
		}
		else {
			date->month += 1;
		}
	}
	else {
		date->day += 1;
	}

}

/* =---------------------------------------------------------------------------=

								Static Functions

   =---------------------------------------------------------------------------=
*/

static int compareInt(int num1, int num2)
{
	if (num1 < num2) {
		return FIRST_DATE_EARLIER;
	}
	else if (num1 > num2) {
		return SECOND_DATE_EARLIER;
	}
	else {
		return EQUAL_DATES;
	}
}