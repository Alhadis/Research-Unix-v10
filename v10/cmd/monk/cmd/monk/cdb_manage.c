#include	<stdio.h>
#include	<ctype.h>
#include	"search.h"
#include	"warn.h"
#include	"rd.h"
#include	"dbcompress.h"

#ifdef CMONK
struct db_buffer	db_struct[B_NUMBER];
#endif

char *
bufmalloc(buf_number, size)
int	buf_number;
unsigned	size;
{
	char	*malloc();

#ifndef CMONK
	char	*p;

#ifdef lint
	fprintf(stderr, "Using buf_number %d to make lint happy\n", buf_number);
#endif
	if ((p = malloc(size)) == (char *) 0)
		fprintf(stderr, "Malloc: cannot allocate %u bytes\n", size);
	return(p);
#else
	char	*buf;

	if (buf_number < 0 || buf_number > B_NUMBER) {
		fprintf(stderr,
			"Buffer %d requested: only %d buffers available\n",
						buf_number, B_NUMBER);
		return((char *) 0);
	}
	if (db_struct[buf_number].top == (char *) 0) {
		/* ZZZZZ - need to realloc, fixing pointers (sub and fix)
			when space buffer all used up */
	/* Health: 4 needs to be 6 on 5.0
			--- need to convert so buffers grow */
		buf = mymalloc((unsigned) SAFESIZ*20);
		db_struct[buf_number].top = db_struct[buf_number].current = buf;
		db_struct[buf_number].bottom = buf + SAFESIZ*20;
	} else
		buf = db_struct[buf_number].current;
	if ((db_struct[buf_number].current += size) <
						db_struct[buf_number].bottom)
		return(buf);
	else {	int buf_size[B_NUMBER];
		fprintf(stderr, "Buffer %d out of space\n", buf_number);
		for (buf_number = 0; buf_number < B_NUMBER; ++buf_number) {
			buf_size[buf_number] = db_struct[buf_number].current
						- db_struct[buf_number].top;
			fprintf(stderr, "\nBuffer %d: size %d of %d, ",
				buf_number, buf_size[buf_number],
				db_struct[buf_number].bottom
						- db_struct[buf_number].top);
			fprintf(stderr, "requested %d", size);
		}
			fprintf(stderr, "\n");
		exit(-1);
	}
	return((char *) 0);
#endif
}

buffree(pointer, number)
char	*pointer;
int	number;
{
#ifndef CMONK
	if (pointer == (char *) 0 || pointer == (char *) EOF) {
		fprintf(stderr, "Free %d: no block to free\n", number);
		return;
	}
	free(pointer);
#endif
#ifdef lint
	pointer = NULL;
	number = 0;
#endif
}

char *
r_buftoken(file, space)
FILE	*file;
int	space;
{
	int	i;
	register char	*p;
	char	*token;
	char	temp[BUFSIZ];

	if (space == OK_SPACE) {	
		while ((i = mygetc(file)) != EOF && isspace(i));
		myungetc(i, file);
	}
	for (p = temp; (i = mygetc(file)) != EOF && isokchar(i); )
		 *p++ = i;
	myungetc(i, file);
	if (p == temp)
		if (i == EOF)
			return((char *) EOF);
		else
			return((char *) 0);
	*p = '\0';
	token = bufmalloc(B_TEXT, (unsigned) strlen(temp)+1);
	strcpy(token, temp);
	return(token);
}

char *
r_bufarg(file, delims, outer_delim)
FILE	*file;
short	delims;
char	outer_delim;
{
	int	i;
	register char	*p;
	char	close_delim;
	char	*token;
	char	temp[SAFESIZ];
	
	while ((i = mygetc(file)) != EOF && isspace(i));
	myungetc(i, file);
	/* should be in loop, not special case out here */
	if (i == outer_delim)
		return((char *) 0);
	if (delims != TRUE || (close_delim = read_delim(file, TRUE))
							!= '"') {
		for (p=temp; (i=mygetc(file))!=EOF && isargchar(i);)
		 	*p++ = i;
		if (i == EOF)
			return((char *) EOF);
		if (!isspace(i))
			myungetc(i, file);
		if (p == temp)
			return((char *) 0);
		*p = '\0';
		p = temp;
	} else {
		p = read_until(file, close_delim);
		if (p == (char *) 0 || p == (char *) EOF)
			return(p);
	}
	token = bufmalloc(B_TEXT, (unsigned) strlen(p)+1);
	strcpy(token, p);
	return(token);
}

struct strings *
strarg(s, delims)
register char	*s;
short	delims;
{
	static struct strings	str;
	int	length;
	char	close_delim;
	char	*arg;
	
	for (; *s != '\0' && iswhite(*s); ++s);
	arg = s;
	if (delims != TRUE ||
			(close_delim = close_match(*s)) == (char) FALSE)
		for (;*s != '\0' && isargchar(*s); ++s);
	else
		for (;*s != '\0' && *s != close_delim; ++s);
	if ((length = s - arg) == 0)
		return((struct strings *) 0);
	str.newp = s;
	str.token = bufmalloc(B_TEXT, (unsigned) length+1);
	strncpy(str.token, arg, length);
	str.token[length] = '\0';
	return(&str);
}

void
read_macro(database, name, db_path, close_delim, mesg_mode)
FILE	*database;
char	*name, *db_path;
int	close_delim, mesg_mode;
{
	int	c, inner_close;

	if ((inner_close = read_delim(database, TRUE)) == (char) FALSE) {
		warn(mesg_mode, PR_FILENAME | PR_LINENUMBER,
			"Improper opening delim for %s %s\n", MACRO, name);
		return;
	}
#ifndef CMONK
	while (((c = mygetc(database)) != EOF) && c != inner_close)
		textputc(c, MONK_DB);
#ifdef lint
	fprintf(stderr, "Using db_path %s to make lint happy\n", db_path);
#endif
#else
	/* alternatively could write directly into B_HASH if had a routine
		to give back space to buffer */
	{
	short	count;
	char	buf[BUFSIZ];
	register char	*b, *be, *p;

	b = &buf[0]; be = &buf[BUFSIZ];
	while (((c = mygetc(database)) != EOF) && c != inner_close) {
		*b = c;
		if (++b == be) {
			p = bufmalloc(B_MACRO, BUFSIZ);
			strncpy(p, &buf[0], BUFSIZ);
			b = &buf[0];
		}
	}
	count = b - &buf[0];
	p = bufmalloc(B_MACRO, count);
	strncpy(p, &buf[0], count);
	}
#endif
	if (r_nonblank(database) != close_delim)
		warn(mesg_mode, PR_FILENAME | PR_LINENUMBER,
			"Improper end delim for %s %s\n", MACRO, name);
}
