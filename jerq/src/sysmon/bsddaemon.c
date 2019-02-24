#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <nlist.h>
#include <stdio.h>
#include <libc.h>
#include "defs.h"
#include "load.h"

#define NULL 0

/* globals */
struct nlist nl[] ={
#define CPUTIME 0
    {"_cp_time",0},
#define LOADAVE 1
    {"_avenrun",0},
    { 0,0 },
};
#ifndef BSD
char *sys = "/unix";
#else
char *sys = "/vmunix";
#endif
char *core = "/dev/kmem";
char *log = "/usr/spool/mail/mail.log";
int mem;
int errno;
char token[64];
char toklen;
long loglen;
FILE *mf;

main ()
{
	long before, now;

	if(init()<0)
		return(1);
	while (1) {
		if(doload() < 0)
			return(0);
		now=time((long *)0);
		if(now-before>15){
			before=now;
			if(domail() < 0)
				return(0);
		}

		/* don't hog the CPU */
		sleep (5);
	}
}

/*
 * find where load info resides in kernel and
 * create token to search for in mail log
 */
init()
{
	struct stat st;
	static char mailname[32];

	/* get mail name */
	read(0, mailname, sizeof(mailname)-1);
	if(*mailname=='\0')
		strcpy(mailname, getlogin());
	donlist(sys, nl);
	mem = open(core, 0);
	if (mem<0) {
		fprintf(stderr, "can't open %s\n", core);
		return(-1);
	}
	strcpy(token, "delivered ");
	strcat(token, mailname);
	strcat(token, " From ");
	toklen=strlen(token);
	if(stat(log, &st)>=0){
		loglen=st.st_size;
		mf=fopen(log, "r");
	}
	return(0);
}

/*
 * do nlist in another process to keep swap space down
 *	norman, this one's for you
 */
donlist()
{
	int pfd[2];
	long status;

	if(pipe(pfd)<0){
		/* might as well just do it */
		nlist(sys, nl);
		return;
	}
	switch(fork()){
	case -1:
		/* might as well just do it */
		nlist(sys, nl);
		return;
	case 0:
		/* do nlist in child */
		nlist(sys, nl);

		/* give result to parent */
		write(pfd[1], nl, sizeof(nl));

		exit(0);
	default:
		/* get result */
		read(pfd[0], nl, sizeof(nl));
		close(pfd[0]);
		close(pfd[1]);
		wait(&status);
		break;
	}
}

/*
 * notify about mail received
 */
domail()
{
	char *p;
	char buf[128];
	char line[256];
	int len;
	struct stat st;

	/* get the size of the log file (it may have shrunk) */
	if(stat(log, &st)<0){
		if(mf!=NULL)
			fclose(mf);
		mf=NULL;
		loglen=0;
		return(0);
	}
	if(st.st_size<loglen){
		if(mf!=NULL)
			fclose(mf);
		mf=NULL;
		loglen=0;
	}
	if(mf==NULL)
		mf=fopen(log, "r");
	if(mf==NULL)
		return(0);

	/* check for mail if its time */
	if (fseek(mf, loglen, 0) < 0)
		return(0);
	while (fgets(line, sizeof(line), mf) != NULL) {
		loglen += strlen(line);
		if (strncmp(token, line, toklen)!=0)
			continue;
		p = (char *)strchr(&line[toklen], ' ');
		if (p==NULL)
			continue;
		*p = '\0';
		strcpy(buf, "from ");
		strcat(buf, &line[toklen]);
		strcat(buf, "\n");
		len = strlen(buf);
		if (write (1, buf, len) != len)
			return(-1);
	}
	return (0);
}

/* print the load */
doload()
{
	char buf[256];
	int len;

	lseek(mem, (long)nl[CPUTIME].n_value, 0);
	read(mem, (char *)load.l_cp, sizeof(load.l_cp));
	lseek(mem, (long)nl[LOADAVE].n_value, 0);
	read(mem, (char *)&(load.l_runq), sizeof(load.l_runq));
	load.l_time = time ((long *)0);
	sprintf(buf, "load %f time %d vec %d %d %d %d %d\n",
		load.l_runq, load.l_time, load.l_cp[0], load.l_cp[1],
		load.l_cp[2], load.l_cp[3], load.l_cp[4]);
	len = strlen(buf);
	return(write(1, buf, len)!=len ? -1 : 0);
}
