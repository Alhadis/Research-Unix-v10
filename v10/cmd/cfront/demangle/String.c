/*ident	"@(#)ctrans:demangler/String.c	1.2"*/
/*
 * C++ Demangler Source Code
 * @(#)master	1.5
 * 7/27/88 13:54:37
 */
#include "String.h"
#include <stdio.h>
#include <assert.h>
extern char *malloc();
/* This code emulates the C++ String package
 * in a crude way.
 */

/* This function will expand the space
 * availabe to a String so that more data
 * can be appended to it
 */
static String *
grow(s)
String *s;
{
	int sz = s->sg.max * 2;
	assert(sz > 0);
	s = (String *)realloc(s,sz + sizeof(StringGuts)+1);
	assert(s != 0);
	s->sg.max = sz;
	return s;
}

/* This function will expand the space
 * available to a String so that more data
 * can be prepended to it.
 */
static String *
ror(s,n)
String *s;
int n;
{
	int i;
	assert(s != 0);
	while(s->sg.end + n > s->sg.max)
		s = grow(s);
       for(i = s->sg.end - 1;i >= s->sg.start;i--)
		s->data[i+n] = s->data[i];
	s->sg.end += n;
	s->sg.start += n;
	s->data[s->sg.end] = 0;
	return s;
}

/* This function will prepend c
 * to s
 */
String *
prep_String(c,s)
char *c;
String *s;
{
	int n = strlen(c);
	return nprep_String(c,s,n);
}

/* This function will prepend the
 * first n characters of c to s
 */
String *
nprep_String(c,s,n)
char *c;
String *s;
int n;
{
	int len = strlen(c);
	assert(s != 0);
	if(len > n)
		len = n;
	if(len >= s->sg.start)
		s = ror(s,len);
	while(len--)
		s->data[--(s->sg.start)] = c[len];
	s->data[s->sg.end] = '\0';
	return s;
}

/* This function will append
 * c to s.
 */
String *
app_String(s,c)
String *s;
char *c;
{
	int n = strlen(c);
	s = napp_String(s,c,n);
	return s;
}

/* This function will append the
 * first n characters of c to s
 */
String *
napp_String(s,c,n)
String *s;
char *c;
{
	int len = strlen(c);
	int catlen;
	assert(s != 0);
	if(n < len)
		len = n;
	catlen = s->sg.end + len;
	while(catlen > s->sg.max)
		s = grow(s);
	while(len--)
		s->data[s->sg.end++] = *c++;
	s->data[s->sg.end] = '\0';
	assert(s->sg.end == catlen);
	return s;
}

/* This function initializes a
 * String.  It returns
 * its argument is non-zero.
 * This prevents the same string
 * from being re-initialized.
 */
String *
mk_String(s)
String *s;
{
	if(s)
		return s;
	s = (String *)malloc(STRING_START + sizeof(StringGuts)+1);
	assert(s != 0);
	s->sg.start = s->sg.end = STRING_START/2;
	s->sg.max = STRING_START;
	s->data[s->sg.end] = '\0';
	return s;
}

void
free_String(s)
String *s;
{
	if(s) free(s);
}

/* This function copies
 * c into s
 */
String *
set_String(s,c)
String *s;
char *c;
{
	int len = strlen(c)*2;
	while(len > s->sg.max)
		s = grow(s);
	s->sg.start = s->sg.end = s->sg.max / 2;
	s = app_String(s,c);
	return s;
}
