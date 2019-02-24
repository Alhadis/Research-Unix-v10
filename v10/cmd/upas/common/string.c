#include <stdio.h>
#include <ctype.h>
#include "mail.h"
#include "string.h"

/* imports */
extern char *malloc();
extern char *realloc();
extern void exit();

/* global to this file */
#define STRLEN 128
#define STRALLOC 128
#define MAXINCR 250000

/* buffer pool for allocating string structures */
typedef struct {
	string s[STRALLOC];
	int o;
} stralloc;
static stralloc *freep=NULL;

/* pool of freed strings */
static string *freed=NULL;

void
s_free(sp)
	string *sp;
{
	if (sp != NULL) {
		sp->ptr = (char *)freed;
		freed = sp;
	}
}

/* allocate a string head */
static string *
s_alloc() {
	if (freep==NULL || freep->o >= STRALLOC) {
		freep = (stralloc *)malloc(sizeof(stralloc));
		if (freep==NULL) {
			perror("allocating string");
			exit(1);
		}
		freep->o = 0;
	}
	return &(freep->s[freep->o++]);
}

/* create a new `short' string */
extern string *
s_new()
{
	string *sp;

	if (freed!=NULL) {
		sp = freed;
		freed = (string *)(freed->ptr);
		sp->ptr = sp->base;
		return sp;
	}
	sp = s_alloc();
	sp->base = sp->ptr = malloc(STRLEN);
	if (sp->base == NULL) {
		perror("allocating string");
		exit(1);
	}
	sp->end = sp->base + STRLEN;
	s_terminate(sp);
	return sp;
}

/* grow a string's allocation by at least `incr' bytes */
extern int
s_simplegrow(sp, incr)	
	string *sp;
	int incr;
{
	char *cp;
	int size;

	/*
	 *  take a larger increment to avoid mallocing too often
	 */
	if((sp->end-sp->base) < incr && MAXINCR < incr)
		size = (sp->end-sp->base) + incr;
	else if((sp->end-sp->base) > MAXINCR)
		size = (sp->end-sp->base)+MAXINCR;
	else
		size = 2*(sp->end-sp->base);

	cp = realloc(sp->base, size);
	if (cp == NULL) {
		perror("string:");
		exit(1);
	}
	sp->ptr = (sp->ptr - sp->base) + cp;
	sp->end = cp + size;
	sp->base = cp;
}

/* grow a string's allocation */
extern int
s_grow(sp, c)
	string *sp;
	int c;
{
	s_simplegrow(sp, 2);
	s_putc(sp, c);
	return c;
}

/* return a string containing a character array (this had better not grow) */
string *
s_array(cp, len)
	char *cp;
	int len;
{
	string *sp = s_alloc();

	sp->base = sp->ptr = cp;
	sp->end = sp->base + len;
	return sp;
}

/* return a string containing a copy of the passed char array */
extern string*
s_copy(cp)
	char *cp;
{
	string *sp;
	int len;

	sp = s_alloc();
	len = strlen(cp)+1;
	sp->base = malloc(len);
	if (sp->base == NULL) {
		perror("string:");
		exit(1);
	}
	sp->end = sp->base + len;	/* point past end of allocation */
	strcpy(sp->base, cp);
	sp->ptr = sp->end - 1;		/* point to NULL terminator */
	return sp;
}

/* convert string to lower case */
extern int
s_tolower(sp)
	string *sp;
{
	char *cp;

	for(cp=sp->ptr; *cp; cp++)
		if(isupper(*cp))
			*cp = tolower(*cp);
}

/* append a char array to a string */
extern string *
s_append(to, from)
	register string *to;
	register char *from;
{
	int want, have;

	if (to == NULL)
		to = s_new();
	if (from == NULL)
		return to;
	for(; *from; from++)
		s_putc(to, *from);
	s_terminate(to);
	return to;
}

/* append a char array ( of up to n characters) to a string */
extern string *
s_nappend(to, from, n)
	register string *to;
	register char *from;
{
	int want, have;

	if (to == NULL)
		to = s_new();
	if (from == NULL)
		return to;
	for(; *from && n; from++, n--)
		s_putc(to, *from);
	s_terminate(to);
	return to;
}

/* Append a logical input sequence into a string.  Ignore blank and
 * comment lines.  Backslash preceding newline indicates continuation.
 * The `lineortoken' variable indicates whether the sequence to beinput
 * is a whitespace delimited token or a whole line.
 *
 * Returns a pointer to the string (or NULL). Trailing newline is stripped off.
 */
extern string *
s_seq_read(fp, to, lineortoken)
	FILE *fp;		/* stream to read from */
	string *to;		/* where to put token */
	int lineortoken;	/* how the sequence terminates */
{
	register int c;
	int done=0;

	if(feof(fp))
		return NULL;

	/* get rid of leading goo */
	do {
		c = getc(fp);
		switch(c) {
		case EOF:
			if (to != NULL)
				s_terminate(to);
			return NULL;
		case '#':
			while((c = getc(fp)) != '\n' && c != EOF);
			break;
		case ' ':
		case '\t':
		case '\n':
		case '\r':
		case '\f':
			break;
		default:
			done = 1;
			break;
		}
	} while (!done);

	if (to == NULL)
		to = s_new();

	/* gather up a sequence */
	for (;;) {
		switch(c) {
		case '\\':
			c = getc(fp);
			if (c != '\n') {
				s_putc(to, '\\');
				s_putc(to, c);
			}
			break;
		case EOF:
		case '\r':
		case '\f':
		case '\n':
			s_terminate(to);
			return to;
		case ' ':
		case '\t':
			if (lineortoken == TOKEN) {
				s_terminate(to);
				return to;
			}
			/* fall through */
		default:
			s_putc(to, c);
			break;
		}
		c = getc(fp);
	}
}

/* Append an input line to a string.
 *
 * Returns a pointer to the string (or NULL).
 * Trailing newline is left on.
 */ 
extern char *
s_read_line(fp, to)
	register FILE *fp;
	register string *to;
{
	register int c;
	register int len=0;

	s_terminate(to);

	/* end of input */
	if (feof(fp) || (c=getc(fp)) == EOF)
		return NULL;

	/* gather up a line */
	for(;;) {
		len++;
		switch(c) {
		case EOF:
			s_terminate(to);
			return to->ptr-len;
		case '\n':
			s_putc(to, '\n');
			s_terminate(to);
			return to->ptr-len;
		default:
			s_putc(to, c);
			break;
		}
		c=getc(fp);
	}
}

/*
 *  Read till eof or some limit is passed.  If the limit is passed,
 *  return a negative counr.
 */
extern int
s_read_to_lim(fp, to, lim)
	register FILE *fp;
	register string *to;
	int lim;
{
	register int c;
	register int got;
	register int have;

	s_terminate(to);

	for(;;){
		if(lim && to->ptr - to->base > lim){
			s_terminate(to);
			return -(to->ptr - to->base);
		}
		if(feof(fp))
			break;
		/* allocate room for a full buffer */
		have = to->end - to->ptr;
		if(have<4096)
			s_simplegrow(to, 4096);

		/* get a buffers worth */
		have = to->end - to->ptr;
		got = fread(to->ptr, 1, have, fp);
		if(got<=0)
			break;
		to->ptr += got;
	}

	/* null terminate the line */
	s_terminate(to);
	return to->ptr - to->base;
}

/*
 *  read to eof
 */
extern int
s_read_to_eof(fp, to)
	register FILE *fp;
	register string *to;
{
	return s_read_to_lim(fp, to, 0);
}

/* Get the next field from a string.  The field is delimited by white space,
 * single or double quotes.
 */
extern string *
s_parse(from, to)
	string *from;	/* string to parse */
	string *to;	/* where to put parsed token */
{
	while (isspace(*from->ptr))
		from->ptr++;
	if (*from->ptr == '\0')
		return NULL;
	if (to == NULL)
		to = s_new();
	if (*from->ptr == '\'') {
		from->ptr++;
		for (;*from->ptr != '\'' && *from->ptr != '\0'; from->ptr++)
			s_putc(to, *from->ptr);
		if (*from->ptr == '\'')	
			from->ptr++;
	} else if (*from->ptr == '"') {
		from->ptr++;
		for (;*from->ptr != '"' && *from->ptr != '\0'; from->ptr++)
			s_putc(to, *from->ptr);
		if (*from->ptr == '"')	
			from->ptr++;
	} else {
		for (;!isspace(*from->ptr) && *from->ptr != '\0'; from->ptr++)
			s_putc(to, *from->ptr);
	}
	s_terminate(to);

	return to;
}

