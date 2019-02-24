#ifndef TEXT_H
#define TEXT_H

class Text:public Thing {
public:
	Point p;
	String *s,*t;
	int used;
	Text(Point pp, String *ss, String *tt)	{p = pp; s = ss; t = tt; used = 0;}
	~Text()		{delete s; delete t;}
};

class TextList:public Vector {
public:
	TextList(int nn):(nn)	{}
	Text *text(int i)	{return (Text *) a[i];}
	void walk(Vector *, int (*)(...));	/* the workhorse */
};
#endif
