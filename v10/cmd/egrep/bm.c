#include	"hdr.h"

#define	LARGE	100000

static int delta0[256];
static char cmap[256];
static char *bmpat;

bmprep(pattern)
	char *pattern;
{
	register int j, patlen;

	patlen = strlen(bmpat = pattern);
	for(j = 0; j < 256; j++){
		delta0[j] = patlen;
		cmap[j] = j;
	}
	for(j = 0; j < patlen-1; j++)
		delta0[pattern[j]] = patlen-j-1;
	delta0[pattern[patlen-1]] = LARGE;
	if(iflag){
		for(j = 0; j < patlen-1; j++)
			if(islower(pattern[j]))
				delta0[toupper(pattern[j])] = patlen-j-1;
		if(islower(pattern[patlen-1]))
			delta0[toupper(pattern[patlen-1])] = LARGE;
		for(j = 'A'; j <= 'Z'; j++)
			cmap[j] = tolower(j);
	}
}

bmexecute(file)
char *file;
{
	register char *p;
	register char *endpt;
	register char *s;
	register int j;
	int len, patlen = strlen(bmpat);
	char *rdpt;
	int fd, eof, n;
	long seek;
	char *nlp, *proc, *np, *op;
	char buf[8*8192];

	if(file){
		if((fd = open(file, 0)) < 0){
			fprint(2, "%s: can't open %s\n", progname, file);
			badbotch=1;
			return;
		}
	} else
		fd = 0;
	lnum = 1;
	tln = 0;
	rdpt = buf;
	seek = 0;
	for(eof = 0; eof == 0;){
		Fflush(1);
		if((n = read(fd, rdpt, &buf[sizeof buf]-rdpt)) <= 0){
			if(rdpt == buf)
				break;		/* eof, nothing left to process */
			*rdpt = '\n';		/* terminate */
			endpt = rdpt+1;
			eof = 1;
		} else {
			for(p = &rdpt[n]; p >= rdpt;)
				if(*--p == '\n')
					break;
			if(p < rdpt){	/* line bigger than buf!! */
				rdpt = &buf[sizeof buf/2];	/* chop in half */
				continue;	/* reading */
			}
			endpt = p+1;
		}
		/*
			invariants:
				no newline between buf and rdpt[-1]
				lnum = nlines before buf
				seek = char offset of buf[0]
				last char read is rdpt[n-1]
				first char after last \n is endpt

			following is exited to three places:
			succeed:	match
			refresh:	!match (readjust ptrs and loop)

		*/
		p = buf+patlen-1;
		proc = buf-1;
	scan:
		for(;;){
			while((p += delta0[*(unsigned char *)p]) < endpt)
				;
			if(p < (buf+LARGE)){	/* no match */
				goto refresh;
			}
			p -= LARGE;
			for(j = patlen-2, s = p-1; j >= 0; j--)
				if(cmap[*s--] != bmpat[j])
					break;
			if(j < 0)	/* match!! */
				goto succeed;
		fail:
			p++;
		}
	succeed:
		if(bmegrep || !cflag || !sflag){	/* lflag doesn't matter */
			nlp = memchr(p, '\n', (endpt+1)-p);
			op = p;
			p -= patlen-1;
			while(*p != '\n')
				if(--p < buf) break;
			p++;
			if(bmegrep)
				if(legrep(p) == 0){
					p = op;
					goto fail;
				}
		}
		nsucc = 1;
		if (cflag){
			tln++;
			p = nlp+1;
			goto scan;
		} else if(sflag){
			if(scanexit) exit(0);
		} else if(lflag){
			Fprint(1, "%s\n", file);
			close(fd);
			return;
		} else {
			if (nfile > 1 && hflag)
				Fprint(1, "%s:", file);
			if (bflag)
				Fprint(1, "%ld:", (seek+(p-buf))/BLKSIZE);
			if (nflag){
				while(np = memchr(proc+1, '\n', p-proc)){
					lnum++;
					proc = np;
				}
				Fprint(1, "%ld:", lnum);
			}
			Fwrite(1, p, nlp-p+1);
			p = nlp+1;
			if(p < endpt)
				goto scan;
		}
	refresh:
		if(nflag){	/* count newlines that we haven't proc */
			while(proc = memchr(proc+1, '\n', endpt-proc))
				lnum++;
		}
		memcpy(buf, endpt, j = &rdpt[n]-endpt);
		rdpt = &buf[j];
		seek += n;
	}
done:	close(fd);
	if (cflag) {
		if (nfile > 1)
			Fprint(1, "%s:", file);
		Fprint(1, "%ld\n", tln);
	}
#ifdef	DOSTATS
	nbytes += seek;
#endif
}

/*
	isn't egrep pretty when it just has to mtach or not?
*/

legrep(p)
	register char *p;
{
	register State *cstat, *t;

	if(reinit == 1)
		clearg();
	cstat = istat;
	if(cstat->out)
		return(1);
	for(;;){
		if((t = cstat->gotofn[*(unsigned char *)p]) == 0)
			cstat = nxtst(cstat, *(unsigned char *)p);
		else
			cstat = t;
		if(cstat->out){
			return(1);
		}
		if(*p++ == RIGHT){
			return(0);
		}
	}
}
