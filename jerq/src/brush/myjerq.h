/*
 *
 *	extensions to jerq.h for Brush   --  brush/myjerq.h
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 */

#include <jerq.h>

#ifdef J5620
#ifdef OLDROM
#define button1()	bttn1()
#define button2()	bttn2()
#define button3()	bttn3()
#define button1_3()	bttn123()
#define button12()	bttn12()
#define button23()	bttn23()
#define button13()	bttn13()
#endif

#define Texture32	Texture

#endif

#ifdef JERQ
#define Texture16	Texture

#define	texture16( bp,r,t,c )	texture( bp,r,t,c )


#define bttn1()		button1()
#define bttn2()		button2()
#define bttn3()		button3()
#define bttn123()	button123()
#define bttn12()	button12()
#define bttn23()	button23()
#define bttn13()	button13()
#endif


#define allbuttons	((mouse.buttons&7)==7)
#define downbutton(n)	((n>=0&&n<=3)?(mouse.buttons&(8>>n)):0)
#define upbutton(n)	(!downbutton(n))

#define extent(r)	sub(r.corner,r.origin)
#define midpt(r)	div(add(r.origin,r.corner),2)
#define nullrect( r )	eqpt(r.origin,r.corner)

#define	RHEIGHT(r)	(r.corner.y-r.origin.y)
#define RWIDTH(r)	(r.corner.x-r.origin.x)

#define	UP	0
#define	DOWN	1

#define	FALSE	0
#define	TRUE	1

#define ALL( items,list )	items=list; items; items=items->next
#define mjstring( s,p,c )	string(&defont,s,&display,xformpt(p,Jrect,Drect),c)


#define	waitbuttons(updown) { do wait(CPU); while((bttn123()!=0) != updown); }

#define jstringat(s,p)	{ jmoveto( p ); jstring( s ); }

#define sign(n)		((n>0) ? 1 : ((n<0) ? -1 : 0 ))

#define null(type)	((type *)0)
#define talloc(type)	( (type *) alloc( sizeof( type ) ) )
#define tfree(ptr)	free((char *)ptr)

#define DIST(p,q)	(norm(sub(p,q),0))

char *type();
Rectangle xformrect(),makerect(),centerect();
Point xformpt(),jrubberpt(),makept();


/*
 * 	global symbols from mpx's turf
 *      these are dependant upon the particular release of blit code
 *
 *	to find them: mnm -g /usr/jerq/lib/layersys | grep symbolname
 *
 */
#ifdef JERQ
#define	proctab		((struct Proc *)0x51b4)
#define lkbdproc	(*((struct Proc **)0x4f9c))
#define menutext	((char **)0x5002)
#endif

#ifdef J5620

#define Screen		((Bitmap *)0116250)
#define kbdproc		(*((struct Proc **)034354640))
#define windowmenu	((struct Menu *)034356600)
#define menutext	(windowmenu->item)
#define proctab		((struct Proc *)034356714)
#define mr_mouse	((struct Mouse *)034312434)	/* global mouse pos */

#endif

/*
 *
 *	define transform to take pt from J <--> D coords
 *
 */

#define JtoD(p)		xformpt( p,Jrect,Drect )
#define DtoJ(p)		xformpt( p,Drect,Jrect )
#define DtoJrect(r)	xformrect(r,Drect,Jrect)
#define JtoDrect(r)	xformrect(r,Jrect,Drect)


