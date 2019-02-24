struct S {
	long	x;
	long	s;
};
struct S *ptr(), data();
main(){
	struct S *a;
	int i;
	register struct S *r;
	*(ptr(a)) = data( r, i+1, a);
}

