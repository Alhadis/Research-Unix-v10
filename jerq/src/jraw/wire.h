#ifndef WIRE_H
#define WIRE_H
#include "list.h"
class Wire:public List {
public:
	Wire(Point p)	{P = Q = p; sel = 12; mod = WIREHIT;}
	Wire(Point p, Point q)	{P = p; Q = q; sel = 15;}
	void put(FILE *f)	{fprintf(f,"w %d %d %d %d\n",X,Y,U,V);}
	Rectangle mbb(Rectangle);
	List *copy();
	int down(Point);
	int selleck(Rectangle);
	int myborder(int, Rectangle);
	Hit typeat(Point, Hit);
	void draw(Bitmap *, int);
	List *unsel(int, List *);
	void snap();		/* use this to merge collinear wires */
	List *collinear(Point *, Wire *, List *);
};
class Ortho:public Wire {
public:
	Ortho(Point p):(p)	{}
	Ortho(Point p, Point q):(p,q)	{}
	void put(FILE *f)	{fprintf(f,"o %d %d %d %d\n",X,Y,U,V);}
	void draw(Bitmap *, int);
};
#endif
