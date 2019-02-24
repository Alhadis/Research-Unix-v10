# include  "stdio.h"
# include "ctype.h"
# include "assert.h"
char *infile, *udir, *uname; extern int yylineno;
int destyet=0;
main (argc, argv)
	char *argv[];
{
/*
 * argv 1  list of users
 * argv 2  list of stories
 */
FILE *fu, *fs;
char uline[250], *strchr(), *svc();
int k;
extern FILE *yyin;
assert(argc>2);
fu = fopen(argv[1], "r");
fs = fopen(argv[2], "r");
assert(fu!=NULL);
assert(fs!=NULL);
while (fgets( uline, 250, fu))
	{
	trimnl(uline);
	uname = strchr(uline, ' ');
	assert(uname!=0);
	*uname++ = 0; /* now uline is file, uname is name */
	udir = strchr(uname, ' ');
	assert(udir!=0);
	*udir++ = 0; /* now udir is directory */
	udir = svc(udir);
	yyin = fopen(infile=uline, "r");
	yylineno=destyet=0;
	if (yyin==NULL)
		{
		fprintf(stderr, "Can't read %s\n",uline);
		continue;
		}
	k = yyparse();
	fclose(yyin);
	}
dump();
}

trimnl(s)
	char *s;
{
while (*s) s++;
if (*--s== '\n') *s=0;
}
