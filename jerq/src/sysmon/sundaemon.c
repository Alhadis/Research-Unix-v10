#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <nlist.h>
#include <stdio.h>
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
char *sys = "/vmunix";
char *core = "/dev/kmem";
char *log = "/usr/spool/mail/mail.log";
int mem;
int errno;
char token[64];
char toklen;
long loglen;
FILE *mf;
FILE *vislog;
int debug=0;
#define DEBUG	if (debug)

main ()
{
	long before, now;

	DEBUG vislog = fopen("/tmp/vislog", "a");
	DEBUG setbuf(vislog, 0);
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
	int err, len;

	/* get mail name */
	if((len=read(0, mailname, sizeof(mailname)-1)) <= 0) {
		DEBUG fprintf(vislog, "mail name read error %d %d", errno, len);
	}
	if(*mailname=='\0') {
		char *logname, *getlogin(), *getpwuid();
		if ((logname=getlogin())==NULL)
			logname=getpwuid(getuid());
		strcpy(mailname, logname);
	}
	if ((err=nlist(sys, nl))!=0) {
		DEBUG fprintf(vislog, "%d nlist entries not found\n", err);
		return(-1);
	}
	mem = open(core, 0);
	if (mem<0) {
		DEBUG fprintf(vislog, "can't open %s\n", core);
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
		p = (char *)strchr(&line[toklen], ' ');
		if (p==NULL)
			continue;
		*p = '\0';
		strcpy(buf, "from ");
		strcat(buf, &line[toklen]);
		strcat(buf, "\n");
		len = strlen(buf);
		if (write (1, buf, len) != len) {
			DEBUG fprintf(vislog, "Error writing mail file %d\n", errno);
			return(-1);
		}
	}
	return (0);
}

/* print the load */
doload()
{
	double fabs();
	char buf[256];
	int len, ret;
	long loadave[3];

	if (lseek(mem, (long)nl[CPUTIME].n_value, 0) < 0) {
		DEBUG fprintf(vislog, "Error seeking CPU %d\n", errno);
		return -1;
	};
	if ((len=read(mem, (char *)load.l_cp, sizeof(load.l_cp)))
			!= sizeof(load.l_cp)) {
		DEBUG fprintf(vislog, "Error reading CPU %d\n", errno);
		return -1;
	};
	if (lseek(mem, (long)nl[LOADAVE].n_value, 0) < 0) {
		DEBUG fprintf(vislog, "Error seeking load %d\n", errno);
		return -1;
	};
	if ((len=read(mem, (char *)loadave, sizeof(loadave)))
			!= sizeof(loadave)) {
		DEBUG fprintf(vislog, "Error reading load %d\n", errno);
		return -1;
	};
	load.l_time = time ((long *)0);
	sprintf(buf, "load %f time %d vec %d %d %d %d %d\n",
		loadave[0]/256.0, load.l_time, load.l_cp[0], load.l_cp[1],
		load.l_cp[2], load.l_cp[3], load.l_cp[4]);
	len = strlen(buf);
	DEBUG fprintf(vislog, "writing  %s", buf);
	if ((ret=write(1, buf, len))!=len) {
		DEBUG fprintf(vislog, "write returned err %d  len=%d\n", errno, ret);
		return(-1);
	}
	return(0);
}
