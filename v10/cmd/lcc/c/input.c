/* C compiler: input processing */

#include "c.h"

unsigned char *cp;	/* current input character */
char *file;		/* current input file name */
char *firstfile;	/* first input file */
unsigned char *limit;	/* points to last character + 1 */
char *line;		/* current line */
int lineno;		/* line number of current line */

static int infd;	/* input file descriptor */
static int bsize;	/* number of chars in last read */
static unsigned char buffer[MAXLINE+1 + BUFSIZE+1];	/* input buffer */

dclproto(static void pragma,(void));
dclproto(static void resynch,(void));

/* inputInit - initialize input processing */
void inputInit(fd) {
	limit = cp = &buffer[MAXLINE + 1];
	lineno = 0;
	file = 0;			/* omit */
	bsize = -1;
	infd = fd;
	nextline();
}

/* inputstring - arrange to read str as next input */
void inputstring(str) char *str; {
	limit = cp = &buffer[MAXLINE+1];
	while (*limit++ = *str++)
		;
	*limit = '\n';
	bsize = 0;
}

/* fillbuf - fill the input buffer, moving tail portion if necessary */
void fillbuf() {
	if (bsize == 0)
		return;
	if (limit <= cp)
		cp = &buffer[MAXLINE + 1];
	else {		/* move tail portion */
		int n = limit - cp;
		unsigned char *s = &buffer[MAXLINE + 1] - n;
		assert(s >= buffer);
		line = (char *)s - ((char *)cp - line);
		while (cp < limit)
			*s++ = *cp++;
		cp = &buffer[MAXLINE + 1] - n;
	}
	bsize = read(infd, (char *)&buffer[MAXLINE + 1], BUFSIZE);
	assert(bsize >= 0);
	limit = &buffer[MAXLINE + 1 + bsize];
	*limit = '\n';
}

/* nextline - prepare to read next line */
void nextline() {
	if (cp >= limit) {	/* refill buffer */
		fillbuf();
		if (cp >= limit) {	/* signal end of file */
			cp = limit = &buffer[MAXLINE+1];
			*limit = '\0';
		}
		if (lineno > 0)
			return;
	}
	lineno++;
	for (line = (char *)cp; *cp == ' ' || *cp == '\t'; cp++)
		;
	if (*cp == '#') {			/* omit */
		resynch();			/* omit */
		nextline();			/* omit */
	}					/* omit */
}

/* pragma - handle #pragma ref id... */
static void pragma() {
	if ((t = gettok()) == ID && strcmp(token, "ref") == 0)
		for (;;) {
			while (*cp == ' ' || *cp == '\t')
				cp++;
			if (*cp == '\n' || *cp == 0)
				break;
			if ((t = gettok()) == ID && tsym) {
				tsym->ref++;
				use(tsym, src);
			}	
		}
}

/* resynch - set line number/file name in # n [ "file" ] and #pragma ... */
static void resynch() {
	for (cp++; *cp == ' ' || *cp == '\t'; )
		cp++;
	if (limit - cp < MAXLINE)
		fillbuf();
	if (strncmp((char *)cp, "pragma", 6) == 0) {
		cp += 6;
		pragma();
	} else if (*cp >= '0' && *cp <= '9') {
	line:	for (lineno = 0; *cp >= '0' && *cp <= '9'; )
			lineno = 10*lineno + *cp++ - '0';
		lineno--;
		while (*cp == ' ' || *cp == '\t')
			cp++;
		if (*cp == '"') {
			file = (char *)++cp;
			while (*cp && *cp != '"' && *cp != '\n')
				cp++;
			file = stringn(file, (char *)cp - file);
			if (*cp == '\n')
				warning("missing \" in preprocessor line\n");
			if (firstfile == 0)
				firstfile = file;
		}
	} else if (strncmp((char *)cp, "line", 4) == 0) {
		for (cp += 4; *cp == ' ' || *cp == '\t'; )
			cp++;
		if (*cp >= '0' && *cp <= '9')
			goto line;
		if (Aflag >= 2)
			warning("unrecognized control line\n");
	} else if (Aflag >= 2 && *cp != '\n')
		warning("unrecognized control line\n");
	while (*cp)
		if (*cp++ == '\n')
			if (cp == limit + 1)
				nextline();
			else
				break;
}
