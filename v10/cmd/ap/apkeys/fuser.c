# include "stdio.h"
# include "ctype.h"
# include "assert.h"
main(argc,argv)
	char *argv[];
{
/*
 * looks through password for people with file argv[1]
 * prints lines with full file name, userid, home directory
 */
FILE *f;
char pline[250], fname[100];
char *p, *s, *strchr();
assert(argc>1);
f = fopen("/etc/passwd", "r");
assert(f!=NULL);
while (fgets(pline, 250, f))
	{
	p = strchr(pline, ':');
	assert(p!=0);
	*p= 0; /* end of userid */
	p = strchr(p+1, ':'); assert(p!=0); /* end of password */
	p = strchr(p+1, ':'); assert(p!=0); /* end of user number */
	p = strchr(p+1, ':'); assert(p!=0); /* end of group number */
	p = strchr(p+1, ':'); assert(p!=0); /* end of gcos account */
	p = strchr(s=p+1, ':'); assert(p!=0); /* end of home directory */
	*p=0;
	sprintf(fname, "%s/%s", s, argv[1]);
	if (access(fname, 04)==0)
		printf("%s %s %s\n", fname, pline, s);
	}
fclose(f);
}
