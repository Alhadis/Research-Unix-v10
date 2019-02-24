/*
  %Z%  %M%  version %I% %Q%of %H% %T%
  Last Delta:  %G% %U% to %P%
*/

#include "cip.h"
#define CEOF 0xff
char c;

char *getText();
Point getPoint();
struct macro *findMacro();

extern struct macro *macroList;
extern Texture hourglass;
extern int Xoffset;
extern int Yoffset;

struct thing *
readPic(fp,h) 
register FILE *fp; 
register struct thing *h;
{
  register struct thing *t, *l;
  struct macro *m;
  Rectangle r;
  register int num, i, a , b;
  Point *plist, p, q;
  register char *s, type;
  
  while ( c != CEOF) {
    t = TNULL;
    type = c;
    getChar(fp);
    switch(type) {
      case CEOF:
      case ' ':
      case '\n':
      case '\t': {
	break;
      }
      case 'm': { 	/* moveto */
	p = getPoint(fp);
	break;
      }
      case 'e':	{	/* ellipse */
	p = getPoint(fp);
	if ((t = newEllipse(p)) == TNULL) {
	  c = CEOF;		/* Terminate */
	}
	else {
	  t->otherValues.ellipse.wid = (getInt(fp))<<1;
	  t->otherValues.ellipse.ht = (getInt(fp))<<1;
	}
	break;
      }
      case 'c':	{	/* circle */
	p = getPoint(fp);
	if ((t = newCircle(p)) == TNULL) {
	  c = CEOF;
	}
	else {
	  t->otherValues.radius = getInt(fp);
	}
	break;
      }
      case 'a': { 	/* arc */
	p = getPoint(fp);
	q = getPoint(fp);
	if ((t = newArc(q,getPoint(fp))) == TNULL) {
	  c = CEOF;		/* Terminate */
	}
	else {
	  t->origin = p;
	}
	break;
      }
      case 'l':	{	/* line */
	a=0;
	skipWhiteSpace(fp);
	if (c=='<') {
	  a += startARROW;	
	  getChar(fp);
	}
	if (c=='>') {
	  a += endARROW;	
	  getChar(fp);
	}
	if (c=='.' || c=='-') {
	  b = (c=='.')? DOTTED : DASHED;
	  getChar(fp);
	  getInt(fp); /*size*/
	}
	else {
	  b = SOLID;
	}
	num =  getInt(fp);
	q = getPoint(fp);
	for (i=1; i<num; i++) {
	  p = getPoint(fp);
	  if ((t = newLine(q,p)) == TNULL) {
	    c = CEOF;		/* Terminate */
	    break;
	  }
	  t->boorder = b;
	  t->arrow = a;
	  BoundingBox(t);
	  draw(t,Pt(Xmin,YPIC));
	  h = insert(t,h);
	  q = p;
	}
	break;
      }
      case 'b':	{	/* box */
	if (c=='.' || c=='-') {
	  a = (c=='.')? DOTTED : DASHED;
	  getChar(fp);
	  getInt(fp); /*size*/
	}
	else {
	  a = SOLID;
	}
	p = getPoint(fp);
	q = getPoint(fp);
	if ((t = newBox(canon (p, q))) == TNULL) {
	  c = CEOF;		/* terminate */
	}
	else {
	  t->boorder = a;
	}
	break;
      }
      case 't': {  	/* text */
	if (c!='c') {
	  p = getPoint(fp);
	  skipWhiteSpace(fp);
	  type = c;
	  getChar(fp);
	  s = getText(fp,'\n');
	  if ((t = newText(p,s)) == TNULL) {
	    c = CEOF;		/* Terminate */
	  }
	  else {
	    extractFontandPointSize(t);
	    t->origin.y -= (t->otherValues.text.f->f->height>>1);
	    BoundingBox(t);
	    switch(type) {
	      case 'C':
	      case 'c': {
		t->otherValues.text.just = CENTER;
		break;
	      }
	      case 'L':
	      case 'l': {
		t->otherValues.text.just = LEFTJUST;
		break;
	      }
	      case 'R':
	      case 'r': {
		t->otherValues.text.just = RIGHTJUST;
		break;
	      }
	    }
	  }
	}
	break;
      }
      case '~': {
	a = 0;
	skipWhiteSpace(fp);
	if (c=='<') {
	  a += startARROW;	
	  getChar(fp);
	}
	if (c=='>') {
	  a += endARROW;	
	  getChar(fp);
	}
	num = getInt(fp);
	plist= (Point *) getSpace((num+3)*sizeof(Point));
	if (plist==(Point *) NULL) {
	  c = CEOF;		/* Terminate */
	  break;
	}
	for (i=1; i<=num; i++) {
	  plist[i]=getPoint(fp);
	}
	plist[num+1]=plist[num];
	if ((t = newSpline(num+1,num,plist)) == TNULL) {
	  c = CEOF;
	}
	else {
	  t->arrow = a;
	}
	break;
      }
      case '.': {
	switch (c) {
	  case 'P': {
	    cursswitch ((Word *)NULL);
	    getBox (fp,h);	/* Get x and y offsets */
	    cursswitch (&hourglass);
	    break;
	  }
	  case 'U':
	  case 'u': {
	    /* start of macro */
	    getChar(fp);
	    skipWhiteSpace(fp);
	    s = getText(fp,' ');
	    flushLine(fp);
	    l = readPic(fp,TNULL);
	    r = macroBB(l);
	    if ((m=findMacro(s))==(struct macro *)NULL) {
	      if ((t=l)!=TNULL) {
		do {
		  makeRelative(t,r.origin);
		  t = t->next;
		} while (t != l);
	      }
	      m=recordMacro(l,rsubp(r,r.origin),NULL,NULL,s);
	    }
	    t = newMacro(r.origin,m);
	    break;
	  }
	  case 'E':
	  case 'e': {
	    /* end of macro */
	    flushLine(fp);
	    return(h);
	    break;
	  }
	  default: {
	    flushLine(fp);
	    break;
	  }
	}
	break;
      }
      default: {
	flushLine(fp);
	break;
      }
    }
    if ((t != TNULL) && (t->type != LINE)) {
      BoundingBox(t);
      if (t->type != MACRO) {
	draw(t,Pt(Xmin,YPIC));
      }
      h = insert(t,h);
    }
  }
  return(h);
}

getBox (fp, h)
FILE *fp;
struct thing *h;
{
  int height, width;
  Rectangle r;
  Point offset;

  getChar(fp);
  getChar(fp);
  height = getInt (fp);
  width = getInt (fp);
  Yoffset = (YPicSize-height)>>1;
  Xoffset = (XPicSize-width)>>1;
  r.origin.x = Xoffset;
  r.origin.y = Yoffset;
  r.corner.x = Xoffset + width;
  r.corner.y = Yoffset + height;
  offset.x = Xmin;
  offset.y = YPIC;

  cursset (raddp(r,offset));
  xbox (raddp(r,offset));	/* Show the box on the screen */

  changeButtons (READbuttons);
  while (!button12 () || button3()) {
    wait (MOUSE);
  }
  if (button2 ()) {
    r = moveBox (r.origin, r, h, offset);
    Xoffset = r.origin.x;
    Yoffset = r.origin.y;
  }
  xbox (raddp(r,offset));	/* Remove the box */
  changeButtons (BLANKbuttons);
}

Point 
getPoint(f) 
FILE *f;
{
  Point p;

  p.x = getInt(f) + Xoffset;
  p.y = getInt(f) + Yoffset;
  return(p);
}

int 
getInt(f) 
FILE *f;
{
  register int i;

  skipWhiteSpace(f);
  i=0;
  while (c >= '0' && c<='9' ) {
    i = 10 * i + c - '0';
    getChar(f);
  } 
  return(i);
}

getChar(f) 
FILE *f;
{	
  c = getc(f);
}

skipWhiteSpace(f) 
FILE *f;
{
  while( c==' ' || c=='\t' ) getChar(f);
}

char *
getText(f,term) 
FILE *f; 
register char term;
{
  register char s[MAXTEXT]; 
  register char *ss, *t, *tt; 
  register int i;

  for (i=0; c != term; getChar(f)) {
    if (i < MAXTEXT) {
      s[i++]=c;
    }
  }
  s[i]=0;
  if ((t = (char *) getSpace(strlen(s)))==NULL) {
    return( 0 );	
  }
  for (ss = s, tt = t; *tt++ = *ss++;) {
  }
  getChar(f);
  return(t);
}

flushLine(f) 
FILE *f;
{
  while (c != '\n') {
    getChar(f);
  }
  getChar(f);
}

struct macro *
findMacro(s) 
register char *s;
{
  struct macro *m;

  for (m=macroList; ((m!=(struct macro *)NULL) &&
       (compare(s,m->name)==0)); m=m->next) ;
  return(m);
}

int 
compare(s1,s2) 
register char *s1, *s2;
{
  while ( *s1!='\0' && *s2!='\0' && *s1==*s2) {
    s1++; s2++;
  }
  return( (*s1=='\0' && *s2=='\0')?1:0 );
}

extractFontandPointSize(t) 
register struct thing *t;
{
  register short ps;
  register int f, i, j, len; 
  register char *s;

  s = t->otherValues.text.s;
  len = strlen(s);
  if (compare(&s[len-6],"\\f1\\s0") && s[0]=='\\' && s[1]=='f') {
    switch(s[2]) {
      case '1':
      case 'R': {
	f = ROMAN;
	break;	
      }
      case '2':
      case 'I': {
	f = ITALIC;
	break;	
      }
      case '3':
      case 'B': {
	f = BOLD;
	break;	
      }
      default: {
	f = ROMAN;
	break;
      }
    }
    ps = s[5] - '0';
    if (isdigit(s[6])) {
      ps = ps*10 + s[6] - '0';
      i = 7;		/* Skip over \f1\snn */
    }
    else {
      i = 6;		/* Skip over \f1\sn */
    }
    if (s[i] == '\\' && s[i+1] == '&') {
      i += 2;		/* Skip over \& */
    }
    for (j=0; i<len-6; ) {
      s[j++] = s[i++];
    }
    s[j] = '\0';
    t->otherValues.text.f = findFont(ps,f);
  }
}		
