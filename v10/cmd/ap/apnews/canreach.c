# include "stdio.h"
main (argc,argv)
    char *argv[];
{
char *s, fname[100]; int c; FILE *f;
if (argc < 2) return(1);
for (s=argv[1]; *s && *s!= '!'; s++)
    ;
if (*s == 0) return(0); /* local mail, ok */
*s=0;
sprintf (fname, "/usr/spool/uucp/STST.%.7s", argv[1]);
f = fopen(fname, "r");
if (f==NULL) return (0); /* no file, ok */
c = fgetc (f);
if (c== '3') /* uucp is happy */
    return(0);
return(1);
}
