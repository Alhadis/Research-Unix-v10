# 1 ""





# 1 "./cip.h"








# 18 "./cip.h"

# 1 "/usr/jerq/include/jerq.h"






# 1 "/usr/jerq/include/sys/2681.h"










































































struct duart
{
	int : 16; int :8; unsigned char mr1_2a;	  
	int : 16; int :8; unsigned char a_sr_csr; 
	int : 16; int :8; unsigned char a_cmnd;	  
	int : 16; int :8; unsigned char a_data;   

	int : 16; int :8; unsigned char ipc_acr;  
	int : 16; int :8; unsigned char is_imr;	  
	int : 16; int :8; unsigned char ctur;	  
	int : 16; int :8; unsigned char ctlr;	  

	int : 16; int :8; unsigned char mr1_2b;	  
	int : 16; int :8; unsigned char b_sr_csr; 
	int : 16; int :8; unsigned char b_cmnd;	  
	int : 16; int :8; unsigned char b_data;   

	int : 16; int :8; int :8;		  

	int : 16; int :8; unsigned char ip_opcr;  
	int : 16; int :8; unsigned char scc_sopbc;
	int : 16; int :8; unsigned char scc_ropbc;
};

























 


























































































# 8 "/usr/jerq/include/jerq.h"






















typedef int	Word;		

typedef unsigned int	UWord;	

typedef struct Point {
	short	x;
	short	y;
} Point;

typedef struct Rectangle {
	Point origin;
	Point corner;
} Rectangle;

typedef struct Bitmap {
	Word	*base;		
	unsigned width;		
	Rectangle rect;		
	char	*_null;		
} Bitmap;

typedef struct Menu{
	char	**item;			
	char	*(*generator)();	
	short	prevhit;		
	short	prevtop;		
} Menu;

typedef struct Texture32 {
	Word bits[32];
} Texture32;

typedef struct Texture {
	short	bits[16];
} Texture;

extern struct Mouse {
	Point	xy;
	short	buttons;
} mouse;

# 75 "/usr/jerq/include/jerq.h"















Rectangle getrectb(), getrect();










extern Word topbits[], botbits[];	
extern Rectangle Jrect;
extern Bitmap display;




typedef int	Code;




Point add(), sub(), mul(), div(), jstring(), string();
Rectangle rsubp(), raddp(), inset();
Word *addr();
char	*alloc(), *gcalloc();
Bitmap *balloc();
Texture *cursswitch();










# 133 "/usr/jerq/include/jerq.h"












# 20 "./cip.h"
# 1 "/usr/jerq/include/layer.h"


typedef struct Layer{
	
	Word *base;
	unsigned width;
	Rectangle rect;	







	short someobs, nonevis;
	struct Layer *front;	
	struct Layer *back;	
	Bitmap *obs;		
}Layer;

typedef Layer Obscured;

Rectangle rsubp();
Layer *newlayer();

extern Layer *lfront, *lback;


# 21 "./cip.h"
# 1 "/usr/jerq/include/jerqio.h"



extern	struct	_iobuf {
	int	count;
	char	*cp;
	char	*base;
	char	flag;
	char	file;
} _iob[15];










# 23 "/usr/jerq/include/jerqio.h"












struct _iobuf	*fopen();
struct _iobuf	*popen();
# 22 "./cip.h"
# 1 "/usr/jerq/include/font.h"















typedef struct Fontchar
{
	short x;		
	unsigned char top;	
	unsigned char bottom;	
	char left;		
	unsigned char width;	
} Fontchar;

typedef struct Font
{
	short n;		
	char height;		
	char ascent;		
	long unused;		
	Bitmap *bits;		
	Fontchar info[1];		
} Font;

extern Font *infont();		
extern Font *getfont();		

# 40 "/usr/jerq/include/font.h"

extern Font defont;



# 23 "./cip.h"





typedef struct Proc { 
	Bitmap *layer
} Proc;

extern Proc *P;




# 40 "./cip.h"




# 47 "./cip.h"











extern short Xmin;		
extern short Ymin;		

extern Point drawOffset;	

extern short Xoffset;	
extern short Yoffset;






# 74 "./cip.h"














































































































struct FONTBlk {
	short		ps; 	
	short		num;	
	short		useCount;
	Font *		f;
	struct FONTBlk *	next;
	struct FONTBlk *	last;
};

typedef struct {
	Point start, end;
} pointPair;

typedef struct {
	short ht, wid;
} intPair;

typedef struct {
	short	used; 
	short	size;
	Point *	plist
} pointStream;

typedef struct {
	short		just;		
	short		spacing;	
	char *		s;
	struct FONTBlk *	f;
	short		outName;
} textString;

struct macro {
	char *		name;
	short 		outName;
	short 		useCount;
	Rectangle	bb;
	struct thing *	parts;
	struct macro *	next;
};

struct thing {
	short		type;
	Point		origin;	
	Rectangle 	bb;
	union {
		short		brush;
		short		radius;
		Point		corner;
		Point		end;
		pointPair	arc;
		intPair		ellipse;
		textString	text;
		pointStream	spline;
		struct macro *	list;
	} otherValues;
	short		arrow;
	short		boorder;
	struct thing *	next; 
	struct thing *	last;
};

extern struct FONTBlk *fonts;

extern Rectangle *select();
extern Rectangle moveBox();
extern Rectangle macroBB();


extern Rectangle canon();


extern struct macro *recordMacro();

extern struct thing *newCircle();
extern struct thing *newBox();
extern struct thing *newEllipse();
extern struct thing *newLine();
extern struct thing *newArc();
extern struct thing *newText();
extern struct thing *newSpline();
extern struct thing *newMacro();
extern struct thing *selectThing();
extern struct thing *copyThing();
extern struct thing *deleteThing();
extern struct thing *deleteAllThings();
extern struct thing *insert();
extern struct thing *remove();
extern struct thing *doMouseButtons();
extern struct thing *place();
extern struct thing *displayCommandMenu();
extern struct thing *displayThingMenu();
extern struct thing *doGet();
extern struct thing *doClear();
extern struct thing *defineMacro();
extern struct thing *makeRelative();
extern struct thing *reflect();

extern Point track();
extern Point track2();
extern Point computeArcOrigin();
extern Point jchar();

extern Point	align();
extern Point	alignUp();
extern Point	alignDown();
extern int	alignInt();

extern char *getString();
extern char *getSpace();

extern struct _iobuf *popen();

extern struct FONTBlk *findFont();

extern xtipple ();

extern Texture crossHairs;
extern Texture menucursor;

extern char * 	addBackslashes();

extern short	currAlignment;
extern short	currPS;
extern short	currFont;
extern short	currJust;
extern short	currLineType;
extern short	currBoxType;
extern short	currLineArrows;
extern short	currSplineArrows;
extern short	currSpacing;
# 7 ""


char jpicCmd[sizeof ("xJpic ") + 64] = "xJpic ";


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

char tempString[500];


Texture hourglass = {
	 0x7FFE, 0x7FFE, 0x2814, 0x2814, 0x27E4, 0x27E4, 0x23C4, 0x2184,
	 0x2184, 0x2244, 0x24A4, 0x2424, 0x2894, 0x29D4, 0x7FFE, 0x7FFE,
};



char fixedString[500];







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
				


				fixedString[j++] = '\\';
				fixedString[j++] = '\\';
				fixedString[j++] = '\\';
				i++;
			} else if ( (s[i+1] != '(') && (s[i+1] != '*') ) {
				
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


putText(fp,h)		
	struct _iobuf *fp;
	register struct thing *h;
{
	register struct thing *t;
        register int q,v,start;
	int 	hi;
        char *	s;
	short	spacing;
	Point	w;

	if ((t = h) != (struct thing *)0) {
	    do {
	    	if (t->type == 6) {
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
	    		        case 1:
	                     fprintf(fp,"\\s%d\\&%s\\f1\\s0\" at %d,%d\n",
	    				t->otherValues.text.f->ps,
	    	                        tempString, (w.x>>1), hi );
	    			      break;
	    		        case 0:
                             fprintf(fp,"\\s%d\\&%s\\f1\\s0\" at %d,%d %s\n",
	    				t->otherValues.text.f->ps,
	    	                        tempString, 0, hi, "ljust" );
	    			      break;
	    			case 2:
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
       	    	} else if (t->type == 7) {
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
	struct _iobuf *fp;
	char buf [256];
	struct macro *m;
	int saveB;

	initMessage();

	if (h==(struct thing *)0) {
	  putMessage("Nothing to write out!");
	  return;
	}

	if( editDepth != 0 ) {
		((struct duart *)0x200000)->b_data=0x08,nap(3),((struct duart *)0x200000)->b_data=0;
		putMessage("Warning: currently editing a macro.\r");
		putMessage("(Select edit depth box to leave.)\r");
	}

	saveB = buttonState;
	putMessage("Put file: ");
	getMessage((jpicCmd + sizeof ("xJpic ") - 1));

	if ( (jpicCmd + sizeof ("xJpic ") - 1)[0] == '\0' ) {
		message("*** put file cancelled ***");

	} else if ((fp = fopen((jpicCmd + sizeof ("xJpic ") - 1),"w")) == (struct _iobuf *) ((char *)0)) {
		fileError("open",(jpicCmd + sizeof ("xJpic ") - 1));

	} else {
		fp->cp = fp->base = buf;
		fp->count = 256;		
		fprintf(fp,".nf\n.PS\nscale=100\n");
		cursswitch(&hourglass);
		changeButtons(5);
		findBBpic(h);

		putText(fp,h);

		
		nextMacroName = 0;
		for (m=macroList; m!=(struct macro *)0; m=m->next) {
			if (m->useCount>0) {
				m->outName = nextMacroName++;
				fprintf(fp,"define m%d |\n",m->outName);
				fprintf(fp,"[ box invis ht %d wid %d with .sw at 0,0\n",
				    m->bb.corner.y, m->bb.corner.x);
				if ((t=m->parts) !=(struct thing *)0) {
					do {
						writePIC( t, fp, (((((short)(0))<<16)|((unsigned short)(0)))), ( 						          m->bb.corner) );
# 230 ""
						t=t->next;
					} while (t!=m->parts);
				}
				fprintf(fp,"] |\n\n");
			}
		}

		
		fprintf(fp,"box invis ht %d wid %d with .sw at 0,0\n",
		    BBpic.corner.y - BBpic.origin.y,
		    BBpic.corner.x - BBpic.origin.x);

		
		if ((t = h) != (struct thing *)0) {
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
	register struct _iobuf *fp;
	register struct macro *m;

	initMessage();

	if( editDepth != 0 ) {
		((struct duart *)0x200000)->b_data=0x08,nap(3),((struct duart *)0x200000)->b_data=0;
		putMessage("Warning: currently editing a macro.\r");
		putMessage("(Select edit depth box to leave.)\r");

	} else if( h != (struct thing *)0 ) {
		putMessage("Note: adding to existing picture.\r");
	}

	putMessage("Get file: ");
	getMessage((jpicCmd + sizeof ("xJpic ") - 1));
	if ((jpicCmd + sizeof ("xJpic ") - 1)[0]=='\0') {
		message("*** get file cancelled ***");
	} else {
		if (access((jpicCmd + sizeof ("xJpic ") - 1),4)!=0) {
			fileError("access",(jpicCmd + sizeof ("xJpic ") - 1));
		}
		else {
			if ((fp = popen(jpicCmd,"r")) == (struct _iobuf *) ((char *)0)) {
				fileError("open pipe for",(jpicCmd + sizeof ("xJpic ") - 1));
			}
			else {
				cursswitch(&hourglass);
				changeButtons(5);
				getChar(fp);
				h = readPic(fp,h);
				for (m = macroList; m != (struct macro *)0; m = m->next) {
					free (m->name);
					
					
					m->name = (char *)((char *)0);	
				}
				pclose(fp);
				cursSwitch();
			}
		}
	}
	changeBrush(-1);
	changeButtons(0);
	return(h);
}


struct thing *
doClear(h)
struct thing *h;
{
	deleteAllThings(h);
	cursinhibit();
	stipple(brushes[7]);
	cursallow();
	copyFlag = 0;
	changeBrush(-1);
	changeButtons(0);
	if (gridState==1) {
		drawGrid();
	}
	return((struct thing *)0);
}

struct thing *
defineMacro(h)
register struct thing *h;
{
	Point p, q;
	register struct thing *s, *t, *l = { 
		(struct thing *)0 	};
	Rectangle r;
	struct thing dummy;
	char *z;		

	changeBrush(-1);
	
	if ((z=getSpace(sizeof(struct macro)+sizeof(struct thing))) == ((char *)0)) {
		return (h);
	}
	free (z);
	changeButtons(6);

	while( !(mouse.buttons&6) )
	  nap(2);
	
	if( (mouse.buttons&4) ) {
	  
	  while( (mouse.buttons&4) )
	    nap(2);
	  changeButtons(0);
	  return( h );
	}

	p = sub((sub(mouse.xy, ((((short)(1))<<16)|((unsigned short)(1))))),((((short)(Xmin))<<16)|((unsigned short)((Ymin+(2+5+60+2)+5)))));
	q = track(p,((((short)(Xmin))<<16)|((unsigned short)((Ymin+(2+5+60+2)+5)))),1,h);
	r = canon (p, q);
	h = insert(&dummy,h);
	if ((t = h->next) != (struct thing *)0) {
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
	if ((t = l) != (struct thing *)0) {
		r = macroBB(l);		
		do {
			makeRelative(t,r.origin);	
			t = t->next;
		} while (t != l);
		p = r.origin;		 
		r = rsubp (r, r.origin); 
		h = insert(newMacro(p,recordMacro(l,r,((char *)0))),h);

		

		h = h->last;
	}

	changeButtons(0);
	return(h);
}

	Rectangle 
macroBB(l)
	struct thing *l;
{
	Point p, q, p1, p2;
	register struct thing *t;
	Rectangle r;

	p.x = (Xmin+(2+770+2)); 
	p.y=((Ymin+(2+5+60+2)+5)+2+840+2); 
	q.x=0; 
	q.y=0;
	if ((t=l) != (struct thing *)0) {
		do {
			if (t->type == 6) {
				


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

	
	if (p.x == q.x) 
		q.x += 1;
	if (p.y == q.y)
		q.y += 1;

	r.origin = p; 
	r.corner = q;
	return(r);
}






struct macro *
recordMacro(list,r,s)
struct thing *list;		
Rectangle r;			
char *s;			
{
	register struct macro *m, *l, *n;

	if ((m = (struct macro *) getSpace(sizeof(struct macro)))
	    !=(struct macro *)0) {
		m->name = s;
		m->bb = r;
		m->useCount = 0;
		m->parts = list;
		
		l = (struct macro *)0;
		for (n=macroList; n!=(struct macro *)0; n=n->next) {
			l=n;
		}
		if (l == (struct macro *)0) {
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
	case 0:
	case 2:
	case 6: 
		{
			break;
		}
	case 3: 
		{
			t->otherValues.end = sub(t->otherValues.end,p);
			break;
		}
	case 1: 
		{
			t->otherValues.corner = sub(t->otherValues.corner,p);
			break;
		}
	case 4: 
		{
			t->otherValues.arc.start = sub(t->otherValues.arc.start,p);
			t->otherValues.arc.end = sub(t->otherValues.arc.end,p);
			break;
		}
	case 5: 
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
	for ( ; i>0 && ((s[i-1]>='0' && (s[i-1])<='9') || ((s[i-1]>='a' && (s[i-1])<='z') || (s[i-1]>='A' && (s[i-1])<='Z'))); ) {
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











stipple(r)
	Rectangle r;
{
	static Texture plain = { 
		0 	};
	static Texture fancy = { 
		0x1000 	};
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

	if (videoState == 0) {
		tp = ( (own()&4) || (version()>=0x80705) )
			? &rvplain
			: &rvfancy;
	}
	else {
		tp = ( (own()&4) || (version()>=0x80705) )
			? &plain
			: &fancy;
	}
	texture16 (P->layer, r, tp, ((Code) 0));
}



# 640 ""



cursSwitch ()
{
	if (own() & 4) {
		cursswitch(ptinrect((sub(mouse.xy, ((((short)(1))<<16)|((unsigned short)(1))))),brushes[7])
		    ? &crossHairs : (Texture *)0);
	}
}








	int
editText( t, offset, charPos )
	register struct thing *	t;
	Point			offset;
	int			charPos; 
{
	register char *		s;
	register int 		i,j,k;
	char 			temp_str[500];
	register char		c;

	s = t->otherValues.text.s;
	i = charPos;

	draw(t,offset);  

	
	strncpy(temp_str,s,charPos);

	c = kbdchar();


	if (c >= 0x82 && c <= 0x89)  {
		j = c - 0x82;
		k = 0;
		while ((c = BRAM->pfkeys[j][i].byte) && (++k <= PFKEYSIZE)) {
			if (i >= 500) {
				((struct duart *)0x200000)->b_data=0x08,nap(3),((struct duart *)0x200000)->b_data=0;
				break;
			}
			else {
				temp_str[i++] = c;
			}
		}
	} else {

		switch (c) {
		case '\033':	
		case '\177':	
		case '\t':	
			{
				break;
			}
		case '@':
		case 'U'-'@': 	
			{
				i=0;
				break;
			}
		case 'W'-'@': 	
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
		case '\r':	
		case '\n':	
				c = '\r';  
				
		default: 
			{
				if (i >= 500) {
					((struct duart *)0x200000)->b_data=0x08,nap(3),((struct duart *)0x200000)->b_data=0;
				}
				else {
					temp_str[i++] = c;
				}
				break;
			}
		}

	}


	
	for( j=charPos, charPos=i; (temp_str[i++] = s[j++]) != '\0'; )
		;

	free(s);
	s = getSpace(i);
	strcpy(s,temp_str);
	t->otherValues.text.s = s;

	draw( t, offset );  

	return( charPos );
}


drawText(p,s,just,spacing,f)
	Point p;		
	register char *s;
	short just;		
	short spacing;
	Font *f;
{
	register int line;	
	register int lx;	
	register int ly;	
	register int q;
	register int m;
	int num; 		

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
		case 1:
			lx = p.x - ( strwidth(f, tempString)>>1 );
			break;
		case 0:
			lx = p.x;
			break;
		case 2:
			lx = p.x - strwidth(f, tempString);
			break;
		}
		string(f, tempString,&display, ((((short)(lx))<<16)|((unsigned short)( (p.y + ly)))), ((Code) 3));
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



# 846 ""


Word textMarker[] = {		
	 0x00000000, 0x00000000, 0x00000000, 0x00800000,
	 0x00800000, 0x01C00000, 0x01C00000, 0x03E00000,
	 0x03E00000, 0x07F00000, 0x07F00000, 0x0FF80000,
	 0x0FF80000, 0x0FF80000, 0x0FF80000, 0x00000000,
};

Word markerMask[] = {		
	 0x00000000, 0x00000000, 0x00800000, 0x01C00000,
	 0x01C00000, 0x03E00000, 0x03E00000, 0x07F00000,
	 0x07F00000, 0x0FF80000, 0x0FF80000, 0x1FFC0000,
	 0x1FFC0000, 0x1FFC0000, 0x1FFC0000, 0x1FFC0000,
};






Word	bgsave[16];	

Bitmap	markerbm = { 
		textMarker,	
		1,
		0, 0, 16, 16,
		0
	};

	int
moveTextSpot(t,offset)
 	struct thing *	t;
	Point		offset;
{
	register char *	s;
	register int 	i;	
	int		istart;	
	register int	j;	
	register int 	line;	
	register int 	x;	
	register int 	y;	
	Point		m;	
	Point		pt;	
	int		spacing;
	int		prevI;	
	int		w;	
	int 		num; 	
	short		just;	
	Font *		f;

	s = t->otherValues.text.s;
	f = t->otherValues.text.f->f;
	just = t->otherValues.text.just;
	spacing = f->height + t->otherValues.text.spacing;

	pt.x = offset.x;
	pt.y = offset.y;
	cursinhibit();

	markerbm.base = bgsave;		
	bitblt( P->layer, raddp(markerbm.rect,pt),
		&markerbm,markerbm.rect.origin, ((Code) 0) );

	prevI = -1;

	do {
		m = sub((sub(mouse.xy, ((((short)(1))<<16)|((unsigned short)(1))))),offset);

		num = numLines(s);
		x = t->origin.x;
		y = t->origin.y - ( ( spacing * (num-1) ) >> 1 );


		



		i = 0;
		line = 1;
		for (; (line < num) && (m.y >= (y + spacing)); line++) {

			while (s[i++] != '\r')
				;
			y += spacing;
		}


		

		istart = i;	

		for (j=0; (s[i] != '\r') && (s[i] != '\0'); ) {
			tempString[j++] = s[i++];
		}
		tempString[j] = '\0';


		

		w = strwidth( f, tempString);
		switch (just) {
		case 1:
			x = t->origin.x - ( w >> 1 );
			break;
		case 0:
			x = t->origin.x;
			break;
		case 2:
			x = t->origin.x - w;
			break;
		}


		i = istart;	

		

		while (( x <= m.x) && (s[i] != '\0') && (s[i] != '\r')) {
			x += f->info[s[i++]].width;    
		}

		if( i != prevI ) {
			prevI = i;

			
			markerbm.base = bgsave;
			bitblt( &markerbm, markerbm.rect, P->layer, pt, ((Code) 0) );

			pt.x = x - 9 + offset.x;
			pt.y = y + f->height - (f->height>>2) -4 + offset.y;

			
			bitblt( P->layer, raddp(markerbm.rect,pt),
				&markerbm,markerbm.rect.origin, ((Code) 0) );
			markerbm.base = markerMask;	
			bitblt( &markerbm, markerbm.rect, P->layer, pt, ((Code) 1) );
			markerbm.base = textMarker;	
			bitblt( &markerbm, markerbm.rect, P->layer, pt, ((Code) 3) );

			


		}

		nap(2);
	} while( (mouse.buttons&4) );

	
	markerbm.base = bgsave;
	bitblt( &markerbm, markerbm.rect, P->layer, pt, ((Code) 0) );

	cursallow();
	return( i );
}
