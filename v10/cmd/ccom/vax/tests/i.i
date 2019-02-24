struct S {
	int	(*f)();
struct	S	*table[10][20];
struct	S	*ptr;
};

main(){
	struct S *a;
	int i, j;
	register struct S *r;
	
	a->ptr = r->table[ i = (*a->f)(a)&07 ] [ j = 1 ];	/* no call */
}

