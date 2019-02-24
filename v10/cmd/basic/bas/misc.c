#include <stdio.h>
#include "ctype.h"
#include "typedef.h"
#include "basic.h"
#include "tokens.h"

double	popfloat(), cvtnumber();
Linep	findline();
Lnr	cvtlnr();
char	*getsvar();
Symptr	getvar();


/*
 * moredatastmt --- check for more data before reading
 */

moredatastmt()
{

	register char	*p;
	register Linep	s;

		/* if we have found a data line, move past
		any spaces */

		if (data.k_un.k_gosub.g_inptr != NULL)
			while (*data.k_un.k_gosub.g_inptr == ' ')
				++data.k_un.k_gosub.g_inptr;

		/* if we have not found a data line yet or
		we are at the end of a data line, see if there
		is another data line   */

		if (data.k_un.k_gosub.g_inptr == NULL  ||
				*data.k_un.k_gosub.g_inptr == 0) {
			for (s = data.k_un.k_gosub.g_curline; 
					isline(s); s = nextline(s)) {
				p = s->l_line;
				if (*p++ == DATA) {
					data.k_un.k_gosub.g_inptr = p;
					data.k_un.k_gosub.g_curline 
								= nextline(s);
					return(1);
					}
				}
			return(0);
		}
		else
			return(1);



}



/*
 * ifstmt --- interpret an IF statement
 */

ifstmt()
{
	Lnr		line1;
	int		s;


	if (endtest()) badsyn();  /* check for end of statement */

	while (*inptr == ' ' || *inptr == TAB)
		++inptr;

	/* check for  moredata  token */

	if (*inptr == MORE) {
		++inptr;
		while (*inptr == ' ' || *inptr == TAB)
			++inptr;

		/* 
		 *  we've got a more so let's check for a unit(file) number 
		 */

		if (endtest()) badsyn();  /* check for end of statement */

		if (*inptr == SHARP)
			s = morefiledata();

		/* 
		 *  found a # so it's a file.  go do morefiledata() in io.c 
		 */

		else
			s = moredatastmt();

		/* 
		 *  no # so it's a check for more data in data statements 
		 */

		if (s != 0)
			goto thendo;
		else
			goto elsedo;
		}
	else {

		/* 
		 *  it's not a check for moredata, so usual routine 
		 */

		if (fexpr() != 0) {
			thendo:
			while (*inptr == ' ' || *inptr == TAB)
				++inptr;

			if (endtest()) 
				badsyn();
				  /* check for end of statement */

			if (*inptr == THEN)
				++inptr;

			if (endtest()) 
				badsyn();
				  /* check for end of statement */

			if (isdigit(*inptr)) {
				line1 = cvtlnr();
				curline = findline(line1, EXACTLNR);
				inptr = NULL;
				}
			}
		else {
				/* move past then actions and look for
					end of line or an else		*/
			elsedo:
			while (*inptr && *inptr != ELSE)
				++inptr;
	
				/* if we are not at the end of the line
					then we must have found an else	*/
	
			if (*inptr)
				++inptr;
				/* if not at end of line, must be an else.
					so, move past it.	*/

			if (!endtest()) {
				while (*inptr == ' ' || *inptr == TAB)
					++inptr;
				if (isdigit(*inptr)) {
					line1 = cvtlnr();
					endchk();
					curline = findline(line1, EXACTLNR);
					inptr = NULL;
					}
				}
				
			else {

			/* we were at the end of line so no else  */

				curline = nextline(curline);
				inptr = NULL;
				}
			}
		}
}


/*
 * gotostmt --- interpret a GOTO statement
 */

gotostmt()
{
	register Lnr	line1;

	line1 = cvtlnr();
	endchk();
	curline = findline(line1, EXACTLNR);
	inptr = NULL;
}


/*
 * forstmt --- interpret a FOR statement
 */

forstmt()
{
	register Symptr v;
	Stkfr		f;
	int		type;

	f.k_len = FORFRLEN;
	f.k_type = STK_FOR;
	f.k_un.k_frk.frk_symp = v = getvar(&type, NO);
	if (type != FLOAT)	
		typeerr();
	expectc(EQ);
	if (SINGLE)
		v->v_un.v_float = fexpr();
	else
		v->v_un.v_double = fexpr();
	expectc(TO);
	f.k_un.k_frk.frk_last = fexpr();
	if (*inptr == STEP) {
		++inptr;
		f.k_un.k_frk.frk_incr = fexpr();
		}
	else
		f.k_un.k_frk.frk_incr = 1.0;
	endchk();
	f.k_un.k_frk.frk_inptr = inptr;
	f.k_un.k_frk.frk_curline = curline;
	push(&f);
}


/*
 * nextstmt --- interpret a NEXT statement
 */

nextstmt()
{
	register Stkptr s;
	register Symptr v;
	int		type;
	double		f;

	s = (Stkptr)stkptr;
	if (s->k_type != STK_FOR)
		err("for missing");
	v = s->k_un.k_frk.frk_symp;
 	while (!endtest() && getvar(&type, NO) != v) {
 		pop(STK_FOR);
 		s = (Stkptr)stkptr;
 		if (s->k_type != STK_FOR)
 			err("for missing");
 		v = s->k_un.k_frk.frk_symp;
		}
	endchk();
	if (SINGLE)
		f = v->v_un.v_float += s->k_un.k_frk.frk_incr;
	else
		f = v->v_un.v_double += s->k_un.k_frk.frk_incr;
	if (s->k_un.k_frk.frk_incr > 0)
		if (f > s->k_un.k_frk.frk_last + .00000000000001)
			pop(STK_FOR);
		else {
			inptr = s->k_un.k_frk.frk_inptr;
			curline = s->k_un.k_frk.frk_curline;
			}
	else
		if (f <  s->k_un.k_frk.frk_last)
			pop(STK_FOR);
		else {
			inptr = s->k_un.k_frk.frk_inptr;
			curline = s->k_un.k_frk.frk_curline;
			}
}


/*
 * typeerr --- report a data type error
 */

typeerr()
{

	err("invalid type");
}


/*
 * readdata --- interpret a READ statement
 */

readdata()
{
	register char	*v;
	char		*ptr;
	int		type, len;

	while (!endtest()) {
		v = getsvar(&type);
		optional(askdelims);
		if (data.k_un.k_gosub.g_inptr == NULL)
			getdata();
		while (*data.k_un.k_gosub.g_inptr == ' ')
			++data.k_un.k_gosub.g_inptr;
		if (*data.k_un.k_gosub.g_inptr == 0)
			getdata();
		cvtdata(v, type, &data.k_un.k_gosub.g_inptr);
		}
}


/*
 * getdata --- scan input lines for DATA statements
 */

getdata()
{
	register char	*p;
	register Linep	s;

	for (s = data.k_un.k_gosub.g_curline; isline(s); s = nextline(s)) {
		p = s->l_line;
		if (*p++ == DATA) {
			data.k_un.k_gosub.g_inptr = p;
			data.k_un.k_gosub.g_curline = nextline(s);
			return;
			}
		}
	err("out of data");
}


/*
 * cvtdata --- convert one data item from a DATA statement
 */

cvtdata(vp, type, ptr)
char	**ptr, *vp;
{
	register char	*v;
	char		*p;

	v = vp;
	p = *ptr;
	while (*p == ' ')
		++p;
	switch(type) {
	case INT:
		*(int *)v = cvtnumber(&p, MAXINT);
		break;
	case FLOAT:
		if (SINGLE)
			*(float *)v = cvtnumber(&p, MAXINT);
		else
			*(double *)v = cvtnumber(&p, MAXINT);
		break;
	case STRING:
		cvtstring(&p);
		storestring(v);
		break;
	default:
		badtype();
		}
	while (*p == ' ')
		++p;
	if (*p)
		if (*p == ',')
			++p;
		else
			err("bad input");
	*ptr = p;
}


/*
 * cvtstring --- convert a string element of a DATA statement
 */

cvtstring(cvtptr)
char	**cvtptr;
{
	register int	c;
	register char	*p, *ptr;
	int		len;

	p = *cvtptr;
	if (*p == '"' || *p == '\'')
		c = *p++;
	else
		c = ',';
	ptr = p;
	while (*p && *p != c)
		++p;
	len = p - ptr;
	if (*p && *p != ',')
		++p;
	*cvtptr = p;
	pushstring(ptr, len);
}


/*
 * ongoto --- interpret an ON statement
 */

ongoto()
{
	register int	l, s;
	register Lnr	lnr;

	l = fexpr();
	s = *inptr++;
	if (s != GOTO && s != GOSUB)
		badsyn();
	if (l <= 0)
		err("on index less than one");
	while (!endtest()) {
		lnr = cvtlnr();
		if (--l == 0) {
			if (s == GOSUB) {
				while (!endtest())
					++inptr;
				gosub.k_un.k_gosub.g_curline = curline;
				gosub.k_un.k_gosub.g_inptr = inptr;
				gosub.k_type = STK_GOSUB;
				gosub.k_len = GOSUBFRLEN;
				push(&gosub);
				}
			curline = findline(lnr, EXACTLNR);
			inptr = NULL;
			return;
			}
		if (*inptr == COMMA)
			++inptr;
		else
			break;
		}
	err("on index too big");
}


/*
 * hgr ---
 */

hgr()
{

	pltcls();
	pltini(NULL);
}


/*
 * hplot ---
 */

hplot()
{
	register int	x, y;

	do {
		x = fexpr();
		expectc(COMMA);
		y = fexpr();
		plot(x, y);
		pendn();
		} while (*inptr == TO && ++inptr);
	penup();
}
