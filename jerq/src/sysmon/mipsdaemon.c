#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <errno.h>
#include <nlist.h>
#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "load.h"

/* globals */
struct nlist nl[] ={
    {"sysinfo",0},
#define SYSINFO 0
    {"avenrun",0},
#define LOADAVE 1
    { 0,0 },
};
char *sys = "/unix";
char *core = "/dev/mem";
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
		mailname[0]='\0';
	nlist(sys, nl);
	if (nl[LOADAVE].n_value==0) {
		fprintf(stderr, "No namelist\n");
		return(-1);
	}
	nl[LOADAVE].n_value &= 0x7fffffff;	/*can't lseek high addrs*/
	nl[SYSINFO].n_value &= 0x7fffffff;	/*can't lseek high addrs*/
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
		p = strchr(&line[toklen], ' ');
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
	char buf[128];
	int len, err;
	long runq;
	double loadave;
	struct sysinfo si;

	if ((err=lseek(mem, (long)(nl[SYSINFO].n_value), 0))<0)
		perror("seek sysinfo");
	if ((len=read(mem, &si, sizeof(si))) != sizeof(si))
		printf("Only read sysinfo %d\n", len);
	if ((err=lseek(mem, (long)(nl[LOADAVE].n_value), 0))<0)
		perror("read load");
	if ((len=read(mem, (char *)&runq, sizeof(runq))) != sizeof(runq))
		printf("Only read load %d\n", len);
	load.l_time = time ((long *)0);
	loadave = runq/256.0;
	len = sprintf(buf, "load %f time %d vec %u %u %u %u %u\n",
		loadave, load.l_time,
		si.cpu[CPU_USER],
		0,			/* no nice reported on Sys V */
		si.cpu[CPU_KERNEL],
		si.cpu[CPU_IDLE],
		si.cpu[CPU_WAIT]);
	return(write(1, buf, len)!=len ? -1 : 0);
}
