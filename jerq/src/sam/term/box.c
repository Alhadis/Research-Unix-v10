#include "frame.h"

#define	SLOP	25

addbox(f, bn, n)	/* add n boxes after bn, shift the rest up,
			 * box[bn+n]==box[bn] */
	register Frame *f;
{
	register i;
	if(bn>f->nbox)
		panic("addbox");
	if(f->nbox+n>f->nalloc)
		growbox(f, n+SLOP);
	for(i=f->nbox; --i>=bn; )
		f->box[i+n]=f->box[i];
	f->nbox+=n;
}
closebox(f, n0, n1)	/* inclusive */
	register Frame *f;
{
	register i;
	if(n0>=f->nbox || n1>=f->nbox || n1<n0)
		panic("closebox");
	n1++;
	for(i=n1; i<f->nbox; i++)
		f->box[i-(n1-n0)]=f->box[i];
	f->nbox-=n1-n0;
}
delbox(f, n0, n1)	/* inclusive */
	register Frame *f;
{
	if(n0>=f->nbox || n1>=f->nbox || n1<n0)
		panic("delbox");
	freebox(f, n0, n1);
	closebox(f, n0, n1);
}
freebox(f, n0, n1)	/* inclusive */
	register Frame *f;
{
	register i;
	if(n1<n0)
		return;
	if(n0>=f->nbox || n1>=f->nbox)
		panic("freebox");
	n1++;
	for(i=n0; i<n1; i++)
		if(f->box[i].len>=0)
			freestr(f->box[i].ptr);
}
growbox(f, delta)
	register Frame *f;
	register delta;
{
	gcrealloc((char **)&f->box,
		f->nbox*sizeof(Box), (f->nalloc+delta)*sizeof(Box));
	f->nalloc+=delta;
}
gcrealloc(sp, old, new)
	register char **sp;
{
	register i;
	register long *p, *q;
	long *hold;
	if(*sp==0){
		if(gcalloc((unsigned long)new, sp)==0)
			goto Burma;
		return;
	}
	if(new<=old)
		return;
	if(*sp==0 || gcalloc((unsigned long)old, &hold)==0)
    Burma:
		panic("gcrealloc");
	old=(old+sizeof(long)-1)/sizeof(long);
	new=(new+sizeof(long)-1)/sizeof(long);
	for(p=(long *)*sp,q=hold,i=0; i<old; i++)
		*q++=*p++;
	gcfree(*sp);
	if(gcalloc((unsigned long)new*sizeof(long), sp)==0)
		goto Burma;
	for(p=(long *)*sp,q=hold,i=0; i<old; i++)
		*p++=*q++;
	gcfree(hold);
}
dupbox(f, bn)
	register Frame *f;
{
	if(f->box[bn].len<0)
		panic("dupbox");
	addbox(f, bn, 1);
	if(f->box[bn].len>=0){
		f->box[bn+1].ptr=allocstr(f->box[bn].len+1);
		copystr(f->box[bn].ptr, f->box[bn+1].ptr);
	}
}
truncatebox(f, b, n)	/* drop last n chars; no allocation done */
	register Frame *f;
	register Box *b;
{
	if(b->len<0 || b->len<n)
		panic("truncatebox");
	b->ptr[b->len-=n]=0;
	b->wid=strwidth(f->font, b->ptr);
}
chopbox(f, b, n)		/* drop first n chars; no allocation done */
	register Frame *f;
	register Box *b;
{
	if(b->len<0 || b->len<n)
		panic("chopbox");
	copystr(b->ptr+n, b->ptr);
	b->len-=n;
	b->wid=strwidth(f->font, b->ptr);
}
splitbox(f, bn, n)
	register Frame *f;
	register bn, n;
{
	dupbox(f, bn);
	truncatebox(f, &f->box[bn], f->box[bn].len-n);
	chopbox(f, &f->box[bn+1], n);
}
mergebox(f, bn)		/* merge bn and bn+1 */
	register Frame *f;
{
	register Box *b=&f->box[bn];
	insure(b, b[0].len+b[1].len+1);
	copystr(b[1].ptr, b[0].ptr+b[0].len);
	b[0].wid+=b[1].wid;
	b[0].len+=b[1].len;
	delbox(f, bn+1, bn+1);
}
findbox(f, bn, p, q)	/* find box containing q and put q on a box boundary */
	register bn;
	register Frame *f;
	register Posn p, q;
{
	register Box *b;
	for(b=&f->box[bn]; bn<f->nbox && p+LEN(b)<=q; bn++, b++)
		p+=LEN(b);
	if(p!=q)
		splitbox(f, bn++, q-p);
	return bn;
}
