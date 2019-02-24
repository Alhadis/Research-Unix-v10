#include <jerq.h>
typedef void* PV;
typedef void (*PF)(PV);
typedef void (*PFVV)();

PFVV _new_handler = 0;

PFVV set_new_handler(PFVV handler)
{
	PFVV rr = _new_handler;
	_new_handler = handler;
	return rr;
}

PV _new(unsigned size)
{
	char* p;
/* 	extern char* alloc(unsigned);	 not for mux! */
	while ( (p=alloc(size))==0 ) {
		if(_new_handler)
			(*_new_handler)();
		else return 0;
	}
	return PV(p);
}

void _delete(PV p)
{
	if (p) free( (char*)p );
}

PV _vec_new(PV op, int n, int sz, PV f)
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
