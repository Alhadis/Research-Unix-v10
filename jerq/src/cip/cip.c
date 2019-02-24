/*
  %Z%  %M%  version %I% %Q%of %H% %T%
  Last Delta:  %G% %U% to %P%
*/

#include "cip.h"

#define Xbut1 (Xbut+(Xmax-Xbut)/6)
#define Xbut2 ((Xmax+Xbut)>>1)
#define Xbut3 (Xmax-(Xmax-Xbut)/6)
#define Ybut123 (Ybut+((ButSize*3)>>2))
#define _GETENV		9

Point jchar();

int gridState = GRIDoff;
int videoState = WHITEfield;
Rectangle brushes[NUMBR+2];
int currentBrush = {-1};

int copyFlag = 0;
int thingSelected = 0;
int editDepth = 0;
int buttonState = 0;
fontBlk *fonts = {(fontBlk *)NULL};

Texture crossHairs = {
	 0x0100, 0x0100, 0x0100, 0x0100,
	 0x0100, 0x0100, 0x0000, 0xFC7E,
	 0x0000, 0x0100, 0x0100, 0x0100,
	 0x0100, 0x0100, 0x0100, 0x0000,
};

char *but1text[numButtonStates] = 
  {"select","select","select","","","","select","select","no","center"};
char *but2text[numButtonStates] = 
  {"","draw","edit","draw","","draw box","copy","move","no","move box"};
char *but3text[numButtonStates] = 
  {"menu","menu","menu","end spline","","","menu","menu","yes",""};
extern insertFont ();
extern void message ();
extern struct macro *macroList;
extern Word *saveBase;
extern Rectangle saveScreenmap;
extern Texture hourglass;
short noSpace;		/* Indicates out of initial amount of memory */
char envfont[]="/usr/jerq/font";
char *dgetenv();
char *strcat();

Texture grid = {0x8080,0,0,0,0,0,0,0,0x8080,0,0,0,0,0,0,0};

main()
{	
  register struct thing *currentThing =  TNULL ;

  request(KBD | MOUSE | RCV | SEND);

  initFontList ();
  noSpace = 0;
  P->state |= RESHAPED;		/* Indicate frames not drawn yet. */
  brushInit();	/* Init outline of brush boxes. */
  for (;;) {
    currentThing = doMouseButtons(currentThing,Pt(Xmin,YPIC));
  }
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
    brushes[i].corner.y = YBR;
  }
  brushes[PIC].origin.x = Xmin+LW;
  brushes[PIC].origin.y = YPIC+LW;
  brushes[PIC].corner.x = Xmax-LW;
  brushes[PIC].corner.y = YBOT-LW;
  brushes[ED].origin.x = XeditD;
  brushes[ED].origin.y = Ybut;
  brushes[ED].corner.x = Xbut - LW;
  brushes[ED].corner.y = Ymax;
}

drawFrame()
{
  register int i;

  saveBase = P->layer->base;
  saveScreenmap = P->layer->rect;

  for (i=0; i<NUMBR; i++) {
    rectf(&display, brushes[i],F_XOR);
    rectf(&display, inset(brushes[i],LW),F_XOR);
    drawBrush(i);
  }
  rectf(&display, brushes[PIC],F_XOR);
  rectf(&display, inset(brushes[PIC],-LW),F_XOR);
  /* Init message frame */
  rectf (&display, Rect(Xmin, Ybut, XeditD-LW, Ymax), F_XOR);
  rectf (&display, inset(Rect (Xmin, Ybut, XeditD-LW, Ymax),LW), F_XOR);
  Buttons();
}	

drawBrush(i) 
register int i;
{
  register int r;  
  Point m; 
  Point p[6];

  r = (YBR-Ymin)*3>>3;
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
Select(t, m, offset)
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

  w = (jstrwidth(s)+8)>>1;
  if (w<=4) {
    w=butHt;
  }
  centeredText(add(p,Pt(0,(butHt>>1)-10)),s);
  box(Rpt(sub(p,Pt(w,butHt)),add(p,Pt(w,butHt))));
}

Point PtCurrent;
void
jMoveTo (xy)
Point xy;
{
  PtCurrent = xy;
}

Point
jString(s)
register char *s;
{
  return PtCurrent=string(&defont,s,&display,PtCurrent,F_XOR);
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

drawGrid()
{	
  cursinhibit();
  texture(P->layer,inset (Rect(Xmin,YPIC,Xmax,YBOT), LW),&grid,F_XOR);
  cursallow();
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
    flash(&brushes[currentBrush],Pt(0,0));
  }
  if (new>-1) {
    flash(&brushes[new],Pt(0,0));
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
    labeledButton(Pt(Xbut1,Ybut123),but1text[i]);
    labeledButton(Pt(Xbut2,Ybut123),but2text[i]);
    labeledButton(Pt(Xbut3,Ybut123),but3text[i]);
}

beep()
{
  ringbell();
}



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
