/*
 *
 *	up/downloading files for Brush  --  brush/updown.c
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 */

#include "brush.h"

Bitmap *readbitmap();

extern char *pix_dir;		/* name of picture directory	*/
extern char *tex_dir;		/* name of texture directory	*/

save_text( tp,n )
	Texture16 *tp[];
	Word n;
{
	register FILE *fp;
	char *fname;
	Texture16 *oldcursor;
	Help old_set;

	if ( !(fname = type( "file name? ",tex_dir )) ) return;
	if ( ( fp = fopen( fname,"w" ) ) == 0 )
	   {
		errprintf( "can't open %s",fname );
		return;
	   }

	oldcursor = ( Texture16 *) cursswitch( &hourglass );
	puttextures( tp,n,fp );
	fclose( fp );
	cursswitch( oldcursor );
}

recall_text( tp,r,current,n )
	Texture16 *tp[];
	Rectangle r;
	Word current;		/* index of current value for hilighting */
	Word n;
{
	register FILE *fp;
	char *fname;
	Texture16 *oldcursor;
	Bitmap *bp;

	if ( !(fname = type( "file name? ",tex_dir )) ) return;

	if ( !( fp=fopen( fname,"r" ) ) )
	   {
		errprintf( "can't open %s",fname );
		return;
	   }

	oldcursor = ( Texture16 *) cursswitch( &hourglass );

	gettextures( tp,n,fp );
	fclose( fp );
	imagemenu( r,current,tp,n );
	cursswitch( oldcursor );
}

recall_pic()
{
	register FILE *fp;
	char *fname;
	Texture16 *tp;
	Bitmap *bp;

	if ( !(fname = type( "file name? ",pix_dir )) )
	     return;

	if ( ( fp=fopen( fname,"r" ) ) == null(FILE) )
	   {
		errprintf( "can't open %s",fname );
		return;
	   }

	tp = ( Texture16 *) cursswitch( &hourglass );

	if ( ( bp = readbitmap( fp ) ) != null( Bitmap ) )
	   {
		wait( MOUSE );
		Ibfree( Now_Pic.bp );
		Now_Pic = make_image( bp,bp->rect,
					mouse.xy,Pt(0,0) );
		position( &Now_Pic,drawmode );
	   }
	fclose( fp );
	cursswitch( tp );
}

save_pic()
{
	Rectangle r;
	register FILE *fp;
	char *fname;
	Texture16 *tp;
	Help old_set;

	if ( !(fname = type( "file name? ",pix_dir )) ) return;
	if ( ( fp = fopen( fname,"w" ) ) == null(FILE) )
	   {
		errprintf( "can't open %s",fname );
		return;
	   }

	old_set = helpswitch( sweep_set );
	r = sweeprect( Pt(1,1) );
	r.origin = align( r.origin,Pt( 0,0 ) );
	helpswitch( old_set );

	tp = ( Texture16 *) cursswitch( &hourglass );
	putbitmap( &display,r,fp,&display );
	fclose( fp );
	cursswitch( tp );
}
