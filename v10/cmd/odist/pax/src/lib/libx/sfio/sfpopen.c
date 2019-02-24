#include	"sfhdr.h"

/* fork function to use */
#ifdef LIBX
#include	"FEATURE/vfork"
#else
#ifdef BSD
#define		_lib_vfork
#endif
#ifdef sparc
#define		_hdr_vfork
#endif
#endif /*LIBX*/

#ifdef _lib_vfork
#define fork	vfork
#ifdef _hdr_vfork
#include	<vfork.h>
#endif
#endif

#if __cplusplus
extern "C" {
#endif
extern int fork();
extern int wait _SFA_((int*));
extern int pipe _SFA_((int*));
extern int execl _SFA_((char*,char*,...));
#if __cplusplus
}
#endif

/* pipe ends */
#define READ	0
#define WRITE	1

/* to keep track of opened processes */
typedef struct _sfp_
{
	int		pid;	/* process id */
	Sfile_t		*one;	/* streams associated with */
	Sfile_t		*two;
	struct _sfp_	*fore;	/* link list */
} Popen_t;
static Popen_t	*Process;

#if __STDC__ || __cplusplus
static _sfpclose(reg Sfile_t* f)
#else
static _sfpclose(f)
reg Sfile_t	*f;	/* stream to close */
#endif
{
	reg Popen_t	*last, *p;
	int		pid, status;

	/* find the associated process structure */
	for(last = NIL(Popen_t*), p = Process; p; last = p, p = p->fore)
		if(f == p->one || f == p->two)
			break;

	if(!p || (p->one && SFFROZEN(p->one)) || (p->two && SFFROZEN(p->two)))
		return -1;

	/* close the associated streams */
	if(p->one)
	{
		p->one->flags &= ~SF_PROCESS;
		sfclose(p->one);
	}
	if(p->two)
	{
		p->two->flags &= ~SF_PROCESS;
		sfclose(p->two);
	}

	/* wait for process termination */
	while((pid = wait(&status)) != p->pid && pid != -1)
		;

	/* delete from process table */
	if(last)
		last->fore = p->fore;
	else	Process = p->fore;
	free((char*)p);

	return (pid == -1 ? -1 : status);
}

#if __STDC__ || __cplusplus
Sfile_t *sfpopen(const char* command, const char* mode, Sfile_t** compf)
#else
Sfile_t *sfpopen(command,mode,compf)
char	*command;	/* command to execute */
char	*mode;		/* mode of the stream */
Sfile_t	**compf;	/* to return the companion stream if mode is "x+" */
#endif
{
	reg int		pid, keep, ckeep, flg, cflg;
	reg Sfile_t	*f, *cf;
	reg Popen_t	*p;
	int		parent[2], child[2];

	/* sanity check */
	if(!command || !command[0] || !mode || (mode[0] != 'r' && mode[0] != 'w'))
		return NIL(Sfile_t*);

	/* set the close function */
	_Sfpclose = _sfpclose;

	/* make a process structure */
	if(!(p = (Popen_t*) malloc(sizeof(Popen_t))) )
		return NIL(Sfile_t*);

	/* make pipes */
	parent[0] = parent[1] = child[0] = child[1] = -1;
	if(pipe(parent) < 0)
		goto error;
	if(mode[1] == '+' && pipe(child) < 0)
		goto error;

	f = cf = NIL(Sfile_t*);
	switch(pid = fork())
	{
	default :	/* in parent process */
		/* determine which pipe ends to keep */
		if(mode[0] == 'r')
			{ keep = READ; ckeep = WRITE; flg = SF_READ; cflg = SF_WRITE; }
		else	{ keep = WRITE; ckeep = READ; flg = SF_WRITE; cflg = SF_READ; }

		/* make the streams */
		if(!(f = sfnew(NIL(Sfile_t*),NIL(uchar*),-1,parent[keep],flg)))
			goto error;
		close(parent[!keep]);

		if(mode[1] == '+')
		{	/* make the companion stream */
			if(!(cf = sfnew(NIL(Sfile_t*),NIL(uchar*),-1,child[ckeep],cflg)))
				goto error;
			close(child[!ckeep]);
			*compf = cf;
			cf->flags |= SF_PROCESS;
		}

		/* save process info */
		f->flags |= SF_PROCESS;
		p->one = f;
		p->two = cf;
		p->pid = pid;
		p->fore = Process;
		Process = p;
		return f;

	case 0 :	/* in child process */
		/* determine what to keep */
		if(mode[0] == 'r')
			{ keep = WRITE; ckeep = READ; }
		else	{ keep = READ; ckeep = WRITE; }

		/* make std-streams */
		close(keep);
		if(dup(parent[keep]) != keep)
			_exit(127);
		close(parent[!keep]);
		if(mode[1] == '+')
		{
			close(ckeep);
			if(dup(child[ckeep]) != ckeep)
				_exit(127);
			close(child[!ckeep]);
		}

		/* now exec the command */
		execl("/bin/sh", "sh", "-c", command, NIL(char*));
		_exit(127);
		return NIL(Sfile_t*);

	case -1 :	/* error */
	error:
		close(parent[0]); close(parent[1]);
		close(child[0]); close(child[1]);
		free((char*)p);
		if(f)
			sfclose(f);
		if(cf)
			sfclose(cf);
		return NIL(Sfile_t*);
	}
}
