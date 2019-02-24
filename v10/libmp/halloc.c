/* list allocator */

typedef struct hdr {
	struct hdr *next;
	struct hdr **list;
	short d[1];
} hdr;
extern char *malloc();

hdr *h1, *h4, *h6;

hfree(d)
short *d;
{	hdr *p;
	if(d == (short *)1)
		return;
	p = (hdr *)((char *)d - 2 * sizeof(hdr *));
	if(p->list == 0) {
		free((char *)p);
		return;
	}
	p->next = *p->list;
	*p->list = p;
}

short *
halloc(n)
{	short *x;
	hdr *p;
	if(n <= 0)
		return((short *)1);	/* and unusable */
	if(n <= 2) {
		if(!h1)
			makelist(4, &h1);
		x = h1->d;
		h1 = h1->next;
		return(x);
	}
	if(n <= 8) {
		if(!h4)
			makelist(16, &h4);
		x = h4->d;
		h4 = h4->next;
		return(x);
	}
	if(n <= 64) {
		if(!h6)
			makelist(128, &h6);
		x = h6->d;
		h6 = h6->next;
		return(x);
	}
	p = (hdr *) malloc(sizeof(hdr) + (n - 1) * sizeof(short));
	p->list = 0;
	return(p->d);
}

makelist(n, p)
hdr **p;
{	int i, d;
	char *s;
	hdr *h;
	d = sizeof(hdr) + sizeof(short) * (n-1);
	d = (d + 3) & (~3);
	s = malloc(128 * d);
	h = *p = (hdr *)s;
	for(i = 0; i < 127; i++) {
		h->next = (hdr *)(s + d);
		h->list = p;
		s += d;
		h = (hdr *)s;
	}
	h->next = 0;
	h->list = p;
}
