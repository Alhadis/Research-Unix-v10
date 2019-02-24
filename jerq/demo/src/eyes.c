/*
	eyes -- public-domain implementation of a widget which follows the mouse
		around, for Blit-like terminals (5620 DMD and 630 MTG, at least)

	last edit:	89/09/12	D A Gwyn

	SCCS ID:	@(#)eyes.c	1.4

To compile:
	$ dmdcc -o eyes.m eyes.c	# -g -O also recommended

To run:
	$ dmdld eyes.m			# only useful in mpx (layers) mode
	$ dmdld eyes.m -l		# for local process on 630 only

To operate:
	Operates itself.  To terminate, select the "eyes" layer then type
	"q", or simply delete the layer.  If running as a local process
	on a 630, to terminate the process you must delete the layer.
	Works best in a small layer near the edge of the screen.

Credits:
	This program was inspired by the "xeyes" included with X11R3,
	which in turn was stolen from a NeWS demo at SIGGRAPH '88.
	However, the present code was written from scratch by
	Douglas A. Gwyn, who has placed it into the public domain.
*/

#ifndef lint
static char	SCCS_ID[] = "@(#)eyes.c	1.4 89/09/12";	/* for "what" utility */
#endif

#ifndef STATIC
#define	STATIC	static			/* empty string when debugging */
#endif


#ifdef DMD630

#include	<dmd.h>
#include	<5620.h>		/* (easier than thinking about it) */

#else	/* !DMD630 */			/* model 5620 assumed */

#include	<jerq.h>		/* old name for <dmd.h> */

/*#include	<sa.h>*/
#define	RealMouse	mouse		/* what a kludge! */

#define	Texture16	Texture
#ifndef T_background
STATIC Texture16	T_background =		/* background texture */
	{
	0x1111, 0x4444, 0x1111, 0x4444,
	0x1111, 0x4444, 0x1111, 0x4444,
	0x1111, 0x4444, 0x1111, 0x4444,
	0x1111, 0x4444, 0x1111, 0x4444
	};
#endif

#endif	/* DMD630 */

#ifndef INSET				/* thickness of layer border */
#define	INSET	(Drect.origin.x - display.rect.origin.x)
#endif


/* Tweakable parameter for process tuning: */

#define	PERIOD	2			/* dormancy cycle, in ticks (min. 2) */


/* Geometric parameters for drawing eye parts, in screen coordinates: */

/* Canonical units for drawing the eye parts: */
#define	X_UNIT		((Drect.corner.x - Drect.origin.x) / 9)
#define	Y_UNIT		((Drect.corner.y - Drect.origin.y) / 4)

/* Ellipse semi-axes: */
#define	PUPIL_A		unit.x
#define	PUPIL_B		unit.y
#define	BALL_A		(2 * unit.x)
#define	BALL_B		(2 * unit.y)

/* Ellipse centers: */
#define	BALL_LX		(INSET + 2 * unit.x)
#define	BALL_RX		(INSET + 7 * unit.x)
#define	BALL_Y		(INSET + 2 * unit.y)
/* (Pupil positions are computed dynamically.) */


#define	LEFT	0
#define	RIGHT	1

STATIC Point		ball[2];	/* eyeball centers wrt rect.origin */
STATIC Rectangle	eye_rect[2];	/* for limiting range of bitblt */
STATIC Bitmap		*image = 0;	/* copy of display for double-buffer */
STATIC int		lr;		/* determines which pupil to draw */
STATIC Point		mouse_loc;	/* recorded mouse position */
STATIC Point		pupil[2];	/* pupil centers wrt rect.origin */
STATIC Point		unit;		/* unit box size: (X_UNIT,Y_UNIT) */


/* LocPupil: Compute the correct pupil position for current mouse_loc. */

STATIC Point
LocPupil()
	{
	Point	disp;			/* eyeball_center-to-mouse vector */
	Point	ctr;			/* computed pupil center */
	int	dist;			/* scaled distance from eye center */
#define	SC	32			/* crude scaling factor (see below) */

	ctr = ball[lr];
	disp = sub( mouse_loc, add( display.rect.origin, ctr ) );
	dist = norm( muldiv( disp.x, SC, unit.x ), muldiv( disp.y, SC, unit.y ),
		     0
		   );

	/* "Google eyes" algorithm.  Note that no trig calls are necessary.
	   The primitive scaling sometimes results in the pupil being
	   drawn slightly over the edge of the eyeball (up to an SCth
	   of the pupil radius), but in practice it's not noticeable. */

	if ( dist < SC )		/* cursor sufficiently within eyeball */
		ctr = add( ctr, disp );	/* center pupil on cursor */
	else	{			/* roll pupil around edge of eyeball */
		ctr.x += muldiv( disp.x, SC, dist );
		ctr.y += muldiv( disp.y, SC, dist );
		}

	return ctr;
#undef SC
	}


/* XorPupil: Draw or erase the pupil at the already-computed position. */

STATIC void
XorPupil( b )
	register Bitmap	*b;		/* where to do the drawing */
	{
	eldisc( b, add( pupil[lr], b->rect.origin ), PUPIL_A, PUPIL_B, F_XOR );
	}


/* DrawPupil: Erase one old pupil and draw a new one that tracks the mouse. */

STATIC void
DrawPupil()
	{
	Point		new_pupil;	/* desired pupil position */
	register Bitmap	*b;

	mouse_loc = RealMouse.xy;
	new_pupil = LocPupil();

	if ( eqpt( pupil[lr], new_pupil ) )
		return;			/* pupil position is already correct */

	/* Pupil does need to be moved. */

	if ( image == 0 )
		b = &display;	/* draw directly in layer */
	else	{
		b = image;	/* draw changes in backing buffer */

		/* Clear the relevant piece of the backing buffer. */

		rectf( b, eye_rect[lr], F_CLR );
		}

	/* Erase the pupil at the old position. */

	if ( pupil[lr].x >= 0 )
		XorPupil( b );
	/* else first time for this pupil after Repaint(); no pupil to erase */

	/* Update the recorded pupil position. */

	pupil[lr] = new_pupil;

	/* Draw the pupil at the new position. */

	XorPupil( b );

	if ( image != 0 )	/* use backing buffer to update the display */
		bitblt( b, eye_rect[lr], &display, eye_rect[lr].origin, F_XOR );
	}


/* Repaint: Draw everything except the pupils. */

STATIC void
Repaint()
	{
	/* Clear the layer, including the border, to background texture. */

	texture/*16*/( &display, display.rect, &T_background, F_STORE );

	/* Compute canonical units for drawing the eye. */

	if ( (unit.x = X_UNIT) <= 0 )
		unit.x = 1;

	if ( (unit.y = Y_UNIT) <= 0 )
		unit.y = 1;

	/* Draw the whites of the eyes, also "eyeliner. */

	ball[LEFT].x = BALL_LX;
	ball[RIGHT].x = BALL_RX;

	for ( lr = RIGHT; lr >= LEFT; --lr )
		{
		Point	dis_ball;	/* display coords for eyeball center */

		ball[lr].y = BALL_Y;
		dis_ball = add( ball[lr], display.rect.origin );
		eldisc( &display, dis_ball, BALL_A, BALL_B, F_CLR );
		ellipse( &display, dis_ball, BALL_A + 1, BALL_B + 1, F_STORE );

		/* Also set up the smallest containing rectangle, for bitblt. */

		eye_rect[lr].origin.x = dis_ball.x - BALL_A;
		eye_rect[lr].corner.x = dis_ball.x + BALL_A;
		eye_rect[lr].origin.y = dis_ball.y - BALL_B;
		eye_rect[lr].corner.y = dis_ball.y + BALL_B;

		/* And set the "first time" indicators for DrawPupil(). */

		pupil[lr].x = -1;
		}

	/* Replace the old backing buffer, if necessary. */

	if ( image != 0 )
		if ( eqpt( sub( image->rect.corner, image->rect.origin ),
			   sub( display.rect.corner, display.rect.origin )
			 )		/* i.e. the same dimensions */
		   )
			image->rect = display.rect;	/* just update coords */
		else	{		/* release the old backing buffer */
			bfree( image );
			image = 0;	/* need a replacement */
			}

	if ( image == 0 )		/* try to allocate a backing buffer */
		image = balloc( display.rect );	/* may be 0; no problem */

	P->state &= ~(MOVED | RESHAPED);	/* display is now up to date */

	/* Pupils will be drawn later, in the main loop. */

	lr = LEFT;			/* left one first */
	}


main( argc, argv )
	int		argc;
	char		*argv[];
	{
	register int	kbd_res;	/* constant: 0 or KBD, depending */

#ifdef DMD630
	if ( argc > 1 )
		{
		(void)local();		/* detach from host channel */
		(void)cache( "eyes", 0 );
		kbd_res = 0;		/* delete local layer to terminate */
		}
	else
#endif
		kbd_res = KBD;		/* type `q' to terminate */

	request( kbd_res | MOUSE );	/* so we can tell if layer is current */

	P->state |= RESHAPED;		/* force initial display */

	while ( kbdchar() != 'q' )
		{
		static int	old_res = 0;	/* previous own() result */
		register int	new_res = own();

		/* If the "currency" state of this layer has toggled, the border
		   has changed and should be restored to background texture. */

		if ( ((new_res ^ old_res) & MOUSE) != 0	/* "current" toggled */
		  || (P->state & (MOVED | RESHAPED)) != 0 /* geometry change */
		   )
			Repaint();	/* (clears MOVED and RESHAPED) */

		old_res = new_res;

		/* At most one pupil is drawn per iteration, to reduce
		   interference with other processes sharing the terminal. */

		DrawPupil();		/* update one of the pupils */
		lr = LEFT + RIGHT - lr;	/* toggle which one to draw next */

		request( kbd_res );	/* let layersys handle button-3 hits */
		sleep( PERIOD );	/* relinquish the processor */
		request( kbd_res | MOUSE );	/* so we can tell if current */
		}

	if ( image != 0 )		/* release the backing buffer */ 
		bfree( image );		/* (not strictly necessary) */

	exit();
	}
