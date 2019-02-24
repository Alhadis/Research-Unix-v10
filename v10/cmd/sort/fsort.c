/* Copyright 1990, AT&T Bell Labs */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "fsort.h"

int mflag = 0;
int cflag = 0;
int keyed = 0;

extern void readin(void);
extern void dumptotemp(void);
extern void sealstack(struct rec *p);
extern void filelist(int, char**);
extern int getopt(int, char*const*, const char*);
extern char *optarg;
extern int optind;

FILE *input;
char *oname = "-";
char *tname[] = { "/usr/tmp"/*substitutable*/, "/usr/tmp", "/tmp", 0 };

char **files;
int nfiles;
char *nofiles[] = { "-", 0 };

main(int argc, char **argv)
{
	int c, n;
	static char s[3] = { '-' };
	for(;;) {
		if(optind<argc && argv[optind][0]=='+' &&
		   isdigit(argv[optind][1])) {
			optind += fieldarg(argv[optind],
					   argv[optind+1]);
			continue;
		}
		n = optind;	/* for sake of case '?' */
		c = getopt(argc,argv,"a:bcdfgik:mno:rst:uw:y:MT:");
		switch(c) {
		case '?':
			fatal("bad option", argv[n], 0);
		default:
			fatal("implementation error 1","",0);
		case 'b':
		case 'd':
		case 'f':
		case 'g':
		case 'i':
		case 'n':
		case 'M':
		case 'r':
			s[1] = c;
			fieldarg(s, 0);
			continue;
		case 'c':
			cflag++;
			continue;
		case 'm':
			mflag++;
			continue;
		case 's':
			sflag++;
			continue;
		case 'u':
			uflag++;
			sflag++;
			continue;
		case 'a':
			aflag++;
			uflag++;
			sflag++;
			optionk(optarg, accum, &naccum);
			continue;
		case 'k':
			optionk(optarg, fields, &nfields);
			continue;
		case 'o':
			oname = optarg;
			continue;
		case 'T':
			tname[0] = optarg;
			continue;
		case 'w':
			nmerge = atoi(optarg);
			if(nmerge < 2)
				fatal("-w too small","",0);
			continue;
		case 'y':
			optiony(optarg);
			continue;
		case 't':
			if(strlen(optarg) != 1)
				fatal("bad argument for -t", "", 0);
			tab = optarg[0];
			continue;
		case 'z':
			warn("nonstandard option -z ignored","",0);
		case -1:
			break;
		}
		break;
	}
	filelist(argc, argv);
	if(cflag)
		aflag = 0;
	fieldwrapup();
	tabinit();
	setsigs(cleanup);

	if(cflag) {
		if(nfiles > 1)
			fatal("-c takes just one file", "", 0);
		check(files[0]);
		return 0;
	}
	if(mflag) {
		merge(0);
		return 0;
	}
	for(n=0; n<nfiles; n++) {
		input = fileopen(files[n], "r");
		readin();
		fileclose(input, files[n]);
	}
	if(stack->head==0 && nextfile==0) {	/* empty input */
		if(strcmp(oname,"-") != 0) 
			fileclose(fileopen(oname, "w"), oname);
		return 0;
	}
	if(stack->head && stack->head->next)
		sort(stack, 0);
	if(nextfile > 0) {
		if(stack->head)
			dumptotemp();
		tabfree();
		merge(1);
	} else {
		FILE *f;
		f = fileopen(oname, "w");
		printout(stack->head, f, oname);
		fileclose(f, oname);
	}
	return 0;
}

void
filelist(int argc, char **argv)
{
	int i, j;
	files = nofiles;
	nfiles = argc - optind;
	if(nfiles != 0)
		files = argv + optind;
	else
		nfiles = 1;
	if(strcmp(argv[optind-1], "--") == 0)
		return;
	for(i=j=0; i<nfiles; i++) {
		if(strncmp(files[i], "-o", 2) == 0) {
			if(files[i][2] != 0)
				oname = files[i] + 2;
			else if(i < argc-1)
				oname = files[++i];
			else
				fatal("incomplete -o", "", 0);
		} else
			files[j++] = files[i];
	}
	files[j] = 0;
	nfiles = j;
}

void
readin(void)
{
	int n;
	struct rec *new;
	struct rec *p = stack->tail;
	struct rec *r = p? succ(p): buffer;

	for(;;) {
		if(bufmax-(uchar*)r < MINREC) {
			sealstack(p);
			dumptotemp();
			p = 0;
			r = buffer;
		}
		r->next = (struct rec*)bufmax;
		new = getline(r, input);
	recenter:
		if(new == 0) {
			r->next = 0;
			if(p)
				p->next = r;
			p = r;
			r = succ(r);
		} else if(new == ENDFILE) {
			sealstack(p);
			return;
		} else {
			sealstack(p);
			dumptotemp();
			p = 0;
			r = buffer;
			n = data(new)-(uchar*)new+new->dlen+new->klen;
			if((uchar*)r+n > bufmax)
				fatal("monster record", "", 0);
			memmove(r, new, n);
			free(new);
			new = 0;
			goto recenter;
		}
	}
}

void
sealstack(struct rec *p)
{
	if(p == 0)
		return;
	p->next = 0;
	if(stack->head == 0)
		stack->head = buffer;
	stack->tail = p;
}

void
printout(struct rec *r, FILE *f, char *name)
{
	int c, n;
	uchar *dp, *ep;
	for( ; r; r=r->next) {
		dp = data(r);
		n = r->dlen;
		ep = dp + n++;
		c = *ep;
		*ep = '\n';
		if(fwrite((char*)dp, 1, n, f) != n)
			fatal("error writing", name, 0);
		*ep = c;
	}
}

void
dumptotemp()
{
	char *tempfile = filename(nextfile++);
	FILE *temp = fileopen(tempfile,"w");

	if(stack->head == 0)
		fatal("monster record", "", 0);
	stack->tail->next = 0;		/* for good measure */
	sort(stack, 0);
	printout(stack->head, temp, tempfile);
	fileclose(temp, tempfile);
	free(tempfile);
	stack->head = stack->tail = 0;
	return;
}
