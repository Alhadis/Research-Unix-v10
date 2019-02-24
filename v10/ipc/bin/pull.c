/*%cc -o pull % -ldk
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <ipc.h>
#include <libc.h>

extern int errno;

char SERVER[]="Rpush";
int server;

#define MAXCHARS 8192
char args[MAXCHARS];

char *bldargs(argv)
	register char *argv[];
{
	register char *s, *t;
	extern char *strcpy();
	s=strcpy(args, SERVER);
	s+=strlen(s);
	*s++=' ';
	while(t= *argv++){	/* assignment = */
		while(*s = *t++)
			if(s++ >= &args[MAXCHARS-1]){
				error("arg list too long", "");
				exit(1);
			}
		*s++=' ';
	}
	s[-1]='\0';
	return(args);
}

char *
basename(s)
	register char *s;
{
	extern char *strrchr();
	register char *t;
	t=strrchr(s, '/');
	return(t? t+1 : s);
}

char *
str(s, n)
	register char *s;
{
	static char strbuf[129];
	(void)strncpy(strbuf, s, n);
	strbuf[n]=0;
	return(strbuf);
}

int errors=0;
int vflag;

main(argc, argv)
	char *argv[];
{
	register n;
	char *targetdir;
	char buf[512];

	if(argc>1 && strcmp(argv[1], "-v")==0){
		vflag=1;
		argv++;
		--argc;
	}
	if(basename(argv[0])[0]=='R'){
		if(argc!=2){
			error("arg count", "");	/* It's ok; other guy will decrypt it */
			exit(1);
		}
		targetdir=argv[1];
	}else{
		if(argc<4){
			error("Usage: pull machine <remotefiles|dirs> localdir", "");
			exit(1);
		}
		targetdir=argv[argc-1];
		server=0;	/* Standard input */
		server=ipcexec(argv[1], "heavy delim hup", bldargs(&argv[1]));
		if(server<0){
			fprintf(stderr, "pull: can't execute %s (%s)\n", SERVER,
				errstr);
			exit(1);
		}
	}
	if(mkdir(targetdir, 0777)==0){
		error("can't make directory", argv[1]);
		exit(1);
	}
	while((n=read(server, buf, sizeof buf - 1))>0){
		switch(buf[0]){
		case 'D':
			if(mkdir(str(buf+4, n-4), o3(buf+1))==0)
				error("can't make directory", str(buf+1, n-1));
			break;
		case 'E':
			error("(remote)", str(buf+1, n-1));
			break;
		case 'F':
			createfile(str(buf+4, n-4), o3(buf+1));
			continue;	/* createfile() will read null message */
		default:
			buf[n]='\0';
			error("unknown message", buf);
			break;
		}
		if(read(server, buf, sizeof buf)!=0){
			error("synchronization error", "");
			do; while(read(server, buf, sizeof buf)>0);
		}
	}
	return(errors);
}

createfile(s, m)
	register char *s;
{
	char buf[512];
	register f;
	register cc;
	if(server>1 && vflag)
		printf("pull: receive %s\n", s);
	if((f=creat(s, m))<0)
		error("can't create", s);
	else{
		if(read(server, buf, sizeof buf)>0){
			error("synchronization", "error");
			close(f);
			return;
		}
		while((cc=read(server, buf, sizeof buf))>0)
			if(write(f, buf, cc)!=cc){
				error("write error", s);
				if(errno==ENOSPC){
					error("file system is full; aborting", "");
					exit(1);
				}
				close(f);
				return;
			}
		close(f);
	}
}

error(s, t)
	char *s, *t;
{
	char ebuf[128];
	errors=1;
	sprintf(ebuf, "%s%s %s\n", server>0? "pull: " : "" ,s, t);
	write(2, ebuf, strlen(ebuf));
}

mkdir(s, m)
	register char *s;
{
	struct stat statbuf;
	int status;
	if(stat(s, &statbuf)<0){
		if(fork()==0){
			execl("/bin/mkdir", "mkdir", s, 0);
			exit(1);
		}
		(void)wait(&status);
		chmod(s, m);
		return(!status);
	}
	return((statbuf.st_mode&S_IFMT)==S_IFDIR);
}

o3(s)
	register char *s;
{
	return(((s[0]-'0')*0100)+((s[1]-'0')*010)+(s[2]-'0'));
}
