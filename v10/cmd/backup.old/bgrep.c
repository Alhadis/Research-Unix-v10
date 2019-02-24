#include	<fio.h>
#include	<cbt.h>
#include	<regexp.h>
#include	"backup.h"
#include	<string.h>
#include	<utsname.h>
#include	<libc.h>

int dflag = 1;
int Dflag = 0;
int gflag = 0;
extern char *strcpy();
extern char *ctime(), *date();
extern long atol();
extern char **fakeargv();
long cutoff = 0x7FFFFFFF;
int ccmp = 1;
bfile *db;
char bdir[FNAMELEN];
void vproc(), fproc(), aproc(), pproc(), dproc();
char **fakeargv();

main(argc, argv)
	char **argv;
{
	char buf[FNAMELEN], *s;
	void (*fn)() = fproc;
	int server;

	backupdir(bdir);
	if(server = (strcmp(argv[1], "-S") == 0))
		argv = fakeargv(Frdline(0), &argc);
	else {
		struct utsname name;

		uname(&name);
		for(s = bdir; *s++;)
			;
		if(strrchr(s, '/'))
			s = strrchr(s, '/')+1;
		if(strcmp(s, name.sysname))
			relay(bdir, argv+1, "bkpgrep", "heavy");	/* it doesn't return */
	}
	for(argv++; *argv && (**argv == '-'); argv++)
		switch(argv[0][1])
		{
		case 'e':	fn = pproc; break;
		case 'a':	fn = aproc; break;
		case 'd':	dflag = 0; break;
		case 'V':	fn = vproc; break;
		case 'D':	fn = dproc; break;
		case '>':	ccmp = -1; /* fall thru */
		case '<':	cutoff = atol(&argv[0][2]); break;
		case 'g':	gflag = 1; break;
		default:	usage();
		}
	sprint(buf, "%s/%s", bdir, BDBM);
	if((db = bopen(buf, 0)) == 0){
		perror(buf);
		exit(1);
	}
	if(fn == aproc)
		(*fn)("no arg");
	else if(*argv)
		while(*argv)
			(*fn)(*argv++);
	else
		while(s = Frdline(0))
			(*fn)(s);
	if(server){
		Fputc(1, 0);
		Fputc(1, '\n');
	}
	exit(0);
}

usage()
{
	fprint(2, "usage:  backup grep [-e] [-d] [-a] [-V] [-D] [-[<>]n] [strings]\n");
	fprint(2, "\t-e\treg exp lookup\n");
	fprint(2, "\t-a\tall keys\n");
	fprint(2, "\t-V\tno reg exp, all versions\n");
	fprint(2, "\t-d\tprint dates as numbers\n");
	fprint(2, "\t-[<>]n\tonly files <=[>=] cutoff date n\n");
	fprint(2, "\t-D\tfind tree from this directory\n");
	exit(1);
}

char *
date(s)
	char *s;
{
	long t;

	if(dflag){
		t = atol(s);
		s = ctime(&t);
		s[24] = 0;
	}
	return(s);
}

static char *str;

void
ppproc(s)
	char *s;
{
	mbuf d, key;
	char *unique;
	regexp *r;
		char c;
	char buf1[4096], buf[4096];

	d.mdata = buf;
	key.mdata = buf1;

	r = regcomp(str = s);	/* cannot return 0 because regerror exits */
	for(bfirst(db); bread(db, &key, &d) == 0;){
		if(regexec(r, key.mdata, (regsubexp *)0, 0)){
			for(unique = key.mdata; *unique; unique++)
			if((unique[0] == '/') && (unique[1] == '/'))
				break;
			if(*unique){
				c = *unique;
				*unique = 0;
				if(cut(unique+2))
					continue;
				Fprint(1, "%s//%s -- %s\n", key.mdata,
					date(unique+2), d.mdata);
				*unique = c;
			}
			else
				if(!cut(d.mdata))
					Fprint(1, "%s -- %s\n", key.mdata, date(d.mdata));
		}
	}
}

void
fproc(s)
	char *s;
{
	mbuf d, key;
	char *unique;
	char *ss;
	char buf1[4096], buf[4096];

	d.mdata = buf;
	key.mdata = buf1;
	key.mdata = s;
	key.mlen = strlen(s)+1;
	for(unique = s; *unique; unique++)
		if((unique[0] == '/') && (unique[1] == '/'))
			break;
	if(bseek(db, key) == 1){
		bread(db, (mbuf *)0, &d);
		if(*unique){
			if(ss = strchr(d.mdata, ' '))
				*ss = 0;
			Fprint(1, "%s/%s\n", bdir, d.mdata);
		}
		else
			Fprint(1, "%s//%s\n", key.mdata, date(d.mdata));
	}
}

void
vproc(s)
	char *s;
{
	mbuf d, key;
	char *unique;
	char *ss;
	char buf1[4096], buf[4096];

	d.mdata = buf;
	key.mdata = buf1;
	sprint(key.mdata, "%s//000000000", s);
	key.mlen = strlen(key.mdata)+1;
	bseek(db, key);
	for(;;){
		if(bread(db, &key, &d)) break;
		for(unique = key.mdata; *unique; unique++)
			if((unique[0] == '/') && (unique[1] == '/'))
				break;
		*unique = 0;
		if(strcmp(s, key.mdata)) break;
		if(!gflag){
			if(ss = strchr(d.mdata, ' '))
				*ss = 0;
		}
		if(!cut(unique+2))
			Fprint(1, "%s//%s %s/%s\n", key.mdata, date(unique+2), bdir, d.mdata);
	}
}

void
dproc(s)
	char *s;
{
	mbuf d, key;
	char *unique;
	int n, found1;
	char *ss;
	char buf1[4096], buf[4096], oldf[4096], oldline[4096];

	d.mdata = buf;
	key.mdata = buf1;
	strcpy(key.mdata, s);
	n = strlen(s);
	oldf[0] = 0;
	found1 = 0;
	key.mlen = strlen(key.mdata)+1;
	bseek(db, key);
	for(;;){
		if(bread(db, &key, &d)) break;
		if(strncmp(key.mdata, s, n) > 0) break;
		for(unique = key.mdata; *unique; unique++)
			if((unique[0] == '/') && (unique[1] == '/'))
				break;
		if(*unique == 0) continue;
		*unique = 0;
		if(strcmp(oldf, key.mdata)){
			if(found1){
				Fprint(1, "%s", oldline);
				found1 = 0;
			}
			strcpy(oldf, key.mdata);
		}
		if(ss = strchr(d.mdata, ' '))
			*ss = 0;
		if(!cut(unique+2)){
			sprint(oldline, "%s//%s %s/%s\n", key.mdata,
				date(unique+2), bdir, d.mdata);
			found1 = 1;
		}
	}
	if(found1)
		Fprint(1, "%s", oldline);
}

void
pproc(s)
	char *s;
{
	mbuf d, key;
	char *unique;
	int n, found1;
	char *ss;
	regexp *r;
	char buf1[4096], buf[4096], oldf[4096], oldline[4096];

	r = regcomp(s);
	for(ss = s; *ss; ss++){
		if(*ss == '\\'){
			if(*++ss)
				continue;
			else
				break;
		}
		if((*ss == '.') || (*ss == '[') || (*ss == '$')
				|| (*ss == '^') || (*ss == '|')
				|| (*ss == '+') || (*ss == '?')
				|| (*ss == '(') || (*ss == '*'))
			break;
	}
	*ss = 0;
	d.mdata = buf;
	key.mdata = buf1;
	strcpy(key.mdata, s);
	n = strlen(s);
	oldf[0] = 0;
	found1 = 0;
	key.mlen = strlen(key.mdata)+1;
	bseek(db, key);
	for(;;){
		if(bread(db, &key, &d)) break;
		if(strncmp(key.mdata, s, n) > 0) break;
		for(unique = key.mdata; *unique; unique++)
			if((unique[0] == '/') && (unique[1] == '/'))
				break;
		if(*unique == 0) continue;
		*unique = 0;
		if(strcmp(oldf, key.mdata)){
			if(found1){
				Fprint(1, "%s", oldline);
				found1 = 0;
			}
			strcpy(oldf, key.mdata);
		}
		if(ss = strchr(d.mdata, ' '))
			*ss = 0;
		if((!cut(unique+2)) && regexec(r, key.mdata, (regsubexp *)0, 0)){
			sprint(oldline, "%s//%s %s/%s\n", key.mdata,
				date(unique+2), bdir, d.mdata);
			found1 = 1;
		}
	}
	if(found1)
		Fprint(1, "%s", oldline);
}

void
aproc()
{
	mbuf d, key;
	char buf1[4096], buf[4096];

	d.mdata = buf;
	key.mdata = buf1;
	for(bfirst(db); bread(db, &key, &d) == 0;)
		Fprint(1, "%s -- %s\n", key.mdata, d.mdata);
}

regerror(msg)
	char *msg;
{
	fprint(2, "'%s': %s\n", str, msg);
	exit(1);
}

cut(s)
	char *s;
{
	return((atol(s) - cutoff)*ccmp >= 0);
}
