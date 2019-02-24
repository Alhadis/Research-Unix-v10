#ifndef BOX_H
#define BOX_H

class ConnList;

class Box:public Thing {
public:
	Rectangle R;
	Text *part;
	char *suffix;		/* index of suffix in part */
	ConnList *pins;
	Box();
	Box(Rectangle);
	~Box()			{delete pins;}
	Rectangle mbb(Rectangle r)	{return r.mbb(R);}
	int namepart(Text *t);
	int namepin(Text *t);
	void put(FILE *);
	void getname(WireList *);
};

class BoxList:public Vector {
public:
	BoxList(int nn):(nn)	{}
	Box *box(int i)	{return (Box *) a[i];}
	int parts(Text *t);
	int pins(Text *t);
	void nets(WireList *);
	void expand(Vector *);
};
#endif
