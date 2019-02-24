/*	@(#) setupterm.c: 1.2 10/31/83	(1.36	3/18/83)	*/
#include "curses.ext"
#include "../local/uparm.h"

extern	struct	term _first_term;
extern	struct	term *cur_term;

static char firststrtab[2048];
static int called_before = 0;	/* To check for first time. */
char *getenv();
char *malloc();
char ttytype[128];
#ifndef termpath
#define termpath(file) "/usr/lib/terminfo/file"
#endif
#define MAGNUM 0432

#define getshi()	getsh(ip) ; ip += 2

/*
 * "function" to get a short from a pointer.  The short is in a standard
 * format: two bytes, the first is the low order byte, the second is
 * the high order byte (base 256).  The only negative number allowed is
 * -1, which is represented as 255, 255.  This format happens to be the
 * same as the hardware on the pdp-11 and vax, making it fast and
 * convenient and small to do this on a pdp-11.
 */

#ifdef vax
#define getsh(ip)	(* (short *) ip)
#endif
#ifdef pdp11
#define getsh(ip)	(* (short *) ip)
#endif

#ifndef getsh
/*
 * Here is a more portable version, which does not assume byte ordering
 * in shorts, sign extension, etc.
 */
getsh(p)
register char *p;
{
	register int rv;
	if (*p == 0377)
		return -1;
	rv = *p++;
	rv += *p * 256;
	return rv;
}
#endif

/*
 * setupterm: low level routine to dig up terminfo from database
 * and read it in.  Parms are terminal type (0 means use getenv("TERM"),
 * file descriptor all output will go to (for ioctls), and a pointer
 * to an int into which the error return code goes (0 means to bomb
 * out with an error message if there's an error).  Thus, setupterm(0, 1, 0)
 * is a reasonable way for a simple program to set up.
 */
setupterm(term, filenum, errret)
char *term;
int filenum;	/* This is a UNIX file descriptor, not a stdio ptr. */
int *errret;
{
	char tiebuf[4096];
	char fname[128];
	register char *ip;
	register char *cp;
	int n, tfd;
	char *lcp, *ccp;
	int snames, nbools, nints, nstrs, sstrtab;
	char *strtab;

	if (term == NULL)
		term = getenv("TERM");
	if (term == NULL || *term == '\0')
		term = "unknown";
	tfd = -1;
	if (cp=getenv("TERMINFO")) {
		strcpy(fname, cp);
		cp = fname + strlen(fname);
		*cp++ = '/';
		*cp++ = *term;
		*cp++ = '/';
		strcpy(cp, term);
		tfd = open(fname, 0);
	}
	if (tfd < 0) {
		strcpy(fname, termpath(a/));
		cp = fname + strlen(fname);
		cp[-2] = *term;
		strcpy(cp, term);
		tfd = open(fname, 0);
	}

	if( tfd < 0 )
	{
		if( access( termpath( . ), 0 ) )
		{
			if( errret == 0 )
				perror( termpath( . ) );
			else
				*errret = -1;
		}
		else
		{
			if( errret == 0 )
			{
				write(2, "No such terminal: ", 18);
				write(2, term, strlen(term));
				write(2, "\r\n", 2);
			}
			else
			{
				*errret = 0;
			}
		}
		if( errret == 0 )
			exit( -2 );
		else
			return -1;
	}

	if( called_before && cur_term )		/* 2nd or more times through */
	{
		cur_term = (struct term *) malloc(sizeof (struct term));
		strtab = NULL;
	}
	else					/* First time through */
	{
		cur_term = &_first_term;
		called_before = TRUE;
		strtab = firststrtab;
	}

	if( filenum == 1 && !isatty(filenum) )	/* Allow output redirect */
	{
		filenum = 2;
	}
	cur_term -> Filedes = filenum;
	def_shell_mode();

	if (errret)
		*errret = 1;
	n = read(tfd, tiebuf, sizeof tiebuf);
	close(tfd);
	if (n <= 0) {
corrupt:
		write(2, "corrupted term entry\r\n", 22);
		if (errret == 0)
			exit(-3);
		else
			return -1;
	}
	if (n == sizeof tiebuf) {
		write(2, "term entry too long\r\n", 21);
		if (errret == 0)
			exit(-4);
		else
			return -1;
	}
	cp = ttytype;
	ip = tiebuf;

	/* Pick up header */
	snames = getshi();
	if (snames != MAGNUM) {
		goto corrupt;
	}
	snames = getshi();
	nbools = getshi();
	nints = getshi();
	nstrs = getshi();
	sstrtab = getshi();
	if (strtab == NULL) {
		strtab = (char *) malloc(sstrtab);
	}

	while (snames--)
		*cp++ = *ip++;	/* Skip names of terminals */

	/*
	 * Inner blocks to share this register among two variables.
	 */
	{
		register char *sp;
		char *fp = (char *)&cur_term->Columns;
		register char s;
		for (cp= &cur_term->Auto_left_margin; nbools--; ) {
			s = *ip++;
			if (cp < fp)
				*cp++ = s;
		}
	}

	/* Force proper alignment */
	if (((unsigned int) ip) & 1)
		ip++;

	{
		register short *sp;
		short *fp = (short *)&cur_term->strs;
		register int s;

		for (sp= &cur_term->Columns; nints--; ) {
			s = getshi();
			if (sp < fp)
				*sp++ = s;
		}
	}

#ifdef JWINSIZE
	/*
	 * ioctls for Blit - you may need to #include <jioctl.h>
	 * This ioctl defines the window size and overrides what
	 * it says in terminfo.
	 */
	{
		struct winsize w;

		if (ioctl(2, JWINSIZE, &w) != -1) {
			lines = w.bytesy;
			columns = w.bytesx;
		}
	}
#endif
	lcp = getenv("LINES");
	ccp = getenv("COLUMNS");
	if (lcp)
		lines = atoi(lcp);
	if (ccp)
		columns = atoi(ccp);

	{
		register char **pp;
		char **fp = (char **)&cur_term->Filedes;

		for (pp= &cur_term->strs.Back_tab; nstrs--; ) {
			n = getshi();
			if (pp < fp) {
				if (n == -1)
					*pp++ = NULL;
				else
					*pp++ = strtab+n;
			}
		}
	}

	for (cp=strtab; sstrtab--; ) {
		*cp++ = *ip++;
	}

	/*
	 * If tabs are being expanded in software, turn this off
	 * so output won't get messed up.  Also, don't use tab
	 * or backtab, even if the terminal has them, since the
	 * user might not have hardware tabs set right.
	 */
#ifdef USG
	if ((cur_term -> Nttyb.c_oflag & TABDLY) == TAB3) {
		cur_term->Nttyb.c_oflag &= ~TABDLY;
		tab = NULL;
		back_tab = NULL;
		reset_prog_mode();
		return 0;
	}
#else
	if ((cur_term -> Nttyb.sg_flags & XTABS) == XTABS) {
		cur_term->Nttyb.sg_flags &= ~XTABS;
		tab = NULL;
		back_tab = NULL;
		reset_prog_mode();
		return 0;
	}
#endif
#ifdef DIOCSETT
	reset_prog_mode();
#endif 
#ifdef LTILDE
	ioctl(cur_term -> Filedes, TIOCLGET, &n);
	if (n & LTILDE);
		reset_prog_mode();
#endif
	return 0;
}
