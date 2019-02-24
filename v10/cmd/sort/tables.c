/* Copyright 1990, AT&T Bell Labs */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "fsort.h"

/* STACK=1000 makes lots of room under normal conditions.
   But it can be broken.  40 cycles like this will do it:
	a,b,...z,za,zb,...zz,zza,zzb,...,zzz,zzza,...
   Minor changes to rsort() would allow the stack to be
   extended discontiguously.  Pity we don't have alloca any more.
*/

#define BUFFER 6000000
#define MINBUF 10000	/* not worth trying */
#define STACK 1000

uchar *bufmax;
struct rec *buffer;
unsigned long bufsiz = BUFFER;
struct rec endfile;
struct list *stack;
struct list *stackmax;

uchar ident[256];	/* identity transform */
uchar fold[256];	/* fold upper case to lower */

uchar all[256];		/* all chars significant */
uchar dict[256];	/* sig chars for dictionary order */
uchar ascii[256];	/* ascii graphics significant */

void
tabinit(void)
{
	int i;
	memset((char*)all,1,256);

	memset((char*)(dict+'0'),1,10);
	memset((char*)(dict+'A'),1,26);
	memset((char*)(dict+'a'),1,26);
	dict[' '] = dict['\t'] = 1;

	memset((char*)(ascii+040),1,0137); /* 040-0176 */

	for(i=0; i<256; i++)
		fold[i] = ident[i] = i;
	for(i='a'; i<='z'; i++)
		fold[i] += 'A' - 'a';

	stack = (struct list*)malloc(STACK*sizeof(*stack));
	do {
		if((buffer=(struct rec*)malloc(bufsiz)) != 0)
			break;
	} while((bufsiz/=2) > MINBUF);
	if(buffer==0 || stack==0)
		fatal("can't get working space", "", 0);
	bufmax = (uchar*)buffer + bufsiz - 2*sizeof(*buffer);
	stack->head = stack->tail = 0;
	stackmax = stack + STACK;
}

void
tabfree(void)
{
	free(stack);
	free(buffer);
}

void
optiony(char *s)
{
	long size = atol(s);
	if(!isdigit(s[0]))
		fatal("no size for -y","",0);
	if(size >= MINBUF/10)	/* tiny helps debugging */
		bufsiz = size;
	else if(size == 0)
		bufsiz = 10L*BUFFER;
	else fatal("-y too small", "", 0);
}
