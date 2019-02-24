#include	"sfhdr.h"

/*	The main engine for reading formatted data
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#define	S_NORMAL	0	/* argument is a pointer to a normal object */
#define S_LONG		1	/* argument is a pointer to a long object */
#define S_SHORT		2	/* argument is a pointer to a short object */

#define a2f(s)	strtod(s,NIL(char**))

#define A_SIZE		(1<<8)	/* alphabet */
#if __STDC__ || __cplusplus
static char *setclass(reg char* form, reg char* accept)
#else
static char *setclass(form,accept)
reg char	*form;		/* format string */
reg char	*accept;	/* accepted characters are set to 1 */
#endif
{
	reg int		fmt, c, yes;

	if((fmt = *form++) == '^')
	{	/* we want the complement of this set */
		yes = 0;
		fmt = *form++;
	}
	else	yes = 1;

	for(c = 0; c < A_SIZE; ++c)
		accept[c] = !yes;

	if(fmt == ']' || fmt == '-')
	{	/* special first char */
		accept[fmt] = yes;
		fmt = *form++;
	}

	for(; fmt != ']'; fmt = *form++)
	{	/* done */
		if(!fmt)
			return (form-1);

		/* interval */
		if(fmt != '-' || form[0] == ']' || form[-2] > form[0])
			accept[fmt] = yes;
		else for(c = form[-2]+1; c < form[0]; ++c)
			accept[c] = yes;
	}

	return form;
}

#define SETARG(val,argf,args,type,fmt,form) \
	{ if(!argf) \
		*va_arg(args,type*) = (type)val; \
	  else if((*argf)(fmt,(char*)(&val),sizeof(val)) < 0) \
		form = ""; \
	}

#if __STDC__ || __cplusplus
sfvscanf(Sfile_t* f, reg const char* form, va_list args)
#else
sfvscanf(f,form,args)
Sfile_t		*f;		/* file to be scanned */
reg char	*form;		/* scanning format */
va_list		args;
#endif
{
	reg int		fmt, inp;
	reg char	*d;
	reg int		skip, size, length, n_match;
	reg Sfile_t	*sf;
	reg char	*sp;
	char		accept[MAXDIGITS];
	reg char	*endd;
	Argf_s		argf;
	Extf_s		extf;
	reg Fa_t	*fa, *fast;
	va_list		alist;

	/* fast io system */
#define SFSET(f)	(d = (char*)f->next, endd = (char*)f->endb)
#define SFEND(f)	(f->next = (uchar*)d)
#define SFFILL(f,c)	(SFEND(f), (SFopen(f),c = _sffilbuf(f,1),SFLOCK(f)), SFSET(f), c)
#define SFGETC(f,c)	(d >= endd ? SFFILL(f,c) : (c = (int)(*d++)))
#define SFUNGETC(f,c)	(--d)

	if(f->mode != SF_READ && SFMODE(f,SF_READ) < 0)
		return -1;
	SFLOCK(f);
	SFSET(f);
	n_match = 0;
	inp = -1;
	sf = NIL(Sfile_t*);
	argf = NIL(Argf_s);
	extf = NIL(Extf_s);
	fast = NIL(Fa_t*);

loop_fa :
	while(fmt = *form++)
	{
		switch(fmt)
		{
		default : /* literal match */
		literal :
			if(SFGETC(f,inp) != fmt)
			{	/* premature eof */
				if(inp < 0)
					return -1;
				SFUNGETC(f,inp);
				goto done;
			}
			continue;

		case ' ' : /* skip blanks */
		case '\t':
		case '\n':
			do	{ SFGETC(f,inp); }
			while(isspace(inp) || inp == '\n');
			if(inp < 0)
				goto done;
			else	SFUNGETC(f,inp);
			continue;

		case '%' : /* matching some pattern */
			skip = 0;
			switch(fmt = *form++)
			{
			case '%' : /* match % literally */
				goto literal;

			case '@' : /* set argument getting function */
				argf = va_arg(args,Argf_s);
				continue;

			case '&' : /* set extension function */
				extf = va_arg(args,Extf_s);
				continue;

			case ':' : /* stack a pair of format/arglist */
				if(!FAMALLOC(fa))
					goto done;
				fa->form = (char*)form;
				if(!(form = va_arg(args,char*)))
					form = "";
				alist = va_arg(args,va_list);
				fa->args = args;
				args = alist;
				fa->extf.s = extf;
				fa->argf.s = argf;
				fa->next = fast;
				fast = fa;
				continue;

			case '*' :	/* skip one argument */
				skip = 1;
				fmt = *form++;

			default :
				/* scan length */
				length = 0;
				while(isdigit(fmt))
				{	length = length*10 + (fmt - '0');
					fmt = *form++;
				}

				/* size of object to be assigned */
				if(fmt == 'l')
					{ size = S_LONG; fmt = *form++; }
				else if(fmt == 'h')
					{ size = S_SHORT; fmt = *form++; }
				else	size = S_NORMAL;

				/* canonicalize format */
				switch(fmt)
				{
				case 'D' : case 'I' : case 'U' :
					size = S_LONG;
				case 'i' : case 'u' :
					fmt = 'd';
					break;
				case 'O' :
					size = S_LONG;
					fmt = 'o';
					break;
				case 'X' :
					size = S_LONG;
					fmt = 'x';
					break;
				case 'E' : case 'F' : case 'G' :
					size = S_LONG;
				case 'e' : case 'f' : case 'g' :
					fmt = 'f';
					break;
				}
			}
		}

		/* scan length */
		if(length == 0)
			length = fmt == 'c' ? 1 : SF_BUFSIZE;

		/* define the first input character */
		if(fmt == 'c' || fmt == '[')
			SFGETC(f,inp);
		else
		{	/* skip starting blanks */
			do	{ SFGETC(f,inp); }
			while(isspace(inp) || inp == '\n')
				;
		}
		if(inp < 0)
			goto done;

		if(fmt == 'f')
		{	/* a float or double */
			reg char	*val;
			reg int		dot, exponent;
			double		dval;

			val = accept;
			if(length >= MAXDIGITS)
				length = MAXDIGITS-1;
			dot = exponent = 0;
			do
			{	if(isdigit(inp))
					*val++ = inp;
				else if(inp == '.')
				{	/* too many dots */
					if(dot++ > 0)
						break;
					*val++ = '.';
				}
				else if(inp == 'e' || inp == 'E')
				{	/* too many e,E */
					if(exponent++ > 0)
						break;
					*val++ = inp;
					SFGETC(f,inp);
					if(inp == '-' || inp == '+' || isdigit(inp))
						*val++ = inp;
					else	break;
				}
				else if(inp == '-' || inp == '+')
				{	/* too many signs */
					if(val > accept)
						break;
					*val++ = inp;
				}
				else	break;

			} while(--length > 0 && SFGETC(f,inp) >= 0);

			if(val > accept)
			{	/* there is something to convert */
				n_match += 1;
				*val = '\0';

				if(!skip)
				{	/* assign */
					dval = a2f(accept);
					switch(size)
					{
					case S_LONG:
						SETARG(dval,argf,args,double,'F',form);
						break;
					case S_SHORT :
					case S_NORMAL:
						SETARG(dval,argf,args,float,'f',form);
						break;
					}
				}
			}
		}
		else if(fmt == 's' || fmt == 'c' || fmt == '[')
		{	n_match += 1;

			sp = NIL(char*);
			if(!skip)
			{	if(!argf)
					sp = va_arg(args,char*);
				else if(!sf)
					sf = sfnew(NIL(Sfile_t*),NIL(uchar*),-1,-1,
						   SF_STRING|SF_WRITE);
				else	sfseek(sf,0L,0);
			}

			if(fmt == 's')
			{	/* copy a string */
				do
				{	if(isspace(inp) || inp == '\n')
						break;
					if(sp)
						*sp++ = inp;
					else if(!skip && sf)
						sfputc(sf,inp);
				} while(--length > 0 && SFGETC(f,inp) >= 0);
			}
			else if(fmt == 'c')
			{	/* copy characters */
				do
				{	if(sp)
						*sp++ = inp;
					else if(!skip && sf)
						sfputc(sf,inp);
				} while(--length > 0 && SFGETC(f,inp) >= 0);
			}
			else
			{	/* copy characters from a class */
				form = setclass((char*)form,accept);
				do
				{	if(!accept[inp])
						break;
					if(sp)
						*sp++ = inp;
					else if(!skip && sf)
						sfputc(sf,inp);
				} while(--length > 0 && SFGETC(f,inp) >= 0);
			}
			if(sp)
			{	if(fmt != 'c' || length > 1)
					*sp = '\0';
			}
			else if(!skip && sf)
			{	sfputc(sf,'\0');
				if((*argf)('s',(char*)sf->data,sf->next-sf->data) < 0)
					form = "";
			}
		}
		else if(fmt == 'd' || fmt == 'o' || fmt == 'x')
		{	/* some integer type */
			long	lval;
			reg int	base, sign;

			base = fmt == 'o' ? 8 : fmt == 'x' ? 16 : 10;
			if(inp == '-' || inp == '+')
			{	/* get the sign */
				sign = inp == '-' ? -1 : 1;
				SFGETC(f,inp);
				length -= 1;
			}
			else	sign = 1;
			if(inp < 0)
				goto done;

			/* skip leading 0's */
			do
			{	if(inp != '0')
					break;
			} while(--length > 0 && SFGETC(f,inp) >= 0);

			/* now convert */
			lval = 0;
			if(base == 10)
			{	if(length > 0) do
				{	if(inp < '0' || inp > '9')
						break;
					lval = (lval << 3) + (lval << 1) + (inp - '0');
				} while(--length > 0 && SFGETC(f,inp) >= 0);
			}
			else if(base == 8)
			{	if(length > 0) do
				{	if(inp < '0' || inp > '7')
						break;
					lval = (lval << 3) + (inp - '0');
				} while(--length > 0 && SFGETC(f,inp) >= 0);
			}
			else	/* base 16 */
			{	if(length > 0 && (inp == 'x' || inp == 'X'))
				{	/* skip leading x */
					SFGETC(f,inp);
					length -= 1;
				}
				if(length > 0) do
				{	if(inp >= '0' && inp <= '9')
						inp = inp - '0';
					else if(inp >= 'a' && inp <= 'f')
						inp = (inp - 'a') + 10;
					else if(inp >= 'A' && inp <= 'F')
						inp = (inp - 'A') + 10;
					else	break;
					lval = (lval << 4) + inp;
				} while(--length > 0 && SFGETC(f,inp) >= 0);
			}

			/* assign */
			n_match += 1;
			if(sign < 0)
				lval = -lval;
			if(!skip) switch(size)
			{
			case S_SHORT :
				SETARG(lval,argf,args,short,'h',form);
				break;
			case S_NORMAL :
				SETARG(lval,argf,args,int,'d',form);
				break;
			case S_LONG :
				SETARG(lval,argf,args,long,'D',form);
				break;
			}
		}
		else if(extf)
		{	/* call extension function */
			char	*rv;
			int	n;
			SFEND(f);
			SFOPEN(f);
			n = (*extf)(f,fmt,length,&rv);
			SFLOCK(f);
			SFSET(f);
			if(n >= 0)
			{	n_match += 1;
				if(!skip)
				{	if(!argf)
					{	sp = va_arg(args,char*);
						while(n--)
							*sp++ = *rv++;
					}
					else if((*argf)(fmt,rv,n) < 0)
						form = "";
				}
			}
			continue;
		}

		if(inp < 0)
			break;
		else if(length > 0)
			SFUNGETC(f,inp);
	}

	if(fa = fast)
	{	/* check for stacked formats/arglists */
		form = fa->form;
		args = fa->args;
		argf = fa->argf.s;
		extf = fa->extf.s;
		fast = fa->next;
		FAFREE(fa);
		goto loop_fa;
	}

done:
	if(sf)
		sfclose(sf);
	SFEND(f);
	SFOPEN(f);
	return (n_match == 0 && inp < 0) ? -1 : n_match;
}
