/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	////////////// bseek.c //////////////
 *	/// J. P. Hawkins HL X2268 2D-227 ///
 *	///// Sun Nov 29 09:55:00 1981 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *
 * BASIC SEEK routine
 * PART OF BITE
 * For random file access
 */
/*   "@(#) bseek.c:  V 1.1  11/29/81" */
#include	"bas.h"
__seek()
{
	register char *ptr;	/* pointer to program string */
	char	*prncpy();
	int	fd;		/* file descriptor */
	int	fildes;		/* file designator for BASIC */
	extern struct FILTBL filtbl[];
	double	evalx();
	long	offset;
	int	mode;
	char	field[20];

	ptr = expr;	/* point to command operand string */

	if(*ptr == '_' && (*(ptr+1) >= '0' && *(ptr+1) <= '8'))
	{
		ptr++;
		fildes = *ptr - '1';	/* get BASIC file desig */
		fd = filtbl[fildes].fildes; /* get file descriptor */
	}
	else
	{
		error(inst.thing.linno, 57);
		return(-1);
	}
	ptr++;
	if(*ptr++ != ',')	/* else it better be a , */
	{
		error(inst.thing.linno, 13); /* delimiter error */
		return(-1);
	}
	ptr=prncpy(field,ptr);		/* get field betw. ',' */
	offset = (long)evalx(field);
	if(*ptr++ != ',')	/* else it better be a , */
	{
		error(inst.thing.linno, 13); /* delimiter error */
		return(-1);
	}
	ptr=prncpy(field,ptr);		/* get field betw. ',' */
	mode = (int)evalx(field);
	if(offset > 0L)
		offset--;

	if(lineseek(fd, offset, mode) < 0)
	{
		error(inst.thing.linno, 58);
		return(-1);
	}
	return(0);
}

/*
 * Seek to the specified line in the file.
 */
lineseek(fd, offset, mode)
long offset;
{
	register long o;

	switch(mode)
	{
	case 0: /* ABSOLUTE SEEK */
		bof(fd);		/* rewind first */
	case 1: /* RELATIVE SEEK */
		for(o=0; o<offset; o++)	/* seek from there */
			skipline(fd);
		break;
	default:
		error(inst.thing.linno, 59);
		return -1;
		break;
	}
	return 0;
}

/*
 * Skip read to the next line in file
 */
skipline(fd)
{
	char c;

	while(read(fd, &c, 1) != 0)
	{
		if(c == '\n')
			break;
	}
}

/*
 * rewind to Beginning Of File
 */
bof(fd)
{
	lseek(fd, 0L, 0);
}
/*
 *
 * BASIC "rewind" command
 */
rewin()
{
	char *ptr;
	int	fd;		/* file descriptor */
	int	fildes;		/* file designator for BASIC */

	ptr = expr;
	if(*ptr == '_' && (*(ptr+1) >= '0' && *(ptr+1) <= '8'))
	{
		ptr++;
		fildes = *ptr - '1';	/* get BASIC file desig */
		fd = filtbl[fildes].fildes; /* get file descriptor */
	}
	else
	{
		error(inst.thing.linno, 60);
		return(-1);
	}
	bof(fd);
	return(0);
}
