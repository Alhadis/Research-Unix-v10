/*	"@(#) aputvars.c: V 1.9 6/14/83"	*/

/*
*	aputvars.c: 	functions for assigning to array variables
*/

#include	<stdio.h>
#include	<values.h>
#include	"bas.h"
#include	"tokens.h"

struct sym	*stsrch(),
		*stinsert();

char		*strget();

float		mkfloat();

extern	short	base;

/*
*	give double precision array element a value
*/

daputvar(vstr, value)
char 	*vstr;
double	value;
{
	short	offset,
		numdims,
		dimlist[MAXDIM],
		defdims[MAXDIM];
	char	arname[IDSIZE+1],
		*ptr;
	struct sym	*currec;
	register int	i;


	ptr = vstr;
	if (getinds(&ptr, arname, &numdims, dimlist) < 0)
		return(-1);
	
	if ((currec = stsrch(arname, DBLAID)) == NULL) /*auto dimension*/
	{
		for(i=0; i < numdims; i++)
			defdims[i] = DEFDIM;
		if (ddim(arname, numdims, defdims) < 0)
			return(-1);
		currec = stsrch(arname, DBLAID);
	}

	if ((offset = getoffset(currec->data.darr.header, numdims, dimlist)) < 0)
		return(-1);

	*((currec->data.darr.arblok) + offset) = value;
	return(0);
}



/*
*	give single precision array element a value
*/

faputvar(vstr, value)
char 	*vstr;
double	value;
{
	short	offset,
		numdims,
		dimlist[MAXDIM],
		defdims[MAXDIM];
	char	arname[IDSIZE+1],
		*ptr;
	struct sym	*currec;
	register int	i;


	ptr = vstr;
	if (getinds(&ptr, arname, &numdims, dimlist) < 0)
		return(-1);
	
	if ((currec = stsrch(arname, SNGAID)) == NULL) /*auto dimension*/
	{
		for(i=0; i < numdims; i++)
			defdims[i] = DEFDIM;
		if (fdim(arname, numdims, defdims) < 0)
			return(-1);
		currec = stsrch(arname, SNGAID);
	}

	if ((offset = getoffset(currec->data.farr.header, numdims, dimlist)) < 0)
		return(-1);

	*((currec->data.farr.arblok) + offset) = mkfloat(value);
	return(0);
}



/*
*	give integer array element a value
*/

iaputvar(vstr, value)
char 	*vstr;
double	value;
{
	short	offset,
		numdims,
		dimlist[MAXDIM],
		defdims[MAXDIM];
	char	arname[IDSIZE+1],
		*ptr;
	struct sym	*currec;
	register int	i;


	ptr = vstr;
	if (getinds(&ptr, arname, &numdims, dimlist) < 0)
		return(-1);
	
	if ((currec = stsrch(arname, INTAID)) == NULL) /*auto dimension*/
	{
		for(i=0; i < numdims; i++)
			defdims[i] = DEFDIM;
		if (idim(arname, numdims, defdims) < 0)
			return(-1);
		currec = stsrch(arname, INTAID);
	}

	if ((offset = getoffset(currec->data.iarr.header,numdims,dimlist)) < 0)
		return(-1);
	*((currec->data.iarr.arblok) + offset) = mkint(value);
	return(0);
}



char	*
elemaddr(sname)
char 	sname[];
{
	short		offset,
			numdims,
			dimlist[MAXDIM],
			defdims[MAXDIM];
	char		arname[IDSIZE+1],
			*snptr;
	struct sym	*currec;
	int		type;
	register int	i;

	snptr = sname;
	if ((type = getinds(&snptr, arname, &numdims, dimlist)) < 0)
		return(NULL);
	
	if ((currec = stsrch(arname, type)) == NULL) /*auto dimension*/
	{
		for (i = 0; i < numdims; i++)
			defdims[i] = DEFDIM;
		switch(type)
		{
		case SNGAID:
			if (fdim(arname, numdims, defdims) < 0)
				return(NULL);
			break;
		case DBLAID:
			if (ddim(arname, numdims, defdims) < 0)
				return(NULL);
			break;
		case INTAID:
			if (idim(arname, numdims, defdims) < 0)
				return(NULL);
			break;
		case STRAID:
			if (sdim(arname, numdims, defdims) < 0)
				return(NULL);
			break;
		}
		currec = stsrch(arname, type);
	}

	switch (type)
	{
	case SNGAID:
		if ((offset = 
		       getoffset(currec->data.farr.header,numdims,dimlist)) < 0)
			return(NULL);
		return((char *) (currec->data.farr.arblok + offset));
	case DBLAID:
		if ((offset = 
		       getoffset(currec->data.darr.header,numdims,dimlist)) < 0)
			return(NULL);
		return((char *) (currec->data.darr.arblok + offset));
	case INTAID:
		if ((offset = 
		       getoffset(currec->data.iarr.header,numdims,dimlist)) < 0)
			return(NULL);
		return((char *) (currec->data.iarr.arblok + offset));
	case STRAID:
		if ((offset = 
		       getoffset(currec->data.sarr.header,numdims,dimlist)) < 0)
			return(NULL);
		return((char *) (currec->data.sarr.arblok + offset));
	}
	return(NULL);
}



/*
*	give string array element a value
*/

saputvar(sname, sptr)
char 	sname[];
char	*sptr;
{
	char	**addr;

	if ((addr = (char **) elemaddr(sname)) == NULL)
		return(-1);
	strfree(*addr);
	if (((*addr) = strget(sptr)) == NULL)
	{
		(*addr) = strget("");
		return(-1);
	}
	return(0);
}



/*
*	get offset of array element from array base
*	offset(a(d1, d2, ..., dn)) = d1-base + (d2-base)*a2 + ... + (dn-base)*an
*	the ai's are stored in the header block:
*	ak+1 = (dimk - base + 1)(dimk-1 - base + 1)...(dim1 - base + 1)
*	see dim.c for header structure
*/

getoffset(headptr, numdims, dimlist)
short	*headptr,
	numdims,
	dimlist[];
{
	short	offset,
		*cptr,
		i;

	cptr = headptr;
	if (*cptr != numdims)
	{
		error(inst.thing.linno,inst.thing.stmtno, 26); 
		return(-1); /*wrong # of dimensions*/
	}
	if ((dimlist[0] < base) || (dimlist[0] > *(++cptr)))
	{
		error(inst.thing.linno,inst.thing.stmtno, 86);
		return(-1);	/* first index out of range */
	}
	offset = dimlist[0] - base;
	for (i = 1; i < numdims; i++)
	{
		offset += (dimlist[i] - base) * (*(++cptr));
		if ((dimlist[i] < base) || (dimlist[i] > *(++cptr)))
		{
			error(inst.thing.linno,inst.thing.stmtno, 86);
			return(-1);	/* index out of range */
		}
	}
	return(offset);
}
