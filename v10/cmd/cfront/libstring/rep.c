#include "String.h"

#define FALSE	0
#define TRUE	(!FALSE)

extern "C" {
	extern char	*memcpy(char *to, const char *from, int);
}

inline char*	// thanx to Jerry Schwarz
my_mcpy(char *to, const char *from, int n)
{
	if (n==1) {
		*to = *from;
		return to;
	} else return memcpy(to, from, n);
}

inline char*
my_Mcpy(char *to, const char *from, int n)
{
	if (n==1) {
		*to = *from;
		return to;
	} else return Memcpy(to, from, n);
}

int	fieldSize = 32768;	/* size of field to allocate */

Rep::Rep(const char *s, unsigned slen)
{
	switch (slen) {
	case 0:
		this = nullRep;
		break;
	case 1:
		this = &oneChar[*s];
		break;
	default:
		this = new Rep(slen);
		my_mcpy(start, s, slen);
		break;
	}
}

Rep::Rep(const Rep &rr)
{
	switch (rr.len) {
	case 0:
		this = nullRep;
		break;
	case 1:
		this = &oneChar[*rr.start];
		break;
	default:
		this = new Rep(rr.len);
		my_mcpy(start, rr.start, len);
		break;
	}
}

Rep::Rep(unsigned slen)
{
	Charfield	*oldCurr = currfield;
	Rep	*temp;
	if (slen <= 1) abort();	// internal String error
	while (!(temp = currfield->newRep(slen)))
		if (currfield->compactify(slen) &&
				(currfield=currfield->next) == oldCurr) {
			oldCurr = oldCurr->next;
			currfield->next = new Charfield(slen);
			(currfield=currfield->next)->next = oldCurr;
		}
	this = temp;
	this->refCount = 1;
}

Rep::~Rep()
{
	if (!is_constant()) {
		Charfield	*field = myField();
		if (field) {
			field->usedSpace -= len;
			field->putMt((MtRep *)this);
		}
	}
	this = 0;
}

Charfield*
Rep::myField() _const
{
	Charfield	*rslt = currfield;
	do {
		if (rslt->field <= (char*)this && rslt->end > (char*)this)
			return rslt;
	} while ((rslt=rslt->next) != currfield);
	return 0;
}

int
Charfield::compactify(unsigned need)
{
	char	*bigBuf;
	register	charSpace = (char*)lastRep - field;
	if (4*usedSpace > 3*charSpace || charSpace - usedSpace - sizeof(Rep) < need)
		return TRUE;
	bigBuf = new char[usedSpace];
	char	*ptr = bigBuf;
	Rep	*aRep = (Rep *)end;
	while ( --aRep >= lastRep ) {
		if( !aRep->isMt() ) {
			my_mcpy(ptr, aRep->start, aRep->len);	/* copy out */
			aRep->start = field + (ptr - bigBuf);
			aRep->mutable();
			ptr += aRep->len;
		}
	}
	my_mcpy(field, bigBuf, usedSpace);	/* copy in */
	firstFree = field + usedSpace;
	delete bigBuf;
	return FALSE;
}

Charfield::Charfield()
{
	end = (firstFree = field = new char[fieldSize]) + fieldSize;
	usedSpace = 0;
	lastRep = (Rep *)end;
	emptyHead = 0;
}

Charfield::Charfield(unsigned sz)
{
	if (sz > fieldSize/2)
		sz = ( sz + fieldSize/2 + 3 ) & ~3;
	else
		sz = fieldSize;
	end = (firstFree = field = new char[sz]) + sz;
	usedSpace = 0;
	lastRep = (Rep *)end;
	emptyHead = 0;
}

Rep*
Charfield::newRep(unsigned ln)
{
	Rep	*rslt;
	/* first get an empty Rep and free space in the field */
	if (rslt = getMt()) {
		if ((char *)lastRep - firstFree >= ln) {
			rslt->start = firstFree;
			rslt->flags = 0;  // not_constant and mutable
			rslt->len = ln;
			firstFree += ln;
			usedSpace += ln;
			return rslt;
		} else
			putMt((MtRep *)rslt);
	}
	/* give up */
	return 0;
}

Rep*
Charfield::getMt()
{
	if (emptyHead) {
		if (emptyHead->dummy != empty_dummy)	// for debugging
			abort();	// double destruction!
		Rep	*rslt = (Rep *) emptyHead;
		emptyHead = emptyHead->next;
		rslt->refCount = 1;
		rslt->flags = 0;
		return rslt;
	}
	if ((char *)lastRep - firstFree >= sizeof(Rep) ) {
		lastRep -= 1;
		lastRep->refCount = 1;
		lastRep->flags = 0;
		return lastRep;
	}
	return 0;
}

void
Charfield::putMt(MtRep *x)
{
	if (emptyHead && emptyHead->dummy != empty_dummy ||
			x->dummy == empty_dummy)	// for debugging
		abort();	// double destruction!
	x->next = emptyHead;
	x->dummy = empty_dummy;	/* mark this as empty */
	emptyHead = x;
}

int
Rep::hashval() const	/* for use in hash tables, etc.  */
{
	register int rslt = 0;
	register char	*p = start;
	register char	*q = start + len;
	while ( p < q ) {
		rslt += rslt + *p++;
		if ( rslt < 0 )
			rslt = -rslt;
	}
	return rslt;
}

Rep*
Rep::newSub(int offset, int length)
{
	Charfield	*myF;
	Rep	*rslt;
	if (!(myF = myField()) || !(rslt = myF->getMt()))
		return NULL;
	rslt->start = start + offset;
	rslt->immutable();
	rslt->len = length;
	immutable();
	myF->usedSpace += length;	/* this space is now shared */
	return rslt;
}

/* try for a new Rep which will be this plus something */
Rep*
Rep::canCat(int plus)
{
	Charfield	*myF = myField();
	if (myF == 0 || start+len != myF->firstFree)
		return 0;	/*  no way if not at top */
	Rep	*rslt = myF->getMt();
	if (rslt)
		if ((myF->firstFree += plus) < (char *)myF->lastRep) {
			rslt->start = start;
			rslt->immutable();
			rslt->len = len + plus;
			immutable();
			myF->usedSpace += len + plus;
		} else {
			myF->firstFree -= plus;	/* good try, but ... */
			myF->putMt((MtRep*)rslt);
			rslt = 0;
		}
	return rslt;
}

bit
Rep::extend(register int plus)	// TRUE if the Rep can be extended (or shrunk)
{
	register Charfield	*myF = myField();
	if (!myF) return FALSE;
	if (plus <= 0) {	// if shrink (could try to adjust firstFree)
		myF->usedSpace += plus;
		len += plus;
		return TRUE;
	}
	if (start + len == myF->firstFree)
		if ((myF->firstFree += plus) < (char *)myF->lastRep) {
			myF->usedSpace += plus;
			len += plus;
			return TRUE;
		} else
			myF->firstFree -= plus;	// good try, but ...
	return FALSE;
}

int
Rep::compare (register const char *q) _const	/* like strcmp(3C) */
{
	/* do this in assembler some day */
	register char	*p = start;
	register int	i = len+1;
	register char	c;

	while (--i && (c = *q++) &&*p++ == c)
		;
	return i ? (c ? (*--p > *--q)*2-1 : 1) : *q ? -1 : 0;
}

int
Rep::match(const Rep& r) _const	// return first differing position
{
	/* do this in assembler some day */
	register char	*p = start;
	register char	*q = r.start;
	register int	i = min(len, r.len)+1;

	while (--i && *p++ == *q++)
		;
	return i ? p - start -1 : p - start;
}

int
Rep::match(register const char *q) _const	// return first differing position
{
	/* do this in assembler some day */
	register char	*p = start;
	register int	i = len+1;
	register char	c;

	while (--i && (c = *q++) &&*p++ == c)
		;
	return i ? (c ? p - start - 1 : p - start) : len;
}

int
Rep::strchr(char c) _const	/* position of first occurrence of char */
{
	/* do this in assembler some day */
	register char	*p = start-1, *q = start + len;
	while ( ++p < q )
		if ( *p == c )
			return p - start;
	return -1;
}

int
Rep::strrchr(char c) _const	/* ... last ... (-1 if char not there) */
{
	/* do this in assembler some day */
	register char	*p = start + len, *q = start;
	while ( --p >= q )
		if ( *p == c )
			return p - start;
	return -1;
}

int
Rep::strpbrk(const Rep &rr) _const
{
	/* do this in assembler some day */
	char	tab[256];
	register char	*p, *q;
	for ( p = tab, q=&tab[256]; p<q;)
		*p++ = 0;
	for ( p = rr.start, q=rr.start+rr.len; p<q; )
		tab[(unsigned char)*p++] = 1;
	for ( p = start-1, q=start+len; ++p < q; )
		if ( tab[(unsigned char)*p] )
			return p - start;
	return -1;
}

int
Rep::strspn(const Rep &rr) _const
{
	/* do this in assembler some day */
	char	tab[256];
	register char	*p, *q;
	for ( p = tab, q=&tab[256]; p<q;)
		*p++ = 0;
	for ( p = rr.start, q=rr.start+rr.len; p<q; )
		tab[(unsigned char)*p++] = 1;
	for ( p = start-1, q=start+len; ++p < q; )
		if ( !tab[(unsigned char)*p] )
			break;
	return p - start;
}

int
Rep::strcspn(const Rep &rr) _const
{
	/* do this in assembler some day */
	char	tab[256];
	register char	*p, *q;
	for ( p = tab, q=&tab[256]; p<q;)
		*p++ = 0;
	for ( p = rr.start, q=rr.start+rr.len; p<q; )
		tab[(unsigned char)*p++] = 1;
	for ( p = start-1, q=start+len; ++p < q; )
		if ( tab[(unsigned char)*p] )
			break;
	return p - start;
}
