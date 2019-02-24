static	char *sccsid = "@(#)dosys.c	8th Edition (Bell Labs) 85/10/28";
#include "defs"
#include <wait.h>
#include <errno.h>
int intrupt();
extern int errno;

#ifdef VERSION8
#	define FORK fork
#else
#	define FORK fork
#endif

dosys(comstring, nohalt, nowait, prefix)
register char *comstring;
int nohalt;
int nowait;
char *prefix;
{
int status;
register struct process *procp;

/* make sure there is room in the process stack */
if(nproc >= MAXPROC)
	waitstack(MAXPROC-1);

/* make sure fewer than proclimit processes are running */
while(proclive >= proclimit)
	{
	enbint(SIG_IGN);
	waitproc(&status);
	enbint(intrupt);
	}

if(prefix)
	{
	fputs(prefix, stdout);
	fputs(comstring, stdout);
	}

procp = procstack + nproc;
procp->pid = (forceshell || metas(comstring) ) ?
	doshell(comstring,nohalt) : doexec(comstring);
if(procp->pid == -1)
	fatal("fork failed");
procstack[nproc].nohalt = nohalt;
procstack[nproc].nowait = nowait;
procstack[nproc].done = NO;
++proclive;
++nproc;

if(nowait)
	{
	printf(" &%d\n", procp->pid);
	fflush(stdout);
	return 0;
	}
if(prefix)
	{
	putchar('\n');
	fflush(stdout);
	}
return waitstack(nproc-1);
}

metas(s)   /* Are there are any  Shell meta-characters? */
register char *s;
{
register char c;

while( (funny[c = *s++] & META) == 0 )
	;
return( c );
}




doclose()	/* Close open directory files before exec'ing */
{
register struct dirhd *od;

for (od = firstod; od; od = od->nxtdirhd)
	if(od->dirfc)
		closedir(od->dirfc);
}

/*  wait till none of the processes in the stack starting at k is live */
int waitstack(k)
int k;
{
int npending, status, totstatus;
register int i;

totstatus = 0;
npending = 0;
for(i=k ; i<nproc; ++i)
	if(! procstack[i].done)
		++npending;
enbint(SIG_IGN);
if(dbgflag > 1)
	printf("waitstack(%d)\n", k);

while(npending>0 && proclive>0)
	{
	if(waitproc(&status) >= k)
		--npending;
	totstatus |= status;
	}

if(nproc > k)
	nproc = k;
enbint(intrupt);
return totstatus;
}

waitproc(statp)
int *statp;
{
int pid;
union wait status;
register int i;
register struct process *procp;
char junk[50];
static int inwait = NO;

if(inwait)	/* avoid infinite recursions on errors */
	return MAXPROC;
inwait = YES;

pid = wait(&status);
if(dbgflag > 1)
	fprintf(stderr, "process %d done, status = %d\n", pid, status.w_status);
if(pid == -1)
	{
	if(errno == ECHILD)	/* multiple deaths, no problem */
		{
		if(proclive)
			{
			for(i=0, procp=procstack; i<nproc; ++i, ++procp)
				procp->done = YES;
			proclive = nproc = 0;
			}
		return MAXPROC;
		}
	fatal("bad wait code");
	}
for(i=0, procp=procstack; i<nproc; ++i, ++procp)
	if(procp->pid == pid)
		{
		--proclive;
		procp->done = YES;

		if(status.w_status)
			{
			if(procp->nowait)
				printf("%d: ", pid);
			if( status.w_T.w_Retcode )
				printf("*** Error code %d", status.w_T.w_Retcode );
			else	printf("*** Termination code %d", status.w_T.w_Termsig);
		
			printf(procp->nohalt ? "(ignored)\n" : "\n");
			fflush(stdout);
			if(!keepgoing && !procp->nohalt)
				fatal(CHNULL);
			}
		*statp = status.w_status;
		inwait = NO;
		return i;
		}

sprintf(junk, "spurious return from process %d", pid);
fatal(junk);
/*NOTREACHED*/
}

doshell(comstring,nohalt)
char *comstring;
int nohalt;
{
int pid;
#ifdef SHELLENV
char *getenv(), *rindex();
char *shellcom = getenv("SHELL");
char *shellstr;
#endif

if((pid = FORK()) == 0)
	{
	enbint(SIG_DFL);
	doclose();

#ifdef SHELLENV
	if (shellcom == 0) shellcom = SHELLCOM;
	shellstr = rindex(shellcom, '/') + 1;
	execl(shellcom, shellstr, (nohalt ? "-c" : "-ce"), comstring, 0);
#else
	execl(SHELLCOM, "sh", (nohalt ? "-c" : "-ce"), comstring, 0);
#endif
	fatal("Couldn't load Shell");
	}

return pid;
}

doexec(str)
register char *str;
{
register char *t, *tend;
char **argv;
register char **p;
int nargs;
int pid;

while( *str==' ' || *str=='\t' )
	++str;
if( *str == '\0' )
	return(-1);	/* no command */

nargs = 1;
for(t = str ; *t ; )
	{
	++nargs;
	while(*t!=' ' && *t!='\t' && *t!='\0')
		++t;
	if(*t)	/* replace first white space with \0, skip rest */
		for( *t++ = '\0' ; *t==' ' || *t=='\t'  ; ++t)
			;
	}

/* now allocate args array, copy pointer to start of each string,
   then terminate array with a null
*/
p = argv = (char **) ckalloc(nargs*sizeof(char *));
tend = t;
for(t = str ; t<tend ; )
	{
	*p++ = t;
	while( *t )
		++t;
	do	{
		++t;
		} while(t<tend && (*t==' ' || *t=='\t') );
	}
*p = NULL;
/*TEMP for(p=argv; *p; ++p)printf("arg=%s\n", *p); */

if((pid = FORK()) == 0)
	{
	enbint(SIG_DFL);
	doclose();
	enbint(intrupt);
	execvp(str, argv);
	printf("\n");
	fatal1("Cannot load %s",str);
	}

free( (char *) argv);
return pid;
}

#include <sys/stat.h>




touch(force, name)
int force;
char *name;
{
struct stat stbuff;
char junk[1];
int fd;

if( stat(name,&stbuff) < 0)
	if(force)
		goto create;
	else
		{
		fprintf(stderr, "touch: file %s does not exist.\n", name);
		return;
		}

if(stbuff.st_size == 0)
	goto create;

if( (fd = open(name, 2)) < 0)
	goto bad;

if( read(fd, junk, 1) < 1)
	{
	close(fd);
	goto bad;
	}
lseek(fd, 0L, 0);
if( write(fd, junk, 1) < 1 )
	{
	close(fd);
	goto bad;
	}
close(fd);
return;

bad:
	fprintf(stderr, "Cannot touch %s\n", name);
	return;

create:
	if( (fd = creat(name, 0666)) < 0)
		goto bad;
	close(fd);
}
