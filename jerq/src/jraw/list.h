#ifndef LIST_H
#define LIST_H

/* nonsense because unions don't do it */
#define P	R.o
#define Q	R.c
#define X	P.x
#define Y	P.y
#define U	Q.x
#define V	Q.y
#define ul	P
#define ur	Point(U,Y)
#define ll	Point(X,V)
#define lr	Q

class Master;
class Wire;

class List {
public:
	List *next;
	Rectangle R;
	char sel,mod;	/* selection state 1->p.x, 2->p.y, 4->q.x, 8->q.y */
	List()	{next = 0; X = -1; U = -2;}
	virtual void put(FILE *f)	{}
	virtual Rectangle mbb(Rectangle r)	{return r.mbb(R);}
	virtual int enter()	{return 0;}
	virtual List *copy()	{return new List;}
	virtual int down(Point);		/* mouse down at p */
	virtual int selleck(Rectangle);	/* hunk not reserved */
	virtual int myborder(int, Rectangle);	/* select if on myborder */
	virtual Hit typeat(Point, Hit);
	virtual type(int);
	virtual void move(Point);		/* move sellecked items by dp */
	virtual void draw(Bitmap *, int);			/* draw self */
	virtual List *unsel(int, List *);	/* clear selection */
	virtual void snap(){}			/* adjust self if necessary */
	virtual List *collinear(Point *p, Wire *w, List *l) {return this;}
	List *remove(List *prev) {prev->next = next; delete this; return prev;}
	List *cons(List *l)	{next = l; return this;}
	void insert(List *l)	{l->next = next; next = l;}
	List *append(List *);
	void sort();		/* fix R in things that care */
};
#endif
