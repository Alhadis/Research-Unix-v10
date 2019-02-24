#ifndef WIRE_H
#define WIRE_H

class Box;

class Wire:public Thing {
public:
	Rectangle R;
	Text *net;
	Wire(Rectangle);
	~Wire()		{delete net;}
	Rectangle mbb(Rectangle r)	{return r.mbb(R);}
	int prop(Wire *);
	int contains(Point);
	int namenet(Text *);
	void put(FILE *);
	void namebox(Box *);
};

class WireList:public Vector {
public:
	WireList(int nn):(nn)	{}
	Wire *wire(int i)	{return (Wire *) a[i];}
	int partition();
	void prop();
	void prop1();
	int nets(Text *);
};
#endif
