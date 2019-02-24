//#define A_LARGE 64
//#define A_USER 0x55000000
//
//void PadsError(char* ...);
//
//enum Event { POOL=0, ALLOC=1, FREE=2, EVENT=3 };
//
//#define	log(e, u, d) { event[u][e] += d; }
//
//class Allocator {
//	union M {
//		long size;
//		M    *link;
//	};
//	M	*freelist[A_LARGE];
//	long	req[A_LARGE];
//	long	event[A_LARGE][EVENT];
//public:
//	void	*alloc(long);
//	void	free(void*);
//	char	*profile(long);
//
//};
//
//Allocator NewDel;
//
//char *Allocator.profile(long u)
//{
//	static char report[64];
//	long p = event[u][POOL], a = event[u][ALLOC], f = event[u][FREE];
//
//	if( u >= A_LARGE ) return 0;
//	if( !(p|a|f) ) return "";
//	u = u ? (u-1)*sizeof(M) : 0;
//	sprintf(report, "size=%3d: pool=%4d new=%4d del=%4d tot=%8d",
//		u, p, a, f, (u+4)*p);
//	return report;
//}
//
//void *Allocator.alloc(register long u)
//{
//	M *malloc(long);
//	register M *m;
//	register r;
//
//	if( u <= 0 ) PadsError( "Allocator.alloc(%d)", u );
//	u = ((u-1)/sizeof(M)) + 2;
//	if( u >= A_LARGE ){
//		log(ALLOC,0,u*sizeof(M));
//		m = malloc(u*sizeof(M));
//		if( !m ) PadsError("host out of memory");
//	} else {
//		if( !freelist[u] ){
//			if( req[u] == 0 ) req[u] = 1;
//			r = req[u];
//			if( req[u] < 256 ) req[u] *= 2;
//			log(POOL,u,r);
//			freelist[u] = malloc(r*u*sizeof(M));
//			if( !freelist[u] ) PadsError("host out of memory");
//			for( m = freelist[u]; --r > 0; m = m->link = m+u );
//			m->link = 0;
//		}
//		log(ALLOC,u,1);
//		m = freelist[u];
//		freelist[u] = m->link;
//	}
//	m->size = u|A_USER;
//	for( r = 1; r < u; )
//		(&m->size)[r++] = 0;
//	return m+1;
//}
//
//void Allocator.free(void *v)
//{
//	void free(M*);
//	register M* m = (M*) v;
//	register long u;
//
//	--m;
//	if( (m->size&0xFF000000) != A_USER ) PadsError( "delete error" );
//	u = (m->size &= 0xFFFFFF);
//	if( u >= A_LARGE ){
//		log(FREE,0,u*sizeof(M));
//		::free(m);
//	}
//	else {
//		log(FREE,u,1);
//		m->link = freelist[u];
//		freelist[u] = m;
//	}
//}

typedef void* PV;
typedef void (*PF)(PV);

void *operator new(long size)
{
	void *calloc(unsigned, unsigned);
	if( size<= 0 ) PadsError("new(%d)", size);
	return calloc(size, 1);				// NewDel.alloc(size);
}

void operator delete(PV p)
{
	void free(void*);
	if(p) free(p);					// NewDel.free(p);
}

PV _vec_new(PV op, int n, int sz, PV f)
{
	register int i;
	register char* p;
	if (op == 0) op = PV( new char[n*sz] );
	p = (char*) op;
	for (i=0; i<n; i++) ( *PF(f) )( PV(p+i*sz) );
	return PV(p);
}

void _vec_delete(PV op, int n, int sz, PV f, int)	// what is that last int?
{
	register int i;
	register char* p = (char*) op;
	for (i=0; i<n; i++) ( *(PF)f )( (PV)(p+i*sz) );
}

extern void _main() {}
