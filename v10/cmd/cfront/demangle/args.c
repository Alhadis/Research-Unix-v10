/*ident	"@(#)ctrans:demangler/args.c	1.1"*/
/*
 * C++ Demangler Source Code
 * @(#)master	1.5
 * 7/27/88 13:54:37
 */
#include "String.h"
#include <ctype.h>

/* This structure is used to keep
 * track of pointers to argument
 * descriptions in the mangled string.
 * This is needed for N and T encodings
 * to work.
 */
typedef struct {
	char *list[10];
	int pos;
} Place;

Place here;

/* Strings and flags needed by
 * the argument demangles.
 */
typedef struct {
	String *arg,*ptr;
	int Sign,Uns,Cons,Vol;
} Arg_Remem;

/* initialize Arg_Remem */
static void
mkar(r)
Arg_Remem *r;
{
	r->arg = mk_String(0);
	r->ptr = mk_String(0);
	r->Sign = r->Uns = r->Cons = r->Vol = 0;
}

/* free data for Arg_Remem */
static void
delar(r)
Arg_Remem *r;
{
	free_String(r->ptr);
	free_String(r->arg);
}

/* This routine formats a single argument
 * on the buffer sptr.
 */
static void
setarg(sptr,r,c)
String **sptr;
Arg_Remem *r;
char *c;
{
	if(r->Cons)
		r->arg = prep_String("const ",r->arg);
	if(r->Vol)
		r->arg = prep_String("volatile ",r->arg);
	if(r->Uns)
		r->arg = prep_String("unsigned ",r->arg);
	if(r->Sign)
		r->arg = prep_String("signed ",r->arg);
	r->arg = app_String(r->arg,c);
	r->arg = app_String(r->arg,PTR(r->ptr));
	(*sptr) = app_String(*sptr,PTR(r->arg));
	delar(r);
}


/* Demangle a single function argument */
int
demangle_doarg(sptr,c)
String **sptr;
char *c;
{
	register int i;
	Arg_Remem ar;
	mkar(&ar);

	if(here.pos < 10 && here.pos >= 0)
		here.list[here.pos++] = c;

	for(i=0;c[i];i++) {
		switch(c[i]) {

		case 'T':
			{
				Place tmp;
				tmp = here;
				here.pos = c[1] - '1';
				if(here.pos < 0 || here.pos >= tmp.pos-1) {
					delar(&ar);
					return -1;
				}
				demangle_doarg(sptr,here.list[here.pos]);
				here = tmp;
				delar(&ar);
				return 2;
			}
		case 'N':
			{
				Place tmp;
				int cycles,pos;
				cycles = c[1] - '0'; pos = c[2] - '1';
				tmp = here;
				if(cycles < 1 || cycles > 9 || pos < 0 || pos >= tmp.pos-1) {
					delar(&ar);
					return -1;
				}
				while(cycles--) {
					here = tmp;
					here.pos = pos;
					demangle_doarg(sptr,here.list[here.pos]);
					(*sptr) = app_String(*sptr,",");
				}
				(*sptr)->data[--(*sptr)->sg.end] = '\0';
				here = tmp;
				delar(&ar);
				return 3;
			}

		/* Class encodings */
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			{
				int n = 0,cnt = 0;
				while(isdigit(c[i+n])) {
					cnt = cnt * 10 + c[i+n] - '0';
					n++;
				}
				i += n;
				if(strlen(c+i) < cnt) {
					delar(&ar);
					return -1;
				}
				ar.arg = napp_String(ar.arg,c+i,cnt);
				i += cnt;
				setarg(sptr,&ar,"");
				return i;
			}

		/* Qualifiers to type names */
		case 'S':
			ar.Sign++;
			break;
		case 'U':
			ar.Uns++;
			break;
		case 'C':
			ar.Cons++;
			break;
		case 'V':
			ar.Vol++;
			break;

		/* Pointers, references, and Member Pointers */
		case 'P':
		case 'R':
		case 'M':
			if(ar.Cons) {
				ar.ptr = prep_String(" const",ar.ptr);
				ar.Cons = 0;
			}
			if(ar.Vol) {
				ar.ptr = prep_String(" volatile",ar.ptr);
				ar.Vol = 0;
			}
			if(c[i] == 'P')
				ar.ptr = prep_String("*",ar.ptr);
			else if(c[i] == 'R')
				ar.ptr = prep_String("&",ar.ptr);
			else {
				int n = 1,cnt = 0;
				ar.ptr = prep_String("::*",ar.ptr);
				while(isdigit(c[i+n])) {
					cnt = cnt * 10 + c[i+n] - '0';
					n++;
				}
				i += n;
				ar.ptr = nprep_String(c+i,ar.ptr,cnt);
				ar.ptr = prep_String(" ",ar.ptr);
				i += cnt;
				i--;
			}
			break;

		/* Demangle for basic built-in types */
		case 'i':
			setarg(sptr,&ar,"int");
			return ++i;
		case 'c':
			setarg(sptr,&ar,"char");
			return ++i;
		case 's':
			setarg(sptr,&ar,"short");
			return ++i;
		case 'l':
			setarg(sptr,&ar,"long");
			return ++i;
		case 'f':
			setarg(sptr,&ar,"float");
			return ++i;
		case 'd':
			setarg(sptr,&ar,"double");
			return ++i;
		case 'r':
			setarg(sptr,&ar,"long double");
			return ++i;

		/* Ellipsis and void */
		case 'e':
			(*sptr) = app_String(*sptr,"...");
			delar(&ar);
			if(ar.Cons || ar.Vol || ar.Uns || ar.Sign)
				return -1;
			return ++i;
		case 'v':
			(*sptr) = app_String(*sptr,"void");
			(*sptr) = app_String(*sptr,PTR(ar.ptr));
			delar(&ar);
			if(ar.Cons || ar.Vol || ar.Uns || ar.Sign)
				return -1;
			return ++i;

		/* Arrays */
		case 'A':
			if(*PTR(ar.ptr)) {
				ar.ptr = prep_String("(",ar.ptr);
				ar.ptr = app_String(ar.ptr,")");
			}
			ar.ptr = app_String(ar.ptr,"[");
			{
				int cnt = 0;
				i++;
				while(isdigit(c[i+cnt]))
					cnt++;
				ar.ptr = napp_String(ar.ptr,c+i,cnt);
				i += cnt;
				if(c[i] != '_') {
					delar(&ar);
					return -1;
				}
			}
			ar.ptr = app_String(ar.ptr,"]");
			break;

		/* Functins
		 * This will always be called as a pointer
		 * to a function.
		 */
		case 'F':
			ar.ptr = prep_String("(",ar.ptr);
			ar.ptr = app_String(ar.ptr,")");
			{
				Place tmp;
				tmp = here;
				i++;
				i += demangle_doargs(&ar.ptr,c+i);
				if(c[i] != '_') {
					delar(&ar);
					return -1;
				}
				here = tmp;
			}
			break;

		/* Needed when this is called to demangle
		 * an argument of a pointer to a function.
		 */
		case '_':
			delar(&ar);
			return 0;

		default:
			delar(&ar);
			return -1;
		}
	}

	/* Did the argument list terminate properly? */
	{
		int rc = 0;
		if(*PTR(ar.ptr) || ar.Uns || ar.Sign || ar.Cons || ar.Vol)
			rc = -1;
		delar(&ar);
		return rc;
	}
}

/* This function is called to demangle
 * an argument list.
 */
int
demangle_doargs(sptr,c)
String **sptr;
char *c;
{
	int i,n = 0;
	here.pos = 0;

	(*sptr) = app_String(*sptr,"(");
	while(*c && (i = demangle_doarg(sptr,c)) > 0) {
		c += i;
		n += i;
		(*sptr) = app_String(*sptr,",");
	}

	if(i < 0)
		return -1;

	(*sptr)->data[(*sptr)->sg.end - 1] = ')';
	return n;
}
