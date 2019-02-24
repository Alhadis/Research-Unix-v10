#include	<sys/param.h>

/*
 * long-long support
 */

#define	M 0x80000000

unsigned
Lshift(ll, l)
llong_t ll;
long l;
{
	return (ll.hi<<(32-l)) | (ll.lo>>l);
}

llong_t
ltoL(l)
long l;
{
	llong_t t;

	t.hi = 0;
	t.lo = l;
	return t;
}

llong_t
Lladd(ll, l)
llong_t ll;
long l;
{
	llong_t t;
	long cin;

	t = ll;
	t.lo += l;
	cin = ll.lo^t.lo;
	if (l>=0) {
		if ((ll.lo&cin)&M)
			t.hi++;
	} else {
		if ((~ll.lo&cin)&M)
			t.hi--;
	}
	return t;
}

llong_t
Luadd(ll, u)
llong_t ll;
unsigned long u;
{
	llong_t t;
	long cin;

	t = ll;
	t.lo += u;
	cin = ll.lo^t.lo;
	if ((ll.lo&cin)&M)
		t.hi++;
	return t;
}

llong_t
LLadd(lla, llb)
llong_t lla, llb;
{
	llong_t t;

	t.hi = lla.hi+llb.hi;
	t.lo = lla.lo+llb.lo;
	if ((lla.lo&llb.lo | lla.lo&~t.lo | llb.lo&~t.lo)&M)
		t.hi++;
	return t;
}

llong_t
Llmul(a, b)
	llong_t a;
	unsigned long b;
{
	llong_t r;

	r = ltoL(0);
	while(b){
		if(b&1)
			r = LLadd(r, a);
		b >>= 1;
		a = LLadd(a, a);
	}
	return(r);
}
