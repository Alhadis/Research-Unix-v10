class vector : public common {
	common** vec;
	int size;
public:
		vector(int sz) { size = (sz<=0) ? 1 : sz; vec = new common*[size]; }
		~vector() {};
	virtual common* range_error() { return 0; }
	common& operator[](int i) { return (0<=i && i<size) ? vec[i] : range_error(); }
	int	max() { return size-1; }
	void	grow(int size);
};

class table : public vector {
	short*	hashtbl;
	table*	next;
	void	grow();
	common* lookup(char* s, common* p, int i);
public:
		table(int size);
		table(vector&);
	common*	insert(common& oo)		{ return lookup(oo.swho(),&oo,1); }
	common* look(char* s)			{ return lookup(s,0,1); } 
	common* look(common& oo, int i = 0)	{ return lookup(oo.swho(),&oo,i); }
};

