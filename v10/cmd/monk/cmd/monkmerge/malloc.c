/*
 * Copyright (c) 1982, 1983, 1984, and 1985
 * by Thaddeus Julius Kowalski
 * 
 * This software is furnished under a license and may be used and
 * copied only in accordance with the terms of such license and with
 * the inclusion of the above copyright notice.  This software or
 * any other copies thereof may not be provided or otherwise made
 * available to any other persons.  No title to and ownership of the
 * software is hereby transfered.
 * 
 * The information in this software is subject to change without
 * notice and should not be construed as a commitment by Thaddeus
 * Julius Kowalski.  Thaddeus Julius Kowalski assumes no
 * responsibility for the use or reliability of this software.
 */

#define MSTATS

/*
 *  This is a very fast storage allocator.  It allocates blocks of a small 
 *  number of different sizes, and keeps free lists of each size.  Blocks that
 *  don't exactly fit are passed up to the next larger size.  In this 
 *  implementation, the available sizes are (2^n)-4 (or -12) bytes long.
 *  This is designed for use in a program that uses vast quantities of memory,
 *  but bombs when it runs out.  To make it a little better, it warns the
 *  user when he starts to get near the end.
 */

/* nextf[i] is the pointer to the next free block of size 2^(i+3).  The
 * smallest allocatable block is 8 bytes.  The overhead information will
 * go in the first int of the block, and the returned pointer will point
 * to the second.
 *
#ifdef MSTATS
 * nmalloc[i] is the difference between the number of mallocs and frees
 * for a given block size.
#endif MSTATS
 */

#define IN_USE		((char)0xff)
#define SIZES		30

static unsigned int *nextf[SIZES];


#ifdef MSTATS
static unsigned int nmalloc[SIZES];
#include "stdio.h"
#endif MSTATS

#ifdef debug
#define ASSERT(p)   if (!(p))botch("p"); else
botch(s)
char *s;
{
	printf("assertion botched: %s\n", s);
	abort();
}
#else
#define ASSERT(p)
#endif

extern char etext;           /* end of the program */
#define NULL 0

#ifdef rcheck
#define MAGIC 0x55555555
#endif

/*      The overhead on a block will be four bytes long.  When free, it will
 *  contain a pointer to the next free block, and the bottom two bits must
 *  be zero.  When in use, the first byte will be set to IN_USE, and the second
 *  byte will be the size index.  The other two bytes are only used for
 *  alignment.  If you are range checking, and the size of the block will fit
 *  into two bytes, then the top two bytes hold the size of the requested block
 *  plus the range checking words, and the header word MINUS ONE.
 */

static *morecore(nu)    /* ask system for more memory */
register int nu;        /* size index to get more of  */
{   char *sbrk();
    register unsigned int *cp;
    register int rnu;       /* 2^rnu bytes will be requested */
    register int nblks;     /* that becomes nblks blocks of the desired size */
    register int siz;       /* size in ints, not bytes */
    static int warnlevel=0;
    register int used;

    if (nextf[nu]!=NULL)
	return;

    cp=((unsigned int *)sbrk(0));
    used=(int)cp;
    used-= (int)&etext;
    if ((((int)cp)&0x3ff) != 0)       /* land on 1K boundaries */
	sbrk(1024-(((int)cp)&0x3ff));

    rnu=(nu<=8)?11:nu+3;    /* take 2k unless the block is bigger than that */
    nblks=1<<(rnu-(nu+3));  /* how many blocks to get */
    if (rnu<nu) rnu=nu;
    if ((int)(cp=(unsigned int*)sbrk(1<<rnu)) == -1)      /* no more room! */
	return;
    if ((((int)cp) & 7)!=0){
	cp=(unsigned int*)((((int)cp)+8)&~7);
	nblks--;}
    nextf[nu]=cp;
    siz= 1<<(nu+1);
    while (--nblks>0){
	((unsigned int**)cp)[0]= &cp[siz];
	cp= (unsigned int*)&cp[siz];}
}

char *malloc(nbytes)    /* get a block */
register unsigned nbytes;
{
    register unsigned char *p;
    register int nunits=0;
    register unsigned shiftr;

#ifdef rcheck
    nbytes+=12;     /* make sure the range checkers will fit */
#else
    nbytes+=4;      /* add on for the overhead */
#endif
    nbytes=(nbytes+3)&~3;       /* round up, but still measure in bytes */
    shiftr=(nbytes-1)>>2;
    while ((shiftr>>=1)!=0)     /* apart from this loop, this is O(1) */
	nunits++;
    if (nextf[nunits]==NULL)    /* needed block, nunits is the size index */
	morecore(nunits);
    if ((p=(unsigned char*)(nextf[nunits]))==NULL)
	return(NULL);
    nextf[nunits]= (unsigned int*)*nextf[nunits];
    p[0]=IN_USE;;
    p[1]=nunits;
#ifdef MSTATS
    nmalloc[nunits]++;
#endif MSTATS
#ifdef rcheck
    if (nbytes<=0x10000)
	((unsigned short*)p)[1]=(unsigned short)nbytes-1;
    *((int*)(p+4))=MAGIC;
    *((int*)(p+nbytes-4))=MAGIC;
    return((char*)(p+8));
#else
    return((char*)(p+4));
#endif
}

free(ap)
register char *ap;
{   register int si;

    if (ap==NULL)
	return;
#ifdef rcheck
    ap-=4;
    ASSERT(*(int*)ap==MAGIC);
#endif
    ap-=4;                              /* point back to overhead word */
#ifdef debug
    ASSERT(ap[0]==IN_USE);                /* make sure it was in use */
#else
    if (ap[0]!=IN_USE)
	return;
#endif
#ifdef rcheck
    if (ap[1]<=13){
	si=((unsigned short *)ap)[1]-11;  /* get the size of the data */
	ASSERT(*((int*)(ap+si+8))==MAGIC);  /* check for overflow */
    }
#endif
    ASSERT(ap[1]<SIZES);
    si=ap[1];
    *((unsigned int**)ap)=nextf[si];
    nextf[si]=(unsigned int*)ap;
#ifdef MSTATS
    nmalloc[si]--;
#endif MSTATS
}

char *realloc(p, nbytes)
register char *p; register unsigned nbytes;
{   register char *res;
    register unsigned int onb;

    if (p==NULL)
	return(malloc(nbytes));
#ifdef rcheck
    if (p[-7]<13)
	onb= ((unsigned short*)p)[-3]-11;  /* old number of data bytes only */
    else
	onb=(1<<(p[-7]+3))-12;
#else
    onb=(1<<(p[-3]+3))-4;
#endif
    if ((res=malloc(nbytes))==NULL)
	return(NULL);
    copymem((nbytes<onb)?(int)nbytes:(int)onb,p,res);
    free(p);
    return(res);
}

copymem(n, from, to)
int n;
register char * from, * to; /* to and from are used by the movc3! */
{
    register int i;

    while (n > 0)
    {
	i = n > 65535L ? 65535L : n;
	asm("	movc3	r9,(r11),(r10)");	/* glug! */
	n -= i;
	from += i;
	to += i;
#ifdef lint
	printf("Use %d and %d to make lint happy\n", from, to);
#endif
    }
}

#ifdef MSTATS
/* ****************************************************************
 * mstats - print out statistics about malloc
 * 
 * Prints two lines of numbers, one showing the length of the free list
 * for each size category, the second showing the number of mallocs -
 * frees for each size category.
 */

mstats(s)
char *s;
{
    register int i, j;
    register unsigned int * p;
    int totfree = 0,
        totused = 0;

    fprintf(stderr, "Memory allocation statistics %s\nfree:\t", s);
    for (i=0; i<SIZES; i++)
    {
	for (j=0, p=nextf[i]; p; p = (unsigned int *)*p, j++)
	    ;
	fprintf(stderr, " %d", j);
	totfree += j * (1 << (i+3));
    }
    fprintf(stderr, "\nused:\t");
    for (i=0; i<SIZES; i++)
    {
	fprintf(stderr, " %d", nmalloc[i]);
	totused += nmalloc[i] * (1 << (i+3));
    }
    fprintf(stderr, "\n\tTotal in use: %d, total free: %d\n", totused, totfree);
}
#else
mstats()
{					/* dummy to keep people happy */
}
#endif
