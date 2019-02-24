/*
 *
 *	icon handling for Brush  --  brush/icons.c
 *
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 */

#include "brush.h"

Word spraywords[] =   { 0x00000452,0x0003B108,0x00038A22,0x0007C188,
			0x00082054,0x00319820,0x00220808,0x00238800,
			0x00208800,0x00238800,0x00200800,0x00230800,
			0x00228800,0x00230800,0x00220800,0x00200800,
			0x00230800,0x00228800,0x00238800,0x00228800,
			0x00200800,0x00210800,0x00228800,0x00238800,
			0x00228800,0x00200800,0x00228800,0x00238800,
			0x00210800,0x00410C00,0x00400400,0x007FFC00,	};
Bitmap spraymap = { spraywords,1,{ { 0,0 },{ 32,32 } } };

Word dashwords[] =     { 0x01800000,0x01B00000,0x0C300000,0x0C000000,
			0x000C0000,0x180C0000,0x18000000,0x00060000,
			0x30060000,0x30000000,0x00030000,0x60030000,
			0x60000000,0x00018000,0x00018000,0xC0000000,
			0xC000C000,0x0000C000,0x00000003,0x00000003,
			0x00006000,0x00006000,0x00000006,0x00003006,
			0x00003000,0x00000018,0x00001818,0x00001800,
			0x00000030,0x00000C30,0x00000D80,0x00000180,	};
Bitmap dashmap = { dashwords,1,{ { 0,0 },{ 32,32 } } };

Word dotwords[] =     { 0x01800000,0x01B00000,0x0C300000,0x0C000000,
			0x000C0000,0x000C0000,0x00000000,0x00000000,
			0x30000000,0x30000000,0x00030000,0x00030000,
			0x00000000,0x00000000,0x00000000,0xC0000000,
			0xC000C000,0x0000C000,0x00000003,0x00000003,
			0x00000000,0x00000000,0x00000000,0x00003000,
			0x00003000,0x00000018,0x00001818,0x00001800,
			0x00000030,0x00000C30,0x00000D80,0x00000180,	};
Bitmap dotmap = { dotwords,1,{ { 0,0 },{ 32,32 } } };

Word curvewords[] =   { 0x03DC0000,0x06620000,0x0C710000,0x0C790000,
			0x18988000,0x18988000,0x310C4000,0x310C4000,
			0x310C4000,0x62062000,0x62062000,0x62062000,
			0xC4031000,0xC4031000,0xC4031000,0xC4031000,
			0x00018806,0x00018806,0x00018806,0x00018806,
			0x0000C40C,0x0000C40C,0x0000C40C,0x00006218,
			0x00006218,0x00006218,0x00003131,0x00003131,
			0x000018F2,0x000018E2,0x00000CC4,0x000007B8,	};
Bitmap curvemap = { curvewords,1,{ { 0,0 },{ 32,32 } } };

Word negwords[] =     { 0x003FF800,0x00FFFE00,0x03800380,0x070001C0,
			0x0E0000E0,0x1C0000F0,0x380001F8,0x7000039C,
			0x6000070C,0xC0000E06,0xC0001C06,0xC0003806,
			0xC0007006,0xC000E006,0xC001C006,0xC0038006,
			0xC0070006,0xC00E0006,0xC01C0006,0xC0380006,
			0xC0700006,0xC0E00006,0x61C0000C,0x7380001C,
			0x3F000038,0x1E000070,0x0E0000E0,0x070001C0,
			0x03800380,0x00FFFE00,0x003FF800,0x00000000,	};
Bitmap negmap = { negwords,1,{ { 0,0 },{ 32,32 } } };

Word smoothwords[] =   { 0x00000000,0x00000000,0x60000000,0x90000000,
			0x88000000,0x42000000,0x22000000,0x13200000,
			0x8AE00000,0x4AA00000,0x32260000,0x02290000,
			0x02290000,0x00293000,0x00294800,0x00094A00,
			0x00064980,0x000048C0,0x00004890,0x00003090,
			0x00000091,0x00000099,0x00000095,0x00000013,
			0x00000011,0x00000011,0x00000001,0x00000001,
			0x00000000,0x00000000,0x00000000,0x00000000,	};

Bitmap smoothmap  = { smoothwords,1,{ { 0,0 },{ 32,32 } } };

Word alignwords[] =   { 0x00000000,0x00000000,0x00000000,0x00000000,
			0x10000000,0x28000000,0x44000000,0x44000000,
			0x7C800000,0x44800000,0x44800000,0x00800000,
			0x00810000,0x00810000,0x00F90000,0x00010000,
			0x00011C00,0x00012200,0x00012000,0x00002000,
			0x00002644,0x00002264,0x00001E54,0x00000054,
			0x0000004C,0x00000044,0x00000044,0x00000000,
			0x00000000,0x00000000,0x00000000,0x00000000,	};

Bitmap alignmap = { alignwords,1,{ { 0,0 },{ 32,32 } } };

Word brusher(),shader(),drawer(),sprayer(),
     smoother(),styler(),moder(),helper(),
     phonter(),unixer(),copier(),flexor(),aligner(),squarer();

Icon Brush = { {{0,0},{0,0}},0,brusher };
Icon Shade = { {{0,0},{0,0}},7,shader };
Icon Draw  = { {{0,0},{0,0}},0,drawer };
Icon Spray = { {{0,0},{0,0}},0,sprayer };
Icon Smooth= { {{0,0},{0,0}},TRUE,smoother };
Icon Style = { {{0,0},{0,0}},NORMAL,styler };
Icon Mode  = { {{0,0},{0,0}},1,moder };
Icon Helper= { {{0,0},{0,0}},0,helper };
Icon Phont = { {{0,0},{0,0}},0,phonter };
Icon Copy  = { {{0,0},{0,0}},0,copier };
Icon Unix  = { {{0,0},{0,0}},0,unixer };
Icon Flex  = { {{0,0},{0,0}},0,flexor };
Icon Align = { {{0,0},{0,0}},TRUE,aligner };
Icon Square= { {{0,0},{0,0}},FALSE,squarer };

Icon *Icons[NICONS] = { &Brush,&Shade,&Draw,&Style,
			&Spray,&Smooth,&Mode,&Helper,
			&Phont,&Unix,&Copy,&Flex,&Align,&Square };

icon_handler()
{
	int i;

	for ( i=0; i<NICONS; i++ )
		if ( ptinrect( mouse.xy,Icons[i]->r ) )
		   {
			Icons[i]->handler( SELECT );
			break;
		   }
	waitbuttons( UP );
}

#define TPOS(r,v,num)	add( r.origin,Pt(4,4+v*(RHEIGHT(r)/num)) )

brusher( msg )		/* handler for Brush menu icon	*/
	Word msg;
{
	Word n,oldval;

	switch( msg )
	      {
		case INIT:	Brush.r = Drect;
				Brush.r.origin.x = Brush.r.corner.x - MWIDTH;
				Brush.r.origin.y += MHEIGHT;
				imagemenu( Brush.r,Brush.value,brushes,NBRUSHES );
				break;
		case SELECT:	oldval = Brush.value;
				if ( ( n = select( Brush.r,NBRUSHES ) ) != -1 )
		   		   {
					Brush.value = n;
					highlight( TPOS(Brush.r,oldval,NSHADES ) );
					highlight( TPOS(Brush.r,n,NSHADES ) );
					setbrush( brushes[n] );
				   }
				break;
	      }
}

shader( msg )		/* handler for Shade menu icon	*/
	Word msg;
{
	Word n,oldval;

	switch( msg )
	      {
		case INIT:	Shade.r = Drect;
				Shade.r.origin.y += MHEIGHT;
				Shade.r.corner.x = Shade.r.origin.x + MWIDTH;
				imagemenu( Shade.r,Shade.value,shades,NSHADES );
				break;
		case SELECT:	oldval = Shade.value;
				if ( ( n = select( Shade.r,NSHADES ) ) != -1 )
		   		   {
					Shade.value = n;
					highlight( TPOS(Shade.r,oldval,NSHADES ) );
					highlight( TPOS(Shade.r,n,NSHADES ) );
					shade = shades[n];
				   }
				break;
	      }
}


Rectangle
init_rect( pos,size )
	Point pos,size;
{
	Rectangle r;

	r.origin = add( Drect.origin,pos );
	r.corner = add( r.origin,size );
	return r;
}

helper( msg )		/* handler for drawing Helper icon	*/
	Word msg;
{
	Point p;
	Word n;

	switch( msg )
	      {
		case INIT:	Helper.r = init_rect( Pt( 6,6 ),Pt( 80,28 ) );
				rectf( &display,Helper.r,F_STORE );
				draw_help( curr_set );
				break;
		case SELECT:	break;
	      }
}

smoother( msg )		/* handler for Smooth icon	*/
	Word msg;
{
	Point p;

	switch( msg )
	      {
		case INIT:	Smooth.r = init_rect( Pt( 105,4 ),Pt( 32,32 ) );
				bitblt( &smoothmap,smoothmap.rect,
					&display,Smooth.r.origin,F_STORE );
				if ( !smoothing() )
					bitblt( &negmap,negmap.rect,
						&display,Smooth.r.origin,F_XOR );
				break;
		case SELECT:	Smooth.value ^= TRUE;
				bitblt( &negmap,negmap.rect,
					&display,Smooth.r.origin,F_XOR );
				break;
	      }
}

Texture16 squiggle = {
			0x0001, 0x003A, 0x0044, 0x0040,
			0x0040, 0x0020, 0x0010, 0x3C08,
			0x4608, 0x4190, 0x4060, 0x2000,
			0x2000, 0x1000, 0x1000, 0xE000,
		     };
Texture16 straight = {
			0xFFFF, 0x8000, 0x8000, 0x8000,
			0x8000, 0x8000, 0x8000, 0x8000,
			0x8000, 0x8000, 0x8000, 0x8000,
			0x8000, 0x8000, 0x8000, 0x8000,
		     };

Bitmap *sprayicons[] = { &curvemap,&dashmap,&dotmap,&spraymap };

sprayer( msg )		/* handler for Spray icon	*/
	Word msg;
{
	static Word style = 0;
	static Word stylebits[] = { CONTBIT,CONTBIT|DASHBIT,DASHBIT,SPRAYBIT };

	switch( msg )
	      {
		case INIT:	Spray.r = init_rect( Pt( 260,4 ),Pt( 32,32 ) );
				Spray.value = stylebits[style];
				bitblt( sprayicons[style],spraymap.rect,
					&display,Spray.r.origin,F_STORE );
				break;
		case SELECT:	style = (style+1)%4;
				Spray.value = stylebits[style];
				bitblt( sprayicons[style],spraymap.rect,
					&display,Spray.r.origin,F_STORE );
				break;
	      }
}

aligner( msg )		/* handler for Align icon	*/
	Word msg;
{
	switch( msg )
	      {
		case INIT:	Align.r = init_rect( Pt( 154,4 ),Pt( 32,32 ) );
				bitblt( &alignmap,alignmap.rect,
					&display,Align.r.origin,F_STORE );
				if ( !aligning() )
					bitblt( &negmap,negmap.rect,
						&display,Align.r.origin,F_XOR );
				break;
		case SELECT:	Align.value ^= TRUE;
				bitblt( &negmap,negmap.rect,
					&display,Align.r.origin,F_XOR );
				break;
	      }
}

char *emph_text[] = { "normal text","outline text",
			"bold text  ","shadow text","italic text",NULL };
Menu emph_menu = { emph_text };

styler( msg )		/* handler for font Style icon	*/
	Word msg;
{
	Word n;

	switch( msg )
	      {
		case INIT:	Style.r = init_rect( Pt( 516,10 ),Pt( 110,25 ) );
				rectf( &display,Style.r,F_CLR );
				textblt( &defont,emph_text[Style.value],
					 &display,Style.r.origin,Style.value );
				break;
		case SELECT:	n = menuhit( &emph_menu,3 );
				if ( n != -1 ) Style.value = n;
				rectf( &display,Style.r,F_CLR );
				textblt( &defont,emph_text[Style.value],
					 &display,Style.r.origin,Style.value );
				break;
	      }
}

int drawmode   = F_OR;
int modes[]    = { F_XOR,F_OR,F_STORE,F_AND,F_COPY };
char *mtext2[] = { "xor mode  ","or mode   ","store mode",
		   "and mode  ","copy mode ",NULL };
Menu modemenu  = { mtext2 };

moder( msg )		/* handler for drawing Mode icon	*/
	Word msg;
{
	Word n;
	char buf[20];

	switch( msg )
	      {
		case INIT:	Mode.r = init_rect( Pt( 409,10 ),Pt( 90,25 ) );
				string( &defont,mtext2[Mode.value],
					&display,Mode.r.origin,F_STORE );
				break;
		case SELECT:	n = menuhit( &modemenu,3 );
				if ( n != -1 ) Mode.value = n;
				string( &defont,mtext2[Mode.value],
					&display,Mode.r.origin,F_STORE );
				drawmode = modes[Mode.value];
				break;
	      }
}

extern char *ftext[];

phonter( msg )		/* handler for drawing Phont icon	*/
	Word msg;
{
	switch( msg )
	      {
		case INIT:	Phont.r = init_rect( Pt( 643,10 ),Pt( 60,25 ) );
				rectf( &display,Phont.r,F_CLR );
				string( &defont,ftext[Phont.value],
					&display,Phont.r.origin,F_STORE );
				break;
		case SELECT:	pick_font();
				rectf( &display,Phont.r,F_CLR );
				string( &defont,ftext[Phont.value],
					&display,Phont.r.origin,F_STORE );
				break;
	      }
}

char *ctext[]    = { "same","magnify","shrink","flip",
		     "rotate","slant","outline","new",NULL };
Menu copymenu    = { ctext };

char *fliptext[] = { "flip x","flip y",NULL };
Menu flipmenu    = { fliptext };

char *rottext[]  = { "clockwise","counter",NULL };
Menu rotmenu     = { rottext };

#define SHEAR	6
#ifdef JERQ
#define lasty	prevhit
#endif

#ifndef OLDROM
#define lasty	prevhit
#endif

copier( msg )		/* handler for drawing Copy icon	*/
	Word msg;
{
	Word choice,stat;

	switch( msg )
	      {
		case INIT:	Copy.r=init_rect( Pt( 195,10 ),Pt( 40,33 ) );
				string( &defont,"image",
					&display,Copy.r.origin,F_STORE );
				break;
		case SELECT:	copymenu.lasty = 0;
				choice = menuhit( &copymenu,3 );
				if ( choice == -1 ) break;
				switch( choice )
				      {
					case 0:	stat = SUCCESS;
						break;
					case 1: stat = blowup( CurrentImage );
						break;
					case 2: stat = crunch( CurrentImage );
						break;
					case 3:	switch( menupick3( &flipmenu ) )
						{
						  case 0: stat=flippit( CurrentImage,
									   FLIPX );
							  break;
						  case 1: stat=flippit( CurrentImage,
									   FLIPY );
						}
						break;
					case 4:	switch( menupick3( &rotmenu ) )
						{
						  case 0: stat=mrotate(CurrentImage);
							  break;
						  case 1: stat=protate(CurrentImage);
						}
						break;
					case 5: stat = shear( CurrentImage );
						break;
					case 6: stat = edgeit( CurrentImage );
						break;
					case 7: stat = new_image();
						break;
				      }
				if ( stat==SUCCESS )
					MoveImage( &CurrentImage,drawmode );
	      }
	waitbuttons( UP );
	cursswitch( brush );
}

char *utext[]  = { "save","recall","exit",NULL };
Menu unixmenu  = { utext };

char *utext2[] = { "bitmap","shades","brushes",NULL };
Menu umenu2    = { utext2 };

unixer( msg )		/* handler for drawing Unix icon	*/
	Word msg;
{
	Help old_set;

	switch( msg )
	      {
		case INIT:	Unix.r = init_rect( Pt( 720,10 ),Pt( 40,25 ) );
				string( &defont,"i/o",
					&display,Unix.r.origin,F_STORE );
				break;
		case SELECT:	switch( menuhit( &unixmenu,3 ) )
				{
				  case 0: switch( menupick3( &umenu2 ) )
					  {
					    case 0: save_pic();
						    break;
					    case 1: save_text( shades,NSHADES );
						    break;
					    case 2: save_text( brushes,NBRUSHES );
						    break;
					   }
					  break;
				  case 1: switch( menupick3( &umenu2 ) )
					  {
					    case 0: recall_pic();
						    break;
					    case 1: recall_text( shades,
							         Shade.r,
								 Shade.value,
								 NSHADES);
						    break;
					    case 2: recall_text( brushes,
							         Brush.r,
								 Brush.value,
								 NBRUSHES);
						    setbrush( brush );
						    break;
					   }
					  break;
				  case 2: old_set = helpswitch( exit_set );
					  if ( are_you_sure( 2 ) )  exit();
					  helpswitch( old_set );
					  break;
				}
	      }
}

char *flextext[] = { "off","y=0","x=0","both",NULL };
Menu flexmenu    = { flextext };

flexor( msg )		/* handler for drawing Flex icon	*/
	Word msg;
{
	switch( msg )
	      {
		case INIT:	Flex.r = init_rect( Pt( 358,4 ),Pt( 34,34 ) );
				show_flex( Flex.r.origin );
				break;
		case SELECT:	switch( menuhit( &flexmenu,3 ) )
				      {
					case 0:	Flex.value = 0;
						break;
					case 1: Flex.value = XAXIS;
						break;
					case 2:	Flex.value = YAXIS;
						break;
					case 3:	Flex.value = XAXIS|YAXIS;
						break;
					default: return;
				      }
				show_flex( Flex.r.origin );
	      }
}

show_flex( p )
	Point p;
{
	static Rectangle box16 = { { 0,0 },{ 16,16 } };

	rectf( &display,Flex.r,F_CLR );
	tx16blt( &drawcursor,box16,
		 &display,add( p,Pt(16,0) ),F_STORE );
	if ( yreflect(Flex.value) )
		flip_x( &display,raddp( Rect( 16,0,32,16 ),p ),
			&display,p );
	if ( xreflect(Flex.value) )
		flip_y( &display,raddp( Rect( 0,0,32,16 ),p ),
			&display,add( p,Pt(0,16) ) );
}

squarer( msg )		/* handler for Square icon	*/
	Word msg;
{
	Point p;

	switch( msg )
	      {
		case INIT:	Square.r = init_rect( Pt( 309,4 ),Pt( 32,32 ) );
				p = Square.r.origin;
				tx16blt( &drawcursor,Rect(0,0,16,16),
					 &display,add( p,Pt(16,0) ),F_STORE );
				if ( Square.value )
				     tx16blt( &straight,Rect(0,0,16,16),
					      &display,add( p,Pt(0,16) ),F_STORE );
				else tx16blt( &squiggle,Rect(0,0,16,16),
					      &display,add( p,Pt(0,16) ),F_STORE );
				break;
		case SELECT:	Square.value ^= TRUE;
				p = Square.r.origin;
				if ( Square.value )
				     tx16blt( &straight,Rect(0,0,16,16),
					      &display,add( p,Pt(0,16) ),F_STORE );
				else tx16blt( &squiggle,Rect(0,0,16,16),
					      &display,add( p,Pt(0,16) ),F_STORE );
				break;
	      }
}

drawer( msg )		/* handler for Draw icon	*/
	Word msg;
{
	switch( msg )
	      {
		case INIT:	Draw.r.origin = add( DORG,Pt( MWIDTH,MHEIGHT ) );
				Draw.r.corner = sub( DCOR,Pt( MWIDTH,0 ) ) ;
				break;
		case SELECT:	main_menu();		
				break;
	      }
}

#define LINES		0
#define CURVES		1
#define BOX		2
#define CIRCLE		3
#define DISC		4
#define STRING		5
#define FASTFILL	6
#define CLEAR		7
#define TEXTURE		8
#define INVERT		9
#define FADE		10
#define NEWBRUSH	11
#define NEWSHADE	12
#define DETAILS		13

char *mtext[] = { "lines","curves","box","ellipse","disc",
		  "string","fill","clear","texture","invert",
		  "fade","new brush","new shade",
		  "details",NULL };

Menu mainmenu = { mtext };

char *edsntext[] = { "edit","snarf",NULL };

Menu edsnmenu    = { edsntext };

main_menu()
{
	int op,n,stat;

	cursswitch( null( Texture16 ) );
	switch( op = menuhit( &mainmenu,3 ) )
	      {
		case LINES:	dolines();
				break;
		case CURVES:	do_splines();
				break;
		case CIRCLE:	stat = discs( CurrentImage,UNFILLED );
				if ( stat==SUCCESS )
					MoveImage( &CurrentImage,drawmode );
				break;
		case DISC:	stat = discs( CurrentImage,FILLED );
				if ( stat==SUCCESS )
					MoveImage( &CurrentImage,drawmode );
				break;
		case STRING:	stat = text( CurrentImage,drawmode );
				if ( stat==SUCCESS )
					MoveImage( &CurrentImage,drawmode );
				break;
		case FASTFILL:	fastfill();
				break;
		case CLEAR:    
		case TEXTURE:  
		case INVERT:   
		case BOX:   
		case FADE:	rect_op( op );
				break;
		case NEWBRUSH:	if ( (n = menupick3( &edsnmenu ))<0 ) break;
				*brush = (n==0) ? edit_text( brush )
						: snarf_text( brush );
				setbrush( brush );
				imagemenu( Brush.r,Brush.value,brushes,NBRUSHES );
				break;
		case NEWSHADE:	if ( (n = menupick3( &edsnmenu ))<0 ) break;
				*shade = (n==0) ? edit_text( shade )
						: snarf_text( shade );
				imagemenu( Shade.r,Shade.value,shades,NSHADES );
				break;					 
		case DETAILS:	details();
				break;
	     }
	cursswitch( brush );
	waitbuttons( UP );
}

menupick3( mp )
	Menu *mp;
{
	Texture16 *tp;
	Help old_set;
	int n = -1;

	tp = (Texture16 *)cursswitch( &menucursor );
	old_set = helpswitch( menu_set );

	for ( ; !bttn123(); wait(CPU) );

	if ( bttn3() ) n = menuhit( mp,3 );

	cursswitch( tp );
	helpswitch( old_set );
	return n;
}

Point alignment();

rect_op( op )
	Word op;
{
	Bitmap *bp;
	Help old_set;
	Rectangle r;
	Point p;
	Texture16 *tp;
	Image NewImage;

	old_set = helpswitch( sweep_set );

	r = sweeprect( Pt(1,1) );

	if ( !nullrect( r ) )
		switch( op )
		      {
			case CLEAR:	rectf( &screen,r,F_CLR );
					break;
			case TEXTURE:	if ( bp = balloc( r ) )
					   {
						texture16( bp,r,shade,F_STORE );
						NewImage = MakeImage( bp,r,
							r.origin,
							alignment(r.origin) );
					     SetCurrentImage( NewImage );
					     MoveImage( &CurrentImage,drawmode );
					   }
					break;
			case INVERT:	rectf( &screen,r,F_XOR );
					break;
			case FADE:	fade( r );
					break;
			case BOX:	r = inset( r,-8 );
					p = r.origin;

					if ( aligning() ) r = rsubp( r,p );
					if ( bp = balloc( r ) )
					   {
						tp = (Texture16 *)
							cursswitch(&hourglass);
						rectf( bp,bp->rect,F_CLR );
						drawbox( bp,inset( r,8 ),F_STORE );
						NewImage = MakeImage( bp,bp->rect,
							p,
							alignment(r.origin) );
						SetCurrentImage( NewImage );
					        MoveImage( &CurrentImage,drawmode );
						cursswitch( tp );
					   }
					break;
		      }
	helpswitch( old_set );
}

drawbox( bp,r,code )	/* draw rect r ( D coords ) using function code	*/
	Bitmap *bp;
	Rectangle r;
	Word code;
{
	pline( bp,RORG,Pt(RCOR.x,RORG.y),code,0 );
	pline( bp,Pt(RCOR.x,RORG.y),RCOR,code,0 );
	pline( bp,RCOR,Pt(RORG.x,RCOR.y),code,0 );
	pline( bp,Pt(RORG.x,RCOR.y),RORG,code,0 );
}

fade( r )
	Rectangle r;
{
	rectf( &bigmask,bigmask.rect,F_XOR );

	helpswitch( fade_set );

	for ( ; !bttn3(); wait(CPU) )
		for ( ; bttn2(); wait(CPU) )
			splatter( &screen,r );

	rectf( &bigmask,bigmask.rect,F_XOR );
}
		  
