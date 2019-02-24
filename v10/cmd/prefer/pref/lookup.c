/*
 * plook - use keywords to look up a complete reference
 */


# include <stdio.h>
# include "streams.h"
# include "bib.h"

char *locate(), *fgets();

char 	*plib =	PLIB;		/* prefer library */
char    INDEX[maxstr];		/* name of index file */
char	*DINPUT = BIBFILE;	/* default reference database */

int     argc;
char    **argv;

#define USAGE	"plook [-p database ] [keys...]\n"


main(argcount,arglist)
int argcount;
char **arglist;
{
	char *refs;		/* references returned here */
	char keys[maxstr];	/* keys from command line or stdin */


	argc= argcount-1;
	argv= arglist+1;


	/* make path names: common file, index; get option flags */

	strcpy(INDEX,DINPUT);
	flags();
	strcat(INDEX,".i");


	/* get keys from command line and locate references */

	*keys = '\0';
	for (; argc>0 ; argc--, argv++) {
		strcat(keys,*argv);
		strcat(keys," ");
	}
	if(*keys != '\0') {
		refs = locate(keys, INDEX);
		if(refs != NULL) {
			if(*refs != NULL) {
				puts(refs);
				exit(0);
			}
			else exit(1);
		}
		else exit(1);
	}


	/* if no command line keys, take from stdin until EOF */

	while(fgets(keys, sizeof(keys), stdin)) {
		refs = locate(keys, INDEX);
		if(refs != NULL) {
			if(*refs != NULL)
				puts(refs);
			else {
				putchar('\n');
			}
		}
		else
			exit(1);
	}
	exit(0);
}

# define    operand     (strlen(*argv+2)==0 ? (argv++,argc--,*argv) : *argv+2)


/*
 *  get option flags
 *
 * -p	pathname of database file (default BIBFILE)
 */

flags()
{

	for (; argc>0 && *argv[0]=='-';  argc--,argv++) {
		switch ((*argv)[1]) {
		case 'l':
		case 'c':
			fprintf(stderr,USAGE);
			break;
		case 'p':
			strcpy(INDEX,operand);
			break;
		default:
			fprintf(stderr,USAGE);
			exit(1);
		}
	}
}
