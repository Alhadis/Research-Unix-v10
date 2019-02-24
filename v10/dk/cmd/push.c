/*%cc -o push % -ldk
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/stat.h>
extern char *strcat();

char targetdir[128];

char SERVER[]="/usr/lib/Rpull";
int server=2;	/* should be >1 until proven guilty */

char *
basename(s)
	register char *s;
{
	extern char *strrchr();
	register char *t;
	t=strrchr(s, '/');
	return(t? t+1 : s);
}

int errors=0;
int vflag;

main(argc, argv)
	char *argv[];
{
	register i;
	int status;
	register pid;
	char buf[129];
	if(argc>1 && strcmp(argv[1], "-v")==0){
		vflag=1;
		argv++;
		--argc;
	}
	if(argc<4){
		error("Usage: push machine <files|dirs> remotedir>", "");
		exit(1);
	}
	(void)strcat(strcpy(targetdir, argv[argc-1]), "/");
	if(basename(argv[0])[0]=='R')
		server=1;
	else{
		/* Set up the remote process */
		sprintf(buf, "%s %s", SERVER, argv[argc-1]);
		server=tdkexec(argv[1], buf);
		if(server<0){
			extern char *dkerror;
			fprintf(stderr, "push: can't execute %s (%s)\n", SERVER,
				dkerror);
			exit(1);
		}
		/* Must print remote errors locally */
		if((pid=fork())==0){
			while((i=read(server, buf, sizeof buf))>0)
				report(buf, i);
			exit(0);
		}else if(pid==-1){
			error("can't fork", "try again");
			exit(1);
		}
	}
	for(i=2; i<argc-1; i++)
		sendfile(argv[i]);
	write(server, buf, 0);		/* End of file */
	if(server>1)
		wait(&status);		/* To get all the error messages */
	return(errors);
}

#define	NDIRSTACK 20
char directory[128];		/* Name buffer for directories */
char *dirstack[NDIRSTACK];
char *basep=directory;
int dirstkp=0;

pushdir(s, m)
	register char *s;
{
	char buf[128];
	if(dirstkp>=NDIRSTACK){
		error("directory stack overflow", "");
		exit(1);
	}
	dirstack[dirstkp++]=directory+strlen(directory);
	(void)strcat(directory, s);
	if(dirstkp==1)	/* First entry on stack */
		basep=basename(directory);
	sprintf(buf, "%03o%s%s", m, targetdir, basep);
	message('D', buf);
	(void)strcat(directory, "/");
}

popdir()
{
	if(dirstkp<=0){
		error("directory stack underflow", "");
		exit(1);
	}
	*dirstack[--dirstkp]='\0';
	if(dirstkp<=0)
		basep=directory;
}


sendfile(s)
	register char *s;
{
	struct stat statbuf;
	char filebuf[128];
	char remotebuf[128];
	(void)strcat(strcpy(filebuf, directory), s);
	if(stat(filebuf, &statbuf)<0){
		error("can't stat", s);
		return;
	}
	if(server>1 && vflag)
		printf("push: send %s\n", filebuf);
	switch(statbuf.st_mode&S_IFMT){
	case S_IFDIR:
		senddir(filebuf, s, statbuf.st_mode&0777);
		break;
	case S_IFREG:
		sprintf(remotebuf, "%03o%s%s%s", statbuf.st_mode&0777,
			targetdir, basep, basename(s));
		senddata(filebuf, remotebuf);
		break;
	case S_IFBLK:
	case S_IFCHR:
		error(s, "is a special file; can't send it");
		break;
	}
}

senddata(s, remote)
	register char *s, *remote;
{
	char buf[1024];
	register f, cc;
	if((f=open(s, 0))<0){
		error("can't open", s);
		return;
	}
	message('F', remote);
	while((cc=read(f, buf, sizeof buf))>0)
		write(server, buf, cc);
	write(server, buf, 0);
	close(f);
}

senddir(fullname, localname, m)
	register char *fullname, *localname;
{
	/* Same as <sys/dir.h> but with DIRSIZ+1 chars */
	struct direct15{
		ino_t	d_ino;
		char	d_name[DIRSIZ+1];
	}direct15;
	register df;
	if((df=open(fullname, 0))<0){
		error("can't open directory", fullname);
		return;
	}
	pushdir(localname, m);
	direct15.d_name[14]='\0';
	while(read(df, &direct15, sizeof(struct direct))==sizeof(struct direct)){
		if(direct15.d_ino && strcmp(direct15.d_name, ".")!=0
				  && strcmp(direct15.d_name, "..")!=0)
			sendfile(direct15.d_name);
	}
	popdir();
	close(df);
}
error(s, t)
	char *s, *t;
{
	char ebuf[512];
	errors=1;
	if(server>1)
		fprintf(stderr, "push: %s %s\n", s, t);
	else{
		sprintf(ebuf, "%s %s", s, t);
		message('E', ebuf);
	}
}

report(s, n)
	register char *s;
{
	static char msgbuf[512];
	static char *msgp=msgbuf;
	register i;
	for(i=0; i<n; i++){
		*msgp= *s++;
		if(*msgp++=='\n'){
			write(2, "push: (remote) ", 14);
			write(2, msgbuf, msgp-msgbuf);
			msgp=msgbuf;
		}
	}
}

message(c, s)
	register char *s;
{
	char buf[128];
	buf[0]=c;
	strcpy(buf+1, s);
	write(server, buf, strlen(buf));
	write(server, buf, 0);		/* Terminate & synchronize */
}
