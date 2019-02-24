/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 *
 * ship support -- a single file for easy bootstrap
 *
 * shipop newer [ reference [ file ... ] ]
 *
 *	true if reference file is newer than file ...
 *	false if any of the files do not exist
 *
 * shipop seal [ file ... ]
 *
 *	generate 32 bit multiplicative congruential PRG checksum for files
 *
 * shipop state reference [ file ... | <files ]
 *
 *	generate mam state file
 *
 * shipop time [ file ]
 *
 *	generate seconds-since-epoch time in hex for today [ file ]
 *
 * shipop xap [ file ... ]
 *
 *	restore pax self-delta files extracted by non-pax cpio
 */

#if !lint
static char id[] = "\n@(#)shipop (AT&T Bell Laboratories) 02/11/91\0\n";
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#define elements(x)	(sizeof(x)/sizeof(x[0]))

struct optab
{
	char*	name;
	int	(*func)();
};

static char*	cmd = "shipop";

static int	shipnewer();
static int	shipseal();
static int	shipstate();
static int	shiptime();
static int	shipxap();

static struct optab ops[] =
{
	"newer",	shipnewer,
	"seal",		shipseal,
	"state",	shipstate,
	"time",		shiptime,
	"xap",		shipxap,
};

static char*	op;

static int	status;

static void	error();

main(argc, argv)
int	argc;
char**	argv;
{
	register char*	s;
	register int	i;

	if (!(s = *++argv))
		exit(0);
	for (i = 0; i < elements(ops); i++)
		if (!strcmp(s, ops[i].name))
		{
			op = ops[i].name;
			exit((*ops[i].func)(argv + 1));
		}
	fprintf(stderr, "Usage: %s %s", cmd, ops[0].name);
	for (i = 1; i < elements(ops); i++)
		fprintf(stderr, " | %s", ops[i].name);
	fprintf(stderr, " [ file ... ]\n");
	exit(2);
}

static void
error(level, file, msg)
char*	file;
char*	msg;
{
	fprintf(stderr, "%s: %s", cmd, op);
	if (level == 1) fprintf(stderr, ": warning");
	if (file) fprintf(stderr, ": %s", file);
	if (msg) fprintf(stderr, ": %s", msg);
	fprintf(stderr, "\n");
	if (level > status)
	{
		if (level > 2) exit(level - 2);
		status = level;
	}
}

/*
 * shipop newer [ reference [ file ... ] ]
 */

static int
shipnewer(argv)
register char**	argv;
{
	unsigned long	ref;
	struct stat	st;

	if (!*argv || stat(*argv, &st)) return(1);
	ref = (unsigned long)st.st_mtime;
	while (*++argv)
		if (stat(*argv, &st) || ref < (unsigned long)st.st_mtime) return(1);
	return(0);
}

/*
 * shipop seal [ file ... ]
 */

#define HASHPART(h,c)	(h = (h) * 987654321L + 123456879L + (c))

static char		buf[4096];

static unsigned long	sealfile();

static int
shipseal(argv)
register char**	argv;
{
	register char*	f;
	register int	fd;
	unsigned long	s;

	s = 0;
	if (!*argv) s = sealfile("/dev/stdin", 0, s);
	else while (f = *argv++)
	{
		if (*f == '-' && !*(f + 1)) s = sealfile("/dev/stdin", 0, s);
		else if ((fd = open(f, 0)) < 0) error(2, f, "cannot read");
		else
		{
			s = sealfile(f, fd, s);
			(void)close(fd);
		}
	}
	printf("%08x\n", s);
	return(status);
}

static unsigned long
sealfile(file, fd, s)
char*		file;
int		fd;
unsigned long	s;
{
	register unsigned char*	b;
	register unsigned char*	e;
	register int		n;

	HASHPART(s, 0);
	while ((n = read(fd, buf, sizeof(buf))) > 0)
	{
		b = (unsigned char*)buf;
		e = b + n;
		while (b < e) HASHPART(s, *b++);
	}
	if (n < 0) error(2, file, "read error");
	return(s);
}

/*
 * shipop state reference [ file ... | <file-list ]
 */

static int
shipstate(argv)
register char**	argv;
{
	register char*	s;
	register int	c;
	long		ref;
	struct stat	st;

	if (!(s = *argv++) || stat(s, &st)) error(3, (char*)0, "reference file omitted");
	ref = (long)st.st_mtime;
	if (s = *argv++) do
	{
		if (!stat(s, &st))
			printf("%s %ld\n", s, (long)st.st_mtime - ref);
	} while (s = *argv++);
	else do
	{
		s = buf;
		while ((c = getchar()) != EOF && c != ' ' && c != '\n')
			if (s < buf + sizeof(buf) - 1) *s++ = c;
		if (s > buf)
		{
			*s = 0;
			if (!stat(buf, &st))
				printf("%s %ld\n", buf, (long)st.st_mtime - ref);
		}
	} while (c != EOF);
	return(status);
}

/*
 * shipop time [ file ]
 */

static int
shiptime(argv)
register char**	argv;
{
	struct stat	st;
	time_t		date;

	extern time_t	time();

	if (*argv && !stat(*argv, &st)) date = st.st_mtime;
	else time(&date);
	printf("%08x\n", (long)date);
	return(status);
}

/*
 * shipop xap [ file ... ]
 */

#define XAPHEADER	1024

#define round(x)	(((x)+(XAPHEADER*8)-1)&~((XAPHEADER*8)-1))

static void	xapfile();
static int	isdelta();

extern char*	malloc();

/* ----- enough of sfio and delta/update to do update() ----- */
#define reg	register
#define NIL(t)	((t)0)

typedef struct
{
	unsigned char*	buf;
	unsigned char*	nxt;
	unsigned char*	end;
	long		siz;
} Sfile_t;

#define sfgetc(f)	((f)->nxt<(f)->end?*(f)->nxt++:(-1))
#define sfgetu(f)	((_Sfi = sfgetc(f)) < 0 ? -1 : \
				((_Sfi&SF_MORE) ? _sfgetu(f) : (unsigned long)_Sfi))
#define sfread(f,b,n)	(((f)->nxt+=(n))<=(f)->end?(n):(-1))
#define sfseek(f,o,w)	((f)->nxt=(f)->buf+(o),(o))
#define sfsync(f)

#define SFUVALUE(v)	((v)&(SF_MORE-1))
#define SF_UBITS	7
#define SF_MORE		(1<<SF_UBITS)

static long	_Sfi;

static unsigned long
_sfgetu(f)
Sfile_t*	f;
{
	register int		c;
	register unsigned long	v;

	v = SFUVALUE(_Sfi);
	do
	{
		if ((c = sfgetc(f)) < 0) return(-1);
		v = (v << SF_UBITS) | SFUVALUE(c);
	} while (c & SF_MORE);
	return(v);
}

static int
sfmove(fr, fw, n)
Sfile_t*	fr;
Sfile_t*	fw;
register int	n;
{
	register char*	r;
	register char*	w;
	register char*	e;

	r = (char*)fr->nxt;
	w = (char*)fw->nxt;
	e = w + n;
	while (w < e) *w++ = *r++;
	fr->nxt = (unsigned char*)r;
	fw->nxt = (unsigned char*)w;
	return(n);
}

#define CODE_BIT	8
#define DELTA_ADD	(0)
#define DELTA_MOVE	(1<<(CODE_BIT-1))
#define M_MIN		4
#define C_BITS		(3)
#define C_SIZE		((1<<C_BITS)-3)
#define A_SIZE		(1 << CODE_BIT)
#define C_ADDRS		(C_SIZE)
#define C_CADDR		(C_SIZE+1)
#define C_RADDR		(C_SIZE+2)
#define C_SIZEOF(addr,indx)	(indx == C_ADDRS ? 1 : sfulen(addr))
#define C_INIT(c,cache,addrs) \
	{ cache[c=0] = 128; while((c += 1) < C_SIZE) cache[c] = (c+1)*128; \
	  addrs[c=0] = 256; while((c += 1) < A_SIZE) addrs[c] = c+256; \
	}
#define C_SET(real,indx,cache,c,addrs) \
	{ if((c += 1) >= C_SIZE) c = 0; cache[c] = real; \
	  if(indx != C_ADDRS) addrs[real&(A_SIZE-1)] = real; \
	}
#define C_REAL(real,addr,indx,caddr,cache,addrs) \
	{ real = indx == C_RADDR ? addr : \
		 indx == C_ADDRS ? addrs[addr] : \
		 indx == C_CADDR ? caddr-addr : addr+cache[indx]; \
	}
#define A_BITS		(CODE_BIT-3)
#define A_HERE(i)	((i) & ((1<<A_BITS)-2))
#define A_LOCAL		((1<<A_BITS)-1)
#define A_read(f,i,s)	((s) = A_HERE(i) ? ((i)&A_LOCAL)-1 : sfgetu(f)+A_LOCAL )

#define M_BITS		(CODE_BIT-(C_BITS+1))
#define M_LOCAL		((1<<M_BITS) + M_MIN-1)
#define M_HERE(i)	((i) & ((1<<M_BITS)-1))
#define M_sread(f,i,s)	((s) = ((s) = M_HERE(i)) ? \
				(s)+(M_MIN-1) : sfgetu(f)+M_LOCAL )
#define M_cread(i,c)	((c) = ((i)>>M_BITS) & ((1<<C_BITS)-1) )
#define M_aread(f,c,a)	((a) = (c) == C_ADDRS ? sfgetc(f) : sfgetu(f) )
#define M_READ(f,i,a,c,s) \
		((M_cread(i,c)<0 || M_aread(f,c,a)<0 || M_sread(f,i,s)<0) ? -1 : 0)

#define T_MBITS		(CODE_BIT - (A_BITS+1))
#define T_ABITS		(CODE_BIT - (T_MBITS+C_BITS+1))
#define T_TINY(i)	((i) & (((1<<T_MBITS)-1) << (C_BITS+T_ABITS)) )
#define T_MREAD(f,i,a,c,s) \
		(((c) = ((i)>>T_ABITS) & ((1<<C_BITS)-1)), \
		 ((s) = (((i)>>(C_BITS+T_ABITS))&((1<<T_MBITS)-1)) + M_MIN-1), \
		 M_aread(f,c,a) )
#define T_AREAD(i,s)	((s) = ((i) & ((1<<T_ABITS)-1)) + 1)
#define A_READ(f,i,s)	(T_TINY(i) ? T_AREAD(i,s) : A_read(f,i,s))

/* now the untouched libdelta/update.c */

/*
**	Reconstruct a target file from a source file and a delta file.
**	The delta file contain block move instructions computed by delta().
**
**	Written by Kiem-Phong Vo, 03/27/90
*/

static long supdate(fsrc,fdel,wtar,rtar,sbase,tbase)
Sfile_t		*fsrc;	/* source file */
Sfile_t		*fdel;	/* delta instruction file */
Sfile_t		*wtar;	/* target file for writing */
Sfile_t		*rtar;	/* target file for reading */
reg long	*sbase, *tbase;	/* current bases */
{
	reg int		inst;
	reg int		c;
	reg long	caddr;
	long		cache[C_SIZE], addrs[A_SIZE];
	long		n_src, n_tar;
#ifdef DEBUG
	int		n_inst = 0;
#endif
	if((n_src = sfgetu(fdel)) < 0 || (n_tar = sfgetu(fdel)) < 0)
		return -1;

	C_INIT(c,cache,addrs);
	caddr = n_src;
	while((inst = sfgetc(fdel)) >= 0)
	{
		reg long	addr, real, size;
		reg int		indx;
#ifdef DEBUG
		n_inst++;
#endif
		if(inst == 0)
		{
			if(sbase)
				*sbase += n_src;
			if(tbase)
				*tbase += n_tar;
			return caddr == (n_tar+n_src) ? n_tar : -1L;
		}

		if((inst&DELTA_MOVE) == 0)
		{	/* do add */
			if(A_READ(fdel,inst,size) < 0)
				return -1L;
			if(sfmove(fdel,wtar,size) < 0)
				return -1L;
			caddr += size;

			if(T_TINY(inst))
			{	/* read the merged move instruction */
				if(T_MREAD(fdel,inst,addr,indx,size) < 0)
					return -1;
				goto do_move;
			}
		}
		else
		{	/* move instruction */
			if(M_READ(fdel,inst,addr,indx,size) < 0)
				return -1;
		do_move:
			C_REAL(real,addr,indx,caddr,cache,addrs);
			C_SET(real,indx,cache,c,addrs);
			if(real >= n_src || !sbase)
			{	/* self-move */
				sfsync(wtar);
				real = (real-n_src) + *tbase;
				if(sfseek(rtar,real,0) < 0)
					return -1L;
				if(sfmove(rtar,wtar,size) < 0)
					return -1L;
			}
			else
			{	/* source-move */
				if(sbase)
					real += *sbase;
				if(sfseek(fsrc,real,0) < 0)
					return -1L;
				if(sfmove(fsrc,wtar,size) < 0)
					return -1L;
			}
			caddr += size;
		}
	}

	/* should never get here */
	return -1L;
}

long update(fsrc,fdel,wtar,rtar)
Sfile_t	*fsrc;	/* source stream */
Sfile_t	*fdel;	/* delta stream */
Sfile_t	*wtar;	/* write stream for target file */
Sfile_t	*rtar;	/* read stream for target file */
{
	reg long	nsrc, ntar, header, t, tar;
	char		magic[4];
	long		sbase, tbase;
	extern long	supdate();

	if(sfread(fdel,magic,4) != 4 || sfgetc(fdel) < 0)
		return -1L;

	if((nsrc = sfgetu(fdel)) < 0 || (ntar = sfgetu(fdel)) < 0 ||
	   (header = sfgetu(fdel)) < 0)
		return -1L;

/* ----- cheat here to setup wtar and rtar ----- */
	if (wtar->siz < ntar)
	{
		if (wtar->buf) free(wtar->buf);
		wtar->siz = round(ntar);
		if (!(wtar->buf = (unsigned char*)malloc(wtar->siz)))
			error(3, (char*)0, "out of space");
		rtar->siz = wtar->siz;
	}
	rtar->nxt = rtar->buf = wtar->nxt = wtar->buf;
/* ----- */
	for(tbase = sbase = tar = 0; ;)
	{	/* process all windows */
		if(header > 0)
			t = supdate(fsrc,fdel,wtar,rtar,NIL(long*),&tbase);
		else	t = supdate(fsrc,fdel,wtar,rtar,&sbase,&tbase);
		if(t >= 0)
		{
			if((tar += t) == ntar)
				break;
			else if(tar < ntar)
				continue;
		}

		/* error happened */
		ntar = -1L;
		break;
	}

	return ntar;
}
/* ----- */

static int
shipxap(argv)
char**	argv;
{
	char*	file;

	umask(0);
	while (file = *argv++) xapfile(file);
	return(status);
}

static void
xapfile(file)
char*	file;
{
	int		fd;
	long		n;
	int		m;
	struct stat	st;

	static Sfile_t	fdel;
	static Sfile_t	rtar;
	static Sfile_t	wtar;

	if ((fd = open(file, 0)) < 0)
	{
		error(2, file, "cannot read");
		return;
	}
	if (fstat(fd, &st))
	{
		error(2, file, "cannot stat");
		close(fd);
		return;
	}
	n = st.st_size;
	if (n > fdel.siz)
	{
		if (fdel.buf) free(fdel.buf);
		fdel.siz = round(n);
		if (!(fdel.buf = (unsigned char*)malloc(fdel.siz)))
			error(3, file, "out of space");
	}
	m = n > XAPHEADER ? XAPHEADER : n;
	if (read(fd, fdel.buf, m) != m)
	{
		error(2, file, "cannot read header");
		close(fd);
		return;
	}
	fdel.nxt = fdel.buf;
	fdel.end = fdel.buf + m;
	if (!isdelta(&fdel))
	{
		close(fd);
		return;
	}
	if ((n -= m) > 0)
	{
		if (read(fd, fdel.buf + m, n) != n)
		{
			error(2, file, "cannot read");
			close(fd);
			return;
		}
		fdel.end += n;
	}
	close(fd);
	if ((n = update((Sfile_t*)0, &fdel, &wtar, &rtar)) < 0)
	{
		error(2, file, "cannot update");
		return;
	}
	if ((fd = creat(file, st.st_mode&0777)) < 0)
	{
		if (chmod(file, st.st_mode | 0200) || (fd = creat(file, st.st_mode&0777)) < 0)
		{
			error(2, file, "cannot open for writing");
			return;
		}
		chmod(file, st.st_mode);
	}
	if (write(fd, wtar.buf, n) != n)
		error(2, file, "cannot write");
	close(fd);
}

static int
isdelta(f)
register Sfile_t*	f;
{
	register int	b;
	register int	c;

	if (sfgetc(f) != 'c' || sfgetc(f) != 'A') return(0);
	b = 0;
	for (;;)
	{
		if ((c = sfgetc(f)) < 0) return(0);
		if (c) b = 0;
		else
		{
			if (b) return(1);
			b = 1;
		}
	}
}
