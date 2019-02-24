/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	////////////// copen.c //////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Fri Aug 24 17:35:10 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 * FILE OPEN ROUTINES FOR BITE
 * if LSX not defined, compiles for TS system calls
 * call format:	fd = copen(filename,mode)
 *
 *	where:	fd is returned file descriptor
 *		filename is string pointer to filename
 *		mode = 0 for input, 1 for output and 2 for append
 */
/*   "@(#) copen.c:  V 1.1  12/21/80" */
#include	"bas.h"

copen(s,mode)
char	s[];
int	mode;
{
	int	fd;
	switch(mode)
	{
		case 0:	/* OPEN FOR INPUT */
			fd = open(s, 0);
			break;
		case 1: /* OPEN FOR OUTPUT */
			fd = creat(s, 0666);
			break;
		case 2: /* OPEN FOR APPEND */
			if((fd = open(s, 1)) < 0) /* try to open */
				fd = creat(s, 0666); /* create new file
							if one doesn't exist */
			else
			{
				/*
				 * SEEK TO END OF FILE FOR APPEND
				 */
				lseek(fd, 0L, 2);
			}
		default:
			break;
	}
	return(fd);
}
