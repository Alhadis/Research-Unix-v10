#include "jim.h"
#include "file.h"
#include "msgs.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

char errfile[64];

extern	tempfile;
char	*strcat();

Unix(f, type, unixcmd, wholefile, display)
	File *f;
	char *unixcmd;
{
	register pid, rpid;
	int (*onbpipe)();
	int retcode;
	int	pipe1[2];
	int	pipe2[2];
	long	nbytes=0;
	long	posn, ntosend;
	if(errfile[0]==0)
		sprintf(errfile, "%s/jim.err", homedir? homedir : "/tmp");
	if(wholefile){
		posn=0;
		ntosend=length(f);
	}else{
if(f->selloc>length(f) || f->selloc+f->nsel>length(f))panic("in Unix(): can't happen");
		posn=f->selloc;
		ntosend=f->nsel;
	}
	Fputblock(f);
	if(type != '!'){
		if(pipe(pipe1) == -1)
			error("couldn't make pipe", (char *)0);
	}
	if(type == '|')	/* align temp files for both sides of pipe */
		splitblock(f, (long)Fseek(f, posn));
	unlink(errfile);
	if ((pid = fork()) == 0) {
		int fd=creat(errfile, 0666);
		if(fd<0)
			fd=creat("/dev/null", 0666);
		close(2);
		dup(fd);
		close(fd);
		if(type=='<' || type=='|'){
			close(1);
			dup(pipe1[1]);
		}else if(type == '>'){
			close(0);
			dup(pipe1[0]);
			close(1);
			dup(2);
		}
		if (type != '!') {
			close(pipe1[0]);
			close(pipe1[1]);
		}
		if(type == '|'){
			if(pipe(pipe2) == -1)
				exit(1);
			if((pid=fork()) == 0){
				/*
				 * It's ok if we get SIGPIPE here
				 */
				signal(SIGPIPE, (int (*)())0);
				close(pipe2[0]);
				exit(Fwritepart(f, posn, ntosend, pipe2[1]));
			}
			if(pid == -1){
				write(2, "Can't fork\n?!", 13);
				exit(1);
			}
			close(0);
			dup(pipe2[0]);
			close(pipe2[0]);
			close(pipe2[1]);
		}
		close(tempfile);
		if(type == '<'){
			close(0);	/* so it won't read from jim.m */
			open("/dev/null", 0);	/* some people don't like closed 0 */
		}
		execl("/bin/sh", "sh", "-c", unixcmd, 0);
		exit(-1);
	}
	if(pid == -1)
		error("Can't fork", (char *)0);
	if(type=='<' || type=='|') {
		close(pipe1[1]);
		nbytes=Fread(f, posn+ntosend, (String *)0, FALSE, pipe1[0]);
		close(pipe1[0]);
	} else if(type == '>') {
		close(pipe1[0]);
		Fwritepart(f, posn, ntosend, pipe1[1]);
		close(pipe1[1]);
	}
	do; while ((rpid = wait(&retcode)) != pid && rpid != -1);
	retcode = (retcode>>8)&0377;
	if(display){
		if(type == '|' || type=='<'){
			if(retcode == 0){
				Fdeltext(f, posn, ntosend);
				if(wholefile)
					f->selloc=f->origin=0;
				f->nsel=nbytes;
			}else
				dprintf("status!=0; original not deleted.\n", (char *)0);
		}
		checkerrs();
		if(nbytes)
			modified(f);
		if(type=='<' || type=='|')
			refresh(f);
	}
}
refresh(f)
	register File *f;
{
	register x=f->nsel>32000? 32000 : f->nsel;
	send(f->id, O_SELECT, (int)(f->selloc-f->origin), 2, data2(x));
	send(f->id, O_MOVE, -1, 2, data2(32767));
	tellseek(f);
}
checkerrs(){
	struct stat statb;
	char buf[64];
	register f;
	register char *p;
	if(stat(errfile, &statb)==0 && statb.st_size){
		if((f=open(errfile, 0)) != -1){
			if(read(f, buf, sizeof buf-1)>0){
				for(p=buf; p<&buf[sizeof buf-1]; p++)
					if(*p=='\n')
						break;
				*p=0;
				dprintf("%s", buf);
				if(p-buf < statb.st_size-1)	/* -1 for newline */
					dprintf("...");
			}
			close(f);
		}
		dprintf("\n");
	} else
		unlink(errfile);
}
File *
getname(s)
	char *s;
{
	register File *f, *new=0, *old=0;
	char name[128];
	File *names;
	String text;
	int nextloc;
	/* this is slick */
	if((names=Fnew())==0)
		error("couldn't create temp buffer", (char *)0);
	Fload(Fcreat(names, ""));
	strinit(&text);
	sprintf(name, "echo %s", s);
	Unix(names, '<', name, 0, 0);
	Fsave(names, &text, 0L, length(names));
	Ffree(names);
	for(nextloc=0; nextname(name, &text, &nextloc); ){
		for(f=file[0].next; f; f=f->next)
			if(f->name.s && strcmp(name, charstar(&f->name))==0){
				if(!old)
					old=f;
				goto AlreadyThere;
			}
		f=Fnew();
		if(f==0)
			break;
		unmodified(Fcreat(f, jerqname(f, name)));
		if(!new)
			new=f;
    AlreadyThere:;
	}
	strfree(&text);
	if(new){
		dprintf("grabbing %s\n", charstar(&new->name));
		return new;
	}
	if(old)
		dprintf("grabbing %s\n", charstar(&old->name));
	return old;
}
nextname(as, str, np)
	char *as;
	String *str;
	int *np;
{
	register char *s=as;
	register c;
	do; while((c=namegetc(str, np))==' ');
	do{
		if((*s++=c)==0)
			return 0;
	}while((c=namegetc(str, np))!=' ');
	*s=0;
	return s>as;
}
namegetc(s, np)
	String *s;
	int *np;
{
	register c;
	if(*np>=s->n)
		return 0;
	if((c=s->s[*np])<' ' || (c&0200))	/* ignore frogs */
		c=' ';
	(*np)++;
	return c;
}
