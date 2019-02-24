
// 
// C++ multiple precision integer arithmetic library.
//
// Integers are encoded as INT_REPs. An INT_REP contains a 
// reference count, to be used by the user data structure
// INT. The beg pointer points to the beginning of the
// heap allocated array of longs; last points to beg plus
// the length of the array. Integers are represented in
// base 2^16, with the least significant digit written at
// beg, the next at beg+1, etc. The wt pointer points
// to the long after the most significant digit. Negatives
// are stored in twos complement notation, with the most
// significant digit a long -1. 
// Examples:
//  12345678901(10) <=> 2dfdc1c35 <=> beg-> 1c35,dfdc,2
//   3950617284     <=> eb79a2c4  <=> beg-> a2c4,eb79
//  -3950617284     <=>           <=> beg-> 5d3c,1486,-1
// The representations are always normalized to the shortest
// equivalent representation. Thus, zero has length 0;
// beg-> 5d3c,0,0 becomes beg->5d3c, and beg-> ffff,-1 becomes
// beg-> -1.
//
// 2^16 was chosen as the base since it is the largest base
// in which single precision operations can be carried out
// using machine longs (assuming longs have >= 32 bits).
// (This is not entirely true, as an operation in division
// potentially produces a value of more than 32 bits; a trick
// is used to avoid this problem.) Most of the algorithms
// are inherently independent of BASE, as long as BASE^2
// fits in a long. However, there are some short cuts used
// that make use of the fact that the digits contain 16 bits
// and that a long corresponds to 2 digits.
//

#include "bignum.h"
#include <ctype.h>

extern "C" {
	extern int strlen(const char*);
	extern char *realloc (char *, int);
	extern void exit(int);
}

#define NUMHDS 64               /* No. of INT_REPs allocated in a block. */
#define BASE   65536            /* Base BASE arithmetic : 2^16 */
#define BlockLen 9				/* Maximum no. of decimal digits fitting in a long. */

inline int Min(int a,int b)		{ return(a < b ? a : b); }
inline int Max(int a,int b)		{ return(a > b ? a : b); }

#define equalsOne(r)	((r->wt == r->beg+1) && (*(r->beg) == 1))
#define MakeZero(t)		t->beg=new long[0];t->wt=t->last=t->beg

enum Mode {
	decimal, hexadecimal, octal
};

static INT_REP *hfree;					// Pointer to free list of headers.
static INT_REP OneRep("1");             // Representation of 1.
static INT_REP TwoRep("2");             // Representation of 2.
static INT_REP *Remainder;				// Remainder from division, if requested.
static INT_REP BigTen(1000000000L);		// Largest power of 10 fitting in long.
INT_REP ZeroRep(0, 0);					// Representation of 0.
static INT Zero;						// Guarantees refcnt of ZeroRep always > 0.
                                                                         

/* IROutBuf:
 * Construct for handling reverse strings of
 * indefinite length.
 */

#define IRBufSize 256

struct IROutBuf {
	char *beg;
	char *last;
	char *wt;

	IROutBuf()			{ beg = new char[IRBufSize]; wt = beg; last = beg + IRBufSize; }
	~IROutBuf()			{ delete beg; }

	int Length ()		{ return ((int)(wt - beg)); }

	void Flush (register iostream &i) 
	{
		register char *p = wt;

		while (p > beg) {
			i.put(*(--p));
		}
		wt = beg;
	}

	void Flush (register OutFn outf)
	{
		register char *p = wt;

		while (p > beg) {
			(*outf)(*(--p));
		}
		(*outf)('\0');
		wt = beg;
	}

	void Flush (register char *buf, int len) 
	{
		register char *p = wt;
		register char *endp = wt - len;

		while (p > endp) {
			*buf++ = *(--p);
		}
		*buf = '\0';
		wt = beg;
	}

	void Put (char c)
	{
		int newsize, oldsize;

		if (wt == last) {		// Need more storage.
			oldsize = (int)(last - beg);
			newsize = oldsize + IRBufSize;
			beg = realloc (beg, newsize);
			last = beg + newsize;
			wt = beg + oldsize;
		}
		*wt++ = c;
	}

};
static IROutBuf obuf;

/* Fatal:
 * Fatal error; print message and exit.
 */
static void
Fatal (char *msg)
{

	fprintf (stderr, "bignum - %s\n", msg);
	exit (1);
}

/* MakeLong:
 * Convert decimal string to unsigned long.
 * We assume that the decimal will fit.
 */   
unsigned long MakeLong (char *sp, char *ep)
{
	unsigned long ret = 0;
	int c;

	while (sp < ep) {
		c = *sp++;
		if(!isdigit(c))
			Fatal ("MakeLong - illegal decimal digit.");

		ret = 10*ret + (c - '0');
	}
	return(ret);	
	
}

/* HighBit:
 * Given positive long v < 2^16,
 * return number of bits val must be left
 * shifted to be >= 2^15.
 */
int HighBit (register long v)
{
	register int ret = 0;

	while ((v & 0x8000) == 0) {
		v <<= 1;
		ret++;
	}

	return (ret);
}

/* Shift:
 * Given an array of longs, all < 2^16, left shift them shift
 * bits, putting numfill of the resultant words in the storage
 * supplied by fill, using 16 bits per long.
 */
void Shift (long *sp, long *ep, int shift, long *fill, int numfill)
{
	long val;

	// Easy case; no shifting, just copy.
	if (shift == 0) {
		while (numfill--)
			*fill++ = (sp >= ep ? *sp-- : 0);
		return;
	}

	*fill = ((*sp--) << shift) & 0xFFFF;
	while (numfill--) {
		val = (sp >= ep ? *sp-- : 0);
		*fill++ |= (val >> (16 - shift)) & 0xFFFF;
		if (numfill == 0)
			break;
		*fill = (val << shift) & 0xFFFF;
	}

}

/* IRMorehd:
 * Allocate more headers.
 */
void INT_REP::IRMorehd ()
{
	register INT_REP *h, *kk;

	hfree = h = (INT_REP *)new char [sizeof(INT_REP) * NUMHDS];
	if(hfree == 0)
		Fatal ("IRMorehd : no more memory");

	// Link the headers together using the wt pointer.
	kk = h;
	while (h < hfree + NUMHDS) {
		h->wt = ((long *)++kk);
		h++;
	}
	h--;
	h->wt = ((long *)0);
}
   
/* IRAllocRep:
 * Return next available header;
 * allocate more if necessary.
 */
static inline INT_REP *IRAllocRep () {
	register INT_REP *next;

	if (hfree == 0) {
		INT_REP::IRMorehd ();
	}
	next = hfree;
	hfree = (INT_REP *)(next->wt);
	return (next);
}

/* StripZero:
 * Strip trailing zeros and reset wt pointer.
 */
void INT_REP::StripZero()
{
	register long *pp;
         
	if(wt == beg)
		return;
	pp = wt - 1;
	while ((*pp == 0) && (pp > beg))
		pp--;
	if (*pp != 0)
		pp++;
	wt = pp;
}

/* StripMinus:
 * If this is negative,
 * strip trailing -1's and reset wt pointer.
 */
void INT_REP::StripMinus()
{
	register long *pp;

	if(isNegative()) {
		pp = wt - 1;		// Points at -1.
		if (pp > beg) {
			pp--;
			while ((*pp == (BASE-1)) && (pp > beg))
				pp--;
			if (*pp != (BASE-1))
				pp++;
			*pp++ = -1;
			wt = pp;
		}
	}
}

/* Twos:
 * Return largest power of 2 dividing this.
 * Assume this is nonzero.
 */
int INT_REP::Twos()
{
	register int ret = 0;
	register long *p, val;

	p = beg;
	while(*p == 0) {
		ret += 16;
		p++;
	}

	val = *p;
	while ((val & 0x1) == 0) {
		val >>= 1;
		ret++;
	}
	
	return (ret);
}

/* FillLong:
 * Use long to fill this.
 * Assume storage has already been allocated.
 */
inline void INT_REP::FillLong(unsigned long val)
{
	beg[0] = val & 0xFFFF;
	beg[1] = (val >> 16) & 0xFFFF;
	if (val & 0xFFFF0000)
		wt = beg + 2;
	else if (val & 0xFFFF)
		wt = beg + 1;
	else 
		wt = beg;
}

/* Trunc:
 * Use low order digits to fill a long.
 * If digits are lost, set error = 1;
 * else error = 0.
 */
long INT_REP::Trunc(int &error)
{
	long ret = 0;
	int len;
	register long *ptr;

	error = 0;
	if(isZero())
		return(ret);

	len = Length();
	ptr = beg;

	if (isNegative()) {
		if (len == 1)
			ret = -1;
		else {
			ret = *ptr++;
			ret |= (*ptr << 16) & 0x7FFFFFFF;
			if ((len > 3) || ((*ptr & 0x8000) == 0))
				error = 1;
		}
	}
	else {
		ret = *ptr++;
		if (len > 1) {
			ret |= *ptr << 16;
			if ((len > 2) || (*ptr & 0x8000))
				error = 1;
		}
	}

	return (ret);
}

/* Comp:
 * this < b  => -1
 * this == b => 0
 * this > b  => 1
 */
int INT_REP::Comp(INT_REP *b)
{
	INT_REP *diff;
	int df;

	diff = Sub(b);
	if (diff->isZero())
		df = 0;
	else if (diff->isNegative())
		df = -1;
	else
		df = 1;

	delete diff;
	return (df);
}

                       
/* Add:
 * Return new rep, which is the sum of
 * this and b.
 */
INT_REP *INT_REP::Add(INT_REP *b)
{
	register INT_REP *p;
	register long n;
	register int carry;
	int size;
	long *ap, *bp, *pp;

	if ((this == 0) || (b == 0))
		Fatal ("Add - Null pointers.");

	if (isZero()) {
		p = new INT_REP(b);
		return (p);
	}
	if (b->isZero()) {
		p = new INT_REP(this);
		return (p);
	}

	size = Max (Length(), b->Length());
	p = new INT_REP(size);

	ap = this->beg;
	bp = b->beg;
	pp = p->beg;
	carry = 0;
	while(--size >= 0){
		n = (ap == wt ? 0 : *ap++) + (bp == b->wt ? 0 : *bp++) + carry;
		if (n >= BASE) {
			carry = 1;
			n -= BASE;
		}
		else if (n < 0) {
			carry = -1;
			n += BASE;
		}
		else carry = 0;
		*pp++ = n;
	}
	p->wt = pp;
	if(carry != 0)
		p->Putc(carry);

	// Remove trailing zeros.
	p->StripZero();
	
	// Remove redundant -1's in front of terminating -1.
	p->StripMinus();

	return(p);
}

/* Sub:
 * Return new rep, which is the difference of
 * this and b.
 */
INT_REP *INT_REP::Sub(INT_REP *b)
{
	INT_REP *nb, *diff;

	if ((this == 0) || (b == 0))
		Fatal ("Sub - Null pointers.");

	nb = new INT_REP (b);
	nb->Chsign();

	diff = Add(nb);
	delete nb;
	return (diff);
}

/* DivMod:
 * Return new rep, which is:
 * mode == 0 => quotient of this and ddivr;
 * mode == 1 => remainder of quotient of this and ddivr;
 * mode == 2 => quotient of this and ddivr, with the
 *              remainder stored in Remainder.
 * Based on the multiple precision division algorithm
 * in Knuth, v.2.
 */
INT_REP *INT_REP::DivMod(INT_REP *ddivr, int mode)
{
	register INT_REP *quot, *divd, *divr;
	long *quotp, *divrp, *divdp;
	int remsign, divsign, offset;
	long carry, borrow;
	long *ap, q;
	unsigned long uval, delta;
	long val;
	int divlen, n, exp;
	long varray[2], uarray[3];

	if ((this == 0) || (ddivr == 0))
		Fatal ("DivMod - Null arguments.");

	if (ddivr->isZero()) 
		Fatal ("DivMod : divide by zero.");

	// Make divisor and dividend positive.
	Remainder = 0;
	divsign = remsign = 0;
	if (ddivr->isNegative ()) {
		divr = new INT_REP(ddivr);
		divr->Chsign();
		divsign = ~divsign;
	}
	else 
		divr = ddivr;

	divd = new INT_REP(this);
	if (divd->isNegative()) {
		divd->Chsign();
		divsign = ~divsign;
		remsign = ~remsign;
	}

	offset = divd->Length() - divr->Length();
	if(offset < 0) {
		quot = new INT_REP(0);
		goto ddone;
	}

	// Allocate quotient.
	quot = new INT_REP (offset + 1);
	quot->wt = quot->last;

	val = *(divr->wt - 1);			// Most significant "digit" in divisor.
	divlen = divr->Length();

	// If divisor is 1 digit, handle specially.
	if (divlen == 1) {
		quotp = quot->beg + offset;
		divdp = divd->beg + offset;
		carry = 0;

		while (offset-- >= 0) {
			uval = carry*BASE + *divdp--;
			*quotp-- = uval/val;
			carry = uval%val;
		}

		// Set remainder.
		divd->beg[0] = carry;
		divd->wt = divd->beg + 1;

		goto ddone;
	}

	// General case.
	// Initialize. We are essentially finding the least power of 2
	// that we can multiply times the most significant digit of the divisor
	// and make it >= BASE/2. The algorithm then guarantees that our
	// "guess" for the quotient will be at most two greater than the
	// correct answer.

	divd->Putc (0);
	exp = HighBit (val);			// 0 <= exp < 16.
	Shift (divr->wt - 1, divr->beg, exp, varray, 2);
	divdp = divd->wt - 1;
	divd->wt -= offset;
	quotp = quot->wt - 1;

//	fprintf(stderr, "exp = %d , v1 = %x, v2 = %x\n", exp, varray[0], varray[1]);
	while (offset-- >= 0) {
//		divd->Prt();
		Shift (divdp, divd->beg, exp, uarray, 3);
//		fprintf(stderr, "u1 = %x, u2 = %x, u3 = %x\n", uarray[0], uarray[1], uarray[2]);

		// Guess a quotient.
		if (uarray[0] == varray[0])
			q = BASE-1;
		else
			q = ((unsigned long)(BASE*uarray[0] + uarray[1]))/varray[0];
//		fprintf(stderr, "initial q = %x\n",q);

		// Adjust.
		// while q*v1 > (BASE*u0 + u1 - q*v0)BASE + u2, decrement q.
		// To avoid an overflow problem, first check if
		// (BASE*u0 + u1 - q*v0) >= BASE; if it is, the right side is
		// >= BASE^2, while the left is < BASE^2 and we are done.
		// Otherwise, the right side is < BASE^2, and we can do an
		// ordinary arithmetic check.
		while (1) {
			delta = BASE*uarray[0] + uarray[1] - q*varray[0];
			if (delta >= BASE) 
				break;
			if ((unsigned long)(varray[1]*q) <= (unsigned long)(delta*BASE+uarray[2]))
				break;
			q--;
		}
//		fprintf(stderr, "final q = %x\n",q);
		
		// Multiply divisor by q, and subtract from dividend.
		n = divlen;
		carry = 0;
		borrow = 0;
		ap = divdp - n;
		divrp = divr->beg;

		while (n >= 0) {
			if (n > 0)
				uval = q*(*divrp++) + carry;
			else
				uval = carry;

			if (uval >= BASE) {
				carry = uval/BASE;
				uval %= BASE;
			}
			else
				carry = 0;
			
			val = (*ap) - uval - borrow;
//			fprintf(stderr, "n = %d, borrow = %x, carry = %x\n",
//				n, borrow, carry);
//			fprintf(stderr, "*ap = %x, val = %x, uval = %x\n",
//				*ap, val, uval);
			if (val < 0) {
				val += BASE;
				borrow = 1;
			}
			else
				borrow = 0;

			*ap++ = val;
			n--;
		}

		// If guess is still too high (remainder negative => had to borrow), 
		// subtract one from quotient and add one copy of divisor
		// back into remainder.
		if (borrow) {
			*quotp-- = q - 1;
			n = divlen;
			carry = 0;
			ap = divdp - n;
			divrp = divr->beg;

			while (n >= 0) {
				if (n > 0)
					uval = (*ap) + (*divrp++) + carry;
				else
					uval = (*ap) + carry;
//				fprintf(stderr, "borrow n = %d, carry = %x, *ap = %x, uval = %x\n",
//					n, carry, *ap, uval);

				if (uval >= BASE) {
					uval -= BASE;
					carry = 1;
				}
				else
					carry = 0;
				
				*ap++ = uval;
				n--;
			}
		}
		else
			*quotp-- = q;

		divdp--;
	}

ddone:
	if (divr != ddivr) delete divr;
	switch (mode) {
	case 0 : // Return quotient; delete remainder.
		delete divd;
		break;
	case 1 : // Return remainder; delete quotient and pretend
			 // that the remainder is the quotient.
		delete quot;
		quot = divd;
		divsign = remsign;
		break;
	case 2 : // Return quotient; store remainder in Remainder.
		divd->StripZero();
		if (remsign < 0) divd->Chsign();		// Use correct sign.
		Remainder = divd;
		break;
	}

	// Remove prefix zeros.
	quot->StripZero();
	if (divsign < 0) quot->Chsign();		// Use correct sign.

	return (quot);
}

/* Exp:
 * Return new rep, which is the first argument
 * raised to the second argument power.
 */
INT_REP *INT_REP::Exp(INT_REP *exp)
{
	INT_REP *r;
	INT_REP *e;
	INT_REP *p;
	INT_REP *t, *cp, *e1;
	int neg = 0;

	r = new INT_REP(&OneRep);

	// Check for special cases.
	if((exp->isZero()) || equalsOne(this))
		return (r);
	if(isZero()) {
		r->SetZero();
		return (r);
	}

	// Create copies of arguments.
	p = new INT_REP(this);
	e = new INT_REP(exp);

	// Check for negative exponent.
	if (e->isNegative()) {
		neg++;
		e->Chsign();
	}

	// Compute exponentiation.
	while (e->isNonzero()) {
		e1 = e->DivMod(&TwoRep,2);
		delete e;
		e = e1;

		if (Remainder->isNonzero()) {
			e1 = p->Mult(r);
			delete r;
			r = e1;
		}
		delete Remainder;

		t = new INT_REP(p);
		cp = p->Mult(t);
		delete p;
		delete t;
		p = cp;
	}

	delete e;
	delete p;

	// Adjust for negative exponent.
	if (neg) {
		t = OneRep.Div(r);
		delete r;
		return (t);
	}
	else
		return (r);

}

/* Mult:
 * Return new rep, which is the product of
 * this and ddivr.
 */
INT_REP *INT_REP::Mult(INT_REP *q)
{
	register INT_REP *mp,*mq,*mr;
	int sign,offset,carry;
	unsigned long mt;
	long cq, cp, mcr;
	long *pptr, *qptr, *rptr;

	if ((this == 0) || (q == 0))
		Fatal ("Mult - Null pointers.");

	offset = sign = 0;
	if (isNegative()) {
		mp = new INT_REP (this);
		mp->Chsign();
		sign = ~sign;
	}
	else 
		mp = this;

	if (q->isNegative()) {
		mq = new INT_REP (q);
		mq->Chsign();
		sign = ~sign;
	}
	else
		mq = q;

	mr = new INT_REP (mp->Length() + mq->Length());
	mr->Zero();
	qptr = mq->beg;
	while (qptr < mq->wt) {
		cq = *qptr++;
		pptr = mp->beg;
		rptr = mr->beg + offset;
		carry = 0;
		while (pptr < mp->wt) {
			cp = *pptr++;
			mcr = (rptr == mr->wt ? 0 : *rptr);
			mt = cp*cq + carry + mcr;
			carry = mt>>16; 						// mt/BASE.
			mr->Alterc(mt&0xFFFF, rptr++); 			// mt%BASE.
		}
		offset++;
		if(carry != 0){
			mcr = (rptr == mr->wt ? 0 : *rptr);
			mr->Alterc(mcr+carry, rptr);			// Why is mcr+carry < BASE?
		}
	}

	if(sign < 0){
		mr->Chsign();
	}
	if(mp != this) delete mp;
	if(mq != q) delete mq;

	return (mr);
}
                                       
/* INT_REP:
 * Create new rep, with
 * size bytes of storage.
 * If setRef == 0, do not initialize refcnt. This is used by
 * ZeroRep in case it gets initialized after some INT with
 * no arguments.
 */
INT_REP::INT_REP (int size, int setRef)
{
	long *ptr;

	if (this == 0)
		this = IRAllocRep ();
                       
	ptr = new long[(unsigned)size];
	if (ptr == 0)
		Fatal ("INT_REP - Out of memory.");

	wt = beg = ptr;
	last = ptr + size;
	if (setRef) refcnt = 0;
}

/* INT_REP:
 * Create new rep, 
 * using supplied long.
 */
INT_REP::INT_REP (long val)
{
	int neg = 0;
	int size = 2;		// Number of longs needed to store positive 32 bits.

	if (this == 0)
		this = IRAllocRep ();

	if (val == 0) {		// If zero, ...
		MakeZero (this);
		return;
	}

	if (val < 0) {
		neg++;
		size++;			// May need extra word for sign.
	}
	beg = new long[size];
	if (beg == 0)
		Fatal ("INT_REP - Out of memory.");
	last = beg + size;
	refcnt = 0;
               
	// Store bits.
	FillLong((unsigned long)val);

	// If negative, add sign, and strip redundant signs.
	if (neg) {
		Putc(-1);
		StripMinus();
	}
	
}

/* INT_REP:
 * Create new rep, 
 * duplicating old rep.
 */
INT_REP::INT_REP (INT_REP *old)
{
	long *ptr, *oldptr;
	int size = old->Length();

	if (this == 0)
		this = IRAllocRep ();
                       
	ptr = new long[(unsigned)size];
	if (ptr == 0)
		Fatal ("INT_REP - Out of memory.");
	beg = ptr;
	wt = last = ptr + size;
	refcnt = 0;

	oldptr = old->beg;
	while (ptr < last) {
		*ptr++ = *oldptr++;
	}
}

/* INT_REP:
 * Create new rep, 
 * from ascii representation s.
 */
INT_REP::INT_REP (char *s)
{
	register char *optr;
	short minus = 0;
	Mode mode;
	int size;
             
	if (this == 0)
		this = IRAllocRep ();
	refcnt = 0;
                      
	// Handle NULL pointer or NULL string.
	if ((s == (char *)0) || (*s == '\0')) {
		MakeZero(this);
		return;
	}
        
	// Check for sign.
	if (*s == '-') {
		s++;
		minus = 1;
	}

	// Determine mode.
	if (*s == '0') {
		s++;
		if (*s == '\0') {
			MakeZero(this);
			return;
		}
		else if ((*s == 'x') || (*s == 'X')) {
			s++;
			mode = hexadecimal;
		}
		else
			mode = octal;
	}
	else
		mode = decimal;

	// Find end of string.
	optr = s;
	while(*optr) optr++;
	if (s == optr) 
		Fatal ("INT_REP : Improper string.");
                        
	// Fill representation, using correct mode.
	if (mode == decimal)
		FillDec (s, optr);
	else {
		// Allocate enough space.
		size = (strlen(s) + 3)/4;
		beg = new long[size];
		if (beg == 0)
			Fatal ("INT_REP - Out of memory.");
		last = beg + size;

		// Fill.
		if (mode == hexadecimal)
			FillHex(s, optr);
		else
			FillOct(s, optr);

		// Remove extraneous zeros.
		StripZero();
	}
                          
	// Correct for sign.
	if (minus)
		Chsign ();
}
                               
/* ~INT_REP:
 * destructor.
 */
INT_REP::~INT_REP ()
{
	if(beg) delete beg;

	// Put back on free list.
	wt = (long *)hfree;
	hfree = this;
	this = 0;
}

/* More:
 * Obtain more storage for this,
 * using realloc, and resetting
 * pointers.
 */
void INT_REP::More()
{ 
	register unsigned size;
	register long *p;

	if ((size = (last - beg) * 2) == 0) 
		size = 1;
	p = (long *)realloc((char *)beg, (unsigned)(size*sizeof(long)));
	if(p == 0)
		Fatal ("More: realloc failed");

	wt = p + (wt - beg);
	beg = p;
	last = p + size;
}

/* Zero:
 * Zero out all the storage.
 */
void INT_REP::Zero()
{ 
	register long *pp;

	for (pp = beg; pp < last;)
		*pp++ = 0;
}


/* Prt:
 * Print internal information;
 * for debugging.
 */
void INT_REP::Prt()
{
	long *ptr = beg;

	while (ptr != wt)
		fprintf(stderr, "%x,", *ptr++);
	fprintf (stderr, "   : this = %x, beg = %x, last = %x, wt = %x\n",
		this, beg, last, wt);
}
   
/* PutDec:
 * Store decimal representation of this in obuf.
 * NOTE : The routine destroys this.
 */
INT_REP *INT_REP::PutDec ()
{
	register INT_REP *divd = this, *div, *mod;
	register long val;
	int count;

	while (1) {
		div = divd->DivMod (&BigTen, 2);
		mod = Remainder;
//		div->Prt();
//		mod->Prt();

		// Store remainder.
		val = 0;
		switch (mod->Length()) {
		case 2 :
			val = (mod->beg[1]) << 16;
			/* Fall through. */
		case 1 :
			val |= (mod->beg[0]);
		}
//		fprintf (stderr, "val = %d (0x%x)\n",val, val);
		delete mod;
		delete divd;

		count = BlockLen;
		while (val) {
			obuf.Put((char)(val%10 + '0'));
			val /= 10;
			count--;
		}

		if (div->isZero())
			return (div);

		while (count--)
			obuf.Put('0');
		divd = div;
	}

}

/* PutHex:
 * Store hexadecimal representation of this in obuf.
 */
INT_REP *INT_REP::PutHex ()
{
	register unsigned long	*ptr = (unsigned long *)beg;
	register unsigned long	val, digit;
	register int			count;

	while (1) {
		val = *ptr++;
		count = 4;				// 4 hex digits per long

		while (val) {         
			if ((digit = (val & 0xF)) < 10) 
				obuf.Put((char)(digit + '0'));
			else
				obuf.Put((char)(digit + ('A' - 10)));
			val >>= 4;
			count--;
		}

		if (ptr == (unsigned long *) wt)
			return (this);

		while (count--)
			obuf.Put ('0');
	}
}

/* PutOct:
 * Store octal representation of this in obuf.
 */
INT_REP *INT_REP::PutOct ()
{
	register unsigned long	*ptr = (unsigned long *)beg;
	register unsigned long	val = 0;
	register int			count;
	register int			numbits = 0;

	while (1) {
		val |= (*ptr++) << numbits;
		if (numbits == 2)
			count = 6;
		else 
			count = 5;
		numbits += 16;

		while (val && count) {
			obuf.Put((char)((val & 0x7) + '0'));
			val >>= 3;
			numbits -= 3;
			count--;
		}
     
		if (ptr == (unsigned long *) wt)
			return (this);
                               
		if (count) {
			numbits -= (count * 3);
			while (count--) 
				obuf.Put ('0');
		}
	}
}

/* Print:
 * Print INT_REP.
 * We use the output mode supplied by the iostream.
 * After taking care of 0 and the minus sign,
 * we use the appropriate function to store the
 * representation in obuf.
 * Finally, we flush obuf onto the iostream.
 */
void INT_REP::Print(iostream &i)
{
	register INT_REP *divd;
#ifdef HEX
	register long *rp;
#endif HEX
	
	if (Length() == 0) {
		i.put('0');
		return;
	}

	divd = new INT_REP (this);
	if (isNegative()) {
		divd->Chsign();
		i.put('-');
	}
                   

	// Print.
#ifdef PRE6
	switch (i.convbase()) {
	case 0 :
	case 10 :
		divd = divd->PutDec ();
		break;
	case 8 :
		divd = divd->PutOct ();
		break;
	case 16 :
		divd = divd->PutHex ();
		break;
	}
#else
	switch (i.flags()) {
	default :
	case ios::dec :
		divd = divd->PutDec ();
		break;
	case ios::oct :
		divd = divd->PutOct ();
		break;
	case ios::hex :
		divd = divd->PutHex ();
		break;
	}
#endif
        
	delete divd;
	obuf.Flush(i);
#ifdef HEX
	rp = wt - 1;
	while (rp >= beg)
		printf("%04x", *rp--);
	printf("\n");
#endif HEX

}
              
/* Print:
 * Print INT_REP.
 * We use the output function supplied by the user.
 * The mode parameter determines the output mode.
 * After taking care of 0 and the minus sign,
 * we use the appropriate function to store the
 * representation in obuf.
 * Finally, we flush obuf using the user's function.
 */
void INT_REP::Print(OutFn outf, int mode)
{
	register INT_REP *divd;
	
	if (Length() == 0) {
		(*outf)('0');
		(*outf)('\0');
		return;
	}

	divd = new INT_REP (this);
	if (isNegative()) {
		divd->Chsign();
		(*outf)('-');
	}
                   
	// Print.
	switch (mode) {
	case 0 :
		divd = divd->PutDec ();
		break;
	case 1 :
		divd = divd->PutOct ();
		break;
	case 2 :
		divd = divd->PutHex ();
		break;
	}
        
	delete divd;
	obuf.Flush(outf);

}
              
/* Store:
 * Store representation of this in given buffer,
 * null terminated.
 * Buflen tells how many characters are in the user's
 * buffer; -1 => unspecified.
 * Representation is base 10 <=> mode = 0
 *                   base  8 <=> mode = 1
 *                   base 16 <=> mode = 2
 * Return number of (non-null) characters.
 */
int INT_REP::Store (char *buf, int buflen, int mode)
{
	register INT_REP	*divd;
	int					length;
	                           
	if (buflen == 0)
		return (0);
	else if (buflen == 1) {
		*buf = '\0';
		return (0);
	}

	if (isZero()) {
		*buf++ = '0';
		*buf = '\0';
		return (1);
	}

	divd = new INT_REP (this);
	if (isNegative())
		divd->Chsign();
                   
	// Print.
	switch (mode) {
	case 0 :
		divd = divd->PutDec ();
		break;
	case 1 :
		divd = divd->PutOct ();
		break;
	case 2 :
		divd = divd->PutHex ();
		break;
	}
	delete divd;          

	// If necessary, add minus sign.
	if (isNegative())
		obuf.Put('-');		
                                    
	// Compute number of characters to store.
	length = obuf.Length ();
	if ((buflen > 0) && (length + 1 > buflen))
		length = buflen - 1;

	obuf.Flush(buf, length);

	return (length);

}

/* Chsign:
 * Change sign of the rep.
 */
void INT_REP::Chsign ()
{
	register long carry;
	register long ct;
	register long *readp;

	if (Length () == 0) return;

	carry = 0;
	readp = beg;
	while (readp != wt) {
		ct = BASE - *readp - carry;

		if(ct >= BASE) {
			ct -= BASE;
			if (ct == 1) {
				*readp = 1;
				return;
			}
			else if (carry == 1) {
				wt = readp;
				return;
			}
			else
				carry = 0;
		}
		else
			carry = 1;

		*readp++ = ct;
	}
	readp--;
	if (*readp == BASE-1)
		*readp = -1;
	else
		Putc(-1);

}
   
/* FillDec:
 * Use decimal representation to fill this.
 * sp points to first character, ep points
 * to terminating NULL. Use standard decimal
 * to binary conversion. Assume no
 * storage has already been allocated.
 */
void INT_REP::FillDec(char *sp, char *ep)
{
	char *pp;
	INT_REP *u, *newi, *product;
	unsigned long val;
	
    // Create utility INT_REP.
	u = new INT_REP(2);
	
	// Determine leftmost block of characters.
	pp = sp + ((int)(ep - sp))%BlockLen;

	// Convert to an INT_REP.
	val = MakeLong(sp, pp);
	u->FillLong (val);
	sp = pp;
	newi = new INT_REP (u);

	while (sp < ep) {
		product = newi->Mult(&BigTen);
		delete newi;
		val = MakeLong(sp, sp+BlockLen);
		u->FillLong (val);
		sp += BlockLen;
		newi = product->Add(u);
		delete product;
	}

	// Copy newi into this.
	beg = newi->beg;
	last = newi->last;
	wt = newi->wt;
	newi->beg = 0;

	// Delete temporaries.
	delete newi;
	delete u;
	
}

/* FillHex:
 * Use hex representation to fill this.
 * sp points to first character, ep points
 * to terminating NULL. Assume adequate 
 * storage has already been allocated.
 */
void INT_REP::FillHex(char *sp, char *ep)
{
	long *lptr = beg;
	long word = 0;
	long c;
	short shift = 0;

	while (ep > sp) {
		c = *(--ep);
		if(!isxdigit(c))
			Fatal ("FillHex - Illegal hex character.");
		if (isdigit(c))
			c -= '0';
		else if (islower(c))
			c -= 'a' - 10;
		else
			c -= 'A' - 10;

		word |= c << shift;
		shift += 4;
		if (shift == 16) {
			shift = 0;
			*lptr++ = word;
			word = 0;
		}
	}	

	// Add incomplete word.
	if (shift)
		*lptr++ = word;
	
	// Set wt.
	wt = lptr;
}

/* FillOct:
 * Use octal representation to fill this.
 * sp points to first character, ep points
 * to terminating NULL. Assume adequate 
 * storage has already been allocated.
 */
void INT_REP::FillOct(char *sp, char *ep)
{
	long *lptr = beg;
	long word = 0;
	long c;
	short shift = 0;

	while (ep > sp) {
		c = *(--ep);
		if(!isdigit(c) || (c > '7'))
			Fatal ("FillOct - Illegal octal character.");

		c -= '0';
		word |= c << shift;
		shift += 3;
		if (shift >= 16) {
			*lptr++ = word & 0xFFFF;
			shift -= 16;
			word = c >> (3 - shift);
		}
	}	

	// Add incomplete word.
	if (shift)
		*lptr++ = word;
	
	// Set wt.
	wt = lptr;
	
}

/* And:
 * Bit-wise and.
 */
INT_REP *INT_REP::And(INT_REP *b)
{
	INT_REP *newi;
	int length;
	long *ap, *bp, *pp;

	if (isZero() || b->isZero())
		return (&ZeroRep);

	length = Min(Length(), b->Length());
	newi = new INT_REP(length);

	ap = beg;
	bp = b->beg;
	pp = newi->beg;

	while (length-- > 0)
		*pp++ = (*ap++) & (*bp++);
	newi->wt = pp;
                  
	// Strip trailing zeros.
	newi->StripZero();

	return (newi);
}

/* Or:
 * Bit-wise or.
 */
INT_REP *INT_REP::Or(INT_REP *b)
{
	INT_REP *newi;
	int length;
	long *ap, *bp, *pp;

	if (isZero()) {
		newi = new INT_REP(b);
		return newi;
	}	
	if (b->isZero()) {
		newi = new INT_REP(this);
		return newi;
	}	

	if (isNegative()) {
		if(b->isNegative())
			length = Min(Length(), b->Length());
		else
			length = Length();
	}
	else {
		if(b->isNegative())
			length = b->Length();
		else
			length = Max(Length(), b->Length());
	}
	newi = new INT_REP(length);

	ap = beg;
	bp = b->beg;
	pp = newi->beg;

	while (length-- > 0)
		*pp++ = (ap < wt ? *ap++ : 0) & (bp < b->wt ? *bp++ : 0);
	newi->wt = pp;

	// Strip possible trailing -1's.
	newi->StripMinus();
     
	return (newi);

}

/* Xor:
 * Bit-wise xor.
 */
INT_REP *INT_REP::Xor(INT_REP *b)
{
	INT_REP *newi;
	INT_REP *pos, *neg;
	long *ap, *bp, *pp;
	long extval;
	int length;

	if (isZero()) {
		newi = new INT_REP(b);
		return newi;
	}	
	if (b->isZero()) {
		newi = new INT_REP(this);
		return newi;
	}	

	if(isNegative() == b->isNegative()) {			// Same sign.
		length = Max(Length(), b->Length());
		newi = new INT_REP(length);

		ap = beg;
		bp = b->beg;
		pp = newi->beg;
		if(isNegative())
			extval = -1;
		else
			extval = 0;

		while (length-- > 0)
			*pp++ = ((ap < wt ? *ap++ : extval) ^ (bp < b->wt ? *bp++ : extval)) & 0xFFFF;
		newi->wt = pp;

		// Remove trailing zeros.
		newi->StripZero();
	}
	else {											// Different sign.
		// Set neg pointing to negative value, pos pointing to positive.
		if (isNegative()) {
			neg = this;
			pos = b;
		}
		else {
			neg = b;
			pos = this;
		}

		length = Max(pos->Length()+1, neg->Length());		
		newi = new INT_REP(length);

		ap = pos->beg;
		bp = neg->beg;
		pp = newi->beg;

		while (--length > 0)
			*pp++ = ((ap < pos->wt ? *ap++ : 0) ^ (bp < neg->wt ? *bp++ : -1)) & 0xFFFF;
		*pp++ = -1;
		newi->wt = pp;

		// Remove trailing -1's.
		newi->StripMinus();
	}

	return (newi);
}

/* Not:
 * Bit-wise not.
 */
INT_REP *INT_REP::Not()
{
	int length;
	long *ap, *pp;
	INT_REP *newi;

	if (isZero()) {
		newi = new INT_REP(1);
		newi->Putc(-1);
		return (newi);
	}

	if (isNegative()) {
		length = Length() - 1;
		newi = new INT_REP(length);

		ap = beg;
		pp = newi->beg;
		while(length--)
			*pp++ = (~(*ap++)) & 0xFFFF;
		newi->wt = pp;
	}
	else {
		length = Length();
		newi = new INT_REP(length+1);

		ap = beg;
		pp = newi->beg;
		while(length--)
			*pp++ = (~(*ap++)) & 0xFFFF;
		*pp++ = -1;
		newi->wt = pp;
	}

	return (newi);
}

/* LeftShift:
 * Bit-wise left shift.
 */
INT_REP *INT_REP::LeftShift(INT_REP *b)
{
	long shift;
	INT_REP *bb, *newi;
	int error;

	shift = b->Trunc(error);
	if (!error)  			// Shift can be represented as a long.
		return(LeftShift(shift));

	if (isZero()) {
		newi = new INT_REP(this);
		return newi;
	}
	if (b->isNegative()) {
		bb = new INT_REP(b);
		bb->Chsign();
		newi = RightShift(bb);
		delete bb;
		return (newi);
	}

	bb = TwoRep.Exp(b);
	newi = Mult(bb);
	delete bb;
	return (newi);
	
}

INT_REP *INT_REP::LeftShift(long shift)
{ 
	INT_REP *newi;
	int length;
	int bitshift;
	long val;
	long *ap, *pp;

	// Special cases.
	if ((shift == 0) || isZero()) {
		newi = new INT_REP(this);
		return newi;
	}
	if (shift < 0) {
		return (RightShift(-1*shift));
	}

	length = Length() + ((shift + 15)>>4);	// (shift + 15)/16.
	bitshift = shift & 0xF;					// shift%16.
	newi = new INT_REP(length);
	newi->wt = newi->last;

	// Initialize.
	ap = wt-1;
	pp = newi->last - 1;

	// Shift.
	if(bitshift) {
		if (isNegative())
			*pp = -1;
		else
			*pp = 0;

		while (ap >= beg) {
			val = *(ap--) & 0xFFFF;
			*pp-- |= val >> (16 - bitshift);
			*pp = (val << bitshift) & 0xFFFF;
		}
	}
	else {
		while (ap >= beg)  {
			*pp-- = *ap--;
		}
		pp++;
	}

	// Fill with 0's.
	while (pp > newi->beg)
		*(--pp) = 0;


	if (isNegative())
		newi->StripMinus();
	else
		newi->StripZero();
	
	return (newi);
}

/* RightShift:
 * Bit-wise right shift.
 */
INT_REP *INT_REP::RightShift(INT_REP *b)
{
	long shift;
	INT_REP *bb, *newi;
	int error;

	shift = b->Trunc(error);
	if (!error)  			// Shift can be represented as a long.
		return(RightShift(shift));

	if (isZero()) {
		newi = new INT_REP(this);
		return newi;
	}
	if (b->isNegative()) {
		bb = new INT_REP(b);
		bb->Chsign();
		newi = LeftShift(bb);
		delete bb;
		return (newi);
	}

	bb = TwoRep.Exp(b);
	newi = Div(bb);
	delete bb;
	return (newi);
	
}

INT_REP *INT_REP::RightShift(long shift)
{
	INT_REP *newi;
	int length;
	int offset, bitshift;
	long val;
	long *ap, *pp;

	// Special cases.
	if ((shift == 0) || isZero()) {
		newi = new INT_REP(this);
		return newi;
	}
	if (shift < 0) {
		return (LeftShift(-1*shift));
	}

	offset = shift>>4;				// shift/16.
	// If offset >= Length(), shifting produces either 0 or -1.
	if (offset >= Length()) {
		if (isNegative()) {
			newi = new INT_REP(1);
			newi->Putc(-1);
			return newi;
		}
		else {
			newi = new INT_REP(0);
			return newi;
		}
	}

	length = Length() - offset;
	bitshift = shift & 0xF;			// shift%16.
	newi = new INT_REP(length);
	newi->wt = newi->last;

	// Initialize.
	ap = beg + offset;
	pp = newi->beg;

	// Shift.
	if (bitshift) {
		*pp = (*ap++ >> bitshift) & 0xFFFF;

		while (ap < wt) {
			val = *ap++ & 0xFFFF;
			*pp++ |= (val << (16 - bitshift)) & 0xFFFF;
			*pp = (val >> bitshift);
		}

		if (isNegative()) {
			*pp = -1;
			newi->StripMinus();
		}
		else
			newi->StripZero();
	}
	else {
		while (ap < wt) 
			*pp++ = *ap++;
	}
	
	return (newi);
}

/* GCD:
 * Returns greatest common divisor.
 */
INT_REP *INT_REP::GCD(INT_REP *b)
{
	INT_REP *newi;
	INT_REP *t, *u, *v, *tmp;
	int k, l;

	// If this or b is 0.
	if (isZero()) {
		newi = new INT_REP (b);
		if (b->isNegative())
			newi->Chsign();
		return (newi);
	}
	if (b->isZero()) {
		newi = new INT_REP (this);
		if (this->isNegative())
			newi->Chsign();
		return (newi);
	}

	// Make this and b positive.
	if (isNegative()) {
		u = new INT_REP (this);
		u->Chsign();
	}
	else
		u = this;
	if (b->isNegative()) {
		v = new INT_REP (b);
		v->Chsign();
	}
	else
		v = b;

	// Compute highest power of 2 dividing both this and b;
	// divide out this power.
	k = Min (Twos(), b->Twos());
	tmp = u->RightShift(k);
	if (u != this) delete u;
	u = tmp;
	tmp = v->RightShift(k);
	if (v != b) delete v;
	v = tmp;

	// Initialize.
	if (u->isOdd()) {
		t = new INT_REP (v);
		t->Chsign();
	}
	else 
		t = new INT_REP (u);
	
	// Loop.
	while (t->isNonzero()) {
		
		// Remove powers of 2 from t.
		l = t->Twos();
		tmp = t->RightShift (l);
		delete t;
		t = tmp;

		if (t->isNegative()) {
			delete v;
			v = new INT_REP (t);
			v->Chsign();
		}
		else {
			delete u;
			u = new INT_REP (t);
		}

		delete t;
		t = u->Sub(v);
	}

	// Add in power of 2 computed above.
	newi = u->LeftShift (k);

	delete u;
	delete v;

	return (newi);
}

