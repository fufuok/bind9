
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <windows.h>

#include <isc/assertions.h>
#include <isc/time.h>

/***
 *** Intervals
 ***/

void
isc_interval_set(isc_interval_t i,
		 unsigned int seconds, unsigned int nanoseconds) {

	/*
	 * Set 'i' to a value representing an interval of 'seconds' seconds
	 * and 'nanoseconds' nanoseconds, suitable for use in isc_time_add()
	 * and isc_time_subtract().
	 */

	REQUIRE(i != NULL);
	REQUIRE(nanoseconds < 1000000000);

	i->interval = (LONGLONG)seconds * 10000000 + nanoseconds / 100;
}

isc_boolean_t
isc_interval_iszero(isc_interval_t i) {

	/*
	 * Returns ISC_TRUE iff. 'i' is the zero interval.
	 */

	REQUIRE(i != NULL);
	if (i->interval == 0)
		return (ISC_TRUE);

	return (ISC_FALSE);
}


/***
 *** Absolute Times
 ***/

static FILETIME epoch = { 0, 0 };

void
isc_time_settoepoch(isc_time_t t) {
	/*
	 * Set 't' to the time of the epoch.
	 */

	REQUIRE(t != NULL);

	t->absolute = epoch;
}

isc_boolean_t
isc_time_isepoch(isc_time_t t) {
	/*
	 * Returns ISC_TRUE iff. 't' is the epoch ("time zero").
	 */

	REQUIRE(t != NULL);

	if (CompareFileTime(&t->absolute, &epoch) == 0)
		return (ISC_TRUE);

	return (ISC_FALSE);
}

isc_result
isc_time_get(isc_time_t t) {
	/*
	 * Set *t to the current absolute time.
	 */
	
	REQUIRE(t != NULL);

	GetSystemTimeAsFileTime(&t->absolute);

	return (ISC_R_SUCCESS);
}

int
isc_time_compare(isc_time_t t1, isc_time_t t2) {
	LARGE_INTEGER i1, i2;

	/*
	 * Compare the times referenced by 't1' and 't2'
	 */

	REQUIRE(t1 != NULL && t2 != NULL);

	return ((int)CompareFileTime(&t1->absolute, &t2->absolute));
}

void
isc_time_add(isc_time_t t, isc_interval_t i, isc_time_t result)
{
	LARGE_INTEGER i1, i2;

	/*
	 * Add 't' to 'i', storing the result in 'result'.
	 */

	REQUIRE(t != NULL && i != NULL && result != NULL);

	i1.LowPart = t->absolute.dwLowDateTime;
	/* XXX trouble here if high bit set (i.e. signed -> unsigned?) */
	i1.HighPart = t->absolute.dwHighDateTime;

	i2.QuadPart = i1.QuadPart + i.interval;
	
	/* XXX potential signed to unsigned problem */
	result->absolute.dwLowDateTime = i2.LowPart;
	result->absolute.dwHighDateTime = i2.HighPart;
}

void
isc_time_subtract(isc_time_t t, isc_interval_t i, isc_time_t result) {
	LARGE_INTEGER i1, i2;

	/*
	 * Subtract 'i' from 't', storing the result in 'result'.
	 */

	REQUIRE(t != NULL && i != NULL && result != NULL);

	i1.LowPart = t->absolute.dwLowDateTime;
	/* XXX trouble here if high bit set (i.e. signed -> unsigned?) */
	i1.HighPart = t->absolute.dwHighDateTime;

	i2.QuadPart = i1.QuadPart - i.interval;
	
	/* XXX potential signed to unsigned problem */
	result->absolute.dwLowDateTime = i2.LowPart;
	result->absolute.dwHighDateTime = i2.HighPart;
}

/***
 *** Win32 Only
 ***/

unsigned int
isc_time_millidiff(isc_time_t t1, isc_time_t t2) {
	LARGE_INTEGER i1, i2;
	LONGLONG i3;

	i1.LowPart = t1->absolute.dwLowDateTime;
	/* XXX trouble here if high bit set (i.e. signed -> unsigned?) */
	i1.HighPart = t1->absolute.dwHighDateTime;
	i2.LowPart = t2->absolute.dwLowDateTime;
	/* XXX trouble here if high bit set (i.e. signed -> unsigned?) */
	i2.HighPart = t2->absolute.dwHighDateTime;

	if (i1.QuadPart <= i2.QuadPart)
		return (0);
	i3 = (i1.QuadPart - i2.QuadPart) / 10000; /* convert to milliseconds */
	if (i3 > 1000000000)			  /* XXX arbitrary! */
		return (1000000000);
	return ((unsigned int)(i3));
}
