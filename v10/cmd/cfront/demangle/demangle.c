/*ident	"@(#)ctrans:demangler/demangle.c	1.1"*/
/*
 * C++ Demangler Source Code
 * @(#)master	1.5
 * 7/27/88 13:54:37
 */
#include "String.h"
#include <ctype.h>

/* The variable "hold" contains the pointer to the
 * array initially handed to demangle.  It is returned
 * if it is not possible to demangle the string.  Thus
 * one can do the following:
 *
 * char *mn = "Some mangled name";
 * char *dm = mangle(mn);
 * if(dm == mn)
 * 		printf("name could not be demangled\n");
 * else
 *		printf("demangled name is: %s\n",dm);
 */
static char *hold;

/* this String is the working buffer for the demangle
 * routine.  A pointer into this String is returned
 * from demangle when it is possible to demangle the
 * String.  For this reason, the pointer should not
 * be saved between calls of demangle(), nor freed.
 */
static String *s = 0;

static int
getint(c)
char **c;
{
	int n = 0;
	while(isdigit(**c)) {
		n = n * 10 + (**c) - '0';
		(*c)++;
	}
	return n;
}

/* If a mangled name has a __
 * that is not at the very beginning
 * of the string, then this routine
 * is called to demangle that part
 * of the name.  All overloaded functions,
 * and class members fall into this category.
 */
static char *
second(c)
char *c;
{
	int n;
	if(strncmp(c,"__",2))
		return hold;
	c += 2;

	if(!(isdigit(*c) || *c == 'F'))
		return hold;

	/* a member? */
	if(isdigit(*c)) {
		int ln;
		n = getint(&c);
		if(n == 0)
			return hold;
		s = prep_String("::",s);
		ln = strlen(PTR(s));
		s = nprep_String(c,s,n);
		if(ln + n != strlen(PTR(s)))
			return hold;
		c += n;
	}

	/* an overloaded function? */
	if(*c == 'F') {
		if(c[1] == 0)
			return hold;
		if(strcmp(c,"Fv")==0)
			s = app_String(s,"()");
		else if(demangle_doargs(&s,c+1) < 0)
			return hold;
	}
	return PTR(s);
}

char *
demangle(c)
char *c;
{
	register int i = 0;
	hold = c;
	s = mk_String(s);
	s = set_String(s,"");

	if(c == 0 || *c == 0)
		return hold;

	if(strncmp(c,"__",2) == 0) {
		char *x;
		c += 2;

		/* For automatic variables, or internal static
		 * variables, a __(number) is prepended to the
		 * name.  If this is encountered, strip this off
		 * and return.
		 */
		if(isdigit(*c)) {
			while(isdigit(*c))
				c++;
			return c;
		}

		/* Handle operator functions -- this
		 * automatically calls second, since
		 * all operator functions are overloaded.
		 */
		if(x = findop(c)) {
			s = app_String(s,"operator");
			s = app_String(s,x);
			c += oplen;
			return second(c);
		}

		/* Operator cast does not fit the mould
		 * of the other operators.  Its type name
		 * is encoded.  The cast function must
		 * take a void as an argument.
		 */
		if(strncmp(c,"op",2) == 0) {
			int r;
			s = app_String(s,"operator ");
			c += 2;
			r = demangle_doarg(&s,c);
			if(r < 0)
				return hold;
			c += r;
			return second(c);
		}

		/* Constructors and Destructors are also
		 * a special case of operator name.  Note
		 * that the destructor, while overloaded,
		 * must always take the same arguments --
		 * none.
		 */
		if(strncmp(c,"ct__",4) == 0 || strncmp(c,"dt__",4) == 0) {
			int n;
			char *c2 = c+2;
			char x = c[0];
			c += 4;
			n = getint(&c);
			if(n == 0)
				return hold;
			if(x == 'd' && strcmp(c+n,"Fv"))
				return hold;
			s = napp_String(s,c,n);
			if(x == 'd') {
				s = app_String(s,"::~");
				s = napp_String(s,c,n);
				s = app_String(s,"()");
				return PTR(s);
			} else {
				return second(c2);
			}
		}

		return hold;
	}

	/* If a name does not begin with a __
	 * but it does contain one, it is either
	 * a member or an overloaded function.
	 */
	while(c[i] && strncmp(c+i,"__",2))
		i++;
	while(c[i] && strncmp(1+c+i,"__",2) == 0)
		i++;

	if(strncmp(c+i,"__",2) == 0) {
		s = napp_String(s,c,i);
		return second(c+i);
	} else
		return hold;
}
