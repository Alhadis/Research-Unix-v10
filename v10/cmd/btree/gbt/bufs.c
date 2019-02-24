#include "cbt.h"

typedef struct bhd {
	struct bhd *next;
	short fd;
	long which;
	char *where;
} bhd;

enum {Nhash = 601, Max = 1000};
bhd *bhds[Nhash], *bhfree;
bhd bhpool[Max];
int btptr;
#define hash(a, b) ((b << 8) | a) % Nhash

btbufinit()
{	int i;
	char *p;
	p = (char *) malloc(Max * NDSZ);
	if(!p)
		abort();
	for(i = 1; i < Max; i++) {
		bhpool[i-1].next = bhpool + i;
		bhpool[i].where = p + NDSZ * i;
	}
	bhpool[0].where = p;
	bhfree = bhpool + 0;
}

btbufdone(fd)
{	int i;
	bhd *p, *q;
	for(i = 0; i < Nhash; i++) {
		p = bhds[i];
loop:
		if(!p)
			continue;
		if(p->fd != fd) {
			p = p->next;
			goto loop;
		}
		lseek(p->fd, p->which*NDSZ, 0);
		write(p->fd, p->where, NDSZ);	/* check these dammit */
		q = p;
		p = p->next;
		q->next = bhfree;
		goto loop;
	}
}

btbufwrt(fd, which, from)
char *from;
{	int n;
	bhd *p;
	n = hash(fd, which);
	for(p = bhds[n]; p; p = p->next)
		if(p->fd == fd && p->which == which) {
			btbcpy(p->where, from, NDSZ);
			return;
		}
	if(!bhfree)
		btbuffree();
	p = bhfree;
	bhfree = p->next;
	p->next = bhds[n];
	bhds[n] = p;
	p->fd = fd;
	p->which = which;
	btbcpy(p->where, from, NDSZ);
}

btbuffree()
{	bhd *p;
loop:
	if(btptr >= Max)
		btptr = 0;
	p = bhpool + btptr++;
	if(!p->next)
		goto loop;
	bhfree = p->next;
	p->next = bhfree->next;
	bhfree->next = 0;
	lseek(bhfree->fd, bhfree->which*NDSZ, 0);
	write(bhfree->fd, bhfree->where, NDSZ);	/* check this dammit */
}

btbufread(fd, which, to)
char *to;
{	int n;
	bhd *p;
	static init;
	if(!init++)
		btbufinit();
	n = hash(fd, which);
	for(p = bhds[n]; p; p = p->next)
		if(p->fd == fd && p->which == which) {
			btbcpy(to, p->where, NDSZ);
			return(0);
		}
	
	if(!bhfree)
		btbuffree();
	p = bhfree;
	bhfree = p->next;
	p->next = bhds[n];
	bhds[n] = p;
	p->fd = fd;
	p->which = which;
	if(lseek(fd, which * NDSZ, 0) == -1)
		return(EOF);
	if(read(fd, p->where, NDSZ) != NDSZ)
		return(EOF);
	btbcpy(to, p->where, NDSZ);
	return(0);
}

btbcpy(to, from, sz)
register int *to, *from, sz;
{	register int cnt = sz/sizeof(int);
	while(cnt-- > 0)
		*to++ = *from++;
}
