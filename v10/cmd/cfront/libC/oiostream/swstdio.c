/*ident	"@(#)ctrans:lib/stream/swstdio.c	1.1.1.1" */
/**************************************************************************
			Copyright (c) 1984 AT&T
	  		  All Rights Reserved  	

	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
	
	The copyright notice above does not evidence any   	
	actual or intended publication of such source code.

swstdio.c:

*****************************************************************************/


#include <iostream.h>
#include <stdiostream.h>

void ios::sync_with_stdio() 
{
	switch ( Iostream_init::stdstatus )
	{
	case 1 : cout.flush() ; cerr.flush() ; clog.flush() ; break ;
	case 2 : break ;
	default : return ; }
 
	cin = new stdiobuf(stdin) ;
	cout = new stdiobuf(stdout) ;
	cerr = new stdiobuf(stderr) ;
	clog = cerr ;
	cin.rdbuf()->setbuf((char*)0,0) ;
	cout.rdbuf()->setbuf((char*)0,0) ;
	cerr.rdbuf()->setbuf((char*)0,0) ;
	clog.rdbuf()->setbuf((char*)0,0) ;
	cin.skip(1) ;
	cin.tie(&cout) ;
	cerr.tie(&cout) ;
	clog.tie(&cout) ;
	Iostream_init::stdstatus = 2 ;
	}
