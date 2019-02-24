/*
 *
 *	area fills for Brush  --  brush/newfill.c
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 */

#include "brush.h"

seedfill()
{
	Rectangle r;
	register Word rox,roy,rcx,rcy;
	register Bitmap *t,*safe;
	Word i,limit;
	Help old_set;

	old_set = helpswitch( sweep_set );	
	r = sweeprect( Pt(1,1) );
	rox = r.origin.x;
	roy = r.origin.y;
	rcx = r.corner.x;
	rcy = r.corner.y;
	helpswitch( old_set );

	if ( nullrect(r) ) return;	

	if ( !( t = balloc( r ) ) ) erreturn( "no memory" );

	if ( !( safe = balloc( r ) ) )
	   {
		free( t );
		erreturn( "no memory" );
	   }

	/* make a safe copy */

	bitblt( &display,r,safe,r.origin,F_STORE );

	helpswitch( pos_set );	
	cursswitch( null(Texture) );

	i = errmsg( "seed points?" );
	if ( i==1 ) point( &display,mouse.xy,F_STORE );

	while ( !bttn3() )
		if ( bttn12() )
		   {
			point( &display,mouse.xy,realmode(F_STORE) );
			while( bttn12() );
		   }

	waitbuttons( UP );

	/* erase all but seed points */

	bitblt( safe,r,&display,r.origin,F_XOR );
	helpswitch( kill_set );	

	limit = max( RHEIGHT(r),RWIDTH(r) );

	for ( i=1; i<limit && !bttn123(); i++,wait( CPU ) )
	    {
		bitblt( &display,r,t,Pt(rox,roy),F_STORE );
		bitblt( &display,Rect(rox,roy+1,rcx,rcy),t,Pt(rox,roy),F_OR );
		bitblt( &display,Rect(rox,roy,rcx,rcy-1),
			t,Pt(rox,roy+1),F_OR );
		bitblt( &display,Rect(rox,roy,rcx-1,rcy),
			t,Pt(rox+1,roy),F_OR );
		bitblt( &display,Rect(rox+1,roy,rcx,rcy),t,Pt(rox,roy),F_OR );
		bitblt( t,r,&display,Pt(rox,roy),F_STORE );
		bitblt( safe,r,&display,Pt(rox,roy),F_CLR );
	    }

	texture16( t,r,shade,F_XOR );
	bitblt( t,r,&display,r.origin,F_CLR );
	bitblt( &display,r,safe,r.origin,F_OR );

	bitblt( safe,r,&display,r.origin,F_STORE );
	bfree( safe );
	bfree( t );
	helpswitch( old_set );	
}
