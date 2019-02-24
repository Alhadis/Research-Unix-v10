typedef void* PV;
typedef void (*PF)(PV);

extern PV _vec_new(PV op, int n, int sz, PV f)
/*
	allocate a vector of "n" elements of size "sz"
	and initialize each by a call of "f"
*/
{
	register int i;
	register char* p;
	if (op == 0) op = PV( new char[n*sz] );
	p = (char*) op;
	for (i=0; i<n; i++) ( *PF(f) )( PV(p+i*sz) );
	return PV(p);
}

void _vec_delete(PV op, int n, int sz, PV f)
{
	register int i;
	register char* p = (char*) op;
	for (i=0; i<n; i++) ( *(PF)f )( (PV)(p+i*sz) );
}
