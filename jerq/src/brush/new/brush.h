/*
 *
 *	includes for Brush  --  brush/brush.h
 *
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 */

#include "myjerq.h"
#include <jerqio.h>
#include <font.h>

#define F_AND		((Code)4)	/* target &= source	*/

#define spraying()	(Spray.value)
#define smoothing()	(Smooth.value)
#define aligning()	(Align.value)
#define squaring()	(Square.value)
#define textyle()	(Style.value)

#define XAXIS		1
#define YAXIS		2
#define xreflect(axis)	(axis&XAXIS)
#define yreflect(axis)	(axis&YAXIS)

#define RectAt( r,p )	raddp( rsubp(r,r.origin),p )

#define erreturn(msg)	{ errmsg( msg ); return; }
#define realmode(mode)	((bttn2())?F_CLR:mode)

#define BRUSHRECT	Rect(-8,-8,8,8)

#define NSHADES		28
#define NBRUSHES	28
#define NICONS		14

#define FLIPX		0
#define FLIPY		1

/* type styles */

#define NORMAL		0
#define OUTLINE		1
#define BOLD		2
#define SHADOW		3
#define ITALIC		4

/* disc styles */

#define UNFILLED	0
#define FILLED		1

#define MWIDTH		24
#define MHEIGHT		40

#define DORG		Drect.origin
#define DCOR		Drect.corner
#define RORG		r.origin
#define RCOR		r.corner

#define EXPAND		1

/* Icon messages */

#define INIT		0
#define SELECT		1

typedef
struct Icon	{
			Rectangle r;
			Word value;
			Word (*handler)();
		} Icon;

extern Icon Brush,Shade,Draw,Spray,Smooth,Style,
	    Mode,Helper,Phont,Flex,Align,Square,*Icons[];

typedef
struct Help	{
			Texture16 *text[3];
		} Help;

extern Help	sweep_set,pos_set,point_set,default_set,
		curr_set,exit_set,kill_set,fade_set,menu_set,
		helpswitch(),circ_set,snarf_set;

typedef
struct Image	{
			Bitmap		*bp;
			Rectangle	r;
			Point		pos;
			Point		alg;
		} Image;

extern Image make_image(),discs(),get_image(),blowup(),text(),flippit(),
		rotate(),shear();

extern Image Now_Pic,NoPic;

/*	declare our brush masks as 16 bit textures	*/

extern Texture16 circle3,circle5,circle7,dot2,dot4,dot6,dot8,bar6,bar10,bar14,
		 dash6,dash10,dash14,
		 diag3,ldiag3,grp3,lgrp3,slash16,slash12,slash8,
		 null1,null2,null3,null4,
		 null5,null6,null7,null8;

extern Texture16 *brush,*brushes[],edit_text(),snarf_text();

/*	declare our shades as 16 bit textures	*/

extern Texture16 white,grey1,grey2,grey3,grey4,grey5,grey6,grey7,black,
		 bigcheck,bigrcheck,bigstcheck,cross,thinvs,thickvs,
		 thinhs,thickhs,star,diags,zig,tictac,blot1,blot2,tactic,zag,
		 smile2,smile3,smile4,scowley,cube,xhatch,deathstar,leafs,woven;

extern Texture16 *shade,*shades[];

/* don't forget the cursor icons */

extern Texture16 skull,menucursor,sweepcursor,hand,sweepcircle,
		 drawcursor,erasecursor,thumbsdown,smile2,scowley,
		 hourglass;

extern Bitmap screen,bigscreen,mask,notmask,spraymask,bigmask;
extern Font *curr_font,*fontlist[];


Point rubberpt(),xformpt(),constrain(),makept(),mod16(),align(),tweak();
Rectangle sweeprect();
extern Word maskwords[],notmaskwords[];
extern int drawmode;
