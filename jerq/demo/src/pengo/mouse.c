/*
	mouse looker after...

	Self contained mouse handler
*/

#include "pengo.h"

char	*menu3text[] = { "Pause", "Stats", "New Game", "Quit", "Auto", NULL };

#define M_PAUSE	0
#define	M_STATS	1
#define	M_NEW	2
#define	M_QUIT	3
#define M_AUTO	4	/* this is a bozo convention */
int autoflag;

Menu	menu3 = { menu3text };
Texture skull = {
	 0x03C0, 0x0660, 0x0810, 0x0A50,
	 0x1248, 0x1008, 0x1998, 0x0C30,
	 0x0000, 0x0180, 0x0660, 0x0420,
	 0x0020, 0x00C0, 0x0100, 0x0100,
};

Texture sunset = {
	0x5006, 0xA819, 0x00A0, 0x04A0,
	0x049F, 0x12A4, 0x0808, 0x03E0,
	0x2412, 0x0808, 0x0808, 0x3FFF,
	0x3C1F, 0x7E7E, 0x783E, 0xFCFC,
};

Texture sleepz = {
	0xF800, 0xF800, 0x1800, 0x3000,
	0x67C0, 0xC7C0, 0xF8C0, 0xF980,
	0x033E, 0x063E, 0x07C6, 0x07CC,
	0x0018, 0x0030, 0x003E, 0x003E,
};

Texture	grey = {
	0xAAAA, 0x5555, 0xAAAA, 0x5555,
	0xAAAA, 0x5555, 0xAAAA, 0x5555,
	0xAAAA, 0x5555, 0xAAAA, 0x5555,
	0xAAAA, 0x5555, 0xAAAA, 0x5555,
};

Texture	*old;

Rectangle	graph;
Rectangle	graphc;
Rectangle	graphr;
Rectangle	graphb;

short	gstore[6 * 120];	/* storage for tmp store */

Bitmap	graphs	= { (Word *) gstore, shorts(6), { { 0, 0 }, { 16 * 5, 120 } } };

wait_mouse()
{
	while (!(own() & MOUSE))
	{
		wait(CPU);
		old_mouse = mouse.xy;
	}
	hit = button1() ? TRUE : hit;
	if (button3())
	{
		old = cursswitch((Word *)0);
		switch (menuhit(&menu3, 3))
		{
		case M_AUTO:
			autoflag = !autoflag;
			if(autoflag)
				menu3text[M_AUTO] = "Manual";
			else
				menu3text[M_AUTO] = "Auto";
			break;
		case M_QUIT:
			cursswitch(&sunset);
			while (!button12())
				if (button3())
					exit();
				else
					wait(CPU);
			break;
		case M_NEW:
			cursswitch(&skull);	/* pause cursor */
			while (!button12())
				if (button3())
				{
					extern	int	reshaped;

					cursswitch(old);
#ifdef	JERQ
#ifndef	MPX
/*
 *	Don't know why these lines have to be here, but only on
 *	stand alone jerq the 'cursswitch(old)' leaves the old
 *	cursor in place... very strange.
 */
					cursswitch(char_space);
#endif	MPX
#endif	JERQ
#ifdef	MPX
					P->state |= RESHAPED;  /* force a restart */
#else
					reshaped = TRUE;
#endif
					return(TRUE);
				}
				else
					wait(CPU);
			break;
		case M_PAUSE:
			cursswitch(&sleepz);
			while(!button3())
				wait(CPU);
			while(button3())
				wait(CPU);
			break;

		case M_STATS:	/* change movement stats */
			graph.origin =
				add(
					board.origin,
					div(
						sub(
							sub(
								board.corner,
								board.origin
							),
							Pt(80, 120)
						),
						2
					)
				);
			graph.corner = add(graph.origin, Pt(80, 120));
			cursswitch(old);
#ifdef	JERQ
#ifndef	MPX
			cursswitch(char_space);
#endif	MPX
#endif	JERQ
			bitblt(&display, raddp(graphs.rect, graph.origin),
				&graphs, Pt(0,0), F_STORE);	/* save display */
			graphc = raddp(Rect(0, 16, 20, 120), graph.origin);
			graphr = raddp(graphc, Pt(30, 0));
			graphb = raddp(graphr, Pt(30, 0));
			rectf(&display, graph, F_CLR);
			bitblt(bchar('C'), Rect(0,0,16,16), &display,
				graph.origin, F_STORE);
			bitblt(bchar('R'), Rect(0,0,16,16), &display,
				add(graph.origin, Pt(30, 0)), F_STORE);
			bitblt(bchar('B'), Rect(0,0,16,16), &display,
				add(graph.origin, Pt(60, 0)), F_STORE);
			rectf(&display, graphc, F_STORE);
			rectf(&display, inset(graphc, 1), F_CLR);
			rectf(&display, graphr, F_STORE);
			rectf(&display, inset(graphr, 1), F_CLR);
			rectf(&display, graphb, F_STORE);
			rectf(&display, inset(graphb, 1), F_CLR);
			gdisp(inset(graphc, 2), sb_change);
			gdisp(inset(graphr, 2), sb_random);
			gdisp(inset(graphb, 2), sb_break);
			cursswitch((Word *)0);
			sleep(2);	/* sync */
			while(!button3())
			{
				/* here we adjust the graphs */
				if (ptinrect(mouse.xy, inset(graphc,2))&& button1())
					mgdisp(graphc, &sb_change);
				if (ptinrect(mouse.xy, inset(graphr,2))&& button1())
					mgdisp(graphr, &sb_random);
				if (ptinrect(mouse.xy, inset(graphb,2))&& button1())
					mgdisp(graphb, &sb_break);
			}
			while(button3())
				wait(CPU);
			cursswitch(old);
#ifdef	JERQ
#ifndef	MPX
			cursswitch(char_space);
#endif	MPX
#endif	JERQ
			bitblt(&graphs,graphs.rect,&display,graph.origin, F_STORE);
			break;

		}
		cursswitch(old);
#ifdef	JERQ
#ifndef	MPX
		cursswitch(char_space);
#endif	MPX
#endif	JERQ
	}
	return(FALSE);
}

gdisp(box, value)
Rectangle	box;
int		value;
{
	texture(&display,
		Rect(
			box.origin.x,
			box.corner.y - value,
			box.corner.x,
			box.corner.y
		),
		&grey,
		F_STORE);
	rectf(&display,
		Rect(
			box.origin.x,
			box.origin.y,
			box.corner.x,
			box.corner.y - value
		),
		F_CLR);
}

mgdisp(box, vp)
Rectangle	box;
int		*vp;
{
	Point	tmp;

	cursswitch(old);
#ifdef	JERQ
#ifndef	MPX
	cursswitch(char_space);
#endif	MPX
#endif	JERQ
	while (button1() && ptinrect((tmp = mouse.xy), inset(box, 2)))
	{
		cursswitch((Word *)NULL);
		*vp = box.corner.y - tmp.y - 2;
		sleep(2);			/* sync with display */
		cursswitch(old);
#ifdef	JERQ
#ifndef	MPX
		cursswitch(char_space);
#endif	MPX
#endif	JERQ
		rectf(&display, box, F_STORE);
		gdisp(inset(box, 2), *vp);	/* put it on the screen */
	}
	rectf(&display, box, F_STORE);
	rectf(&display, inset(box, 1), F_CLR);
	gdisp(inset(box, 2), *vp);
	cursswitch((Word *)NULL);
}
