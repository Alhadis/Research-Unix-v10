/*
	%Z%  %M%  version %I% %Q%of %H% %T%
	Last Delta:  %G% %U% to %P%
*/

#define SETUP_OWN
#include "cip.h"

extern short noSpace;
extern Point PtCurrent;

#define INT long
#define ALIGN int
#define NALIGN 1
#undef BUSY
#define BUSY 1

/* This routine clears the message area and sets a pointer to */
/* the beginning of it. */

void
initMessage ()
{
  PtCurrent.x = Xtext+LW+1;
  PtCurrent.y = Ytext+LW+1;
  xtipple (&display, inset(Rect (Xmin, Ybut, XeditD-LW, Ymax),LW));
}

/* This routine displays a string at 'PtCurrent'. */

void
putMessage (str)
char *str;
{
  char c;

  while (*str != '\0') {
    jchar (*str++);
    if ((PtCurrent.x) > XeditD-CW-LW-1) {
      PtCurrent.x = Xtext+LW+1;
      PtCurrent.y += NS;
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

/* This routine allocates memory and then displays the estimated */
/* number of this that can be drawn. */

char *
getSpace (numbytes)
short numbytes;
{
  char *b;		/* Pointer to allocated memory */

  if ((b = alloc (numbytes)) == (char *) NULL)
    outOfSpace ();
  return (b);
}

/* This routine prints an error message */
/* telling the user cip is out of space. */

outOfSpace()
{
  beep();
  message("Out of Storage - PUT and QUIT");
  sleep(240);
  noSpace = 1;
}
