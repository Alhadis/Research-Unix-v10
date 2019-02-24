#define	MONITOR(a)
#include "jim.h"
#include "file.h"
#include "msgs.h"
#include <stdio.h>
#include <signal.h>
#include <sgtty.h>
#include "/usr/jerq/include/jioctl.h"
#include <setjmp.h>
#define	NAMELEN	25
jmp_buf		jmpbuf;
int		initflag=1;
char		*zflag="-";
extern Message	m;
extern char	pattern[];
struct sgttyb	sttybuf;
struct sgttyb	sttysave;
char		*argv0;
int		rescue();
int		rescuing=FALSE;
char		*strcpy();
char		*strncpy();
char		*getenv();
File		*getname();
char		tempname[]="/tmp/jim.XXXXXX";
int		tempfile;
int		diagnewline;	/* last char in diagnostic line was a `\n' */
int		searchdir;
int		unixtype;
char		*homedir;
String		lookstring;
#define	CMDSIZE	128
char	unixcmd[CMDSIZE];
main(argc, argv)
	char *argv[];
{
	register i;
	SIG_TYP onhup;
	char *mktemp();
	char *jerqprog="/usr/jerq/mbin/jim.m";
	int fflag=0;
	register File *f;

	argv0=argv[0];
	if(argc>2 && strcmp(argv[1], "-f")==0){
		jerqprog=argv[2];
		argc-=2;
		argv+=2;
		fflag++;
	}
	homedir=getenv("HOME");
	allocinit();
	close(creat(mktemp(tempname), 0600));
	if((tempfile=open(tempname, 2))==-1){
		fprintf(stderr, "%s: tmp file %s vanished!\n", argv0, tempname);
		exit(1);
	}
	if(!fflag)	/* probably debugging */
		unlink(tempname);
	ioctl(0, TIOCGETP, &sttysave);
	sttybuf=sttysave;
	sttybuf.sg_flags=RAW|ANYP;
	ioctl(0, TIOCSETP, &sttybuf);
	if(argc>1 && strcmp(argv[1], "-z")==0){
		zflag=argv[1];
		argv++; --argc;
	}
	if(!boot(jerqprog))
		quit("32ld errors");
	ioctl(0, TIOCEXCL, 0);	/* Exclusive use */
	strinit(&buffer);
	strinit(&transmit);
	strinit(&lookstring);
	Fload(Fcreat(Fnew(), ""));	/* DIAG */
	/* DIAG's name is built in to jim.m; don't need to jerqname() */
	--argc, argv++;
	if(argc>=MAXFILES){
		argc=MAXFILES-1;
		dprintf("only %d files open\n", MAXFILES-1);
	}
	for(i=0; i<argc; i++){
		f=Fnew();
		if(f==0)
			break;
		unmodified(Fcreat(f, jerqname(f, argv[i])));
	}
	if(f=file[0].next){	/* assignment = */
		dprintf("grabbing %s\n", charstar(&f->name));
		send(f->id, O_SWEEP, 0, 0, (char *)0);
	}
	initflag=0;
	signal(SIGPIPE, (int (*)())1);	/*  so if stdout closes we don't die */
	onhup=signal(SIGHUP, rescue);
	if(onhup==SIG_IGN)
		(void)signal(SIGHUP, SIG_IGN);
	setjmp(jmpbuf);
	send(0, O_DONE, 0, 0, (char *)0);
	for(;;){
		rcv();
		message();
	}
}
char *
jerqname(cf, s)
	register File *cf;
	register char *s;
{
	char buf[128+1];
	namecompact(s, buf);
	sendstr(cf->id, O_FILENAME, 0, buf);
	return s;
}
/*
 * Compact path elements in as, result in ad
 */
namecompact(as, ad)
	char *as, *ad;
{
	register char *s=as, *d=ad, *slash;
	register len;
	extern char *index();
	
	if(strlen(as)>NAMELEN){
		/* copy to first '/' */
		while(*s && *s!='/')
			*d++ = *s++;
		while(*s){
			*d++ = *s++;	/* copy the slash */
			while(*s=='/')	/* skip multiple slashes */
				s++;
			if(*s)
				*d++ = *s++;	/* an interesting non-slash */
			if((slash=index(s, '/')) == 0)
				break;
			s=slash;
		}
	}
	strcpy(d, s);
	if((len=strlen(ad))>NAMELEN){	/* desperation */
		strncpy(d=ad+len-15-2, "<-", 2);	/* 15 gives last element */
		strcpy(ad, d);	/* only works because of order in strcpy */
	}
}
rescue()
{
	register fd;
	register File *f;
	char buf[512];
	char name[256];
	int nnameless=0;
	signal(SIGHUP, SIG_IGN);
	rescuing=TRUE;
	for(f=file[0].next; f; f=f->next)
		if(f->block && f->str.s[0] && f->changed)
			goto Save;
	quit("HUP");
   Save:
	sprintf(buf, "%s/jim.recover", homedir? homedir : ".");
	fd=creat(buf, 0777);
	if(fd>0){
		for(f=file[0].next; f; f=f->next){
			if(f->block==0 || f->changed==0)
				continue;
			if(f->name.s && f->name.n && f->str.s[0])
				strcpy(name, charstar(&f->name));
			else
				sprintf(name, "nameless_%d", ++nnameless);
			sprintf(buf,
			   "/usr/jerq/lib/jimunpack %s %s \"$@\" <<'-%@$ %s'\n",
				"m", name, name);
			write(fd, buf, strlen(buf));
			Fwrite(f, (String *)0, fd);
			sprintf(buf, "\n-%@$ %s\n", name);
			write(fd, buf, strlen(buf));
		}
	}
	quit("HUP");
}
message()
{
	register File *f=Fileid((int)m.file);
	register n, op;
	register posn;
	register unsigned char *data;
	register data2;
	n=m.nbytes;
	op=m.op;
	posn=m.posn;
	data=(unsigned char *)m.data;
	data2=data[0]|(data[1]<<8);
	data[n]=0;
	switch(op){
	case O_SETSNARF:	/* set snarf buffer */
		strinsure(&buffer, (ulong)posn);
		buffer.n=posn;
		do{
			rcv();
			bcopy(m.data, m.data+m.nbytes, buffer.s+m.posn, 1);
		}while(m.nbytes);
		break;
	case O_DIAGNOSTIC:	/* end of input */
		if(diagnewline)	/* input is just '\n' */
			Freset(DIAG);
		diagnewline=TRUE;
		commands(f);	/* f==DIAG if no frames open */
		send(0, O_DONE, 0, 0, (char *)0);
		break;
	case O_SEARCH:		/* execute remembered command */
		/* posn: -2: look for text; 0: prev search; 1: prev unix */
		if(posn<0){
			if(f->nsel==0 && buffer.n==0)
				dprintf("no pattern selected\n");
			else{
				extern mustcompile;
				if(f->nsel==0)
					compile(charstar(&buffer), FALSE);
				else{
					Fsave(f, &lookstring, f->selloc, f->nsel);
					compile(charstar(&lookstring), FALSE);
				}
				if(execute(f, '/'))
					moveto(f, loc1, loc2);
				else
					dprintf("%s not found\n", charstar(f->nsel? &lookstring : &buffer));
				mustcompile=TRUE;
			}
		}else if(posn==1 || searchdir==0)
			Unix(f, unixtype, unixcmd, 0, 1);
		else{
			if(searchdir && execute(f, searchdir))
				moveto(f, loc1, loc2);
			else
				dprintf("%s not found\n", pattern);
		}
		send(0, O_DONE, 0, 0, (char *)0);
		break;
	case O_BACKSPACE:
	case O_INSERT:
		if(f->str.s==0 || f->name.s==0)	/* raced with CLOSE */
			break;
		if(f==DIAG && diagnewline){
			Freset(f);
			diagnewline=FALSE;
		}
		if(op==O_BACKSPACE){
			Fdeltext(f, f->origin+posn, (long)data2);
			f->selloc=f->origin+posn;
			f->nsel=0;
			break;
		}
		data[n]=0;
		Finstext(f, f->origin+posn, bldstring((char *)data));
		/* it's always typing, so... */
		f->selloc=f->origin+posn+n;
		f->nsel=0;
		if(f==DIAG && n>0 && data[n-1]=='\n')
			diagnewline=TRUE;
		break;
	case O_CUT:
		if(f->nsel>0)	/* Jerq doesn't change buffer if nsel<=0 */
			Fsave(f, &buffer, f->selloc, f->nsel);
		Fdeltext(f, f->selloc, f->nsel);
		f->nsel=0;
		break;
	case O_PASTE1:
		Fdeltext(f, f->selloc, f->nsel);
		break;
	case O_PASTE2:
		Finstext(f, f->selloc, &buffer);
		f->nsel=buffer.n;
		break;
	case O_SELECT:
		f->selloc=f->origin+posn;
		f->nsel=data2;
		break;
	case O_SNARF:
		Fsave(f, &buffer, f->selloc, f->nsel);
		break;
	case O_REQUEST:
		Fsave(f, &transmit, f->origin+posn, (long)min((long)data2, (long)NDATA));
		/* transmit->n==0 if at EOF */
		send(m.file, O_INSERT, posn, (unsigned)transmit.n, transmit.s);
		break;
	case O_SEEK:
		if(f->str.s==0)
			Fload(f);
		f->origin=Forigin(f, (long)posn);
		f->selloc=f->origin;
		f->nsel=0;
		tellseek(f);
		break;
	case O_CLOSE:
		if(f==0)
			break;
		if(f->changed && fileschanged){
			fileschanged = FALSE;
			error(charstar(&f->name), "changed");
		}
		sendstr(f->id, O_CLOSE, 0, (char *)0);
		Ffree(f);
		fileschanged=TRUE;	/* to be safe */
		break;
	case O_SCROLL:	/* Request scroll posn lines; answer with posn=# of chars */
		f->nsel=0;
		if(posn>0)	/* Forward */
			posn=Fforwnl(f, f->origin, posn);
		else{
			if(f->origin<=0)
				posn=0;
			else
				posn= -Fbacknl(f, f->origin-1, -posn)-1;
		}
		f->origin+=posn;
		tellseek(f);
		/* he'll give me a SELECT if it's different now */
		send(0, O_SCROLL, posn, 0, (char *)0);
		break;
	case O_FILENAME:	/* Tell jerq which file to open */
		for(f=file[0].next; f; f=f->next)
			if(length(f)==0 && (f->name.s==0 || f->name.n==0))
				break;
		if(f==0 && (f=Fnew())==0){
			sendstr(0, O_FILENAME, 0, (char *)0);
			error("too many files open; 255 max", (char *)0);
		}
		(void)jerqname(f, "");
		unmodified(Fload(Fcreat(f, "")));
		break;
	case O_WRITE:
		if(f->name.n==0)
			error("no file name", (char *)0);
		Fwrite(f, &f->name, 0);
		mesg("wrote", charstar(&f->name));
		send(0, O_DONE, 0, 0, (char *)0);
		unmodified(f);
		break;
	default:
		error("unix unk", (char *)data);
	}
}
char *
skipbl(p)
	register char *p;
{
	while(*p==' ' || *p=='\t')
		p++;
	return p;
}
commands(f)
	register File *f;
{
	register char *p;
	char fname[128];
	register c, i;
	register char *s;
	int wholefile=FALSE;
	straddc(&DIAG->str, '\0');
	p=DIAG->str.s;
	switch(c = *p){
	case 0:
	case '\n':
	case ' ':
	case '\t':
		break;
	case '?':
	case '/':
		if(*++p)
			compile(p, TRUE);
		else
			dprintf("%c%s\n", c, pattern);
		send(0, O_SEARCH, 0, 0, (char *)0);
		if(execute(f, searchdir=c))
			moveto(f, loc1, loc2);
		else
			dprintf("%s not found\n", pattern);
		break;
	case '=':
		dprintf("%d\n", Fcountnl(f, f->selloc));
		break;
	case '*':
		if((c=p[1])!='<' && c!='|' && c!='>')
			goto Default;
		c= *++p;
		wholefile=TRUE;
		/* fall through */
	case '<':
	case '>':
	case '|':
		if(*++p){
			strncpy(unixcmd, p, CMDSIZE);
		}else
			dprintf("%c%s\n", c, unixcmd);
		send(0, O_SEARCH, 1, 0, (char *)0);
		Unix(f, unixtype=c, unixcmd, wholefile, 1);
		break;
	case '$':
		p="10000000";
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		i=atoi(p);
		if(i<=0){
			loc1=0;
			loc2=0;
		}else{
			loc1=Fforwnl(f, 0L, i-1);	/* 1-indexed */
			loc2=loc1+Fforwnl(f, loc1, 1);
		}
		moveto(f, loc1, loc2);
		break;
	case 'c':
		if(*++p=='d'){
			if(*(p=skipbl(p+1))==0 && (p=getenv("HOME"))==0)
				error("no HOME set", (char *)0);
			if(chdir(p)==-1)
				error(p, ": bad directory");
			else for(f=file; f; f=f->next)
				if(f->str.s && f->str.s[0]!='/')
					f->date=DUBIOUS;
		}else
			goto Default;
		break;
	case 'w':
		if(p[1]=='\'' && p[2]==0){
			writechanged();
			break;
		}
		goto case__weEf;
	case 'e':
		if(f->changed && fileschanged){
			fileschanged = FALSE;
			error(charstar(&f->name), "changed");
		}
		/* fall through */
	case 'E':
	case 'f':
	case__weEf:
		if(f==DIAG)
			error("no frame open", (char *)0);
		s=charstar(&f->name);
		if(p[1]==0)
			strcpy(fname, s);
		else if(*++p!=' ')
			error("syntax", (char *)0);
		else{
			p=skipbl(p);
			strncpy(fname, p, sizeof fname -1);
		}
		if(c=='f'){
			if(strcmp(fname, s)!=0){
				f->date=DUBIOUS;
				(void)jerqname(f, fname);
				modified(f);
			}
			strdup(&f->name, fname);
			dprintf("%c. %s\n", " '"[f->changed], fname);
			checkifdups(f);
			break;
		}
		if(fname[0]==0)
			error("no file name", (char *)0);
		if(c=='e' || c=='E'){
			Fclose(f);
			(void)Fload(Fcreat(f, jerqname(f, fname)));
			checkifdups(f);
			f->changed=TRUE;
			unmodified(f);	/* force the message */
			tellseek(f);
			send(f->id, O_RESET, 0, 0, (char *)0);
		}else if(c=='w'){
			if(f->name.n==0){
				f->date=DUBIOUS;
				strdup(&f->name, jerqname(f, fname));
			}
			Fwrite(f, bldstring(fname), 0);
			mesg("wrote", fname);
			if(strcmp(fname, charstar(&f->name))==0)
				unmodified(f);
		}
		break;
	case 'g':
		if(f=getname(strcpy(fname, skipbl(p+1))))
			send(f->id, O_SWEEP, 0, 0, (char *)0);
		else
			dprintf("no files\n");
		break;
	case 'q':
		if(fileschanged)
			for(f=file; f; f=f->next)
				if(f->changed){
					fileschanged=FALSE;
					error("files changed", (char *)0);
				}
		/* fall through */
	case 'Q':
		if(p[1]==0)
			quit((char *)0);
		/* fall through */
	default:
	Default:
		dprintf("you typed: '%s'\n", DIAG->str.s);
	}
}
checkifdups(f)
	register File *f;
{
	register File *g;
	for(g=file[0].next; g; g=g->next)
		if(f!=g && f->name.n==g->name.n
			&& strncmp(f->name.s, g->name.s, (int)f->name.n)==0)
			dprintf("warning: %s already loaded\n", charstar(&f->name));
}
writechanged()
{
	register File *f;
	for(f=file[0].next; f; f=f->next)
		if(f->name.s && f->name.n && f->changed){
			Fwrite(f, &f->name, 0);
			mesg("wrote", charstar(&f->name));
			unmodified(f);
		}
}
char *
data2(n){
	static char x[2];
	x[0]=n;
	x[1]=n>>8;
	return x;
}
long
labs(a)
	long a;
{
	if(a<0)
		return -a;
	return a;
}
moveto(f, p1, p2)
	register File *f;
	register long p1, p2;
{
	register long p0=p1-Fbacknl(f, p1-1, 2)-1;
	register long nseen;	/* number of chars on screen */
	register posn, ntosend;
	register long x;
	if(p0<0)
		p0=0;
	/* Try to save some screen redrawing */
	send(f->id, O_CHARSONSCREEN, 0, 0, (char *)0);
	rcv();
	nseen=9*m.posn/10;
	if(labs(f->origin-p1)>nseen){	/* just redraw */
		posn= -1;
		ntosend=32767;
	}else if(f->origin<p1){		/* on screen now */
		posn=1;
		ntosend=0;
		p0=f->origin;
	}else{				/* back a little */
		posn=0;
		ntosend=f->origin-p0;
	}
	f->origin=p0;
	f->selloc=p1;
	f->nsel=p2-p1;
	x=p2-p1;
	if(x>32000)
		x=32000;	/* jerq'll truncate it, anyway */
	send(f->id, O_SELECT, (int)(p1-p0), 2, data2((int)x));
	send(f->id, O_MOVE, posn, 2, data2(ntosend));
	tellseek(f);
}
tellseek(f)
	register File *f;
{
	register n;
	register l=length(f);
	if(l>0)
		n=f->origin*YMAX/l;
	else
		n=0;
	if(n<0)
		n=0;
	if(n>YMAX)
		n=YMAX;
	send(f->id, O_SEEK, n, 0, (char *)0);
}
error(s, t)
	register char *s, *t;
{
	if(rescuing)
		return;
	mesg(s, t);
	/*?????tellseek(f);???why is this here??*/
	send(0, O_DONE, 0, 0, (char *)0);
	if(!initflag)
		longjmp(jmpbuf, 0);
}
quit(s)
	char *s;
{
	register File *f;
	if(strcmp(s, "32ld errors")!=0 && strcmp(s, "HUP")!=0)
		ioctl(0, JTERM, (struct sgttyb *)0);
	for(f=file; f; f=f->next)
		if(f->str.s != 0)
			Fclose(f);
	ioctl(0, TIOCSETP, &sttysave);
	ioctl(0, TIOCNXCL, 0);
	if(s)
		printf("%s: %s\n", argv0, s);
	MONITOR(0);
	exit(0);
}
ioerr(s, t)
	register char *s, *t;
{
	extern errno;
	char buf[128];
	sprintf(buf, "%s error (%d) on %s", s, errno, t);
	error(buf, (char *)0);
}
mesg(s, t)
	register char *s, *t;
{
	dprintf(t? "%s %s\n": "%s\n", s, t);
}
sendstr(f, op, posn, d)
	unsigned f;
	register char *d;
{
	register n=strlen(d), l;
	do{
		if((l=n)>NDATA)
			l=NDATA;
		send(f, op, posn, l, d);
		posn+=l;
		d+=l;
		n-=l;
	}while(n>0);
}
/*VARARGS1*/
dprintf(a, b, c, d, e, f, g)
	char *a;
{
	register s;
	String junk;	/* avoid bldstring() to avoid clashes with others */
	char buf[128];
	if(rescuing)
		return;
	sprintf(buf, a, b, c, d, e, f, g);
	sendstr(0, O_DIAGNOSTIC, 0, buf);
	if(diagnewline)
		Freset(DIAG);
	s=strlen(buf);
	if(s>0 && buf[s-1]=='\n'){
		diagnewline=TRUE;
		buf[--s]=0;
	}else
		diagnewline=FALSE;
	junk.n=strlen(buf);
	junk.s=buf;
	Finstext(DIAG, (long)DIAG->str.n, &junk);
}
panic(s)
	char *s;
{
	sendstr(0, O_DIAGNOSTIC, 0, "panic: ");
 	sendstr(0, O_DIAGNOSTIC, 0, s);
	sendstr(0, O_DIAGNOSTIC, 0, " (call rob)");
	abort();
}
boot(s)
	char *s;
{
	if(system("/usr/jerq/bin/32ld", "32ld", zflag, s))
		return(0);
	return(1);
}
system(s, t, u, v)
char *s, *t, *u, *v;
{
	int status, pid, l;

	if ((pid = fork()) == 0) {
		execl(s, t, u, v, 0);
		_exit(127);
	}
	while ((l = wait(&status)) != pid && l != -1)
		;
	if (l == -1)
		status = -1;
	return(status);
}
modified(f)
	File *f;
{
	if(f!=DIAG){	/* we can change DIAG all we want */
		if(!f->changed){
			f->changed=TRUE;
			send(f->id, O_MODIFIED, TRUE, 0, (char *)0);
		}
		fileschanged=TRUE;
	}
}
unmodified(f)
	File *f;
{
	if(f!=DIAG && f->changed){
		f->changed=FALSE;
		send(f->id, O_MODIFIED, FALSE, 0, (char *)0);
	}
}
