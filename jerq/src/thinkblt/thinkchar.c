#include <jerq.h>
#include <font.h>
#include <muxpcbs.h>
#define THINKCHAR
#include "think.h"

#define TBSIZE	80
#define NTB	24

static Tbuf *head, *tail, *avail, *current;

int _piointr(), (*thquitf)(), thinkvec = 1;

static int int_high, th_intr, (*oldipc)(), (*oldpc)(), dont_quit();

thinkstart(quitfun)
int (*quitfun)();
{
	register Tbuf *tb;
	register i;
	++th_intr;
	int_high = thinkvec;
	thquitf = quitfun ? quitfun : dont_quit;
	for (i=0; i<NTB; i++) {
		if ((tb = (Tbuf *)alloc(sizeof(Tbuf))) == 0)
			return -1;
		if (gcalloc(TBSIZE, &tb->data) == 0)
			return -1;
		tb->size = TBSIZE;
		tb->next = avail;
		avail = tb;
	}
	pswmax();
	if (int_high) {
		oldipc = pioh_pcb.initpc;
		oldpc = pioh_pcb.pc;
		pioh_pcb.initpc = _piointr;
		pioh_pcb.pc = _piointr;
	} else {
		oldipc = piol_pcb.initpc;
		oldpc = piol_pcb.pc;
		piol_pcb.initpc = _piointr;
		piol_pcb.pc = _piointr;
	}
	pswback();
	return 0;
}

thinkchar(c)
{
	register Tbuf *tb;
	if ((tb = current) == 0 && (tb = gettbuf()) == 0)
		return 1;
	tb->data[tb->n++] = c;
	if (tb->n >= tb->size)
		if (puttbuf())
			return 1;
	return 0;
}

thinknchars(n, p)
register n; register unsigned char *p;
{
	register Tbuf *tb;
	tb = current;
	while (--n >= 0) {
		if (tb == 0 && (tb = gettbuf()) == 0)
			return 1;
		tb->data[tb->n++] = *p++;
		if (tb->n >= tb->size) {
			if (puttbuf())
				return 1;
			tb = 0;
		}
	}
	return 0;
}

thinkflush()
{
	if (current && current->n)
		if (puttbuf())
			return 1;
	while (head || *TH_PORT & TH_BUSY) {
		if ((*thquitf)())
			return thinkabort();
		wait(CPU);
	}
	return 0;
}

thinkstop()
{
	thinkflush();
	pswmax();
	if (int_high) {
		pioh_pcb.initpc = oldipc;
		pioh_pcb.pc = oldpc;
	} else {
		piol_pcb.initpc = oldipc;
		piol_pcb.pc = oldpc;
	}
	pswback();
}

thinkabort()
{
	pswmax();
	if (head) {
		tail->next = avail;
		avail = head;
		head = 0;
	}
	if (current) {
		current->next = avail;
		avail = current;
		current = 0;
	}
	pswback();
	return 1;
}

Tbuf *
gettbuf()
{
	register Tbuf *tb;
	while (avail == 0) {
		if ((*thquitf)()) {
			thinkabort();
			return 0;
		}
		wait(CPU);
	}
	pswmax();
	current = tb = avail;
	avail = tb->next;
	tb->next = 0;
	tb->i = tb->n = 0;
	pswback();
	return tb;
}

puttbuf()
{
	pswmax();
	if (head) {
		tail->next = current;
		tail = current;
		current = 0;
	} else {
		head = current;
		tail = current;
		current = 0;
		if (*TH_PORT & TH_BUSY) {
			pswback();
			do {
				if ((*thquitf)())
					return thinkabort();
				wait(CPU);
			} while (*TH_PORT & TH_BUSY);
			pswmax();
		}
		piointr();
	}
	pswback();
	return 0;
}

piointr()
{
	register Tbuf *tb;
	register w;
	if ((tb = head) == 0) {
		w = *TH_PORT;
	} else {
		*TH_PORT = tb->data[tb->i++];
		if (--tb->n <= 0) {
			head = tb->next;
			tb->next = avail;
			avail = tb;
		}
	}
}

intrentry(_piointr, piointr)

static
dont_quit()
{
	return 0;
}
