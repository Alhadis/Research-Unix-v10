/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	///////////// bfiles.c //////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Fri Aug 24 17:18:03 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 *
 *	File Access Preparation Routines
 */
/*   "@(#) bfiles.c:  V 1.4  9/9/81" */
#include	"bas.h"
extern	moreflag[];

struct FILTBL filtbl[] = {
	{'\0', 2},	/* File Designator # 1 */
	{'\0', 4},	/* File Designator # 2 */
	{'\0', 6},	/* File Designator # 3 */
	{'\0', 8},	/* File Designator # 4 */
	{'\0', 10},	/* File Designator # 5 */
	{'\0', 12},	/* File Designator # 6 */
	{'\0', 14},	/* File Designator # 7 */
	{'\0', 16},	/* File Designator # 8 */
	{'e', -1}	/* END MARKER */
	};

/*
 * ////// OPENO //////
 *
 * Open file for output.
 */
openo()
{
	setfile(1,'w');
}

/*
 * ////// OPENI //////
 *
 * Open file for input.
 */
openi()
{
	setfile(0,'r');
}

/*
 * ////// APPEND //////
 *
 * Open file for append or create if non-existant.
 */
append()
{
	setfile(2,'w');
}
/*
 *
 * //// OPEN FILES for READ, WRITE or APPEND ////
 *
 * write is mode 1
 *
 *
 */
setfile(modnum,modchar)
int	modnum;
char	modchar;
{
	int	size;	/* field size */
	char	field[20];
	int	save;
	char	*ptr;
#ifdef	STRINGS
	char	*savptr;
#endif
	int	i;
	int	madeit;
	ptr = expr;

	i = 0;
	while(filtbl[i].mode != 'e')
	{
		madeit = 0;	/* flag fact that no file was open */
		save = (int)ptr;
		if(scanfld(&ptr,field,',') == 0)
			break;
#ifdef	STRINGS
		savptr = field;
		if(class(&savptr,scratch) >= STCLASS)
		{
			evals(field,field); /* expand expr */
		}
#endif
		size = strlen(field);
		if(filtbl[i].mode == '\0') /* if this slot not used */
		{
			if(size > 14)
			{
				error(inst.thing.linno, 34); /* filename too long */
				return(-1);
			}
			if((filtbl[i].fildes = copen(field, modnum)) < 0)
			{
				error(inst.thing.linno, 2); /* open error */
				return(-1);
			}
			filtbl[i].mode = modchar; /* set mode for write */
			madeit = 1;	/* flag the fact an entry was made */
			moreflag[i] = 0;
		}
		else
		{
			ptr = (char *)save;  /* back up input string pointer */
		}
		i++;
	}
	if(!madeit && (filtbl[i].mode == 'e'))
	{
		error(inst.thing.linno, 35);	/* FILE DES. USED UP */
		return(-1);
	}
	return(0);
}
/*
 *
 * ////// FILE CLOSE //////
 *
 * format:	closef _des
 */
_closef()
{
	int	fd;
	if((expr[0] == '_') && (expr[1] >= '0' && expr[1] <= '4'))
	{
		fd = expr[1] - '1';
		close(filtbl[fd].fildes);
		moreflag[fd] = 0;	/* init more flag for this desc. */
		filtbl[fd].mode = '\0';	/* mark it available */
	}
	else
	{
		error(inst.thing.linno, 32);
		return(-1);
	}
	return(0);
}

/*
 * ////// CLOSEALL //////
 *
 * Close ALL open files.
 */
clall()
{
	int	i;

	for(i=0; filtbl[i].mode != 'e'; i++)	/* scan file table */
	{
		if(filtbl[i].mode != '\0')	/* if this entry has
						    an open file */
		{
			filtbl[i].mode = '\0';	/* mark this slot unused */
			close(filtbl[i].fildes); /* close this file */
			moreflag[i] = 0;	/* init moreflag */
		}
	}
	return(0);
}
/*
 *
 * ////// SCANFLD //////
 *
 * scan for fields delimited by dlm
 */
scanfld(ptr,field,dlm)
char	**ptr;
char	field[];
char	dlm;
{
	int	i;

	for(i=0; (**ptr != dlm) && (**ptr != '\0'); )
	{
		field[i++] = *(*ptr)++;
	}
	field[i] = '\0';	/* null term */
	if(**ptr == dlm) *(*ptr)++;	/* bump past comma */
	return(i);
}
