#include "paint.h"
Image *ialloc(r)
Rectangle r;
{
	Image *i=(Image *)alloc(sizeof(Image));
	if(i==0)
		return(0);
	if((i->grn=balloc(r))==0){
		free(i);
		return(0);
	}
	if((i->alf=balloc(r))==0){
		bfree(i->grn);
		free(i);
		return(0);
	}
	i->rect=r;
	return(i);
}
ifree(i)
Image *i;
{
	bfree(i->grn);
	bfree(i->alf);
	free(i);
}
/*
 * composite two images according to the given compositing operator.
 * Not guaranteed if grn has any bits on outside alf.  Also not guaranteed
 * if grn and alf are not disjoint (can't happen if all Images are created
 * using ialloc).  Returns 1 on success, 0 on failure to allocate a needed
 * temporary.
 */
composite(a, ar, b, bp, op)
Image *a, *b;
Rectangle ar;
Point bp;
{
	static Bitmap *t=0;
	Rectangle br;
	Point disp;
	/*
	 * clip
	 */
	disp=sub(bp, ar.or);
	rectclip(&ar, a->rect);
	br=raddp(ar, disp);
	rectclip(&br, b->rect);
	ar=rsubp(br, disp);
	/*
	 * In comments A means a->grn, a means a->alf, B means b->grn,
	 * b means b->alf, t means t
	 */
	switch(op){		/* cases not needing a temporary */
	case C_CLEAR:
		rectf(b->grn, br, F_CLR);			/* B=0   */
		rectf(b->alf, br, F_CLR);			/* b=0   */
		return(1);
	case C_A:
		bitblt(a->grn, ar, b->grn, br.or, F_STORE);	/* B=A   */
		bitblt(a->alf, ar, b->alf, br.or, F_STORE);	/* b=a   */
		return(1);
	case C_B:
		return(1);
	case C_AoverB:
		bitblt(a->alf, ar, b->grn, br.or, F_CLR);	/* B&=~a */
		bitblt(a->grn, ar, b->grn, br.or, F_OR);	/* B|=A  */
		bitblt(a->alf, ar, b->alf, br.or, F_OR);	/* b|=a  */
		return(1);
	case C_BoutA:
		bitblt(a->alf, ar, b->alf, br.or, F_CLR);	/* B&=~a */
		bitblt(a->alf, ar, b->grn, br.or, F_CLR);	/* b&=~a */
		return(1);
	case C_AatopB:
		bitblt(a->alf, ar, b->grn, br.or, F_CLR);	/* B&=~a */
		bitblt(a->grn, ar, b->grn, br.or, F_OR);	/* B|=A  */
		rectf(b->alf, br, F_XOR);			/* b=~b  */
		bitblt(b->alf, br, b->grn, br.or, F_CLR);	/* B&=~b */
		rectf(b->alf, br, F_XOR);			/* b=~b  */
		return(1);
	}
	if(t==0
	|| t->rect.or.x!=ar.or.x || t->rect.co.x!=ar.co.x
	|| t->rect.or.y!=ar.or.y || t->rect.co.y!=ar.co.y){
		if(t!=0)
			bfree(t);
		t=balloc(ar);
		if(t==0)
			return(0);
	}
	switch(op){
	case C_BoverA:
		bitblt(a->grn, ar, t, ar.or, F_STORE);		/* t=A   */
		bitblt(b->alf, br, t, ar.or, F_CLR);		/* t&=~b */
		bitblt(t, ar, b->grn, br.or, F_OR);		/* B|=t  */
		bitblt(a->alf, ar, b->alf, br.or, F_OR);	/* b|=a  */
		break;
	case C_AoutB:
		bitblt(a->grn, ar, b->grn, br.or, F_STORE);	/* B=A   */
		bitblt(b->alf, br, b->grn, br.or, F_CLR);	/* B&=~b */
		bitblt(a->alf, ar, t, ar.or, F_STORE);		/* t=a   */
		bitblt(b->alf, br, t, ar.or, F_CLR);		/* t&=~b */
		bitblt(t, ar, b->alf, br.or, F_STORE);		/* b=t   */
		break;
	case C_AinB:
		bitblt(b->alf, br, t, ar.or, F_STORE);		/* t=b   */
		rectf(t, ar, F_XOR);				/* t=~t  */
		bitblt(a->grn, ar, b->grn, br.or, F_STORE);	/* B=A   */
		bitblt(t, ar, b->grn, br.or, F_CLR);		/* B&=~t */
		bitblt(a->alf, ar, b->alf, br.or, F_STORE);	/* b=a   */
		bitblt(t, ar, b->alf, br.or, F_CLR);		/* b&=~t */
		break;
	case C_BinA:
		bitblt(a->alf, ar, t, ar.or, F_STORE);		/* t=a   */
		rectf(t, ar, F_XOR);				/* t=~t  */
		bitblt(t, ar, b->grn, br.or, F_CLR);		/* B&=~t */
		bitblt(t, ar, b->alf, br.or, F_CLR);		/* b&=~t */
		break;
	case C_BatopA:
		bitblt(a->grn, ar, t, ar.or, F_STORE);		/* t=A   */
		bitblt(b->alf, br, t, ar.or, F_CLR);		/* t&=~b */
		bitblt(t, ar, b->grn, br.or, F_OR);		/* B|=t  */
		rectf(a->alf, ar, F_XOR);			/* a=~a  */
		bitblt(a->alf, ar, b->grn, br.or, F_CLR);	/* B&=~a */
		rectf(a->alf, ar, F_XOR);			/* a=~a  */
		bitblt(a->alf, ar, b->alf, br.or, F_STORE);	/* b=a   */
		break;
	case C_AxorB:
		bitblt(a->grn, ar, t, ar.or, F_STORE);		/* t=A   */
		bitblt(b->alf, br, t, ar.or, F_CLR);		/* t&=~b */
		bitblt(a->alf, ar, b->grn, br.or, F_CLR);	/* B&=~a */
		bitblt(t, ar, b->grn, br.or, F_OR);		/* B|=t  */
		bitblt(a->alf, ar, b->alf, br.or, F_XOR);	/* b^=a  */ 
		break;
	}
	return(1);
}
