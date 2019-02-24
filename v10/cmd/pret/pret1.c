#include <stdio.h>
#include "pret.h"

#define LOOK	0
#define ADD	1

struct QSET_EL {
	char name[MAXNAME];		/* message name           */
	char status;			/* usage of this message  */
	int id;				/* number assigned to msg */
	struct QSET_EL *nxtel;
};

struct QSETS {
	int nrelmnts;
	int status;			/* usage of the qset itself */
	int qstatus;			/* usage of the setowner (a queue) */
	char setname[MAXNAME];		/* symbolic name */
	char setowner[MAXNAME];		/* queue */
	int qind;			/* NONE if simple q, #expr if array */
	struct QSET_EL *elmnts;
	struct QSETS *nxtset;
	int closed;
};

struct QSETS *frstset;
int nrsets = 0;

extern struct QTABLE qtable[NQUEUES];
extern char procname[MAXNAME];
extern char refname[MAXNAME];
extern pid, rid;
char *Emalloc();

isqset(str)
	char *str;
{	struct QSETS *q = frstset;
	char buf1[MAXNAME], buf2[MAXNAME];
	int i;

	buf1[0] = '\0';
	if (pid != NONE)
		strncpy(buf1, procname, 8);
	else if (rid != NONE)
		strncpy(buf1, refname, 8);
	strcat(buf1, ":");

	strcpy(buf2, "global:");
	strcat(buf1, str);
	strcat(buf2, str);

	for (i = 0; i < nrsets; i++, q = q->nxtset)
	{	if (strcmp(buf1, q->setname) == 0
		||  strcmp(buf2, q->setname) == 0)
			return 1;
	}
	return 0;
}

struct QSETS *
findset(which)
{	struct QSETS *hook = frstset;
	int i;

	if (which >= nrsets || which < 0)
		whoops("cannot happen - findset");

	for (i = 0; i < which; i++)
		hook = hook->nxtset;

	return hook;
}

inqset(which, what, usage, qind)
	char *what;
{
	struct QSETS   *hook  = findset(which);
	struct QSET_EL *hookl = hook->elmnts;
	int i;

	for (i = 0; i < hook->nrelmnts; i++)
	{	if (strcmp(what, hookl->name) == 0)
		{	hookl->status |= usage;
			if (hookl->id == -1)
				whoops("unnumbered parameter");
			if (qind != hook->qind)
				yyerror("queue indexing error, qset, %s", what);
			return hookl->id;
		}
		hookl = hookl->nxtel;
	}

	return -1;
}

renumqset(which, N)
{ struct QSETS * hook = findset(which);
  struct QSET_EL * hookl = hook->elmnts;
  int i, j;

	qsetowner(which, 0);
	for (i = 0, j = N; i < hook->nrelmnts; i++)
	{	hookl->id = j++;
		hookl = hookl->nxtel;
	}
	return j;
}

matchset(a, b)
{ struct QSETS * one = findset(a);
  struct QSETS * two = findset(b);
  struct QSET_EL * onel;
  struct QSET_EL * twol;
  int i;

	if (one->nrelmnts != two->nrelmnts)
	{	yyerror("qset does not match formal parameter, %s", one->setname);
		return;
	}
	onel = one->elmnts;
	twol = two->elmnts;

	for (i = 0; i < one->nrelmnts; i++)
	{	onel->status |= twol->status;
		onel = onel->nxtel;
		twol = twol->nxtel;
	}
	return two->qstatus;
}

matchowner(a, str, how, qind)
	char *str;
{ struct QSETS * hook = findset(a);

	if (strcmp(hook->setowner, str) == 0)
	{	hook->qstatus |= how;
		if (qind != hook->qind)
			yyerror("queue indexing error (qset ref), %s", str);
		return 1;
	}
	return 0;
}

struct QSET_EL *
mkelmnt(str, how)
	char *str;
{ struct QSET_EL *try = (struct QSET_EL *)
		Emalloc( sizeof( struct QSET_EL ) );

	strcpy(try->name, str);
	try->nxtel = NULL;
	try->status = how;
	try->id = -1;

	return try;
}

struct QSETS *
mkset(str1, str2, how, qind)
	char *str1, *str2;
{ struct QSET_EL *hook;
  struct QSETS *try = (struct QSETS *)
		Emalloc( sizeof( struct QSETS ) );

	if (how == RFR)
		yyerror("undeclared qset, %s", str1);

	try->status = how;
	try->qstatus = 0;
	try->nxtset = NULL;
	strcpy(try->setname,  str1);
	strcpy(try->setowner, str2);
	try->closed = 0;
	try->qind = qind;

	try->nrelmnts = 0;
	try->elmnts = mkelmnt(" tau", SAR);
	try->nrelmnts++;

	hook = try->elmnts;
	hook->nxtel = mkelmnt(" any", SND);
	try->nrelmnts++;

	nrsets++;
	return try;
}

qsetowner(which, how)
{	struct QSETS *hook = findset(which);
	return newqname(hook->setowner, how, NONE, hook->qind);
}

inset(str, str2, mask, how, qind)
	char *str, *str2;
{ struct QSETS *hook = frstset;
  struct QSETS *last = frstset;
  int i;
	for (i = 0; i < nrsets && hook != NULL; i++)
	{	if (strcmp(hook->setname, str) == 0)
		{	if (mask == DCL)
				yyerror("qset redeclared, %s", str);
			hook->status |= mask;
			break;
		}
		last = hook;
		hook = hook->nxtset;
	}
	if (how == ADD && i == nrsets)
		last->nxtset = mkset(str, str2, mask, qind);

	return i;
}

newqset(str1, str2, mask, qind)
	char *str1, *str2;
{
	int i = nrsets;
	char str[MAXNAME];

	if (pid != NONE)
		strncpy(str, procname, 8);
	else if (rid != NONE)
		strncpy(str, refname, 8);
	else
		strcpy(str, "global");
	strcat(str, ":");
	strcat(str, str1);

	if (mask == RFR && (pid != NONE || rid != NONE))
	{	if (inset(str, str2, mask, LOOK, qind) == nrsets)
		{	strcpy(str, "global");
			strcat(str, ":");
			strcat(str, str1);
	}	}

	if (nrsets == 0)
		frstset = mkset(str, str2, mask, qind);
	else
		i = inset(str, str2, mask, ADD, qind);

	return i;
}

closeqset(which)
{	struct QSETS *where = findset(which);
	where->closed = 1;
}

isclosed(which)
{	struct QSETS *where = findset(which);
	return where->closed;
}

addsetname(str, which, susp)
	char *str;
{ struct QSETS   *where = findset(which);
  struct QSET_EL *hook  = where->elmnts;
  int i, j;

	j = newqname(where->setowner, 0, NONE, where->qind);
	i = where->nrelmnts;

	if (i == 0)
		where->elmnts = mkelmnt(str, 0);
	else
	{	for (i = 1; i < where->nrelmnts; i++)
			hook = hook->nxtel;

		hook->nxtel = mkelmnt(str, 0);
	}

	/* if setowner is a formal parameter name
	 * the returned value `j' refers to the qset
	 * in which it was defined + an offset `MANY'
	 * if so, we must check that the message added
	 * here was also defined in the original set
	 */

	if (susp && j >= MANY && isclosed(j - MANY))	/* current set is open */
		addmsg(str, j, 0, NORM, where->qind);	/* check validity msg  */

	where->nrelmnts++;

	return i;
}

callist(which, hit)
{ int i, j;
  struct QSETS   * hook = findset(which);
  struct QSET_EL * kooh = hook->elmnts;

	for (i = 0; i < hook->nrelmnts; i++)
	{	j = addmsg(kooh->name, hit, kooh->status, NORM, hook->qind);
		callentry(ISM, j);
		kooh = kooh->nxtel;
	}
}
