#include	<libc.h>
#include	<fio.h>
#include	"crossbar.h"
#include	"switch.h"

extern int verbose;

Switch::Switch(char *nam, int n)
{
	int i;

	name = nam;
	nwires = n;
	maxlines = 0;
	for(i = 0; i < MAXLABEL; i++)
		lines[i][0] = lines[i][1] = 0;
}

void
Switch::add(int inout, char label, char *n1, char *n2, Crossbar *c, int l)
{
	int n = labeltoint(label);

	if(n > maxlines)
		maxlines = n;
	if(lines[n][inout])
		fprint(2, "warning: redefining label %s.%c\n", name, label);
	lines[n][inout] = new Line(label, n1, n2, c, l);
}

void
Switch::reinit()
{
}

void
Switch::describe()
{
	int imax, omax, n, i;
	char bi[256], bo[256];

	if(verbose)
		Fprint(1, "%s video switch:\n", name);
	imax = 6, omax = 7;	/* inputs/outputs */
	for(i = 0; i <= maxlines; i++){
		n = lines[i][0]->pr(i, bi, 0);
		if(n > imax) imax = n;
		n = lines[i][1]->pr(i, bi, 0);
		if(n > omax) omax = n;
	}
	blank(1, n = (imax-6)/2);
	Fprint(1, "inputs");
	blank(1, imax+3+(omax-7)/2-n-6);
	Fprint(1, "outputs\n");
	for(i = 0; i <= maxlines; i++){
		lines[i][0]->pr(i, bi, imax+3);
		lines[i][1]->pr(i, bo, 0);
		Fprint(1, "%s%s\n", bi, bo);
	}
}

void
Switch::pr(char opt)
{
	int i, j, l[50], k;
	Crossbar *c[50];
	Line *lp, *ll;
	char buf[256];
	int didit;

	if(!verbose)
		Fprint(1, "-%c", opt);
	for(i = 0; i <= maxlines; i++)
		if(ll = lines[i][0]){
			if(k = ll->c->map(ll->line, c, l)){
				didit = 0;
				for(j = 0; c[j]; j++)
					if((k = lineofcl(1, c[j], l[j])) >= 0){
						if(didit == 0){
							if(verbose)
								Fprint(1, "%s[i%c] ==>", ll->name[0], ll->label);
							didit = 1;
						}
						lp = lines[k][1];
						if(verbose)
							Fprint(1, " %s[o%c]", lp->name[0], lp->label);
						else
							Fprint(1, " %c%c", lines[i][1]->label, lp->label);
					}
				if(didit && verbose)
					Fprint(1, "\n");
			}
		}
	if(!verbose)
		Fprint(1, "\n");
}

void
Switch::connect(char input, char output)
{
	int i, o;
	Line *lp;

	i = labeltoint(input);
	o = labeltoint(output);
	lp = lines[i][0];
	lp->c->connect(lp->line, lines[o][1]->c, lines[o][1]->line, nwires);
}

int
Switch::lineofcl(int output, Crossbar *c, int l)
{
	int i;
	register Line *lp;

	for(i = 0; i <= maxlines; i++){
		lp = lines[i][output];
		if((lp->c == c) && (lp->line == l))
			return(i);
	}
	return(-1);
}

int
labeltoint(char label)
{
	if((label >= '0') && (label <= '9'))
		return(label-'0');
	if((label >= 'a') && (label <= 'z'))
		return((label-'a')+10);
	fprint(2, "bad arg to labeltoint '%c'\n", label);
	abort();
	return(0);
}

char
inttolabel(int i)
{
	if((i >= 0) && (i < MAXLABEL))
		return("0123456789abcdefghijklmnopqrstuvwxyz"[i]);
	fprint(2, "bad arg to inttolabel %d\n", i);
	abort();
	return(0);
}

void
blank(int fd, int n)
{
	while(n-- > 0)
		Fputc(fd, ' ');
}

#include	<sys/filio.h>
#include	<sys/ttyio.h>
#include	<sysent.h>
#include	<ipc.h>

int
opendk(char *name)
{
	int rfs;
	char buf[256];
	struct sgttyb vvec;
	extern int tty_ld, buf_ld;

	sprint(buf, "/cs/dk!%s", name);
	rfs = ipcopen(buf, "light");
	if(rfs < 0){
		perror(buf);
		exit(1);
	}
	ioctl(rfs, FIOPUSHLD, &buf_ld);
	if(ioctl(rfs, FIOLOOKLD, 0) != tty_ld){
		if(ioctl(rfs, FIOPUSHLD, &tty_ld) < 0){
			perror("tty_ld");
			exit(1);
		}
	}
	vvec.sg_flags = RAW;
	vvec.sg_erase = 0;
	vvec.sg_kill = 0;
	if(ioctl(rfs, TIOCSETP, &vvec) < 0){
		perror("ioctl");
		exit(1);
	}
	return(rfs);
}
