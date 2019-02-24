/*ident	"%W%" */
/**************************************************************************
			Copyright (c) 1984 AT&T
	  		  All Rights Reserved  	

	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
	
	The copyright notice above does not evidence any   	
	actual or intended publication of such source code.

*****************************************************************************/
#include <task.h>

/* Functions with C linkage, for sswap to call, to avoid dependency
 * on internal name-encoding algorithm.  These functions call the ones
 * sswap really wants.
 */

extern "C" {
int* swap_call_new(int);
void swap_call_delete(int*);
}

int*
swap_call_new(int size)
{
	int*	p;
	p = new int[size];
	while (p == 0) object::task_error(E_STORE, (object*)0);
	return p;
}

void
swap_call_delete(int* p)
{
	delete p;
}
