#include "term.h"
#include "../protocol.h"

#define	USER		8	/* a downloaded process */
#define	NPROC		16

struct udata {
	Rectangle	Drect;
	Layer	 	*Jdisplayp;
	int		argc;
	char		**argv;
	struct Mouse	mouse;
};

Proc *ProcTab;

FindProcTab()
{
	debug();
	ProcTab = getproctab();
	journal( "ProcTab", "%x NPROC=%d", ProcTab, NPROC );
}
#define PROCSIZE (sizeof(Proc))
IsProcPtr(p)
register long p;
{
	p -= (long) ProcTab;
	return p >= 0 && p < PROCSIZE*NPROC && p%PROCSIZE == 0;
}

char *Argv0(p)
register Proc *p;
{
	register char *argv0, **uargv;
	register struct udata *u;

	argv0 = "?";
	if( p->state&MUX_BUSY ){
		u = (struct udata *)p->data;
		if( !ReadErr(u, 4) ){
			uargv = u->argv;
			if( !ReadErr(uargv, 4) )
				argv0 = uargv[0];
		}
	}
	return argv0;
}

Jps()
{
	register Proc *p;

	SendUChar(NPROC);
	for( p = ProcTab; p < ProcTab+NPROC; ++p ){
		journal( "Jps", "p=%x %s", p, Argv0(p) );
		SendLong(p);
		SendString( Argv0(p) );
	}
}

PiTexture(p)
register Proc *p;
{
	register i = 0;
	if( !p || !p->layer ) return;
	for(;;){
		texture( p->layer, p->layer->rect, &PiIcon, F_XOR );
		if( i ) break;
		i = 1;
		nap(20);
	}
}

Debug()
{
	register Proc *p= getproc();
	register char *argv0 = "?";

	cursswitch(Cursor);
	if( p  && p->layer ){
		PiTexture(p);
		argv0 = Argv0(p);
	}
	journal( "Debug", "p=%x %s", p, argv0 );
	SendLong(p);
	SendString(argv0);
}
