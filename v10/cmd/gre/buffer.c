#include	"re.h"
#include	"lre.h"
#include	"hdr.h"

/*#define		DEBUG		/**/

/* the following ifdef aids testing of the buffering code */
#ifndef	BUFSIZE
#define	BUFSIZE	50000
#endif

#define	OVERFLOW	(BUFSIZE/10)
/*
	lines less than BUFSIZE are preserved. larger lines have at least
	BUFSIZE-OVERFLOW preserved
*/

static char rbuf[BUFSIZE+1] = "\n";
static char *buf = rbuf+1;
static char *next, *proc;
/* invariants:
	valid text in buffer:	buf <= text < next
	text to be processed:	proc <= text < next
	buf, proc always point at beginning of lines
*/

greprd(register char **b, register char **e)
{
	int n;
	int keepingsome;
	register char *p;

	if(*b == 0)			/* set up invariants */
		*b = *e = next = proc = buf;
	keepingsome = *b != *e;
again:	/* this is only used for overflowing input lines */
#ifdef	DEBUG
	fprint(2, "%d <> %d; keep=%d (%d'%.10s'..%d) proc=%d\n", next, &buf[BUFSIZE], keepingsome, *b, *b?*b:"", *e, proc);
#endif
	if(next < &buf[BUFSIZE]){
		/*
			next is fine but *b may not be set
		*/
		if(!keepingsome)
			*b = proc;
	} else {
		/*
			find a \n so we can shift the buffer
		*/
		if(keepingsome){
			for(p = *b-1; p >= buf; p--)
				if(*p == '\n') break;
			p++;
			/* the best new buffer start is p */
			if(p == buf){	/* progressing? */
	longline:
				if(!longlinewarned){
					EPR "%s: %s: warning: ", progname, curfile);
					if(bflag)
						EPR "%ld: ", nbytes/offsetunit);
					if(nflag)
						EPR "%ld: ", lnum);
					EPR "line too long (> %d chars); text skipped\n", BUFSIZE);
					longlinewarned = 1;
				}
				next -= OVERFLOW;
				noverflow += OVERFLOW;
				goto again;
			}
		} else {
			/* not keeping any; we only have to look at unprocessed */
			for(p = next-1; p >= proc; p--)
				if(*p == '\n') break;
			p++;
			if(p == buf)
				goto longline;
			*b = p;
		}
		/* process any we haven't */
		if(proc < p){
			(*failfn)(proc, p);
			proc = p;
		}
		/* move it! */
		n = p-buf;
		memcpy(buf, p, next-p);
		proc -= n;
		next -= n;
		*b -= n;
	}
	/*
		*b points to start of returned (saved) text
		next points to first available text for reading
	*/
	FLUSH;
	if((n = read(ifd, next, &buf[BUFSIZE] - next)) <= 0){
		if(proc < next){
			(*failfn)(proc, next);
		}
		proc = next;
		return(n);
	}
	next += n;
	*e = next;
#ifdef	DEBUG
fprint(2, "greprd returns %d .. %d\n", *b, *e);
#endif
	return(1);
}

grepmatch(register char **b, register char **e)
{
	char *s, *f;
	int eoffset, n, ret = 1;
#ifdef	DEBUG
fprint(2, "match! *b=%d@%d='%.100s' *e=%d@%d\n", **b, *b, *b, **e, *e);/**/
#endif
	for(s = *b; s >= proc; s--)
		if(*s == '\n')
			break;
	if(s != *b)
		s++;
	if(proc < s){
		(*failfn)(proc, s);
		proc = s;
	}
	f = *e;
	for(;;){
		for(; f < next; f++)
			if(*f == '\n')
				goto done;
		eoffset = f-s;
		if((n = greprd(&s, &f)) <= 0){
			ret = n;
			goto done;
		}
		f = s+eoffset;
	}
done:
	f++;
	if(s > f)
		abort();
	(*succfn)(s, f);
	proc = *b = f;
	*e = next;
#ifdef	DEBUG
fprint(2, "match at '%.20s'; resuming at '%.20s'@%d\n", s, f, f);/**/
#endif
	return(ret);
}

cwxrd(register char **b, register char **e)
{
	int n;

	n = greprd(b, e);
	if(n > 0){
		(*b)--;
#ifdef	DEBUG
		fprint(2, "grepxrd returns %d .. %d\n", *b, *e);
#endif
	}
	return(n);
}

cwxmatch(register char **b, register char **e)
{
	char *s, *f;
	int eoffset, n, ret = 1;

#ifdef	DEBUG
fprint(2, "cwxmatch! *b=%d@%d='%.100s' *e=%d@%d\n", **b, *b, *b, **e, *e);/**/
#endif
	for(s = *b; s >= proc; s--)
		if(*s == '\n')
			break;
	s++;
	if(proc < s){
/*
fprint(2, "cwxfail! *b=%d@%d='%.50s' *e=%d@%d\n", **b, *b, *b, **e, *e);
fprint(2, "s=%d, proc=%d, dbg.b=%d dbg.e=%d dbg.resume=%d\n", s, proc,dbg.b, dbg.e, dbg.resume);
*/
		(*failfn)(proc, s);
		proc = s;
	}
	f = *e - 1;
	for(;;){
		for(; f < next; f++)
			if(*f == '\n')
				goto done;
		eoffset = f-s;
		if((n = greprd(&s, &f)) <= 0){
			ret = n;
			goto done;
		}
		f = s+eoffset;
	}
done:
	f++;
	if(s > f)
		abort();
	(*succfn)(s, f);
	proc = *b = f;
	*e = next;
	(*b)--;
#ifdef	DEBUG
fprint(2, "cwxmatch at '%.20s'; resuming at '%.20s'@%d\n", s, f, f);/**/
#endif
	return(ret);
}

bmxmatch(register char **b, register char **e)
{
	char *s, *f;
	int eoffset, n, ret = 1;

#ifdef	DEBUG
fprint(2, "bmxmatch! *b=%d@%d='%.100s' *e=%d@%d\n", **b, *b, *b, **e, *e);/**/
#endif
	for(s = *b; s >= proc; s--)
		if(*s == '\n')
			break;
	s++;
	if(proc < s){
		(*failfn)(proc, s);
		proc = s;
	}
	f = *e - 1;
	for(;;){
		for(; f < next; f++)
			if(*f == '\n')
				goto done;
		eoffset = f-s;
		if((n = greprd(&s, &f)) <= 0){
			ret = n;
			goto done;
		}
		f = s+eoffset;
	}
done:
	f++;
	if(s > f)
		abort();
	(*((*b == s)? succfn:failfn))(s, f);
	proc = *b = f;
	*e = next;
#ifdef	DEBUG
fprint(2, "bmxmatch at '%.20s'; resuming at '%.20s'\n", s, f);/**/
#endif
	return(ret);
}
