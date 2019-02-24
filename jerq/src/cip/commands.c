/*
  %Z%  %M%  version %I% %Q%of %H% %T%
  Last Delta:  %G% %U% to %P%
*/

#ifndef	JPIC
#define	JPIC	"/usr/jerq/bin/Jpic "
#endif	JPIC

#include "cip.h"

char jpicCmd[sizeof (JPIC) + MAXNAMESIZE] = JPIC;
#define filename (jpicCmd + sizeof (JPIC) - 1)

int Yoffset = YPIC, Xoffset = Xmin;
struct macro *macroList;
int nextMacroName;
Rectangle macroBB();

extern int currentBrush, copyFlag, gridState, buttonState;
extern int videoState;
extern char c;
extern Rectangle brushes[];
extern Rectangle BBpic;
extern struct thing *readPic();
extern void initMessage ();
extern void putMessage ();
extern Point jString ();
extern void jMoveTo ();

Texture prompt = {
  0x0000, 0x0000, 0x07E0, 0x7FFF, 0xFFFF, 0xFFFF, 0x8AE3, 0xBAEF,
  0x9AE7, 0xBAEF, 0xBA23, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000
};

Texture hourglass = {
  0x1FF0, 0x1FF0, 0x0820, 0x0820, 0x0820, 0x0C60, 0x06C0, 0x0100,
  0x0100, 0x06C0, 0x0C60, 0x0820, 0x0820, 0x0820, 0x1FF0, 0x1FF0
};
 

doPut(h) 
struct thing *h;
{
  register struct thing *t;
  register FILE *fp; 
  register struct macro *m;
  register int saveB;

  saveB = buttonState;
  getFileName();
  if (filename[0]!='\0') {
    if ((fp = fopen(filename,"w")) != (FILE *) NULL) {
      fprintf(fp,".PS\nscale=100\n");
      cursswitch(&hourglass);
      findBBpic(h);
      nextMacroName = 0;
      for (m=macroList; m!=(struct macro *)NULL; m=m->next) {
	if (m->useCount>0) {
	  m->outName = nextMacroName++;
	  fprintf(fp,"define m%d |\n",m->outName);
          fprintf(fp,"[ box invis ht %d wid %d with .sw at 0,0\n",
                     m->bb.corner.y, m->bb.corner.x);
	  if ((t=m->parts) !=TNULL) {
	    do {
	      writePIC(t,fp,Rpt(Pt(0,0),m->bb.corner));
	      t=t->next;
	    } while (t!=m->parts);
	  }
	  fprintf(fp,"] |\n\n");
	}
      }
      fprintf(fp,"box invis ht %d wid %d with .sw at 0,0\n",
                  BBpic.corner.y - BBpic.origin.y,
                  BBpic.corner.x - BBpic.origin.x);
      if ((t = h) != TNULL) {
        do {
          writePIC(t,fp,BBpic);
          t = t->next;
        } while (t!=h);
      }
      fprintf(fp,".PE\n");
      fclose(fp);
      cursSwitch();
    }
    else {
      fileError("open",filename);
    }
  }
  changeButtons(saveB);
}

fileError(s,fn) 
register char *s, *fn;
{	
  beep();
  initMessage ();
  putMessage("cannot ");
  putMessage(s);
  putMessage(" file ");
  putMessage(fn);
  sleep( 120 );
  beep();
}

struct thing *
doGet(h) 
register struct thing *h;
{
  register FILE *fp;

  getFileName();
  if (filename[0]!='\0') {
    if (access(filename,4)!=0) {
      fileError("access",filename);
    }
    else {
      if ((fp = popen(jpicCmd,"r")) == (FILE *) NULL) {
        fileError("open pipe for",filename);
      }
      else {
        cursswitch(&hourglass);
	getChar(fp);
        h = readPic(fp,h);
        pclose(fp);
        cursSwitch();
      }
    }
  }
  changeBrush(-1);
  changeButtons(INITbuttons);
  return(h);
}

getFileName() 
{
  register int i;
  Point p;

  p = MOUSE_XY;
  p.y += 20;
  changeButtons(BLANKbuttons);
  centeredText(p,filename);
  for (i=0; filename[i]!='\0'; i++) {
  }
  getKbdText (filename, p, i, &prompt, MAXNAMESIZE);
}

struct thing *
doClear(h) 
struct thing *h;
{
  deleteAllThings(h);
  cursinhibit();
  xtipple(&display,brushes[PIC]);
  cursallow();
  copyFlag = 0;
  changeBrush(-1); 
  changeButtons(INITbuttons);
  if (gridState==GRIDon) {
    drawGrid();
  }
  return(TNULL);
}

struct thing *
defineMacro(h) 
register struct thing *h;
{
  Point p, q;
  register struct thing *s, *t, *l = { TNULL }; 
  Rectangle r;
  struct thing dummy;
  char *z;		/* Temporary alloc space */

  changeBrush(-1);
  /* Test to see if there is enough memory to create this macro */
  if ((z=getSpace(sizeof(struct macro)+sizeof(struct thing))) == NULL) {
    return (h);
  }
  free (z);
  changeButtons(MACRObuttons);
  for (; !button2(); nap(2)) ;
  p = sub(MOUSE_XY,Pt(Xmin,YPIC));
  q = track(p,Pt(Xmin,YPIC),BOX,h);
  r = canon (p, q);
  h = insert(&dummy,h);
  if ((t = h->next) != TNULL) {
    do {
      if (inside(r,t->bb)) {
        s = remove(t);
        l = insert(t,l);
        t = s;
      }
      else {
	t = t->next;
      }
    } while (t != &dummy);
  }
  h = remove(&dummy);
  if ((t = l) != TNULL) {
    r = macroBB(l);
    do {
      makeRelative(t,r.origin);
      t = t->next;
    } while (t != l);
    p = r.origin;
    r = rsubp (r, r.origin);
    changeButtons(INITbuttons);
    return insert(newMacro(p,recordMacro(l,r,NULL,NULL,NULL)),h);
  }
  else {
    changeButtons(INITbuttons);
    return(h);
  }
}	

Rectangle 
macroBB(l) 
struct thing *l;
{
  Point p, q, p1, p2;
  register struct thing *t;
  Rectangle r;

  p.x = Xmax; p.y=YBOT; q.x=0; q.y=0;
  if ((t=l) != TNULL) {
    do {
      if (t->type != LINE) {
        p1 = t->bb.origin;
        p2 = t->bb.corner;
      }
      else {
        p1.x = min(t->origin.x,t->otherValues.end.x);
        p1.y = min(t->origin.y,t->otherValues.end.y);
        p2.x = max(t->origin.x,t->otherValues.end.x);
        p2.y = max(t->origin.y,t->otherValues.end.y);
      }
      p.x = min(p.x,p1.x);
      p.y = min(p.y,p1.y);
      q.x = max(q.x,p2.x);
      q.y = max(q.y,p2.y);
      t=t->next; 
    } while (t != l);
  }
  r.origin = p; r.corner = q;
  return(r);
}

/* This routine places a new macro athe the end of the macro list. */
/* The head of the list is pointed to by macroList. */

struct macro *
recordMacro(list,r,xro,yro,s) 
struct thing *list;		/* List of things making up macro */
Rectangle r;			/* BB of macro with origin at (0,0) */
struct macro *xro;		/* X-reflection of macro */
struct macro *yro;		/* Y-reflection of macro */
char *s;
{
  register struct macro *m, *l, *n;

  if ((m = (struct macro *) getSpace(sizeof(struct macro)))
       !=(struct macro *)NULL) {
    m->name = s;
    m->bb = r;
    m->useCount = 0;
    m->parts = list;
    m->xReflectionOf = xro;
    m->yReflectionOf = yro;
    m->next = (struct macro *)NULL;
    l = (struct macro *)NULL;
    for (n=macroList; n!=(struct macro *)NULL; n=n->next) {
      l=n;
    }
    if (l == (struct macro *)NULL) {
      macroList = m;
    }
    else {
      l->next = m;
    }
  }
  return(m);
}

int 
inside(r,s) 
Rectangle r,s;
{
  return((r.origin.x <= s.origin.x) && (r.origin.y <= s.origin.y)
    && (r.corner.x >= s.corner.x) && (r.corner.y >= s.corner.y));
}

struct thing *
makeRelative(t,p) 
register struct thing *t;
Point p;
{
  register int i;

  t->origin = sub(t->origin,p);
  switch(t->type) {
    case CIRCLE:
    case ELLIPSE:
    case TEXT: {
      break;
    }
    case LINE: {
      t->otherValues.end = sub(t->otherValues.end,p);	
      break;
    }
    case BOX: {
      t->otherValues.corner = sub(t->otherValues.corner,p);
      break;
    }
    case ARC: {
      t->otherValues.arc.start = sub(t->otherValues.arc.start,p);
      t->otherValues.arc.end = sub(t->otherValues.arc.end,p);
      break;
    }
    case SPLINE: {
      for (i=0; i<=t->otherValues.spline.used; i++)  {
	t->otherValues.spline.plist[i] = 
	      sub(t->otherValues.spline.plist[i],p);
      }
      break;
    }
  }
  BoundingBox(t);
  return(t);
}

int 
backspaceOneWord(s,i) 
register char *s; 
register int i;
{
  s[(i>0)? --i : 0] = '\0';
  for ( ; i>0 && (isdigit(s[i-1]) || isletter(s[i-1])); ) {
    s[--i]='\0';
  }
  return(i);
}

Rectangle
canon (p1, p2)
Point p1, p2;
{
  Rectangle r;

  r.origin.x = min (p1.x, p2.x);
  r.origin.y = min (p1.y, p2.y);
  r.corner.x = max (p1.x, p2.x);
  r.corner.y = max (p1.y, p2.y);
  return (r);
}

/* This routine places a stippled texture into the rectangle */
/* specified by r in the layer specified by P->layer.  No stippling */
/* is used if the user owns the mouse.  Instead a blank */
/* stipple pattern is used.  Note, that when STORE mode is used, */
/* anything that was in the rectangle is wiped out.  */
/* Reverse textures are used if reverse video is set. */

xtipple (b, r)
Bitmap *b;
Rectangle r;
{
  static Texture plain = { 0 };
  static Texture fancy = { 0x1000 };
  static Texture rvplain = { 
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
  };
  static Texture rvfancy = {
    0xEFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
  };
  Texture *tp;

  if (videoState == BLACKfield) {
    tp = own ()&MOUSE? &rvplain: &rvfancy;
  }
  else {
    tp = own ()&MOUSE? &plain: &fancy;
  }
  texture (P->layer, r, tp, F_STORE);
}

cursSwitch ()
{
  if (own() & MOUSE) {
    cursswitch(ptinrect(MOUSE_XY,brushes[PIC])
			? &crossHairs : (Texture *)0);
  }
}

getKbdText (s, p, size, cursorTexture, maximum)
char *s;
Point p;
int size;		/* Size of text in s */
Texture *cursorTexture;
int maximum;		/* Maximumimum size of s */
{
  int i;

  cursswitch (cursorTexture);
  clearKeyboard();
  wait(KBD);
  for(i=size; (c=kbdchar())!='\r'; wait(KBD)) {
    if (i!=0) {
      centeredText(p,s);
    }
    if (c >= 0x82 && c <= 0x89) {
      int j,k;
      j = c - 0x82;
      k = 0;
      while ((c = BRAM->pfkeys[j][k].byte) && (++k <= PFKEYSIZE)) {
	if (i >= maximum) {
	  ringbell ();
	  s[i] = '\0';
	  break;
	}
	else {
	  s[i++] = c;
	}
      }
    }
    else {
      switch (c) {
	case '@':
	case 'U'-'@': {
	  i=0;
	  s[0] = '\0';
	  break;
	}
	case 'W'-'@': {
	  i = backspaceOneWord(s,i);
	  break;
	}
	case '\b': {
	  s[(i>0)? --i : 0] = '\0';
	  break;
	}
	default: {
	  if (i >= maximum) {
	    ringbell ();
	  }
	  else {
	    s[i++] = c;
	    s[i] = '\0';
	  }
	  break;
	}
      }
    }
    centeredText(p,s);
  }
  if (i != 0) {
    centeredText(p,s);
  }
  cursSwitch ();
}
