/* modified to only contain hashenter, hashfind and hsearch */
/*	@(#)hsearch.c	1.2	*/
/*LINTLIBRARY*/
/* Compile time switches:

   MULT - use a multiplicative hashing function.
   DIV - use the remainder mod table size as a hashing function.
   CHAINED - use a linked list to resolve collisions.
   OPEN - use open addressing to resolve collisions.
   BRENT - use Brent's modification to improve the OPEN algorithm.
   SORTUP - CHAINED list is sorted in increasing order.
   SORTDOWN - CHAINED list is sorted in decreasing order.
   START - CHAINED list with entries appended at front.
   DRIVER - compile in a main program to drive the tests.
   DEBUG - compile some debugging printout statements.
   USCR - user supplied comparison routine.
*/

#include "search.h"
#include "hash.h"
#include <stdio.h>

/* comment out the 2 following lines to eliminate debug messages */
/* #define	DEBUG		1 */
/* #define DEBUG_HASH	1         */
/* end of debug facility settings */

ENTRY	*
hashenter(hashtable, enterit)
int	hashtable;
ENTRY	*enterit;
{
	if ((enterit = hsearch(hashtable, *enterit, ENTER)) == (ENTRY *) NULL) {
#ifdef DEBUG_HASH
		fprintf(stderr, "hashenter: cannot enter `%s' in table %d\n",
							enterit->key, hashtable);
#endif
		return((ENTRY *) NULL);
	}
	return(enterit);
}

ENTRY	*
hashfind(hashtable, key)
int	hashtable;
char	*key;
{
	ENTRY	findit;
	ENTRY	*grabit;

	findit.key = key;
	if ((grabit = hsearch(hashtable, findit, FIND)) == (ENTRY *) NULL) {
#ifdef DEBUG_HASH
		fprintf(stderr, "hashfind: cannot find `%s' in table %d\n",
							findit.key, hashtable);
#endif
		return((ENTRY *) NULL);
	}
	return(grabit);
}

#ifdef NOT_DEMONK
isinhash(hashtable, key)
int	hashtable;
char	*key;
{
	ENTRY	findit;

	findit.key = key;
	if (hsearch(hashtable, findit, FIND) == (ENTRY *) NULL)
		return(FALSE);
	return(TRUE);
}

struct table_info	*
gethashtbl(table_number)
int	table_number;
{
	static struct table_info	t;

	t.start = &table[table_number][0];
	t.length = length[table_number];
	return(&t);
}

#ifdef DRIVER

main()
{
    char line[80];	/* Room for the input line */
    int i = 0;		/* Data generator */
    short table_number = 0;	/* Table number */
    ENTRY *res;		/* Result of hsearch */
    ENTRY *new;		/* Test entry */

    if(hcreate(table_number, 5))
	printf("Length = %u, m = %u\n", length[table_number], m[table_number]);
    else {
	fprintf(stderr, "Out of core\n");
	exit(FAIL);
    }

    repeat {
	hdump(table_number);
	printf("Enter a probe: ");
	until (EOF == scanf("%s", line));
#ifdef DEBUG
	printf("%s, ", line);
	printf("division: %d, ", hashd(line));
	printf("multiplication: %d\n", hashm(line));
#endif
	new = (ENTRY *) malloc(sizeof(ENTRY));
	if(new == NULL) {
	    fprintf(stderr, "Out of core \n");
	    exit(FAIL);
	}
	else {
	    new->key = malloc((unsigned) strlen(line) + 1);
	    if(new->key == NULL) {
		fprintf(stderr, "Out of core \n");
		exit(FAIL);
	    }
	    strcpy(new->key, line);
	    new->data = malloc(sizeof(int));
	    if(new->data == NULL) {
		fprintf(stderr, "Out of core \n");
		exit(FAIL);
	    }
	    *new->data = i++;
	}
	res = hsearch(table_number, *new, ENTER);
	printf("The number of probes required was %d\n", prcnt);
	if(res == (ENTRY *) 0)
	    printf("Table %d is full\n", table_number);
	else {
	    printf("Success: ");
	    printf("Key = %s, Value = %d\n", res->key, *res->data);
	}
    }
    exit(SUCCEED);
}
#endif
#endif

int
hcreate(table_number, size)	/* Create a hash table no smaller than size */
short table_number;
int size;		/* Minimum size for hash table */
{
    unsigned int unsize;	/* Holds the shifted size */

    if(size <= 0)
	return(FALSE);

    unsize = size;	/* +1 for empty table slot; -1 for ceiling */
    length[table_number] = 1;		/* Maximum entries in tabbe */
    m[table_number] = 0;		/* Log2 length */
    while(unsize) {
	unsize >>= 1;
	length[table_number] <<= 1;
	m[table_number]++;
    }
#ifdef OPEN
    count[table_number] = 0;
#endif

    table[table_number] =
		(TABELEM *) calloc(length[table_number], sizeof(TABELEM));
    return(table[table_number] != NULL);
}

#ifdef NOT_DEMONK
void
hdestroy(table_number)	/* Reset the module to its initial state */
short table_number;
{
    free((POINTER) table[table_number]);
#ifdef OPEN
    count[table_number] = 0;
#endif
}
#endif

#ifdef OPEN
/* Hash search of a fixed-capacity table.  Open addressing used to
   resolve collisions.  Algorithm modified from Knuth, Volume 3,
   section 6.4, algorithm D.  Labels flag corresponding actions.
*/

ENTRY
*hsearch(table_number, item, action)	/* Find or insert item into table */
short table_number;
ENTRY item;		/* Item to be inserted or found */
ACTION action;		/* FIND or ENTER */
{
    unsigned int i;	/* Insertion index */
    unsigned int c;	/* Secondary probe displacement */

    prcnt[table_number] = 1;

/* D1: */ 
    i = HASH(table_number, item.key);	/* Primary hash on key */
#ifdef DEBUG
    if(action == ENTER) {
	printf("hash = %o\n", i);
    } 
#endif

/* D2: */
    if(table[table_number][i].key == NULL) {	/* Empty slot? */
	goto D6;
    }
    else {
       if(COMPARE(table[table_number][i].key, item.key) == 0) {	/* Match? */
	return(&table[table_number][i]);
       }
     }

/* D3: */
    c = HASH2(table_number, item.key);	/* No match => compute secondary hash */
#ifdef DEBUG
    if(action == ENTER) {
	printf("hash2 = %o\n", c);
    }
#endif

D4: 
    i = (i + c) % length[table_number];	/* Advance to next slot */
    prcnt[table_number]++;

/* D5: */
    if(table[table_number][i].key == NULL)	/* Empty slot? */
	goto D6;
    else if(COMPARE(table[table_number][i].key, item.key) == 0)	/* Match? */
	return(&table[table_number][i]);
    else
	goto D4;

D6: if(action == FIND)		/* Insert if requested */
	return((ENTRY *) NULL);
    if(count[table_number] == (length[table_number] - 1)) /* Table full? */
	return((ENTRY *) 0);

#ifdef BRENT
/* Brent's variation of the open addressing algorithm.  Do extra
   work during insertion to speed retrieval.  May require switching
   of previously placed items.  Adapted from Knuth, Volume 3, section
   4.6 and Brent's article in CACM, volume 10, #2, February 1973.
*/

    {   unsigned int p0 = HASH(table_number, item.key);   /* First probe index */
	unsigned int c0 = HASH2(table_number, item.key);	/* Main branch increment */
						/* Current minimum distance */
	unsigned int r = prcnt[table_number] - 1;
	unsigned int j;         /* Counts along main branch */
	unsigned int k;         /* Counts along secondary branch */
	unsigned int curj;      /* Current best main branch site */
	unsigned int curpos;    /* Current best table index */
	unsigned int pj;        /* Main branch indices */
	unsigned int cj;        /* Secondary branch increment distance*/
	unsigned int pjk;       /* Secondary branch probe indices */

	if(prcnt[table_number] >= 3) {
					/* Count along main branch */
	    for(j = 0; j < prcnt[table_number]; j++) {
					/* New main branch index */
		pj = (p0 + j * c0) % length[table_number];
					/* Secondary branch incr. */
		cj = HASH2(table_number, table[table_number][pj].key);
		for(k=1; j+k <= r; k++) { /* Count on secondary branch*/
					/* Secondary probe */
		    pjk = (pj + k * cj) % length[table_number];
					/* Improvement found */
		    if(table[table_number][pjk].key == NULL) {
		        r = j + k;	/* Decrement upper bound */
		        curj = pj;	/* Save main probe index */
		        curpos = pjk;	/* Save secondeary index */
		    }
		}
	    }
	    if(r != prcnt[table_number] - 1) { /* If an improvement occurred */
		table[table_number][curpos] = table[table_number][curj];
				 /* Old key to new site */
#ifdef DEBUG
		printf("Switch curpos = %o, curj = %o, oldi = %o\n",
		    curj, curpos, i);
		fprintf(stderr,"hsearch: Switch curpos = %o, curj = %o, oldi = %o\n",
		    curj, curpos, i);
#endif
		i = curj;
	    }
	}
    }
#endif
    count[table_number]++;		/* Increment table occupancy count */
    table[table_number][i] = item;	/* Save item */
    return(&table[table_number][i]);	/* Address of item is returned */
}
#endif

#ifdef NOT_DEMONK
#ifdef USCR
#    ifdef DRIVER
static int
compare(a, b)
POINTER a;
POINTER b;
{
    return(strcmp(a, b));
}

int (* hcompar)() = compare;
#    endif
#endif

#ifdef CHAINED
#    ifdef SORTUP
#        define STRCMP(A, B) (COMPARE((A), (B)) > 0)
#    else
#    ifdef SORTDOWN
#        define STRCMP(A, B) (COMPARE((A), (B)) < 0)
#    else
#        define STRCMP(A, B) (COMPARE((A), (B)) != 0)
#    endif
#    endif

ENTRY
*hsearch(table_number, item, action)	/* Chained search with sorted lists */
short table_number;
ENTRY item;		/* Item to be inserted or found */
ACTION action;		/* FIND or ENTER */
{
    NODE *p;		/* Searches through the linked list */
    NODE **q;		/* Where to store the pointer to a new NODE */
    unsigned int i;	/* Result of hash */
    int res;		/* Result of string comparison */

    prcnt[table_number] = 1;

    i = HASH(table_number, item.key);	/* Table[i] contains list head */

    if(table[table_number][i] == (NODE*)NULL) { /* List has not yet been begun */
	if(action == FIND)
	    return((ENTRY *) NULL);
	else
	    return(build(&table[table_number][i], (NODE *) NULL, item));
    }
    else {			/* List is not empty */
	q = &table[table_number][i];
	p = table[table_number][i];
	while(p != NULL && (res = STRCMP(item.key, p->item.key))) {
	    prcnt[table_number]++;
	    q = &(p->next);
	    p = p->next;
	}

	if(p != NULL && res == 0)	/* Item has been found */
	    return(&(p->item));
	else {			/* Item is not yet on list */
	    if(action == FIND)
		return((ENTRY *) NULL);
	    else
#ifdef START
		return(build(&table[table_number][i], table[table_number][i],
									item));
#else
		return(build(q, p, item));
#endif
	}
    }
}

static ENTRY
*build(last, next, item)
NODE **last;		/* Where to store in last list item */
NODE *next;		/* Link to next list item */
ENTRY item;		/* Item to be kept in node */
{
    NODE *p = (NODE *) malloc(sizeof(NODE));

    if(p != NULL) {
	p->item = item;
	*last = p;
	p->next = next;
	return(&(p->item));
    }
    else
	return(NULL);
}
#endif
#endif

#ifdef DIV
static unsigned int
hashd(table_number, key)		/* Division hashing scheme */
short table_number;
POINTER key;		/* Key to be hashed */
{
    return(crunch(key) % length[table_number]);
}
#else
#ifdef MULT
/*
    NOTE: The following algorithm only works on machines where
    the results of multiplying two integers is the least
    significant part of the double word integer required to hold
    the result.  It is adapted from Knuth, Volume 3, section 6.4.
*/

static unsigned int
hashm(table_number, key)		/* Multiplication hashing scheme */
short table_number;
POINTER key;		/* Key to be hashed */
{
    static int first = TRUE;	/* TRUE on the first call only */

    if(first) {		/* Compute the number of bits in a byte */
	unsigned char c = ~0;	/* A byte full of 1's */
	bitsper = 0;
	while(c) {		/* Shift until no more 1's */
	    c >>= 1;
	    bitsper++;		/* Count number of shifts */
	}
	first = FALSE;
    }
    return((int) (((unsigned) (crunch(key) * FACTOR)) >> SHIFT(table_number)));
}

/*
 * Secondary hashing, for use with multiplicitive hashing scheme.
 * Adapted from Knuth, Volume 3, section 6.4.
 */

static unsigned int
hash2m(table_number, key)		/* Secondary hashing routine */
short table_number;
POINTER key;		/* String to be hashed */
{
    return((int) (((unsigned) ((crunch(key) * FACTOR) << m[table_number])
						>> SHIFT(table_number)) | 1));
}
#endif
#endif

static unsigned int
crunch(key)		/* Convert multicharacter key to unsigned int */
POINTER key;
{
    unsigned int sum = 0;	/* Results */
    int s;			/* Length of the key */

    for(s = 0; *key; s++)	/* Simply add up the bytes */
	sum += *key++;

    return(sum + s);
}

#ifdef NOT_DEMONK
#ifdef DRIVER
static void
hdump(table_number)			/* Dumps loc, data, probe count, key */
short table_number;
{
    unsigned int i;	/* Counts table slots */
#ifdef OPEN
    unsigned int sum = 0;	/* Counts probes */
#else
#ifdef CHAINED
    NODE *a;		/* Current Node on list */
#endif
#endif

    for(i = 0; i < length[table_number]; i++)
#ifdef OPEN
	if(table[table_number][i].key == NULL)
	    printf("%o.\t-,\t-,\t(NULL)\n", i);
	else {
					/* Save current probe count */
	    unsigned int oldpr = prcnt[table_number];
	    hsearch(table[table_number][i], FIND);
	    sum += prcnt[table_number];
	    printf("%o.\t%d,\t%d,\t%s\n", i, *table[table_number][i].data,
		prcnt[table_number], table[table_number][i].key);
	    prcnt[table_number] = oldpr;
	}
    printf("Total probes = %d\n", sum);
#else
#ifdef CHAINED
	if(table[table_number][i] == NULL)
	    printf("%o.\t-,\t-,\t(NULL)\n", i);
	else {
	    printf("%o.", i);
	    for(a = table[table_number][i]; a != NULL; a = a->next)
		printf("\t%d,\t%#0.4x,\t%s\n",
		    *a->item.data, a, a->item.key);
	}
#endif
#endif
}
#endif
#endif
