/* Copyright 1990, AT&T Bell Labs */
#include "fsort.h"

/* Radix sort by bytes.  Records are chained in a list.
   There are up to 257 bins at each recursion level.
   The "null bin" is for short strings that have run out;
   the rest are for byte values 0-255 */

int aflag = 0;
int uflag = 0;
int rflag = 0;
int sflag = 0;

/* Sort the list s by distributing according to
   digit d into an array of bins, sorting the bins
   recursively, and re-collecting them back into the list.
   If uflag is nonzero return only 1 representative of
   each equivalence class.  The squeeze step noted
   below determines what bins are in use, and squeezes
   those into a stack frame, which is contiguous to s.
   The null bin is handled separately to cut the span
   of the squeeze loop.
   Precondition: length(s) > 1 */

#define tiebreak(t)					\
	if(uflag) {					\
		t->tail = !aflag? t->head:		\
			  listaccum(t->head, t->tail);	\
		t->tail->next = 0;			\
	} else if(keyed && !sflag && t->head->next) {	\
		rflag = fields[0].rflag;		\
		keyed = 0;				\
		sort(t, 0);				\
		keyed = 1;				\
		rflag = 0;				\
	} else

void
sort(struct list *s, int d)
{
	extern int uflag;
	struct list *t;
	struct rec *r, *q;
	struct list *frame = s + 1;	/* stack frame */
	static struct list bins[257];
#	define nullbin (bins+256)
	int nbin;		/* number of filled bins */
	struct list *low; /* lowest unsorted nonnull bin */
loop:
	r = s->head;
	low = bins + 256;
	nbin = 0;
	while(r) {
		if(keyed)
			if(r->klen > (len_t)d)
				t = bins + key(r)[d];
			else
				t = nullbin;
		else
			if(r->dlen > (len_t)d)
				t = bins + data(r)[d];
			else
				t = nullbin;
		if(t->head == 0) {
			if(low > t)
				low = t;
			t->head = r;
			nbin++;
		} else
			t->tail->next = r;	/* stable sort */
		t->tail = r;
		r = r->next;
	}
	t = frame;			/* t is stack top */
	if(t+nbin > stackmax)
		fatal("stack overflow", "", 0);
#	define push(b)		/* onto stack */	\
		*t = *b,				\
		t->tail->next = 0,			\
		t++,					\
		b->head = 0,				\
		nbin--
	if(nullbin->head)
		push(nullbin);
	for( ; nbin>0; low++)
		if(low->head)
			push(low);
	if(t == frame+1) {		/* tail recursion */
/*		debug(frame, t, d);*/
		r = frame->head;
		if(r->len[keyed] <= d) {
			tiebreak(frame);
			*s = *frame;
			return;		/* string ran out */
		}
		d++;
		goto loop;
	}
/*	debug(frame, t, d);*/
	t--;
	if(t->head->next)		/* skip 1-item list */
		sort(t, d+1);
	if(!rflag) {			/* forward order */
		r = t->tail;
		while(t > frame) {	
			q = t->head;
			if((--t>frame || t->head->len[keyed]>d)
			   && t->head->next)
				sort(t, d+1);
			else if(t==frame)
				tiebreak(t);
			t->tail->next = q;	/* concatenate */
		}
		s->head = frame->head;
		s->tail = r;
	} else {			/* reverse order */
		r = t->head;
		while(t > frame) {	
			q = t->tail;
			if((--t>frame || t->head->len[keyed]>d)
			   && t->head->next)
				sort(t, d+1);
			else if(t==frame)
				tiebreak(t);
			q->next = t->head;	/* concatenate */
		}
		s->head = r;
		s->tail = frame->tail;
	}
/*	printf("return\n"); debug(s,s+1,d);*/
}

/*
debug(struct list *stack, struct list *top, int d)
{
	printf("----------------------level %d\n", d);
	while(stack<top) {
		printout(stack->head, stdout, "stdout");
		printf("----------------------\n");
		stack++;
	}
}
*/
