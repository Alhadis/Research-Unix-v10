#ifndef DOT_H
#define DOT_H
#include "list.h"
class Dots:public List {
public:
	Dots(Point p)	{P = Q = p; sel = 12; mod = DOTHIT; draw(&display,F_XOR);}
	Dots(Rectangle r)	{R = r; sel = 15;}
	void put(FILE *f)	{fprintf(f,"d %d %d %d %d\n",X,Y,U,V);}
	List *copy();
	int down(Point);
	int selleck(Rectangle);
	void draw(Bitmap *, int);
	List *unsel(int, List *);
};
#endif
