/*
	%Z%  %M%  version %I% %Q%of %H% %T%
	Last Delta:  %G% %U% to %P%
*/

#ifdef DUMP
#include "cip.h"

extern struct macro *macroList;

char *types [] = {
  "CIRCLE", "BOX", "ELLIPSE", "LINE", "ARC", "SPLINE", "TEXT", "MACRO"
};
char *linetypes [] = {
  "SOLID", "DASHED", "DOTTED"
};
char fn[] = "dumpfile";
FILE *fp;

dump (h)
struct thing *h;
{
  struct macro *ml;
  fontBlk *f;

  if ((fp = fopen (fn, "w")) != (FILE *) NULL) {
    fprintf (fp, "\n*******        THING   LIST        *******\n");
    sleep (60);
    printThing (0, h);
    fprintf (fp, "\n*******        MACRO   LIST        *******\n\n");
    sleep (60);
    for (ml = macroList; ml != (struct macro *) NULL; ml=ml->next) {
      printMacro (0, ml);
      fprintf (fp, "\n");
      sleep (60);
    }
    fprintf (fp, "\n*******        FONT    LIST        *******\n\n");
    sleep (60);
    if ((f = fonts) != (fontBlk *)NULL) {
      do {
	printFont (0, f);
	fprintf (fp, "\n");
	sleep (60);
      } while ((f=f->next) != fonts);
    }
    fclose (fp);
  }
}

char *_doprnt ();

printit (ind, fmt, x1)
int ind;
char *fmt;
unsigned x1;
{
  int i;

  for (i=0; i<ind; i++) {
    fprintf (fp, "   ");
  }
  _doprnt (fp, fmt, &x1);
}
printThing (ind, h)
int ind;
struct thing *h;
{
  int i;
  Point *p;
  struct thing *t;

  if ((t = h) != TNULL) {
    do {
      fprintf (fp,  "\n");
      sleep (60);
      printit (ind, "%s @ %d\n", types[t->type], t);
      printit (ind, "   origin              = {%d,%d}\n", 
			     t->origin.x, t->origin.y);
      printit (ind, "   bb                  = {%d,%d,%d,%d}\n", 
			     t->bb.origin.x, t->bb.origin.y,
			     t->bb.corner.x, t->bb.corner.y);
      switch (t->type) {
	case CIRCLE: {
	  printit (ind, "   radius              = %d\n", 
			     t->otherValues.radius);
	  break;
	}
	case BOX: {
	  printit (ind, "   corner              = {%d,%d}\n",
			     t->otherValues.corner.x,
			     t->otherValues.corner.y);
	  printit (ind, "   %s\n", linetypes[t->boorder]);
	  break;
	}
	case ELLIPSE: {
	  printit (ind, "   height	= %d\n",
			     t->otherValues.ellipse.ht);
	  printit (ind, "   width	= %d\n",
			     t->otherValues.ellipse.wid);
	  break;
	}
	case LINE: {
	  printit (ind, "   end 	= {%d,%d}\n",
			     t->otherValues.end);
	  printit (ind, "   %s\n", linetypes[t->boorder]);
	  break;
	}
	case ARC: {
	  printit (ind, "   start	= {%d,%d}\n",
			     t->otherValues.arc.start);
	  printit (ind, "   end	= {%d,%d}\n",
			     t->otherValues.arc.end);
	  break;
	}
	case SPLINE: {
	  printit (ind, "   used	= %d\n",
			     t->otherValues.spline.used);
	  printit (ind, "   size	= %d\n",
			     t->otherValues.spline.size);
	  p = t->otherValues.spline.plist;
	  printit (ind, "   PLIST @ %d\n", p);
	  for (i=0; i<t->otherValues.spline.used+3; i++) {
	    printit (ind, "      {%d,%d}\n", p->x, p->y);
	    p++;
	  }
	  break;
	}
	case TEXT: {
	  printit (ind, "   text                = %s\n",
		             t->otherValues.text.s);
	  printFont (ind, t->otherValues.text.f);
	  break;
	}
	case MACRO: {
	  printit (ind, "   list               = %d\n",
			     t->otherValues.list);
	  break;
	}
      }
      printit (ind, "   next            = %d\n", t->next);
      printit (ind, "   last            = %d\n", t->last);
      t = t->next;
    } while (t != h);
  }
}

printMacro (ind, ml)
int ind;
struct macro *ml;
{
  printit (ind, "   macro @ %d\n", ml);
  printit (ind, "   list->name          = %s\n", ml->name);
  printit (ind, "   list->outName       = %s\n", ml->outName);
  printit (ind, "   list->useCount      = %d\n", ml->useCount);
  printit (ind, "   list->bb            = {%d,%d,%d,%d}\n",
		     ml->bb.origin.x,
		     ml->bb.origin.y,
		     ml->bb.corner.x,
		     ml->bb.corner.y);
  printit (ind, "   list->next          = %d\n", ml->next);
  printit (ind, "   list->xReflect      = %d\n", ml->xReflect);
  printit (ind, "   list->yReflect      = %d\n", ml->yReflect);
  printit (ind, "   list->parts         = %d\n", ml->parts);
  printThing (ind+1, ml->parts);
}

printFont (ind, f)
int ind;
fontBlk *f;
{
  printit (ind, "   text @              = %d\n", f);
  printit (ind, "   text.f->ps          = %d\n", f->ps);
  printit (ind, "   text.f->num         = %d\n", f->num);
  printit (ind, "   text.f->useCount    = %d\n", f->useCount);
  printit (ind, "   text.f->next        = %d\n", f->next);
  printit (ind, "   text.f->last        = %d\n", f->last);
}
#endif
