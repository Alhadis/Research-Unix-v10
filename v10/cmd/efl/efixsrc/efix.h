#define SYMBOL 1
#define OPERATOR 2
#define STRING 3
#define CHARCONST 4
#define ARROW 5
#define FEND 6
#define Eof 256
#define TOKSIZE 127	/* max token size */

struct Tok {
	struct Tok *next, *prev;
	int inlevel;	/* == 0 except when reading includes */
	int type;
	char white[4096];
	char tok[TOKSIZE+1];
	} *T;

extern char *adjoin(), *eofmsg, *progname, *strcpy();
extern int untok, verbose;
extern void exit();
