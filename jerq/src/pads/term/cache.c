#include "univ.h"

char **ICache;
Carte ***CCache;
Index I_SIZE, C_SIZE;

void CacheOp(p)
Protocol p;
{
	Index i;
	register size;
	register Carte *c;
	register Index *rcv;

	i = RcvShort();
	switch( (int) p ){
	case P_I_DEFINE:
		assert(!ICache);
		I_SIZE = i;
		ICache = (char **) Alloc(MJR(I_SIZE) * sizeof(char*) );
		break;
	case P_C_DEFINE:
		assert(!CCache);
		C_SIZE = i;
		CCache = (Carte***) Alloc(MJR(C_SIZE) * sizeof(Carte*) );
		break;
	case P_I_CACHE:
		if( !ICache[MJR(i)] ) GCAlloc(MNR(I_SIZE), &ICache[MJR(i)]);
		RcvString( &ICache[MJR(i)][MNR(i)] );
		break;
	case P_C_CACHE:
		size = RcvUChar();
		if( !CCache[MJR(i)] ) CCache[MJR(i)] =
			(Carte**) Alloc(MNR(C_SIZE) * sizeof(Carte*) );
		assert( !CCache[MJR(i)][MNR(i)] );
		c = (Carte*) GCAlloc(CARTESIZE(size), &CCache[MJR(i)][MNR(i)]);
		c->attrib = RcvUChar();
		for( rcv = c->bin; size-- >= 0; *rcv++ = RcvShort()) {}
		c->size = RcvUChar();			/* recursive size */
		c->width = RcvUChar();			/* recursive widest */
		break;
	default:
		ProtoErr( "CacheOp(): " );

	}
}

char *IndexToStr(i)
Index i;
{
	assert( !(MJR(i)&CARTE) && ICache[MJR(i)] && MJR(i)<MJR(I_SIZE) );
	return &ICache[MJR(i)][MNR(i)];
}

Carte *IndexToCarte(i)
Index i;
{
	assert( MJR(i)&CARTE );
	i &= ~(CARTE<<8);
	assert(MJR(i)<MJR(C_SIZE) && CCache[MJR(i)] && CCache[MJR(i)][MNR(i)]);
	return CCache[MJR(i)][MNR(i)];
}
