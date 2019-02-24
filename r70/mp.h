typedef struct {
	int len;
	short *val;
} mint;
#define odd(x)	((x->len == 0 || !(x->val[0] & 1))? 0: 1)
#define shfree(u) hfree((short *)u)
#ifndef vax
struct half
{	short high; 	big-endian assumed (also check ifdefs in .c)
	short low;
};
#else
struct half
{	short low;
	short high;
};
#endif
extern mint *itom();
extern short *xalloc();

typedef struct {
	mint *a;
	mint *b;
} form;
#define idform(f) (f->a->len == 1 && f->a->val[0] == 1)
extern short primetab[];	/* all short primes */
extern unsigned char prbits[];	/* the first 10,000 primes, 16 odds to a byte */
