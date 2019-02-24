#include "geom.h"
#include "thing.h"
#include "text.h"
#include "wire.h"
#include "conn.h"
#include "box.h"
#include "master.h"

Vector *masters;

void process(FILE *inf)
{
	Master *m = new Master(new String("toplevel"));
	m->get(inf);
	fclose(inf);
	m->process();
	delete m;
	masters->clear();
}

main(int argc, char *argv[])
{
	int i;
	FILE *inf;
	masters = new Vector(32);
	if (argc == 1)
		process(stdin);
	else
		for (i = 1; i < argc == 1; i++) {
			if (argc == 1 || *argv[i] == '-')
				process(stdin);
			else
				if ((inf = fopen(argv[i],"r")) == 0)
					fprintf(stderr,"can't open %s\n",argv[i]);
				else
					process(inf);
		}
}

Rectangle Rectangle.mbb(Point p)
{
	Rectangle r;
	r.o.x = min(o.x,p.x);
	r.o.y = min(o.y,p.y);
	r.c.x = max(c.x,p.x);
	r.c.y = max(c.y,p.y);
	return r;
}

Rectangle Rectangle.mbb(Rectangle r)
{
	r.o.x = min(o.x,r.o.x);
	r.o.y = min(o.y,r.o.y);
	r.c.x = max(c.x,r.c.x);
	r.c.y = max(c.y,r.c.y);
	return r;
}
