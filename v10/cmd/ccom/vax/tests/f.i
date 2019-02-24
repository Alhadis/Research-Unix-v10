union U {
	long	u;
};

union U *f();

struct S {
	long	x;
	long	s;
};

union U Z;

main(){
	struct S *a;
	register struct S *r;
	
	a->s = f(r, a->s, (union U*) &Z)->u;
}

