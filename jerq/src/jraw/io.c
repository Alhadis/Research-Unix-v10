#include "jraw.h"
#include "text.h"
#include "wire.h"
#include "box.h"
#include "inst.h"
#include "thing.h"

output(FILE *f)
{
	ml->put(f);
	displacelist(dl,-offset);
	putlist(dl,f);
	displacelist(dl,offset);
	return 1;
}

putlist(register List *l, FILE *f)
{
	for (; l != 0; l = l->next)
		l->put(f);
	return 1;
}

clearout()
{
	removelist(dl);
	ml->remove();
	return 1;
}

inint(FILE *inf)
{
	register c,i=0;
	int sign = 1;
	while ((c = getc(inf)) == ' ')		/* really special case */
		;
	if (c == '-') {
		sign = -1;
		c = getc(inf);			/* better be a digit */
	}
	for (; c >= '0' && c <= '9'; c = getc(inf))
		i = 10*i + (c-'0');
	return i*sign;
}

Point inpoint(FILE *inf)
{
	Point p;
	p.x = inint(inf);
	p.y = inint(inf);
	return p + offset;
}

Rectangle inrect(FILE *inf)
{
	Rectangle r;
	r.o = inpoint(inf);
	r.c = inpoint(inf);
	return r;
}

char *instring(char *s, FILE *inf)
{
	register c;
	while ((c = getc(inf)) == ' ')
		;
	for (; c != '@'; c = getc(inf))		/* HACK!! */
		*s++ = c;
	*s = 0;
	return s;
}

Master *findormake(char *s)
{
	register Master *m;
	if ((m = (Master *) (*ml)[s]) == 0) {
		m = new Master(s);
		(*ml) += m->s;
		(*ml)[s] = m;
	}
	return m;
}

List *inlist(register char c, FILE *inf)
{
	char buf[80],buf2[80];
	int i,j;
	Point p;
	List *list = 0;
	register List *l;
	for (l = (List *) &list;; c = getc(inf))
		switch (c) {
		case EOF:
		case 'e':
			return list;
		case 'w':
			p = inpoint(inf);
			l->next = new Wire(p,inpoint(inf));
			l = l->next;
			break;
		case 'o':
			p = inpoint(inf);
			l->next = new Ortho(p,inpoint(inf));
			l = l->next;
			break;
		case 'b':
			l->next = new Box(inrect(inf));
			l = l->next;
			break;
		case 'z':
			l->next = new Macro(inrect(inf));
			l = l->next;
			break;
		case 'd':
			/* nope, no Dots
			l->next = new Dots(inrect(inf));
			l = l->next;
			 */
			while ((c = getc(inf)) != '\n')
				;
			break;
		case 't':
			instring(buf,inf);
			i = inint(inf);
			instring(buf2,inf);
			j = inint(inf);
			l->next = new Text(inpoint(inf),buf,i,buf2,j);
			l = l->next;
			break;
		case 'i':
			instring(buf,inf);
			p = inpoint(inf);
			l->next = new Inst(findormake(buf),p);
			l = l->next;
			break;
		}
}

input(FILE *inf)
{
	register c;
	char buf[80];
	register Master *m;
	for (;;)
		switch (c = getc(inf)) {
		case EOF:
			dl = 0;
			return 0;
		case 'm':
			instring(buf,inf);
			m = findormake(buf);
			m->dl = inlist(0,inf);
			break;
		case 'b':
		case 'z':
		case 'd':
		case 'o':
		case 'w':
		case 'i':
		case 't':
			dl = inlist(c,inf);
			drawlist(dl,0,F_XOR);
			return 1;
		}
}
