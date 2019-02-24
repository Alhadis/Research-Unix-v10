/*	"@(#) prnt_using.c: V 1.5 6/28/83"	*/


#include <stdio.h>
#include "bas.h"

short fmttype;	/*Holds the type of the current format*/
char *fmt;	/*Pointer to the beginning of the format string*/
char *curr;	/*Pointer to the next character in the format
		 *string to be decoded*/

/*fmttype can hold the following values:
 *	STRING 0	(defined elsewhere)
 */
#define NUMBER	1	
#define LITERAL	2
#define EMPTY	3

#define DIGIT	24 /* Total number of digits allowed to be
			specified in a format */


typedef	struct {
                int ndigits,
                    decpt,
                    decimal,
                    comma,
                    dollar,
                    asterisk,
                    leading,    /* + */
                    trailing,   /* + */
                    minus,      /* trailing - */
                    exp;
                } digits;

/*The format string is decoded (one format item at a time)
 *and stored in the following structure.
 *This is referred to as the "format structure" in the following comments.
 */
union	{
	char	lit[LINMAX+1];	/*literal*/
	int	str;	/*refers format to sprintf a string*/
	digits	number;
	} format;

digits zeros = {0,0,0,0,0,0,0,0,0,0};

/* string formats use the following to sprintf a string */
#define FMT0	"%.1s"
#define FMT1	"%s"
#define FMT2	"%%-.%ds"

extern char *delimcpy();
extern char *strcpy();
extern short	startpos,
		margin;

getno(ptr)
char **ptr;
{
/* The format decoding function (crackfmt) calls this function
 * to decode a number format.
 * INPUT: ptr points to a pointer to the format string.
 * OUTPUT: the format structure is filled in with information
 *	   about the format--number of digits (before & after the decimal pt)
 *			     decimal point included,
 *			     exponent included,
 *			     commas included.
 *	   The format is checked for the total number of digits allowed
 *	   in a format (DIGIT) and produces an error message if appropriate.
 *	   Ptr updates the format pointer to point to the next item in
 *	   the format string.
*/

char *next;
int i;
int nodgts;
nodgts = 0;
next = *ptr;
while(*next == '#') {format.number.decpt++; next++;}
if(*next == ',') {format.number.comma++; next++;}
if(*next == '.') {format.number.decimal++; next++;}
while(*next == '#') {nodgts++; next++;}
while(*next == '^') {format.number.exp++; next++;}
if(format.number.exp != 0)

/* NOTICE: the following assumes that this BASIC is to be released on a VAX
 * and on other machines which use the IEEE floating point format standard.
 * If other machines are included this ifdef must change.
 */
#ifdef vax
	if(format.number.exp != 4)
	       {error(inst.thing.linno, inst.thing.stmtno,96);
		return(-1);
	       }
#else	/* machines using IEEE floating point format, 3B20, 3B5/2 */
	if(!((format.number.exp == 4) || (format.number.exp == 5)))
	       {error(inst.thing.linno,inst.thing.stmtno,96);
		return(-1);
	       }
#endif
	
format.number.decpt = format.number.decpt + 2*format.number.asterisk +
	format.number.dollar;
if(format.number.dollar && format.number.asterisk) format.number.decpt--;

format.number.ndigits = format.number.decpt + nodgts;

/* The total number of digits places allowed is 24 (which includes places
 * for the decimal point and dollar sign.)  
 */
nodgts = format.number.ndigits + format.number.decimal + format.number.dollar;

if(nodgts > DIGIT)
               {error(inst.thing.linno,inst.thing.stmtno,96);
		return(-1);
	       }
*ptr = next;
return(0);
}


#define isfmtchar(c)\
       ( (c == '+') || (c == '-') || (c == '*') || (c == '$') ||\
	 (c == '#') || (c == '.') || (c == '!') || (c == '&') ||\
	 (c == '\\') || (c == '\0') )


char *
cktrail(str)
char *str;
{
/* This function is called by the format decoding functions.
 * It checks for a trailing + or - in the format.
 * INPUT: str points to the next character in the format.
 * OUTPUT: The format structure is updated for a trailing sign
 *	   or a trailing minus sign for negative numbers.
 *	   A pointer to the next character in the format string is
 *	   returned.
*/
if (*str == '+')
       {str++;
	format.number.trailing++;
       }
if (*str == '-')
       {str++;
	format.number.minus++;
       }
return(str);
}

char *
literal(str)
char *str;
{
/* This function is called by the format decoding function.
 * It collects a string of literal characters in a format string.
 * It stops when a valid format charater is encountered (or a NULL).
 * INPUT: str points to the next character in the format string.
 * OUTPUT: The literal string is stored in the format structure.
 *	   A pointer to the next character in the format string is returned.
 */ 
char *ptr;
ptr = format.lit;
while( !isfmtchar(*str))
       {if(*str == '_')
	       {str++;
		if(str != '\0') *ptr++ = *str++;
	       }
	else    *ptr++ = *str++;
       }
*ptr = '\0';
return(str);
}

extern char *ecvt();

numcvt(number, buf, fit, sign, exprbuf)
double	number;
char	*buf;
int	*fit;
int	*sign;
char	*exprbuf;
{
/* This function is called by the print_using driver to convert a double
 * precision number to ASCII characters for printing.
 * INPUT: Information from the format structure is used to determine
 *	  the appearance of the number--number of digits to print,
 *	  exponent, decimal point, etc.
 *	  number is the double precision value to be converted.
 *	  exprbuf holds the orginal ASCII argument to print using
 *		from the BASIC program.  This is used in case the number
 * 		is too large for the specified format.  In this case
 *		the function used by the print statement is called
 *		(asc_con) to complete the number conversion.
 * OUTPUT:buf holds the final ASCII representation of the input number
 *	  fit indicates whether or the number fit into the specified
 *		format (1 if number is too large, 0 otherwise).
 *	  sign returns zero if the number is postive, otherwise the
 *		number is negative.
 */

int	ndigit,
	decpt;
int	done;
register int i,
	     fmtdecpt;
register char *p1, *p2 = buf;

decpt = 0;
*fit = 0;
p1 = ecvt(number,format.number.ndigits,&decpt,sign);
ndigit = format.number.ndigits;
fmtdecpt = format.number.decpt;

if((format.number.exp == 0) && (decpt <= fmtdecpt ))
   						/* Not E-style format */
       {while((fmtdecpt > 0) && (fmtdecpt > decpt))
	       {if(fmtdecpt-- == 1)     *p2++ = '0'; 
		else *p2++ = ' ';
		ndigit--;
	       }
	if(fmtdecpt == 0 && format.number.decimal != 0)	*p2++ = '.';
	while(decpt < 0)
	       {decpt++;
		ndigit--;
		*p2++ = '0';
	       }
	for(i=1; i<=ndigit; i++)
	       {*p2++ = *p1++;
		if((i == decpt) && (format.number.decimal != 0))
			 *p2++ = '.';
	       }
       }

else if(format.number.exp == 0)
       {*fit=1;
	if(asc_con(exprbuf,&number) == -1) return(-1);
	strcpy(buf,exprbuf);
	return(0);
       }
else
       {if(format.number.leading == 0 && format.number.trailing == 0 &&
		     format.number.minus == 0)
	       {*p2++ = ' ';
		ndigit--;
		fmtdecpt--;
	       }
	while(fmtdecpt-- > 0)
	       {ndigit--;
		decpt--;
		*p2++ = *p1++;
	       }
	if(format.number.decimal != 0)
	       {*p2++ = '.';
		for(i=1; i<=ndigit; i++)
			*p2++ = *p1++;
	       }

	*p2++ = 'e';
	if(decpt < 0)
	       {decpt = -decpt;
		*p2++ = '-';
	       }
	else	*p2++ = '+';
	for(i=1000; i!=0; i/=10)
		if(i <= decpt || i <= 10)
			*p2++ = (decpt / i) % 10 + '0';
       }

/* In case too many significant digits were returned from ecvt, round. */
if(*p1 > '4' && *p1 <= '9')
       {done = 0;
	*p2 = '\0';
	if(format.number.exp)
		while(*p2 != 'e')
			*p2--;
	while( !done && (p2 != buf))
	       {p2--;
		if(*p2 != '.')
		       {if(*p2 == '9')
						*p2 = '0';
			else if(*p2 == ' ')
					       {*p2 = '1';
						done = 1;
					       }
			else
					       {*p2 = *p2 + 1;
						done = 1;
					       }
		       }
	       }
	while(*p2 != '\0') p2++;
       }
*p2 = '\0';
return(0);
}


crackfmt()
{
/* This function decodes or cracks the format of a print using statment.
 * The format for one print item is decoded and returned by this function.
 * INPUT: The format string is accessed by the global variables curr and fmt.
 * OUTPUT:The decoded format is stored in the format structure.
 *	  The fmttype variable is also set for the type (string, number,
 *	  literal, or empty) of the format.
 */
char	*next;
int	count;

format.number = zeros;
fmttype = EMPTY;

next = curr;
begin:
	switch(*next++) {
		case '\0':
			if(fmt == curr || *expr == '\0') return(0);
			else   {curr = fmt;
				next = fmt;
				goto begin;
			       }
		case '+':
			if((*next == '*') || (*next == '$') || (*next == '#') ||
			   (*next == '.'))
			       {format.number.leading++;
				goto begin;
			       }
			else   {error(inst.thing.linno,inst.thing.stmtno,96);
				return(-1);
			       }
		case '*':
			if(*next == '*')
			       {fmttype = NUMBER;
				format.number.asterisk++;
				next++;
				if(*next == '$')
				       {format.number.dollar++;
					next++;
				       }
				if(getno(&next) == -1) return(-1);
				next = cktrail(next);
			       }
			else   {error(inst.thing.linno,inst.thing.stmtno,96);
				return(-1);
			       }
			break;
		case '$':
			if(*next == '$')
			       {fmttype = NUMBER;
				format.number.dollar++;
				next++;
				if(getno(&next) == -1) return(-1);
				next = cktrail(next);
			       }
			else   {error(inst.thing.linno,inst.thing.stmtno,96);
				return(-1);
			       }
			break;
		case '#':
			next--;
			fmttype = NUMBER;
			if(getno(&next) == -1) return(-1);
			next = cktrail(next);
			break;
		case '.':
			if(*next == '#')
			       {next--;	
			        fmttype = NUMBER;
				if(getno(&next) == -1) return(-1);
				next = cktrail(next);
			       }
			else   {error(inst.thing.linno,inst.thing.stmtno,96);
				return(-1);
			       }
			break;
		case '!':
			fmttype = STRING;
			format.str = 0;
			break;
		case '&':
			fmttype = STRING;
			format.str = 1;
			break;
		case '\\':
			fmttype = STRING;
			count = 2;
			while(*next == ' ') {count++; next++;}
			if (*next != '\\')	/*not string format after all*/
			       {error(inst.thing.linno,inst.thing.stmtno,96);
				return(-1);
			       }
			else   {next++;
				format.str = count;
			       }
			break;
		case '^':
		       {error(inst.thing.linno,inst.thing.stmtno,96);
			return(-1);
		       }
		default:
			fmttype = LITERAL;
			next = literal(curr);
			if(curr == fmt && *next == '\0') return(1);
			break;
	}
curr = next;
return(0);
}

getitem(exprbuf,quit,sawdelim,formt)
char	*exprbuf;
int	*quit;
int	*sawdelim;
int	formt;
{
/* This function retrieves the next item from the print using statment line.
 * The item might be the format string, or an item to be printed.
 * The item is converted to internal format (eval).
 * INPUT: expr points to the next item in the print using statment of the
 *	  BASIC program.
 *	  formt has the value 1 if getitem is being called to retrieve a
 *		format string; it is 0 otherwise.
 * OUTPUT:exprbuf holds the ASCII characters of the current print item as
 *		they appear in the original BASIC program.
 *	  quit is 1 if the last print item has been retrieved, 0 otherwise.
 *	  sawdelim is 1 if a delimiter (, or ;) was found; 0 otherwise.  
 *		This is used to determine if a newline should be printed
 *		at the end of the print using statement.
 *	  exprtype and exprval contain the type and value of the print
 *		item (as set by eval).
 */
skip00(&expr);
expr = delimcpy(exprbuf,expr,";,");
if(*exprbuf == '\0')
	if(*expr == '\0')
	       {*quit = 1;
		return(0);
	       }
	else   {error(inst.thing.linno, inst.thing.stmtno,8);
		return(-1);
	       }
if(eval(exprbuf) < 0)	return(-1);
if(formt)
       {if(*expr == ';')		       {expr++;
						*sawdelim = 1;
					       }
	else   {error(inst.thing.linno,inst.thing.stmtno,21);
		return(-1);
	       }
	if(exprtype != STRING)
	       {error(inst.thing.linno,inst.thing.stmtno,51);
		return(-1);
	       }
       }
else	if(*expr == ';' || *expr == ',')       {expr++;
						*sawdelim = 1;
					       }
	else *sawdelim = 0;
}

manipstr(tmp,buf,sign,fit)
char	*tmp;
char	*buf;
int	sign,
	fit;
{
/* This function manipulates the ASCII string of a number before is it
 * printed.  A number is converted to an ASCII string of the proper
 * number of characters, then sent to this routine to add the dollar
 * sign, commas, plus or minus sign, etc.
 * INPUT: The format structure is used to determine what must be added to
 *	  or changed in the ASCII string.
 *	  buf contains the ASCII string for the number.
 *	  sign is zero if the number is positive, otherwise the number is
 *		negative.
 *	  fit is 1 if the number is too large to fit into the specified
 *		format, zero otherwise.
 * OUTPUT:tmp holds the final ASCII output after the string has been
 *		completed.
 */

int	i;
char	*p;
if(fit)
       {*tmp++ = '%';
	if(*buf == '-' && format.number.trailing != 0 ||
		format.number.minus != 0)
		buf++;
       }
else if(sign)
	if(format.number.leading == 0 && format.number.trailing == 0 &&
		     format.number.minus == 0)
       		if(*buf != ' ')        {*tmp++ = '%';
					*tmp++ = '-';
				       }

while(*buf == ' ') 
       {
	buf++;
	if(format.number.asterisk)	*tmp++ = '*';
	else				*tmp++ = ' ';
       }
if(sign && !format.number.leading)
	*(tmp-1) = '-';
if(format.number.leading)	*tmp++ = (sign)?'-':'+';
if(format.number.dollar)
       {if(format.number.exp || format.number.leading) 
	       {error(inst.thing.linno,inst.thing.stmtno,96);
		return(-1);
	       }
	else if(sign && !format.number.minus)
	       {error(inst.thing.linno,inst.thing.stmtno,51);
		return(-1);
	       }
	else   *tmp++ = '$';
       }
if(format.number.comma)	       {p = buf;
				i = 0;
				while(*p != '.' && *p != '\0')
				       {p++;
					i++;
				       }
				for(--i; i > 0; i--)
				       {*tmp++ = *buf++;
				        if((i % 3) == 0)
						*tmp++ = ',';
				       }
			       }
while(*buf != '\0')	*tmp++ = *buf++;
if(format.number.trailing)	*tmp++ = (sign)?'-':'+';
if(format.number.minus)		*tmp++ = (sign)?'-':' ';
*tmp = '\0';
}

applyfmt(exprbuf,buf)
char	*exprbuf;
char	*buf;
{
/* Apply the current format to the print item.  The format type and
 * the type of the print item are checked.  If it's a match the
 * print item is converted to ASCII format according to the format
 * specification.
 * INPUT: exprbuf contains the print item as it appeared in the original
 *		BASIC program.
 *	  The format structure contains the format information.
 *	  exprtype and exprval contain the type and value of the print item.
 * OUTPUT:buf contains the ASCII characters which are the result of
 *		applying the format to the print item.
 */
int	fit,
	sign;
char	tmp[LINMAX +1];
int	nochars;
char	strfmt[10];
switch(fmttype) {
	case STRING:	if(exprtype == STRING)
			       {
				if(format.str == 0)
					strcpy(strfmt, FMT0);
				else if(format.str == 1)
					strcpy(strfmt, FMT1);
				else sprintf(strfmt,FMT2,format.str);
				nochars = sprintf(buf,strfmt,exprval.sval);
				while(nochars < format.str)
			    		buf[nochars++] = ' ';
				buf[nochars] = '\0';
			       }
			else   {error(inst.thing.linno,inst.thing.stmtno,51);
				return(-1);
			       }
			break;

	case NUMBER:   {if(exprtype == INTEGER)
			       {if(numcvt((double)exprval.ival,tmp,&fit,&sign,
					exprbuf) == -1) return(-1);
			       }
			else if(exprtype == DOUBLE)
			       {if(numcvt(exprval.dval,tmp,&fit,&sign,
					exprbuf) == -1) return(-1);
			       }
			else if(exprtype == SINGLE)
			       {if(numcvt(exprval.fval,tmp,&fit,&sign,
					exprbuf) == -1) return(-1);
			       }
			else   {error(inst.thing.linno,inst.thing.stmtno,49);
				return(-1);
			       }
			if(manipstr(buf,tmp,sign,fit) == -1) return(-1);
		       }
	}
return(0);
}

prtstr(filptr,startpos,ftblind,buf)
FILE	*filptr;
short	*startpos;
int	ftblind;
char	*buf;
{
/* Print the formatted item.
 * INPUT: filptr, startpos, and ftblind are passed through the print using
 *		functions to cput.
 *	  buf contains the formatted print item.
 */
if(*startpos + strlen(buf) > margin)
	cput('\n',filptr,startpos,ftblind);
while(*buf != '\0')
	cput(*buf++,filptr,startpos,ftblind);
}

prt_using(filptr,startpos,ftblind)
FILE	*filptr;
short	*startpos;
int	ftblind;
{
/* Formatted print statement.
 * A format for a single print item is retrieved.  If it is a literal
 * string the string is printed, otherwise a print item is retrieved.
 * The format is applied to the print item and the result is printed.
 * This is repeated until there are no more print items.
 * Note that the format decoding is not saved even though it may need
 * to be repeated (in case there are more print items than specified
 * in the format)!
 */
char	fbuf[LINMAX+1];
char	strbuf[LINMAX+1];
char	exprbuf[LINMAX+1];
int	quit;
int	sawdelim;
int 	ret;
ret = 0;
sawdelim = 0;
quit = 0;
fmt = curr = fbuf;
if(getitem(exprbuf,&quit,&sawdelim,1) == -1) return(-1);
strcpy(fmt,exprval.sval);

while(!quit)
       {if((ret = crackfmt()) == -1) return(-1);
	if(ret == 1)  quit++;
	switch(fmttype) {
		case EMPTY:
			quit++;
			break;

		case LITERAL:
			prtstr(filptr,startpos,ftblind,format.lit);
			break;

		case STRING:
		case NUMBER:
			if(getitem(exprbuf,&quit,&sawdelim,0) == -1)
				return(-1);
			if(!quit)
			       {
			        if(applyfmt(exprbuf,strbuf) == -1) return(-1);
				if(prtstr(filptr,startpos,ftblind,strbuf) == -1)
					return(-1);
			       }
		}/*end switch*/
       }
if(!sawdelim)
	if(cput('\n',filptr,startpos,ftblind) < 0)	return(-1);
return(0);
}
