#ifndef _BITS_H
#define _BITS_H

#include <Block.h>

typedef unsigned long Bits_chunk;
static const int Bits_shift = 5;
static const int Bits_len = 1 << Bits_shift;
static const int Bits_mask = Bits_len - 1;

Blockdeclare(Bits_chunk)

class Bits {
private:
	Block(Bits_chunk) b;
	unsigned n;

	// the chunk number that contains bit n
	unsigned chunk(unsigned n) const {
		return n >> Bits_shift;
	}

	// the number of chunks needed to contain an n-bit string
	unsigned bound(unsigned n) const {
		return (n + Bits_len - 1) >> Bits_shift;
	}

	// a pointer to the (non-existent) chunk immediately
	// after the last one in this Bits
	Bits_chunk* limit() {
		return b + bound(n);
	}

	const Bits_chunk* limit() const {
		return b + bound(n);
	}

	// turn off unused high-order bits in the high-order chunk
	void normalize() {
		register int ct = n & Bits_mask;
		if (ct)
			b[chunk(n)] &= ~(~Bits_chunk(0) << ct);
	}

	int compare(const Bits&) const;
	int equal(const Bits&) const;

public:
	Bits() { n = 0; }
	Bits(Bits_chunk, unsigned = 1);
	unsigned size() const { return n; }
	unsigned size(unsigned);
	friend Bits operator& (const Bits&, const Bits&);
	friend Bits operator| (const Bits&, const Bits&);
	friend Bits operator^ (const Bits&, const Bits&);
	friend Bits operator~ (const Bits&);
	friend Bits operator<< (const Bits&, int);
	friend Bits operator>> (const Bits&, int);
	friend int operator< (const Bits&, const Bits&);
	friend int operator> (const Bits&, const Bits&);
	friend int operator<= (const Bits&, const Bits&);
	friend int operator>= (const Bits&, const Bits&);
	friend int operator== (const Bits&, const Bits&);
	friend int operator!= (const Bits&, const Bits&);
	Bits& operator&= (const Bits&);
	Bits& operator|= (const Bits&);
	Bits& operator^= (const Bits&);
	Bits& operator<<= (int);
	Bits& operator>>= (int);
	Bits& compl();
	Bits& concat(const Bits&);
	Bits& set(unsigned i) {
		if (i < n)
			b[chunk(i)] |= Bits_chunk(1) << (i&Bits_mask);
		return *this;
	}
	Bits& set(unsigned i, unsigned long x) {
		if (i < n) {
			register Bits_chunk* p = &b[chunk(i)];
			register Bits_chunk mask = Bits_chunk(1) << (i&Bits_mask);
			if (x)
				*p |= mask;
			else
				*p &= ~mask;
		}
		return *this;
	}
	Bits& reset(unsigned i) {
		if (i < n)
			b[chunk(i)] &= ~(Bits_chunk(1) << (i&Bits_mask));
		return *this;
	}
	Bits& compl(unsigned i) {
		if (i < n)
			b[chunk(i)] ^= Bits_chunk(1) << (i&Bits_mask);
		return *this;
	}
	unsigned count() const;
	operator Bits_chunk() const;
	int operator[] (unsigned i) const {
		if (i >= n)
			return 0;
		else
			return (b[chunk(i)] >> (i&Bits_mask)) & 1;
	}
	unsigned signif() const;
	unsigned trim() { return size(signif()); }
};

inline int
operator< (const Bits& a, const Bits& b)
{
	return a.compare(b) < 0;
}

inline int
operator> (const Bits& a, const Bits& b)
{
	return a.compare(b) > 0;
}

inline int
operator<= (const Bits& a, const Bits& b)
{
	return a.compare(b) <= 0;
}

inline int
operator>= (const Bits& a, const Bits& b)
{
	return a.compare(b) >= 0;
}

inline int
operator== (const Bits& a, const Bits& b)
{
	return a.equal(b);
}

inline int
operator!= (const Bits& a, const Bits& b)
{
	return !a.equal(b);
}

Bits concat(const Bits&, const Bits&);

#endif
