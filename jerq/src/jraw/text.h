#ifndef TEXT_H
#define TEXT_H
#include "list.h"
class Text:public List {
public:
	char *s,*t;		/* two strings */
	char f,g;		/* orientation */
	char which;		/* one we are looking at */
	Text(){}
	Text(Point, int, int);
	Text(Point, char *, int, char *, int);
	~Text()			{delete s; delete this;}
	void put(FILE *ouf)	{fprintf(ouf,"t %s@ %d %s@ %d %d %d\n",s,f&255,t,g&255,X,Y);}
	Rectangle mbb(Rectangle);
	List *copy();
	int down(Point);
	int selleck(Rectangle);
	int myborder(int,Rectangle);
	Hit typeat(Point, Hit);
	int type(int);
	void draw(Bitmap *, int);
	List *unsel(int, List *);
	void snap();
};
#endif
