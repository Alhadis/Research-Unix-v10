/*	"@(#) lex.c: V 1.16 8/19/83"	*/

#include	<setjmp.h>
#include	<ctype.h>
#include	<values.h>
#include	"bas.h"
#include	"tokens.h"

#define OCTNUM(C) ((C) >= '0' && (C) <= '7')

#define HEXNUM(C) (((C) >= '0' && (C) <= '9') || ((C) >= 'a' && (C) <= 'f'))



/* general purpose lexical analyzer for BASIC tokens */
lex(linptr, tokbuf)
char	**linptr,
	*tokbuf;
{
	register char 	*tokptr;
	register int	digflg;
	register int	namelen;

	tokptr = tokbuf;
	*tokptr = '\0';
	skip00(linptr);
	switch ((**linptr) & 0377)
	{
		case '"'	:  (*linptr)++;
				   while (*(*linptr) && !(*(*linptr) == '"'))
					*tokptr++ = *(*linptr)++;
				   *tokptr = '\0';
				   if (*(*linptr) == '"')
				   	(*linptr)++;
				   return(STRCONST);
		case '^'	:  *tokptr++ = *(*linptr)++;
				   *tokptr = '\0';
				   return(LEXP);
		case '-'	:  *tokptr++ = *(*linptr)++;
				   *tokptr = '\0';
				   return(LMINUS);
		case '*'	:  *tokptr++ = *(*linptr)++;
				   if (*(*linptr) == '*')
				   {
					*tokptr++ = *(*linptr)++;
					*tokptr = '\0';
					return(LEXP);
				   }
				   *tokptr = '\0';
				   return(LMULT);
		case '/'	:  *tokptr++ = *(*linptr)++;
				   *tokptr = '\0';
				   return(LDIV);
		case '\\'	:  *tokptr = *(*linptr)++;
				   *tokptr = '\0';
				   return(LIDIV);
		case '+'	:  *tokptr = *(*linptr)++;
				   *tokptr = '\0';
				   return(LPLUS);
		case '('	:  *tokptr = *(*linptr)++;
				   *tokptr = '\0';
				   return(LPAR);
		case ')'	:  *tokptr = *(*linptr)++;
				   *tokptr = '\0';
				   return(RPAR);
		case ','	:  *tokptr = *(*linptr)++;
				   *tokptr = '\0';
				   return(COMMA);
		case '#'	:  *tokptr = *(*linptr)++;
				   *tokptr = '\0';
				   return(LPOUND);
		case ';'	:  *tokptr = *(*linptr)++;
				   *tokptr = '\0';
				   return(LSEMI);
		case '!'	:  *tokptr = *(*linptr)++;
				   *tokptr = '\0';
				   return(LBANG);
		case '='	:  *tokptr++ = *(*linptr)++;
				   if (*(*linptr) == '>')
				   {
					*tokptr++ = *(*linptr)++;
				        *tokptr ='\0';
				        return(LGE);
				   }
				   if (*(*linptr) == '<')
				   {
					*tokptr++ = *(*linptr)++;
				        *tokptr ='\0';
				        return(LLE);
				   }
				   *tokptr = '\0';
				   return(LEQ);
		case '>'	:  *tokptr++ = *(*linptr)++;
				   if (*(*linptr) == '=')
				   {
					*tokptr++ = *(*linptr)++;
				        *tokptr ='\0';
				        return(LGE);
				   }
				   else if (*(*linptr) == '<')
				   {
					*tokptr++ = *(*linptr)++;
				        *tokptr ='\0';
				        return(LNE);
				   }
				   *tokptr = '\0';
				   return(LGT);
		case '<'	:  *tokptr++ = *(*linptr)++;
				   if (*(*linptr) == '>')
				   {
					*tokptr++ = *(*linptr)++;
				        *tokptr ='\0';
				        return(LNE);
				   }
				   else if (*(*linptr) == '=')
				   {
					*tokptr++ = *(*linptr)++;
				        *tokptr ='\0';
				        return(LLE);
				   }
				   *tokptr = '\0';
				   return(LLT);
		case 0000	:  return(0);
		case 0377	:  *tokptr++ = *(*linptr)++;
				   *tokptr++ = *(*linptr)++;
				   *tokptr = '\0';
				   return(FUNCID);
		case ENcode	:
		case NEcode	:  *tokptr++ = *(*linptr)++;
				   *tokptr = '\0';
				   return(LNE);
		case LEcode	:
		case ELcode	:  *tokptr++ = *(*linptr)++;
				   *tokptr = '\0';
				   return(LLE);
		case LTcode	:  *tokptr++ = *(*linptr)++;
				   *tokptr = '\0';
				   return(LLT);
		case GEcode	:
		case EGcode	:  *tokptr++ = *(*linptr)++;
				   *tokptr = '\0';
				   return(LGE);
		case GTcode	:  *tokptr++ = *(*linptr)++;
				   *tokptr = '\0';
				   return(LGT);
		case EQcode	:  *tokptr++ = *(*linptr)++;
				   *tokptr = '\0';
				   return(LEQ);
		case ANDcode	:  *tokptr++ = *(*linptr)++;
				   *tokptr = '\0';
				   return(LAND);
		case EQVcode	:  *tokptr++ = *(*linptr)++;
				   *tokptr = '\0';
				   return(LEQV);
		case IMPcode	:  *tokptr++ = *(*linptr)++;
				   *tokptr = '\0';
				   return(LIMP);
		case MODcode	:  *tokptr++ = *(*linptr)++;
				   *tokptr = '\0';
				   return(LMOD);
		case NOTcode	:  *tokptr++ = *(*linptr)++;
				   *tokptr = '\0';
				   return(LNOT);
		case ORcode	:  *tokptr++ = *(*linptr)++;
				   *tokptr = '\0';
				   return(LOR);
		case XORcode	:  *tokptr++ = *(*linptr)++;
				   *tokptr = '\0';
				   return(LXOR);
	}
	if ((*(*linptr) & 0377) >= 0200)
	{
		*tokptr++ = *(*linptr)++;
		*tokptr = '\0';
		return(TOKERR+1);
	}
	if (ALPHA(*(*linptr)))
	{
		namelen = 1;
		while (ALPHA(**linptr) || NUM(**linptr) || **linptr == '.')
			if (namelen++ <= IDSIZE)
				*tokptr++ = *(*linptr)++;
			else
				(*linptr)++;
		*tokptr = '\0';
		if (*(*linptr) == '%')
		{
			(*linptr)++;
			if (fnpref(tokbuf))
				return(INTUID);
			if (*(*linptr) == '(')
				return(INTAID);
			else
				return(INTID);
		}
		else if (*(*linptr) == '$')
		{
			(*linptr)++;
			if (fnpref(tokbuf))
				return(STRUID);
			if (*(*linptr) == '(')
				return(STRAID);
			else
				return(STRID);
		}
		else if (*(*linptr) == '#')
		{
			(*linptr)++;
			if (fnpref(tokbuf))
				return(DBLUID);
			if (*(*linptr) == '(')
				return(DBLAID);
			else
				return(DBLID);
		}
		else if (*(*linptr) == '!')
		{
			(*linptr)++;
			if (fnpref(tokbuf))
				return(SNGUID);
			if (*(*linptr) == '(')
				return(SNGAID);
			else
				return(SNGID);
		}
		else	/* default type */
		{
			if (fnpref(tokbuf))
				return(defudef(*tokbuf));
			if (*(*linptr) == '(')
				return(defarray(*tokbuf));
			else
				return(defscalar(*tokbuf));
		}
	}
	if (**linptr == '.' && !NUM(*(*linptr + 1)))
	{
		*tokptr++ = *(*linptr)++;
		*tokptr = '\0';
		return(LDOT);
	}
	if (NUM(**linptr) || **linptr == '.')
	{
		digflg = 0;
		while (NUM(**linptr))
		{
			*tokptr++ = *(*linptr)++;
			digflg = 1;
		}
		if (*(*linptr) == '.')
		{
			*tokptr++ = *(*linptr)++;
			while (NUM(**linptr))
			{
				*tokptr++ = *(*linptr)++;
				digflg = 1;
			}
			if (**linptr != 'e' && **linptr != 'E' 
			    && **linptr != 'd' && **linptr != 'D' && digflg)
			{
				*tokptr = '\0';
				if (**linptr == '!')
				{
					(*linptr)++;
					return(SNGCONST);
				}
				else if (**linptr == '#')
				{
					(*linptr)++;
					return(DBLCONST);
				}
				return(FIXCONST);
			}
		}
		if ((**linptr == 'e' || **linptr == 'E') && digflg)
		{
			*tokptr++ = *(*linptr)++;
			if (**linptr == '-' || **linptr == '+')
				*tokptr++ = *(*linptr)++;
			digflg = 0;
			while (NUM(**linptr))
			{
				*tokptr++ = *(*linptr)++;
				digflg = 1;
			}
			*tokptr = '\0';
			if (digflg)
				return(SNGCONST);
			return(TOKERR);
		}
		else if ((**linptr == 'd' || **linptr == 'D') && digflg)
		{
			*tokptr++ = *(*linptr)++;
			if (**linptr == '-' || **linptr == '+')
				*tokptr++ = *(*linptr)++;
			digflg = 0;
			while (NUM(**linptr))
			{
				*tokptr++ = *(*linptr)++;
				digflg = 1;
			}
			*tokptr = '\0';
			if (digflg)
				return(DBLCONST);
			return(TOKERR);
		}
		else if (digflg)
		{
			*tokptr = '\0';
			if (**linptr == '%')
			{
				(*linptr)++;
				return(DECCONST);
			}
			else if (**linptr == '!')
			{
				(*linptr)++;
				return(SNGCONST);
			}
			else if (**linptr == '#')
			{
				(*linptr)++;
				return(DBLCONST);
			}
			return(INTCONST);
		}
	}
	if (**linptr == '&')
	{
		(*linptr)++;
		if (**linptr == 'h')
		{
			(*linptr)++;
			if (!HEXNUM(**linptr))
				return(TOKERR);
			while (HEXNUM(**linptr))
				*tokptr++ = *(*linptr)++;
			*tokptr = '\0';
			return(HEXCONST);
		}
		if (**linptr == 'o')
			(*linptr)++;
		if (!OCTNUM(**linptr))
			return(TOKERR);
		while (OCTNUM(**linptr))
			*tokptr++ = *(*linptr)++;
		*tokptr = '\0';
		return(OCTCONST);
	}
	(*linptr)++;
	*tokptr = '\0';
	return(TOKERR);
}


/*
* true if name is a user-defined function name (i.e. prefixed with "fn" or "FN")
*/
fnpref(name)
char	name[];
{
	if (!strncmp(name, "fn", 2) || !strncmp(name, "FN", 2))
		return(1);
	else
		return(0);
}


/*
*	like lex but puts raw token string in buf
*/
gettoken(ptrptr, buf)
char	**ptrptr,
	buf[];
{
	char	dummybuf[LINMAX+1];
	register char	*from,
			*to;
	int	rettype;

	skip00(ptrptr);
	from = *ptrptr;
	to = buf;
	rettype = lex(ptrptr, dummybuf);
	while (from < *ptrptr)
		*to++ = *from++;
	*to = '\0';
	return(rettype);
}



/*
*	gets an identifier, returns type, and moves pointer
*	past trailing white space.
*/
getid(ptr, buf)
char 	**ptr,
	buf[];
{
	int type;

	switch(type = lex(ptr, buf))
	{
	case DBLID:
	case DBLAID:
	case DBLUID:
	case SNGID:
	case SNGAID:
	case SNGUID:
	case INTID:
	case INTAID:
	case INTUID:
	case STRID:
	case STRAID:
	case STRUID:
		skip00(ptr);
		return(type);
	default:
		error(inst.thing.linno,inst.thing.stmtno, 3);
		return(-1);
	}
}


relop(ttype)
int ttype;
{
	switch(ttype)
	{
	case LLT:
	case LLE:
	case LEQ:
	case LNE:
	case LGE:
	case LGT:
		return(1);
	}
	return(0);
}



extern double	atof1();
extern jmp_buf	overenv;

getnum(ptr, number)
char	**ptr;
double	*number;
{
	char	buf[LINMAX+1];
	int	i,
		sign = 1;

	switch(lex(ptr,buf))
	{
		case LMINUS:
			sign = -1;
		case LPLUS:
			if ((i = lex(ptr,buf)) != INTCONST && i != SNGCONST
					   && i != FIXCONST && i != DBLCONST)
				return(-1);
			if (setjmp(overenv) != 0)
				*number = sign * MAXDOUBLE;
			else if ((*number = (sign * atof1(buf))) == MAXDOUBLE)
				error(inst.thing.linno,inst.thing.stmtno, 60);
			return(0);
		case INTCONST:
		case FIXCONST:
		case SNGCONST:
		case DBLCONST:
			if (setjmp(overenv) != 0)
				*number = MAXDOUBLE;
			else if ((*number = atof1(buf)) == MAXDOUBLE)
				error(inst.thing.linno,inst.thing.stmtno, 60);
			return(0);
		default:
			return(-1);
	}
}
