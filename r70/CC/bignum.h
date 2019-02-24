// 
// C++ multiple precision integer arithmetic library.
//

#ifndef BIGNUM_H
#define BIGNUM_H
#include <stream.h>

typedef void (*OutFn)(char);

class INT_REP {

	short	refcnt;				// Reference count.
	long	*wt;				// First unused space.
	long	*beg;				// Start of allocated memory.
	long	*last;				// End of allocated memory.
                
	INT_REP (INT_REP *); 		// Duplicate INT_REP.

	void More ();				// Realloc.
	void Zero();				// Zero out everything.
	void Print(iostream &);		// Print using iostream.
	void Print(OutFn, int);		// Print using user supplied fn.
	void Prt();					// Print internal representation.
	void Chsign ();				// Change sign.
	void StripZero();			// Strip redundant zeros.
	void StripMinus();			// Strip redundant -1's.
	int  Twos();				

	int  Length()				{ return (wt-beg); }
	int  isNegative()			{ return ((wt > beg) && (*(wt-1) < 0));  }
	int  isZero ()				{ return (wt == beg); }
	int  isNonzero ()			{ return (wt != beg); }
	int  isOdd ()				{ return (*beg & 0x1); }	// Assuming positive.
	int  isEven ()				{ return (!(*beg & 0x1)); }	// Assuming positive.
	void SetZero ()				{ wt = beg; }
	void Putc(long c)			{ if (wt==last) More(); *wt++ = c; }
	void Alterc(int c, long *p)	{ if (p == last) More(); *p++ = c; if (p > wt) wt = p; }
                  
	void	FillDec(char *, char *);
	void	FillHex(char *, char *);
	void	FillOct(char *, char *);
	void	FillLong(unsigned long);

	INT_REP *PutDec();
	INT_REP *PutHex();
	INT_REP *PutOct();

	int		Store(char *, int, int);

	int     Comp(INT_REP *);
	long    Trunc(int &);
	INT_REP *Add(INT_REP *);
	INT_REP *Sub(INT_REP *);
	INT_REP *Mult(INT_REP *);
	INT_REP *DivMod(INT_REP *, int);
	INT_REP *Div(INT_REP *b)		{ return DivMod(b,0); }
	INT_REP *Mod(INT_REP *b)		{ return DivMod(b,1); }
	INT_REP *Exp(INT_REP *b);
	INT_REP *GCD(INT_REP *b);

	INT_REP *And(INT_REP *);
	INT_REP *Or(INT_REP *);
	INT_REP *Xor(INT_REP *);
	INT_REP *Not();
	INT_REP *LeftShift(long);
	INT_REP *RightShift(long);
	INT_REP *LeftShift(INT_REP *);
	INT_REP *RightShift(INT_REP *);
    
  public :

	static void IRMorehd ();

	INT_REP (int, int = 1);		// Create INT_REP of specified size.
	INT_REP (long);				// Create INT_REP of long.
	INT_REP (char *); 			// Create INT_REP from ascii representation.
	~INT_REP (); 

	friend class INT;
	friend INT_REP *IRAllocRep ();
	friend INT operator >> (INT b, long a);
	friend INT operator << (INT b, long a);
};
	
extern INT_REP ZeroRep;

class INT {
	INT_REP *rep;

	INT Reset (INT_REP *newrep)
	{
		rep->refcnt--;
		if (rep->refcnt == 0)
			delete rep;
		rep = newrep;
		rep->refcnt++;
		return (*this);
	}

	void Print(iostream &i)		// Print decimal representation on iostream.
		{ rep->Print(i); }

  public :

	INT(long i)					// Initialized to i.
		{ rep = new INT_REP (i); if (rep) rep->refcnt++; }
	INT ()						// Initialized to zero.
		{ rep = &ZeroRep; rep->refcnt++;          }
	INT (INT_REP *r)			// For return values from INT_REP operations.
		{ rep = r;	if(r) r->refcnt++;	}
	INT (INT &a)				// For assignment.
		{ rep = a.rep; if(rep) rep->refcnt++; }
	INT (char *s)				// Character representation, in decimal, octal or hex.
		{ rep = new INT_REP (s); if(rep) rep->refcnt++; }
	~INT ()
		{ rep->refcnt--; if(rep->refcnt == 0) delete rep; }
                                
		// Assignment operators.
	INT operator= (INT &a)				{ return (Reset (a.rep)); }
	INT operator+= (INT &a)				{ return (Reset (rep->Add(a.rep))); }
	INT operator-= (INT &a)				{ return (Reset (rep->Sub(a.rep))); }
	INT operator*= (INT &a)				{ return (Reset (rep->Mult(a.rep))); }
	INT operator/= (INT &a)				{ return (Reset (rep->Div(a.rep))); }
	INT operator%= (INT &a)				{ return (Reset (rep->Mod(a.rep))); }
	INT operator|= (INT &a)				{ return (Reset (rep->Or(a.rep))); }
	INT operator&= (INT &a)				{ return (Reset (rep->And(a.rep))); }
	INT operator^= (INT &a)				{ return (Reset (rep->Xor(a.rep))); }
	INT operator<<= (INT &a)			{ return (Reset (rep->LeftShift(a.rep))); }
	INT operator>>= (INT &a)			{ return (Reset (rep->RightShift(a.rep))); }

		// Increment/decrement.
	INT operator++ ()					{ INT_REP orep(1l); return (Reset (rep->Add(&orep))); }
	INT operator-- ()					{ INT_REP orep(-1l); return (Reset (rep->Add(&orep))); }

		// Binary arithmetic operators.
	INT operator + (INT b)				{ return INT(rep->Add(b.rep)); }
	INT operator - (INT b)				{ return INT(rep->Sub(b.rep)); }
	INT operator * (INT b)				{ return INT(rep->Mult(b.rep)); } 
	INT operator / (INT b)				{ return INT(rep->Div(b.rep)); }  
	INT operator % (INT b)				{ return INT(rep->Mod(b.rep)); }  

		// Logical operators.
	int operator <= (INT b)				{ return (rep->Comp(b.rep) == 1 ? 0 : 1); }            
	int operator >= (INT b)				{ return (rep->Comp(b.rep) == -1 ? 0 : 1); }           
	int operator <  (INT b)				{ return (rep->Comp(b.rep) == -1 ? 1 : 0); }           
	int operator >  (INT b)				{ return (rep->Comp(b.rep) == 1 ? 1 : 0); }            
	int operator == (INT b)				{ return (rep->Comp(b.rep) == 0 ? 1 : 0); }            
	int operator != (INT b)				{ return (rep->Comp(b.rep) == 0 ? 0 : 1); }            
	int operator ! ()					{ return (rep->isZero() ? 1 : 0);  }                   
	int operator || (INT b)
		{ return (rep->isNonzero()>0 ? 1 : (b.rep->isNonzero()>0 ? 1 : 0)); }
	int operator && (INT b)
		{ return (rep->isZero() ? 0 : (b.rep->isZero() ? 0 : 1)); }

		// Bitwise operators.
	INT operator >> (INT b)				{ return INT(rep->RightShift(b.rep)); }
	INT operator << (INT b)				{ return INT(rep->LeftShift(b.rep)); }
	INT operator | (INT b)				{ return INT(rep->Or(b.rep)); }
	INT operator & (INT b)				{ return INT(rep->And(b.rep)); }
	INT operator ^ (INT b)				{ return INT(rep->Xor(b.rep)); }
	INT operator ~ ()					{ return INT(rep->Not()); }

		// Miscellaneous.	
	INT operator - ()			// Unary minus.
		{ INT_REP *b = new INT_REP(rep); b->Chsign(); return INT(b); }
	INT Exp (INT b)				// Exponentiate this to the given power.
		{ return INT(rep->Exp(b.rep)); }
	INT GCD (INT b)				// GCD of this and parameter.
		{ return INT(rep->GCD(b.rep)); }
	long Long(int &error)					// Convert this to signed long;
		{ return (rep->Trunc(error));  }	// if bits are lost, error will be nonzero.
	operator void * ()
		{ return (rep->isZero() ? (void *)0 : (void *)rep);  } 
                         
		// Output.
	int	Dec (char *buf, int buflen = -1)	{ return (rep->Store (buf, buflen, 0)); }
	int	Oct (char *buf, int buflen = -1)	{ return (rep->Store (buf, buflen, 1)); }
	int	Hex (char *buf, int buflen = -1)	{ return (rep->Store (buf, buflen, 2)); }
	void Dec (OutFn fn)			{ rep->Print (fn, 0); }
	void Oct (OutFn fn)			{ rep->Print (fn, 1); }
	void Hex (OutFn fn)			{ rep->Print (fn, 2); }
	friend iostream & operator << (iostream &i, INT n)
		{ n.Print(i); return(i); }
#ifdef DEBUG
	void Prt()					// Print internal representation.
		{ rep->Prt(); }
#endif DEBUG

		// Friends.
	friend INT operator >> (INT b, long a);
	friend INT operator << (INT b, long a);

};


			/* Mixed long and INT functions. */

	// Binary arithmetic operators.
inline INT operator + (long a, INT b)				{ return (INT(a) + b); }
inline INT operator + (INT b, long a)				{ return (INT(a) + b); }
inline INT operator - (long a, INT b)				{ return (INT(a) - b); }
inline INT operator - (INT b, long a)				{ return (INT(a) - b); }
inline INT operator * (long a, INT b)				{ return (INT(a) * b); }
inline INT operator * (INT b, long a)				{ return (INT(a) * b); }
inline INT operator / (long a, INT b)				{ return (INT(a) / b); }
inline INT operator / (INT b, long a)				{ return (INT(a) / b); }
inline INT operator % (long a, INT b)				{ return (INT(a) % b); }
inline INT operator % (INT b, long a)				{ return (INT(a) % b); }

	// Logical operators.
inline int operator <= (long a, INT b)				{ return (INT(a) <= b); }
inline int operator <= (INT b, long a)				{ return (INT(a) <= b); }
inline int operator >= (long a, INT b)				{ return (INT(a) >= b); }
inline int operator >= (INT b, long a)				{ return (INT(a) >= b); }
inline int operator <  (long a, INT b)				{ return (INT(a) <  b); }
inline int operator <  (INT b, long a)				{ return (INT(a) <  b); }
inline int operator >  (long a, INT b)				{ return (INT(a) >  b); }
inline int operator >  (INT b, long a)				{ return (INT(a) >  b); }
inline int operator == (long a, INT b)				{ return (INT(a) == b); }
inline int operator == (INT b, long a)				{ return (INT(a) == b); }
inline int operator != (long a, INT b)				{ return (INT(a) != b); }
inline int operator != (INT b, long a)				{ return (INT(a) != b); }
inline int operator || (long a, INT b) 				{ return (a ? 1 : (b ? 1 : 0)); }
inline int operator || (INT b, long a) 				{ return (b ? 1 : (a ? 1 : 0)); }
inline int operator && (long a, INT b) 				{ return (a==0 ? 0 : (b ? 1 : 0)); }
inline int operator && (INT b, long a) 				{ return (!b ? 0 : (a==0 ? 0 : 1)); }

	// Bitwise operators.
inline INT operator >> (long a, INT b)				{ return (INT(a)>>b); }
inline INT operator >> (INT b, long a)				{ return INT(b.rep->RightShift(a)); }
inline INT operator << (long a, INT b)				{ return (INT(a)<<b); }
inline INT operator << (INT b, long a)				{ return INT(b.rep->LeftShift(a)); }
inline INT operator | (long a, INT b)				{ return (INT(a) | b); }
inline INT operator | (INT b, long a)				{ return (INT(a) | b); }
inline INT operator & (long a, INT b)				{ return (INT(a) & b); }
inline INT operator & (INT b, long a)				{ return (INT(a) & b); }
inline INT operator ^ (long a, INT b)				{ return (INT(a) ^ b); }
inline INT operator ^ (INT b, long a)				{ return (INT(a) ^ b); }

	// Miscellaneous functions.
overload Exp;
overload GCD;
inline INT Exp (long a, INT b)						{ return (INT(a).Exp(b)); }
inline INT Exp (INT b, long a)						{ return (b.Exp(INT(a))); }
inline INT GCD (long a, INT b)						{ return (b.GCD(INT(a))); }
inline INT GCD (INT b, long a)						{ return (b.GCD(INT(a))); }

#endif BIGNUM_H
