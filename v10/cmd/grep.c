/*
 * grep -- print lines matching (or not matching) a pattern
 *
 *	status returns:
 *		0 - ok, and some matches
 *		1 - ok, but no matches
 *		2 - some error
 */
/*#define	DOSTATS		/* define this to gather stats */
#ifdef	DOSTATS
char *statsexpr;
long nlines, nbytes, ndepth, nmaxdepth;
char statsflags[1024], *statsfptr = statsflags;
dostats();
#endif

#include <ctype.h>
#include	<stdio.h>
#include	<fio.h>

#define	CBRA	1
#define	CCHR	2
#define	CDOT	4
#define	CCL	6
#define	NCCL	8
#define	CDOL	10
#define	CEOF	11
#define	CKET	12
#define	CBACK	18

#define	STAR	01

#define	LBSIZE	512
#define	ESIZE	256
#define	NBRA	9

char	expbuf[ESIZE];
long	lnum;
char	stdbuf[BUFSIZ];
char	*linebuf = stdbuf;
char	ybuf[5000];
int	bflag;
int	lflag;
int	nflag;
int	cflag;
int	vflag;
int	nfile;
int	hflag	= 1;
int	sflag;
int	scanexit;
int	iflag;
int	retcode = 0;
int	circf;
long	tln;
int	nsucc;
char	*braslist[NBRA];
char	*braelist[NBRA];
char	bittab[] = {
	1,
	2,
	4,
	8,
	16,
	32,
	64,
	128
};

main(argc, argv)
char **argv;
{
	extern etext();

	while (--argc > 0 && (++argv)[0][0]=='-'){
#ifdef	DOSTATS
		*statsfptr++ = argv[0][1];
#endif
		switch (argv[0][1]) {

		case 'i':
			iflag++;
			continue;

		case 'h':
			hflag = 0;
			continue;

		case 's':
			sflag++;
			continue;

		case 'v':
			vflag++;
			continue;

		case 'b':
			bflag++;
			continue;

		case 'l':
			lflag++;
			continue;

		case 'c':
			cflag++;
			continue;

		case 'n':
			nflag++;
			continue;

		case 'e':
			--argc;
			++argv;
			goto out;

		default:
			errexit("grep: unknown flag %c\n", argv[0][1]);
			continue;
		}
	}
out:
	if (argc<=0)
		exit(2);
	if (iflag) {
		register char *p;
		for ( p = *argv; *p; p++ )
			*p = tolower(*p);
	}
	
#ifdef	DOSTATS
	statsexpr = (char *)strdup(*argv);
	onexit(dostats);
#endif
	compile(*argv);
	nfile = --argc;
	if (argc<=0) {
		scanexit = 1;
		execute("/dev/stdin");
	} else while (--argc >= 0) {
		argv++;
		scanexit = argc == 0;
		execute(*argv);
	}
	exit (retcode != 0 ? retcode : nsucc == 0);
}

compile(astr)
char *astr;
{
	register c;
	register char *ep, *sp;
	char *cstart;
	char *lastep;
	int cclcnt;
	char bracket[NBRA], *bracketp;
	char numbra;
	char neg;

	ep = expbuf;
	sp = astr;
	lastep = 0;
	bracketp = bracket;
	numbra = 0;
	if (*sp == '^') {
		circf++;
		sp++;
	}
	for (;;) {
		if (ep >= &expbuf[ESIZE])
			goto cerror;
		if ((c = *sp++) != '*')
			lastep = ep;
		switch (c) {

		case '\0':
			*ep++ = CEOF;
			return;

		case '.':
			*ep++ = CDOT;
			continue;

		case '*':
			if (lastep==0 || *lastep==CBRA || *lastep==CKET)
				goto defchar;
			*lastep |= STAR;
			continue;

		case '$':
			if (*sp != '\0')
				goto defchar;
			*ep++ = CDOL;
			continue;

		case '[':
			if(&ep[17] >= &expbuf[ESIZE])
				goto cerror;
			*ep++ = CCL;
			neg = 0;
			if((c = *sp++) == '^') {
				neg = 1;
				c = *sp++;
			}
			cstart = sp;
			do {
				if (c=='\0')
					goto cerror;
				if (c=='-' && sp>cstart && *sp!=']') {
					for (c = sp[-2]; c<*sp; c++)
						ep[c>>3] |= bittab[c&07];
					sp++;
				}
				ep[c>>3] |= bittab[c&07];
			} while((c = *sp++) != ']');
			if(neg) {
				for(cclcnt = 0; cclcnt < 16; cclcnt++)
					ep[cclcnt] ^= -1;
				ep[0] &= 0376;
			}

			ep += 16;

			continue;

		case '\\':
			if((c = *sp++) == '(') {
				if(numbra >= NBRA) {
					goto cerror;
				}
				*bracketp++ = numbra;
				*ep++ = CBRA;
				*ep++ = numbra++;
				continue;
			}
			if(c == ')') {
				if(bracketp <= bracket) {
					goto cerror;
				}
				*ep++ = CKET;
				*ep++ = *--bracketp;
				continue;
			}

			if(c >= '1' && c <= '9') {
				char *bp;
				c -= '1';
				if(c >= numbra)
					goto cerror;
				for(bp=bracket; bp<bracketp; bp++)
					if(c == *bp)
						goto cerror;	
				*ep++ = CBACK;
				*ep++ = c;
				continue;
			}

		defchar:
		default:
			*ep++ = CCHR;
			*ep++ = c;
		}
	}
    cerror:
	errexit("grep: RE error\n", (char *)0);
}

execute(file)
char *file;
{
	register char *p1, *p2;
	register c;
	register fd;

	if ((fd = open(file, 0)) < 0) {
		Fprint(2, "grep: can't open %s\n", file);
		retcode = 2;
		return;
	}
	Finit(fd, (char *)0);
	Ftie(fd, 1);
	lnum = 0;
	tln = 0;
	for (;;) {
		if (tln && lflag) {
			close(fd);
			return;
		}
		lnum++;
		if((p1 = linebuf = Frdline(fd)) == 0) {
			if (cflag) {
				if (nfile>1)
					Fprint(1, "%s:", file);
				Fprint(1, "%ld\n", tln);
			}
#ifdef	DOSTATS
			nbytes += FIOSEEK(fd);
			nlines += lnum-1;
#endif
			close(fd);
			return;
		}
		if (iflag) {
			char *s = p1;
			char *t = ybuf;
			do {
				*t++ = tolower(*s);
			} while (*s++);
			p1 = ybuf;
		}
		p2 = expbuf;
		if (circf) {
			if (advance(p1, p2))
				goto found;
			goto nfound;
		}
		/* fast check for first character */
		if (*p2==CCHR) {
			c = p2[1];
			do {
				if (*p1!=c)
					continue;
				if (advance(p1, p2))
					goto found;
			} while (*p1++);
			goto nfound;
		}
		/* regular algorithm */
		do {
			if (advance(p1, p2))
				goto found;
		} while (*p1++);
	nfound:
		if (vflag)
			succeed(file, fd);
		continue;
	found:
		if (vflag==0)
			succeed(file, fd);
	}
}

advance(lp, ep)
register char *lp, *ep;
{
	register char *curlp;
	char c;
	char *bbeg;
	int ct;

#ifdef	DOSTATS
	if(++ndepth > nmaxdepth) nmaxdepth = ndepth;
#endif
	for (;;) switch (*ep++) {

	case CCHR:
		if (*ep++ == *lp++)
			continue;
#ifdef	DOSTATS
		ndepth--;
#endif
		return(0);

	case CDOT:
		if (*lp++)
			continue;
#ifdef	DOSTATS
		ndepth--;
#endif
		return(0);

	case CDOL:
		if (*lp==0)
			continue;
#ifdef	DOSTATS
		ndepth--;
#endif
		return(0);

	case CEOF:
#ifdef	DOSTATS
		ndepth--;
#endif
		return(1);

	case CCL:
		c = *lp++ & 0177;
		if(ep[c>>3] & bittab[c & 07]) {
			ep += 16;
			continue;
		}
#ifdef	DOSTATS
		ndepth--;
#endif
		return(0);
	case CBRA:
		braslist[*ep++] = lp;
		continue;

	case CKET:
		braelist[*ep++] = lp;
		continue;

	case CBACK:
		bbeg = braslist[*ep];
		if (braelist[*ep]==0){
#ifdef	DOSTATS
			ndepth--;
#endif
			return(0);
		}
		ct = braelist[*ep++] - bbeg;
		if(ecmp(bbeg, lp, ct)) {
			lp += ct;
			continue;
		}
#ifdef	DOSTATS
		ndepth--;
#endif
		return(0);

	case CBACK|STAR:
		bbeg = braslist[*ep];
		if (braelist[*ep]==0){
#ifdef	DOSTATS
			ndepth--;
#endif
			return(0);
		}
		ct = braelist[*ep++] - bbeg;
		curlp = lp;
		while(ecmp(bbeg, lp, ct))
			lp += ct;
		while(lp >= curlp) {
			if(advance(lp, ep)){
#ifdef	DOSTATS
				ndepth--;
#endif
				return(1);
			}
			lp -= ct;
		}
#ifdef	DOSTATS
		ndepth--;
#endif
		return(0);


	case CDOT|STAR:
		curlp = lp;
		while (*lp++);
		goto star;

	case CCHR|STAR:
		curlp = lp;
		while (*lp++ == *ep);
		ep++;
		goto star;

	case CCL|STAR:
		curlp = lp;
		do {
			c = *lp++ & 0177;
		} while(ep[c>>3] & bittab[c & 07]);
		ep += 16;
		goto star;

	star:
		if(--lp == curlp) {
			continue;
		}

		if(*ep == CCHR) {
			c = ep[1];
			do {
				if(*lp != c)
					continue;
				if(advance(lp, ep)){
#ifdef	DOSTATS
					ndepth--;
#endif
					return(1);
				}
			} while(lp-- > curlp);
#ifdef	DOSTATS
			ndepth--;
#endif
			return(0);
		}

		do {
			if (advance(lp, ep)){
#ifdef	DOSTATS
				ndepth--;
#endif
				return(1);
			}
		} while (lp-- > curlp);
#ifdef	DOSTATS
		ndepth--;
#endif
		return(0);

	default:
		errexit("grep RE botch\n", (char *)0);
	}
}

succeed(f, fd)
char *f;
int fd;
{

	nsucc = 1;
	if (sflag){
		if(scanexit) exit(0);
		return;
	}
	if (cflag) {
		tln++;
		return;
	}
	if (lflag) {
		Fprint(1, "%s\n", f);
		tln++;
		return;
	}
	if (nfile > 1 && hflag)
		Fprint(1, "%s:", f);
	if (bflag)
		Fprint(1, "%ld:", (FIOSEEK(fd)-FIOLINELEN(fd)-1)/1024);
	if (nflag)
		Fprint(1, "%ld:", lnum);
	{
		register x = FIOLINELEN(fd);
		linebuf[x] = '\n';
		Fwrite(1, linebuf, x+1);
	}
}

ecmp(a, b, count)
char	*a, *b;
{
	register cc = count;
	while(cc--)
		if(*a++ != *b++)	return(0);
	return(1);
}

errexit(s, f)
char *s, *f;
{
	Fprint(2, s, f);
	exit(2);
}

#ifdef	DOSTATS
#include	<errno.h>
#define		NAME		"/tmp/grepdata"
dostats()
{
	int mailfd;

	umask(0);
	mailfd = open(NAME, 1);
	if((mailfd < 0) && (errno != ECONC)){
		umask(0);
		mailfd = creat(NAME, 03666);
	}
	if(mailfd >= 0){
		Finit(mailfd, (char *)0);
		Fseek(mailfd, 0L, 2);
		*statsfptr = 0;
		Fprint(mailfd, "\321grep:%s:%d:%d:%d: %s\n", statsflags, nlines, nbytes, nmaxdepth, statsexpr);
		Fflush(mailfd);
		close(mailfd);
	}
}
#endif
