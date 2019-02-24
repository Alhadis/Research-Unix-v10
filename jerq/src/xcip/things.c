#ifndef	FONT
#define	FONT "$DMD/font"
#endif	FONT

#include "cip.h"

extern int nextMacroName;
extern Texture16 hourglass;
extern char *envfont;
extern int editDepth;
extern void removeMacro ();
extern void putMessage ();
extern void message ();
extern void StringSize();
extern short fontBells;
extern Point alignDown();
extern Point alignUp();


struct thing *
newCircle(p) 
Point p;
{
  register struct thing *b;

  if ((b = (struct thing *) getSpace(sizeof(struct thing))) != TNULL) {
    b->type = CIRCLE;
    b->origin = p;
    b->otherValues.radius = RADdefault;
    BoundingBox(b);
    b->arrow = 0;
  }
  return(b);
}

struct thing *
newBox(r) 
Rectangle r;
{
  register struct thing *b;

  if ((b = (struct thing *) getSpace(sizeof(struct thing))) != TNULL) {
    b->type = BOX;
    b->origin = r.origin;
    b->otherValues.corner = r.corner;
    BoundingBox(b);
    b->boorder = currBoxType;
    b->arrow = 0;
  }
  return(b);
}

struct thing *
newEllipse(p) 
Point p;
{
  register struct thing *b;

  if ((b = (struct thing *) getSpace(sizeof(struct thing))) != TNULL) {
    b->type = ELLIPSE;
    b->origin = p;
    b->otherValues.ellipse.ht = HTdefault;
    b->otherValues.ellipse.wid = WIDdefault;
    BoundingBox(b);
    b->boorder = SOLID;
    b->arrow = 0;
  }
  return(b);
}

struct thing *
newLine(o,c) 
Point o, c;
{
  register struct thing *b;

  if ((b = (struct thing *) getSpace(sizeof(struct thing))) != TNULL) {
    b->type = LINE;
    b->origin = o;
    b->otherValues.end = c;
    BoundingBox(b);
    b->boorder = currLineType;
    b->arrow = currLineArrows;
  }
  return(b);
}

struct thing *
newArc(s,e) 
Point s, e;
{
  register struct thing *b; 

  if ((b = (struct thing *) getSpace(sizeof(struct thing))) != TNULL) {
    b->type = ARC;
    b->otherValues.arc.start = s;
    b->otherValues.arc.end = e;
    b->origin = computeArcOrigin(s,e);
    BoundingBox(b);
    b->boorder = SOLID;
    b->arrow = 0;
  }
  return(b);
}

struct thing *
newText(p,s) 
Point p; char *s;
{
  register struct thing *b; fontBlk *f;
  static int r = 100 ;

  
  if ((b = (struct thing *) getSpace(sizeof(struct thing))) != TNULL) {
    b->type = TEXT;
    b->origin = p;
    f = findFont(currPS,currFont);
    b->otherValues.text.f = f;
    b->otherValues.text.just = currJust;
    b->otherValues.text.spacing = currSpacing;
    b->otherValues.text.s = s;
    BoundingBox(b);
    b->boorder = SOLID;
    b->arrow = 0;
    b->otherValues.text.outName = r++;
  }
  return(b);
}

struct thing *
newSpline(u,s,p) 
int u, s; 
Point *p;
{
  register struct thing *b;

  if ((b=(struct thing *) getSpace(sizeof(struct thing))) != TNULL) {
    b->type = SPLINE;
    b->origin = p[1];
    b->otherValues.spline.used = u;
    b->otherValues.spline.size = s;
    b->otherValues.spline.plist = p;
    BoundingBox(b);
    b->boorder = SOLID;
    b->arrow = currSplineArrows;
  }
  return(b);
}

struct thing *
newMacro(p,l) 
Point p; 
struct macro *l; 
{
  register struct thing *b;

  if ((b = (struct thing *) getSpace(sizeof(struct thing))) != TNULL) {
    b->type = MACRO;
    b->origin = p;
    b->otherValues.list = l;
    l->useCount++;
    BoundingBox(b);
    b->boorder = SOLID;
    b->arrow = 0;
  }
  return(b);
}		


struct thing *
insert(t,list) 
register struct thing *t, *list;
{
  if (t != TNULL) {
    if (list!=TNULL) {
      t->next = list;
      t->last = list->last;
      list->last->next = t;
      list->last = t;
    }
    else {
      t->last = t;
      t->next = t;
    }
    return (t);
  }
  return(list);
}

void
insertFont(t) 
register fontBlk *t;
{
  if (t != (fontBlk *) NULL) {
    t->next = fonts;
    t->last = fonts->last;
    fonts->last->next = t;
    fonts->last=t;
  }
}

struct thing *
remove(t) 
register struct thing *t;
{
  if (t != TNULL) {
    t->last->next = t->next;
    t->next->last = t->last;
  }
  return(( (t==TNULL) || (t==t->next)) ? TNULL : t->next);
}

struct thing *
deleteThing(t) 
register struct thing *t; 
{
  register struct thing *h; 
  register struct thing *t1;

  if (t!=TNULL) {
    t1 = remove(t);
    switch (t->type) {
      case MACRO: {
	if (--t->otherValues.list->useCount == 0) {
	  for (h=t->otherValues.list->parts; h!=TNULL; ) {
	    h = deleteThing (h);
	  }
	  removeMacro (t->otherValues.list);
	}
	break;
      }
      case TEXT: {
	t->otherValues.text.f->useCount--;
	free (t->otherValues.text.s);
	break;
      }
      case SPLINE: {
	free (t->otherValues.spline.plist);
	break;
      }
    }
    free((char *) t);
  }
  else {
    t1 = t;
  }
  return(t1);
}

struct thing *
deleteAllThings(list) 
struct thing *list;
{
  register struct thing *h;

  for (h=list; h!=TNULL; ){
    h=deleteThing (h);
  }
  return(TNULL);
}


  struct thing *
selectThing(m,list) 
  Point m; 
  register struct thing *list;
{
  register struct thing *t; 
  register int i;
  Rectangle r;

  if (list != TNULL) {
    /* Cycle thru things by starting with next thing. */
    t = list->next;
    do {
      switch( t->type ) {
      case SPLINE:
	/* See if point is inside imanginary boxes created
	   by each segment of the spline. */
	for( i=2; i<t->otherValues.spline.used; i++) {
	  r = canon( t->otherValues.spline.plist[i-1],
	             t->otherValues.spline.plist[i]    );
	  if( ptinrect( m , inset(r,-nearEDGE) ) )
	    return( t );
	}
	break;
      case LINE:
	if( (distance(t->origin,t->otherValues.corner) + nearEDGE) >= 
	    (distance(t->origin,m) + distance(t->otherValues.corner,m)) )
	  return( t );
	break;
      default:
	/* See if point is inside or near edge of bounding box. */
	if( ptinrect( m, inset(t->bb,-nearEDGE) ) )
	  return( t );
	break;
      }
      t=t->next;
    } while (t != list->next);
  }
  return ( TNULL );
}

int 
distance(p,q) 
Point p, q;
{	
  return ( norm(q.x-p.x,q.y-p.y,0) );
}

BoundingBox(t) 
  register struct thing *t;
{
  Point p, q, r; 
  register int i, h; 
  int hi, w;
  Font *f; 

  if (t != TNULL) {
    switch(t->type) {
      case CIRCLE: {
	p.x = t->otherValues.radius;
	p.y = p.x;
	t->bb.origin = sub(t->origin,p);
	t->bb.corner = add(t->origin,p);
	break;	
      }
      case BOX: {
	t->bb.origin = t->origin;
	t->bb.corner = t->otherValues.corner;
	break;
      }
      case ELLIPSE: {
	p.x = (t->otherValues.ellipse.wid)>>1;
	p.y = (t->otherValues.ellipse.ht)>>1;
	t->bb.origin = sub(t->origin,p);
	t->bb.corner = add(t->origin,p);
	break;
      }
      case LINE: {
	p = t->origin;
	q = t->otherValues.end;
	t->bb = canon (p, q);
	break;
      }
      case ARC: {
	p = t->origin;
	i = distance(p,t->otherValues.arc.start);
	t->bb.origin = sub(p,Pt(i,i));
	t->bb.corner = add(p,Pt(i,i));
	break;
      }
      case TEXT: {
	f = t->otherValues.text.f->f;
	h = f->height;
	p = add( t->origin , Pt(0,h>>1) );
	StringSize(f,t->otherValues.text.spacing,t->otherValues.text.s,&hi,&w);

	/* Calculate minimal non-aligned bounding box for text */
	switch (t->otherValues.text.just) {
	case CENTER:
	  t->bb.origin = sub( p, Pt(w>>1 , hi>>1) );
	  t->bb.corner = add( p, Pt(w>>1 , hi>>1) );
	  break;
	case LEFTJUST:
	  t->bb.origin = sub( p, Pt(0 , hi>>1) );
	  t->bb.corner = add( p, Pt(w , hi>>1) );
	  break;
	case RIGHTJUST:
	  t->bb.origin = sub( p, Pt(w, hi>>1) );
	  t->bb.corner = add( p, Pt(0, hi>>1) );
	  break;
	}
	break;
      }
      case SPLINE: {
	p.x = Xmin; p.y=YPIC; q.x=Xmax; q.y=YBOT;
	for (i=1; i<t->otherValues.spline.used; i++) {
	  r = t->otherValues.spline.plist[i];
	  p.x = max(p.x,r.x);  p.y = max(p.y,r.y);
	  q.x = min(q.x,r.x);  q.y = min(q.y,r.y);
	}
	t->bb.origin = q;
	t->bb.corner = p;
	break;
      }
      case MACRO: {
	t->bb.origin = add(t->origin,t->otherValues.list->bb.origin);
	t->bb.corner = add(t->origin,t->otherValues.list->bb.corner);
	break;
      }
    }
  }
}

Point 
computeArcOrigin(s,e) 
Point s,e;
{
  Point t;

  if (e.x<s.x) {	/*swap s and e */
    t=s; s=e; e=t;
  }
  return( div(add(add(e,Pt(s.y-e.y,e.x-s.x)),s),2) );
}


char fontString[3] = { '\0', '\0', '\0' };

font2string( n ) 
	int n;
{
      fontString[1] = '\0';
      switch( n ) {
      case ROMAN:
	fontString[0] = 'R';
	break;
      case ITALIC:
	fontString[0] = 'I';
	break;
      case BOLD:
	fontString[0] = 'B';
	break;
      case HELVETICA:
	fontString[0] = 'H';
	break;
      case HI:
	fontString[0] = 'H';
	fontString[1] = 'I';
	break;
      case HB:
	fontString[0] = 'H';
	fontString[1] = 'B';
	break;
      case PALATINO:
	fontString[0] = 'P';
	fontString[1] = 'A';
	break;
      case PI:
	fontString[0] = 'P';
	fontString[1] = 'I';
	break;
      case PB:
	fontString[0] = 'P';
	fontString[1] = 'B';
	break;
      case EXPANDED:
	fontString[0] = 'E';
	break;
      case EI:
	fontString[0] = 'E';
	fontString[1] = 'I';
	break;
      case EB:
	fontString[0] = 'E';
	fontString[1] = 'B';
	break;
      case CONSTANTWIDTH:
	fontString[0] = 'C';
	fontString[1] = 'W';
	break;
      }
}


/* This routine searches the list pointed to by the global 'fonts' */
/* for a font with point size s and type n.  If it can't find that */
/* font then defont is used.                                       */
/* Special case:  If the user runs out of space, then the first    */
/* entry, which is always defont, is used.  This will allow the    */
/* user to write out the file with the font description intact.    */

	fontBlk *
findFont(ps, n)
	register short		ps;	/* Point size of desired font */
	register short		n;  /* Type of font - ROMAN, BOLD, ITALLICS, etc.  */
{
	register fontBlk *	f;
	register int		i;
	char				fn[70];
	char				fname[10];

	/* See if font is already loaded */

	f = fonts->next;		/* Skip over defont entry */
	while (f != fonts) {
		if (f->ps == ps && f->num == n) {
			return f;
		}
		f = f->next;
	}

	/* Must load the font. */
	if ((f = (fontBlk * )getSpace(sizeof(fontBlk))) == (fontBlk * )NULL) {
		outOfSpace ();
		f = fonts;	/* Use defont if there is no room */
		return f;
	} 

	font2string( n );

#ifdef DMD630
	/*See if font is in font cache. */
	sprintf( fname, "%s.%d" , fontString, ps );

	f->f = fontrequest( fname );
	if( f->f == 0 ) {
		/* Font is not in cache so attempt to load in. */
#endif

		if( fontBells )
			ringbell();
		message("Loading font: ");
		cursswitch(&hourglass);

		i = ps + 1;
		do {		/* Look for a font <= R.i */
			sprintf(fn, "%s/%s.%d", envfont, fontString, --i );
		} while ((i > 0) && (access(fn, 4) != 0));

		if (i == 0) {
			message("Font not found: ");
			message(fn);
			ringbell();
			sleep(60);
			f->f = &defont;
			return f;
		} 
			
		message(fn);

		if ((f->f = getfont(fn)) == (Font * ) NULL) {
			message("Internal error: getfont failed");
			ringbell();
			sleep(60);
			cursSwitch();
			f->f = &defont;
			return f;
		} 

		if( fontBells )
			ringbell();

#ifdef DMD630
		/* Add font to cache. */

		/* "fontcache(...) would be preferable, but it aborts -
		   why?? */
		fontsave( fname, f->f );
	}
#endif

	/* Add font to font list. */
	insertFont( f );
	f->ps  = ps;
	f->num = n;
	f->useCount = 0;
	
	cursSwitch();

	f->useCount++;
	return f;
}
