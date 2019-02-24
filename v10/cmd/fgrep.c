static char *sccsid = "@(#)fgrep.c	4.1 (Berkeley) 10/1/80";
/*
 * fgrep -- print all lines containing any of a set of keywords
 *
 *	status returns:
 *		0 - ok, and some matches
 *		1 - ok, but no matches
 *		2 - some error
 */
/*#define	DOSTATS		/* define this to gather stats */
#ifdef	DOSTATS
char *statsexpr, *statspat;
long nlines, nbytes, nfail, ntrans0;
char statsflags[1024], *statsfptr = statsflags;
dostats();
#endif

#include "stdio.h"
# include "ctype.h"

#define	MAXSIZ 6000
#define QSIZE 400
struct words {
	char 	inp;
	char	out;
	struct	words *nst;
	struct	words *link;
	struct	words *fail;
} w[MAXSIZ], *smax, *q;

long	lnum;
int	bflag, cflag, fflag, lflag, nflag, vflag, xflag, iflag;
int	hflag	= 1;
int	sflag;
int	retcode = 0;
int	nfile;
long	blkno;
int	nsucc;
long	tln;
FILE	*wordf;
char	*argptr;

/*
 * preserve the interactive feel with stdio
 */
#define	Read(f, b, s)	(fflush(stdout), read(f, b, s))

main(argc, argv)
char **argv;
{
	while (--argc > 0 && (++argv)[0][0]=='-'){
#ifdef	DOSTATS
		*statsfptr++ = argv[0][1];
#endif
		switch (argv[0][1]) {

		case 's':
			sflag++;
			continue;

		case 'h':
			hflag = 0;
			continue;

		case 'b':
			bflag++;
			continue;

		case 'c':
			cflag++;
			continue;

		case 'e':
			argc--;
			argv++;
			goto out;

		case 'f':
			fflag++;
			continue;

		case 'l':
			lflag++;
			continue;

		case 'n':
			nflag++;
			continue;

		case 'v':
			vflag++;
			continue;

		case 'x':
			xflag++;
			continue;

		case 'i':
			iflag++;
			continue;
		default:
			fprintf(stderr, "fgrep: unknown flag\n");
			continue;
		}
	}
out:
	if (argc<=0)
		exit(2);
	if (fflag) {
		wordf = fopen(*argv, "r");
		if (wordf==NULL) {
			fprintf(stderr, "fgrep: can't open %s\n", *argv);
			exit(2);
		}
	}
	else argptr = *argv;
	argc--;
	argv++;

#ifdef	DOSTATS
	statsexpr = statspat = (char *)malloc(MAXSIZ);
	onexit(dostats);
#endif
	cgotofn();
	cfail();
	nfile = argc;
	if (argc<=0) {
		if (lflag) exit(1);
		execute((char *)NULL);
	}
	else while (--argc >= 0) {
		execute(*argv);
		argv++;
	}
	exit(retcode != 0 ? retcode : nsucc == 0);
}

# define ccomp(a,b) (iflag ? lca(a)==lca(b) : a==b)
# define lca(x) (isupper(x) ? ((x)+'a'-'A') : x)
execute(file)
char *file;
{
	register struct words *c;
	register ccount;
	register char ch;
	register char *p;
	char buf[2*BUFSIZ];
	int f;
	int failed;
	char *nlp;
	if (file) {
		if ((f = open(file, 0)) < 0) {
			fprintf(stderr, "fgrep: can't open %s\n", file);
			retcode = 2;
			return;
		}
	}
	else f = 0;
	ccount = 0;
	failed = 0;
	lnum = 1;
	tln = 0;
	blkno = 0;
	p = buf;
	nlp = p;
	c = w;
	for (;;) {
		if (--ccount <= 0) {
			if (p == &buf[2*BUFSIZ]) p = buf;
			if (p > &buf[BUFSIZ]) {
				if ((ccount = Read(f, p, &buf[2*BUFSIZ] - p)) <= 0) break;
			}
			else if ((ccount = Read(f, p, BUFSIZ)) <= 0) break;
			blkno += ccount;
		}
		nstate:
			if (ccomp(c->inp, *p)) {
#ifdef	DOSTATS
				if(c == w) ntrans0++;
#endif
				c = c->nst;
			}
			else if (c->link != 0) {
				c = c->link;
				goto nstate;
			}
			else {
#ifdef	DOSTATS
				nfail++;
#endif
				c = c->fail;
				failed = 1;
				if (c==0) {
					c = w;
					istate:
					if (ccomp(c->inp ,  *p)) {
						c = c->nst;
					}
					else if (c->link != 0) {
						c = c->link;
						goto istate;
					}
				}
				else goto nstate;
			}
		if (c->out) {
			while (*p++ != '\n') {
				if (--ccount <= 0) {
					if (p == &buf[2*BUFSIZ]) p = buf;
					if (p > &buf[BUFSIZ]) {
						if ((ccount = Read(f, p, &buf[2*BUFSIZ] - p)) <= 0) break;
					}
					else if ((ccount = Read(f, p, BUFSIZ)) <= 0) break;
					blkno += ccount;
				}
			}
			if ( (vflag && (failed == 0 || xflag == 0)) || (vflag == 0 && xflag && failed) )
				goto nomatch;
	succeed:	nsucc = 1;
			if (cflag) tln++;
			else if (sflag)
				;	/* ugh */
			else if (lflag) {
				printf("%s\n", file);
				close(f);
				return;
			}
			else {
				if (nfile > 1 && hflag) printf("%s:", file);
				if (bflag) printf("%ld:", (blkno-ccount-1)/1024);
				if (nflag) printf("%ld:", lnum);
				if (p <= nlp) {
					while (nlp < &buf[2*BUFSIZ]) putchar(*nlp++);
					nlp = buf;
				}
				while (nlp < p) putchar(*nlp++);
			}
	nomatch:	lnum++;
			nlp = p;
			c = w;
			failed = 0;
			continue;
		}
		if (*p++ == '\n')
			if (vflag) goto succeed;
			else {
				lnum++;
				nlp = p;
				c = w;
				failed = 0;
			}
	}
	close(f);
	if (cflag) {
		if (nfile > 1)
			printf("%s:", file);
		printf("%ld\n", tln);
	}
#ifdef	DOSTATS
	nbytes += blkno;
	nlines += lnum-1;
#endif
}

static int mailfd = -1;

getargc()
{
	register c;
	if (wordf)
		c = getc(wordf);
	else if ((c = *argptr++) == '\0')
		c = EOF;
#ifdef	DOSTATS
	if(c != EOF) *statsexpr++ = c;
#endif
	return(c);
}

cgotofn() {
	register c;
	register struct words *s;

	s = smax = w;
nword:	for(;;) {
		c = getargc();
		if (c==EOF)
			return;
		if (c == '\n') {
			if (xflag) {
				for(;;) {
					if (s->inp == c) {
						s = s->nst;
						break;
					}
					if (s->inp == 0) goto nenter;
					if (s->link == 0) {
						if (smax >= &w[MAXSIZ -1]) overflo();
						s->link = ++smax;
						s = smax;
						goto nenter;
					}
					s = s->link;
				}
			}
			s->out = 1;
			s = w;
		} else {
		loop:	if (s->inp == c) {
				s = s->nst;
				continue;
			}
			if (s->inp == 0) goto enter;
			if (s->link == 0) {
				if (smax >= &w[MAXSIZ - 1]) overflo();
				s->link = ++smax;
				s = smax;
				goto enter;
			}
			s = s->link;
			goto loop;
		}
	}

	enter:
	do {
		s->inp = c;
		if (smax >= &w[MAXSIZ - 1]) overflo();
		s->nst = ++smax;
		s = smax;
	} while ((c = getargc()) != '\n' && c!=EOF);
	if (xflag) {
	nenter:	s->inp = '\n';
		if (smax >= &w[MAXSIZ -1]) overflo();
		s->nst = ++smax;
	}
	smax->out = 1;
	s = w;
	if (c != EOF)
		goto nword;
}

overflo() {
	fprintf(stderr, "wordlist too large\n");
	exit(2);
}
cfail() {
	struct words *queue[QSIZE];
	struct words **front, **rear;
	struct words *state;
	register char c;
	register struct words *s;
	s = w;
	front = rear = queue;
init:	if ((s->inp) != 0) {
		*rear++ = s->nst;
		if (rear >= &queue[QSIZE - 1]) overflo();
	}
	if ((s = s->link) != 0) {
		goto init;
	}

	while (rear!=front) {
		s = *front;	/* s is next state in queue */
		if (front == &queue[QSIZE-1])
			front = queue;
		else front++;
	cloop:	if ((c = s->inp) != 0) {	/* g(s,c) != 0 */
			*rear = (q = s->nst);	/* add q = g(s,c) to queue */
			if (front < rear)
				if (rear >= &queue[QSIZE-1])
					if (front == queue) overflo();
					else rear = queue;
				else rear++;
			else
				if (++rear == front) overflo();
			state = s->fail;	/* state = f(s) */
		floop:	if (state == 0)
				state = w;
			if (state->inp == c)
				do {	/* f(q) = g(state,c) for all q links */
					q->fail = state->nst;
					if ((state->nst)->out == 1) q->out = 1;
				} while ((q = q->link) != 0);
			else if (state->link != 0) {
				state = state->link;
				goto floop;
			}
			else if ((state = state->fail) != 0)
				goto floop;	/* state = f(state) */
		}
		if ((s = s->link) != 0)
			goto cloop;
	}
}

#ifdef	DOSTATS
#include	<errno.h>
#define		NAME		"/tmp/grepdata"
dostats()
{
	umask(0);
	mailfd = open(NAME, 1);
	if((mailfd < 0) && (errno != ECONC)){
		umask(0);
		mailfd = creat(NAME, 03666);
	}
	if(mailfd >= 0){
		Finit(mailfd, (char *)0);
		Fseek(mailfd, 0L, 2);
		*statsexpr = 0;
		*statsfptr = 0;
		Fprint(mailfd, "\321fgrep:%s:%d:%d:%d:%d: %s\n", statsflags, nlines, nbytes, ntrans0, nfail, statspat);
	}
	Fflush(mailfd);
}
#endif
