#include <jerq.h>
#include <font.h>
#include <msgs.h>

#define	RUN		1	/* ready to be scheduled */
#define	setrun(p)	((p)->state|=RUN)

#define BORDER	8
#define DXMIN	100
#define DYMIN	40
#define INSET	3

Rectangle r, canon();

int Argc; char **Argv;

main(argc, argv)
char **argv;
{
	Argc = argc; Argv = argv;
	while ((Argc -= 5) > 0) {
		r.origin.x = atoi(*++Argv);
		r.origin.y = atoi(*++Argv);
		r.corner.x = atoi(*++Argv);
		r.corner.y = atoi(*++Argv);
		++Argv;
		shellproc(canon(r));
	}
	sleep(20);
	sendnchars(1, "\n");
	exit();
}

Rectangle
canon(r)
	Rectangle r;
{
	register d, dm;

	d = r.corner.x - r.origin.x;
	if (d != (dm = max(DXMIN, min(XMAX-2*BORDER-1, d)))) {
		r.origin.x = (r.corner.x + r.origin.x - dm)/2;
		r.corner.x = r.origin.x + dm;
	}
	if ((d = BORDER - r.origin.x) > 0 || (d = XMAX-BORDER-1 - r.corner.x) < 0)
		r.origin.x += d, r.corner.x += d;

	d = r.corner.y - r.origin.y;
	if (d != (dm = max(DYMIN, min(YMAX-2*BORDER-1, d)))) {
		r.origin.y = (r.corner.y + r.origin.y - dm)/2;
		r.corner.y = r.origin.y + dm;
	}
	if ((d = BORDER - r.origin.y) > 0 || (d = YMAX-BORDER-1 - r.corner.y) < 0)
		r.origin.y += d, r.corner.y += d;

	return r;
}

shellproc(r)
	Rectangle r;
{
	int sendcmd();
	register struct Proc *p;

	if(p=newproc(sendcmd)){	/* Assignment = */
		p->rect=inset(r, INSET);
		if(p->layer=newlayer(r)){
			muxnewwind(p, C_NEW);
			tolayer(p->layer);
			setrun(p);
			while (p->text==(char *)sendcmd)
				sleep(20);
		}else
			p->state=0;
	}
}

sendcmd()
{
	register n;
	rectf((Bitmap *)P->layer, P->rect, F_STORE);
	sleep(20);
	if (n = strlen(*Argv)) {
		sendnchars(n, *Argv);
		sendchar('\n');
		sleep(20);
	}
	exit();
}
