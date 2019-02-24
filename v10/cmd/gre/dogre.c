#include	<ctype.h>
#include	"re.h"
#include	"lre.h"
#include	"hdr.h"

static void gresucc(char*, char*);	/* does reg exp match after bm */
static int reader(void*, RDFN, MATCHFN);/* plain analog of bmfind, cwfind */
static void readin(char*, char*, unsigned char**, unsigned char**);

void
dogre(Parsetype parser, char *input, char *finput, unsigned char *map, PROCFN *pprocfn, void **pat, RDFN *prdfn, MATCHFN *pmatchfn)
{
	unsigned char *lb, *le;

	readin(input, finput, &lb, &le);
	*pat = 0;
	globre = egprep(parser, lb, le, map, 1);
	*prdfn = greprd;
	*pmatchfn = grepmatch;
	if(sflag || lflag || Lflag || oneflag){
		if(vflag == 0)
			succ2fn = oneshot, failfn = count_m;
		else
			succ2fn = count, failfn = oneshot;
	} else if(cflag){
		if(vflag == 0)
			succ2fn = inc, failfn = null;
		else
			succ2fn = null, failfn = inc_m;
	} else if(vflag){
		if(bflag||nflag)
			succ2fn = count;
		else
			succ2fn = null;
		failfn = pr_m;
	} else {
		succ2fn = pr;
		if(bflag||nflag)
			failfn = count_m;
		else
			failfn = null;
	}
	if(re_lit(globre, &lb, &le)){
		*pat = (void *)re_bmcomp((char *)lb, (char *)le, map);
		*pprocfn = re_bmexec;
		succfn = gresucc;
	} else {
		if(*pat = (void *)re_recw(globre, map))
			*pprocfn = re_cwexec;
		else
			*pprocfn = reader;
		succfn = succ2fn;
	}
}

static
reader(void *UNUSED, RDFN rdfn, MATCHFN matchfn)
{
	unsigned char *b, *e;
	unsigned char *nl;
	int k;
#pragma ref UNUSED

	b = 0;
	while((k = (*rdfn)((char**)&b, (char**)&e)) > 0){
		while(nl = (unsigned char *)memchr((char *)b, '\n', e-b)){
			if(eg_match(globre, b, nl, (unsigned char **)0, (unsigned char **)0)){
				e = nl;
				if((k = (*matchfn)((char**)&b, (char**)&e)) <= 0)
					return(k);
			} else
				b = nl+1;
		}
	}
	return(k);
}

static void
readin(char *in, char *fin, unsigned char **beg, unsigned char **end)
{
	int size, n, fd, left;
	char *base, *p;

	if(in){
		if(xflag){
			size = 4+strlen(in)+1;
			*beg = (unsigned char *)malloc(size);
			if (!*beg){
				EPR "%s: can't malloc %d bytes for -x\n", progname, size);
				exit(2);
			}
			p = (char *)*beg;
			*p++ = '^';
			*p++ = '(';
			memmove(p, in, n = strlen(in));
			p += n;
			*p++ = ')';
			*p++ = '$';
			*end = (unsigned char *)p;
		} else {
			*beg = (unsigned char *)in;
			*end = *beg + strlen(in);
		}
		return;
	}
	/* we know fin is nonzero */
	if((fd = open(fin, 0)) < 0){
		perror(fin);
		exit(2);
	}
	/*
		i object to calling stat; the following crap is not painful
		and at worst involves copying twice the number of bytes.
	*/
	size = 128;
	if((base = malloc(size)) == 0){
		EPR "%s: can't malloc %d bytes for -f %s\n", progname, size, fin);
		exit(2);
	}
	left = size;
	p = base;
	if(xflag){
		*p++ = '^';
		left--;
	}
	for(; (n = read(fd, p, left)) > 0;){
		p += n;
		left -= n;
		if(left == 0){
			size *= 2;
			if((base = realloc(base, size+2)) == 0){
				EPR "%s: can't malloc %d bytes for -f %s\n", progname, size, fin);
				exit(2);
			}
			p = base+size/2;
			left = size/2;
		}
	}
	if(n < 0){
		perror(fin);
		exit(2);
	}
	close(fd);
	if(xflag){
		*p++ = '$';
	}
	*beg = (unsigned char *)base;
	*end = (unsigned char *)p;
}

static void
gresucc(char *b, char *e)
{
	if(eg_match(globre, (unsigned char*)b, (unsigned char*)(e-1), (unsigned char **)0, (unsigned char **)0))
		(*succ2fn)(b, e);
	else
		(*failfn)(b, e);
}
