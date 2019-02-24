/*
 *
 *	text for Brush  --  brush/text.c
 *
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 */

#include "brush.h"

Rectangle str_rect();

Font *curr_font = null( Font );

extern char *font_dir;		/* expanded name of $DMD/font */

#define MAXFONTS	8

char *ftext[] = { "defont","new",NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL };
Menu fontmenu = { ftext };

Word newfont = 1;
Font *fontlist[] = { null(Font),null(Font),null(Font),null(Font),
		     null(Font),null(Font),null(Font),null(Font),null(Font) };

pick_font()
{
	Word choice;

	while( !bttn3() ) wait( CPU );

	if ( ( choice=menuhit( &fontmenu,3 ) ) > -1 )
		if ( choice == newfont )
			if ( newfont >= MAXFONTS )
			   {
				errmsg( "too many fonts" );
				return;
			   }
			else	new_font();
		else {
			Phont.value = choice;
			curr_font = fontlist[choice];
		     }
}

new_font()
{
	char *s,*t,name[50];
	Font *fp;
	Texture16 *tp;

	if ( !( s = type( "font? ","" ) ) ) return;

	sprintf( name,"%s/%s",font_dir,s );

	tp = (Texture16 *) cursswitch( &hourglass );
	fp = getfont( name );
	cursswitch( tp );

	if ( fp )
	   {
		curr_font = fontlist[newfont] = fp;
		Phont.value = newfont;
		t = alloc( strlen(s) + 1 );
		strcpy( t,s );
		ftext[newfont] = t;
		ftext[++newfont] = "new";
	   }

	else	errprintf( "can't find  %s",name );
}

text( oldpic,drawmode )
	Image oldpic;
	Word drawmode;
{
	Word emphasis;
	char *s,str[50];
	Rectangle r;
	Bitmap *tmp;

	if ( s = type( "text? ","" ) )
	   {
		strcpy( str,s );
		r = raddp( str_rect( curr_font,str,textyle() ),mouse.xy );
		if ( tmp = balloc( r ) )
		   {
			rectf( tmp,r,F_CLR );
			textblt( curr_font,str,tmp,r.origin,textyle() );
			oldpic = MakeImage( tmp,r,mouse.xy,Pt(0,0) );
			SetCurrentImage( oldpic );
			return SUCCESS;
		   }
	   }
	return FAILURE;
}

textblt( fp,s,bp,p,emphasis )
	Font *fp;
	char *s;
	Bitmap *bp;
	Point p;
	Word emphasis;
{
	switch( emphasis )
	      {
		case NORMAL:	string( fp,s,bp,p,F_STORE );
				break;
		case SHADOW:	string( fp,s,bp,p,F_OR );
				string( fp,s,bp,add( p,Pt( 1,1 ) ),F_CLR );
				string( fp,s,bp,add( p,Pt( 2,2 ) ),F_OR );
				break;
		case OUTLINE:	string( fp,s,bp,add( p,Pt( 0,1 ) ),F_OR );
				string( fp,s,bp,add( p,Pt( 2,1 ) ),F_OR );
				string( fp,s,bp,add( p,Pt( 1,0 ) ),F_OR );
				string( fp,s,bp,add( p,Pt( 1,2 ) ),F_OR );
				string( fp,s,bp,add( p,Pt( 1,1 ) ),F_CLR );
				break;
		case BOLD:	string( fp,s,bp,p,F_OR );
				string( fp,s,bp,add( p,Pt( 1, 0 ) ),F_OR );
				break;
		case ITALIC:	string( fp,s,bp,p,F_OR );
				horshear( bp,raddp(str_rect(fp,s,ITALIC),p),
					  fp->height/2,TRUE );
				break;
	      }
}

Rectangle
makeRect( x0,y0,x1,y1 )
	short x0,y0,x1,y1;
{
	Rectangle r;

	r.origin.x = x0;
	r.origin.y = y0;
	r.corner.x = x1;
	r.corner.y = y1;
	return r;
}

Rectangle
str_rect( fp,str,emphasis )	/* returns rect big enuff for str to fit	*/
	Font *fp;
	char *str;
	Word emphasis;
{
	switch( emphasis )
	      {
		case NORMAL:	return makeRect( 0,0,strwidth(fp,str),fp->height );
				break;
		case SHADOW:	
		case OUTLINE:	return makeRect( 0,0,
						 strwidth(fp,str)+2,fp->height+2 );
				break;
		case BOLD:	return makeRect( 0,0,
						 strwidth(fp,str)+2,fp->height );
				break;
		case ITALIC:	return makeRect( 0,0,
						 strwidth(fp,str)+2+fp->height/2,
						 fp->height );
				break;
	      }
}

Point
align( p,boundary )		/* align point p to boundary */
	Point p,boundary;
{
	if ( !aligning() || eqpt( boundary,Pt(-1,-1) ) ) return p;
	p.x = ( p.x & 0xFFF0 ) + boundary.x;
	p.y = ( p.y & 0xFFF0 ) + boundary.y;
	return p;
}

