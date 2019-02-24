/*	@(#)nio.c	1.1	*/
#include <stdio.h>
#include <ctype.h>
#include "fio.h"
#include "fmt.h"
#include "lio.h"

/*
 *	namelist io
 *
 *	see f77's proc.c at namelist() for description
 */

typedef struct {
	int	ndims;			/* # of dimensions */
	int	nels;			/* # of elements */
	int	baseoff;		/* how to get to (0,...,0) element */
	int	span[7];		/* span of each dimension +1+ */
} Dims;					/* dimension descriptor */


					/* +1+ note: only # of dimensions
					applies here, i.e. actual array
					is between 0 and 7 elements based
					on ndims */

typedef union {
	char	*pchar;
	short	*pshort;
	int	*pint;
	long	*plong;
	float	*pfloat;
	double	*pdouble;
	char	**pptr;
} Pointer;				/* pointer to all sorts of things */


typedef struct {
	char	varname[18];		/* name of variable */
	Pointer	varaddr;		/* where it is */
	int	type;			/* its type */
	Dims	*dimp;			/* dimension descriptor */
} Nlentry;				/* namelist entry: 1 for each var */


typedef struct {
	char	nlname[18];		/* name of namelist */
	Nlentry	nlvnames[1];		/* array of variable descriptors +2+*/
} Namelist;

					/* +2+ note: this array is not bounded
					but is terminated by an entry with a
					null varname */

/*
 * s_rsne - start read namelist external
 *
 * s_rsne
 *	if (file not initialized)
 *		intialize it
 *	if (file doesn't jive with namelist io)
 *		return error
 *	initialize some global variables
 *	if (not currently reading on file OR not capable of doing so)
 *		return error
 *	read namelist name
 *	if (not correct namelist name)
 *		error
 *	determine number of variables in namelist
 *	read variable name
 *	while (variable name is not "&end")
 *		if (variable is in namelist)
 *			read value(s) for it
 *		else
 *			error
 *	if (not correct number of variables read)
 *		error
 * end s_rsne
 */

#define BSIZ 50

static char nlrs[] = "namelist read";

s_rsne(pnlarg)
cilist *pnlarg;
{
	Namelist *pnl;
	Nlentry *pnlent, *findit();
	int nvars, n;
	char buf[BSIZ], *getword();

	if (!init)
		f_init();
	if (n = c_nle(pnlarg))
		return (n);
	reading = external = sequential = 1;
	formatted = 0;
	if (curunit->uwrt && nowreading(curunit))
		return (1);
	pnl = (Namelist *) pnlarg->cifmt;
	if (getc(cf) != ' ' || getc(cf) != '&')
		err(pnlarg->cierr, 115, nlrs);
	if (strcmp(pnl->nlname, getword(buf, strlen(pnl->nlname))))
		err(pnlarg->cierr, 118, buf);
	n = getc(cf);
	if (!isspace(n))
		err(pnlarg->cierr, 115, nlrs);
	for (nvars=0, pnlent = pnl->nlvnames; strlen(pnlent->varname); ++pnlent)
		++nvars;
	n = 0;
	elist = pnlarg;
	while (nvars--)
		{
		if (!getword(buf, BSIZ - 1))
			err(pnlarg->cierr, 120, nlrs);
		if (!strcmp(buf, "&end"))
			err(pnlarg->cierr, 121, nlrs);
		if (!(pnlent = findit(buf, pnl->nlvnames)))
			err(pnlarg->cierr, 119, buf);
		if (getvar(pnlent))
			err(pnlarg->cierr, 120, nlrs);
		}
	if (!getword(buf, BSIZ - 1))
		err(pnlarg->cierr, 120, nlrs);
	if (strcmp(buf, "&end"))
		err(pnlarg->cierr, 121, nlrs);
	return (0);
}

/* miscellaneous utility functions for namelist read */

/* getword - get a "word" text string from current file */

char *getword(s, n)
char *s;
int n;
{
	int i;
	char *p;

	p = s;
	i = getc(cf);
	while (isspace(i) || (ispunct(i) && i != '&'))
		i = getc(cf);
	while (n--)
		{
		if (i != EOF && i != '=' && !isspace(i))
			if (isupper(i))
				*p++ = tolower(i);
			else
				*p++ = i;
		else
			break;
		i = getc(cf);
		}
	if (feof(cf) && p == s)
		return (NULL);
	*p = '\0';
	return (s);
}

/* findit - find key in list of Nlentrys */

Nlentry *findit(key, list)
char *key;
Nlentry *list;
{
	while (strlen(list->varname))
		{
		if (!strcmp(key, list->varname))
			return (list);
		else
			++list;
		}
	return (NULL);
}

/* getvar - read values for namelist io
 *
 * getvar uses l_read of list io to do all the dirty work, therefore
 * it should be inserted into the library before lread.c (on UNIX
 * systems with barbaric topologically sorted libraries)
 *
 * It sets the cierr flag so that l_read (and its subordinates) will
 * not report errors, but pass them back so that the diagnostic message
 * will appear to come from "namelist read".
 */

getvar(pnlent)
Nlentry *pnlent;
{
	int n, i, size;

	if (pnlent->dimp)
		n = pnlent->dimp->nels;
	else
		n = 1;
	elist->cierr = 1;
	switch (pnlent->type)
		{
		case TYADDR:
			size = sizeof(char *);
			break;
		case TYSHORT:
			size = sizeof(short);
			break;
		case TYLOGICAL:
		case TYLONG:
			size = sizeof(long);
			break;
		case TYREAL:
		case TYCOMPLEX:
			size = sizeof(float);
			break;
		case TYDREAL:
		case TYDCOMPLEX:
			size = sizeof(double);
			break;
		default:
			if (pnlent->type < 0)
				{
				if (n = l_read(&n, pnlent->varaddr,
					    -pnlent->type, TYCHAR))
					err(elist->cierr = 0, n, nlrs);
				return (0);
				}
			else
				err(elist->cierr = 0, 117, nlrs);
		}
	if (n = l_read(&n, pnlent->varaddr, size, pnlent->type))
		err(0, n, nlrs);
	elist->cierr = 0;
	return (0);
}

/*
 * s_wsne - start write namelist external
 *
 * s_wsne
 *	if (file not initialized)
 *		initialize it
 *	if (file doesn't jive with namelist io)
 *		return error
 *	initialize some global variables
 *	if (not currently writing on file OR not capable of doing so)
 *		return error
 *	set up namelist and entry pointers
 *	output namelist name in proper format
 *	do
 *		output variable name
 *		output value based on type
 *		point to next entry
 *	while (there are more to do AND sneakily output a comma separator
 *	output end line
 * end s_wsne
 */

s_wsne(pnlarg)
cilist *pnlarg;
{
	Namelist *pnl;
	Nlentry *pnlent;
	Pointer ptr;
	int i, n, vtype;
	char *pch, buf[BSIZ];

	if (!init)
		f_init();
	if (n = c_nle(pnlarg))
		return(n);
	reading = formatted = 0;
	external = sequential = 1;
	if (!curunit->uwrt && nowwriting(curunit))
		return(1);
	pnl = (Namelist *) pnlarg->cifmt;
	(void) putc(' ', cf);
	(void) putc('&', cf);
	(void) fputs(pnl->nlname, cf);
	(void) putc('\n', cf);
	(void) putc(' ', cf);
	pnlent = pnl->nlvnames;
	do {
		(void) fputs(pnlent->varname, cf);
		(void) putc('=', cf);
		if (pnlent->dimp)
			n = pnlent->dimp->nels;
		else
			n = 1;
		if ((vtype = pnlent->type) < 0 && (pch = pnlent->varaddr.pchar))
			do {
				(void) putc('\'', cf);
				for (i = vtype; i; ++i)
					(void) putc(*pch++, cf);
				(void) putc('\'', cf);
			} while (--n && !t_putc(','));
		else
			{
			ptr.pchar = pnlent->varaddr.pchar;
			do { switch (vtype)
				{
				case TYADDR:
					(void) sprintf(buf,"0x%x", *ptr.pptr++);
					break;
				case TYSHORT:
					(void) sprintf(buf,"%d", *ptr.pshort++);
					break;
				case TYLONG:
					(void) sprintf(buf,"%ld", *ptr.plong++);
					break;
				case TYREAL:
					(void)sprintf(buf,"%.8f",*ptr.pfloat++);
					break;
				case TYDREAL:
					(void) sprintf(buf, "%.18e",
					    *ptr.pdouble++);
					break;
				case TYCOMPLEX:
					(void) sprintf(buf, "(%.8f,%.8f)",
					    *ptr.pfloat, *(ptr.pfloat+1));
					ptr.pfloat += 2;
					break;
				case TYDCOMPLEX:
					(void) sprintf(buf, "(%.18e,%.18e)",
					    *ptr.pdouble, *(ptr.pdouble+1));
					ptr.pdouble += 2;
					break;
				case TYCHAR:
					(void) sprintf(buf, "%c", *ptr.pchar++);
					break;
				case TYLOGICAL:
					(void) sprintf(buf, ".%s.",
					    (*ptr.plong ? "TRUE" : "FALSE"));
					break;
				default:
					err(pnlarg->cierr, 117, "namelist io");
				}
				(void) fputs(buf, cf);
			} while (--n && !t_putc(','));
			}
		++pnlent;
	} while (strlen(pnlent->varname) && !t_putc(','));
	(void) fputs("\n &end\n", cf);
	return (0);
}

/*
 * c_nle - check namelist external
 *
 * c_nle
 *	set up global variables
 *	if (bogus unit)
 *		fatal error
 *	if (unit is unitialized AND can't be)
 *		fatal error
 *	if (can't do unformatted io on unit)
 *		fatal error
 * end c_nle
 */


c_nle(pcl)
cilist *pcl;
{
	fmtbuf = "namelist io";
	if (0 > pcl->ciunit || pcl->ciunit >= MXUNIT)
		err(pcl->cierr, 101, "start namelist io");
	scale = recpos = 0;
	curunit = &units[pcl->ciunit];
	if (curunit->ufd == NULL && fk_open(SEQ, FMT, pcl->ciunit))
		err(pcl->cierr, 102, "namelist io");
	cf = curunit->ufd;
	if (!curunit->ufmt)
		err(pcl->cierr, 103, "namelist io");
	return(0);
}
