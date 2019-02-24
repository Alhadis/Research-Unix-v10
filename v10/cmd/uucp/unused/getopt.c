/*	/sccs/src/cmd/uucp/s.getopt.c
	getopt.c	1.1	8/30/84 17:37:21
*/
#include "uucp.h"
VERSION(@(#)getopt.c	1.1);

/*	@(#)getopt.c	1.2	*/
/*	3.0 SID #	1.2	*/
/*LINTLIBRARY*/
#define ERR(s, c)	if(opterr){(void)fprintf(stderr, "%s%s%c\n", argv[0], s, c);}

extern int strcmp();
extern char *strchr();

int	opterr = 1;
int	optind = 1;
int	optopt;
char	*optarg;

int
getopt(argc, argv, opts)
int	argc;
char	**argv, *opts;
{
	static int sp = 1;
	register int c;
	register char *cp;

	if(sp == 1)
		if(optind >= argc ||
		   argv[optind][0] != '-' || argv[optind][1] == '\0')
			return(EOF);
		else if(strcmp(argv[optind], "--") == NULL) {
			optind++;
			return(EOF);
		}
	optopt = c = argv[optind][sp];
	if(c == ':' || (cp=strchr(opts, c)) == NULL) {
		ERR(": illegal option -- ", c);
		if(argv[optind][++sp] == '\0') {
			optind++;
			sp = 1;
		}
		return('?');
	}
	if(*++cp == ':') {
		if(argv[optind][sp+1] != '\0')
			optarg = &argv[optind++][sp+1];
		else if(++optind >= argc) {
			ERR(": option requires an argument -- ", c);
			sp = 1;
			return('?');
#ifndef NO_MINUS
		} else
			optarg = argv[optind++];
#else NO_MINUS
		} else {
			optarg = argv[optind++];
			if (*optarg == '-') {
			    ERR(": option requires an argument -- ", c);
			    sp = 1;
			    return('?');
			}
		}
#endif NO_MINUS
		sp = 1;
	} else {
		if(argv[optind][++sp] == '\0') {
			sp = 1;
			optind++;
		}
		optarg = NULL;
	}
	return(c);
}
