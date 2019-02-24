/*
 * jx.c - plumbing and setup for system call interpreter
 */

#include <stdio.h>
#include <sgtty.h>
#include <jioctl.h>

struct sgttyb	sttymodes, sttysave, sttychars;
int		jerq, used, ttyout, ttyerr;
char		*getenv();
char		outfile[50], errfile[50];
int		wflag, zflag;
char		*malloc();
char		loader[128];
char		sysint[128];

#define	LOADER	"/bin/32ld"
#define	SYSINT	"/lib/sysint"

main(argc, argv)
	int argc;
	char *argv[];
{
	int i, status, pid, w;
	char s[100],**args;
	strcpy(loader, getenv("DMD")? getenv("DMD") : "/usr/jerq");
	strcpy(sysint, loader);
	strcat(sysint,SYSINT);
	strcat(loader,LOADER);
	while (*(argv[1]) == '-') {
		argv++;
		switch (argv[0][1]) {
		case 'w':
			wflag++;
			break;
		case 'z':
			zflag++;
			break;
		}
	}
	if ((jerq = open("/dev/tty", 2)) == -1){
		fprintf(stderr, "jx: can't open /dev/tty");
		exit(1);
	}
	pushmode();
	if ((args = (char **)malloc((argc+5)*sizeof(char *)))==0) /* 5 enough? */
		error("can't allocate argument area");
	if ((pid = fork()) == 0) {
		char **startargs=args;
		close(0);
		close(1);
		close(2);
		dup(jerq); dup(jerq); dup(jerq);
		args[0] = "32ld";
		if(zflag){
			args++;
			args[0] = "-z";
		}
		for (i = 1; argv[i] != NULL; i++)
			args[i] = argv[i];
		args[i] = NULL;
		execv(loader, startargs);
		fprintf(stderr, "jx: can't exec %s\n", loader);
		_exit(127);
	} else if(pid == -1)
		error("can't fork");
	while ((w = wait(&status)) != pid && w != -1)
		;
	if(status!=0){
		popmode();
		exit(1);
	}
	ttyout = isatty(stdout);
	sprintf(outfile,"%s/.jxout",getenv("HOME"));
	ttyerr = isatty(stderr);
	sprintf(errfile,"%s/.jxerr",getenv("HOME"));
	if ((pid = fork()) == 0) {
		if (ttyout)
			freopen(outfile,"w",stdout);
		if (ttyerr)
			freopen(errfile,"w",stderr);
		args[0] = "sysint";
		args[1] = NULL;
		execv(sysint,args);
		fprintf(stderr, "can't exec sysint");
		_exit(127);
	} else if(pid == -1)
		error("can't fork");
	while ((w = wait(&status)) != pid && w != -1)
		;
	terminal();
	sleep(2);
	if(status!=0)
		printf("jx: standard i/o interpreter didn't exit normally\n");
	if (ttyout)
		cat(outfile,stdout);
	if (ttyerr)
		cat(errfile,stderr);
	return 0;
}

pushmode()
{
	ioctl(jerq, TIOCGETP, &sttymodes);
	sttysave=sttymodes;
	sttymodes.sg_flags|=RAW;
	sttymodes.sg_flags|=ANYP;
	sttymodes.sg_flags&=~ECHO;
	ioctl(jerq, TIOCSETP, &sttymodes);
}

popmode()
{
	ioctl(jerq, TIOCSETP, &sttysave);
}


terminal(){
	ioctl(jerq, JTERM, 0);
	popmode();
}

isatty(f)
FILE *f;
{
	struct sgttyb junk;
	return(gtty(fileno(f),&junk) == 0);
}

error(s)
char *s;
{
	fprintf(stderr,"jx: %s\n",s);
	terminal();
	exit(1);
}

cat(s,ouf)
char *s;
FILE *ouf;
{
	register c;
	FILE *inf;
	if ((inf = fopen(s,"r")) != NULL) {
		while ((c = getc(inf)) != EOF)
			putc(c,ouf);
		fclose(inf);
	}
}
