#ifndef THING_H
#define THING_H
class Thing {
public:
	Thing() {}
	virtual void put(FILE *f)	{fprintf(f,"unknown Thing\n");}
	virtual void init();
};

class Dict:public Thing {
public:
	int n,N;
	char **a;	/* fix this later */
	Thing **x;
	Dict(int i)	{n=0; N=i; a=new char*[N]; x=new Thing*[N];}
	void init();
	void remove();
	void put(FILE *);
	Dict *operator+=(char *);
	Thing *&operator[](char *);
	char *&operator[](int i)	{return /* & */a[i];}
};
#endif
