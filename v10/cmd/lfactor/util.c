#define testing 0
/*
 * compos performs the composition of two reduced_______ forms.
 * Thus we are entitled to assume that
 * |b| <= a <= c
 * and
 * a,b < 1e16
 */
#include "mp.h"
#include "form.h"
form
compos(f1,f2)
form f1, f2;
{
	form f3;
	mint m, r, y;
	mint yq, yr;
	mint v1, v2;
	mint d, d1, s, n;
	mint x1, x2, y1, y2;
	mint temp, temp1, temp2;
	mint zero;
	double dinv();
	double modf();

	zero = itom(0);
	n.high = s.high = 0;
	s.low = (f1.b.low + f2.b.low)*0.5;
	n.low = s.low - f2.b.low;
#if testing
	temp1 = msub(mult(f1.c,f1.a),mult(f2.c,f2.a));
	if(mcmp(temp1,mult(n,s)) != 0){
		printf("compos: unequal determinants!\n");
		tobort(5);
	}
#endif
	d.high = 0;
	x1.high = y1.high = 0;
	d.low = dinv(f1.a.low,f2.a.low,&x1.low,&y1.low);
	d1.high = 0;
	x2.high = 0;
	y2.high = 0;
	v1.high = v2.high = 0;
	if(d.low == 1){
		d1.low = 1;
		v1.low = f1.a.low;
		v2.low = f2.a.low;
		m = mult(y1,n);
	}else{
		d1.low = dinv(s.low,d.low,&x2.low,&y2.low);
		v1.low = f1.a.low/d1.low;
		v2.low = f2.a.low/d1.low;
		mdiv(mult(y1,y2), v1, &temp);
		temp1 = mult(temp, n);
		temp2 = mult(x2, f2.c);
		m = msub(temp1, temp2);
	}
	mdiv(m, v1, &r);
	if((r.low + r.low) > v1.low)
		r.low -= v1.low;
	else if((r.low+r.low) < -v1.low)
		r.low += v1.low;
	yq = mdiv(mult(r,r), v1, &yr);
	temp = mult(f2.c, d1);
	temp = madd(temp, mult(r,f2.b));
	y = madd(temp,mult(v2,yr));
	temp = mdiv(y, v1, &temp2);
	if(mcmp(temp2,zero) != 0) tobort(10);
	f3.a = mult(v1, v2);
	f3.b = madd(f2.b, smult(mult(v2,r),2));
	f3.c = madd(temp, mult(yq,v2));
	reduce(&f3);
	return(f3);;
}
void
reduce(f)
form *f;
{
	mint babs;
	mint zero, one;
	mint mtemp, k;
	mint ka;
	double dt1, dt2, dtemp;

	zero = itom(0);
	one = itom(1);
back:
	if(mcmp(f->a,f->c) > 0){
		mtemp = f->a;
		f->a = f->c;
		f->c = mtemp;
		f->b = mchs(f->b);
	}
	babs = f->b;
	if(mcmp(f->b,zero) < 0)
		babs = mchs(babs);
	if(mcmp(f->a,babs) >= 0){
		if((mcmp(f->a,f->c)==0) || (mcmp(f->a,babs)==0)){
			f->b = babs;
		}
		return;
	}
	dt1 = f->b.high*1e16 + f->b.low;
	dt2 = f->a.high*1e16 + f->a.low;
	modf(dt1/dt2, &dtemp);
	if(dtemp>0) dtemp += 1;
	else if(dtemp<0) dtemp -= 1;
	modf(dtemp/2, &dtemp);
	modf(dtemp/1e16, &k.high);
	k.low = dtemp - 1e16*k.high;
	ka = mult(k,f->a);
	f->b = msub(f->b,madd(ka,ka));
	f->c = msub(f->c,mult(k,madd(ka,f->b)));
	goto back;
}
tobort(n)
int n;
{
	printf("Error %d\n", n);
	abort();
}

form
formpow(f,h)
form f;
double h;
{
	form ftemp;
	int i,n;
	int bit[120];
	double junk;
	double temph;
	double modf();

	if(h == 0.){
		ftemp = f;
		ftemp.b = mchs(ftemp.b);
		ftemp = compos(f, ftemp);
		return(ftemp);
	}

	i = 0;
	temph = h;
	while(temph != 0){
		modf(temph/2, &junk);
		if(2.*junk != temph)
			bit[i++] = 1;
		else
			bit[i++] = 0;
		temph = junk;
	}
	n = i-1;

	ftemp = f;
	for(i=n; i>0; i--){
		ftemp = compos(ftemp, ftemp);
		if(bit[i-1] != 0){
			ftemp = compos(f, ftemp);
		}
	}
	return(ftemp);
}
void
formout(f)
form f;
{
	printf("(");
	mout1(f.a);
	printf(",");
	mout1(f.b);
	printf(",");
	mout1(f.c);
	printf(")");
	return;
}
