/*ident	"@(#)ctrans:src/Block.h	1.2" */

#ifndef BLOCK_H
#define BLOCK_H
#include <new.h>

PT_names
	Block(T)	Block_ T
PT_end

PT_define Blockdeclare(T)

class Block(T) {
public:
	unsigned size() const { return n; }
	unsigned size(unsigned);
	Block(T)() { n = 0; p = 0; }
	Block(T)(unsigned k) { n = 0; p = 0; size(k); }
	Block(T)(const Block(T)& b) { copy(b); }
	~Block(T)();
	Block(T)& operator=(const Block(T)&);
	operator T*() { return p; }
	operator const T*() const { return p; }
	T* end() { return p + n; }
	const T* end() const { return p + n; }
	T& operator[](int i) { return p[i]; }
	const T& operator[](int i) const { return p[i]; }
	int reserve(unsigned k) { return k<n || grow(k); }
	void swap(Block(T)& b);
private:
	T* p;
	unsigned n;
	void move(T*, unsigned);
	void transfer(T*, unsigned);
	void clear(T*, unsigned);
	void copy(const Block(T)&);
	unsigned grow(unsigned);
	static T* default_value();
};

PT_end

PT_define Blockimplement(T)

unsigned Block(T)::size(unsigned k)
{
	if (k != n)
		move(new T[k], k);
	return n;
}
Block(T)::~Block(T)()
{
	delete[] p;
}
Block(T)& Block(T)::operator=(const Block(T)& b)
{
	delete[] p;
	copy(b);
	return *this;
}

/* Clear k elements starting at v */
void
Block(T)::clear(T* v, unsigned k)
{
	register T* p = v;
	register T* lim = v + k;
	T* valptr = default_value();
	while (p < lim) {
		*p++ = *valptr;
	}
}

/* Make this a copy of b */
void
Block(T)::copy(const Block(T)& b)
{
	// assert (p is 0 or garbage)
	p = new T[b.n];
	if (p) {
		n = b.n;
		transfer(b.p, n);
	} else
		n = 0;
}

/* Grow this Block by 1.5 until it can contain at least k+1 */
unsigned
Block(T)::grow(unsigned k)
{
	unsigned nn = n;
	if (nn == 0)
		nn++;
	while (nn <= k)
		nn += (nn >> 1) + 1;
	T* np = new T[nn];
	if (!np) {
		nn = k+1;
		np = new T[nn];
	}
	move(np, nn);
	return n;
}

/* Transfer len (or fewer) elements into this Block. */
void
Block(T)::transfer(T* source, unsigned len)
{
	register T* plim;
	register T* pp = p;
	register T* q = source;

	if (n > len) {
		plim = p + len;
		clear(plim, n - len);
	} else
		plim = p + n;

	while (pp < plim)
		*pp++ = *q++;
}

/*
 * The contents of this Block now live in memory starting at np
 * If np is 0, null out this Block.
 */
void
Block(T)::move(T* np, unsigned nn)
{
	T* oldp = p;
	unsigned oldn = n;
	p = np;
	if (np) {
		n = nn;
		transfer(oldp, oldn);
	} else
		n = 0;
	delete[] oldp;
}

/* Exchange the contents of this Block with another Block */
void
Block(T)::swap(Block(T)& b)
{
	T* bp = b.p;
	unsigned bn = b.n;
	b.p = p;
	b.n = n;
	p = bp;
	n = bn;
}

T*
Block(T)::default_value() {
	static T default_item;
	return(&default_item);
}

PT_end


#endif
