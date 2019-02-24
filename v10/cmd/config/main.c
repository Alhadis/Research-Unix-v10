/*
 * main.c	1.4	81/03/09
 * Config
 *	Do system configuration for VAX/UNIX
 *		1) Build system data structures
 *		2) Build makefile
 *		3) Create header files for devices
 *	Michael Toy -- Berkeley -- 1981
 */

#include <stdio.h>
#include <ctype.h>
#include "y.tab.h"
#include "config.h"

main(argc, argv)
int argc;
char **argv;
{

    if (argc > 1) {
	if (chdir("..") < 0) {
	    perror("..");
	    exit(1);
	}
	if (chdir(argv[1]) < 0) {
	    perror(argv[1]);
	    exit(1);
	}
    }
    if (freopen(LOCAL("conf"), "r", stdin) == NULL)
    {
	perror(LOCAL("conf"));
	exit(2);
    }
    dtab = NULL;
    if (yyparse())
	exit(3);
    ioconf();			/* Print ioconf.c */
    ubglue();			/* Create ubglue.s */
    makefile();			/* build Makefile */
    headers();			/* make a lot of .h files */
    conf();			/* Create conf.c */
    execlp("make", "make", "-s", "depend", NULL);
    perror("make");
    exit(1);
}

/*
 * get_word
 *	returns EOF on end of file
 *	NULL on end of line
 *	pointer to the word otherwise
 */

char *get_word(fp)
register FILE *fp;
{
    static char line[80];
    register int ch;
    register char *cp;

    while((ch = getc(fp)) != EOF)
	if (ch != ' ' && ch != '\t')
	    break;
    if (ch == EOF)
	return WEOF;
    if (ch == '\n')
	return NULL;
    cp = line;
    *cp++ = ch;
    while((ch = getc(fp)) != EOF)
    {
	if (isspace(ch))
	    break;
	*cp++ = ch;
    }
    *cp = '\0';
    if (ch == EOF)
	return WEOF;
    ungetc(ch, fp);
    return line;
}

/*
 * change a pathname to point to where
 * our global data files live
 */

char *
gpath(file)
char *file;
{
    register char *cp;

    if ((cp = malloc(strlen(file)+8+1)) == NULL) {
	fprintf(stderr, "out of memory\n");
	exit(1);
    }
    strcpy(cp, "../conf/");
    strcat(cp, file);
    return cp;
}
