#include "cip.h"

#ifdef DMD630
#include <menu.h>

#else
#include "tmenuhit.h"

#ifdef MPX
#define stipple
#include <pandora.h>
#endif
#endif


extern int		currentBrush;
extern int		copyFlag;
extern int		thingSelected;
extern int		editDepth;
extern int		gridState;
extern int		videoState;
extern int		buttonState;
extern short		fontBells;
extern short		noSpace;

extern struct macro *	macroList;

extern Rectangle	brushes[];

extern Texture16	textCursor;

extern struct thing *	Transform ();
extern Point 		jString();
extern void		getKbdChar();
extern void		jMoveTo();


/**** TITEM STRUCTURES ****/

/* Basic Titem structure with only text field. */
typedef struct BasicItem {
	char *		text;	/* string for menu */
} BasicItem;

/* Checkmark Titem structure with text and icon fields. */
typedef struct ChkmarkItem {
	char *		text;	/* string for menu */
	Bitmap *	icon;	/* pointer to the icons bitmap */
} ChkmarkItem;

/* Top menu Titem structure with text and next fields. */
typedef struct TopMenuItem {
	char *		text;	/* string for menu */
	Tmenu *		next;	/* pointer to sub-menu */
} TopMenuItem;


/**** MENU RELATED FUNCTIONS ****/

/* Set a checkmark in specified menu at specified index and clearing
   checkmarks off all other items in menu. */
	void
setChkmark( tmenu, index )
	Tmenu *		tmenu;
	short		index;
{
	register ChkmarkItem *	ci;
	register int		i;

	ci = (ChkmarkItem *) tmenu->item;

	for( i=0;  ci->text != NULL;  ci++, i++ ) {
		ci->icon = (i==index) ? &B_checkmark : 0 ;
	}
}


/* Returns index in the menu of the selected item (starting at 0). */
	int
indexHit( tmenu) 
	Tmenu *		tmenu;
{
	return tmenu->prevhit + tmenu->prevtop ;
}



/**** Point size menu ****/

int  pointSizes[] = { 2,3,4,5,6,7,8,9,10,11,12,14,16,18,20,22,24 };

ChkmarkItem psText[] = {
	"2\240",	0,
	"3\240",	0,
	"4\240",	0,
	"5\240",	0,
	"6\240",	0,
	"7\240",	0,
	"8\240",	0,
	"9\240",	0,
	"10\240",	0,
	"11\240",	0,
	"12\240",	0,
	"14\240",	0,
	"16\240",	0,
	"18\240",	0,
	"20\240",	0,
	"22\240",	0,
	"24\240",	0,
	0
};

Tmenu psMenu = { (Titem *) psText, 0, 0, 0, TM_TEXT | TM_ICON };

/* Convert from point size to index into menu. */
	int
psIndex( ps )
	int	ps;
{
	register int	i;

	for( i=0; pointSizes[i] < ps; i++ )
		;
	return i;
}

#define indexPS(i)	(pointSizes[i])



/**** Font menu  (Order must agree with constants found in cip.h) ****/

ChkmarkItem fontText[] = {
	"Roman\240",		0,
	"  Italic\240",		0,
	"  Bold\240",		0,
	"Helvetica\240",	0,
	"  Italic\240",		0,
	"  Bold\240",		0,
	"Palatino\240",		0,
	"  Italic\240",		0,
	"  Bold\240",		0,
	"Expanded\240",		0,
	"  Italic\240",		0,
	"  Bold\240",		0,
	"Constant width ",	0,
	0
};

Tmenu fontMenu = { (Titem *) fontText, 0, 0, 0, TM_TEXT | TM_ICON };

/* Convert from font num to index into menu. */
#define fontIndex(num)	((num)-1)

#define indexFont(i)	((i)+1)


/**** Justification menu (Order must agree with constants in cip.h) ****/

ChkmarkItem justText[] = {
	"Left\240",	0,
	"Center\240",	0,
	"Right\240",	0,
	0
};

Tmenu justMenu = { (Titem *) justText, 0, 0, 0, TM_TEXT | TM_ICON };

#define justIndex(just)		(just)

#define indexJust(i)		(i)


/**** Space menu ****/

ChkmarkItem spaceText[] = {
	"-4\240",	0,
	"-2\240",	0,
	"Normal\240",	0,
	"+2\240",	0,
	"+4\240",	0,
	"+6\240",	0,
	"+8\240",	0,
	"+10\240",	0,
	"+12\240",	0,
	"+14\240",	0,
	"+16\240",	0,
	"+18\240",	0,
	"+20\240",	0,
	"+22\240",	0,
	"+24\240",	0,
	"+26\240",	0,
	0
};

Tmenu spacingMenu = { (Titem *)spaceText, 0, 0, 0, TM_TEXT | TM_ICON };

#define spaceMin	-4
#define spaceMax	26

#define spacingIndex(sp)	(((sp)+4)>>1)

#define indexSpacing(i)		(((i)<<1)-4)


/**** Density menu ****/

ChkmarkItem densityText[] = {
	"Solid\240",	0,
	"Dashed\240",	0,
	"Dotted\240",	0,
	0
};

Tmenu densityMenu = { (Titem *) densityText, 0, 0, 0, TM_TEXT | TM_ICON };

#define densityIndex(den)	(den)

#define indexDensity(i)		(i)


/**** Arrow menu ****/

ChkmarkItem arrowText[] = {
	"No arrows\240",	0,
	"Start end\240",	0,
	"Finish end\240",	0,
	"Both ends\240",	0,
	0
};
Tmenu arrowMenu = { (Titem *) arrowText, 0, 0, 0, TM_TEXT | TM_ICON };

#define arrowIndex(a)		(a)

#define indexArrow(i)		(i)


/**** Grid menu ****/

ChkmarkItem gridText[] = {
	"Off\240",	0,
	"On\240",	0,
	0
};
Tmenu gridMenu = { (Titem *) gridText, 0, 0, 0, TM_TEXT | TM_ICON };

#define gridIndex(a)		(a)

#define indexGrid(i)		(i)


/**** Alignment menu ****/

int alignLevels[] = { 2,8,16 };

ChkmarkItem alignText[] = {
	"fine   (2)\240", 	0,
	"medium (8)\240", 	0,
	"coarse (16)\240", 	0,
	0
};
Tmenu alignMenu = { (Titem *)alignText, 0, 0, 0, TM_TEXT | TM_ICON };


/* Convert from point size to index into menu. */
	int
alignIndex( a )
	int	a;
{
	register int	i;

	for( i=0; alignLevels[i] < a; i++ )
		;
	return i;
}

#define indexAlign(i)	(alignLevels[i])


/**** Line menu ****/

TopMenuItem lineText[] = {
	"Density\240",		&densityMenu,
	"Arrow\240",		0,	
        "Reflect x\240",	0,
	"Reflect y\240",	0,
	"Copy\240",		0,
	"Delete\240",		0,
	0
};
Tmenu lineMenu = { (Titem *)lineText, 0, 0, 0, TM_TEXT|TM_NEXT };


/**** Global line menu ****/

TopMenuItem gLineText[] = {
	"Density\240",		&densityMenu,
	"Arrows\240",		&arrowMenu,
	0
};
Tmenu gLineMenu = { (Titem *)gLineText, 0, 0, 0, TM_TEXT|TM_NEXT };


/**** Box menu ****/

TopMenuItem boxText[] = {
	"Density\240",		&densityMenu,
	"Copy\240",		0,	
	"Delete\240",		0,
	0
};
Tmenu boxMenu = { (Titem *)boxText, 0, 0, 0, TM_TEXT|TM_NEXT };


/**** Circle menu ****/

BasicItem circleText[] = {
	"Copy\240",
	"Delete\240",
	0
};
Tmenu circleMenu = { (Titem *)circleText, 0, 0, 0, TM_TEXT };


/**** Spline menu ****/

BasicItem splineText[] = {
	"Arrow\240",
        "Reflect x\240",
	"Reflect y\240",
	"Copy\240",
	"Delete\240",
	0
};
Tmenu splineMenu = { (Titem *)splineText, 0, 0, 0, TM_TEXT };


/**** Arc menu ****/

BasicItem arcText[] = {
        "Reflect x\240",
	"Reflect y\240",
	"Copy\240",
	"Delete\240",
	0
};
Tmenu arcMenu = { (Titem *)arcText, 0, 0, 0, TM_TEXT };


/**** Macro menu ****/

BasicItem macroText[] = {
	"Edit\240",
	"Separate\240",
	"Reflect x\240", 
	"Reflect y\240", 
	"Copy\240", 
	"Delete\240", 
	0
};
Tmenu macroMenu = { (Titem *)macroText, 0, 0, 0, TM_TEXT };


/**** Text object menu ****/

TopMenuItem textText[] = {
	"Point size\240",	&psMenu,
	"Font style\240",	&fontMenu,
	"Justify\240",		&justMenu,
	"Spacing\240",		&spacingMenu,
	"Separate\240",		0,
	"Copy\240",		0,
	"Delete\240",		0,
	0
};

Tmenu textMenu = { (Titem *) textText, 0, 0, 0, TM_TEXT|TM_NEXT };


/**** Global TEXT menu ****/

TopMenuItem gTextData[] = {
	"Point size\240",	&psMenu,
	"Font style\240",	&fontMenu,
	"Justify\240",		&justMenu,
	"Spacing\240",		&spacingMenu,
	0
};

Tmenu gTextMenu = { (Titem *) gTextData, 0, 0, 0, TM_TEXT|TM_NEXT };


/**** Command menu ****/

#define GET		0
#define PUT		1
#define CLEAR		2
#define REDRAW		3
#define DEFINEMACRO	4
#define GRIDTOGGLE	5
#define ALIGNMENT	6
#define CIPVERSION	7
#define ORIGWINDOW	8
#define QUIT		9

TopMenuItem commandText[] = {
	"Get file\240",		0,		
	"Put file\240",		0,	
	"Clear screen\240", 	0,
	"Redraw screen\240",	0,
	"Define macro\240",	0,
	"Grid\240", 		&gridMenu,
	"Alignment\240", 	&alignMenu,
	"Version\240",		0,
	"Close window\240",	0,
	"Quit\240",		0,
	0
};
Tmenu commandMenu = { (Titem *)commandText, 0, 0, 0, TM_TEXT|TM_NEXT };





	struct thing *
reflect(t, p, axis)
	register struct thing *	t;
	Point		p;	/* Offset */
	char		axis;	/* Either 'x' or 'y' */
{
	struct thing *	h;

	drawSelectionLines (t, p);
	draw (t, p);
	h = (axis == 'x') ?
	    Transform (t, Pt(0, t->bb.corner.y+t->bb.origin.y))
	  : Transform (t, Pt(t->bb.corner.x+t->bb.origin.x, 0));
	draw (t, p);
	drawSelectionLines (t, p);
	return (h);
}


	void
copyOperation()
{
	if( !noSpace ) {
		copyFlag = 1;
		changeButtons( COPYbuttons );
	}
}


	struct thing *
deleteOperation(t,p)
	struct thing *	t;
	Point		p;	/* Offset */
{
	drawSelectionLines(t,p);
	draw(t, p);
	t = deleteThing(t);
	thingSelected = 0;
	copyFlag = 0;
	changeButtons( INITbuttons );
	return( t );
}


/* This routine deletes the given macro from the macro list. */
/* It has to scan the macro list looking for that macros entry since */
/* there are no back links within the list. */

	void
removeMacro (m)
	struct macro *m;
{
  register struct macro *ml;	/* Pointer for macro list */

  if (m == macroList) {
    macroList = m->next;
  }
  else {
    for (ml = macroList; ml != MNULL; ml = ml->next) {
      if (ml->next == m) {	/* Macro found delete it. */
	ml->next = m->next;
      }
    }
  }
  if (m->name != (char *)NULL) {
    free (m->name);
  }
  free (m);
  return;
}


	struct thing *
separateLineText(t,p)
	struct thing *	t;
	Point		p;
{
	register int	lp;
	register int	i;
	register int	q;
	char *		ls;
	char *		s;
	char *		ss;
	struct thing *	nt;
	struct thing *	h;
	Font *		f;
	int		ncr = 0; /* number of carriage returns */
	int		start = 0;
	
	draw(t,p); /* erase present text */
	h = t;
	f = t->otherValues.text.f->f;
	ls = t->otherValues.text.s;
	
	for(i=0;ls[i]!='\0';i++) {
		if(ls[i] == '\r')
			ncr++;
	}
	
	lp = -( ((f->height+t->otherValues.text.spacing)*ncr) >> 1 );
	
	for(i = 0; i <= ncr; i++) {
		for(q = start; (ls[q] != '\r') && (ls[q] != '\0'); q++)
			;

		if ( (ss=getSpace(q-start+1)) != NULL ) {
			s = ss;
			for(q = start; (ls[q] != '\r') && (ls[q] != '\0'); q++)
				*s++ = ls[q];
			*s='\0';
		}

		nt = newText( Pt(t->origin.x, (t->origin.y + lp) ), ss);
		nt->otherValues.text.f = 
		    findFont(t->otherValues.text.f->ps,
			     t->otherValues.text.f->num);
		nt->otherValues.text.just = t->otherValues.text.just;
		BoundingBox(nt);
		draw(nt,p);
		h = insert(nt,h);
		start = q + 1;
		lp = lp + f->height + t->otherValues.text.spacing;
	}

	thingSelected = 0;
	changeButtons( INITbuttons );

	return( deleteThing(t) );
}


	struct thing *
displayThingMenu(m,t,p) 
	Point			m;	/* Mouse location */
	Point			p;	/* Offset */
	register struct thing *	t;	/* Thing to be displayed */
{
	register short		item;
	register int		b;
	int			oldED; 
	register struct thing *	pts;
	register struct thing *	h;
	register struct thing *	g;
	register struct thing *	f;
	Rectangle		r; 
	Titem *			ti;

	switch(t->type) {

	case LINE: 
		setChkmark( &densityMenu, densityIndex(t->boorder) );

		ti = tmenuhit( &lineMenu, 3, TM_EXPAND );

		if( ti == (Titem * )NULL )
			break;

		switch( indexHit(&lineMenu) ) {

		case 0: /* Density */
			draw(t,p);
			t->boorder = indexDensity( indexHit(&densityMenu) );
			draw(t,p);
				break;
	
		case 1: /* Arrow - add or remove arrowheads */
			draw(t,p);
			if (distance(m,t->origin) < 
			    distance(m,t->otherValues.end)) {
				if ((t->arrow==startARROW)||
				    (t->arrow==doubleARROW)) {
					t->arrow -= startARROW;
				}
				else {
					t->arrow += startARROW;
				}
			}
			else {
				if ((t->arrow==endARROW)||
				    (t->arrow==doubleARROW)) {
					t->arrow -= endARROW;
				}
				else {
					t->arrow += endARROW;
				}
			}
			draw(t,p);
			break;
	
		case 2: /* Reflect x */
			t = reflect(t, p, 'x');
			break;
	
		case 3: /* Reflect y */
			t = reflect(t, p, 'y');
			break;
	
		case 4: /* Copy */
			copyOperation();
			break;
	
		case 5: /* Delete */
			t = deleteOperation(t,p);
		}
		break;

	case BOX:
		setChkmark( &densityMenu, densityIndex(t->boorder) );

		ti = tmenuhit( &boxMenu, 3, TM_EXPAND );

		if( ti == (Titem * )NULL )
			break;

		switch( indexHit(&boxMenu) ) {

		case 0: /* Density */
			draw(t,p);
			t->boorder = indexDensity( indexHit(&densityMenu) );
			draw(t,p);
			break;
	
		case 1: /* Copy */
			copyOperation();
			break;
	
		case 2: /* Delete */
			t = deleteOperation(t,p);
		}
		break;

	case MACRO:
		ti = tmenuhit( &macroMenu, 3, 0 );

		if( ti == (Titem * )NULL )
			break;

		switch( item = indexHit(&macroMenu) ) {

		case 0: /* edit macro */
			oldED = editDepth;
			pts = t->otherValues.list->parts;

			/* Draw edit button when editDepth==1, other times */
			/* undraw button. */
			if ((editDepth)!=0) {
				drawEDbutton(editDepth);
			}
			drawEDbutton(++editDepth);

			changeButtons (INITbuttons);
			thingSelected = 0;
			p = sub( p, drawOffset );

			while (editDepth>oldED) {
				pts = doMouseButtons(pts, add(p,t->origin) );
			}

			p = add( p, drawOffset );
			if (pts != TNULL) {
				t->otherValues.list->parts = pts;
				r = macroBB(pts);	/* get outline */

				/* Origin is offset by new r.origin */
				t->origin = add( t->origin, r.origin );

				/* Offset all parts by new r.origin */
				g = pts;
				do {
					makeRelative(g,r.origin);	
					g = g->next;
				} while (g != pts);

				/* Set list->bb such that its origin is 0 */
				t->otherValues.list->bb = rsubp( r, r.origin);

				BoundingBox( t );

			} else {
				t = deleteThing (t); /* Delete empty macros */
			}
			if (editDepth==0) {
				doRedraw(t, p);
			}
			break;

		case 1: /* separate */
			h = remove(t);	/* Cut macro from thing list */
			t->otherValues.list->useCount--;
			m = sub(Pt(0,0),t->origin);

			/* Go thru macro's thing list and make 
			   everything relative */
			if ((g=t->otherValues.list->parts)!=TNULL) {
				if (t->otherValues.list->useCount > 0) {

					do {	/* Copy list */
						g = g->next;

						/* Only arcs are not copied 
						   at their origins. */

						f = copyThing (g, 
						      (g->type == ARC) ?
				 		       g->otherValues.arc.start
						      : g->origin, 
						      0, 0);
						/* 0: object wont be drawn */ 

						f = makeRelative (f, m);
						h = insert (f, h);
					} while (g != t->otherValues.list->parts);
				} else {
					do{	/* Move list */
						g = g->next;
						g = makeRelative(g,m);
						f = g;
						g = remove (f);
						h = insert (f, h);
					} while (g != TNULL);

					removeMacro (t->otherValues.list);
				}
			}
	
			thingSelected = 0;
			copyFlag = 0;
			changeButtons(INITbuttons);
			free(t);	/* Remove macro thing */
			t = h;
			break;

		case 2: /* Reflect x */
		case 3: /* Reflect y */
			draw (t, p);
			if ((g = t->otherValues.list->parts) != TNULL) {
				r = t->otherValues.list->bb;
				do {
					h = (item == 0) ?
				  	  Transform(g,Pt(0,r.origin.y+r.corner.y))
					: Transform(g,Pt(r.origin.x+r.corner.x,0));
					g = g->next;
				} while (g != t->otherValues.list->parts);
			}

			if (editDepth) { /* Only draw if not at level 0 */
				draw (t, p);
			} else {	 /* otherwise, redraw entire screen. */
				doRedraw (t, p);
			}
			break;
			
	
		case 4: /* Copy */
			copyOperation();
			break;
	
		case 5: /* Delete */
			t = deleteOperation(t,p);
		}
		break;

	case TEXT:
		setChkmark( &psMenu,	psIndex(t->otherValues.text.f->ps) );
		setChkmark( &fontMenu,	fontIndex(t->otherValues.text.f->num) );
		setChkmark( &justMenu,	justIndex(t->otherValues.text.just) );
		setChkmark( &spacingMenu, spacingIndex(t->otherValues.text.spacing) );

		ti = tmenuhit( &textMenu, 3, TM_EXPAND );

		if( ti == (Titem *) NULL )
			break;

		switch( indexHit( &textMenu ) ) {

		case 0: /* point size */
			draw(t,p); /* erase thing */
			t->otherValues.text.f->useCount--;
			t->otherValues.text.f = 
			findFont(indexPS(indexHit(&psMenu)),
				t->otherValues.text.f->num);
			BoundingBox(t);
			draw(t,p);
			break;

		case 1: /* font style */
			draw(t,p);
			t->otherValues.text.f->useCount--;
			t->otherValues.text.f = 
			findFont(t->otherValues.text.f->ps,
				indexFont(indexHit(&fontMenu)));
			BoundingBox(t);
			draw(t,p);
			break;

		case 2: /* justification */
			draw(t,p);
			t->otherValues.text.just = 
				indexJust(indexHit( &justMenu ));
			BoundingBox(t);
			draw(t,p);
			break;

		case 3: /* spacing */
			draw(t,p);
			t->otherValues.text.spacing = 
				indexSpacing(indexHit(&spacingMenu));
			BoundingBox(t);
			draw(t,p);
			break;

		case 4: /* separate lines of text */
			t = separateLineText(t,p);
			break;
	
		case 5: /* Copy */
			copyOperation();
			break;
	
		case 6: /* Delete */
			t = deleteOperation(t,p);
		}
		break;

	case CIRCLE:
	case ELLIPSE: 
		ti = tmenuhit( &circleMenu, 3, 0 );

		if( ti == (Titem * )NULL )
			break;

		switch( indexHit(&circleMenu) ) {
	
		case 0: /* Copy */
			copyOperation();
			break;
	
		case 1: /* Delete */
			t = deleteOperation(t,p);
		}
		break;

	case ARC:
		ti = tmenuhit( &arcMenu, 3, TM_EXPAND );

		if( ti == (Titem * )NULL )
			break;

		switch( indexHit(&arcMenu) ) {

		case 0: /* Reflect x */
			t = reflect(t, p, 'x');
			break;

		case 1: /* Reflect y */
			t = reflect(t, p, 'y');
			break;
	
		case 2: /* Copy */
			copyOperation();
			break;
	
		case 3: /* Delete */
			t = deleteOperation(t,p);
		}
		break;

	case SPLINE: 
		ti = tmenuhit( &splineMenu, 3, 0 );

		if( ti == (Titem * )NULL )
			break;

		switch( indexHit(&splineMenu) ) {

		case 0: /* arrow */
			b = t->otherValues.spline.used;
			if (distance(m,t->origin)<
			    distance(m,t->otherValues.spline.plist[b])) {
				if ((t->arrow==startARROW)||
				    (t->arrow==doubleARROW)) {
					t->arrow -= startARROW;
				} else {
					t->arrow += startARROW;
				}
				arrow(add(p,t->otherValues.spline.plist[2]),
				      add(p,t->otherValues.spline.plist[1]));
			} else {
				if ((t->arrow==endARROW)||
				    (t->arrow==doubleARROW)) {
					t->arrow -= endARROW;
				} else {
					t->arrow += endARROW;
				}
				arrow(add(p,t->otherValues.spline.plist[b-2]),
				add(p,t->otherValues.spline.plist[b-1]));
			}
			break;
	
		case 1: /* Reflect x */
			t = reflect(t, p, 'x');
			break;
	
		case 2: /* Reflect y */
			t = reflect(t, p, 'y');
			break;
	
		case 3: /* Copy */
			copyOperation();
			break;
	
		case 4: /* Delete */
			t = deleteOperation(t,p);
		}
		break;

	}	/* end of switch on t->type */

	return(t);
}


static Texture16 rusure = {
  0X0000, 0X0EA0, 0X0AA0, 0X08A0,
  0X08A0, 0X08E0, 0X0000, 0X0000,
  0XEAEE, 0X8AAA, 0XEA8E, 0X2A88,
  0XEE8E, 0X0000, 0X0000, 0X0000,
};


RUsure ()
{
	int savebuttonstate;
	int ret;

	ret = 0;
	cursswitch (&rusure);
	savebuttonstate = buttonState;
	if (CORRECTSIZE) {
		changeButtons( QUITbuttons );
	}
	while (!button123())
		nap(2);
	if (button3())
		ret = 1;
	changeButtons( savebuttonstate );
	while (button123())
		nap(2);
	cursSwitch();
	return (ret);
}


	struct thing *
displayCommandMenu(h, offset) 
	register struct thing *	h;
	Point 			offset;
{
	register Titem *	ti;
	int			newAlign;

	switch( currentBrush ) {

	case TEXT:
		setChkmark( &psMenu,	  psIndex(currPS) );
		setChkmark( &fontMenu,	  fontIndex(currFont) );
		setChkmark( &justMenu,	  justIndex(currJust) );
		setChkmark( &spacingMenu, spacingIndex(currSpacing) );

		ti = tmenuhit( &gTextMenu, 3, TM_EXPAND );

		if( ti == (Titem *) NULL )
			break;

		switch( indexHit( &gTextMenu ) ) {
		case 0:	/* Point size */
			currPS = indexPS( indexHit( &psMenu ) );
			break;
		case 1:	/* Font style */
			currFont = indexFont( indexHit( &fontMenu ) );
			break;
		case 2:	/* Justify */
			currJust = indexJust( indexHit( &justMenu ) );
			break;
		case 3:	/* Spacing */
			currSpacing = indexSpacing( indexHit( &spacingMenu ) );
			break;
		}
		break;

	case BOX:
		setChkmark( &densityMenu,  densityIndex(currBoxType) );

		ti = tmenuhit( &densityMenu, 3, 0 );

		if( ti == (Titem *) NULL )
			break;

		currBoxType = indexDensity( indexHit( &densityMenu ) );

		break;

	case SPLINE:
		setChkmark( &arrowMenu,  arrowIndex(currSplineArrows) );

		ti = tmenuhit( &arrowMenu, 3, 0 );

		if( ti == (Titem *) NULL )
			break;

		currSplineArrows = indexArrow( indexHit( &arrowMenu ) );

		break;

	case LINE:
		setChkmark( &densityMenu,  densityIndex(currLineType) );
		setChkmark( &arrowMenu,  arrowIndex(currLineArrows) );

		ti = tmenuhit( &gLineMenu, 3, TM_EXPAND );

		if( ti == (Titem *) NULL )
			break;

		switch( indexHit( &gLineMenu ) ) {

		case 0:	/* Density */
			currLineType = indexDensity( indexHit( &densityMenu ) );
			break;

		case 1:	/* Arrows */
			currLineArrows = indexArrow( indexHit( &arrowMenu ) );
			break;
		}
		break;

	case CIRCLE:
	case ELLIPSE:
	case ARC:
	case MACRO:
		/* No menu when these brushes are selected */
		for( ; button3(); nap(2) );
		break;

	default:	/* Use main menu when no brush is selected */
		setChkmark( &alignMenu, alignIndex(currAlignment) );
		setChkmark( &gridMenu,  gridIndex(gridState) );

		ti = tmenuhit( &commandMenu, 3, TM_EXPAND );

		if( ti == (Titem *) NULL )
			break;

		switch( indexHit( &commandMenu ) ) {

		case GET: 
			fontBells = 0;
			h = doGet(h);
			ringbell();
			fontBells = 1;
			break;

		case PUT: 
			doPut(h);
			ringbell();
			break;

		case CLEAR: 
			if (RUsure()) {
				h = doClear(h);
			}
			break;

		case DEFINEMACRO: 
			if (!noSpace) {
				h = defineMacro(h);
			}
			break;

		case REDRAW:
			doRedraw(h, offset);
			break;

		case QUIT: 
			if (RUsure()) {
				exit (1);
			}
			break;

		case GRIDTOGGLE:
			message("Grid displayed is based on the alignment setting.");
			putMessage("No grid is displayed when\ralignment is 2.");

			if( gridState == GRIDon ) {
				gridState = GRIDoff;
			} else {
				gridState = GRIDon;
			}
			drawGrid();
			break;

		case ALIGNMENT: 
			newAlign = indexAlign( indexHit( &alignMenu ) );
			if( newAlign != currAlignment ) {
				if( gridState == GRIDon ) {
					drawGrid();  /* erases present grid */
					currAlignment = newAlign;
					drawGrid();
				} else {
					currAlignment = newAlign;
				}
			}
			break;

		case CIPVERSION:
			ringbell();
			putVersion();
			break;

		case ORIGWINDOW:
			originalLayer();
			break;
		}
		break;

	}
	return(h);
}
      

doRedraw(h, offset) 
	struct thing *h; 
	Point offset;
{
	register struct thing *t;

	cursinhibit();
	stipple(brushes[PIC]);
	cursallow();
	if (gridState==GRIDon) {
		drawGrid();
	}
	if ((t = h) != TNULL) {
		do {
			if (t->type==MACRO) 
				BoundingBox(t);
			draw(t,offset); 
			t = t->next;
#ifdef MPX
			sw(1);  /* give up CPU */
#endif
		} while (t != h);
	}
}


#ifndef DMD630
#ifdef MPX
	void
setdata (p)	/* Setdata routine copied from layersys/boot.c */
	register struct Proc *p;
{
	register struct udata *u = ((struct udata *)p->data);

	u->Drect = p->rect;
	u->Jdisplayp = p->layer;
}
#endif
#endif
