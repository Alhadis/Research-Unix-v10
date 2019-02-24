
/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	/////////////// more ////////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Sat Feb 21 08:56:12 1981 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	@(#) more.c: V1.2 9/9/81

*/
#include "bas.h"

int	moreflag[8];
char	morebuf[8][132];
extern struct FILTBL filtbl[];

more(fnum)
int	fnum;		/* BASIC file designator (table position) */
{
	int fd;
	fd = filtbl[fnum].fildes;	/* get real filedes */
	moreflag[fnum] = 1;		/* set data pre-read */
	return(getmore(fd,&morebuf[fnum][0]));
}

/*
 *
 * //// GET LINE FROM FILE ////
 */
fgetlin(tmpbuf,fnum)
char	tmpbuf[];
int	fnum;			/* file designator for BASIC */
{
	int fd;

	fd = filtbl[fnum].fildes; /* convert to system file descriptor */
	if(moreflag[fnum])	/* if buffer was read by "if more" */
	{
		strcpy(tmpbuf,&morebuf[fnum][0]); /* just copy data already read */
		moreflag[fnum] = 0;
	}
	else
	{
		getmore(fd,tmpbuf);	/* else do the read now */
	}
}

getmore(fd,tmpbuf)
int	fd;
char	tmpbuf[];
{
	register i;

	i = 0;
	while(read(fd,&tmpbuf[i],1) > 0)
	{
		if(tmpbuf[i] == '\n')
		{
			tmpbuf[i] = '\0';
			return(i);
		}
		i++;
	}
	if(i == 0)  /* make sure buffer looks empty if nothing read */
		tmpbuf[0] = '\0';
	return(i);
}
