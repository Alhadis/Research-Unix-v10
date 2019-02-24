/*		TMENUHIT 		*/

#ifndef DMD630

/* Note: built into the DMD 630 ROMS. */

#include <dmd.h>
#include <font.h>
#include "tmenuhit.h"


static Word checkmark[] = {		/* check mark */
	 0x00000000, 0x00030000, 0x00070000, 0x000E0000,
	 0x001C0000, 0x00380000, 0x00700000, 0x00E00000,
	 0x61E00000, 0x73C00000, 0x3F800000, 0x3F000000,
	 0x1E000000, 0x1E000000, 0x0C000000, 0x00000000,
};
Bitmap B_checkmark = { (Word *) checkmark, 1, 0, 0, 16, 16, 0 };


static Word rtarrow[] =	{		/* right arrow */
 	0x0000, 0x0000, 0x0080, 0x00C0,
	0x00C0, 0x00E0, 0x00F8, 0xFFFE,
	0xFFFE, 0x00F8, 0x00E0, 0x00C0,
	0x00C0, 0x0080, 0x0000, 0x0000,
};
Bitmap B_rtarrow = { (Word *) rtarrow, 1, 16, 0, 32, 16, 0 };


int cursxyonly;


/*
 *      The following set of definitions take advantage of built-in
 *      routines available because of the definitions in the <pandora.h>
 *      file and the standalone vector tables.
 */

#define realmouse
#define Jcursinhibit
#define Jcursallow

#include <pandora.h>

#ifdef MPX
#define mouse           realmouse
#endif


/*
 *	Macro definitions for commonly used functions.
 *
 *	scale()	- converts value from scalar to another another.
 *
 *	bound() - insures that a value is between two bounds and
 *		  will assign the boundary value should the input
 *		  value exceeds the bounds.
 *	xyon()	- sets the cursxyonly variable
 *	xyoff() - clears the cursxyonly variable
 */
#define scale(x, inmin, inmax, outmin, outmax)\
	(outmin + muldiv(x-inmin,outmax-outmin,inmax-inmin))

#define bound(x, low, high) min(high, max( low, x ))
#define xyon()	cursxyonly=1
#define xyoff()	cursxyonly=0

/*
 *	Constant definitions.
 *
 *	BORDER		- the thickness in pixels for the border
 *			  surrounding a menu.
 *	DISPLAY		- maximum number of menu items displayed per menu.
 *	DELTA		- used in scroll bar for hysterisis checks
 *	BARWIDTH	- width of the rectangle for the scroll bar
 *	SPACEPAD	- spacing in pixels for use between text and icons
 *	MAXDEPTH	- maximum levels depth for nesting menus
 *	MAXCHARS	- maximum number of characters allowed per text line.
 *	SELECT,NOSELECT	- = tmselect, did a menu selection occur.
 */
#define BORDER		2
#define DISPLAY		16
#define DELTA		6
#define BARWIDTH	18
#define SPACEPAD	5
#define	MAXDEPTH	8	/* don't use too much stack */
#define MAXCHARS	128
#define SELECT		1
#define NOSELECT	2

/*
 *	Tleaf	- structure definition unique for each recursion level of
 *		  menus.  It contains characteristics that are global to
 *		  a given menu.
 *	org	- the origin point for the menu.
 *	width	- the width in pixels of the menu.
 *	items	- the number of items in the menu.
 *	txtwdith- number of pixels of the widest text string.
 *	tbranch	- contains the coordinates for a rectangle that is
 *		  associated with a sub-menu.  Except for when treegrow is
 *		  set, a submenu can only be displayed while the mouse 
 *		  cursor is in that menu or its tbranch.
 *	spacing	- the number of pixels allocated to the vertical spacing
 *		  of items.  It is 4 pixels plus the larger of the icons
 *		  or the font height.
 *	iconwidth -
 *		  the number of pixels allocated horizontally for all
 *		  icons.  If zero, then no icons are used.
 *	md	- menu data for the tleaf
 *	newhit	- last hit in the menu
 */
typedef struct Tleaf {
	Point org;
	int width;
	int items;
	int txtwidth;
	Rectangle tbranch;
	char spacing;
	char iconwidth;
	struct menudata *md;
	int newhit;
} Tleaf;

/*
 *	Globals
 *
 *	tnoret	    - the address of this variable is used whenever a non-
 *		      selection is made.
 *	tfill	    - character array used for generating the text string
 *		      used for menu items
 *	treegrow    - a flag that when set indicates that sub-menus should
 *		      be generated to the lowest leaf based on the previous
 *		      values.  This is used when menus are generated
 *		      initially on a button depression.
 *	tleaf[]	    - array of type structure Tleaf that contains information
 *		      regarding the size and screen location of menus.
 *	tuser_mi    - contains a pointer to the menu item in the user's
 *		      defined menu structure.
 *	select_menu - the menu that a selection was made from.
 *	tmselect    - true is selection is made. If a menu item was selected,
 *		      it equals SELECT, otherwise it equals NOSELECT    
 *	setorg      - set origin of root menu
 *	redraw      - redraw menu after selection
 *	wasredraw   - returning from a "redraw" via tm_ret(), don't
 *		      update prevhit/prevtop because it was already done
 *		      and it would break setup's use of tmenuhit
 *	tstatic	    - flag that indicates a static menu.
 */
char tfill[MAXCHARS];
int treegrow;
Tleaf tleaf[MAXDEPTH];
Titem *tuser_mi;
Tmenu *select_menu;
int tmselect;
int setorg;
int redraw;
int wasredraw;
int tstatic;

/*
 *	Titem temp - 
 *		a working structure of type Titem used to store the
 *		converted user Titem structure into the internal Titem
 *		structure for processing by the tmhit() code.
 *
 *	tablegen(i, m) -
 *		The default menu generator for the tmenuhit() routine.
 *		The generator returns an address to a menu item.  This
 *		will normally be pointing to the "temp" menu item.
 *		If a user specified menu generator is provided, it is
 *		called here.
 */
Titem temp;

Titem *
tablegen(i, m)
	int i;
	register Tmenu *m;
{
	register unsigned long *fptr;
	register unsigned long *tptr;
	register int count;
	register unsigned int mask;
	register int menumap;

	/*
	 * check the menumap variable.  if not defined, then
	 * set it to all fields, else force the text field to
	 * to always be defined in addition to the user's
	 * definition.
	 */
	menumap = (m->menumap) ? m->menumap | TM_TEXT : 0xffff;

	/*
	 * get the titem variable, whether it comes from a generator
	 * function, or is in an array of titems.
	 */
	if (m->item == 0) {
		fptr = (unsigned long *) m->generator(i, m);
	}
	else {
		for(fptr=(unsigned long *)m->item, mask=0x01; mask != MAX_MASK;
		    mask<<=1)
			if (menumap & mask) fptr += i;
	}
 	tuser_mi = (Titem *) fptr;

	/*
	 * set up the from and to pointers, initialize count, and build
	 * the temp structure from the user Titem structure.
	 */
	tptr = (unsigned long *) &temp;
	for(count=0, mask=0x01; count < MAX_TMFIELDS; count++, menumap>>=1) {
		if (menumap & mask)
			*tptr++ = *fptr++;
		else
			*tptr++ = (unsigned long) 0;
	}

	/*
	 * if the font pointer is not defined, set it to defont.
	 */
	if (temp.font == 0) temp.font = &defont;

	return(&temp);
}

/*
 *	tmenuhit(m, but, flags)
 *		- entry point for the menuhit routine for tree menus.
 *		  It takes four arguments.  "m" is a pointer to a
 *		  menu routine of type structure Tmenu.  "but" is
 *		  the button that is assumed to be depressed when
 *		  this function is called.  When this button is released,
 *		  a selection (or non-selection) is made.  "expand" is
 *		  a flag that application programs can set.  When set,
 *		  tmenuhit() will initially generate the menu tree to
 *		  its lowest leaf based on previous selection values.
 *		  "p" is the position of the origin of the root menu.
 *		  If p.x or p.y are invalid, the menu is centered on
 *		  the mouse.
 *
 *		  tmenuhit() calls scanmenu() to determine the sizes of the
 *		  menu and its sub-menus.  These sizes are then used to
 *		  determine the origin point for each of the menus.  These
 *		  points are stored in the global variable tleaf[].  tmhit()
 *		  is then called with a pointer to the menu structure and that
 *		  function will draw each of the menus.
 */			  
Titem *
tmenuhit(m, but, flags)
register Tmenu *m;
{
	register Tmenu *curr_m, *next_m;
	register Tleaf *t;
	register int depth;
#ifdef MTG630
	register int cursxystate;		/* stores global cursxyonly */
#endif MTG630
	register Point *p;
	void tmhit();
	Tmenu * scanmenu();

	treegrow = flags & TM_EXPAND;
	wasredraw = redraw = flags & TM_NORET;
	tstatic = flags & TM_STATIC;
	if(setorg = (flags & TM_POINT))
		p = (Point *)(&flags + 1);
	curr_m = m;
	t = &tleaf[0];
	depth = 0;

	/*
	 * scan menus to determine the sizes of the menus for this
	 * tree growth.
	 */
	do {
		next_m = scanmenu(curr_m, t++);
		depth++;
	} while ((curr_m = next_m) && treegrow);

	/*
	 * set up origin points for each of the menus such that
	 * the bottom-most leaf menu is centered around the mouse
	 * cursor.  However, make sure that the menus stay within
	 * screen borders and that maximum overlap cannot occur.
	 * If the mouse cursor is such that it cannot remain in the
	 * bottom most leaf menu and still maintain the non-
	 * overlap requirement, then those menus to the right of
	 * the the mouse will not be displayed.
	 *
	 * The user will have to grow them by sliding the mouse
	 * manually to the right.
	 */
	--t; --depth;
	if(!setorg) {
		/*
		 * center the menu on the mouse
		 */
		t->org.y = mouse.xy.y;
		t->org.x = bound(mouse.xy.x
				 	- (SPACEPAD
				   		+ (t->iconwidth ? t->iconwidth+SPACEPAD : 0)
				   		+ t->txtwidth
						+ (t->items > DISPLAY ? BARWIDTH : 0)),
				 0,
				 XMAX-t->width
				);
		for(--t, --depth; t >= &tleaf[0]; --t, --depth) {
			t->org.x = bound((t+1)->org.x - t->width + (BORDER-1),
					0, XMAX);
			t->org.y = (t+1)->org.y;
		}
		t++; depth++;
	}
	else {
		/*
		 * put the root menu's origin at p
		 */
		t = t - depth;
		t->org = *p;
		if((*p).x > XMAX - t->width)
			t->org.x = XMAX - t->width;
		t->org.y += bound(m->prevhit, 0, min(DISPLAY,t->items)-1)*t->spacing + t->spacing/2;
	}
	if (t->org.x == 0 || setorg) {
		/*
		 * We've hit the left side, assume overlap has occurred,
		 * and recalculate menu origins.
		 *
		 * Otherwise, the origin point for the root menu was specified.
		 */
		for(++t, ++depth; depth < MAXDEPTH; t++, depth++) {
			t->org.x = bound((t-1)->org.x + (t-1)->width - (BORDER-1),
					 0, XMAX);
			t->org.y = (t-1)->org.y;
		}
	}

#ifdef MTG630
	cursxystate = cursxyonly;
#endif MTG630
	tmselect = 0;
	tmhit(m, but, 0);
#ifdef MTG630
	cursxyonly = cursxystate;
#endif MTG630
	if(tmselect == SELECT) {
		/*
		 * make sure the Titem returned is correct if it is
		 * filled in by a generator function
		 */
		tablegen(select_menu->prevhit + select_menu->prevtop, select_menu);
		return(tuser_mi);
	}
	else
		return((Titem *)0);
}

/*
 *	menudata	- A structure used in each recursion.  It is a collection
 *			  data that is relevant to tmhit() and subroutines that
 *			  it calls.   This is a single pointer to this structure
 *			  that is passed to routines rather than each individual
 *			  variable.
 *	m		- Member of structure menudata.  It points to the menu
 *			  data structure containing the information supplied
 *			  by the application program for the tmenuhit() function.
 *	top		- indicates the index of the topmost item in the
 *			  menu to be displayed.  This is used only for scrolling
 *			  menus.
 *	lines		- the number of lines to be displayed in the menu.  This
 *			  be equal to DISPLAY or is less if the total number of
 *			  menu items for a given menu is less.
 *	br		- rectangle which defines the scroll bar display.
 *	sr		- defines the rectangle that the scroll bar can travel
 *			  within.
 *	tr		- defines the rectangle for the text portion of the menu.
 *	mr		- defines the entire rectangle for the menu.
 *	b		- pointer to a bitmap which is dynamically assigned
 *			  whenever a menu is generated.  This bitmap stores
 *			  the area that is obscured by the menu.  If no memory
 *			  is available, the pointer is not used and the menu
 *			  is XOR'ed on top of the existing background.
 */
typedef struct menudata {
	Tmenu *m;
	int top, lines;
	Rectangle br, sr, tr, mr;	/* bar, scroll, text, menu */
	Bitmap *b;
} Menudata;

/*
 *	tmhit(m, but, depth)
 *		- This is the heart of the menu routine. "m" contains
 *		  a pointer to a Tmenu structure.  "but" is the button
 *		  number associated with this menu selection.  This
 *		  number is needed so it is known which of the remaining
 *		  two buttons are available for use as help buttons.
 *		  "depth" is passed onto the subroutines called by this
 *		  routine to acquire information previously determined
 *		  for each of the menus and sub-menus involved in the
 *		  initial generation of the menu tree.  The depth is also
 *		  used to compare against the maximum limit of MAXDEPTH
 *		  so as to prevent too much use of the stack space and
 *		  the subsequent problems that can result.
 *
 *		  After the initial generation of the menu tree, this
 *		  routine basically loops, waiting for mouse input to
 *		  determine which menu selection should be highlighted,
 *		  whether to exit a sub-menu, or whether to generate a
 *		  new sub-menu.
 *
 *		  This routine exits when the "but" button is released
 *		  or when the user moves the mouse outside of the right
 *		  arrow rectangle or to the left of the menu.
 *
 */
void
tmhit(m, but, depth)
register Tmenu *m;
{

	/*
	 * Definitions of the following stack variables.
	 *
	 * 	p	- contains the mouse position.  This variable
	 *		  also has a shared function initially to
	 *		  store the origin point for a menu.
	 *	savep	- contains the old mouse position from the last
	 *		  poll.  This is used only with the scroll bar
	 *		  to determine whether cursset() is used to keep
	 *		  the mouse within the scroll bar when it is 
	 *		  being moved.
	 *	rside	- defines the rectangle where are the sub-menu
	 *		  icons (B_rtarrow) are displayed in the menu.
	 *		  When the mouse cursor is in this area, checking
	 *		  for the existence of a sub-menu is made.
	 *	rhit	- The rectangle defining the area of the menu
	 *		  item that is selected.  These coordinates are
	 *		  used for calculating the values of the rectangle
	 *		  "tbranch", and for calculating the values of the
	 *		  rectangle for help menus.
	 *	mi	- pointer to the current menu item of type structure
	 *		  Titem.
	 *	mdata	- A structure of the type Menudata that contains
	 *		  the information regarding the size and layout
	 *		  of the menu for this depth.  This structure was
	 *		  defined so that a larger number of variables can
	 *		  passed with a single pointer to different subroutines.
	 *	t	- Pointer to a global structure that contains the
	 *		  origin point for a menu at a given depth.  These
	 *		  are generated in tmenuhit() and passed to tmhit()
	 *		  for the initial generation of the menu tree.  They
	 *		  are changed whenever the a new sub-menu is made.
	 *	newtop	- contains the index of the menu item that is to
	 *		  displayed for the menu.  Normally this value will
	 *		  zero, except when scrolling menus are used.
	 *	hit	- The index value used to indicate the current menu item
	 *		  that is selected.  This value is offset by the value
	 *		  newtop or md->top to determine the true index into
	 *		  the menu items for a given menu.
	 *	newhit	- Functions the same as newhit except it represents
	 *		  the latest value.  "hit" is then updated to
	 *		  "newhit" after "newhit" has been validated.  Validation
	 *		  includes that the value is not a hit on a greyed item,
	 *		  and that the value is adjusted (through hysterisis)
	 *		  to insure that the mouse has been fully moved over
	 *		  a new selection.
	 */
	Point p, savep;
	Rectangle rside, rhit;
	register Titem *mi;
	Menudata mdata;
	register Tleaf *t;
	register Menudata *md;
	register int newtop, hit, newhit, greyed;
	Tmenu *scanmenu();

#define sro md->sr.origin
#define src md->sr.corner
#define tro md->tr.origin
#define trc md->tr.corner
#define mro md->mr.origin
#define mrc md->mr.corner
#define bro md->br.origin
#define brc md->br.corner
#define tbro tbranch.origin
#define tbrc tbranch.corner

	/*
	 * Set-up data and other pointers for globals and data to be
	 * passed to other subroutines.  This includes the origin point
	 * for the menu, and data regarding the size of the menu itself.
	 */
	t = &tleaf[depth];
	p = t->org;
	md = &mdata;
	md->m = m;

	/*
	 * If the treegrow flag is set, then this is a first invocation of
	 * the menu tree and menus are to be generated to the bottom most
	 * leaf based on the previous values stored in the menu data
	 * structures.  These will be zero if these menus are called for the
	 * first time ever.
	 *
	 * Otherwise, each menu must be scanned by scanmenu() to generate the
	 * necessary data information concerning the menu to be generated.
	 */
tmhit_top:
	if (!treegrow) {
		scanmenu(m, t);
	}
	if((t->items == 0) || ((t+1)->org.x > mouse.xy.x))
		/*
		 * Unset the treegrow flag if we hit a menu that
		 * is zero-sized or if we have a case of overlapping
		 * menus that caused the menu origins to be re-
		 * calculated and therefore moved to the right of
		 * of the mouse.
		 */
		treegrow = 0;

	/*
	 * Set-up all the rectangles which define the size and layout of
	 * the menu to be generated.  Adjustments are made to the origin
	 * for the menu depending on whether or not it fits on the screen.
	 */	
	mro.x = mro.y = 0;
	sro.x = sro.y = src.x = tro.x = tro.y = BORDER;
	bro.x = bro.y = brc.x = brc.y = BORDER;
	if(t->items <= DISPLAY)
		md->lines = t->items;
	else {
		md->lines = DISPLAY;
		tro.x += BARWIDTH;
		src.x += BARWIDTH;
	}
	mrc = add(mro, Pt(t->width, min(t->items, md->lines)*t->spacing + BORDER*2));
	trc = add(mrc, Pt(-BORDER,-BORDER));
	src.y = trc.y;

	newtop = bound(m->prevtop, 0, t->items - md->lines);
	p.y -= bound(m->prevhit, 0, md->lines-1)*t->spacing+t->spacing/2;
	p.x = bound(p.x, 0, XMAX-mrc.x);
	p.y = bound(p.y, 0, YMAX-mrc.y);
	md->sr = raddp(md->sr, p);
	md->tr = raddp(md->tr, p);
	md->mr = raddp(md->mr, p);
	rside = md->mr;
	rside.origin.x = rside.corner.x - 20 - BORDER;

	/*
	 * See if there is enough memory to save the bitmap for the
	 * area of the screen that will be obscured as result of the
	 * menu display.  If there is, then bitblt that portion of the
	 * screen to the allocated memory.  Then clear the screen and
	 * draw the menu border.  Additional testing is done to insure
	 * that should the menu be too large to fit in a screen bitmap,
	 * the rectangle of the menu is clipped to the bitmap of the screen.
	 * The variable rhit is temporarily used to store the clipped rect.
	 *
	 * If there isn't enough bitmap memory, then just XOR the menu.
	 */
	rhit = md->mr;
	rectclip(&rhit, display.rect);
	md->b = balloc(rhit);
	cursinhibit();
	if(md->b)
		bitblt(&display, rhit, md->b, rhit.origin, F_STORE);
	rectf(&display, md->mr, (md->b ? F_CLR:F_XOR));
	if(md->b)
		rectf(&display, inset(md->mr, BORDER-1), F_OR);
	rectf(&display, inset(md->mr, BORDER), (md->b ? F_CLR:F_XOR));
	cursallow();

	/*
	 * Begin that part of the routine where the drawing of the menu
	 * commences.  Set up the proper value for the menu item that
	 * will the first item in the display (if this is a scrolling menu,
	 * otherwise this value will be zero).  A call to the drawmenu()
	 * routine is made which performs the actual drawing of the menu
	 * contents.
	 */
PaintMenu:
	md->top = newtop;
	drawmenu(&mdata, t);
	mi = 0;

	/*
	 * This is the major looping code that processes the movement of the
	 * mouse and whether there were any help menus requested.  Movement
	 * of the mouse on a sub-menu icon (the B_rtarrow icon) generates a
	 * recursive call to this routine to display the submenu.  A current
	 * menu is exited by moving the mouse to the left of the current menu
	 * display except for the topmost (root) menu or when the menu button
	 * is released.  Because of race conditions it is possible to change
	 * the button state multiple times, therefore to insure that this
	 * subroutine exits, if tmselect is non-zero, we're really exiting.
	 */
#ifndef MTG630
	p = mouse.xy;
#endif MTG630
	for(newhit = hit = -1; ((tstatic && ((but && !bttn(but)) ||
	   (!but && !bttn123()))) || (!tstatic && ((but && bttn(but)) ||
	   (!but && bttn123())))) && (tmselect == 0); nap(2)) {
		savep=p;
		p = mouse.xy;
#ifndef MTG630
		if(cursxyonly && !eqpt(savep, p)) {
			register int deltax, deltay;
			/*
			**  Fake 630 firmware for x-y mouse movement
			*/
			deltax = p.x - savep.x;
			deltay = p.y - savep.y;
			if (deltax < 0)
				deltax = -deltax;
			if (deltay < 0)
				deltay = -deltay;
			if (deltax > deltay)
				p.y = savep.y;
			else
				p.x = savep.x;
			cursset(p);
		}
#endif MTG630

		/*
		 * Set the mouse movement for x-y only mode when the
		 * mouse cursor is within a menu.
		 */
		if(ptinrect(p, t->tbranch) || ptinrect(p, md->mr))
			xyon();
		else
			xyoff();

		/*
		 * Check if the menu should be exited because we are not
		 * generating the entire tree and the mouse is not in the
		 * in the sub-menu icon of the previous menu and the
		 * mouse is to the left of the current menu.
		 */
		if(!treegrow && !ptinrect(p, t->tbranch) && depth && (p.x < mro.x))
		{
			break;
		}

		/*
		 * Processing when the mouse is in the scroll bar region.
		 */
		if(ptinrect(p, md->sr)){
			/*
			 * we are still in the scroll bar, so update the
			 * menu appropriately to give the appearance of
			 * scrolling.
			 */
			if (ptinrect(p, md->br) && !ptinrect(savep,md->br)){
				p.y = (bro.y+brc.y)/2;
				cursset(p);
			}
			if (ptinrect(p, md->br) || ptinrect(savep,md->br)){
				newtop = scale(p.y, sro.y, src.y, 0, t->items);
				newtop = bound(newtop-DISPLAY/2, 0, t->items-DISPLAY);
				if(newtop != md->top) {
					erasemenu(&mdata, t, hit, greyed);
/* ->->-> */				goto PaintMenu;
				}
			}
		}

		/*
		 * Processing to determine a menu selection
		 */
		newhit = -1;
		mi = 0;
		if(ptinrect(p, ((t->items > DISPLAY) ? md->tr:md->mr))) {
			newhit = bound((p.y-tro.y)/t->spacing, 0, md->lines-1);
			if(newhit!=hit && hit>=0
			   && (p.y > (tro.y + hit*t->spacing - 3))
			   && (p.y < (tro.y + (hit+1)*t->spacing + 3)))
				newhit = hit;
			rhit = md->tr;
			rhit.corner.y = (rhit.origin.y += newhit*t->spacing)
						       + t->spacing;
		}
		if(treegrow) {
			/*
			 *  If we're supposed to expand, then make it look
			 *  like we've made a selection in this menu.  But,
			 *  made sure that prevhit makes sense since the
			 *  size of the menu may have changed since the last
			 *  time menuhit was invoked.
			 */
			md->top = m->prevtop;
			if ((newhit = m->prevhit) >= md->lines)
			{
				newhit = -1;
				treegrow = 0;
			}
		}
		if(newhit != -1) {
			/*
			 *  Get the menu item associated with the latest
			 *  newhit value.  The subroutine tablegen() also
			 *  sets the global tuser_mi which is the menu 
			 *  item value that eventually is returned to the
			 *  user.
			 */
			mi = tablegen(md->top+newhit, m);
		}
		else
			tuser_mi = (Titem *)0;

		/*
		 * Processing that is done after a selection is determined.
		 * This includes generating the reverse video for the current
		 * selection.
		 */
		if(newhit != hit)
		{
			if(!greyed) {
				flip(md->tr, hit, T_black, t);
			}
			if(mi->ufield.grey) {
				greyed = 1;
			}
			else {
				flip(md->tr, newhit, T_black, t);
				greyed = 0;
			}
			hit = newhit;
		}

		/*
		 * Processing to determine the generation of a sub-menu.
		 * Note that if there are any user-provided functions for
		 * executing before and returning from a sub-menu, that
		 * occurs here.
		 */
		if((newhit != -1) && (ptinrect(p, rside) || treegrow)) {
			if((depth < MAXDEPTH-1) && mi->next && !(mi->ufield.grey&0x01)) {
				if(mi->dfn) (*mi->dfn)(tuser_mi);
				(t+1)->tbranch = md->tr;
				(t+1)->tbro.x = (t+1)->tbrc.x - 20;
				(t+1)->tbrc.x += BORDER;
				(t+1)->tbro.y = bound(tro.y + hit*t->spacing - 3,
						mro.y, mrc.y);
				(t+1)->tbrc.y = bound(tro.y + (hit+1)*t->spacing + 3,
						mro.y, mrc.y);
				if (!treegrow) {
					(t+1)->org.x = mrc.x - (BORDER-1);
					(t+1)->org.y = mro.y
					   	  + newhit*t->spacing
					   	  + t->spacing/2;
				}
				t->md = md;
				t->newhit = newhit;
				tmhit(mi->next, but, depth+1);
				mi = tablegen(t->md->top+newhit, t->md->m);
				if(mi->bfn) (*mi->bfn)(tuser_mi);
			}
			else {
				treegrow = 0;
			}
		}

		/*
		 * Handles re-drawing of scrolling menus.
		 */
		if(newhit==0 && md->top>0){
			newtop = md->top-1;
			p.y += t->spacing;
			cursset(p);
			erasemenu(&mdata, t, hit, greyed);
/* ->->-> */		goto PaintMenu;
		}
		if(newhit==DISPLAY-1 && (md->top < (t->items - md->lines))){
			newtop = md->top+1;
			p.y -= t->spacing;
			cursset(p);
			erasemenu(&mdata, t, hit, greyed);
/* ->->-> */		goto PaintMenu;
		}
	}

	/*
	 * Exiting menu, clean-up screen and bitmaps
	 */
	if(md->b){
		cursinhibit();
		screenswap(md->b, md->b->rect, md->b->rect);
		cursallow();
		bfree(md->b);
	}
	else {
		erasemenu(&mdata, t, hit, greyed);
		rectf(&display, md->mr, F_XOR);
		rectf(&display, inset(md->mr, BORDER), F_XOR);
	}

	/*
	 * Set-up return value and execute user specified function
	 * on menu item selection IFF the button state has changed.
	 */
	if(((tstatic && ((but && bttn(but)) || (!but && bttn123())))
	   || (!tstatic && ((but && !bttn(but)) || (!but && !bttn123()))))
	   && (tmselect == 0)) {
		if((mi == 0) || ((mi=tablegen(md->top+newhit, m)) && (mi->ufield.grey & 0x01)))
		{
			tmselect = NOSELECT;
		}
		else {
			m->prevhit = hit;
			m->prevtop = md->top;
			if(mi->hfn)
				(*(mi->hfn)) (tuser_mi);
			if(!redraw) {
tm_return:
				tmselect = SELECT;
				select_menu = m;
			}
			else {
				if(depth)
				{
					mi = tablegen((t-1)->md->top+(t-1)->newhit, (t-1)->md->m);
					if(mi->bfn)
						(*mi->bfn) (tuser_mi);
					for(newtop=1; newtop<=depth; newtop++) {
						/*
					 	* update all the parent menus'
					 	* prevhit and prevtop. A small
					 	* kludge in that to reduce stack
					 	* space, newtop is used and then
					 	* reset back at the top.
					 	*/
						(t-newtop)->md->m->prevtop = (t-newtop)->md->top;
						(t-newtop)->md->m->prevhit = (t-newtop)->newhit;
					}
					if(mi->dfn)
						(*mi->dfn) (tuser_mi);
					if(!redraw)
						goto tm_return;
				}
				while((tstatic && ((but && bttn(but)) ||
				   (!but && bttn123()))) || (!tstatic && ((but
				   && !bttn(but)) || (!but && !bttn123())))) ;
				p = mro;
				p.y += hit*t->spacing+t->spacing/2;
				goto tmhit_top;
			}
		}
	}
	if(hit>=0 && tmselect==SELECT && !wasredraw)
	{
		m->prevhit = hit;
		m->prevtop = md->top;
	}
/*end*/
}

/*
 *	Draws a menu given the Tleaf structure.  The position of the
 *	menu is calculated elsewhere.  This routine determines the
 *	individual placement of the text and icons within the menu.
 */

drawmenu(md, t)
	register Menudata *md;
	register Tleaf *t;
{
	Point p, q, r;
	register int i, j, k;
	register char *from, *to;
	register Titem *mi;

	cursinhibit();
	if(t->items > DISPLAY){
		bro.y = scale(md->top, 0, t->items, sro.y, src.y);
		bro.x = sro.x + 1;
		brc.y = scale(md->top+DISPLAY, 0, t->items, sro.y, src.y);
		brc.x = src.x - 1;
		rectf(&display, md->br, F_XOR);
	}
	for(j=0, p=tro, i=md->top; i < min(md->top+md->lines, t->items); ++i, ++j){
		q = p;
		mi = tablegen(i, md->m);
		if(mi->ufield.grey & 0x01)
			flip(md->tr, j, T_grey, t);
		q.x += SPACEPAD;
		if (t->iconwidth) {
			if(mi->icon)
			{
				r = q;
				r.y += (t->spacing - (mi->icon->rect.corner.y - 
				       mi->icon->rect.origin.y))/2;
				r.x += (t->iconwidth - (mi->icon->rect.corner.x -
				       mi->icon->rect.origin.x))/2;
				bitblt(mi->icon, mi->icon->rect, &display, r,
					 (md->b ? F_OR:F_XOR));
			}
			q.x += t->iconwidth + SPACEPAD;
		}
		from = mi->text;
		for(to = &tfill[0]; *from && (to < &tfill[MAXCHARS]); ++from)
			if(*from & 0x80) {
				*to = '\0';
				k = t->txtwidth
				    - (strbits(tfill,mi->font)
				       + strbits(from+1,mi->font));
				while(k > 0) {
					k -= mi->font->info[*to++ = *from & 0x7F].width;
				}
			}
			else
				*to++ = *from;
		*to = '\0';
		q.x += (t->txtwidth-strbits(tfill,mi->font))/2;
		q.y += (t->spacing - mi->font->height)/2 + 1;
;
		string(mi->font, tfill, &display, q, (md->b ? F_OR:F_XOR));
		if(mi->next)
		{
			bitblt(&B_rtarrow, B_rtarrow.rect, &display,
			       Pt(trc.x-20, p.y+(t->spacing - 16)/2),
			       (md->b ? F_OR:F_XOR));
		}
		if(mi->ufield.grey & 0x01)
			flip(md->tr, j, T_grey, t);
		p.y += t->spacing;
	}
	cursallow();
}

/*
 *	Erase a menu by drawing over it again in XOR mode,
 *	or clearing the rectangle occupied by the menu.  The
 *	method depends on whether the menu is being XOR'ed on
 *	top of the current screen contents, or the rectangle
 *	for the menu is alloc'ed memory.  This routine is
 *	used for clearing XOR'ed menus, or updating scrolling
 *	menus.
 */

erasemenu(md, t, hit, greyhit)
	register Menudata *md;
	register Tleaf *t;
	register int hit;
{
	if(md->b) {
		cursinhibit();
		rectf(&display, inset(md->mr, BORDER), F_CLR);
		cursallow();
	}
	else {
		if (!greyhit)
			flip(md->tr, hit, T_black, t);
		drawmenu(md, t);
	}
}

/*
 *	Routine which looks at a menu and derives the global
 *	characteristics associated with that menu.  These are
 *	stored in a Tleaf structure.  This routine also returns
 *	a pointer to a submenu, if the submenu exists and was
 *	previously selected as determined by the prevhit and
 *	prevtop values.
 */
Tmenu *
scanmenu(m, t)
	register Tmenu *m;
	register Tleaf *t;
{
	register Titem *mi;
	register int iconwidth, iconheight, fontheight, rside, length, items;
	register Tmenu *next_m;
	Titem *tablegen();

	next_m = 0;
	iconwidth = iconheight = fontheight = rside = length = items = 0;
	for(; (mi=tablegen(items, m))->text; ++items) {
		length = max(length, strbits(mi->text, mi->font));
		fontheight = max(fontheight, mi->font->height);
		if(mi->icon) {
			iconwidth = max(iconwidth, mi->icon->rect.corner.x -
					mi->icon->rect.origin.x);
			iconheight = max(iconheight, mi->icon->rect.corner.y -
					 mi->icon->rect.origin.y);
		}
		if(mi->next) {
			rside=1;
			if( (items == (m->prevhit + m->prevtop))
			    && (mi->ufield.grey == 0))
				next_m = mi->next;
		}
	}
	t->spacing = 4 + max(fontheight, iconheight);
	t->iconwidth = iconwidth;
	t->txtwidth = length;
	t->width = BORDER*2 + SPACEPAD + (length ? (SPACEPAD + length):0)
		 + (iconwidth ? iconwidth + SPACEPAD:0) + (rside ? 20:0)
		 + ((items > DISPLAY) ? BARWIDTH:0);
	t->items = items;
	return(next_m);
}


flip(r, n, code, t)
	Rectangle r;
	Texture code;
	register int n;
	register Tleaf *t;
{
	if(n<0 || n>=t->items)
		return;
	r.corner.y = (r.origin.y += t->spacing*n) + t->spacing;
	texture(&display, r, &code, F_XOR);
}

/*
 *	this function is the same as jstrwidth() except that it
 *	masks out the most significant bit in the char, (the
 *	spread bit in spread chars).  limits the chars to MAXCHARS.
 */

strbits(str, font)

	register char *str;
	register Font *font;
{
	register int bits;
	register int chars;

	chars = bits = 0;
	while(*str && (chars++ < MAXCHARS)) {
		bits += font->info[*str++ & 0x7f].width;
	}
	return(bits);
}


/*
 * The following routine is used by hfn,bfn,dfn when TM_NORET is set
 * so that a return from the menu can be done by a selected item.
 *
 */

void
tm_ret()
{
	redraw = 0;
	tmselect = NOSELECT;
}

#endif
