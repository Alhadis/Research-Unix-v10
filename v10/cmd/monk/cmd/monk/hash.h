/*	@(#)hash.h	1.1 - pulled from modified hsearch.c	*/

#define MAX_TABLE	10
#define SUCCEED		0
#define FAIL		1
#define TRUE		1
#define FALSE		0
#define repeat		for(;;)
#define until(A)	if(A) break;

#ifdef OPEN
#    undef CHAINED
#else
#ifndef CHAINED
#    define OPEN
#endif
#endif

#ifdef MULT
#    undef DIV
#else
#ifndef DIV
#    define MULT
#endif
#endif

#ifdef START
#    undef SORTUP
#    undef SORTDOWN
#else
#ifdef SORTUP
#    undef SORTDOWN
#endif
#endif

#ifdef USCR
#    define COMPARE(A, B) (* hcompar)((A), (B))
     extern int (* hcompar)();
#else
#    define COMPARE(A, B) strcmp((A), (B))
#endif

#ifdef MULT
#    define SHIFT(tn) ((bitsper * sizeof(int)) - m[tn]) /* Shift factor */
#    define FACTOR 035761254233	/* Magic multiplication factor */
#    define HASH hashm		/* Multiplicative hash function */
#    define HASH2 hash2m	/* Secondary hash function */
static unsigned int bitsper;	/* Bits per byte */
static unsigned int hashm();
static unsigned int hash2m();
#else
#ifdef DIV
#    define HASH hashd		/* Division hashing routine */
#    define HASH2(A) 1		/* Secondary hash function */
static unsigned int hashd();
#endif
#endif

typedef char *POINTER;

#ifdef CHAINED
typedef struct node {	/* Part of the linked list of entries */
	ENTRY item;
	struct node *next;
} NODE;
typedef NODE *TABELEM;
static NODE **table[MAX_TABLE];	/* The address of the hash tables */
static ENTRY *build();
#else
#ifdef OPEN
typedef ENTRY TABELEM;	/* What the table contains (TABle ELEMents) */
/* static TABELEM *table[MAX_TABLE];	The address of the hash tables */
static ENTRY *table[MAX_TABLE];	/* The address of the hash tables */
static unsigned int count[MAX_TABLE];	/* Number of entries in hash table */
#endif
#endif

static unsigned int length[MAX_TABLE];	/* Size of the hash table */
static unsigned int m[MAX_TABLE];	/* Log base 2 of length */
static unsigned int prcnt[MAX_TABLE];		/* Number of probes this item */

extern void free();
extern int printf(), fprintf();
extern char *malloc(), *calloc(), *strcpy();

int hcreate();
void hdestroy();
ENTRY *hsearch();
static unsigned int crunch();

#ifdef DRIVER
static void hdump();
#endif
