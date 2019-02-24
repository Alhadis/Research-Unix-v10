#ifndef	defont
#include	<CC/jerq.h>
#endif

class Rulepot;
class Bmappot;
class Globepot;

class Pot
{
	short flags;
	short pad;
	Pot *next;
	Rectangle r;
	friend hitpot(int);
	friend void helppot(int);
	friend void drawpot();
	friend void reshapepot(Rectangle, Rectangle);
	friend Rulepot;
	friend Bmappot;
	friend Globepot;
public:
	Pot(Rectangle, short, short);
	short val;
	char *help;
	virtual void draw();
	virtual void resize(Rectangle);
	virtual void mod(int);
};
extern Pot *base;
extern void prval(Point, short);
extern int hitpot(int);
extern void drawpot(), reshapepot(Rectangle, Rectangle);
extern void helppot(int);
extern Rectangle rcenter(Rectangle, Rectangle);

#define		P_MASK		0xFF
#define		P_STYLE		0x3
#define			P_VERT		0x0
#define			P_HORIZ		0x1
#define			P_CIRC		0x2
#define		P_TICKS		0x4

class Rulepot : public Pot
{
	short maxval;
	short rx;
	Point valoffset, stroffset;
	char *str;
public:
	Rulepot(Rectangle, short, short, short=0);
	Rulepot(Rectangle, short, short, Point, char *, short=0);
	Rulepot(Rectangle, short, short, Point, short=0);
	Rulepot(Rectangle, short, short, Point, Point, char *, short=0);
	setval(short);
	void draw();
	void resize(Rectangle);
	void mod(int);
};

class Bmapitem;

class Bmappot : public Pot
{
	short maxval;
	short nitems;
	Bmapitem *items;
	Point corner, stroffset;
	char *str;
public:
	Bmappot(Rectangle, short, short=0);
	Bmappot(Rectangle, short, Point, char *, short=0);
	void item(Point, char *, Texture32 *);
	void item(Point, char *, Bitmap *);
	setval(short);
	void draw();
	void resize(Rectangle);
	void mod(int);
};

class Globepot : public Pot
{
	short radius, offset;
	Point center, pt;
	Point valoffset, stroffset;
	char *str;
	void ptoc();
	void ctop();
	void calc();
	void prvals();
public:
	Globepot(Rectangle, short, short, short);
	Globepot(Rectangle, short, short, short, Point, char *);
	Globepot(Rectangle, short, short, short, Point);
	Globepot(Rectangle, short, short, short, Point, Point, char *);
	void setval(short, short);
	short w;
	short front;
	void draw();
	void resize(Rectangle);
	void mod(int);
};

extern int min(int, int);
extern int max(int, int);
extern void sprintf(char *, char * ...);
extern int atan2(int, int);
extern void texture32(...);
