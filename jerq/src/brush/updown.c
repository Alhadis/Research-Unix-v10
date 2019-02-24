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

Point alignment();

recall_pic()
{
	register FILE *fp;
	char *fname;
	Texture16 *tp;
	Bitmap *bp;
	Image NewPic;

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
		NewPic = MakeImage( bp,bp->rect,
					mouse.xy,alignment( bp->rect.origin ) );
		SetCurrentImage( NewPic );
		MoveImage( &CurrentImage,drawmode );
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

	old_set = helpswitch( sweep_set );
	r = sweeprect( Pt(1,1) );
	helpswitch( old_set );

	if ( !nullrect( r ) )
	   {
		if ( !(fname = type( "file name? ",pix_dir )) )
			return;
		if ( ( fp = fopen( fname,"w" ) ) == null(FILE) )
			errprintf( "can't open %s",fname );
		else {
			tp = (Texture16 *) cursswitch( &hourglass );
			putbitmap( &display,r,fp,&display );
			cursswitch( tp );
			fclose( fp );
		     }
	   }
}

#ifdef SAFEWAY
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
	helpswitch( old_set );

	if ( !nullrect( r ) )
	   {
		tp = ( Texture16 *) cursswitch( &hourglass );
		putbitmap( &display,r,fp,&display );
		cursswitch( tp );
	   }
	fclose( fp );
}

#endif
