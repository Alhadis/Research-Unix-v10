/*	@(#)ns_curses.h	1.1	(1.1)	*/

/*	Defines and includes required for curses to work in a non-	*/
/*	standard environment.						*/

#include	"RecStruct.h"
#include	"VTio.h"
#include	"VTioctl.h"

/*	Make sure that curshdr.h is included before workst.h is		*/
/*	brought in so that screen can be defined properly.		*/

#ifndef		INFINITY
#include	"curshdr.h"
#endif

#include	"workst.h"

#define		SP		((struct screen *)&(WS->w_screen))

#define		cur_term	WS->w_cur_term

#define		outputspeed(x)	B9600
