/*
	%Z%  %M%  version %I% %Q%of %H% %T%
	Last Delta:  %G% %U% to %P%
*/

#ifdef DUMP
#include "cip.h"

extern struct macro *macroList;

char *types [] = {
  "C", "B", "E", "L", "A", "S", "T", "M"
};
char fn[] = "dumpfile";
FILE *fp;

dump (h)
struct thing *h;
{
  struct macro *ml;
  char buf [BUFSIZ];
  fontBlk *f;

  if ((fp = fopen (fn, "w")) != (FILE *) NULL) {
    fp->cp = fp->base = buf;
    fp->count = BUFSIZ;
    printThing (h);
    for (ml = macroList; ml != (struct macro *) NULL; ml=ml->next) {
      printMacro (ml);
    }
    fclose (fp);
  }
}

printThing (h)
struct thing *h;
{
  int i;
  Point *p;
  struct thing *t;

  if ((t = h) != TNULL) {
    do {
      fprintf (fp, "\n");
      fprintf (fp, "%s%d\n", types[t->type], t);
      fprintf (fp, "%d,%d\n", t->origin.x, t->origin.y);
      fprintf (fp, "%d,%d,%d,%d\n", t->bb.origin.x, t->bb.origin.y,
				    t->bb.corner.x, t->bb.corner.y);
      switch (t->type) {
	case CIRCLE: {
	  fprintf (fp, "%d\n", t->otherValues.radius);
	  break;
	}
	case BOX: {
	  fprintf (fp, "%d,%d\n", t->otherValues.corner.x,
				  t->otherValues.corner.y);
	  break;
	}
	case ELLIPSE: {
	  fprintf (fp, "%d,%d\n", t->otherValues.ellipse.ht,
	                          t->otherValues.ellipse.wid);
	  break;
	}
	case LINE: {
	  fprintf (fp, "%d,%d\n", t->otherValues.end.x,
	  			  t->otherValues.end.y);
	  break;
	}
	case ARC: {
	  fprintf (fp, "%d,%d,%d,%d\n", t->otherValues.arc.start.x,
			     	      t->otherValues.arc.start.y,
	                              t->otherValues.arc.end.x,
			     	      t->otherValues.arc.end.y);
	  break;
	}
	case SPLINE: {
	  fprintf (fp, "%d,", t->otherValues.spline.used);
	  fprintf (fp, "%d", t->otherValues.spline.size);
	  p = t->otherValues.spline.plist;
	  for (i=0; i<t->otherValues.spline.used+3; i++) {
	    fprintf (fp, ",%d,%d", p->x, p->y);
	    p++;
	  }
	  fprintf (fp, "\n");
	  break;
	}
	case TEXT: {
	  fprintf (fp, "%d,%s\n", t->otherValues.text.just,t->otherValues.text.s);
	  break;
	}
	case MACRO: {
	  fprintf (fp, "%d\n", t->otherValues.list);
	  break;
	}
      }
      fprintf (fp, "%d\n", t->next);
      fprintf (fp, "%d\n", t->last);
      t = t->next;
    } while (t != h);
  }
  fprintf (fp, "*******\n");
}

printMacro (ml)
struct macro *ml;
{
  fprintf (fp, "%d\n", ml);
  fprintf (fp, "%s\n", ml->name);
  fprintf (fp, "%d\n", ml->outName);
  fprintf (fp, "%d\n", ml->useCount);
  fprintf (fp, "%d,%d,%d,%d\n", ml->bb.origin.x, ml->bb.origin.y,
		                ml->bb.corner.x, ml->bb.corner.y);
  fprintf (fp, "%d\n", ml->next);
  fprintf (fp, "%d\n", ml->parts);
  printThing (ml->parts);
}
