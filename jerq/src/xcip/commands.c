#ifndef	JPIC
#define	JPIC	"xJpic "
#endif	JPIC


#include "cip.h"


char jpicCmd[sizeof (JPIC) + MAXNAMESIZE] = JPIC;
#define filename (jpicCmd + sizeof (JPIC) - 1)

struct macro *macroList;
int nextMacroName;
Rectangle macroBB();


extern int currentBrush, copyFlag, gridState, buttonState;
extern int videoState;
extern int editDepth;
extern char c;
extern char fontString[];
extern Rectangle brushes[];
extern Rectangle BBpic;
extern struct thing *readPic();
extern void initMessage ();
extern void putMessage ();
extern void StringSize();
extern Point jString ();
extern void jMoveTo ();
extern Point translate();

char tempString[MAXTEXT];


Texture16 hourglass = {
	 0x7FFE, 0x7FFE, 0x2814, 0x2814, 0x27E4, 0x27E4, 0x23C4, 0x2184,
	 0x2184, 0x2244, 0x24A4, 0x2424, 0x2894, 0x29D4, 0x7FFE, 0x7FFE,
};



char fixedString[MAXTEXT];

/* Fixes string by placing backslashes in front of characters
 * that give problems in pic and troff (i.e., double quote
 * and backslash).  Exceptions are "\(" and "\*" which are passed thru
 * untouched to allow users to get things like bullits ("\(bu") and
 * trademark ("\*(Tm").
 */
	char *
addBackslashes( s )
	register char *		s;
{
	register int	i,j;

	for( j=i=0; s[i]!='\0'; i++ ) {
		if( s[i] == '"' ) {
			fixedString[j++] = '\\';
		} else if( s[i] == '\\' ) {
			if( (s[i+1] == '\\') && (
				(s[i+2] == '(') || (s[i+2] == '*') ) ) {
				/* Special cases of \\( turn to \\\\( 
				 * and \\* turn to \\\\*
				 */
				fixedString[j++] = '\\';
				fixedString[j++] = '\\';
				fixedString[j++] = '\\';
				i++;
			} else if ( (s[i+1] != '(') && (s[i+1] != '*') ) {
				/* Normal case - add 3 extra backslashes */
				fixedString[j++] = '\\';
				fixedString[j++] = '\\';
				fixedString[j++] = '\\';
			}
		}
		fixedString[j++] = s[i];
	}

	fixedString[j++] = '\0';
	return( fixedString );
}


  int
numLines(s)
  register char *s;
{
  register int i;
  register int num = 1;

  if (s[0]=='\0') {
    return (0);
  } else {
    for(i=0; s[i]!='\0'; i++)
      if ( s[i] == '\r' )
	num++;
    return (num);
  }
}


putText(fp,h)		/* recursive */
	FILE *fp;
	register struct thing *h;
{
	register struct thing *t;
        register int q,v,start;
	int 	hi;
        char *	s;
	short	spacing;
	Point	w;

	if ((t = h) != TNULL) {
	    do {
	    	if (t->type == TEXT) {
	    	    s = t->otherValues.text.s;
	    	    if ( numLines(s) > 1 ) {
	    		s = addBackslashes(s);
	    		spacing = t->otherValues.text.spacing +
				  t->otherValues.text.f->f->height;
			w = sub(t->bb.corner, t->bb.origin);
	    		fprintf(fp, "define t%d |\n", 
	    			t->otherValues.text.outName);
	    		fprintf(fp, 
	    		   "[ box invis ht %d wid %d with .sw at 0,0\n",
			   w.y, w.x );

	    		hi = w.y - (t->otherValues.text.f->f->height>>1);
	    		q = 0; v = 0; start = 0;
	    		while(s[q] != '\0') {
            	    		for(q=start; (s[q]!='\r')&&(s[q]!='\0');
	    					q++)
                	    		tempString[v++] = s[q];
            	    		tempString[v] = '\0';

	    			font2string(t->otherValues.text.f->num);
	    			if( strlen(fontString) == 2 )
	    			  fprintf(fp, "\"\\f(%s", fontString );
	    			else
	    			  fprintf(fp, "\"\\f%s", fontString );

	    		        switch (t->otherValues.text.just) {
	    		        case CENTER:
	                     fprintf(fp,"\\s%d\\&%s\\f1\\s0\" at %d,%d\n",
	    				t->otherValues.text.f->ps,
	    	                        tempString, (w.x>>1), hi );
	    			      break;
	    		        case LEFTJUST:
                             fprintf(fp,"\\s%d\\&%s\\f1\\s0\" at %d,%d %s\n",
	    				t->otherValues.text.f->ps,
	    	                        tempString, 0, hi, "ljust" );
	    			      break;
	    			case RIGHTJUST:
                             fprintf(fp,"\\s%d\\&%s\\f1\\s0\" at %d,%d %s\n",
	    				t->otherValues.text.f->ps,
	    	                        tempString, w.x, hi, "rjust" );
	    			      break;
	    			}

                    	    	start = q+1;
                    	    	hi = hi - spacing; 
	    	    		v = 0;
                       	    }
                        fprintf(fp, "] |\n\n");
	    	    }
       	    	} else if (t->type == MACRO) {
	    		putText(fp,t->otherValues.list->parts);
	    	}
       	    	t = t->next;
     	    } while(t != h);
   	}
}


doPut(h)
struct thing *h;
{

	register struct thing *t;
	FILE *fp;
	char buf [BUFSIZ];
	struct macro *m;
	int saveB;

	initMessage();

	if (h==TNULL) {
	  putMessage("Nothing to write out!");
	  return;
	}

	if( editDepth != 0 ) {
		ringbell();
		putMessage("Warning: currently editing a macro.\r");
		putMessage("(Select edit depth box to leave.)\r");
	}

	saveB = buttonState;
	putMessage("Put file: ");
	getMessage(filename);

	if ( filename[0] == '\0' ) {
		message("*** put file cancelled ***");

	} else if ((fp = fopen(filename,"w")) == (FILE *) NULL) {
		fileError("open",filename);

	} else {
		fp->cp = fp->base = buf;
		fp->count = BUFSIZ;		/* NOTE 1 */
		fprintf(fp,".nf\n.PS\nscale=100\n");
		cursswitch(&hourglass);
		changeButtons(BLANKbuttons);
		findBBpic(h);

		putText(fp,h);

		/* Write out macro definitions */
		nextMacroName = 0;
		for (m=macroList; m!=MNULL; m=m->next) {
			if (m->useCount>0) {
				m->outName = nextMacroName++;
				fprintf(fp,"define m%d |\n",m->outName);
				fprintf(fp,"[ box invis ht %d wid %d with .sw at 0,0\n",
				    m->bb.corner.y, m->bb.corner.x);
				if ((t=m->parts) !=TNULL) {
					do {
						writePIC( t, fp, Rpt(Pt(0,0),
						          m->bb.corner) );
						t=t->next;
					} while (t!=m->parts);
				}
				fprintf(fp,"] |\n\n");
			}
		}

		/* Write out bounding box. */
		fprintf(fp,"box invis ht %d wid %d with .sw at 0,0\n",
		    BBpic.corner.y - BBpic.origin.y,
		    BBpic.corner.x - BBpic.origin.x);

		/* Write out objects */
		if ((t = h) != TNULL) {
			do {
				writePIC(t,fp,BBpic);
				t = t->next;
			} while (t!=h);
		}
		fprintf(fp,".PE\n.fi\n");
		fclose(fp);
		cursSwitch();

	}

	changeButtons(saveB);
}


/* NOTE 1 */
/* If the terminal is low on alloc space then the i/o routines */
/* (putc specifically) will not be able to set up a buffer.  */
/* Therefore it is done here locally such that it doesn't take */
/* away space for drawing pictures. */

fileError(s,fn)
	register char *s, *fn;
{
	initMessage ();
	putMessage("cannot ");
	putMessage(s);
	putMessage(" file ");
	putMessage(fn);
}


	struct thing *
doGet(h)
	register struct thing *h;
{
	register FILE *fp;
	register struct macro *m;

	initMessage();

	if( editDepth != 0 ) {
		ringbell();
		putMessage("Warning: currently editing a macro.\r");
		putMessage("(Select edit depth box to leave.)\r");

	} else if( h != TNULL ) {
		putMessage("Note: adding to existing picture.\r");
	}

	putMessage("Get file: ");
	getMessage(filename);
	if (filename[0]=='\0') {
		message("*** get file cancelled ***");
	} else {
		if (access(filename,4)!=0) {
			fileError("access",filename);
		}
		else {
			if ((fp = popen(jpicCmd,"r")) == (FILE *) NULL) {
				fileError("open pipe for",filename);
			}
			else {
				cursswitch(&hourglass);
				changeButtons(BLANKbuttons);
				getChar(fp);
				h = readPic(fp,h);
				for (m = macroList; m != MNULL; m = m->next) {
					free (m->name);
					/* Null out name incase */
					/* another file is read */
					m->name = (char *)NULL;	
				}
				pclose(fp);
				cursSwitch();
			}
		}
	}
	changeBrush(-1);
	changeButtons(INITbuttons);
	return(h);
}


struct thing *
doClear(h)
struct thing *h;
{
	deleteAllThings(h);
	cursinhibit();
	stipple(brushes[PIC]);
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
	register struct thing *s, *t, *l = { 
		TNULL 	};
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

	while( !button12() )
	  nap(2);
	
	if( button1() ) {
	  /* abort defing macro */
	  while( button1() )
	    nap(2);
	  changeButtons(INITbuttons);
	  return( h );
	}

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
		r = macroBB(l);		/* Get outline of macro */
		do {
			makeRelative(t,r.origin);	/* Make things relative to this box */
			t = t->next;
		} while (t != l);
		p = r.origin;		 /* Macro is located at original orig */
		r = rsubp (r, r.origin); /* but the bb is made relative. */
		h = insert(newMacro(p,recordMacro(l,r,NULL)),h);

		/* To insure new macro is first thing selected,
		   back up h list by one element. */
		h = h->last;
	}

	changeButtons(INITbuttons);
	return(h);
}

	Rectangle 
macroBB(l)
	struct thing *l;
{
	Point p, q, p1, p2;
	register struct thing *t;
	Rectangle r;

	p.x = Xmax; 
	p.y=YBOT; 
	q.x=0; 
	q.y=0;
	if ((t=l) != TNULL) {
		do {
			if (t->type == TEXT) {
				/* Expand out the bounding box to nearest
				   alignment points to insure alignment
				   of new macro. */
				p1 = alignDown( t->bb.origin );
				p2 = alignUp(   t->bb.corner );
			} else {
				p1 = t->bb.origin;
				p2 = t->bb.corner;
			}
			p.x = min(p.x,p1.x);
			p.y = min(p.y,p1.y);
			q.x = max(q.x,p2.x);
			q.y = max(q.y,p2.y);
			t=t->next;
		} while (t != l);
	}

	/* Add 1 to prevent zero size bounding box. */
	if (p.x == q.x) 
		q.x += 1;
	if (p.y == q.y)
		q.y += 1;

	r.origin = p; 
	r.corner = q;
	return(r);
}

/* This routine places a new macro at the start of the macro list. */
/* The head of the list is pointed to by macroList. */
/* The bounding box for the macro always has a relative origin of */
/* (0,0). */

struct macro *
recordMacro(list,r,s)
struct thing *list;		/* List of things making up macro */
Rectangle r;			/* BB of macro with origin at (0,0) */
char *s;			/* Name of macro */
{
	register struct macro *m, *l, *n;

	if ((m = (struct macro *) getSpace(sizeof(struct macro)))
	    !=MNULL) {
		m->name = s;
		m->bb = r;
		m->useCount = 0;
		m->parts = list;
		/* Append macro to the end of the macro list */
		l = MNULL;
		for (n=macroList; n!=MNULL; n=n->next) {
			l=n;
		}
		if (l == MNULL) {
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
	case TEXT: 
		{
			break;
		}
	case LINE: 
		{
			t->otherValues.end = sub(t->otherValues.end,p);
			break;
		}
	case BOX: 
		{
			t->otherValues.corner = sub(t->otherValues.corner,p);
			break;
		}
	case ARC: 
		{
			t->otherValues.arc.start = sub(t->otherValues.arc.start,p);
			t->otherValues.arc.end = sub(t->otherValues.arc.end,p);
			break;
		}
	case SPLINE: 
		{
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

#if defined(DMD630) || !defined(MPX)
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
#endif


/* This routine places a stippled texture into the rectangle */
/* specified by r in the layer specified by P->layer.  No stippling */
/* is used if the user owns the mouse.  Instead a blank */
/* stipple pattern is used.  Note, that when STORE mode is used, */
/* anything that was in the rectangle is wiped out.  */
/* Reverse textures are used if reverse video is set. */
/* Note: with 2.0 ROMS, stippling is no longer used. */

stipple(r)
	Rectangle r;
{
	static Texture16 plain = { 
		0 	};
	static Texture16 fancy = { 
		0x1000 	};
	static Texture16 rvplain = {
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
	};
	static Texture16 rvfancy = {
		0xEFFF, 0xFFFF, 0xFFFF, 0xFFFF,
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
	};
	Texture16 *tp;

	if (videoState == BLACKfield) {
		tp = ( (own()&MOUSE) || (version()>=0x80705) )
			? &rvplain
			: &rvfancy;
	}
	else {
		tp = ( (own()&MOUSE) || (version()>=0x80705) )
			? &plain
			: &fancy;
	}
	texture16 (P->layer, r, tp, F_STORE);
}



#ifdef DMD630

/* This routine clears out region. */

stipple(r)
	Rectangle r;
{
	static Texture16 plain = { 
		0 	};
	static Texture16 rvplain = {
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
	};

	Texture16 *tp;

	if (videoState == BLACKfield) {
		tp = &rvplain;
	} else {
		tp = &plain;
	}

	texture16 (P->layer, r, tp, F_STORE);
}

#endif


cursSwitch ()
{
	if (own() & MOUSE) {
		cursswitch(ptinrect(MOUSE_XY,brushes[PIC])
		    ? &crossHairs : (Texture16 *)0);
	}
}



/* Get a character from keyboard and add to text string associated with thing
 * unless character is a control character in which case the string is edited.
 * The charPos is the position where editing occurs in the character string.
 * Function returns the new charPos after editing.
 */
	int
editText( t, offset, charPos )
	register struct thing *	t;
	Point			offset;
	int			charPos; /* position where edited */
{
	register char *		s;
	register int 		i,j,k;
	char 			temp_str[MAXTEXT];
	register char		c;

	s = t->otherValues.text.s;
	i = charPos;

	draw(t,offset);  /* erases present string. */

	/* copy part of string before edit cursor into temporary buffer. */
	strncpy(temp_str,s,charPos);

	c = kbdchar();

#ifndef DMD630
	if (c >= 0x82 && c <= 0x89) /* DMD function keys */ {
		j = c - 0x82;
		k = 0;
		while ((c = BRAM->pfkeys[j][i].byte) && (++k <= PFKEYSIZE)) {
			if (i >= MAXTEXT) {
				ringbell ();
				break;
			}
			else {
				temp_str[i++] = c;
			}
		}
	} else {
#endif
		switch (c) {
		case '\033':	/* ignore escape */
		case '\177':	/* ignore delete */
		case '\t':	/* ignore tab */
			{
				break;
			}
		case '@':
		case 'U'-'@': 	/* ctrl-u */
			{
				i=0;
				break;
			}
		case 'W'-'@': 	/* ctrl-w */
			{
				i = backspaceOneWord(temp_str,i);
				break;
			}
		case '\b': 
			{
				if (i > 0)
					i--;
				break;
			}
		case '\r':	/* return */
		case '\n':	/* line feed */
				c = '\r';  /* put on EOL character */
				/* FALL THRU to put EOL character in string */
		default: 
			{
				if (i >= MAXTEXT) {
					ringbell ();
				}
				else {
					temp_str[i++] = c;
				}
				break;
			}
		}
#ifndef DMD630
	}
#endif

	/* Copy any part of string that was after edit cursor and null char. */
	for( j=charPos, charPos=i; (temp_str[i++] = s[j++]) != '\0'; )
		;

	free(s);
	s = getSpace(i);
	strcpy(s,temp_str);
	t->otherValues.text.s = s;

	draw( t, offset );  /* display new string. */

	return( charPos );
}


drawText(p,s,just,spacing,f)
	Point p;		/* point at center of text */
	register char *s;
	short just;		/* justification */
	short spacing;
	Font *f;
{
	register int line;	/* line number */
	register int lx;	/* line x position */
	register int ly;	/* line y position */
	register int q;
	register int m;
	int num; 		/* number of lines */

	num = numLines(s);

	if( num == 0 )
		return;

	ly = -( ( (f->height+spacing) * (num-1) ) >> 1 );

	q = 0;

	for(line = 1; line <= num; line++)
	{
		for(m = 0; (s[q] != '\r') && (s[q] != '\0'); q++ )
			tempString[m++] = s[q];
		tempString[m] = '\0';
		q++;
		switch (just) {
		case CENTER:
			lx = p.x - ( strwidth(f, tempString)>>1 );
			break;
		case LEFTJUST:
			lx = p.x;
			break;
		case RIGHTJUST:
			lx = p.x - strwidth(f, tempString);
			break;
		}
		string(f, tempString,&display, Pt(lx, (p.y + ly)), F_XOR);
		ly = ly + (f->height+spacing);
	}
}


	void 
StringSize(f,spacing,s,h,w)
	register int *h, *w;
	Font *f;
	register char *s;
	short spacing;
{
	register int num;
	register int m,q;
	register int line;

	num = numLines(s);

	*h = (num * f->height) + ( (num-1) * spacing );

	*w = 0;
	q = 0;

	for(line = 1; line <= num; line++) {
		for(m = 0; (s[q] != '\r') && (s[q] != '\0'); q++)
			tempString[m++] = s[q];
		tempString[m] = '\0';

		*w = max( *w, strwidth(f,tempString) );
		q++;
	}

	*w = *w + 5;
}



#ifdef DMD630
/* Word is 16 bits. */
Word textMarker[] = {		/* marker icon */
	 0x0000, 0x0000, 0x0000, 0x0080,
	 0x0080, 0x01C0, 0x01C0, 0x03E0,
	 0x03E0, 0x07F0, 0x07F0, 0x0FF8,
	 0x0FF8, 0x0FF8, 0x0FF8, 0x0000,
};

Word markerMask[] = {		/* one bit bigger marker icon */
	 0x0000, 0x0000, 0x0080, 0x01C0,
	 0x01C0, 0x03E0, 0x03E0, 0x07F0,
	 0x07F0, 0x0FF8, 0x0FF8, 0x1FFC,
	 0x1FFC, 0x1FFC, 0x1FFC, 0x1FFC,
};

#else
/* Word is 32 bits on DMD 5620. */
Word textMarker[] = {		/* marker icon */
	 0x00000000, 0x00000000, 0x00000000, 0x00800000,
	 0x00800000, 0x01C00000, 0x01C00000, 0x03E00000,
	 0x03E00000, 0x07F00000, 0x07F00000, 0x0FF80000,
	 0x0FF80000, 0x0FF80000, 0x0FF80000, 0x00000000,
};

Word markerMask[] = {		/* one bit bigger marker icon */
	 0x00000000, 0x00000000, 0x00800000, 0x01C00000,
	 0x01C00000, 0x03E00000, 0x03E00000, 0x07F00000,
	 0x07F00000, 0x0FF80000, 0x0FF80000, 0x1FFC0000,
	 0x1FFC0000, 0x1FFC0000, 0x1FFC0000, 0x1FFC0000,
};
#endif


#define MARKERWID	16
#define MARKERHT	16

Word	bgsave[MARKERHT];	/* Space to save background */

Bitmap	markerbm = { 
		textMarker,	/* Will be changed to markerMask and back */
		1,
		0, 0, MARKERWID, MARKERHT,
		0
	};

	int
moveTextSpot(t,offset)
 	struct thing *	t;
	Point		offset;
{
	register char *	s;
	register int 	i;	/* position in text string */
	int		istart;	/* saves start of line */
	register int	j;	/* position in temp string */
	register int 	line;	/* line number */
	register int 	x;	/* line x position */
	register int 	y;	/* line y position */
	Point		m;	/* mouse position */
	Point		pt;	/* text marker point */
	int		spacing;
	int		prevI;	/* previous position in text string */
	int		w;	/* string or character width */
	int 		num; 	/* number of lines */
	short		just;	/* justification */
	Font *		f;

	s = t->otherValues.text.s;
	f = t->otherValues.text.f->f;
	just = t->otherValues.text.just;
	spacing = f->height + t->otherValues.text.spacing;

	pt.x = offset.x;
	pt.y = offset.y;
	cursinhibit();

	markerbm.base = bgsave;		/* save initial background */
	bitblt( P->layer, raddp(markerbm.rect,pt),
		&markerbm,markerbm.rect.origin, F_STORE );

	prevI = -1;

	do {
		m = sub(MOUSE_XY,offset);

		num = numLines(s);
		x = t->origin.x;
		y = t->origin.y - ( ( spacing * (num-1) ) >> 1 );


		/* Skip over any text lines above mouse position, except
		 * do not skip over last line even if mouse is below it.
	 	 */

		i = 0;
		line = 1;
		for (; (line < num) && (m.y >= (y + spacing)); line++) {

			while (s[i++] != '\r')
				;
			y += spacing;
		}


		/* Form string from only this line's characters. */

		istart = i;	/* Save start of current line */

		for (j=0; (s[i] != '\r') && (s[i] != '\0'); ) {
			tempString[j++] = s[i++];
		}
		tempString[j] = '\0';


		/* Adjust x for justification */

		w = strwidth( f, tempString);
		switch (just) {
		case CENTER:
			x = t->origin.x - ( w >> 1 );
			break;
		case LEFTJUST:
			x = t->origin.x;
			break;
		case RIGHTJUST:
			x = t->origin.x - w;
			break;
		}


		i = istart;	/* Back to start of line */

		/* Find character hit */

		while (( x <= m.x) && (s[i] != '\0') && (s[i] != '\r')) {
			x += f->info[s[i++]].width;    /* Char width */
		}

		if( i != prevI ) {
			prevI = i;

			/* restore background */
			markerbm.base = bgsave;
			bitblt( &markerbm, markerbm.rect, P->layer, pt, F_STORE );

			pt.x = x - 9 + offset.x;
			pt.y = y + f->height - (f->height>>2) -4 + offset.y;

			/* save background */
			bitblt( P->layer, raddp(markerbm.rect,pt),
				&markerbm,markerbm.rect.origin, F_STORE );
			markerbm.base = markerMask;	/* make dark edge  */
			bitblt( &markerbm, markerbm.rect, P->layer, pt, F_OR );
			markerbm.base = textMarker;	/* draw marker */
			bitblt( &markerbm, markerbm.rect, P->layer, pt, F_XOR );

			/* Note: F_OR & F_XOR used when marking on 
			 * highlighted area; F_CLR & F_OR otherwise.
			 */
		}

		nap(2);
	} while( button1() );

	/* restore background */
	markerbm.base = bgsave;
	bitblt( &markerbm, markerbm.rect, P->layer, pt, F_STORE );

	cursallow();
	return( i );
}
