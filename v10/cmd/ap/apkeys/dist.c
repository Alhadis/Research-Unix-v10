# include "stdio.h"
# include "assert.h"
# include "ctype.h"
# include "signal.h"
/*
 * select and send documents
 * keys files look like
 * *H tiles *P first paragraph *S whole story >Y write in Y
 * or |C pipe to command C or >>Y append to Y.
 * followed by keyword; keyword keyword ; keyword, keyword; keyword . keyword
 * optional !keyword for negation of appearance
 * no parens yet
 * (this order, same sentence, same file)
 */
char *fname, today[10];
int debug=0;
main(argc,argv)
	char *argv[];
{
/*
 * argv 1  list of users
 * argv 2  list of stories
 */
FILE *fs;
extern FILE *yyin;
char dline[250], *dp, *strchr();
int k;
long tm, time(); char *tx, *ctime();
extern FILE *yyin;
signal(SIGPIPE, SIG_IGN);
while (argv[1][0]=='-')
	{
	switch(argv[1][1])
		{
		case 'd': debug++; break;
		}
	argc--;argv++;
	}
assert(argc>2);
tm = time(0);
tx = ctime(&tm);
sprintf(today, "%3.3s%02.2d", tx+4,atoi(tx+8));
fs = fopen(argv[2], "r");
assert(fs!=NULL);
stread();

/* now read a data file */
fprintf(stderr, "finished stread\n");
while (fgets(dline, 200, fs))
	{
	trimnl(dline);
	yyin = fopen(dline, "r");
	fname=dline;
	if (yyin==NULL)
		continue;
	process();
	deliver();
	fclose(yyin);
	}
}
