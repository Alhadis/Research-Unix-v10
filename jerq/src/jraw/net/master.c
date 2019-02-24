#include "geom.h"
#include "thing.h"
#include "text.h"
#include "wire.h"
#include "conn.h"
#include "box.h"
#include "master.h"

extern Vector *masters;

Master.Master(String *ss)
{
	s = ss;
	hasmbb = 0;
	boxes = new BoxList(32);
	macros = new BoxList(2);
	wires = new WireList(32);
	labels = new TextList(32);
}

Master.~Master()
{
	delete wires;
	delete boxes;
	delete macros;
	delete labels;
}

Master.eq(Thing *t)
{
	return s->eq((String *) t);
}

Rectangle Master.findmbb()
{
	if (hasmbb == 0) {
		mbb = wires->mbb(Rect(10000,10000,-10000,-10000));
		mbb = boxes->mbb(mbb);
		boxes->expand(labels);
		hasmbb = 1;
	}
	return mbb;
}

void Master.process()
{
	findmbb();
	labels->walk(macros,BoxList::pins);
	labels->walk(boxes,BoxList::parts);
	labels->walk(boxes,BoxList::pins);
	labels->walk(wires,WireList::nets);
	wires->prop();
	wires->prop1();
	macros->nets(wires);
	boxes->nets(wires);
	boxes->apply((int *) wires,Box::getname);
	if (macros->n != 0)
		macros->box(0)->pins->apply((int *) stdout,Conn::putm);
	boxes->put(stdout);
}

Master.get(FILE *inf)
{
	int c,junk;
	char buf0[80],buf1[80],buf2[80];
	Rectangle R;
	Master *m;
	for (c = getc(inf);; c = getc(inf)) {
		switch (c) {
		case EOF:
		case 'e':
			return 1;
		case 'm':
			fscanf(inf," %[^@]@",buf0);
			masters->append(m = new Master(new String(buf0)));
			m->get(inf);
			break;
		case 'i':
			fscanf(inf," %[^@]@ %d %d",buf0,&X,&Y);
			boxes->append(new Inst(new String(buf0),P));
			break;
		case 'w':
			fscanf(inf,"%d %d %d %d",&X,&Y,&U,&V);
			wires->append(new Wire(R));
			break;
		case 'b':
			fscanf(inf,"%d %d %d %d",&X,&Y,&U,&V);
			boxes->append(new Box(R));
			break;
		case 'z':
			fscanf(inf,"%d %d %d %d",&X,&Y,&U,&V);
			macros->append(new Box(R));
			break;
		case 't':
			fscanf(inf," %[^@]@ %d %[^@]@ %d %d %d",
				buf0,&junk,buf1,&junk,&X,&Y);
			labels->append(new Text(P,new String(buf0),new String(buf1)));
			break;
		}
		buf0[0] = buf1[0] = buf2[0] = 0;
	}
}

Inst.Inst(String *ss, Point p)
{
	s = ss;
	m = 0;
	part = 0;
	P = p;
}

Rectangle Inst.mbb(Rectangle r)
{
	if (m == 0)		/* haven't found my master yet */
		if ((m = (Master *) masters->lookup(s)) == 0) {
			fprintf(stderr,"undefined master: %s\n",s->s);
			return r;
		}
	R = (m->findmbb()).translate(P);
	return Box::mbb(r);
}

void Inst.put(FILE *ouf)
{
	static instnum=0;
	if (suffix || part) {
		Box::put(ouf);
		return;
	}
	fprintf(ouf,".c	$I%04d	%s\n",instnum++,s->s);
	pins->put(ouf);
}

void Inst.expand(Vector *v)
{
	register i;
	register Text *t;
	register TextList *l = m->labels;
	for (i = 0; i < l->n; i++) {
		t = (Text *) l->a[i];
		v->append(new Text(P+t->p,t->s,t->t));
	}
}
