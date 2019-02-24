/*	"@(#) strspace.c: V 1.10 8/19/83"	*/

/*
*	strspace.c: functions and data used to manage the bottom
*		    of txtbuf which is used as heap storage for strings 
*/

#include	<bas.h>

#include	<stdio.h>

extern char	*hicore;

#define		MAXFREE		50

/* freelist structure */
struct freerec {
		char	*frptr;		/* pointer to free space */
		short	size;		/* size of free space    */
	       } freelist[MAXFREE];

char	nullstrng = '\0';

struct freerec	*topptr = freelist,		/* top of freelist array */
		*botptr = freelist + MAXFREE - 1,/* bottom of freelist array */
		*nextfree;			/* next unused freelist record*/


/*
*	The freelist is kept sorted by location (largest addresses at the
*	bottom).  Space is allocated first fit starting from the bottom of
*	the list, hicore is bumped if there is no space large enough on the
*	freelist.  When a space is freed, it is merged with an adjacent 
*	freelist entry if one exists, otherwise a hole is opened and it
*	is inserted into the list.
*/


/*
*	cover up a removed entry on the freelist
*/

static
close(recptr)
struct freerec 	*recptr;
{
	struct freerec	*ptr;

	ptr = recptr + 1;
	while (ptr < nextfree)
	{
		(ptr-1)->frptr = ptr->frptr;
		(ptr-1)->size = ptr->size;
		ptr++;
	}
	nextfree--;
}


/*
*	open up a hole for new list entry
*/

static
open(recptr)
struct freerec	*recptr;
{
	struct freerec	*ptr;

	ptr = nextfree;
	if (nextfree < botptr)
		nextfree++;
	while (ptr > recptr)
	{ 
		ptr->frptr = (ptr-1)->frptr;
		ptr->size  = (ptr-1)->size;
		ptr--;
	}
}


/*
*	add free space to tail of list entry if possible
*/

joinabove(recptr, ptr, size)
struct freerec	*recptr;
char		*ptr;
int		size;
{
	if ((recptr->frptr + recptr->size) == ptr)
	{
		recptr->size += size;
		return(1);
	}
	return(0);
}


/*
*	add space in front of list entry if possible
*/

joinbelow(recptr, ptr, size)
struct freerec	*recptr;
char		*ptr;
int		size;
{
	if ((ptr + size) == recptr->frptr)
	{
		recptr->frptr = ptr;
		recptr->size  += size;
		return(1);
	}
	return(0);
}


/*
*	add top entry on list to available workspace if possible
*/

downcore()
{
	if ((nextfree > topptr) && (topptr->frptr == hicore))
		{
		hicore += topptr->size;
		close(topptr);
		return(1);
	}
	return(0);
}


/*
*	get a free space of size size, return NULL if none is available
*/

char	*
getfree(size)
int	size;
{
	struct freerec	*cand;		/*candidate space in freelist*/
	char		*retptr;

	cand = nextfree - 1;
	while (cand >= topptr)  	/*scan freelist from bottom*/
	{
		if (cand->size == size)
		{
			retptr = cand->frptr;
			close(cand);
			return(retptr);
		} else if (cand->size > size) {
			retptr = cand->frptr + (cand->size - size);
			cand->size -= size;
			return(retptr);
		} else
			cand--;
	}
	/* space was not found on freelist, allocate from top of heap */
	if ((hicore - size) < endofprog + ENCLINMAX)
	{
		error(inst.thing.linno,inst.thing.stmtno, 24);	/* Workspace Full */
		return(NULL);
	}
	else
	{
		hicore -= size;
		return(hicore);
	}
}


/*
*	insert string into string space and return pointer
*/

char 	*
strget(string)
char	*string;
{
	char	*loc;
	int	size;

	if (*string == '\0') 
		return(&nullstrng);
	size = strlen(string) + 1;
	if ((loc = getfree(size)) == NULL)
		return(NULL);
	strcpy(loc, string);
	return(loc);
}


/*
*	free up space pointed to by ptr
*/

strfree(ptr)
char	*ptr;
{
	struct freerec	*slot;
	int		size;

	if (ptr == &nullstrng || ptr == NULL)
		return(0);
	if (nextfree == topptr)		/* freelist empty */
	{
		nextfree->frptr = ptr;
		nextfree->size	= strlen(ptr) + 1;
		nextfree++;
		return(0);
	}
	size = strlen(ptr) + 1;
	slot = nextfree - 1;
	if (ptr > slot->frptr)		/* space comes after any on freelist */
	{
		if (!joinabove(slot, ptr, size))
		{
			nextfree->frptr = ptr;
			nextfree->size	= size;
			if (nextfree < botptr)
				nextfree++;
		}
		return(0);
	}
	slot--;
	while (slot >= topptr)
		if (ptr > slot->frptr)	/*find place in list for new space*/
		{
			if (joinabove(slot, ptr, size))	/*can merge above*/
			{
				if (joinabove(slot, (slot+1)->frptr,
						    (slot+1)->size))
					/* space is missing piece between
					   two free list entries          */
					close(slot+1);
				return(0);
			}
			else if (joinbelow(slot+1, ptr, size))/*can merge below*/
				return(0);
			else		/* new freelist entry required */
			{
				open(slot+1);
				(slot+1)->frptr = ptr;
				(slot+1)->size  = size;
				return(0);
			}
		}
		else
			slot--;

	/* space comes before first freelist entry */
	if (!joinbelow(topptr, ptr, size))
	{
		open(topptr);
		topptr->frptr = ptr;
		topptr->size  = size;
	}
	downcore();
	return(0);
}


/*
*	initialize freelist
*/

freeinit()
{
	nextfree = topptr;
}


dmpstrspace(msg)
char	msg[];
{
	char	*ptr,
		*last;

	printf("Dump Of String Space %s\n", msg);
	last = txtbuf + PGSIZ * NMPAG + ENCLINMAX;
	for (ptr=hicore; ptr<=last; ptr++)
	{
		if (*ptr)
			printf("%c", *ptr);
		else
			printf("X");
		if ((ptr-last) % 79 == 0)
			printf("\n");
	}
	printf("\n");
}
