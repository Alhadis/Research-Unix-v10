#include	<stdio.h>
#include "sed.h"

#define Read(f, buf, n)	(fflush(stdout), read(f, buf, n))
uchar *lformat();

execute(file)
uchar *file;
{
	register uchar *p1, *p2;
	register union reptr	*ipc;
	int	c;
	long	l;
	uchar	*execp;

	if (file) {
		if ((f = open(file, 0)) < 0) {
			fprintf(stderr, "sed: Can't open %s\n", file);
		}
	} else
		f = 0;

	ebp = ibuf;
	cbp = ibuf;

	if(pending) {
		ipc = pending;
		pending = 0;
		goto yes;
	}

	for(;;) {
		if((execp = gline(linebuf)) == badp) {
			close(f);
			return;
		}
		spend = execp;

		for(ipc = ptrspace; ipc->r1.command; ) {

			p1 = ipc->r1.ad1;
			p2 = ipc->r1.ad2;

			if(p1) {

				if(ipc->r1.inar) {
					if(*p2 == CEND) {
						p1 = 0;
					} else if(*p2 == CLNUM) {
						l = p2[1]&0377
							| ((p2[2]&0377)<<8)
							| ((p2[3]&0377)<<16)
							| ((p2[4]&0377)<<24);
						if(lnum > l) {
							ipc->r1.inar = 0;
							if(ipc->r1.negfl)
								goto yes;
							ipc++;
							continue;
						}
						if(lnum == l) {
							ipc->r1.inar = 0;
						}
					} else if(match(p2, 0)) {
						ipc->r1.inar = 0;
					}
				} else if(*p1 == CEND) {
					if(!dolflag) {
						if(ipc->r1.negfl)
							goto yes;
						ipc++;
						continue;
					}

				} else if(*p1 == CLNUM) {
					l = p1[1]&0377
						| ((p1[2]&0377)<<8)
						| ((p1[3]&0377)<<16)
						| ((p1[4]&0377)<<24);
					if(lnum != l) {
						if(ipc->r1.negfl)
							goto yes;
						ipc++;
						continue;
					}
					if(p2)
						ipc->r1.inar = 1;
				} else if(match(p1, 0)) {
					if(p2)
						ipc->r1.inar = 1;
				} else {
					if(ipc->r1.negfl)
						goto yes;
					ipc++;
					continue;
				}
			}

			if(ipc->r1.negfl) {
				ipc++;
				continue;
			}
	yes:
			command(ipc);

			if(delflag)
				break;

			if(jflag) {
				jflag = 0;
				if((ipc = ipc->r2.lb1) == 0) {
					ipc = ptrspace;
					break;
				}
			} else
				ipc++;

		}
		if(!nflag && !delflag) {
			for(p1 = linebuf; p1 < spend; p1++)
				putc(*p1, stdout);
			putc('\n', stdout);
		}

		if(aptr > abuf) {
			arout();
		}

		delflag = 0;

	}
}
match(expbuf, gf)
uchar	*expbuf;
{
	register uchar	*p1, *p2;
	register c;

	if(gf) {
		if(*expbuf)	return(0);
		p1 = linebuf;
		p2 = genbuf;
		while(*p1++ = *p2++);
		locs = p1 = loc2;
	} else {
		p1 = linebuf;
		locs = 0;
	}

	p2 = expbuf;
	if(*p2++) {
		loc1 = p1;
		if(*p2 == CCHR && p2[1] != *p1)
			return(0);
		return(advance(p1, p2));
	}

	/* fast check for first character */

	if(*p2 == CCHR) {
		c = p2[1];
		do {
			if(*p1 != c)
				continue;
			if(advance(p1, p2)) {
				loc1 = p1;
				return(1);
			}
		} while(*p1++);
		return(0);
	}

	do {
		if(advance(p1, p2)) {
			loc1 = p1;
			return(1);
		}
	} while(*p1++);
	return(0);
}
advance(alp, aep)
uchar	*alp, *aep;
{
	register uchar *lp, *ep, *curlp;
	uchar	c;
	uchar *bbeg;
	int	ct;

/*fprintf(stderr, "*lp = %c, %o\n*ep = %c, %o\n", *lp, *lp, *ep, *ep);	/*DEBUG*/

	lp = alp;
	ep = aep;
	for (;;) switch (*ep++) {

	case CCHR:
		if (*ep++ == *lp++)
			continue;
		return(0);

	case CDOT:
		if (*lp++)
			continue;
		return(0);

	case CNL:
	case CDOL:
		if (*lp == 0)
			continue;
		return(0);

	case CEOF:
		loc2 = lp;
		return(1);

	case CCL:
		c = *lp++;
		if(ep[c>>3] & bittab[c & 07]) {
			ep += 32;
			continue;
		}
		return(0);

	case CBRA:
		braslist[*ep++] = lp;
		continue;

	case CKET:
		braelist[*ep++] = lp;
		continue;

	case CBACK:
		bbeg = braslist[*ep];
		ct = braelist[*ep++] - bbeg;

		if(ecmp(bbeg, lp, ct)) {
			lp += ct;
			continue;
		}
		return(0);

	case CBACK|STAR:
		bbeg = braslist[*ep];
		ct = braelist[*ep++] - bbeg;
		curlp = lp;
		while(ecmp(bbeg, lp, ct))
			lp += ct;

		while(lp >= curlp) {
			if(advance(lp, ep))	return(1);
			lp -= ct;
		}
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
			c = *lp++;
		} while(ep[c>>3] & bittab[c & 07]);
		ep += 32;
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
				if(advance(lp, ep))
					return(1);
			} while(lp-- > curlp);
			return(0);
		}

		if(*ep == CBACK) {
			c = *(braslist[ep[1]]);
			do {
				if(*lp != c)
					continue;
				if(advance(lp, ep))
					return(1);
			} while(lp-- > curlp);
			return(0);
		}

		do {
			if(lp == locs)	break;
			if (advance(lp, ep))
				return(1);
		} while (lp-- > curlp);
		return(0);

	default:
		fprintf(stderr, "sed:  RE botch, %o\n", *--ep);
		exit(1);
	}
}
substitute(ipc)
union reptr	*ipc;
{
	register uchar	*oloc2;

	if(match(ipc->r1.re1, 0)) {

		sflag = 1;
		if(!ipc->r1.gfl) {
			dosub(ipc->r1.rhs);
			return(1);
		}

		oloc2 = NULL;
		do {
			if(oloc2 == loc2) {
				loc2++;
				continue;
			} else {
				dosub(ipc->r1.rhs);
				if(*loc2 == 0)
					break;
				oloc2 = loc2;
			}
		} while(match(ipc->r1.re1, 1));
		return(1);
	}
	return(0);
}

dosub(rhsbuf)
uchar	*rhsbuf;
{
	register uchar *lp, *sp, *rp;
	int c;

	lp = linebuf;
	sp = genbuf;
	rp = rhsbuf;
	while (lp < loc1)
		*sp++ = *lp++;
	while(c = *rp++) {
		if (c == '\\') {
			c = *rp++;
			if (c >= '1' && c < NBRA+'1') {
				sp = place(sp, braslist[c-'1'], braelist[c-'1']);
				continue;
			}
		} else if(c == '&') {
				sp = place(sp, loc1, loc2);
				continue;
		}
		*sp++ = c;
		if (sp >= &genbuf[LBSIZE])
			fprintf(stderr, "sed: Output line too long.\n");
	}
	lp = loc2;
	loc2 = sp - genbuf + linebuf;
	while (*sp++ = *lp++)
		if (sp >= &genbuf[LBSIZE]) {
			fprintf(stderr, "sed: Output line too long.\n");
		}
	lp = linebuf;
	sp = genbuf;
	while (*lp++ = *sp++);
	spend = lp-1;
}
uchar	*place(asp, al1, al2)
uchar	*asp, *al1, *al2;
{
	register uchar *sp, *l1, *l2;

	sp = asp;
	l1 = al1;
	l2 = al2;
	while (l1 < l2) {
		*sp++ = *l1++;
		if (sp >= &genbuf[LBSIZE])
			fprintf(stderr, "sed: Output line too long.\n");
	}
	return(sp);
}

command(ipc)
union reptr	*ipc;
{
	register int	i;
	register uchar	*p1, *p2;
	uchar	*execp;


	switch(ipc->r1.command) {

		case ACOM:
			*aptr++ = ipc;
			if(aptr >= &abuf[ABUFSIZE]) {
				fprintf(stderr, "sed: Too many appends after line %ld\n",
					lnum);
			}
			*aptr = 0;
			break;

		case CCOM:
			delflag = 1;
			if(!ipc->r1.inar || dolflag) {
				for(p1 = ipc->r1.re1; *p1; )
					putc(*p1++, stdout);
				putc('\n', stdout);
			}
			break;
		case DCOM:
			delflag++;
			break;
		case CDCOM:
			p1 = p2 = linebuf;

			while(*p1 != '\n') {
				if(*p1++ == 0) {
					delflag++;
					return;
				}
			}

			p1++;
			while(*p2++ = *p1++);
			spend = p2-1;
			jflag++;
			break;

		case EQCOM:
			fprintf(stdout, "%ld\n", lnum);
			break;

		case GCOM:
			p1 = linebuf;
			p2 = holdsp;
			while(*p1++ = *p2++);
			spend = p1-1;
			break;

		case CGCOM:
			*spend++ = '\n';
			p1 = spend;
			p2 = holdsp;
			while(*p1++ = *p2++)
				if(p1 >= lbend)
					break;
			spend = p1-1;
			break;

		case HCOM:
			p1 = holdsp;
			p2 = linebuf;
			while(*p1++ = *p2++);
			hspend = p1-1;
			break;

		case CHCOM:
			*hspend++ = '\n';
			p1 = hspend;
			p2 = linebuf;
			while(*p1++ = *p2++)
				if(p1 >= hend)
					break;
			hspend = p1-1;
			break;

		case ICOM:
			for(p1 = ipc->r1.re1; *p1; )
				putc(*p1++, stdout);
			putc('\n', stdout);
			break;

		case BCOM:
			jflag = 1;
			break;

		case LCOM:
			p1 = linebuf;
			p2 = genbuf;
			while(*p1) {
				p2 = lformat(*p1++ & 0377, p2);
				if(p2>lcomend && *p1) {
					*p2 = 0;
					fprintf(stdout, "%s\\\n", genbuf);
					p2 = genbuf;
				}
			}
			if(p2>genbuf && (p1[-1]==' '||p1[-1]=='\n'))
					p2 = lformat('\n', p2);
			*p2 = 0;
			fprintf(stdout, "%s\n", genbuf);
			break;

		case NCOM:
			if(!nflag) {
				for(p1 = linebuf; p1 < spend; p1++)
					putc(*p1, stdout);
				putc('\n', stdout);
			}

			if(aptr > abuf)
				arout();
			if((execp = gline(linebuf)) == badp) {
				pending = ipc;
				delflag = 1;
				break;
			}
			spend = execp;

			break;
		case CNCOM:
			if(aptr > abuf)
				arout();
			*spend++ = '\n';
			if((execp = gline(spend)) == badp) {
				pending = ipc;
				delflag = 1;
				break;
			}
			spend = execp;
			break;

		case PCOM:
			for(p1 = linebuf; p1 < spend; p1++)
				putc(*p1, stdout);
			putc('\n', stdout);
			break;
		case CPCOM:
	cpcom:
			for(p1 = linebuf; *p1 != '\n' && *p1 != '\0'; )
				putc(*p1++, stdout);
			putc('\n', stdout);
			break;

		case QCOM:
			if(!nflag) {
				for(p1 = linebuf; p1 < spend; p1++)
					putc(*p1, stdout);
				putc('\n', stdout);
			}
			if(aptr > abuf)	arout();
			fclose(stdout);
			lseek(f,(long)(cbp-ebp),2);
			exit(0);
		case RCOM:

			*aptr++ = ipc;
			if(aptr >= &abuf[ABUFSIZE])
				fprintf(stderr, "sed: Too many reads after line%ld\n",
					lnum);

			*aptr = 0;

			break;

		case SCOM:
			i = substitute(ipc);
			if(ipc->r1.pfl && i)
				if(ipc->r1.pfl == 1) {
					for(p1 = linebuf; p1 < spend; p1++)
						putc(*p1, stdout);
					putc('\n', stdout);
				}
				else
					goto cpcom;
			if(i && ipc->r1.fcode)
				goto wcom;
			break;

		case TCOM:
			if(sflag == 0)	break;
			sflag = 0;
			jflag = 1;
			break;

		wcom:
		case WCOM:
			fprintf(ipc->r1.fcode, "%s\n", linebuf);
			fflush(ipc->r1.fcode);
			break;
		case XCOM:
			p1 = linebuf;
			p2 = genbuf;
			while(*p2++ = *p1++);
			p1 = holdsp;
			p2 = linebuf;
			while(*p2++ = *p1++);
			spend = p2 - 1;
			p1 = genbuf;
			p2 = holdsp;
			while(*p2++ = *p1++);
			hspend = p2 - 1;
			break;

		case YCOM:
			p1 = linebuf;
			p2 = ipc->r1.re1;
			while(*p1 = p2[*p1])	p1++;
			break;
	}

}

uchar	*
gline(addr)
uchar	*addr;
{
	register uchar	*p1, *p2;
	register	c;
	sflag = 0;
	p1 = addr;
	p2 = cbp;
	for (;;) {
		if (p2 >= ebp) {
			if ((c = Read(f, ibuf, 512)) <= 0) {
				return(badp);
			}
			p2 = ibuf;
			ebp = ibuf+c;
		}
		if ((c = *p2++) == '\n') {
			if(p2 >=  ebp) {
				if((c = Read(f, ibuf, 512)) <= 0) {
					close(f);
					if(eargc == 0)
							dolflag = 1;
				}

				p2 = ibuf;
				ebp = ibuf + c;
			}
			break;
		}
		if(c)
		if(p1 < lbend)
			*p1++ = c;
	}
	lnum++;
	*p1 = 0;
	cbp = p2;

	return(p1);
}
ecmp(a, b, count)
uchar	*a, *b;
{
	while(count--)
		if(*a++ != *b++)	return(0);
	return(1);
}

arout()
{
	register uchar	*p1;
	FILE	*fi;
	uchar	c;
	int	t;

	aptr = abuf - 1;
	while(*++aptr) {
		if((*aptr)->r1.command == ACOM) {
			for(p1 = (*aptr)->r1.re1; *p1; )
				putc(*p1++, stdout);
			putc('\n', stdout);
		} else {
			if((fi = fopen((*aptr)->r1.re1, "r")) == NULL)
				continue;
			while((t = getc(fi)) != EOF) {
				c = t;
				putc(c, stdout);
			}
			fclose(fi);
		}
	}
	aptr = abuf;
	*aptr = 0;
}

uchar *
lformat(c, p)
uchar *p;
{
	int trans = 
		c=='\b'? 'b':
		c=='\t'? 't':
		c=='\n'? 'n':
		c=='\v'? 'v':
		c=='\f'? 'f':
		c=='\r'? 'r':
		c=='\\'? '\\':
		0;
	if(trans) {
		*p++ = '\\';
		*p++ = trans;
	} else if(c<040 || c>=0177) {
		*p++ = '\\';
		*p++ = ((c>>6)&07) + '0';
		*p++ = ((c>>3)&07) + '0';
		*p++ = (c&07) + '0';
	} else
		*p++ = c;
	return p;
}

