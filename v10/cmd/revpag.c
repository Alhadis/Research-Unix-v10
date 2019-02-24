#include <stdio.h>
#include <signal.h>

#define Signal(s, f)	if (signal(s, SIG_IGN) != SIG_IGN) (void)signal(s, f)

#define PARGVAL	((*argv)[2] ? *argv+2 : --argc ? *++argv : (char *)0)

#define	PUTI(n)	if (debug) fprintf(stderr," %d ",n); \
		if (index >= indend) index = getind(); \
		*index++ = 2*(n) + inrange()

#define Malloc(type,size)	((type *)malloc((size)*sizeof(type)))
#define Realloc(type,ptr,size)	((type *)realloc(ptr,(size)*sizeof(type)))

#define NLIST	32
#define BSIZE	4096
#define ISIZE	1024

typedef struct Range {
	int lo, hi;
} Range;

char *buf, *malloc(), *realloc(), tmpnam[32];

int indsize, bufsiz, nchsrc, infd, outfd, sighup();

Range *olist, *oindex; int nolist, npages;

long *indbase, *indend, *index;

main(argc,argv)
int argc; char **argv;
{
	register int nchin, lcount, nlflag; register char *src;
	long *getind(); int nlpage = 66, dcan = 0, termff = 0, debug = 0;

	Signal(SIGHUP, sighup);
	Signal(SIGINT, sighup);
	Signal(SIGQUIT, sighup);
	Signal(SIGPIPE, sighup);

	if ((buf = malloc(bufsiz = BSIZE)) == NULL)
		fatal("cannot malloc buf","");

	while (--argc > 0 && (*++argv)[0] == '-' && (*argv)[1])
		switch ((*argv)[1]) {
		case 'd':
			dcan++; break;
		case 'f':
			termff++; break;
		case 'l':
			nlpage = atoi(PARGVAL); break;
		case 'o':
			minuso(PARGVAL); break;
		case 'D':
			debug++; break;
		default:
			fatal("unknown option ",*argv);
		}
	++argc; --argv;

	if (debug) {
		register Range *l;
		for (l=olist; l->lo < l->hi; l++)
			fprintf(stderr, "[%d, %d)\n", l->lo, l->hi);
	}

	if ((infd = (argc > 1) ? Open(&argc, &argv) : 0) < 0)
		exit(0);

	if ((outfd = creat(mktemp(strcpy(tmpnam,"/tmp/revpXXXXXX")),0644)) < 0)
		fatal("cannot open ",tmpnam);

	lcount = nchin = nlflag = 0;
	if (!dcan) { PUTI(0); }
	for (;;) {
		while ((nchsrc = read(infd, buf, bufsiz)) <= 0) {
			if (infd) close(infd);
			infd = Open(&argc,&argv);
			if (infd < 0) break;
		}
		if (nchsrc <= 0) break;
		write(outfd, buf, nchsrc);
		src = buf;
		if (!dcan) do switch (*src++) {
		case '\n':
			++nchin;
			if (++lcount >= nlpage) {
				PUTI(nchin);
				lcount = 0; nlflag++;
			}
			break;
		case '\014':
			nchin++;
			if (!nlflag) {
				if (nchin) { PUTI(nchin); }
				lcount = 0;
			}
			nlflag = 0;
			break;
		default:
			nlflag = 0;
			nchin++;
		} while (--nchsrc > 0);
		else do switch (*src++) {
		case '\n':
			if (lcount > 2) { PUTI(nlflag); }
			nlflag = ++nchin;
			lcount = 1;
			break;
		case 'p':
			if (lcount == 1) lcount++;
			else lcount = 0;
			++nchin;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (lcount >= 2) lcount++;
			else lcount = 0;
			++nchin;
			break;
		default:
			lcount = 0;
			++nchin;
		} while (--nchsrc > 0);
	}
	if (!dcan && termff && lcount != 0) {
		write(outfd,"\014",1);
		nchin++;
	}
	if (dcan || !nlflag) { PUTI(nchin); }
	close(outfd); outfd = 1;
	if ((infd = open(tmpnam,0)) < 0)
		fatal("cannot reopen ",tmpnam);

	if (dcan) {
		nchin = *indbase/2;
		while (nchin > bufsiz)
			if ((buf = realloc(buf,bufsiz += BSIZE)) == NULL)
				fatal("cannot realloc buffer","");
		read(infd,buf,nchin);
		write(outfd,buf,nchin);
	}

	if (debug) fprintf(stderr,"\n");
	while (--index > indbase) {
		if (*(index-1) % 2 == 0)
			continue;
		nchin = *index/2 - *(index-1)/2;
		while (nchin > bufsiz)
			if ((buf = realloc(buf,bufsiz += BSIZE)) == NULL)
				fatal("cannot realloc buffer","");
		if (debug) fprintf(stderr,"(%d,%d) ",*(index-1)/2,nchin);
		if (nchin > 0) {
			lseek(infd, *(index-1)/2, 0);
			read(infd,buf,nchin);
			write(outfd,buf,nchin);
		}
	}

	close(infd);
	unlink(tmpnam);
	exit(0);
}

minuso(str)
char *str;
{
	register int dash = 0, i = 0, lo = 0, c;

	do switch (c = *str++) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		i = 10*i + c - '0';
		break;
	case '-':
		++dash; lo = i; i = 0;
		break;
	case ',':
	case '\0':
		merge(dash ? lo : i, i);
		dash = lo = i = 0;
		break;
	} while (c);
}

merge(lo,hi)
{
	Range new, temp; register Range *source, *dest;

	if (lo == 0) lo = 1;
	if (hi == 0) hi = 99999;

	if (lo <= hi) {
		new.lo = lo; new.hi = hi + 1;
	} else {
		new.lo = hi; new.hi = lo + 1;
	}
	if (olist == 0) {
		olist = oindex = Malloc(Range, nolist = NLIST);
		olist->lo = 0;
		olist->hi = -1;
	}
	for (dest=source=olist;;) {
		if (source->lo >= source->hi) {
			*dest++ = new;
			dest->lo = 0;
			dest->hi = -1;
			break;
		} else if (source->hi < new.lo) {
			*dest++ = *source++;
		} else if (new.hi < source->lo) {
			temp = *source++;
			*dest++ = new;
			new = temp;
		} else {
			temp.lo = min(new.lo, source->lo);
			temp.hi = max(new.hi, source->hi);
			new = temp;
			source++;
		}
	}
	if (dest - olist >= nolist)
		olist = Realloc(Range, olist, nolist += NLIST);
}

inrange()
{
	++npages;
	if (olist == 0)
		return 1;
	if (oindex == 0)
		return 0;
	for (; oindex->lo < oindex->hi; oindex++) {
		if (npages < oindex->lo)
			return 0;
		if (npages < oindex->hi)
			return 1;
	}
	oindex = 0;
	return 0;
}

Open(pargc,pargv)
int *pargc; char ***pargv;
{
	int infd = -1;
	while (--(*pargc) > 0) {
		if (strcmp("-",*++(*pargv)) == 0) infd = 0;
		else infd = open(**pargv,0);
		if (infd >= 0) break;
		fprintf(stderr,"cannot open %s\n",**pargv);
		fflush(stderr);
	}
	return infd;
}

long *
getind()
{
	register long *index;
	if (indsize == 0)
		indbase = Malloc(long, ISIZE);
	else
		indbase = Realloc(long, indbase, indsize+ISIZE);
	if (indbase == (long *)0)
		fatal("cannot allocate index","");
	index = indbase + indsize;
	indsize += ISIZE;
	indend = indbase + indsize;
	return index;
}

fatal(str1,str2)
char *str1, *str2;
{
	fprintf(stderr,"%s%s\n",str1,str2);
	if (tmpnam[0]) unlink(tmpnam);
	exit(1);
}

sighup(s)
{
	if (tmpnam[0]) unlink(tmpnam);
	exit(1);
}
