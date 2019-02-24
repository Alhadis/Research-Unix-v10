#include "re.h"
#include "lre.h"
#include "hdr.h"

#define ERR(str, chr)       if(opterr){fprint(2, "%s%s%c\n", argv[0], str, chr);}
#define	EOF	-1
#define	NULL	0
int     opterr = 1;
int     optind = 1;
int	optopt;
char    *optarg;

int
getopt(int argc, char **argv, char *opts)
{
	static int sp = 1;
	register c;
	register char *cp;

	if (sp == 1)
		if (optind >= argc ||
		   argv[optind][0] != '-' || argv[optind][1] == '\0')
			return EOF;
		else if (strcmp(argv[optind], "--") == NULL) {
			optind++;
			return EOF;
		}
	optopt = c = argv[optind][sp];
	if (c == ':' || (cp=strchr(opts, c)) == NULL) {
		ERR (": illegal option -- ", c);
		if (argv[optind][++sp] == '\0') {
			optind++;
			sp = 1;
		}
		return '?';
	}
	if (*++cp == ':') {
		if (argv[optind][sp+1] != '\0')
			optarg = &argv[optind++][sp+1];
		else if (++optind >= argc) {
			ERR (": option requires an argument -- ", c);
			sp = 1;
			return '?';
		} else
			optarg = argv[optind++];
		sp = 1;
	} else {
		if (argv[optind][++sp] == '\0') {
			sp = 1;
			optind++;
		}
		optarg = NULL;
	}
	return c;
}
