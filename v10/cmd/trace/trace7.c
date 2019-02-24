#include <stdio.h>
#include "trace.h"
#include "trace.d"

struct Swiffle *slist, *slast, *flast;
int nswiff = 0, fswiff = 0;

struct Swiffle *
unswiffle(avoid)
	struct STATE *avoid;
{
	register struct Swiffle *try;
	register int i;
	struct Swiffle *result = (struct Swiffle *) NULL;

	for (i = nswiff, try = slist; i > 0; i--, try = try->next)
		if (try->st != avoid)
		{	result = try;
			getput(try, i);
			break;
		}

	return result;
}

swiffle(this, that)
	struct STATE *this;
	struct VISIT *that;
{
	struct Swiffle *getswiff(), *try;

	try = getswiff();
	try->st = this;
	try->vi = that;

	if (nswiff++ == 0)
		slist = try;
	else
	{	slast->next = try;
		try->last = slast;
	}
	slast = try;
}

getput(try, n)
	struct Swiffle *try;
{
/* unlink from slist: */
	if (try == slist)
		slist = try->next;
	else
		try->last->next = try->next;

	if (n > 1)
		try->next->last = try->last;

	if (try == slast)
	{	if (try == slist)
			slast = (struct Swiffle *) NULL;
		else
			slast = try->last;
	}
	nswiff--;

/* relink in flist: */

	try->next = (struct Swiffle *) NULL;

	if (fswiff++ == 0)
		try->last = (struct Swiffle *) NULL;
	else
	{	flast->next = try;
		try->last = flast;
	}
	flast = try;
}

struct Swiffle *
getswiff()
{	struct Swiffle *try;
	char *Smalloc();

	if (fswiff == 0)
		try = (struct Swiffle *) Smalloc(sizeof(struct Swiffle));
	else
	{	fswiff--;
		try = flast;
		if ((flast = try->last) != NULL)
			flast->next = (struct Swiffle *) NULL;
	}
	try->next = try->last = (struct Swiffle *) NULL;

	return try;
}
