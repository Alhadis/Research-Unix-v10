/*
 *
 *	main() and globals for Brush  --  brush/brush.c
 *
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 *
 *	NOTE:  this must be downloaded via 'jx', as it uses jerqio
 */

#include "brush.h"

Rectangle statrect;		/* rectangle for status display	*/

/*	current and available shades	*/

Texture16 *shade;
Texture16 *shades[] = {
			&grey1,&grey2,&grey3,&grey4,&grey5,&grey6,
			&grey7,&black,&leafs,&bigrcheck,&bigstcheck,
			&cross,&woven,&thickvs,&thickhs,&star,&xhatch,&diags,
			&zig,&tictac,&zag,&blot1,&blot2,&tactic,
			&deathstar,&cube,&smile2,&scowley,null( Texture16 )
		      };

/*	current and available brushes	*/

Texture16 *brush;
Texture16 *brushes[] = {
			 &circle3,&circle5,&circle7,&dot2,&dot4,&dot6,&dot8,
			 &bar6,&bar10,&bar14,&dash6,&dash10,&dash14,
			 &slash8,&slash12,&slash16,
			 &diag3,&ldiag3,&grp3,&lgrp3,
			 &null1,&null2,&null3,&null4,
			 &null5,&null6,&null7,&null8,null( Texture16 )
		       };

Word maskwords[16],notmaskwords[16],splatwords[16],bigwords[32];

Bitmap screen;		/* used instead of display to clip to draw area */
Bitmap bigscreen;	/* used to grab bitmaps from area outside layer	*/


Bitmap mask      = { maskwords,1,{{0,0},{16,16}} };
Bitmap notmask   = { notmaskwords,1,{{0,0},{16,16}} };
Bitmap spraymask = { splatwords,1,{{0,0},{16,16}} };
Bitmap bigmask   = { bigwords,1,{{0,0},{32,32}} };

char *font_dir;		/* name of font directory from argv	*/
char *pix_dir;		/* name of picture directory from argv	*/
char *tex_dir;		/* name of texture directory from argv	*/

main( argc,argv )
	int argc;
	char **argv;
{
	init();
	font_dir = argv[1];	/* get name of font directory		*/
	pix_dir  = argv[2];	/* get name of picture directory	*/
	tex_dir  = argv[3];	/* get name of texture directory	*/

	for ( ;; wait(MOUSE) )
	    {
		check_reshape();
		if ( bttn12() ) freehand( drawmode );
		if ( bttn3() )  icon_handler();
	    }
}

init()
{
	sleep(120);		/* bizarre nonsense to pacify IBM's	*/

#ifdef J5620
#ifdef OLDROM
	jinit();
#endif
#endif
	srand( *XMOUSE );
	srandx( *XMOUSE );
	request( MOUSE|KBD );

	/* set default font, shade, button icons, & brush */

	curr_font = fontlist[0] = &defont;
	shade = &black;
	curr_set = default_set;
	setbrush( &circle3 );
	P->state |= RESHAPED;	/* force a redraw upon main loop entry */
}

check_reshape()		/* if reshaped, reset rectangles and redraw */
{
	int i;

	if ( P->state & RESHAPED )
	   {
		P->state &= ~RESHAPED;
		statrect = Drect;
		statrect.corner.y = DORG.y + MHEIGHT;
		rectf( &display,statrect,F_STORE );
		rectf( &display,inset( statrect,2 ),F_XOR );

		for ( i=0; i<32; i++ ) bigwords[i] = sparse32( 4 );
		rectf( &bigmask,bigmask.rect,F_XOR );

		for ( i=0; i<NICONS; i++ ) Icons[i]->handler( INIT );

		screen.base = addr( &display,Draw.r.origin );
		screen.width = display.width;
		screen.rect = Draw.r;
		bigscreen.base = addr( &display,Pt(0,0) );
		bigscreen.width = display.width;
		bigscreen.rect = Jrect;

		string( &defont,"Copyright 1985 AT&T Bell Laboratories",
			&display,add( Draw.r.origin,Pt(16,16) ),F_XOR );
		string( &defont,"All Rights Reserved",
			&display,
			add( Draw.r.origin,Pt(16,16+defont.height) ),F_XOR );
	   }
}

are_you_sure( tries )	/* like above, but requires "tries" clicks on button3	*/
	Word tries;	/*	to verify					*/
{
	Texture16 *t;
	int i,all_threes;

	t = cursswitch( &skull );

	for ( i=0,all_threes=TRUE; i<tries; i++ )
	      if ( !( all_threes &= ( ( click()==3 ) ? TRUE : FALSE ) ) ) break;

	cursswitch( t );
	return all_threes;
}
	
	
click()			/* waits for click of button, returns lowest button hit */
{
	Word result;

	waitbuttons( UP );
	waitbuttons( DOWN );
	for ( result=1; result<4; result++ )
	      if ( mouse.buttons & ( 8 >> result ) ) break;
	waitbuttons( UP );
	return result;
}
