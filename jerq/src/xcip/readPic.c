#include "cip.h"

/* Temporary fix for Jpic problem of making numbers one less.  For now,
   round numbers up to next even number. 
 */
#define JpicFix(n)	( ( ((n)+1) >>1 ) << 1 )

#ifdef DMD630
#define CEOF (-1)
#else
#define CEOF 0xff
#endif

char c;

char *getText();
Point getPoint();
struct macro *findMacro();
extern Point align();

extern struct macro *macroList;
extern Texture16 hourglass;

char tempStr[MAXTEXT];


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
	    Font *f;
	    extractFontandPointSize(t);
	    f = t->otherValues.text.f->f;
	    t->origin.y -= f->height >> 1;
	    /* Adjust text up a little - needed when printed - why? */
	    t->origin.y -= JpicFix(f->height >> 3);
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
	num = getTempInt(fp);
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

	    int j;
	    Point a;
	    char *ns;
	    short yDiff;
	    short height;

	    j = 0;

	    getChar(fp);
	    skipWhiteSpace(fp);
	    s = getText(fp,' ');
	    flushLine(fp);
	    l = readPic(fp,TNULL);	/* See Note 1 */

	    if( s[0] == 't') {

	      /* This a TEXT string in macro form */

	      if(l==TNULL) {
		t = TNULL;
	        free (s);
		break;
	      }
	      r.origin.x = Xmax;	r.origin.y = YBOT;
	      r.corner.x = 0;   	r.corner.y = 0;
	      if((t=l->last) != TNULL){
		do {
	          draw(t,drawOffset); /* remove individual text lines */

		  /* Find minimal non-aligned bounding box. */
		  r.origin.x = min(r.origin.x, t->bb.origin.x);
		  r.origin.y = min(r.origin.y, t->bb.origin.y);
		  r.corner.x = max(r.corner.x, t->bb.corner.x);
		  r.corner.y = max(r.corner.y, t->bb.corner.y);

		  for( i = 0; t->otherValues.text.s[i] != '\0'; i++) 
		    tempStr[j++] = t->otherValues.text.s[i];
		  tempStr[j++] = '\r';

		  t = t->last;
                } while(t!=l->last);
		tempStr[j-1] = '\0';
		switch (l->otherValues.text.just) {
		case CENTER:
		  a.x = (r.origin.x + r.corner.x) >> 1; 
		  break;
		case LEFTJUST:
		  a.x = r.origin.x;
		  break;
		case RIGHTJUST:
		  a.x = r.corner.x;
		  break;
		}

		yDiff = l->origin.y - l->next->origin.y;
		height = l->otherValues.text.f->f->height;

		a.y = JpicFix((r.origin.y + r.corner.y - height)>>1);

		if ((ns = (char *) getSpace(strlen(tempStr)+1))==NULL) {
			tempStr[0] = '\0';
			ns = "";
		}
		for( j=0; tempStr[j] != '\0'; j++ )
			ns[j] = tempStr[j];
		ns[j]='\0';
		t = newText(a,ns);
		t->otherValues.text.f = findFont(l->otherValues.text.f->ps,
						 l->otherValues.text.f->num);
		t->otherValues.text.just = l->otherValues.text.just;
		t->otherValues.text.spacing = yDiff - height;
		deleteAllThings(l);
		free(s);
	      }

	    } else {    

	      /* This is an ordinary macro */
	      r = macroBB(l);

	      if ((m = findMacro (s)) == MNULL) {
	        if ((t=l)!=TNULL) {
	          do {
		      makeRelative(t,r.origin);
		      t = t->next;
		  } while (t != l);
	        }
	        m=recordMacro(l,rsubp(r,r.origin),s);
	      }
	      else {	/* Free up list of things just allocated */
	        deleteAllThings (l);
	        free (s);
	      }
	      t = newMacro(r.origin,m);
	    }
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

/* This little routine reads the next two numbers (height and width) */
/* from fp and calculates the coordinates of a box of that size */
/* centered in the picture drawing area.  It then draws the box */
/* on the screen and allows the user to change its location. */
/* The final results are Xoffset and Yoffset being set to the */
/* origin of this box. */

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
  width  = getInt (fp);
  r.origin = align( Pt((XPicSize-width)>>1, (YPicSize-height)>>1) );
  Xoffset = r.origin.x;
  Yoffset = r.origin.y;
  r.corner.x = Xoffset + width;
  r.corner.y = Yoffset + height;
  offset.x = Xmin;
  offset.y = YPIC;

  cursset (raddp(r,offset));
  xbox (raddp(r,offset));	/* Show the box on the screen */

  ringbell();
  changeButtons (READbuttons);
  while (!button12 () || button3()) {
    wait (MOUSE);
  }
  if (button2 ()) {
    r = moveBox (r.origin, r, offset);
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
  return( JpicFix(i) );
}

/* Temporary getInt without JpicFix rounding - needed for number
   of splines */
  int 
getTempInt(f) 
  FILE *f;
{
  register int i;

  skipWhiteSpace(f);
  i=0;
  while (c >= '0' && c<='9' ) {
    i = 10 * i + c - '0';
    getChar(f);
  } 
  return( i );
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
  if ((t = (char *) getSpace(strlen(s)+1))==NULL) {
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

  for (m=macroList; ((m!=MNULL) &&
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
  register char *s;
  register int f, i, j, len; 
  char c1, c2;

  s = t->otherValues.text.s;
  len = strlen(s);
  if (compare(&s[len-6],"\\f1\\s0") && s[0]=='\\' && s[1]=='f') {
  
    if( s[2] == '(' ) {
	/* 2 character font name */
	c1 = s[3];
	c2 = s[4];
	i = 7;
    } else {
	/* 1 character font name */
	c1 = s[2];
	c2 = ' ';
	i = 5;
    }

    switch(c1) {
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
      case 'C': {
	f = CONSTANTWIDTH;
	break;	
      }
      case 'H': {
	switch(c2) {
	case 'B':
	  f = HB;
	  break;
	case 'I':
	  f = HI;
	  break;
	default:
	  f = HELVETICA;
	  break;
	}
	break;
      }
      case 'P': {
	switch(c2) {
	case 'B':
	  f = PB;
	  break;
	case 'I':
	  f = PI;
	  break;
	default:
	  f = PALATINO;
	  break;
	}
	break;
      }
      case 'E': {
	switch(c2) {
	case 'B':
	  f = EB;
	  break;
	case 'I':
	  f = EI;
	  break;
	default:
	  f = EXPANDED;
	  break;
	}
	break;
      }
      default: {
	f = ROMAN;
	break;
      }
    }

    /* Handle one or two digit point size */
    ps = s[i++] - '0';
    if (isdigit(s[i]))
      ps = ps*10 + s[i++] - '0';

    if (s[i] == '\\' && s[i+1] == '&') {
      i += 2;		/* Skip over \& */
    }

    /* Move in text string */
    for (j=0; i<len-6; ) {
      /* A single backslash is read as two - so skip over one. 
       * Except for the special cases of \\( and \\* - leave them as is.
       */
      if( (s[i] == '\\') && (s[i+1] == '\\') && !(
		(s[i+2] == '(') || (s[i+2] == '*') ) )
	i++;
      s[j++] = s[i++];
    }

    s[j] = '\0';
    t->otherValues.text.f = findFont(ps,f);
  }
}

/* NOTE 1: */
/* There is a bug in the way Jpic processes macros. */
/* The following cip output defines 2 macros around one box. */

/* .PS						*/
/* scale=100					*/
/* define m0 |					*/
/* [ box invis ht 86 wid 85 with .sw at 0,0	*/
/* box ht 86 wid 85 with .nw at 0,86		*/
/* ] |						*/
/*						*/
/* define m1 |					*/
/* [ box invis ht 86 wid 85 with .sw at 0,0	*/
/* m0 with .nw at 0,86				*/
/* ] |						*/
/*						*/
/* box invis ht 280 wid 171 with .sw at 0,0 	*/
/* m1 with .nw at 86,280 			*/
/* m1 with .nw at 0,86 				*/
/* .PE 						*/

/* The output produced by Jpic is (this is only the part which cip */
/* looks at): */

/* .PS 280 171 		*/
/* .u m1		*/
/* .u m0		*/
/* b 85 86 170 0	*/
/* .e			*/
/* .e			*/
/* .u m1		*/
/* .u m0		*/
/* b 0 280 84 194	*/
/* .e			*/
/* .e			*/

/* Note that the first box is a different size than the second. */
/* Since readPic does the drawing of the second box and the before */
/* the first box is used as the macros definition, the box gets */
/* draw wrong. */
