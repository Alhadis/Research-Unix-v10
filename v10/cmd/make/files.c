static	char *sccsid = "@(#)files.c	8th Edition (Bell Labs) 85/10/23";
/* UNIX DEPENDENT PROCEDURES */

#define NAMESPERBLOCK	32

/* DEFAULT RULES FOR UNIX */

char *dfltmacro[] =
	{
#ifdef pwb
	".SUFFIXES : .L .out .o .c .f .e .r .y .yr .ye .l .s .z .x .t .h .cl",
#else
	".SUFFIXES :",
#endif
	"YACC=yacc",
	"YACCR=yacc -r",
	"YACCE=yacc -e",
	"YFLAGS=",
	"LEX=lex",
	"LFLAGS=",
	"CC=cc",
	"XI=xi",
	"AR=ar",
	"AS=as",
	"PC=pc",
	"PFLAGS=",
	"CFLAGS=",
	"XIFLAGS=",
	"FC=f77",
	"RC=f77",
	"RFLAGS=",
	"EC=f77",
	"EFLAGS=",
	"FFLAGS=",
	"LOADLIBES=",
#ifdef pwb
	"SCOMP=scomp",
	"SCFLAGS=",
	"CMDICT=cmdict",
	"CMFLAGS=",
#endif

#ifdef pwb
	".o.L .c.L .t.L:",
	"\t$(SCOMP) $(SCFLAGS) $<",

	".t.o:",
	"\t$(SCOMP) $(SCFLAGS) -c $<",

	".t.c:",
	"\t$(SCOMP) $(SCFLAGS) -t $<",

	".h.z .t.z:",
	"\t$(CMDICT) $(CMFLAGS) $<",

	".h.x .t.x:",
	"\t$(CMDICT) $(CMFLAGS) -c $<",
#endif
	0 };

char *dfltpat[] =
	{
	"%.o : %.c",
	"\t$(CC) $(CFLAGS) -c $<",

	"%.s : %.c",
	"\t$(CC) $(CFLAGS) -S $<",

	"%.o : %.xi",
	"\t$(XI) $(XIFLAGS) -c $<",

	"%.o : %.p",
	"\t$(PC) $(PFLAGS) -c $<",

	"%.o : %.cl",
	"\tclass -c $<",

	"%.o : %.e",
	"\t$(FC) $(EFLAGS) -c $<",

	"%.o : %.r",
	"\t$(FC) $(RFLAGS) -c $<",

	"%.o : %.f",
	"\t$(FC) $(FFLAGS) -c $<",

	"%.o : %.s",
	"\t$(AS) -o $@ $<",

	"%.o : %.y",
	"\t$(YACC) $(YFLAGS) $<",
	"\t$(CC) $(CFLAGS) -c y.tab.c",
	"\trm y.tab.c",
	"\tmv y.tab.o $@",

	"%.o : %.yr",
	"\t$(YACCR) $(YFLAGS) $<",
	"\t$(RC) $(RFLAGS) -c y.tab.r",
	"\trm y.tab.r",
	"\tmv y.tab.o $@",

	"%.o : %.ye",
	"\t$(YACCE) $(YFLAGS) $<",
	"\t$(EC) $(RFLAGS) -c y.tab.e",
	"\trm y.tab.e",
	"\tmv y.tab.o $@",

	"%.o : %.l",
	"\t$(LEX) $(LFLAGS) $<",
	"\t$(CC) $(CFLAGS) -c lex.yy.c",
	"\trm lex.yy.c",
	"\tmv lex.yy.o $@",

	"%.c : %.y",
	"\t$(YACC) $(YFLAGS) $<",
	"\tmv y.tab.c $@",

	"%.c : %.l",
	"\t$(LEX) $(LFLAGS) $<",
	"\tmv lex.yy.c $@",

	"%.r : %.yr",
	"\t$(YACCR) $(YFLAGS) $<",
	"\tmv y.tab.r $@",

	"%.e : %.ye",
	"\t$(YACCE) $(YFLAGS) $<",
	"\tmv y.tab.e $@",

	"% : %.o",
	"\t$(CC) $(CFLAGS) $< $(LOADLIBES) -o $@",

	"% : %.s",
	"\t$(CC) $(CFLAGS) $< $(LOADLIBES) -o $@",

	"% : %.c",
	"\t$(CC) $(CFLAGS) $< $(LOADLIBES) -o $@",

	"% : %.f",
	"\t$(FC) $(FFLAGS) $< $(LOADLIBES) -o $@",
	"\t-rm $*.o",

	"% : %.r",
	"\t$(FC) $(RFLAGS) $< $(LOADLIBES) -o $@",
	"\t-rm $*.o",

	"% : %.e",
	"\t$(FC) $(EFLAGS) $< $(LOADLIBES) -o $@",
	"\t-rm $*.o",

	"% : %.y",
	"\t$(YACC) $(YFLAGS) $<",
	"\t$(CC) $(CFLAGS) y.tab.c $(LOADLIBES) -ly -o $@",
	"\trm y.tab.c",

	"% : %.l",
	"\t$(LEX) $(LFLAGS) $<",
	"\t$(CC) $(CFLAGS) lex.yy.c $(LOADLIBES) -ll -o $@",
	"\trm lex.yy.c",

	"% : %.xi",
	"\t$(XI) $(XIFLAGS) $< -o $@",

	0 };



char *dfltsuff[] =
	{
#ifdef pwb
	".SUFFIXES : .L .out .o .c .f .e .r .y .yr .ye .l .s .z .x .t .h .cl",
#else
	".SUFFIXES : .out .o .c .f .e .r .y .yr .ye .l .s .cl .p",
#endif
	".c.o :",
	"\t$(CC) $(CFLAGS) -c $<",

	".p.o :",
	"\t$(PC) $(PFLAGS) -c $<",

	".cl.o :",
	"\tclass -c $<",

	".e.o .r.o .f.o :",
	"\t$(FC) $(RFLAGS) $(EFLAGS) $(FFLAGS) -c $<",

	".s.o :",
	"\t$(AS) -o $@ $<",

	".y.o :",
	"\t$(YACC) $(YFLAGS) $<",
	"\t$(CC) $(CFLAGS) -c y.tab.c",
	"\trm y.tab.c",
	"\tmv y.tab.o $@",

	".yr.o:",
	"\t$(YACCR) $(YFLAGS) $<",
	"\t$(RC) $(RFLAGS) -c y.tab.r",
	"\trm y.tab.r",
	"\tmv y.tab.o $@",

	".ye.o :",
	"\t$(YACCE) $(YFLAGS) $<",
	"\t$(EC) $(RFLAGS) -c y.tab.e",
	"\trm y.tab.e",
	"\tmv y.tab.o $@",

	".l.o :",
	"\t$(LEX) $(LFLAGS) $<",
	"\t$(CC) $(CFLAGS) -c lex.yy.c",
	"\trm lex.yy.c",
	"\tmv lex.yy.o $@",

	".xi.o :",
	"\t$(XI) -c $<",

	".y.c :",
	"\t$(YACC) $(YFLAGS) $<",
	"\tmv y.tab.c $@",

	".l.c :",
	"\t$(LEX) $(LFLAGS) $<",
	"\tmv lex.yy.c $@",

	".yr.r:",
	"\t$(YACCR) $(YFLAGS) $<",
	"\tmv y.tab.r $@",

	".ye.e :",
	"\t$(YACCE) $(YFLAGS) $<",
	"\tmv y.tab.e $@",

#ifdef pwb
	".o.L .c.L .t.L:",
	"\t$(SCOMP) $(SCFLAGS) $<",

	".t.o:",
	"\t$(SCOMP) $(SCFLAGS) -c $<",

	".t.c:",
	"\t$(SCOMP) $(SCFLAGS) -t $<",

	".h.z .t.z:",
	"\t$(CMDICT) $(CMFLAGS) $<",

	".h.x .t.x:",
	"\t$(CMDICT) $(CMFLAGS) -c $<",
#endif

	".s.out .c.out .o.out :",
	"\t$(CC) $(CFLAGS) $< $(LOADLIBES) -o $@",

	".f.out .r.out .e.out :",
	"\t$(FC) $(EFLAGS) $(RFLAGS) $(FFLAGS) $< $(LOADLIBES) -o $@",
	"\t-rm $*.o",

	".y.out :",
	"\t$(YACC) $(YFLAGS) $<",
	"\t$(CC) $(CFLAGS) y.tab.c $(LOADLIBES) -ly -o $@",
	"\trm y.tab.c",

	".l.out :",
	"\t$(LEX) $(LFLAGS) $<",
	"\t$(CC) $(CFLAGS) lex.yy.c $(LOADLIBES) -ll -o $@",
	"\trm lex.yy.c",

	0 };

#include "defs"


#include <sys/stat.h>

TIMETYPE exists(filename)
char *filename;
{
struct stat buf;
register char *s;
TIMETYPE lookarch();

for(s = filename ; *s!='\0' && *s!='(' &&  *s!=')' ; ++s)
	;

if(*s != '\0')
	return lookarch(filename);

if(stat(filename,&buf) < 0) 
	return 0;
else	return buf.st_mtime;
}


TIMETYPE prestime()
{
TIMETYPE t;
time(&t);
return t;
}

FSTATIC char nmtemp[MAXNAMLEN+1];	/* guarantees a null after the name */
FSTATIC char *tempend = nmtemp + MAXNAMLEN;



depblkp srchdir(pat, mkchain, nextdbl)
register char *pat;	/* pattern to be matched in directory */
int mkchain;		/* nonzero if results to be remembered */
depblkp nextdbl;	/* final value for chain */
{
DIR *dirf;
struct dirhd *dirptr, *opdir();
char *dirname, *dirpref, *endir, *filepat, *p, temp[100];
char fullname[100];
nameblkp q;
depblkp thisdbl;
struct pattern *patp;

struct direct *dptr;


thisdbl = 0;

if(mkchain == NO)
	for(patp=firstpat ; patp ; patp = patp->nxtpattern)
		if(equal(pat, patp->patval)) return 0;

patp = ALLOC(pattern);
patp->nxtpattern = firstpat;
firstpat = patp;
patp->patval = copys(pat);

endir = 0;

for(p=pat; *p!='\0'; ++p)
	if(*p=='/') endir = p;

if(endir==0)
	{
	dirname = ".";
	dirpref = "";
	filepat = pat;
	}
else	{
	dirname = pat;
	*endir = '\0';
	dirpref = concat(dirname, "/", temp);
	filepat = endir+1;
	}

dirptr = opdir(dirname,YES);
dirf = dirptr->dirfc;

for( dptr = readdir(dirf) ; dptr ; dptr = readdir(dirf) )
	{
	register char *p1, *p2;
	p1 = dptr->d_name;
	p2 = nmtemp;
	while( (p2<tempend) && (*p2++ = *p1++)!='\0')
		;
	if( amatch(nmtemp,filepat) )
		{
		concat(dirpref,nmtemp,fullname);
		if( (q=srchname(fullname)) ==0)
			q = makename(copys(fullname));
		if(mkchain)
			{
			thisdbl = ALLOC(depblock);
			thisdbl->nxtdepblock = nextdbl;
			thisdbl->depname = q;
			nextdbl = thisdbl;
			}
		}
	}


if(endir)
	*endir = '/';

cldir(dirptr, YES);

return thisdbl;
}

struct dirhd *opdir(dirname, stopifbad)
char *dirname;
int stopifbad;
{
register struct dirhd *od;

for(od = firstod; od; od = od->nxtdirhd)
	if(equal(dirname, od->dirn) )
		break;

if(od == NULL)
	{
	++nopdir;
	od = ALLOC(dirhd);
	od->nxtdirhd = firstod;
	firstod = od;
	od->dirn = copys(dirname);
	}

if(od->dirfc==NULL && (od->dirfc = opendir(dirname)) == NULL && stopifbad)
	{
	fprintf(stderr, "Directory %s: ", dirname);
	fatal("Cannot open");
	}

return od;
}



cldir(dp, used)
register struct dirhd *dp;
int used;
{
if(nopdir >= MAXDIR)
	{
	closedir(dp->dirfc);
	dp->dirfc = NULL;
	}
else if(used)
	rewinddir(dp->dirfc); /* start over at the beginning  */
}

/* stolen from glob through find */

static amatch(s, p)
char *s, *p;
{
	register int cc, scc, k;
	int c, lc;

	scc = *s;
	lc = 077777;
	switch (c = *p) {

	case '[':
		k = 0;
		while (cc = *++p) {
			switch (cc) {

			case ']':
				if (k)
					return amatch(++s, ++p);
				else
					return 0;

			case '-':
				k |= (lc <= scc)  & (scc <= (cc=p[1]) ) ;
			}
			if (scc==(lc=cc)) k++;
		}
		return 0;

	case '?':
	caseq:
		if(scc) return amatch(++s, ++p);
		return 0;
	case '*':
		return umatch(s, ++p);
	case 0:
		return !scc;
	}
	if (c==scc) goto caseq;
	return 0;
}

static umatch(s, p)
char *s, *p;
{
	if(*p==0) return 1;
	while(*s)
		if (amatch(s++,p)) return 1;
	return 0;
}

#ifdef METERFILE
#include <pwd.h>
int meteron	= 0;	/* default: metering off */

meter(file)
char *file;
{
TIMETYPE tvec;
char *p, *ctime();
FILE * mout;
struct passwd *pwd, *getpwuid();

if(file==0 || meteron==0) return;

pwd = getpwuid(getuid());

time(&tvec);

if( mout = fopen(file,"a") )
	{
	p = ctime(&tvec);
	p[16] = '\0';
	fprintf(mout, "User %s, %s\n", pwd->pw_name, p+4);
	fclose(mout);
	}
}
#endif


/* look inside archives for notations a(b) and a((b))
	a(b)	is file member   b   in archive a
	a((b))	is entry point  _b  in object archive a
*/

#ifdef ASCARCH
#	include <ar.h>
#else
#	include <ar.h>
#endif
#include <a.out.h>

static long arflen;
static long arfdate;
static char arfname[16];
FILE *arfd;
long int arpos, arlen;

static struct exec objhead;

static struct nlist objentry;


TIMETYPE lookarch(filename)
char *filename;
{
char *p, *q, *send, s[15], pad;
int i, nc, nsym, objarch;

for(p = filename; *p!= '(' ; ++p)
	;

*p = '\0';
if( ! openarch(filename) )
	{
	*p = '(';
	return 0L;
	}
*p++ = '(';
if(*p == '(')
	{
	objarch = YES;
	nc = 8;
	pad = '\0';
	++p;
	}
else
	{
	objarch = NO;
	nc = 14;
	pad = ' ';
	}

send = s + nc;
for( q = s ; q<send && *p!='\0' && *p!=')' ; *q++ = *p++ )
	;
if(p[0]==')' && p[1]!='\0')	/* forbid stuff after the paren */
	{
	clarch();
	return 0L;
	}
while(q < send)
	*q++ = pad;
while(getarch())
	{
	if(objarch)
		{
		getobj();
		nsym = objhead.a_syms / sizeof(objentry);
		for(i = 0; i<nsym ; ++i)
			{
			fread( (char *) &objentry, sizeof(objentry),1,arfd);
			if( (objentry.n_type & N_EXT)
			   && ((objentry.n_type & ~N_EXT) || objentry.n_value)
			   && eqstr(objentry.n_un.n_name,s,nc))
				{
				clarch();
				return arfdate;
				}
			}
		}

	else if( eqstr(arfname, s, nc))
		{
		clarch();
/*TEMP fprintf(stderr, "found archive member %14s, time=%d\n", s, arfdate); */
		return arfdate;
		}
	}

clarch();
return  0L;
}


clarch()
{
fclose( arfd );
}


openarch(f)
register char *f;
{
#ifdef ASCARCH
char magic[SARMAG];
#endif
int word;
struct stat buf;
nameblkp p;

stat(f, &buf);
arlen = buf.st_size;

arfd = fopen(f, "r");
if(arfd == NULL)
	return NO;
	/* fatal1("cannot open %s", f); */

fread( (char *) &word, sizeof(word), 1, arfd);

#ifdef ASCARCH
	fseek(arfd, 0L, 0);
	fread(magic, SARMAG, 1, arfd);
	arpos = SARMAG;
	if( ! eqstr(magic, ARMAG, SARMAG) )
		fatal1("%s is not an archive", f);
#else
	arpos = sizeof(word);
	if(word != ARMAG)
		fatal1("%s is not an archive", f);
#endif

if( !(p = srchname(f)) )
	p = makename( copys(f) );
p->isarch = YES;
arflen = 0;
return YES;
}



getarch()
{
struct ar_hdr arhead;
long atol();

arpos += (arflen + 1) & ~1L;	/* round archived file length up to even */
if(arpos >= arlen)
	return 0;
fseek(arfd, arpos, 0);

	fread( (char *) &arhead, sizeof(arhead), 1, arfd);
	arpos += sizeof(arhead);
#ifdef ASCARCH
	arflen = atol(arhead.ar_size);
	arfdate = atol(arhead.ar_date);
#else
	arflen = arhead.ar_size;
	arfdate = arhead.ar_date;
#endif
	strncpy(arfname, arhead.ar_name, sizeof(arhead.ar_name));
return 1;
}


getobj()
{
long int skip;

fread( (char *) &objhead, sizeof(objhead), 1, arfd);
if (N_BADMAG(objhead))
	fatal1("%s is not an object module", arfname);
skip = objhead.a_text + objhead.a_data;
skip += objhead.a_trsize + objhead.a_drsize;
fseek(arfd, skip, 1);
}


eqstr(a,b,n)
register char *a, *b;
int n;
{
register int i;
for(i = 0 ; i < n ; ++i)
	if(*a++ != *b++)
		return NO;
return YES;
}

/* find the directory containing name.
   read it into the hash table if it hasn't been used before or if
   if might have changed since last reference
*/

dirsrch(name)
char *name;
{
DIR *dirf;
struct dirhd *opdir(), *dirp;
TIMETYPE dirt, objt, lookarch();
int dirused, hasparen;
char *dirname, *lastslash;
char *fullname, *filepart, *fileend, *s;
struct direct *dptr;

lastslash = NULL;
hasparen = NO;

for(s=name; *s; ++s)
	if(*s == '/')
		lastslash = s;
	else if(*s=='(' || *s==')')
		hasparen = YES;

if(hasparen)
	{
	if(objt = lookarch(name))
		makename(name)->modtime = objt;
	return;
	}

if(lastslash)
	{
	dirname = name;
	*lastslash = '\0';
	}
else
	dirname = ".";

dirused = NO;
dirp = opdir(dirname, NO);
dirf = dirp->dirfc;
if(dirp->dirok || !dirf)
	goto ret;
dirt = exists(dirname);
if(dirp->dirtime == dirt)
	goto ret;

dirp->dirok = YES;
dirp->dirtime = dirt;
dirused = YES;

/* allocate buffer to hold full file name */
if(lastslash)
	{
	fullname = (char *) ckalloc(strlen(dirname)+MAXNAMLEN+2);
	concat(dirname, "/", fullname);
	filepart = fullname + strlen(fullname);
	}
else
	filepart = fullname = (char *) ckalloc(MAXNAMLEN+1);


fileend = filepart + MAXNAMLEN;
*fileend = '\0';
for(dptr = readdir(dirf) ; dptr ; dptr = readdir(dirf) )
	{
	register char *p1, *p2;
	p1 = dptr->d_name;
	p2 = filepart;
	while( (p2<fileend) && (*p2++ = *p1++)!='\0')
		;
	if( ! srchname(fullname) )
		(void) makename(copys(fullname));
	}

free(fullname);

ret:
	cldir(dirp, dirused);
	if(lastslash)
		*lastslash = '/';
}




baddirs()
{
register struct dirhd *od;

for(od = firstod; od; od = od->nxtdirhd)
	od->dirok = NO;
}
