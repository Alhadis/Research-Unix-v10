#include <jerq.h>
#define	USER	8
struct proc{
	Rectangle rect;
	int saved;
	Layer *bp;
}proc[16];
struct udata{	/* from jerqproc.h */
	Rectangle Drect;
	Layer	*Jdisplayp;
	int	argc;
	char	**argv;
	struct Mouse mouse;
};
cleanup(){
	register struct Proc *tab, *tp;
	register struct proc *p;
	register NPROC;
	register i;
	tab=getproctab();
	NPROC=((int *)tab)[-1];
	for(i=2,tp=tab+2,p=proc+2; i<NPROC; i++,tp++,p++){
		if(tp->layer && tp->layer->obs){
			p->saved++;
			p->rect=tp->layer->rect;
			if(p->bp=(Layer *)balloc(p->rect))
				bitblt(tp->layer, p->rect, p->bp, p->rect.origin, F_STORE);
			dellayer(tp->layer);
		} else
			p->saved=0;
	}
	for(i=2,tp=tab+2,p=proc+2; i<NPROC; i++,tp++,p++){
		if(p->saved){
			tp->layer=newlayer(p->rect);
			if(tp->layer==0)
				continue;	/* should never happen */
			if(p->bp){
				bitblt(p->bp, p->rect, tp->layer, p->rect.origin, F_STORE);
				bfree(p->bp);
			}else
				tp->state|=RESHAPED;
			if(tp->state&USER)
				((struct udata *)tp->data)->Jdisplayp=tp->layer;
		}
	}
	for(i=2,tp=tab+2,p=proc+2; i<NPROC; i++,tp++,p++){
		if(tp->layer && !p->saved)
			upfront(tp->layer);
	}
}
