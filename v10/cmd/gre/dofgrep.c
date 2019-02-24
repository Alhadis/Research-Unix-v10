#include	<ctype.h>
#include	"re.h"
#include	"lre.h"
#include	"hdr.h"

static int addwords(char*, re_cw*, unsigned char**, unsigned char**);

void
dofgrep(char *input, char *finput, unsigned char *map, PROCFN *pprocfn, void **pat, RDFN *prdfn, MATCHFN *pmatchfn)
{
	unsigned char *lb, *le;
	int nwords, k;
	re_cw *cw;

	*pat = (void *)(cw = re_cwinit(map));
	if(finput){
		nwords = addwords(finput, cw, &lb, &le);
	} else {
		register unsigned char *s, *e;

		nwords = 0;
		s = (unsigned char *)input;
		while(*s){
			char ch;

			for(e = s; *e && (*e != '\n'); e++)
				;
			ch = *e;
			if(xflag){
				s[-1] = '\n';
				*e = '\n';
				re_cwadd(cw, (lb = s)-1, (le = e)+1);
				*e = ch;
			} else
				re_cwadd(cw, lb = s, le = e);
			s = *e? (e+1):e;
			nwords++;
		}
		if(nwords == 1){
			s = (unsigned char *)malloc(1 + (k = le-lb));
			if (!s){
				re_error("malloc failure");
				cw->seenerror = 1;
				return;
			}
			memmove((char *)s, (char *)lb, k);
			lb = s;
			le = s+k;
			if(xflag)
				*le++ = '\n';
		}
	}
	if(nwords == 1){
		*pat = (void *)re_bmcomp((char *)lb, (char *)le, map);
		*pprocfn = re_bmexec;
		*prdfn = greprd;
		*pmatchfn = xflag ? bmxmatch : grepmatch;
	} else {
		re_cwcomp(cw);
		*pprocfn = re_cwexec;
		*prdfn = xflag? cwxrd : greprd;
		*pmatchfn = xflag ? cwxmatch : grepmatch;
	}
	if(sflag || lflag || Lflag || oneflag){
		if(vflag == 0)
			succfn = oneshot, failfn = count_m;
		else
			succfn = count, failfn = oneshot;
	} else if(cflag){
		if(vflag == 0)
			succfn = inc, failfn = null;
		else
			succfn = null, failfn = inc_m;
	} else if(vflag){
		if(bflag||nflag)
			succfn = count;
		else
			succfn = null;
		failfn = pr_m;
	} else {
		succfn = pr;
		if(bflag||nflag)
			failfn = count_m;
		else
			failfn = null;
	}
}

static
addwords(char *file, re_cw *pat, unsigned char **b, unsigned char **e)
{
	unsigned char rbuf[MAXLINE+2];
	unsigned char *buf = rbuf+1;
	int fd;
	int nwords = 0, eof, k;
	register unsigned char *nl, *nn, *end;

	if((fd = open(file, 0)) < 0){
		perror(file);
		exit(2);
	}
	rbuf[0] = '\n';
	nl = end = buf+1;
	eof = 0;
	for(;;){
		if((nn = (unsigned char *)memchr((char *)nl, '\n', end-nl)) == 0){
			if(nl == buf){
				EPR "line too long in %s\n", file);
				exit(2);
			}
			memmove((char *)buf, (char *)nl, end-nl);
			end -= nl-buf;
			nl = buf;
			k = read(fd, (char *)end, &buf[MAXLINE]-end);
			if(k > 0){
				end += k;
				continue;
			} else if(k == 0){
				if(nl == end)	/* clean read to end of file */
					break;
				eof = 1;
				*end++ = '\n';
				continue;
			} else {
				perror(file);
				exit(2);
			}
		}
		if(xflag)
			re_cwadd(pat, nl-1, nn+1);
		else
			re_cwadd(pat, nl, nn);
		nl = nn+1;
		nwords++;
		if(eof)
			break;
	}
	close(fd);
	if(nwords == 1){
		k = strlen((char *)buf)-1;
		*b = (unsigned char *)malloc(k+1);
		if (!*b){
			re_error("malloc failed");
			pat->seenerror = 1;
			return 0;
		}
		memmove((char *)*b, (char *)buf, k);
		if(xflag)
			(*b)[k++] = '\n';
		*e = *b + k;
	}
	return(nwords);
}
