#include <stdio.h>
#include "efix.h"

/* add casts to convert efl source into acceptable C */

#define INCLMAX 4	/* max include depth */
#define INCLNAMX 128	/* max include pathname length */

FILE *fopen(), *infile;

char emsg[64], linebuf[4096], type[257];
char *adjnam, *eofmsg, *infname, *lastc = linebuf,
	*nextc = linebuf, *progname, *tstart, **xargv;
char *linelist;
long lineno, linestart;
int adjval, eexit, inlevel, lineinc = 1, rc, untok, verbose;
struct Include {
	FILE *infile;
	char *infname;
	long lineno, linestart;
	} inclstak[INCLMAX+1];
char inclname[INCLMAX][INCLNAMX];
char **te_addr, *te_value;

char *fgets(), *memset(), *strcpy();

next(eof)	/* return next input character */
		/* eof == 0 ==> return Eof on end of file */
		/* eof == 1, 2 ==> quit on end of file */
		/*	1 ==> msg controlled by eofmsg; */
		/*	2 ==> complaint of unexpected eof (mid-token) */
{

	if (nextc >= lastc) {
		if (!fgets(linebuf, sizeof(linebuf), infile)
			&& !nextfile(1)) switch(eof) {
				case 0:
					return Eof;
				case 1:
					if (eofmsg) fprintf(stderr,
						eofmsg, progname, infname);
					exit(rc);
				case 2:
					squawk("unexpected end of file");
					exit(rc);
				}
		linestart += nextc - linebuf;
		nextc = linebuf;
		lastc = linebuf + strlen(linebuf);
		lineno += lineinc;
		lineinc = lastc[-1] == '\n';
		}
	return *nextc++;
	}

#define unget(c) *--nextc = c

where()	/* report position of current token on stderr */
{
	int toff = --tstart - linebuf;
	fprintf(stderr, "%s, line %ld [char %ld] of %s:\n\t",
		progname, lineno, linestart + toff, infname);
	return toff;
	}

scream(msg)
	char *msg;
{
	eexit = 1;
	squawk(msg);
	}

squawk(msg)	/* print error message */
	char *msg;
{
	int toff = where();
	fprintf(stderr, "%s\ncontext:  %.*s >>> %.*s <<< %s", msg,
		toff, linebuf, nextc-tstart, tstart, nextc);
	rc |= 1;
	if (eexit) exit(rc);
	}

options(n)
{
	static char *opts[] = {
		"-L {put out # line comments}",
		"-e {exit at first error}",
		"-v {verbose}",
		0};
	char **o;

	fprintf(stderr, "usage: %s [-Lev] [file [file...]]\noptions:\n",
		progname);
	for(o = opts; *o; o++) fprintf(stderr, "\t%s\n", *o);
	exit(n);
	}

nextfile(loop)	/* start next file (implicitly cat input files) */
{
	char *s;
	struct Include *I;

	while(inlevel > 0) {
		fclose(infile);
		I = inclstak + --inlevel;
		if (!inlevel)
			*te_addr = te_value;
		infile = I->infile;
		infname = I->infname;
		lineno = I->lineno;
		linestart = I->linestart;
		lineinc = 0;
		if (fgets(linebuf, sizeof(linebuf), infile)) {
			nextc = linebuf;
			return 1;
			}
		}
	for(;;) {
		if (!(s = *++xargv)) {
			if (infname) return 0;
			s = "-";
			--xargv;
			}
		if (*s == '-') switch(s[1]) {
			case 0:
				infile = stdin;
				s = "/dev/stdin";
				break;
			case 'L':
				linelist = "\n#\tline %d, file \"%s\"\n";
				continue;
			case 'e':
				eexit = 1;
				continue;
			case 'v':
				verbose = 1;
				continue;
			case '?':
				options(0);
			default:
				fprintf(stderr, "\ninvalid option -%c\n", s[1]);
				options(1);
			}
		else if (!(infile = fopen(s,"r"))) {
			fprintf(stderr, "%s: can't open %s\n", progname, s);
			exit(rc+2);
			}
		if (!loop || fgets(linebuf, sizeof(linebuf), infile)) break;
		}
	infname = s;
	if (linelist) printf(linelist, 1, s);
	linestart = 0;
	nextc = linebuf;
	return 1;
	}

 void
inclopen(s)
 char *s;
{
	FILE *f;
	struct Include *I;
	char buf[INCLNAMX], *be = buf + sizeof(buf), *s1 = s;

	if (!(f = fopen(s,"r")) && *s != '/') {
		s1 = buf;
		adjoin(adjoin(buf,be,"/usr/include/"),be,s);
		f = fopen(s1,"r");
		}
	if (!f) {
		fprintf(stderr, "%s: can't open \"%s\"\n",
				progname, s1);
		scream("include failure");
		}
	I = inclstak + inlevel;
	I->infile = infile;
	I->infname = infname;
	I->lineno = lineno + lineinc;
	I->linestart = linestart + nextc - linebuf;
	lineno = linestart = 0;
	lineinc = 1;
	infname = strcpy(inclname[inlevel++], s1);
	infile = f;
	}

 void
dumpwhite()	/* kludge: dump #include white stuff */
{
	struct Tok *T1;
	if (inlevel == 1) {
		for(T1 = T->next; T1 != T; T1 = T1->next)
			if (!T1->inlevel) {
				T1->inlevel = 1;
				printf("%s%s", T1->white, T1->tok);
				}
		printf("%s", T1->white);
		}
	}			

token(eof)	/* return code for next token; T->tok := its print value */
{
	register c, ct;
	register char *Type = type;
	register char *t;
	int c1, nlsquawk;
	char *t1, *te;
	struct Tok *T1;

	if (untok) {
		c = untok;
		untok = 0;
		return c;
		}
	T = T1 = T->next;
	if (!T1->inlevel)
		printf("%s%s", T1->white, T1->tok);
	t = T1->white;
	te = t + sizeof(T1->white) - 1;

	/* kludge: allow restoration of te by nextfile() */
	te_addr = &te;
	te_value = te;

	if (inlevel)
		te = t;
 top:
	for(;;){
		for(;;) {
			c = next(eof);
			if (ct = Type[c]) break;
			if (t < te) *t++ = c;
			}
		if (c == '/') { /* start of comment? */
			if (next(2) != '*') { --nextc; break; }
			if (t < te) *t++ = '/';
			if (t < te) *t++ = '*';
			for(;;) {
				do {
					c = next(2);
					if (t < te) *t++ = c;
					} while(c != '*');
				do {
					c = next(2);
					if (t < te) *t++ = c;
					if (c == '/') goto top;
					} while(c == '*');
				}
			}
		else if (c == '#') { /* look for include "..." */
			if (t < te) *t++ = c;
			do {
				c = next(eof);
				if (t < te) *t++ = c;
				} while(!Type[c]);
			if (c != 'i') goto eat_rest;
			t1 = t;
			do {
				c = next(eof);
				if (t < te) *t++ = c;
				} while(Type[c] == SYMBOL);
			if (strncmp(t1,"nclude",6))
				goto eat_rest;
				
			if (inlevel >= INCLMAX)
				scream("includes nested too deeply");
			while(!Type[c]) {
				c = next(eof);
				if (t < te) *t++ = c;
				}
			switch(c) {
				case '"':
					c1 = c;
					break;
				case '<':
					c1 = '>';
					break;
				default:
					goto eat_rest;
				}
			t1 = t;
			do {
				c = next(2);
				if (t < te) *t++ = c;
				} while(c > ' ' && c != c1);
			if (t >= te) {
 eat_rest:
				while(c != '\n') {
					c = next(2);
					if (t < te) *t++ = c;
					}
				continue;
				}
			tstart = nextc;
			if (t - t1 > INCLNAMX)
				scream("oversize include name");
			*--t = 0;
			/* open file, put stuff on include stack */
			inclopen(t1);
			*t++ = c1;
			while(nextc < lastc) {
				if (t < te) *t++ = *nextc;
				++nextc;
				}
			*t = 0;
			dumpwhite();
			te = t = T1->white;
			nextc = lastc = linebuf;
			}
		else break;
		}
	*t = 0;
	tstart = nextc;
	t = T1->tok;
	te = t + sizeof(T1->tok) - 1;
	*t++ = c;
	switch(ct) {

		case SYMBOL:
			for(;; ++t) {
				c = next(eof);
				if (Type[c] != SYMBOL) { unget(c); break; }
				if (t >= te) {
					do 	c = next(eof);
						while (Type[c] == SYMBOL);
					unget(c);
					squawk("truncating oversize symbol");
					break;
					}
				*t = c;
				}
		break;

		case OPERATOR /* ()[],;*#<>+/:^~|&!%={}?. */:
			ct = c;
			break;

		case STRING /* " */:
			nlsquawk = 1;
			for(;;) {
				c = next(2);
				if (t < te) *t++ = c;
				switch(c) {
					case '\\':
						c = next(2);
						if (t < te) *t++ = c;
						continue;
					case '\n':
						if (nlsquawk) {
							squawk("newline within string");
							nlsquawk = 0;
							}
						continue;
					case '"':
						goto break2;
					}
				}
 break2:
			if (t >= te) squawk("oversize string");
			break;

		case CHARCONST /* ' */:
			do {
				c = next(2);
				if (t < te) *t++ = c;
				if (c == '\\') {
					c = next(2);
					if (t < te) *t++ = c;
					c = next(2);
					if (t < te) *t++ = c;
					}
				if (c == '\n') {
					squawk("newline in char constant");
					break;
					}
				} while(c != '\'');
			*t = 0;
			break;

		case ARROW /* - */:
			c = next(2);
			if (c == '>') {
				if (t < te) *t++ = c;
				}
			else {
				unget(c);
				ct = '-';
				}
			break;

		case FEND:
			t = T1->tok;
		}
	*t = 0;
	T1->inlevel = inlevel;
	return T1->type = ct;
	}
			
main(argc,argv)
	int argc;
	char **argv;
{
	int c;
	char *s;

	progname = *argv;
	xargv = argv;

	/* set up translate tables */

	for(c = ' ' + 1; c < 0x7f; c++) type[c] = SYMBOL;
	for(s = "()[],;*#<>+/:?^~|&!%={}?."; c = *s; s++) type[c] = OPERATOR;
	type['"'] = STRING;
	type['\''] = CHARCONST;
	type['-'] = ARROW;
	type[Eof] = FEND;

	if (!nextfile(0)) {
		fprintf(stderr, "%s: no (nonempty) input\n");
		options(1);
		}

	/* run */
	process();
	return rc;
	}

 char *
adjoin(t,te,s)	/* string concatenation with error-check and */
		/* pointer to null char at string's end returned */
	register char *t, *te, *s;
{
	for(;; ++s, ++t) {
		if (t >= te) {
			sprintf(emsg, "%s = %d is too small",
				adjnam, adjval);
			squawk(emsg);
			exit(rc);
			}
		if (!(*t = *s)) break;
		}
	return t;
	}
