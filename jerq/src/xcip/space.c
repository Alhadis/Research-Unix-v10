#define SETUP_OWN

#ifndef BRAM
#include "setup.h"
#endif

#include "cip.h"

#ifndef DMD630
#ifdef MPX
#include <sa.h>

#define stipple
#undef spl7
#undef splx
#include <pandora.h>
#endif
#endif

extern short noSpace;
extern Point PointCurr;
extern Texture16 textCursor;
extern int backspaceOneWord();

#define INT long
#define ALIGN int
#define NALIGN 1
#undef BUSY
#define BUSY 1
#define testbusy(p) ((INT)(p)&BUSY)
#define clearbusy(p) (union store *)((INT)(p)&~BUSY)

union store {
	struct {
	  union store *Uptr;
	  char *Uproc;
	} u;
	union store *ptr;
	ALIGN dummy[NALIGN];
	int calloc;	/*calloc clears an array of integers*/
};


/* This routine clears the message area and sets a pointer to */
/* the beginning of it. */

void
initMessage ()
{
  PointCurr.x = Xtext+LW+1;
  PointCurr.y = Ytext+LW+1;
  stipple (inset(Rect (Xmin, Ybut, XeditD-LW, Ymax),LW));
}

/* This routine displays a string at 'PointCurr'. */

void
putMessage (str)
char *str;
{
  while (*str != '\0') {
    if( *str == '\r' ) {
      PointCurr.x = Xtext+LW+1;
      PointCurr.y += NS;
      *str++;
    } else {
      jchar (*str++);
      if( PointCurr.x > (XeditD-CW-LW-1) ) {
        PointCurr.x = Xtext+LW+1;
        PointCurr.y += NS;
      }
    }
  }
}

/* This routine displays a message at the bottom of the cip layer */

void
message (str)
char *str;
{
  initMessage ();
  putMessage (str);
}


/* This routine displays given string in message box and then awaits
 * for user to edit it - terminated by return or mouse buttons.  The 
 * new string is returned in the first parameter.
 */

getMessage (str)
  char *str;
{
  register char c;
  register int i;
  Point startPoint;

  /* wait for release of all buttons */
  for(;button123();nap(2))
    ;

  startPoint = PointCurr;
  putMessage(str);
  i = strlen(str);

  cursswitch( &textCursor );
  clearKeyboard();
  changeButtons(QUITbuttons);

  do {
    wait( KBD | MOUSE );

    if (button12() ) {		/* cancel operation by nulling string */
	str[0] = '\0';
	break;
    }

    if (own() & KBD ) {
    	if ((c = kbdchar()) == '\033')	/* escape key */  break;
    	if (c == '\r')   		/* return key */  break;
    	if (i>0) {
	  /* erase present string */
	  PointCurr = startPoint;
	  putMessage(str);
	}
    	  
#ifndef DMD630
    	if (c >= 0x82 && c <= 0x89) {
    		int j,k;
    		j = c - 0x82;
    		k = 0;
    		while ((c = BRAM->pfkeys[j][k].byte) && (++k <= PFKEYSIZE)) {
    			if (i >= MAXNAMESIZE) {
    				ringbell ();
    				str[i] = '\0';
    				break;
    			}
    			else {
    				str[i++] = c;
    			}
    		}
    	} else {
#endif
    		switch (c) {
    		case '@':
    		case 'U'-'@': 
    			{
    				i=0;
    				str[0] = '\0';
    				break;
    			}
    		case 'W'-'@': 
    			{
    				i = backspaceOneWord(str,i);
    				break;
    			}
    		case '\b': 
    			{
    				str[(i>0)? --i : 0] = '\0';
    				break;
    			}
    		default: 
    			{
    				if (i >= MAXNAMESIZE) {
    					ringbell ();
    				}
    				else {
    					str[i++] = c;
    					str[i] = '\0';
    				}
    				break;
    			}
    		}
#ifndef DMD630
    	}
#endif
        PointCurr = startPoint;
	putMessage(str);
    }
  } while ( !button123() );

  /* Wait for all buttons to be released. */
  for(;button123();nap(2))
    ;
  cursSwitch ();
}


spaceRemaining (reshaped)
int reshaped;		/* Indicates layer reshaped, always display */
{
  extern int end;
  register long i;
  long sum;
  static long lastsum = 0;	/* Previous value of sum */
  register union store *p, *q;
  static char *startp;
  static char *endp = (char *)0;
  static union store *allocStore;
  char str [40];

#ifndef DMD630

/* From DMD 1.1 SGS ???
#ifndef ONEDOTOH
#undef maxaddr
#endif
*/

#ifdef MPX
  if (CORRECTSIZE) {
    if (!endp) {
      startp = (char *) ((int *)Sys[163]);
      endp = (char *) (( (maxaddr[VALMAXADDR] +
			       (int)Sys[163])/2) & 0xfffffffc );
      allocStore = (union store *) ((int *)Sys[163]);
    }

#else
  if (!endp) {
    i = (maxaddr[VALMAXADDR] * 2 + (long)(&end))/3;  /* split mem 2:1 alloc:gcalloc*/
    startp = (char *) (&end);
    endp = (char *) (i & 0xfffffffc );
    allocStore = (union store *) startp;
  }
#endif

    sum = endp - startp;
    p = allocStore;

    while (p < (q = clearbusy(p->ptr))) {
      if (testbusy (p->ptr)) {
	sum -= ((char*)q - (char *)p);
      }
      p = q;
    } 
    if ((sum != lastsum) || reshaped) {
      sprintf (str, "Number of bytes remaining: %d", sum);
      message (str);
    }
    if (sum > 0) {
      noSpace = 0;
    }
    lastsum = sum;

#ifdef MPX
  }
#endif

#endif

}


/* This routine allocates memory and then displays the estimated */
/* number of this that can be drawn. */

char *
getSpace (numbytes)
int numbytes;
{
  char *b;		/* Pointer to allocated memory */

  if (numbytes < 8) numbytes = 8;
  if ((b = alloc (numbytes)) == (char *) NULL) {
    outOfSpace ();
    return (b);
  }
  spaceRemaining (0);
  return (b);
}

/* This routine prints an error message */
/* telling the user cip is out of space. */

outOfSpace()
{
  ringbell();
  message("Out of Storage - PUT and QUIT");
  sleep(240);
  noSpace = 1;
}
