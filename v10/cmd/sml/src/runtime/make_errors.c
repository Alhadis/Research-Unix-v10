/* Copyright 1989 by AT&T Bell Laboratories */
#include <stdio.h>
#include "tags.h"

extern int sys_nerr;
extern char *sys_errlist[];

main()
{int i;
  for(i=0;i<sys_nerr;i++)
    printf("static struct {long tag; char s[%d];} e%d = { %d, \n      \"%s\\0\\0\\0\\0\"};\n",
	    strlen(sys_errlist[i])+4,i,
	    strlen(sys_errlist[i])*power_tags+tag_string,
	    sys_errlist[i]);
	
  printf("int errstrings[]={ %d,\n", sys_nerr*power_tags+tag_array);
  for(i=0;i<sys_nerr;i++)
    printf("(int)e%d.s,\n",i);
  printf("};\n");
  exit(0);
}
