#ifndef BOX_H
#define BOX_H
#include "list.h"
class Box:public List {
public:
	Box(Point p)		{P = Q = p; sel = 12; mod = BOXHIT;}
	Box(Rectangle r)	{R = r; sel = 15;}
	void put(FILE *f)	{fprintf(f,"b %d %d %d %d\n",X,Y,U,V);}
	List *copy();
	int down(Point);
	int selleck(Rectangle);
	Hit typeat(Point, Hit);
	void draw(Bitmap *, int);
	List *unsel(int, List *);
};
class Macro:public Box {
public:
	Macro(Point p):(p)	{}
	Macro(Rectangle r):(r)	{}
	void put(FILE *f)	{fprintf(f,"z %d %d %d %d\n",X,Y,U,V);}
	int down(Point);
	Hit typeat(Point, Hit);
	void draw(Bitmap *, int);
};
/* go without
class Measure:public Box {
public:
	Measure(Point p):(p)	{}
	void draw(Bitmap *, int);
	List *unsel(int i, List *prev)	{i; draw(&display,F_XOR); return remove(prev);}
};
 */
#endif
