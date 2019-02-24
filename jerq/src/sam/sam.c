#include "sam.h"
#include <setjmp.h>
#include <signal.h>

typedef struct Patlist{
	int	nalloc;
	int	nused;
	String	**ptr;
}Patlist;

uchar	genbuf[BLOCKSIZE];
char	*home;
int	io;
int	panicking;
int	rescuing;
Mod	modnum;
String	genstr;
String	rhs;
String	wd;
String	cmdstr;
int	intr();
int	rescue();
char	*getenv();
File	*current();
File	*tofile();
File	*getfile();
File	*curfile;
File	*flist;
File	*cmd;
jmp_buf	mainloop;
Filelist tempfile;
int	quitok=TRUE;
Patlist	globstr;
Patlist	pattern;
int	downloaded;
int	dflag;
int	Rflag;
int	zflag;
char	*machine;
int	dounlock;

main(argc, argv)
	uchar *argv[];
{
	register i;
	int (*onintr)();
	while(argc>1 && argv[1][0]=='-'){
		switch(argv[1][1]){
		case 'd':
			dflag++;
			break;
		case 'r':
			--argc, argv++;
			if(argc==1){
				dprint("usage: sam [-d] -r machine\n");
				return 1;
			}
			machine=(char *)argv[1];
			break;
		case 'z':
			zflag++;
			break;
		case 'R':
			Rflag++;
			break;
		default:
			dprint("sam: unknown flag %c\n", argv[1][1]);
			return 1;
		}
		--argc, argv++;
	}
	allocinit();
	Fstart();
	strinit(&cmdstr);
	strinit(&lastpat);
	strinit(&lastregexp);
	strinit(&genstr);
	strinit(&rhs);
	strinit(&wd);
	gcnew(tempfile.ptr, 0);
	strinit(&unixcmd);
	straddc(&unixcmd, '\0');
	home=getenv("HOME");
	if(home==0)
		home="/tmp";
	if(!dflag){
		if(machine)
			connectto(machine);
		if(!Rflag){
			if(!bootterm(zflag, getenv("TERM")))
				return 1;
		}
		rawmode(1);
		if(machine)
			join();
		downloaded=1;
	}
	if(argc>1){
		for(i=1; i<argc; i++)
			if(!setjmp(mainloop)){
				strdup(&genstr, argv[i]);
				Fsetname(newfile());
			}
	}else if(!downloaded)
		newfile()->state=Clean;
	modnum++;
	onintr=signal(SIGINT, intr);
	if(onintr)
		signal(SIGINT, onintr);
	onintr=signal(SIGHUP, rescue);
	if(onintr)
		signal(SIGHUP, onintr);
	if(file.nused)
		current(file.ptr[0]);
	(void)setjmp(mainloop);
	cmdloop();
	trytoquit();	/* if we already q'ed, quitok will be TRUE */
	if(downloaded)
		rawmode(0);
	return 0;
}
panic(s)
	char *s;
{
	if(!panicking++ && !setjmp(mainloop)){
		dprint("sam: panic: %s\n", s);
		rescue();
	}
	abort();
}
rescue(){
	register i, nblank=0;
	register File *f;
	uchar buf[128];
	if(rescuing++)
		exit(1);
	signal(SIGHUP, SIG_IGN);
	io=-1;
	for(i=0; i<file.nused; i++){
		f=file.ptr[i];
		if(f==cmd || f->nbytes==0 || f->state!=Dirty)
			continue;
		if(io==-1){
			strcpy(buf, (uchar *)home);
			strcpy(buf+strlen(buf), (uchar *)"/sam.save");
			io=creat((char *)buf, 0777);
			if(io<0)
				return;
		}
		strcpy(buf, f->name.s);
		if(buf[0]==0)
			sprint((char *)buf, "nameless.%d", nblank++);
		fprint(io, "/usr/jerq/lib/samsave '%s' $* <<'---%s'\n",
			(char *)buf, (char *)buf);
		addr.r.p1=0, addr.r.p2=f->nbytes;
		writeio(f);
		fprint(io, "\n---%s\n", (char *)buf);
	}
	if(panicking)
		abort();
	exit(0);
}
hiccough(s)
	char *s;
{
	if(rescuing)
		exit(1);
	if(s)
		dprint("%s\n", s);
	resetcmd();
	resetxec();
	compactok();
	if(io>0)
		close(io);
	if(undobuf->nbytes)
		Bdelete(undobuf, (Posn)0, undobuf->nbytes);
	update();
	if(curfile && curfile->state==Unread)
		curfile->state=Clean;
	dounlock=TRUE;
	if(downloaded && curfile && curfile->state!=Unread)
		outTs(Hcurrent, curfile->tag);
	longjmp(mainloop, 1);
}
intr(){
	signal(SIGINT, intr);
	error(Eintr);
}
trytoclose(f)
	register File *f;
{
	if(f==cmd)	/* possible? */
		return;
	if(f->state==Dirty && !f->closeok){
		f->closeok=TRUE;
		error_s(Emodified,
		    f->name.s[0]?(char *)f->name.s : "nameless file");
	}
	if(downloaded && f->rasp)
		outTs(Hclose, f->tag);
	delfile(f);
	if(f==curfile)
		current((File *)0);
}
trytoquit(){
	register c;
	register File *f;
	extern int eof;
	if(!quitok)
		for(c=0; c<file.nused; c++){
			f=file.ptr[c];
			if(f!=cmd && f->state==Dirty){
				quitok=TRUE;
				eof=FALSE;
				error(Echanges);
			}
		}
}
load(f)
	register File *f;
{
	Address saveaddr;
	strdupstr(&genstr, &f->name);
	filename(f);
	if(f->name.s[0]){
		saveaddr=addr;
		edit(f, 'I');
		addr=saveaddr;
	}else
		f->state=Clean;
	Fupdate(f, FALSE, TRUE);
}
cmdupdate(){
	if(cmd && cmd->mod!=0){
		Fupdate(cmd, FALSE, downloaded);
		cmd->dot.r.p1=cmd->dot.r.p2=cmd->nbytes;
		telldot(cmd);
	}
}
update(){
	register i, anymod;
	register File *f;
	settempfile();
	for(anymod=i=0; i<tempfile.nused; i++){
		f=tempfile.ptr[i];
		if(f==cmd)	/* cmd gets done in main() */
			continue;
		if(f->mod==modnum && Fupdate(f, FALSE, downloaded))
			anymod++;
		if(f->rasp)
			telldot(f);
	}
	if(anymod)
		modnum++;
}
File *
current(f)
	register File *f;
{
	return curfile=f;
}
edit(f, cmd)
	register File *f;
{
	register empty=TRUE;
	Posn p;
	int nonascii;
	if(cmd=='r')
		Fdelete(f, addr.r.p1, addr.r.p2);
	if(cmd=='e' || cmd=='I'){
		Fdelete(f, (Posn)0, f->nbytes);
		addr.r.p2=f->nbytes;
	}else if(f->nbytes!=0 || (f->name.s[0] && strcmp(genstr.s, f->name.s)!=0))
		empty=FALSE;
	if((io=open((char *)genstr.s, 0))<0)
		error_s(Eopen, (char *)genstr.s);
	p=readio(f, &nonascii, empty);
	closeio((cmd=='e' || cmd=='I')? -1 : p);
	if(cmd=='r')
		f->dot.r.p1=addr.r.p2, f->dot.r.p2=addr.r.p2+p;
	else
		f->dot.r.p1=f->dot.r.p2=0;
	quitok=f->closeok=empty;
	state(f, empty && !nonascii? Clean : Dirty);
	if(cmd=='e')
		filename(f);
}
getname(f, s, save)
	register File *f;
	register String *s;
{
	register c, i;
	strzero(&genstr);
	if(s==0 || (c=s->s[0])==0){		/* no name provided */
		if(f)
			strdupstr(&genstr, &f->name);
		else
			straddc(&genstr, '\0');
		goto Return;
	}
	if(c!=' ' && c!='\t')
		error(Eblank);
	for(i=0; (c=s->s[i])==' ' || c=='\t'; i++)
		;
	while(s->s[i]>' ')
		straddc(&genstr, s->s[i++]);
	if(s->s[i])
		error(Enewline);
	straddc(&genstr, '\0');
	if(f && (save || f->name.s[0]==0)){
		Fsetname(f);
		if(strcmp(f->name.s, genstr.s)){
			quitok=f->closeok=FALSE;
			f->inumber=0;
			f->date=0;
			state(f, Dirty); /* if it's 'e', fix later */
		}
	}
    Return:
	return genstr.n-1;	/* strlen(name) */
}
filename(f)
	register File *f;
{
	dprint("%c%c%c %s\n", " '"[f->state==Dirty],
		"-+"[f->rasp!=0], " ."[f==curfile], genstr.s);
}
undo()
{
	register File *f;
	register i;
	Mod max;
	if((max=curfile->mod)==0)
		return;
	settempfile();
	for(i=0; i<tempfile.nused; i++){
		f=tempfile.ptr[i];
		if(f!=cmd && f->mod==max)
			undostep(f);
	}
}
undostep(f)
	register File *f;
{
	register Buffer *t;
	register changes;
	Mark mark;
	t=f->transcript;
	changes=Fupdate(f, TRUE, TRUE);
	Bread(t, (uchar *)&mark, sizeof mark, f->markp);
	Bdelete(t, f->markp, t->nbytes);
	f->markp=mark.p;
	f->dot.r=mark.dot;
	f->mark=mark.mark;
	f->mod=mark.m;
	f->closeok=mark.s1!=Dirty;
	if(mark.s1==Dirty)
		quitok=FALSE;
	if(f->state==Clean && mark.s1==Clean && changes)
		state(f, Dirty);
	else
		state(f, mark.s1);
}
cd(str)
	String *str;
{
	register i;
	register File *f;
	readcmd(tempstr((uchar *)"/bin/pwd", 9));
	strdupstr(&wd, &genstr);
	if(wd.s[0]==0){
		wd.n=0;
		warn(Wpwd);
	}else if(wd.s[wd.n-2]=='\n'){
		--wd.n;
		wd.s[wd.n-1]='/';
	}
	if(chdir(getname((File *)0, str, FALSE)? (char *)genstr.s : home))
		syserror("chdir");
	settempfile();
	for(i=0; i<tempfile.nused; i++){
		f=tempfile.ptr[i];
		if(f!=cmd && f->name.s[0]!='/' && f->name.s[0]!=0){
			strinsert(&f->name, &wd, (Posn)0);
			sortname(f);
		}
	}
}
readcmd(s)
	String *s;
{
	if(flist==0)
		(flist=Fopen())->state=Clean;
	addr.r.p1=0, addr.r.p2=flist->nbytes;
	Unix(flist, '<', s, FALSE);
	Fupdate(flist, FALSE, FALSE);
	flist->mod=0;
	strzero(&genstr);
	strinsure(&genstr, flist->nbytes);
	Fchars(flist, genbuf, (Posn)0, flist->nbytes);
	bcopy(genbuf, genbuf+flist->nbytes, genstr.s, 1);
	genstr.n=flist->nbytes;
	straddc(&genstr, '\0');
}
loadflist(s)
	register String *s;
{
	register c, i;
	c=s->s[0];
	for(i=0; s->s[i]==' ' || s->s[i]=='\t'; i++)
		;
	if((c==' ' || c=='\t') && s->s[i]!='\n'){
		if(s->s[i]=='<'){
			strdelete(s, 0L, (long)i+1);
			readcmd(s);
		}else{
			strzero(&genstr);
			while((c=s->s[i++]) && c!='\n')
				straddc(&genstr, c);
			straddc(&genstr, '\0');
		}
	}else{
		if(c!='\n')
			error(Eblank);
		strdup(&genstr, (uchar *)"");
	}
	return genstr.s[0];
}
File *
readflist(readall, delete)
{
	register Posn i;
	register c;
	register File *f;
	for(i=0,f=0; f==0 || readall || delete; ){
		strdelete(&genstr, (Posn)0, i);
		for(i=0; (c=genstr.s[i])==' ' || c=='\t' || c=='\n'; i++)
			;
		if(i>=genstr.n)
			break;
		strdelete(&genstr, (Posn)0, i);
		for(i=0; (c=genstr.s[i]) && c!=' ' && c!='\t' && c!='\n'; i++)
			;
		if(i==0)
			break;
		genstr.s[i++]=0;
		f=lookfile();
		if(delete){
			if(f==0)
				warn_s(Wfile, (char *)genstr.s);
			else
				trytoclose(f);
		}else if(f==0 && readall)
			Fsetname(f=newfile());
	}
	return f;
}
File *
tofile(s)
	String *s;
{
	register File *f;
	if(s->s[0]!=' ')
		error(Eblank);
	if(loadflist(s)==0){
		f=lookfile();	/* empty string ==> nameless file */
		if(f==0)
			error_s(Emenu, (char *)genstr.s);
	}else if((f=readflist(FALSE, FALSE))==0)
		error_s(Emenu, (char *)genstr.s);
	return current(f);
}
File *
getfile(s)
	String *s;
{
	register File *f;
	if(loadflist(s)==0)
		Fsetname(f=newfile());
	else if((f=readflist(TRUE, FALSE))==0)
		error(Eblank);
	return current(f);
}
closefiles(f, s)
	File *f;
	register String *s;
{
	if(s->s[0]==0){
		if(f==0)
			error(Enofile);
		trytoclose(f);
		return;
	}
	if(s->s[0]!=' ')
		error(Eblank);
	if(loadflist(s)==0)
		error(Enewline);
	readflist(FALSE, TRUE);
}
move(f, addr2)
	register File *f;
	Address addr2;
{
	if(addr.r.p2<=addr2.r.p2){
		Fdelete(f, addr.r.p1, addr.r.p2);
		copy(f, addr2);
	}else if(addr.r.p1>=addr2.r.p2){
		copy(f, addr2);
		Fdelete(f, addr.r.p1, addr.r.p2);
	}else
		error(Eoverlap);
}
copy(f, addr2)
	register File *f;
	Address addr2;
{
	register Posn p;
	register ni;
	for(p=addr.r.p1; p<addr.r.p2; p+=ni){
		ni=addr.r.p2-p;
		if(ni>BLOCKSIZE)
			ni=BLOCKSIZE;
		Fchars(f, genbuf, p, p+ni);
		Finsert(addr2.f, tempstr(genbuf, ni), addr2.r.p2);
	}
	addr2.f->dot.r.p2=addr2.r.p2+(f->dot.r.p2-f->dot.r.p1);
	addr2.f->dot.r.p1=addr2.r.p2;
}
Posn
nlcount(f, p0, p1)
	register File *f;
	register Posn p0, p1;
{
	register Posn nl=0;
	Fgetcset(f, p0);
	while(p0++<p1)
		if(Fgetc(f)=='\n')
			nl++;
	return nl;
}
printposn(f, charsonly)
	register File *f;
{
	register Posn l1, l2;
	if(!charsonly){
		l1=1+nlcount(f, (Posn)0, addr.r.p1);
		l2=l1+nlcount(f, addr.r.p1, addr.r.p2);
		/* check if addr ends with '\n' */
		if(addr.r.p2>0 && addr.r.p2>addr.r.p1 && (Fgetcset(f, addr.r.p2-1),Fgetc(f)=='\n'))
			--l2;
		dprint("%lud", l1);
		if(l2!=l1)
			dprint(",%lud", l2);
		dprint("; ");
	}
	dprint("#%lud", addr.r.p1);
	if(addr.r.p2!=addr.r.p1)
		dprint(",#%lud", addr.r.p2);
	dprint("\n");
}
settempfile(){
	if(tempfile.nalloc<file.nused){
		gcfree((uchar *)tempfile.ptr);
		gcnew(tempfile.ptr, file.nused);
		tempfile.nalloc=file.nused;
	}
	tempfile.nused=file.nused;
	bcopy((uchar *)&file.ptr[0], (uchar *)&file.ptr[file.nused],
		(uchar *)&tempfile.ptr[0], 1);
}
