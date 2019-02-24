/*ident	"@(#)ctrans:lib/stream/sbuf.dbp.c	1.1.2.1" */
/**************************************************************************
			Copyright (c) 1984 AT&T
	  		  All Rights Reserved  	

	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
	
	The copyright notice above does not evidence any   	
	actual or intended publication of such source code.

sbuf.dbp.c

*****************************************************************************/


#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <osfcn.h>

void streambuf::dbp()
{
	char msg[256] ;
	sprintf(msg,"buf at %#x, base=%#x, ebuf=%#x, ",
				this, base(), ebuf());
	write(1,msg,strlen(msg)) ;
	sprintf(msg,"pptr=%#x, epptr=%#x, ", pptr(),epptr() );
	write(1,msg,strlen(msg)) ;
	sprintf(msg,"eback=%#x, gptr=%#x, egptr=%#x\n",
			eback(), gptr(), egptr() ) ;
	write(1,msg,strlen(msg)) ;

}
