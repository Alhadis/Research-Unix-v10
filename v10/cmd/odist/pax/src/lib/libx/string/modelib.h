/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * fmtmode() and strperm() common definitions
 */

#ifndef __LIBMODE_H__
#define __LIBMODE_H__

#define MODELEN	10

#define modetab	_mode_table_	/* data hiding				*/

struct modeop			/* ops for each char in mode string	*/
{
	int	mask1;		/* first mask				*/
	int	shift1;		/* first shift count			*/
	int	mask2;		/* second mask				*/
	int	shift2;		/* second shift count			*/
	char*	name;		/* mode char using mask/shift as index	*/
};

extern struct modeop	modetab[];

#endif
