#include <jerq.h>
#include <layer.h>
#include <jerqproc.h>
#include "../msgs.h"

long getlong();

extern Texture cup;

boot(){
	register argc;
	register char **argv;
	register char *address;
	char *bootaddress;
	char *Ualloc();
	register unsigned nbytes, nargchars;
	register long i, amtloaded;
	int text, data, bss;

	Urequest(MOUSE);
	Ucursswitch(&cup);
	argc=getlong();
	nargchars=getlong();
	argv=(char **)Ualloc(nargchars+(argc+1)*sizeof(char *));
	if(argv==0)
		address==0;
	else{
		text=getlong();
		data=getlong();
		bss=getlong();
		nbytes=text+data;	/* the amount to be downloaded */
		bootaddress=address=Ualloc((unsigned)(nbytes+bss));
	}
	P->data=address+text;
	P->bss=P->data+data;
	muxublk(P);
	muxmesg((int)(P-proctab), C_PUSHLD);
	sendwithdelim(4, &bootaddress);
	bldargs(argc, argv);
#define Y(a) (P->rect.corner.y-muldiv(P->rect.corner.y-P->rect.origin.y, a, nbytes))
	amtloaded=0;
	for(i=0; i<nbytes; i++){
		*address++=getchar();
		if(P->state&(RESHAPED|MOVED)){
			if(!(P->state&MOVED))
				amtloaded=0;
			P->state&=~(RESHAPED|MOVED);
		}
		if((i&127)==0){
			lrectf(P->layer, Rect(P->rect.origin.x, Y(i),
				P->rect.corner.x, Y(amtloaded)), F_XOR);
			amtloaded=i;
		}
	}
	muxublk(P);
	clear(P->rect, 1);
	Urequest(0);
	P->text=bootaddress;
	P->state|=USER;
	setdata(P);
#define	udp	((struct udata *)P->data)
	udp->argc=argc;
	udp->argv=argv;	/* these get set as arguments to main in notsolow.o */
	if(P->state&ZOMBOOT){
		P->state&=~ZOMBOOT;
		P->state|=ZOMBIE;
		zombexec(P->text);
	}
	exec(P->text);
}
bldargs(argc, argv)
	register char **argv;
{
	register i;
	register char *p=(char *)(argv+argc+1);
	for(i=0; i<argc; i++){
		*argv++=p;
		do
			*p=getchar();
		while(*p++);
	}
	*argv++=0;
}
setdata(p)
	register struct Proc *p;
{
	register struct udata *u=((struct udata *)p->data);
	u->Drect=p->rect;
	u->Jdisplayp=p->layer;
}
getchar(){
	register c;
	register struct Proc *p=P;

	while(p->nchars==0){
		muxublk(p);
		sw(0);
	}
	c = *(p->cbufpout)++;
	if(p->cbufpout >= &p->cbuf[sizeof(p->cbuf)])
		p->cbufpout = p->cbuf;
	if((--p->nchars)==0)
		muxublk(p);	/* shouldn't be necessary, but... */
	return c;
}

long
getlong(){
	long l;
	register char *p=(char *)&l;
	register i;
	for(i=0; i<4; i++)
		*p++=getchar();
	return(l);
}

shutdown(p)
	register struct Proc *p;
{
	extern struct Proc *debugger;
	extern struct Proc *kbdproc;
	Lbox(p->layer);
	setborder(p);
	if(debugger==p)
		debugger=0;
	muxublk(p);
	p->state&=~(KBDLOCAL|MOUSELOCAL|GOTMOUSE|USER);
	p->nticks=0;
	p->inhibited=0;
	qclear(&p->kbdqueue);
	if((p->state&ZOMBIE)==0)
		freemem(p);
}
freemem(p)
	register struct Proc *p;
{
	extern int end;
	extern int windowstart();
	freeall((char *)p);
	clearname(p);
}
