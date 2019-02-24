/*
n10.c

Device interfaces
*/

#include "tdef.h"
#include "ext.h"
#include "fns.h"
#include <ctype.h>

struct Term t;	/* terminal characteristics */

int	dtab;
int	plotmode;
int	esct;

char	xchname[4 * (NROFFCHARS-ALPHABET)];	/* hy, em, etc. */
short	xchtab[NROFFCHARS-ALPHABET];		/* indexes into chnamep[] */
char	*codestr;
char	*chnamep = xchname;
short	*chtab = xchtab;
int	nchtab = 0;



void n_ptinit(void)
{
	int i;
	char *p, *cp;
	int nread, fd;
	char check[50];

	hmot = n_hmot;
	makem = n_makem;
	setabs = n_setabs;
	setch = n_setch;
	sethl = n_sethl;
	setht = n_setht;
	setslant = n_setslant;
	vmot = n_vmot;
	xlss = n_xlss;
	findft = n_findft;
	width = n_width;
	mchbits = n_mchbits;
	ptlead = n_ptlead;
	ptout = n_ptout;
	ptpause = n_ptpause;
	setfont = n_setfont;
	setps = n_setps;
	setwd = n_setwd;

	if ((p = getenv("NROFFTERM")) != 0)
		strcpy(devname, p);
	if (termtab[0] == 0)
		strcpy(termtab, NTERMDIR);
	if (fontdir[0] == 0)
		strcpy(fontdir, "");
	if (devname[0] == 0)
		strcpy(devname, NDEVNAME);
	pl = 11*INCH;
	po = PO;
	hyf = 0;
	ascii = 1;
	lg = 0;
	fontlab[1] = 'R';
	fontlab[2] = 'I';
	fontlab[3] = 'B';
	fontlab[4] = PAIR('B','I');
	fontlab[5] = 'D';
	bdtab[3] = 3;
	bdtab[4] = 3;

	/* hyphalg = 0;	/* for testing */

	strcat(termtab, devname);
	if ((fd = open(termtab, 0)) < 0) {
		ERROR "cannot open %s", termtab WARN;
		exit(-1);
	}

/* needs fix: */
	codestr = malloc(4000);	/* BUG!!! */

	nread = read(fd, codestr, 4000);
	close(fd);

	p = codestr;
	p = skipstr(p);		/* skip over type, could check */
	p = skipstr(p); p = getint(p, &t.bset);
	p = skipstr(p); p = getint(p, &t.breset);
	p = skipstr(p); p = getint(p, &t.Hor);
	p = skipstr(p); p = getint(p, &t.Vert);
	p = skipstr(p); p = getint(p, &t.Newline);
	p = skipstr(p); p = getint(p, &t.Char);
	p = skipstr(p); p = getint(p, &t.Em);
	p = skipstr(p); p = getint(p, &t.Halfline);
	p = skipstr(p); p = getint(p, &t.Adj);
	p = skipstr(p); p = getstr(p, t.twinit = p);
	p = skipstr(p); p = getstr(p, t.twrest = p);
	p = skipstr(p); p = getstr(p, t.twnl = p);
	p = skipstr(p); p = getstr(p, t.hlr = p);
	p = skipstr(p); p = getstr(p, t.hlf = p);
	p = skipstr(p); p = getstr(p, t.flr = p);
	p = skipstr(p); p = getstr(p, t.bdon = p);
	p = skipstr(p); p = getstr(p, t.bdoff = p);
	p = skipstr(p); p = getstr(p, t.iton = p);
	p = skipstr(p); p = getstr(p, t.itoff = p);
	p = skipstr(p); p = getstr(p, t.ploton = p);
	p = skipstr(p); p = getstr(p, t.plotoff = p);
	p = skipstr(p); p = getstr(p, t.up = p);
	p = skipstr(p); p = getstr(p, t.down = p);
	p = skipstr(p); p = getstr(p, t.right = p);
	p = skipstr(p); p = getstr(p, t.left = p);

	p = getstr(p, check);
	if (strcmp(check, "charset") != 0) {
		ERROR "device table apparently curdled" WARN;
		exit(1);
	}

	for (i = 0; i < ALPHABET; i++)
		t.width[i] = 1;	/* default ascii widths */

	i = 0;
/* this ought to be a pointer array and in place in codestr */
	cp = chnamep + 1;	/* bug if starts at 0, in setch */
	while (p < codestr + nread) {
		while (*p == ' ' || *p == '\t' || *p == '\n')
			p++;
		if (i + ALPHABET >= NROFFCHARS) {
			ERROR "too many names in charset for %s", termtab WARN;
			exit(1);
		}
		chtab[i] = cp - chnamep;	/* index, not pointer */
		*cp++ = *p++;	/* 2-char names */
		*cp++ = *p++;
		*cp++ = '\0';
		while (*p == ' ' || *p == '\t')
			p++;
		t.width[i+ALPHABET] = *p++ - '0';
		while (*p == ' ' || *p == '\t')
			p++;
		t.codetab[i] = p;
		p = getstr(p, p);	/* compress string */
		p++;
		i++;
		nchtab++;
	}

	sps = EM;
	ics = EM * 2;
	dtab = 8 * t.Em;
	for (i = 0; i < 16; i++)
		tabtab[i] = dtab * (i + 1);
	pl = 11 * INCH;
	po = PO;
	spacesz = SS;
	lss = lss1 = VS;
	ll = ll1 = lt = lt1 = LL;
	smnt = nfonts = 5;	/* R I B BI S */
	n_specnames();	/* install names like "hyphen", etc. */
	if (eqflg)
		t.Adj = t.Hor;
}

char *skipstr(char *s)	/* skip over leading space plus string */
{
	while (*s == ' ' || *s == '\t' || *s == '\n')
		s++;
	while (*s != ' ' && *s != '\t' && *s != '\n')
		if (*s++ == '\\')
			s++;
	return s;
}

char *getstr(char *s, char *t)	/* find next string in s, copy to t */
{
	int quote = 0;

	while (*s == ' ' || *s == '\t' || *s == '\n')
		s++;
	if (*s == '"') {
		s++;
		quote = 1;
	}
	for (;;) {
		if (quote && *s == '"') {
			s++;
			break;
		}
		if (!quote && (*s == ' ' || *s == '\t' || *s == '\n'))
			break;
		if (*s != '\\')
			*t++ = *s++;
		else {
			s++;	/* skip \\ */
			if (isdigit(s[0]) && isdigit(s[1]) && isdigit(s[2])) {
				*t++ = (s[0]-'0')<<6 | (s[1]-'0')<<3 | s[2]-'0';
				s += 2;
			} else if (isdigit(s[0])) {
				*t++ = *s - '0';
			} else if (*s == 'b') {
				*t++ = '\b';
			} else if (*s == 'n') {
				*t++ = '\n';
			} else if (*s == 'r') {
				*t++ = '\r';
			} else if (*s == 't') {
				*t++ = '\t';
			} else {
				*t++ = *s;
			}
			s++;
		}
	}
	*t = '\0';
	return s;
}

char *getint(char *s, int *pn)	/* find integer at s */
{
	int base;

	while (*s == ' ' || *s == '\t' || *s == '\n')
		s++;
	base = (*s == '0') ? 8 : 10;
	*pn = 0;
	while (isdigit(*s))
		*pn = base * *pn + *s++ - '0';
	return s;
}

void n_specnames(void)
{
	int	i;

	for (i = 0; spnames[i].n; i++)
		*spnames[i].n = findch(spnames[i].v);
	if (c_isalnum == 0)
		c_isalnum = NROFFCHARS;
}


int findch(char *s)	/* find char s in chnamep */
{
	int	i;

	for (i = 0; chtab[i] != 0; i++)
		if (strcmp(s, &chnamep[chtab[i]]) == 0)
			return(i + ALPHABET);
	return(0);
}

void twdone(void)
{
	if (!TROFF && t.twrest) {
		obufp = obuf;
		oputs(t.twrest);
		flusho();
		if (pipeflg) {
			pclose(ptid);
		}
		restore_tty();
	}
}


void n_ptout(Tchar i)
{
	*olinep++ = i;
	if (olinep >= &oline[LNSIZE])
		olinep--;
	if (cbits(i) != '\n')
		return;
	olinep--;
	lead += dip->blss + lss - t.Newline;
	dip->blss = 0;
	esct = esc = 0;
	if (olinep > oline) {
		move();
		ptout1();
		oputs(t.twnl);
	} else {
		lead += t.Newline;
		move();
	}
	lead += dip->alss;
	dip->alss = 0;
	olinep = oline;
}


void ptout1(void)
{
	int k;
	char *codep;
	int w, j, phyw;
	Tchar * q, i;
	static int oxfont = FT;	/* start off in roman */

	for (q = oline; q < olinep; q++) {
		i = *q;
		if (ismot(i)) {
			j = absmot(i);
			if (isnmot(i))
				j = -j;
			if (isvmot(i))
				lead += j;
			else 
				esc += j;
			continue;
		}
		if ((k = cbits(i)) <= 040) {
			switch (k) {
			case ' ': /*space*/
				esc += t.Char;
				break;
			case '\033':
			case '\007':
			case '\016':
			case '\017':
				oput(k);
				break;
			}
			continue;
		}
		phyw = w = t.Char * t.width[k];
		if (iszbit(i))
			w = 0;
		if (esc || lead)
			move();
		esct += w;
		xfont = fbits(i);
		if (xfont != oxfont) {
			switch (oxfont) {
			case ULFONT:	oputs(t.itoff); break;
			case BDFONT:	oputs(t.bdoff); break;
			case BIFONT:	oputs(t.itoff); oputs(t.bdoff); break;
			}
			switch (xfont) {
			case ULFONT:
				if (*t.iton & 0377) oputs(t.iton); break;
			case BDFONT:
				if (*t.bdon & 0377) oputs(t.bdon); break;
			case BIFONT:
				if (*t.bdon & 0377) oputs(t.bdon);
				if (*t.iton & 0377) oputs(t.iton);
				break;
			}
			oxfont = xfont;
		}
		if ((xfont == ulfont || xfont == BIFONT) && !(*t.iton & 0377)) {
			for (j = w / t.Char; j > 0; j--)
				oput('_');
			for (j = w / t.Char; j > 0; j--)
				oput('\b');
		}
		if (!(*t.bdon & 0377) && ((j = bdtab[xfont]) || xfont == BDFONT || xfont == BIFONT))
			j++;
		else
			j = 1;	/* number of overstrikes for bold */
		if (k < ALPHABET) {	/* ordinary ascii */
			oput(k);
			while (--j > 0) {
				oput('\b');
				oput(k);
			}
		} else if (k >= nchtab + ALPHABET) {
			oput(k - nchtab - ALPHABET);
		} else {
			int oj = j;
			codep = t.codetab[k-ALPHABET];
			while (*codep != 0) {
				if (*codep & 0200) {
					codep = plot(codep);
					oput(' ');
				} else {
					if (*codep == '%')	/* escape */
						codep++;
					oput(*codep);
					if (*codep == '\033')
						oput(*++codep);
					else if (*codep != '\b')
						for (j = oj; --j > 0; ) {
							oput('\b');
							oput(*codep);
						}
					codep++;
				}
			}
		}
		if (!w)
			for (j = phyw / t.Char; j > 0; j--)
				oput('\b');
	}
}


char *plot(char *x)
{
	int	i;
	char	*j, *k;

	oputs(t.ploton);
	k = x;
	if ((*k & 0377) == 0200)
		k++;
	for (; *k; k++) {
		if (*k == '%') {	/* quote char within plot mode */
			oput(*++k);
		} else if (*k & 0200) {
			if (*k & 0100) {
				if (*k & 040)
					j = t.up; 
				else 
					j = t.down;
			} else {
				if (*k & 040)
					j = t.left; 
				else 
					j = t.right;
			}
			if ((i = *k & 037) == 0) {	/* 2nd 0200 turns it off */
				++k;
				break;
			}
			while (i--)
				oputs(j);
		} else 
			oput(*k);
	}
	oputs(t.plotoff);
	return(k);
}


void move(void)
{
	int k;
	char *i, *j;
	char *p, *q;
	int iesct, dt;

	iesct = esct;
	if (esct += esc)
		i = "\0"; 
	else 
		i = "\n\0";
	j = t.hlf;
	p = t.right;
	q = t.down;
	if (lead) {
		if (lead < 0) {
			lead = -lead;
			i = t.flr;
			/*	if(!esct)i = t.flr; else i = "\0";*/
			j = t.hlr;
			q = t.up;
		}
		if (*i & 0377) {
			k = lead / t.Newline;
			lead = lead % t.Newline;
			while (k--)
				oputs(i);
		}
		if (*j & 0377) {
			k = lead / t.Halfline;
			lead = lead % t.Halfline;
			while (k--)
				oputs(j);
		} else { /* no half-line forward, not at line begining */
			k = lead / t.Newline;
			lead = lead % t.Newline;
			if (k > 0) 
				esc = esct;
			i = "\n";
			while (k--) 
				oputs(i);
		}
	}
	if (esc) {
		if (esc < 0) {
			esc = -esc;
			j = "\b";
			p = t.left;
		} else {
			j = " ";
			if (hflg)
				while ((dt = dtab - (iesct % dtab)) <= esc) {
					if (dt % t.Em)
						break;
					oput(TAB);
					esc -= dt;
					iesct += dt;
				}
		}
		k = esc / t.Em;
		esc = esc % t.Em;
		while (k--)
			oputs(j);
	}
	if ((*t.ploton & 0377) && (esc || lead)) {
		oputs(t.ploton);
		esc /= t.Hor;
		lead /= t.Vert;
		while (esc--)
			oputs(p);
		while (lead--)
			oputs(q);
		oputs(t.plotoff);
	}
	esc = lead = 0;
}


void n_ptlead(void)
{
	move();
}


void n_ptpause(void )
{
	char	junk;

	flusho();
	read(2, &junk, 1);
}
