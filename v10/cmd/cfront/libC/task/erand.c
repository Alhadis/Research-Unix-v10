/*ident	"%W%" */
/**************************************************************************
			Copyright (c) 1984 AT&T
	  		  All Rights Reserved  	

	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
	
	The copyright notice above does not evidence any   	
	actual or intended publication of such source code.

*****************************************************************************/
#include <task.h>
#include <math.h>
#include "hw_stack.h"

/* NOTE:  This function is in a separate file because it needs the
 * math library. Programs that bring in this file must link with -lm
 */
int
erand::draw()
{
	return (int)(-mean * log( (double)(MAXINT_AS_FLOAT-randint::draw())
				/ MAXINT_AS_FLOAT) + .5);
}

