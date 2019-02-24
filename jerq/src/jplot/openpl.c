#include "jcom.h"
#include	"jplot.h"
#include	<sgtty.h>
#include	<signal.h>
#include	<stdio.h>
#include	"/usr/jerq/include/jioctl.h"

struct penvir E[9] = {
	800., 0., 0., 0., 1., -1., 800., -800.,1.,.5,0.,0.,1,0,
	800., 0., 0., 0., 1., -1., 800., -800.,1.,.5,0.,0.,1,0,
	800., 0., 0., 0., 1., -1., 800., -800.,1.,.5,0.,0.,1,0,
	800., 0., 0., 0., 1., -1., 800., -800.,1.,.5,0.,0.,1,0,
	800., 0., 0., 0., 1., -1., 800., -800.,1.,.5,0.,0.,1,0,
	800., 0., 0., 0., 1., -1., 800., -800.,1.,.5,0.,0.,1,0,
	800., 0., 0., 0., 1., -1., 800., -800.,1.,.5,0.,0.,1,0,
	800., 0., 0., 0., 1., -1., 800., -800.,1.,.5,0.,0.,1,0,
	800., 0., 0., 0., 1., -1., 800., -800.,1.,.5,0.,0.,1,0
};
struct penvir *e1 = &E[1];
struct penvir *e0 = E;
struct penvir *esave;
int t0[17] = {
	0177777, 0177777, 0177777, 0177777,
	0177777, 0177777, 0177777, 0177777,
	0177777, 0177777, 0177777, 0177777,
	0177777, 0177777, 0177777, 0177777, 1 };
int t1[17];	/* one for mode */

int
	firstime = 0,			/*so we don't open twice*/
	mpx = 0,			
	wantready = 0,			/* 0 if blast ahead, 1 if want READY */
	jerq = -1,			/* file descriptor from jerq */
	opened = 0,			/* open for graphics 0 = no, 1 = yes */
	lasty = -1;			/* current position y */
int openflg = 0;
struct sgttyb savetty;				/* save tty modes */
char plotty[] = "/dev/tty";

void
openpl()
{
	struct sgttyb modes;			/*  tty modes */
	int loaded = 0;
	char 	*ttyname(),		/* return the tty name if found */
		cmd[100],		/* 32ld cmd to be */
		*tty;			/* ttyname return */
	if(firstime)return;
	openflg++;
	e0->texture = t0;
	e1->texture = t0;
	jerq = open(plotty, 2);
	if(jerq == -1){
		fprintf(stderr, "jplot couldn't open /dev/tty\n");
		exit(1);
	}

	if (ioctl(jerq,JMUX,0) != -1) 
		mpx = 1;
	ioctl(jerq, TIOCGETP, &modes);
	savetty = modes;
	modes.sg_flags |= RAW;
	modes.sg_flags &= ~ECHO;
	ioctl(jerq, TIOCSETP, &modes);
	if(!isatty(1) || !isjplot()){
		if(ioctl(1,JMUX,0) != -1)
			mpx = 1;
		sprintf(cmd,"/usr/jerq/bin/32ld %s.%c < %s > %s; stty old!",
			JPLOT, "jm"[mpx], plotty, plotty);
		if(system(cmd)!= 0){
			fprintf(stderr,"jplot couldn't 32ld\n");
			ioctl(jerq, TIOCSETP, &savetty);
			exit(1);
		}
	}
	else loaded = 1;
	firstime = 1;
	sleep(2);	/* kludge for data kit */

	start(loaded);
}
