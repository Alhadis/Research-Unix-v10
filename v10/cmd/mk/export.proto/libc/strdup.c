/*	@(#)strdup.c	1.2	*/
/*LINTLIBRARY*/
/* string duplication
   returns pointer to a new string which is the duplicate of string
   pointed to by s1
   NULL is returned if new string can't be created
*/

#include <string.h>
#ifndef NULL
#define NULL	0
#endif

extern int strlen();
extern char *malloc();

char *
strdup(s1) 

   char * s1;

{  
   char * s2;

   s2 = malloc((unsigned) strlen(s1)+1) ;
   return(s2==NULL ? NULL : strcpy(s2,s1) );
}
