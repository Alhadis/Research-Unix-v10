#include <jerq.h>
#include <layer.h>
#include <jerqproc.h>
#include <font.h>

char		*patchedspot;
char		patch;
char *		itox();
extern long	traploc;
extern short 	traptype;
extern Layer 	*whichlayer();
extern Texture	eightball;
struct Proc	*debugger;

#define	RESET	0
#define PROCESS 1
#define STACK	2
#define NORMAL	3

char *trapmsg[]={
	"",
	"Process",
	"Stack",
	0,	/* Normal, handled by next table */
};
char *normalmsg[]={
	/* 0*/	"Zero divide",
#define TRACE	1
	/* 1*/	"Trace trap",
	/* 2*/	"Illegal opcode",
	/* 3*/	"Reserved opcode",
	/* 4*/	"Invalid address mode",
	/* 5*/	"Memory fault",
	/* 6*/	"Gate vector fault",
	/* 7*/	"Illegal level change",
	/* 8*/	"Reserved data type",
	/* 9*/	"Integer overflow",
	/*10*/	"Privileged opcode",
	/*11*/	0,
	/*12*/	0,
	/*13*/	0,
#define BRKPT	14
	/*14*/	"Breakpoint trap",
	/*15*/	"Privileged register",
};
/* Called at spl7() */
trap(type,bad_pc,bad_psw,psw_bad,bad_pcbp)
	register int type;
	register char **bad_pc;
	register *bad_psw;
	register int psw_bad;
	int bad_pcbp;
{
	register int isc=((psw_bad >> 3) & 0xf);

	if(patchedspot){
		*patchedspot=patch;
		patchedspot=0;
	}
	spl0();
	P->state|=ZOMBIE;
	P->traploc=bad_pc;		 /* for debugger */
	P->traptype=bad_psw;
	if(debugger && (type==NORMAL) && ((isc==TRACE)||(isc==BRKPT))){	
		sw(0);			/* BPT or trace */
		/* ZOMBIE bit is now off; we are free to continue */
		return;
	}
	P->nchars=0;
	P->cbufpin=P->cbufpout=P->cbuf;
	if(type==NORMAL)
		copy(normalmsg[isc]? normalmsg[isc] : "Undefined normal exception");
	else{
		copy(trapmsg[type]);
		if(type==PROCESS){
			copy(" psw: 0x");
			copy(itox(psw_bad));
		}else{
			copy(" exception #0x");
			copy(itox(isc));
		}
	}
	copy(" at 0x");
	copy(itox((unsigned long)(*bad_pc-P->text)));
	*P->cbufpin='\0';
	if(P-proctab<=1)	/* DEMUX, CONTROL */
		error(P-proctab==0? "demux" : "control", P->cbuf);
	/* let the user Delete to kill the Unix process */
	shutdown(P);
	P->state&=~ZOMBIE;
	Urequest(MOUSE);
	Ucursswitch(&eightball);
	P->state|=ZOMBIE;
	mesg(P->cbuf);
	for(;;)
		sw(0);	/* ZOMBIE ==> will never return, but in case of pi */
}

copy(s)
	register char *s;
{
	register unsigned char *p=P->cbufpin;

	while(*s){
		*p++= *s++;
		P->nchars++;
		if(p>=&P->cbuf[sizeof(P->cbuf)])
			p=P->cbuf;
	}
	P->cbufpin=p;
}

char *
itox(n)
	register unsigned long n;
{
	static char hex[10];
	register char *hp;
	hp= &hex[sizeof hex];
	*--hp='\0';
	if(n>0)
		do{
			if((*--hp=(n&0xf)+'0') > '9')
				*hp+='A'-'9'-1;
			n>>=4;
		}while(n);
	else
		*--hp='0';
	return hp;
}
error(s1, s2)
	char *s1, *s2;
{
	char buf[64];
	rectf(&display, Rect(0, 0, 600, 50), F_OR);
	strcpy(buf, s1);
	strcat(buf, ": ");
	strcat(buf, s2);
	string(&defont, buf, &display, Pt(10, 20), F_XOR);
	P->state|=ZOMBIE;
	sw(0);
}
/*
**info(s)
**	char *s;
**{
**	register i;
**	jrectf(Rect(XMAX-300, YMAX-50, XMAX, YMAX), F_CLR);
**	string(&defont, s, &display, Pt(XMAX-290, YMAX-30), F_XOR);
**}
**buzz(n)
**{
**	register i;
**	while(n--)
**	for(i=0; i<200000; i++)
**		;
**}
**sstep(s)
**	char *	s;
**{
**	jrectf(Rect(XMAX-300, YMAX-50, XMAX, YMAX), F_CLR);
**	string(&defont, s, &display, Pt(XMAX-290, YMAX-30), F_XOR);
**	do; while(button12()==0);
**	if(button1())
**		(*(long *)0)=0;
**	do; while(button12());
**	*DADDR = 156*(1024/4);
**	jrectf(Rect(XMAX-300, YMAX-50, XMAX, YMAX), F_XOR);
**}
**help(s, n)
**	char *s;
**	unsigned long n;
**{
**	static char buf[64];
**	strcpy(buf, s);
**	strcat(buf, itox(n));
**	sstep(buf);
**}
*/
