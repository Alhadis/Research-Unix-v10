#include <stdio.h>
#include <pwd.h>
#include <signal.h>
#include "mail.h"
#include "string.h"
#include "process.h"

/*
 *  the following is pretty safe because upas is very stingy
 *  in its file descriptor use.  system V fanatics should
 *  use the approriate system V system call to get the number
 *  of file descriptors
 */
#ifndef NOFILE
#define NOFILE 20
#endif
#ifndef NSYSFILE
#define NSYSFILE 3
#endif

extern char *malloc();
extern void exit();
extern char *getenv();

/* make a stream to a child process */
extern stream *
instream()
{
	stream *rv;
	int pfd[2];

	if ((rv = (stream *)malloc(sizeof(stream))) == NULL)
		return NULL;
	if (pipe(pfd) < 0)
		return NULL;
	if ((rv->fp = fdopen(pfd[1], "w")) == NULL){
		close(pfd[0]);
		close(pfd[1]);
		return NULL;
	}
	rv->fd = pfd[0];	
	return rv;
}

/* make a stream from a child process */
extern stream *
outstream()
{
	stream *rv;
	int pfd[2];

	if ((rv = (stream *)malloc(sizeof(stream))) == NULL)
		return NULL;
	if (pipe(pfd) < 0)
		return NULL;
	if ((rv->fp = fdopen(pfd[0], "r")) == NULL){
		close(pfd[0]);
		close(pfd[1]);
		return NULL;
	}
	rv->fd = pfd[1];	
	return rv;
}

extern void
stream_free(sp)
	stream *sp;
{
	close(sp->fd);
	fclose(sp->fp);
	free((char *)sp);
}

static char *env[] = { "IFS= \t\n", 0, 0, 0, 0 };
static string *spath;
static string *home;
static string *tz;

/* start a new process */
extern process *
proc_start(cmd, inp, outp, errp, uid)
	char *cmd;			/* command to run */
	stream *inp, *outp, *errp;	/* streams to be used */
{
	struct passwd *pw;
	process *pp;
	int i;
	char *tzp;

	if ((pp = (process *)malloc(sizeof(process))) == NULL) {
		if (inp != NULL)
			stream_free(inp);
		if (outp != NULL)
			stream_free(outp);
		if (errp != NULL)
			stream_free(errp);
		return NULL;
	}
	pp->std[0] = inp;
	pp->std[1] = outp;
	pp->std[2] = errp;
	switch (pp->pid = fork()) {
	case -1:
		proc_free(pp);
		return NULL;
	case 0:
		fflush(stdout);
		if (spath==NULL) {
			spath = s_new();
			s_append(spath, "PATH=");
			s_append(spath, UPASROOT);
			s_append(spath, ":/bin:/usr/bin");
			home = s_new();
			env[1] = s_to_c(spath);
		}
		uid=(uid<0)?getuid():uid;
		pw = getpwuid(uid);
		if (pw != NULL){
			setlogname(pw->pw_name);
			s_append(s_restart(home), "HOME=");
			s_append(home, pw->pw_dir);
			env[2] = s_to_c(home);
		} else
			env[2] = "HOME=/";
		if (tz==NULL) {
			if(tzp = getenv("TZ")){
				tz = s_new();
				s_append(tz, "TZ=");
				s_append(tz, tzp);
				env[3] = s_to_c(tz);
			}
		}
		setgid(-1);
		setuid(uid);
		for (i=0; i<3; i++)
			if (pp->std[i] != NULL)
				fclose(pp->std[i]->fp);
		for (i=0; i<NSYSFILE; i++)
			if (pp->std[i] != NULL)
				dup2(pp->std[i]->fd, i);
		for (i = NSYSFILE; i < NOFILE; i++)
			close(i);
		execle("/bin/sh", "sh", "-c", cmd, 0, env);
		perror("proc_start");
		exit(1);
	default:
		for (i=0; i<3; i++)
			if (pp->std[i] != NULL) {
				close(pp->std[i]->fd);
				pp->std[i]->fd = -1;
			}
		return pp;
	}
}

/* wait for a process to stop */
extern int
proc_wait(pp)
	process *pp;		/* description of process */
{
	int status=0;
	int pid;

	while((pid = wait(&status))>=0) {
		if (pid==pp->pid)
			break;
	}
	pp->pid = -1;
	pp->status = status;
	return status;
}

/* free a process */
extern int
proc_free(pp)
	process *pp;
{
	int i;

	for (i = 0; i < 3; i++)
		if (pp->std[i])
			stream_free(pp->std[i]);
	if (pp->pid >= 0){
		kill(pp->pid, SIGKILL);
		(void)proc_wait(pp);
	}
	free((char *)pp);
	return 0;
}
