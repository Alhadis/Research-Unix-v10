/* Copyright 1989 by AT&T Bell Laboratories */
#include "tags.h"
#include "descriptor.h"

/* NOTE: all these functions use c_alloc, and modify saved_dataptr. */

extern int saved_dataptr;

int *c_alloc(siz) int siz;
{int *s=(int *)saved_dataptr;
 saved_dataptr+= 4*siz;
 return s;
}

/* Make a C string into an ML string.  Used by restartFn (through
   mak_str_lst()).
*/
mak_str(s)
char *s;
{int len = strlen(s);
 if (len == 1) return(mak_int(*(char *)s));
 else {int i = (len+3)>>2;
       int *p = 1 + c_alloc(i+1);
       p[i-1] = 0; /* allocate */
       p[-1] = (len<<width_tags) + tag_string;
       strncpy(p,s,len);
       return((int)p);
 }
}

/* Make an ML string into a C string */
char *
get_str(x)
int x;
{
 if (is_ptr(x)) return((char *)x);
 else {char *s = (char *)c_alloc(1); *(int *)s = 0; *s = x; return s;}
}

/* No good for strings/bytearrays (allocates by word, not byte).
   Also a dangerous, as the last word should be allocated first for
   garbage collector safety.  Only used in bootup (run.c) and restartFn
   (through mak_str_lst()). */
mak_obj(l,t)
int l,t;
{int *p = 1 + c_alloc(l+1);
 p[-1] = mak_desc(l,t);
 return((int)p);
}

/* Take a vector of strings (like argv) and turn it into an ML string list.
   Uses mak_obj, so it is unsafe and should be used with caution.  Only
   restartFn uses it for now. */
int *
mak_str_lst(sl)
char **sl;
{
 int *list;
 if (!(*sl)) return ((int *)ML_NIL);
 list = (int *)mak_obj(2,tag_record);
 list[1] = (int)mak_str_lst(sl+1);
 list[0] = mak_str(*sl);
 return list;
}

ml_eqstr(a,b) int *a, *b;
{int alen=a[-1]>>4, blen = b[-1]>>4; int i;
 if (alen != blen) return 0;
 for(i=0; i*4<alen; i++) if (a[i]!=b[i]) return 0;
 return 1;
}

