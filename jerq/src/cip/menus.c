/*
  %Z%  %M%  version %I% %Q%of %H% %T%
  Last Delta:  %G% %U% to %P%
*/

#include "cip.h"


extern int currentBrush, copyFlag, thingSelected, editDepth;
extern Rectangle brushes[];
extern int gridState, videoState, buttonState;
extern short noSpace;
extern Point jString ();
extern void getKbdChar ();
extern void jMoveTo ();
extern struct macro *macroList;

char *lineText[] = {"delete","copy",
        "solid","dotted","dashed","arrow",
        "reflect x","reflect y",NULL};
Menu lineMenu = { lineText };

char *boxText[] = {"delete","copy","solid","dotted","dashed",NULL};
Menu boxMenu = { boxText };

char *circleText[] = {"delete","copy",NULL};
Menu circleMenu = { circleText };

char *splineText[] = {"delete", "copy", "arrow",
		      "reflect x", "reflect y", NULL};
Menu splineMenu = { splineText };

char *arcText[] = {"delete","copy","reflect x","reflect y",NULL};
Menu arcMenu = { arcText };

char *macroText[] = {"delete","copy","reflect x","reflect y",
                     "edit","separate",NULL};
Menu macroMenu = { macroText };

char *textText[] = {"delete","copy","point size","roman",
                    "italic","bold", "center","left justify",
		    "right justify","attributes",
                    NULL};
Menu textMenu = { textText };

#define GET 0
#define PUT 1
#define CLEAR 2
#define REDRAW 3
#define DEFINEMACRO 4
#define GRID 5
#define FLIPVIDEO 6
#define QUIT 7
#define GCMemory 8
#define DUMPIT 9

char *commandText[] = {"get file","put file","clear screen", 
		       "redraw screen","define macro","grid",
		       "reverse video","quit", "GC Memory",
#ifdef DUMP
		       "dump",
#endif
		       NULL};
Menu commandMenu = { commandText };

Texture pointsize = {
  0x07E0, 0x1FF8, 0x3FFC, 0x7FFE, 0x7FFE, 0xE187, 0xEDB7, 0xEDBF,
  0xE18F, 0xEFF7, 0xEFF7, 0x6F86, 0x7FFE, 0x3FFC, 0x1FF8, 0x07E0
};

Texture candle = {
  0xFEFF, 0xFEFF, 0xFC7F, 0xF83F, 0xFC7F, 0xFFFF, 0xF83F, 0xF83F,
  0xF83F, 0xF83F, 0xF83F, 0xF83F, 0xF821, 0xF82D, 0x8001, 0xFFFF
};

int
menuHit (menu, but)
Menu *menu;
int but;
{
  int item;

  cursswitch ((Word *)NULL);
  item = menuhit(menu, but);
  cursSwitch ();
  return (item);
}

struct thing *
insertReflect (t, p, item, which)
struct thing *t;
Point p;		/* Offset */
int item;
int which;		/* Which item to be tested */
{
  register struct thing d;	/* Dummy structure */
  register struct thing *h;	/* New head of thing list */

  drawSelectionLines(t,p);
  draw(t,p);
  h = remove(t);
  d = *t;
  free(t);				
  h = (item==which) ? 
      insert(reflect(&d,Pt(0,d.bb.corner.y+d.bb.origin.y)),h)
    : insert(reflect(&d,Pt(d.bb.corner.x+d.bb.origin.x,0)),h);
  draw(h,p);
  drawSelectionLines(h,p);
  return (h);
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
    macroList = (struct macro *)NULL;
  }
  else {
    for (ml = macroList; ml != (struct macro *)NULL; ml = ml->next) {
      if (ml->next == m) {	/* Macro found delete it. */
	ml->next = m->next;
      }
    }
  }
  free (m);
  return;
}

struct thing *
displayThingMenu(m,t,p) 
Point m;			/* Mouse location */
Point p;			/* Offset */
register struct thing *t;	/* Thing to be displayed */
{
  register int item; 
  register int i, b, oldED; 
  register struct thing *pts;
  register struct thing *h, *g, *f; 
  Rectangle r; register char c, s[10];

  switch(t->type) {
    case LINE: {
      item = menuHit (&lineMenu, 3);
      b = -1;
      switch (item) {
	case 2: {
	  /* make line solid */
	  b = SOLID;
	  break;
	}
	case 3: {
	  /* make line dotted */
	  b = DOTTED;
	  break;
	}
	case 4: {
	  /* make line dashed */
	  b = DASHED;
	  break;
	}
	case 5: {
	  /* add or remove arrowheads */
	  draw(t,p);
	  if (distance(m,t->origin) < distance(m,t->otherValues.end)) {
	    if ((t->arrow==startARROW)||(t->arrow==doubleARROW)) {
	      t->arrow -= startARROW;
	    }
	    else {
	      t->arrow += startARROW;
	    }
	  }
	  else {
	    if ((t->arrow==endARROW)||(t->arrow==doubleARROW)) {
	      t->arrow -= endARROW;
	    }
	    else {
	      t->arrow += endARROW;
	    }
	  }
	  draw(t,p);
	  break;
	}
	case 6:
	case 7: {
	  t = insertReflect (t, p, item, 6);
	  break;
	}
      }
      if (b >= 0 && b!=t->boorder) {
	draw(t,p);
	t->boorder = b;
	draw(t,p);
      }
      break;
    }
    case BOX: {
      item = menuHit(&boxMenu,3);
      b = -1;
      switch(item) {
	case 2: {	/* make box outline solid */
	  b = SOLID;
	  break;
	}
	case 3: {	/* make box outline dotted */
	  b = DOTTED;
	  break;
	}
	case 4: {	/* make box outline dashed */
	  b = DASHED;
	  break;
	}
      }
      if (b>=0 && b!=t->boorder) {
	draw(t,p);
	t->boorder = b;
	draw(t,p);
      }
      break;
    }
    case MACRO: {
      item = menuHit(&macroMenu,3);
      switch (item) {
	case 2: 
	case 3: {
	  r = t->otherValues.list->bb;
	  h = TNULL;
	  draw(t,p);
	  if (item==2 && t->otherValues.list->xReflectionOf
	      != (struct macro *)NULL) {
	    t->otherValues.list->useCount--;
	    t->otherValues.list = t->otherValues.list->xReflectionOf;
	  }
	  else {
	    if (item==3 && t->otherValues.list->yReflectionOf
	        != (struct macro *)NULL) {
	      t->otherValues.list->useCount--;
	      t->otherValues.list = t->otherValues.list->yReflectionOf;
	    }
	    else {
	      /* Go thru parts list, reflect all things, and */
	      /* make copies of them.  The copies go into the list h. */
	      if ((g=t->otherValues.list->parts)
		   !=TNULL) {
		do {
		  h = (item==2) ? 
		      insert(reflect(
		      g,Pt(0,r.origin.y+r.corner.y)),h)
		      : insert(reflect(
		      g,Pt(r.corner.x+r.origin.x,0)),h);
		  g = g->next;
		} while (g != t->otherValues.list->parts);
	      }
	      t->otherValues.list = recordMacro(h,r,
		  (item==2)?t->otherValues.list :(struct macro *)NULL,
		  (item==2)?(struct macro *)NULL :t->otherValues.list,
		  NULL);
	    }
	  }
	  t->otherValues.list->useCount++;
	  draw(t,p);
	  break;
	}
	case 4: {
	  /* edit macro */
	  oldED = editDepth;
	  removeReflectionReferences(t->otherValues.list);
	  pts = t->otherValues.list->parts;
	  /* Draw edit button when editDepth==1, other times */
	  /* undraw button. */
	  if ((editDepth)!=0) {
	    drawEDbutton(editDepth);
	  }
	  drawEDbutton(++editDepth);
	  changeButtons (INITbuttons);
	  thingSelected = 0;
	  while (editDepth>oldED) {
	    pts = doMouseButtons(pts,add(p,t->origin));
	  }
	  t->otherValues.list->parts = pts;
	  t->otherValues.list->bb = macroBB(pts);
	  if (thingSelected) {
	    drawSelectionLines (t, p);
	    thingSelected = 0;
	  }
	  if (editDepth==0) {
	    doRedraw(t, p);
	  }
	  break;
	}
	case 5: {	/* separate */
	  h = remove(t);	/* Cut macro from thing list */
	  t->otherValues.list->useCount--;
	  m = sub(Pt(0,0),t->origin);
	  /* Go thru macro's thing list and make everything relative */
	  if ((g=t->otherValues.list->parts)!=TNULL) {
	    if (t->otherValues.list->useCount > 0) {
	      do {	/* Copy list */
		if ((f=(struct thing *)getSpace(sizeof(struct thing))) 
		     == TNULL) {
		  break;
		}
		g = g->next;
		*f = *g;
		f = makeRelative (f, m);
		h = insert (f, h);
	      } while (g != t->otherValues.list->parts);
	    }
	    else {
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
	  free(t);		/* Remove macro thing */
	  t = h;
	  break;
	}
      }
      break;
    }
    case TEXT: {
      item = menuHit(&textMenu,3);
      switch (item) {
	case 2: {	/* point size */
	  m = MOUSE_XY;
	  m.x += 20;
	  getKbdText (s, m, 0, &pointsize, sizeof (s)-1);
	  b = atoi (s);
	  draw(t,p);
	  t->otherValues.text.f->useCount--;
	  t->otherValues.text.f = 
	      findFont(b,t->otherValues.text.f->num);
	  draw(t,p);
	  BoundingBox(t);
	  break;
	}
	case 3: 	/* roman face */
	case 4: 	/* italic face */
	case 5: {	/* bold face */
	  draw(t,p);
	  t->otherValues.text.f->useCount--;
	  t->otherValues.text.f = 
	      findFont(t->otherValues.text.f->ps,item-2);
	  BoundingBox(t);
	  draw(t,p);
	  break;
	}
	case 6: 	/* center */
	case 7: 	/* left justify */
	case 8: {	/* right justify */
	  draw(t,p);
	  t->otherValues.text.just = item - 6;
	  BoundingBox(t);
	  draw(t,p);
	  break;
	}
	case 9: {	/* show attributes */
	  char buf[25];
	  cursswitch(&candle);
	  sprintf(buf,"%s  f:%c  ps:%d",
	      (t->otherValues.text.just==CENTER)?"center":
	      ((t->otherValues.text.just==LEFTJUST)?"ljust":"rjust"),
	      (t->otherValues.text.f->num==ROMAN)?'R':
	      ((t->otherValues.text.f->num==ITALIC)?'I':'B'),
	      t->otherValues.text.f->ps);
	  m = MOUSE_XY;
	  jMoveTo(Pt(m.x+20,m.y));
	  jString(buf);
	  wait(MOUSE);
	  while (!button123()) {
	    nap(2);
	  }
	  jMoveTo(Pt(m.x+20,m.y));
	  jString(buf);
	  cursSwitch();
	  break;
	}
      }
      break;
    }
    case CIRCLE:
    case ELLIPSE: {
      item = menuHit(&circleMenu,3);
      break;
    }
    case ARC: {
      item = menuHit(&arcMenu,3);
      switch (item) {
	case 2: 
	case 3: {
	  t = insertReflect (t, p, item, 2);
	  break;
	}
      }
      break;
    }
    case SPLINE: {
      item = menuHit(&splineMenu,3);
      switch (item) {
	case 2: {	/* arrow */
	  b = t->otherValues.spline.used;
	  if (distance(m,t->origin)<
	      distance(m,t->otherValues.spline.plist[b])) {
	    if ((t->arrow==startARROW)||(t->arrow==doubleARROW)) {
	      t->arrow -= startARROW;
	    }
	    else {
	      t->arrow += startARROW;
	    }
	    arrow(add(p,t->otherValues.spline.plist[2]),
	          add(p,t->otherValues.spline.plist[1]));
	  }
	  else {
	    if ((t->arrow==endARROW)||(t->arrow==doubleARROW)) {
	      t->arrow -= endARROW;
	    }
	    else {
	      t->arrow += endARROW;
	    }
	    arrow(add(p,t->otherValues.spline.plist[b-2]),
		  add(p,t->otherValues.spline.plist[b-1]));
	  }
	  break;
	}
	case 3: 
	case 4: {
	  t = insertReflect(t, p, item, 3);
	  break;
	}
      }
      break;
    }
  }
  if (item == 1) /* copy */ {
    if (!noSpace) {
      copyFlag=1;
      changeButtons(COPYbuttons);
    }
  }
  else {
    if (item == 0) {
      /* delete */
      drawSelectionLines(t,p);
      t = deleteThing(t,p);
      thingSelected = 0;
      copyFlag = 0;
      changeButtons(INITbuttons);
    }
  }
  return(t);
}

static Texture rusure = {
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
  changeButtons (QUITbuttons);
  while (!button123());
  if (button3()) {
    ret = 1;
  }
  changeButtons (savebuttonstate);
  while (button12());
  cursSwitch();
  return (ret);
}

struct thing *
displayCommandMenu(h, offset) 
register struct thing *h;
Point offset;
{
  int item;
  item = menuHit(&commandMenu,3);
  switch (item) {
    case GET: {
      h = doGet(h);
      break;
    }
    case PUT: {
      doPut(h);
      break;
    }
    case CLEAR: {
      if (RUsure()) {
	h = doClear(h);
      }
      break;
    }
    case DEFINEMACRO: {
      if (!noSpace) {
	h = defineMacro(h);
      }
      break;
    }
    case REDRAW: {
      doRedraw(h, offset);
      break;
    }
    case QUIT: {
      if (RUsure()) {
	exit (1);
      }
      break;
    }
    case GRID: {
      gridState = (gridState==GRIDon)?GRIDoff:GRIDon;
      drawGrid();
      break;
    }
    case FLIPVIDEO: {
      if (videoState==WHITEfield) {
	videoState=BLACKfield;
	rectf (&display, Jrect, F_XOR);
      }
      else {
	videoState=WHITEfield;
	rectf (&display, Jrect, F_XOR);
      }
      break;
    }
    case GCMemory: {
      cursinhibit();
      while(!button123());
      while (button123()) {
	long y = mouse.xy.y;
	if (y<0 || y>1023) {
	  y = 0;
	}
	y = ((y*156)/100)*25;
	*DADDR=y;
      }
      while(button123());
      *DADDR=0;
      cursallow();
      break;
    }
#ifdef DUMP
    case DUMPIT: {
      dump (h);
      break;
    }
#endif
  }
  return(h);
}
      
doRedraw(h, offset) 
struct thing *h; 
Point offset;
{
  register struct thing *t;

  cursinhibit();
  xtipple(&display,brushes[PIC]);
  cursallow();
  if (gridState==GRIDon) {
    drawGrid();
  }
  if ((t = h) != TNULL) {
    do {
      if (t->type==MACRO) BoundingBox(t);
      draw(t,offset); 
      t = t->next;
    } while (t != h);
  }
}

void
setdata (p)	/* Setdata routine based on mux/boot.c */
register struct Proc *p;
{
  Drect = p->rect;
  Jdisplayp = (Bitmap *)p->layer;
}
