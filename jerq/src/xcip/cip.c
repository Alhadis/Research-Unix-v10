#include "cip.h"

#ifdef DMD630
#include <object.h>

#endif

#ifndef MPX
Bitmap cdisplay;
Proc proc;
Proc *P;
#endif

#define Xbut1 ((Xbut+(Xmax-Xbut)/6)+4)
#define Xbut2 (((Xmax+Xbut)>>1)-4)
#define Xbut3 ((Xmax-(Xmax-Xbut)/6)-8)
#define Ybut123 (Ybut+((ButSize*3)>>2))
#define _GETENV		9

Point jchar();

short Xmin;		/* Left edge of frame. */
short Ymin;		/* Top edge of frame.  */

Point drawOffset;	/* Offset into drawing frame. */

short Xoffset;	
short Yoffset;

int videoState = WHITEfield;
Rectangle brushes[NUMBR+2];
int currentBrush = {-1};

int copyFlag = 0;
int thingSelected = 0;
int editDepth = 0;
int buttonState = 0;
short fontBells = 1;
fontBlk *fonts = {(fontBlk *)NULL};
fontBlk *defFont;

int left_hand;

int gridState = GRIDoff;

short currAlignment = 8;
short currPS =   10;
short currFont = ROMAN;
short currJust = CENTER;
short currLineType = SOLID;
short currBoxType = SOLID;
short currLineArrows = 0;
short currSplineArrows = 0;
short currSpacing = 0;

Texture16 crossHairs = {
	 0x0100, 0x0100, 0x0100, 0x0100,
	 0x0100, 0x0100, 0x0000, 0xFC7E,
	 0x0000, 0x0100, 0x0100, 0x0100,
	 0x0100, 0x0100, 0x0100, 0x0000,
};

char *but1text[numButtonStates] = {
	"select",	"",		"select",	"select",
	"", 		"",		"abort",	"select",
	"select",	"no",		 "center",	"OK",
	"select"
};
char *but2text[numButtonStates] = {
	"",		"",		"draw",		"edit",
	"draw",		"",		"draw box",	"copy",	
	"move",		"no",		"move box",	"OK",
	"draw"
};
char *but3text[numButtonStates] = {
	"main menu",	"menu",		"",		"menu",	
	"end spline",	"",		"",		"menu",
	"menu",		"yes",		"",		"OK",
	"glbl menu"
};

extern insertFont ();
extern void message ();
extern Word *saveBase;
extern Rectangle saveScreenmap;
extern Texture16 hourglass;
short noSpace;		/* Indicates out of initial amount of memory */
char *envfont;
char *strcat();

#ifndef MPX
  static char EnvFont[80];
  char *dgetenv();
#endif


main(argc,argv)
  int argc;
  char *argv[];
{	
  register struct thing *currentThing =  TNULL ;

#ifndef DMD630
  jinit();
#endif

  request(KBD | MOUSE | RCV | SEND);

#ifdef MPX

  /* Get the font directory from param list. */
  envfont = argv[1];		

#else

  /* Param list does not work in stand-alone, so must query for the DMD
   * value and form the font directory from it. 
   */
  sleep(120);	/* Must sleep first, on MAXIs especially! */

  if ( NULL == ( dgetenv( "DMD", EnvFont ) ) ) {
	ringbell();
	message("Could not find environment variable DMD");
	sleep(200);
	exit();
  }
  strcat( EnvFont, "/font");
  envfont = EnvFont;

#endif

#ifdef DMD630

  /* Save cip in cache if requested. */
  if( (argc >= 2) && (strcmp( argv[2], "-c" ) == 0) )
	cache( (char *)0, A_NO_SHOW | A_BSS | A_DATA );
  
#endif

  noSpace = 0;

#ifdef MPX
  P->state |= RESHAPED;		/* Indicate frames not drawn yet. */
#else	/* Stand-alone */
  cursswitch( &crossHairs );
  /* Create dummy layer process pointer. */
  cdisplay = display;
  P = &proc;
  P->layer = &cdisplay;
#endif

  initFontList ();

#ifndef MPX
  Xmin = LEFTMOST; Ymin = TOPMOST;
  drawOffset.x = Xmin;
  drawOffset.y = YPIC;
  brushInit();	/* Init outline of brush boxes. */
  redrawLayer(currentThing,Pt(0,0));
#endif

  for (;;) {
    currentThing = doMouseButtons(currentThing,Pt(0,0));
  }
}


#ifdef MPX

Rectangle origRect = {8,8,200,100};	/* Holds original layer size,
					   initialize to upper left.  */
originalLayer()
{	
#ifdef DMD630
	reshape( origRect );
#else
	register struct Layer * l;
	register struct Proc *  p = P;

	dellayer (p->layer);		/* Delete the current layer */
	p->state &= ~MOVED;
	p->state |= RESHAPED;
	l = newlayer(origRect);		/* and create a new one. */
	if (l == 0) {		/* If not enough room, squish the */
				/* layer to tell user no room. */
		origRect.corner = add (origRect.origin, Pt (100, 50));
		l = newlayer (origRect);
		if (l == 0) {
			exit ();	/* Too bad!! */
		}
	}
	/* Set up new process table. */
	p->layer = l;
	p->rect = inset (origRect, 2);
	setdata (p);
	mpxnewwind (p, C_RESHAPE);
#endif
	saveBase = P->layer->base;
	saveScreenmap = P->layer->rect;
}

#else

originalLayer()
{
	message("Original window not available in stand-alone mode");
}

#endif


#ifdef MPX

resizeLayer( h, offset )
  register struct thing *h;
  Point offset;
{
  Rectangle r;
  register struct Layer *l;
  register struct Proc *p =P;
  long	temp;

  origRect = p->layer->rect;
  
  /* Position opened window based on position of close window. */
  if( origRect.origin.x < 10 ) {
	Xmin = LEFTMOST;
  } else if( origRect.corner.x > (RIGHTX - 10) ) {
	Xmin = RIGHTMOST;
  } else {
	/* Take an average postion */
  	temp = (origRect.origin.x + origRect.corner.x) >> 1;
  	Xmin = ( (temp * (RIGHTMOST-LEFTMOST) ) / RIGHTX ) + LEFTMOST;
  }

  if( origRect.origin.y < 10 ) {
	Ymin = TOPMOST;
  } else if( origRect.corner.y > (BOTTOMY - 10) ) {
	Ymin = BOTTOMMOST;
  } else {
	/* Take an average postion */
  	temp = (origRect.origin.y + origRect.corner.y) >> 1;
  	Ymin = ( (temp * (BOTTOMMOST-TOPMOST) ) / BOTTOMY ) + TOPMOST;
  }

  r.origin.x = Xmin;
  r.origin.y = Ymin;
  r.corner.x = Xmax;
  r.corner.y = Ymax;
  r = inset(r,-BORDER);	/* Make the window bigger */

  drawOffset.x = Xmin;
  drawOffset.y = YPIC;

  brushInit();	/* Init outline of brush boxes. */
  
#ifdef DMD630
  reshape( r );
#else
  dellayer (p->layer);	/* Delete the current layer */
  p->state &= ~MOVED;
  p->state |= RESHAPED;
  l = newlayer (r);		/* and create a new one. */
  if (l == 0) {		/* If not enough room, squish the */
			/* the layer to tell user no room. */
    r.origin = origRect.origin;
    r.corner = add (origRect.origin, Pt (100,50));
    l = newlayer (r);
    if (l == 0) {
      exit ();	/* No more space at all */
    }
  }
  /* Set up new process table. */
  p->layer = l;
  p->rect = inset (r, 2);
  setdata (p);
  mpxnewwind (p, C_RESHAPE);
#endif
  if (CORRECTSIZE) {
    redrawLayer(h,offset);
  }
}
#endif


redrawLayer(h,offset)
  struct thing *h;
  Point offset;
{
  cursinhibit();
  drawFrame(); 	/* display drawing area and brushes */
  currentBrush = -1;
  buttonState = INITbuttons;
  thingSelected = 0;
  copyFlag = 0;

  doRedraw( h, add(drawOffset,offset) );

  spaceRemaining (1);

  if (videoState == BLACKfield) {
    rectf(&display, Jrect,F_XOR);
  }
  if (editDepth) {
    drawEDbutton (editDepth);
  }
  cursallow();
#ifdef MPX
  P->state &= ~RESHAPED;
#endif
  putVersion();
}


brushInit()
{
  register int x; 
  register int i;

  x=Xmin;
  for (i=0; i<NUMBR; i++) {
    brushes[i].origin.x = x;
    x += DXBR;
    brushes[i].corner.x = x-LW;
    brushes[i].origin.y = Ymin;
    brushes[i].corner.y = Ymin+YBR;
  }
  brushes[PIC].origin.x = Xmin+LW;
  brushes[PIC].origin.y = YPIC+LW;
  brushes[PIC].corner.x = Xmax-LW;
  brushes[PIC].corner.y = YBOT-LW;
  brushes[ED].origin.x  = XeditD;
  brushes[ED].origin.y  = Ybut;
  brushes[ED].corner.x  = Xbut-LW;
  brushes[ED].corner.y  = Ymax;
}


drawFrame()
{
  register int i;

  saveBase      = P->layer->base;
  saveScreenmap = P->layer->rect;

  for (i=0; i<NUMBR; i++) {
    rectf(&display, brushes[i],F_XOR);
    rectf(&display, inset(brushes[i],LW),F_XOR);
    drawBrush(i);
  }

  rectf(&display, brushes[PIC],F_XOR);
  rectf(&display, inset(brushes[PIC],-LW),F_XOR);

  /* Init message frame */
  rectf(&display, Rect(Xmin, Ybut, XeditD-LW, Ymax), F_XOR);
  rectf(&display, inset(Rect (Xmin, Ybut, XeditD-LW, Ymax),LW), F_XOR);
  Buttons();
}	

drawBrush(i) 
register int i;
{
  register int r;  
  Point m; 
  Point p[6];

  r = (YBR*3)>>3;
  m = div( add( brushes[i].origin, brushes[i].corner), 2);
  switch (i) {
    case CIRCLE: {
      circle(&display, m,r,F_XOR);
      break;
    }
    case BOX: {
      box(Rect(m.x-r,m.y-r,m.x+r,m.y+r)); 
      break;
    }
    case ELLIPSE: {
      Ellipse(m,(r<<1),r*3);
      break;
    }
    case LINE: {
      segment(&display, Pt(m.x-r,m.y),Pt(m.x+r,m.y),F_XOR);
      break;
    }
    case ARC: {
      arc(&display, Pt(m.x,m.y+r),Pt(m.x+r,m.y),Pt(m.x-r,m.y),F_XOR);
      break;
    }
    case SPLINE: {
      p[1] = sub(m,Pt(r,r));
      p[2] = add(m,Pt(r>>1,-r));
      p[3] = sub(m,Pt(r>>1,-r));
      p[4] = add(m,Pt(r,r));
      spline(Pt(0,0),p,5);
      break;
    }
    case TEXT: {
      centeredText(sub(m,Pt(0,10)),"Text");
      break;
    }
  }
}


  Rectangle *
select(t, m, offset)
  struct thing *t;
  Point m;
  Point offset;
{	
  register int i;

  for (i=0; ((i<NUMBR+2)&&(!ptinrect(m,brushes[i]))); i++) ;
  if (i==PIC) {
    return(&brushes[PIC]);  /* temporary */
  }
  else if (i==ED) {
    if (editDepth>0) {
      drawEDbutton(editDepth);	/* Undraw edit button */
      if ((--editDepth)!=0) {
	drawEDbutton(editDepth);
      }
    }
    changeBrush(-1);	
    if (thingSelected) {
      drawSelectionLines (t, offset);
      thingSelected = 0;
    }
    changeButtons(INITbuttons);
    return( (Rectangle *) NULL);
  }
  else if (i==NUMBR+2) {
    changeBrush(-1);	
    changeButtons(INITbuttons);
    return( (Rectangle *) NULL);
  }
  else {
    changeBrush(i);
    return(&brushes[i]);
  }
}
    
Buttons()
{	
  /* Determine if mouse if left of right handed. */
#ifdef DMD630
  left_hand = setupval( S_PREF, 6 );
#else
  left_hand = 0;
#endif

  rectf(&display, Rect(Xbut,Ybut,Xmax,Ymax),F_XOR);
  rectf(&display, inset(Rect(Xbut,Ybut,Xmax,Ymax),LW),F_XOR);
  centeredText(Pt(Xbut2,Ytext), "Mouse Buttons");
  writeButtonLabels(INITbuttons);
  buttonState = INITbuttons;
}
  
labeledButton(p,s) 
  Point p; 
  register char *s;
{
  register int w;
  register int bh;

  bh = butHt;

  w = (jstrwidth(s)+8)>>1;
  if (w<=4) {
    w=bh;
  }
  centeredText(add(p,Pt(0,(bh>>1)-10)),s);
  box(Rpt(sub(p,Pt(w,bh)),add(p,Pt(w,bh))));
}

extern Point PointCurr;

void
jMoveTo (xy)
Point xy;
{
  PointCurr = xy;
}

Point
jString(s)
register char *s;
{
  return PointCurr=string(&defont,s,&display,PointCurr,F_XOR);
}

Point 
jchar(c) 
register char c;
{
  char s[2];

  s[0]=c; s[1] = '\0';
  return(jString(s));
}
  
drawSelectionLines(t,p) 
register struct thing *t; 
Point p; 
{
  Point p1;

  if (t!=TNULL) {
    if (t->type==SPLINE) {
      drawZigZag(p,t->otherValues.spline.plist,
        t->otherValues.spline.used);
    }
    else if (t->type==ARC) {
      p1 = add(t->origin,p);
      xsegment(p1,add(p,t->otherValues.arc.start));
      xsegment(p1,add(p,t->otherValues.arc.end));
    }	
  }
}


Texture grid;
Texture T_white;

Bitmap gridBm = { (Word *)&grid, 1, 0, 0, 16, 16, 0 };

drawGrid()
{	
	int		xpos;
	int		ypos;
	int		xpos8;
	int		ypos8;
#ifndef DMD630
	register int	i;
	Texture16	grid16;
#endif

	if( currAlignment > 2 ) {
		grid = T_white;
		xpos = (Xmin) & 15;	/* mod 16 */
		ypos = (YPIC) & 15;

		cursinhibit();

		point( &gridBm, Pt(xpos,ypos), F_STORE );

		if( currAlignment == 8 ) {
			xpos8 = (Xmin+8) & 15;	/* mod 16 */
			ypos8 = (YPIC+8) & 15;
			point( &gridBm, Pt(xpos,ypos8), F_STORE );
			point( &gridBm, Pt(xpos8,ypos), F_STORE );
			point( &gridBm, Pt(xpos8,ypos8), F_STORE );
		}

#ifdef DMD630
		texture16( P->layer, inset(Rect(Xmin,YPIC,Xmax,YBOT), LW ), 
			 &grid, F_XOR );
#else
		for( i=0; i<16; i++ ) {
			grid16.bits[i] = (short) (grid.bits[i] >> 16);
		}
		texture16( P->layer, inset(Rect(Xmin,YPIC,Xmax,YBOT), LW ), 
			 &grid16, F_XOR );
#endif

		cursallow();
	}
}


drawEDbutton(depth) 
  int depth;
{	
  char s[5]; 
  Point p;
  register int dy;

  rectf (&display, brushes[ED], F_XOR);
  rectf (&display, inset(brushes[ED],LW),F_XOR);
  p.x = (brushes[ED].corner.x + brushes[ED].origin.x)>>1;
  dy = (brushes[ED].corner.y - brushes[ED].origin.y)/3;
  p.y = brushes[ED].origin.y + dy - 10;
  centeredText(p,"edit depth");
  p.y = brushes[ED].origin.y + (dy<<1) - 10;
  sprintf(s,"%d",depth);
  centeredText(p,s);
}


changeBrush(new) 
  register int new;
{
  if (currentBrush>-1) {
    /* Note: address of function result!  appears to work. */
    flash( &inset(brushes[currentBrush],1) , Pt(0,0) );
  }
  if (new>-1) {
    flash( &inset(brushes[new],1) , Pt(0,0) );
  }
  currentBrush = new;
}


changeButtons(new) 
  register int new;
{	
  if (buttonState != new) { 
    writeButtonLabels(buttonState);
    writeButtonLabels(new);
    buttonState = new;
  }
}
    

writeButtonLabels(i) 
  register int i;
{
	if( left_hand ) {
		labeledButton(Pt(Xbut3,Ybut123),but1text[i]);
   		labeledButton(Pt(Xbut2,Ybut123),but2text[i]);
   		labeledButton(Pt(Xbut1,Ybut123),but3text[i]);
	} else {
		labeledButton(Pt(Xbut1,Ybut123),but1text[i]);
   		labeledButton(Pt(Xbut2,Ybut123),but2text[i]);
   		labeledButton(Pt(Xbut3,Ybut123),but3text[i]);
	}
}


#ifndef MPX
  char *
dgetenv(s , q)
  register char *s;
  register char *q;
{
  sleep(240);
  tag( _GETENV );
  jputstr( s );
  jflush();
  jgetstr(q);
  return(q);
}
#endif


initFontList ()
{
  if ((fonts=(fontBlk *)getSpace(sizeof(fontBlk))) == (fontBlk *) NULL) {
    message ("No memory in terminal");
    sleep (100);
    exit ();
  }
  fonts->f = &defont;
  fonts->ps = 10;
  fonts->num = DEFONT;
  fonts->useCount = 0;
  fonts->next = fonts;
  fonts->last = fonts;
}

putVersion()
{
	message("cip version 3.0");
	drawCopyright( Pt(Xtext+170,Ytext+3) );
}


sameSizeRect( rectA, rectB )
	Rectangle	rectA;
	Rectangle	rectB;
{
	return eqpt( sub(rectA.corner,rectA.origin) ,
		     sub(rectB.corner,rectB.origin) );
}
