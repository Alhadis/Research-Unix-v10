#ifndef INST_H
#define INST_H
#include "box.h"
#include "thing.h"
class Master:public Thing {
public:
	char s[20];
	List *dl;
	Bitmap *b;
	short mbbcnt;		/* number of times mbb is updated */
	Master(char *t)		{strcpy(s,t); dl = 0; mbbcnt = 0;}
	Master(List *l);
	~Master()		{removelist(dl); delete this;}
	void put(FILE *);
	void init();
	int edit(Point);
};

class Inst:public List {
public:
	Master *m;
	short mbbcnt;		/* use this to track master */
	Inst(Master *, Point);
	void put(FILE *f)	{fprintf(f,"i %s@ %d %d\n",m->s,X,Y);}
	Rectangle mbb(Rectangle);
	int enter()	{return sel ? m->edit(P) : 0;}
	List *copy();
	int down(Point);
	int selleck(Rectangle);
	Hit typeat(Point p, Hit h);
	void draw(Bitmap *, int);
};
#endif
