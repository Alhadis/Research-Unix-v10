#include	"mk.h"

typedef struct Event
{
	int pid;
	Job *job;
} Event;
static Event *events;
static int nevents, nrunning;
typedef struct Process
{
	int pid;
	int status;
	struct Process *b, *f;
} Process;
static Process *phead, *pfree;
static void pnew(), pdelete();
static char *envy[1024];
static char **special;
static pidslot();

run(j)
	Job *j;
{
	register Job *jj;

	if(jobs){
		for(jj = jobs; jj->next; jj = jj->next)
			;
		jj->next = j;
	} else 
		jobs = j;
	j->next = 0;
	/* this code also in waitup after parse redirect */
	if(nrunning < nproclimit)
		sched();
}

sched()
{
	register Job *j;
	char buf[BIGBLOCK];
	int slot, pip[2], pid;
	Node *n;

	if(jobs == 0){
		account();
		return;
	}
	j = jobs;
	jobs = j->next;
	if(DEBUG(D_EXEC))
		fprint(1, "firing up job for target %s\n", wtos(j->t));
	slot = nextslot();
	events[slot].job = j;
	dovars(j, slot);
	shprint(j->r->recipe, envy, buf);
	if(!tflag && !mflag && (nflag || !(j->r->attr&QUIET)))
		Fwrite(1, buf, (long)strlen(buf));
	if(mflag){
		for(n = j->n; n; n = n->next)
			symlook(n->name, S_MAKEFILE, (char *)j);
	}
	if(nflag||tflag){
		for(n = j->n; n; n = n->next){
			if(tflag){
				if(!(n->flags&VIRTUAL))
					touch(n->name);
				else if(explain)
					Fprint(1, "no touch of virtual '%s'\n", n->name);
			}
			n->time = time((long *)0);
			MADESET(n, MADE);
		}
	} else {
		Fexit(0);
		if(j->r->attr&RED){
			if(pipe(pip) < 0){
				perror("pipe");
				Exit();
			}
		}
		if((pid = fork()) < 0){
			perror("mk fork");
			Exit();
		}
		if(pid == 0){
			if(j->r->attr&RED){
				close(pip[0]);
				dup2(pip[1], 1);
				close(pip[1]);
			}
			if(pipe(pip) < 0){
				perror("pipe-i");
				Exit();
			}
			if((pid = fork()) < 0){
				perror("mk fork");
				Exit();
			}
			if(pid != 0){
				close(pip[1]);
				dup2(pip[0], 0);
				close(pip[0]);
				execle(SHELL, "sh", "-e", (char *)0, envy);
				perror(SHELL);
				_exit(1);
			} else {
				int k;
				char *s, *send;

				close(pip[0]);
				s = j->r->recipe;
				send = s+strlen(s);
				while(s < send){
					if((k = write(pip[1], s, send-s)) < 0)
						break;
					s += k;
				}
				_exit(0);
			}
		}
		account();
		nrunning++;
		if(j->r->attr&RED)
			close(pip[1]), j->fd = pip[0];
		else
			j->fd = -1;
		if(DEBUG(D_EXEC))
			fprint(1, "pid for target %s = %d\n", wtos(j->t), pid);
		events[slot].pid = pid;
	}
}

waitup(echildok, retstatus)
	int *retstatus;
{
	int status, pid;
	int slot;
	Symtab *s;
	Word *w;
	Job *j;
	char buf[64];
	char buf1[BIGBLOCK];
	int uarg = 0;
	int done;
	Node *n;
	Process *p;
	extern int errno, runerrs;

	/* first check against the proces slist */
	if(retstatus)
		for(p = phead; p; p = p->f)
			if(p->pid == *retstatus){
				*retstatus = p->status;
				pdelete(p);
				return(-1);
			}
again:		/* rogue processes */
	if((pid = wait(&status)) < 0){
		if(echildok > 0){
			return(1);
		} else {
			fprint(2, "mk: (waitup %d) ", echildok);
			perror("mk wait");
			Exit();
		}
	}
	if(DEBUG(D_EXEC))
		fprint(1, "waitup got pid=%d, status=0x%ux\n", pid, status);
	if(retstatus && (pid == *retstatus)){
		*retstatus = status;
		return(-1);
	}
	slot = pidslot(pid);
	if(slot < 0){
		if(DEBUG(D_EXEC))
			fprint(2, "mk: wait returned unexpected process %d\n", pid);
		pnew(pid, status);
		goto again;
	}
	j = events[slot].job;
	account();
	nrunning--;
	events[slot].pid = -1;
	if(status){
		dovars(j, slot);
		shprint(j->r->recipe, envy, buf1);
		front(buf1);
		Fprint(2, "mk: %s: exit status=%d", buf1, 0xFF&(status>>8));
		status &= 0xFF;
		if(status&0x7F)
			Fprint(2, " signal=%d", status&0x7F);
		if(status&0x80)
			Fprint(2, ", core dumped");
		for(n = j->n, done = 0; n; n = n->next)
			if(n->flags&DELETE){
				if(done++ == 0)
					Fprint(2, ", deleting");
				Fprint(2, " '%s'", n->name);
			}
		Fputc(2, '\n');
		for(n = j->n, done = 0; n; n = n->next)
			if(n->flags&DELETE){
				if(done++ == 0)
					Fflush(2);
				delete(n->name);
			}
		if(kflag){
			runerrs++;
			uarg = 1;
			Fflush(2);
		} else {
			jobs = 0;
			Exit();
		}
	}
	if(j->fd >= 0){
		sprint(buf, "process %d", pid);
		parse(buf, j->fd, 0, 0);
		execinit();	/* reread environ */
		nproc();
		while(jobs && (nrunning < nproclimit))
			sched();
	}
	for(w = j->t; w; w = w->next){
		if((s = symlook(w->s, S_NODE, (char *)0)) == 0)
			continue;	/* not interested in this node */
		update(uarg, (Node *)s->value);
	}
	if(nrunning < nproclimit)
		sched();
	return(0);
}

execinit()
{
	extern char **environ;
	extern char **vardump();
	register char *s, *ss, c;
	Symtab *st;

	environ = envy;
	special = vardump(envy);
	if(st = symlook("ENVIRON", S_VAR, (char *)0))
		for(s = st->value; *s;){
			for(ss = s; *ss && (*ss != 1); ss++);
			c = *ss;
			*ss = 0;
			*special++ = strdup(s);
			s = ss;
			if(*ss = c)
				s++;
		}
	*special = 0;
}

char *myenv[] =
{
	"target", "stem", "prereq", "pid", "nproc", "newprereq",
	"alltarget",
	"stem1", "stem2", "stem3", "stem4", "stem5", "stem6",
	"stem7", "stem8", "stem9", "stem0", 0
};

dovars(j, slot)
	register Job *j;
{
	char buf[BIGBLOCK];
	char *s, *t;
	int i, n = 0;

#define	SPECIAL	((sizeof myenv)/(sizeof myenv[0])-1)
#define	VSET(name, exp)	{strcpy(buf, "name="); strcpy(strchr(buf, 0), exp);}

	for(i = 0; i < SPECIAL; i++)
		if(special[i])
			free(special[i]);
	VSET(target, s = wtos(j->t));
	special[n++] = strdup(buf);
	free(s);
	/* WATCH OUT; stem set below if reg exp!! */
	VSET(stem, j->stem);
	special[n++] = strdup(buf);
	VSET(prereq, s = wtos(j->p));
	special[n++] = strdup(buf);
	free(s);
	sprint(buf, "pid=%d", getpid());
	special[n++] = strdup(buf);
	sprint(buf, "nproc=%d", slot);
	special[n++] = strdup(buf);
	VSET(newprereq, s = wtos(j->np));
	special[n++] = strdup(buf);
	free(s);
	VSET(alltarget, s = wtos(j->at));
	special[n++] = strdup(buf);
	free(s);
	for(i = 0; i <= 9; i++){
		sprint(buf, "stem%d=", i);
		if(j->r->attr&REGEXP){
			for(s = buf; *s; s++);
			for(t = j->match[i].sp; t < j->match[i].ep; *s++ = *t++);
			*s = 0;
		}
		special[n+i] = strdup(buf);
		if((i == 1) && (j->r->attr&REGEXP)){
			buf[1] = 's'; buf[2] = 't'; buf[3] = 'e'; buf[4] = 'm';
			special[1] = strdup(buf+1);
		}
	}
	special[SPECIAL] = 0;
}

nproc()
{
	register Symtab *sym;

	if(sym = symlook("NPROC", S_VAR, (char *)0))
		nproclimit = atoi(sym->value);
	if(nproclimit < 1)
		nproclimit = 1;
	if(DEBUG(D_EXEC))
		fprint(1, "nprocs = %d\n", nproclimit);
	if(nproclimit > nevents){
		if(nevents)
			events = (Event *)realloc((char *)events, nproclimit*sizeof(Event));
		else
			events = (Event *)malloc(nproclimit*sizeof(Event));
		while(nevents < nproclimit)
			events[nevents++].pid = 0;
	}
}

nextslot()
{
	register i;

	for(i = 0; i < nproclimit; i++)
		if(events[i].pid <= 0) return(i);
	assert("out of slots!!", 0);
	return(0);	/* cyntax */
}

static
pidslot(pid)
{
	register i;

	for(i = 0; i < nevents; i++)
		if(events[i].pid == pid) return(i);
	return(-1);
}

static void
pnew(pid, status)
{
	register Process *p;

	if(pfree){
		p = pfree;
		pfree = p->f;
	} else
		p = (Process *)Malloc(sizeof(Process));
	p->pid = pid;
	p->status = status;
	p->f = phead;
	phead = p;
	if(p->f)
		p->f->b = p;
	p->b = 0;
}

static void
pdelete(p)
	Process *p;
{
	if(p->f)
		p->f->b = p->b;
	if(p->b)
		p->b->f = p->f;
	else
		phead = p->f;
	p->f = pfree;
	pfree = p;
}

static long tslot[1000];
static long tick;

account()
{
	long t;

	time(&t);
	if(tick)
		tslot[nrunning] += (t-tick);
	tick = t;
}

praccount()
{
	int i;

	account();
	for(i = 0; i <= nevents; i++)
		Fprint(1, "%d: %ld\n", i, tslot[i]);
}
