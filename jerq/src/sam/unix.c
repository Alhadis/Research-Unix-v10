#include "sam.h"
#include "parse.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <setjmp.h>

extern	jmp_buf	mainloop;

uchar	errfile[64];
String	unixcmd;	/* null terminated */
Buffer	*unixbuf;

Unix(f, type, s, nest)
	register File *f;
	register type;
	register String *s;
{
	register pid, rpid;
	long l;
	int m;
	int (*onbpipe)();
	int retcode;
	int pipe1[2], pipe2[2];
	if(s->s[0]==0 && unixcmd.s[0]==0)
		error(Enocmd);
	else if(s->s[0])
		strdupstr(&unixcmd, s);
	if(downloaded){
		strcpy(errfile, (uchar *)home);
		strcat((char *)errfile, "/sam.err");
	}else
		strcpy(errfile, (uchar *)"/dev/tty");
	if(type!='!' && pipe(pipe1)==-1)
		error(Epipe);
	if(type=='|')
		snarf(f, addr.r.p1, addr.r.p2, unixbuf, 1);
	if(downloaded)
		unlink((char *)errfile);
	if((pid=fork()) == 0){
		if(downloaded){	/* also put nasty fd's into errfile */
			int fd=creat((char *)errfile, 0666);
			if(fd<0)
				fd=creat("/dev/null", 0666);
#ifdef	V8
			close(3);	/* v8 /dev/tty */
			open("/dev/null", 2);
#endif
			close(2);
			dup(fd);
			close(fd);
			/* 2 now points at err file */
			close(1);
			if(type == '>')
				dup(2);
			else if(type=='!'){
				dup(2);
				fd=open("/dev/null", 0);
				close(0);
				dup(fd);
				close(fd);
			}
		}
		if(type!='!') {
			if(type=='<' || type=='|'){
				close(1);
				dup(pipe1[1]);
			}else if(type == '>'){
				close(0);
				dup(pipe1[0]);
			}
			close(pipe1[0]);
			close(pipe1[1]);
		}
		if(type=='|'){
			if(pipe(pipe2)==-1)
				exit(1);
			if((pid=fork())==0){
				/*
				 * It's ok if we get SIGPIPE here
				 */
				close(pipe2[0]);
				if(retcode=!setjmp(mainloop))	/* assignment = */
					for(l=0; l<unixbuf->nbytes; l+=m){
						m=unixbuf->nbytes-l;
						if(m>BLOCKSIZE)
							m=BLOCKSIZE;
						Bread(unixbuf, genbuf, m, l);
						(void)Write(pipe2[1], genbuf, m);
					}
				exit(retcode);
			}
			if(pid==-1){
				fprint(2, "Can't fork?!\n");
				exit(1);
			}
			close(0);
			dup(pipe2[0]);
			close(pipe2[0]);
			close(pipe2[1]);
		}
		Dclosefd();
		if(type=='<'){
			close(0);	/* so it won't read from terminal */
			open("/dev/null", 0);
		}
		execlp("sh", "sh", "-c", unixcmd.s, (char *)0);
		exit(-1);
	}
	if(pid==-1)
		error(Efork);
	if(type=='<' || type=='|'){
		int nonascii;
		if(downloaded)
			outTl(Hsnarflen, addr.r.p2-addr.r.p1);
		snarf(f, addr.r.p1, addr.r.p2, snarfbuf, 0);
		Fdelete(f, addr.r.p1, addr.r.p2);
		close(pipe1[1]);
		io=pipe1[0];
		f->tdot.p1=-1;
		f->dot.r.p2=addr.r.p2+readio(f, &nonascii, 0);
		f->dot.r.p1=addr.r.p2;
		closeio((Posn)-1);
	}else if(type=='>'){
		onbpipe = signal(SIGPIPE, SIG_IGN);
		close(pipe1[0]);
		io=pipe1[1];
		(void)writeio(f);
		closeio((Posn)-1);
		signal(SIGPIPE, onbpipe);
	}
	do; while ((rpid = wait(&retcode)) != pid && rpid != -1);
	retcode=(retcode>>8)&0377;
	if(type=='|' || type=='<')
		if(retcode!=0)
			warn(Wbadstatus);
	if(downloaded)
		checkerrs();
	if(!nest)
		dprint("!\n");
}
checkerrs(){
	struct stat statb;
	char buf[256];
	register f;
	register n, nl;
	register char *p;
	if(stat((char *)errfile, &statb)==0 && statb.st_size){
		if((f=open((char *)errfile, 0)) != -1){
			if((n=read(f, buf, sizeof buf-1))>0){
				for(nl=0,p=buf; nl<3 && p<&buf[n]; p++)
					if(*p=='\n')
						nl++;
				*p=0;
				dprint("%s", buf);
				if(p-buf < statb.st_size-1)
					dprint("(sam: more in %s)\n", errfile);
			}
			close(f);
		}
	}else
		unlink((char *)errfile);
}
