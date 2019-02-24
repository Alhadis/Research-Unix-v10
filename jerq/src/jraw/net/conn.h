#ifndef CONN_H
#define CONN_H

class Conn:public Thing {
public:
	Text *pin;
	Text *net;
	Conn(Text *t) {pin = t; net = 0;}
	void nets(WireList *);
	void put(FILE *);
	void putm(FILE *);
	void fixup(char *);
};

class ConnList:public Vector {
public:
	ConnList(int nn):(nn)	{}
	Conn *conn(i)	{return (Conn *) a[i];}
	void nets(WireList *);
};
#endif
