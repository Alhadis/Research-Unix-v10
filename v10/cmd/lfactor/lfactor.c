#include <stdio.h>
#include "mp.h"
#include "form.h"
#define testing 0
#define fmax 20
#define gridmax 30000
form stockf[fmax];
struct grid { short a; float b;};
union{
	struct grid grida[gridmax+1];
	form powers[512];
}ugly;
FILE *ptab;

mint factab[100];
int facptr;
double flimit;

main(argc, argp)
int argc;
char *argp[];
{
	form formt1;
	form formtt;
	form bigf, bigfi;
	form smallf;
	form formttt;
	form regen;
	unsigned giant;
	struct grid *j1, *j2;
	int ordind;
	int prime;
	mint nn;
	mint nugget;
	mint mjunk, mtemp;
	mint mtemp1;
	mint zero, one, two, four;
	double a, b;
	mint ma, mb, mc;
	double temp;
	mint det;
	double lastpr;
	double m,n;
	double prod;
	double hbar, h;
	double pi = 3.141592653589793;
	double ht, hr, it;
	double logord;
	double maxord;
	double order[fmax];
	double tt, ttt;
	float ftt, fttt;
	int baby;
	int kbaby;
	register j;
	int i;
	double k;
	int goti, gotj, gotit;
	int ttn;
	int junk;
	int uneq;
	int jmax;
	int fcount;
	int parity;
	double sqroot, fifth;
	int verbose = 0;
	int nofact = 0;
	int compare();
	int compar1();
	double getpr();
	double modf();
	double log(), exp(), sqrt();

	setbuf(stdout,NULL);
	verbose = 0;
	if(argc > 1){
		if(*argp[1] == '+'){
			verbose = 1;
		}
		if(*argp[1] == '-'){
			nofact = 1;
		}
	}
	zero = itom(0);
	one = itom(1);
	two = itom(2);
	four = itom(4);
	facptr = 0;

	if(min(&nn) == EOF) exit(0);
	if(mcmp(nn,zero) <= 0) exit(0);
	while(1){
		mtemp = sdiv(nn,2,&i);
		if(i==0){
			printf("Prime factor: 2\n");
			nn = mtemp;
		}else break;
	}

	ptab = fopen("/usr/lib/ptab","r");
	if(ptab == NULL){
		printf("ptab?\n");
		exit(1);
	}
retry:
	getpr(-1);
	lastpr = 2.;
	if(mcmp(nn,itom(1))==0)
		exit(0);
	sqroot = sqrt(nn.high*1e16 + nn.low);
	fifth = exp(0.2*log(nn.high*1e16+nn.low));
	if(fifth < 1000.) fifth = 1000.;
	sdiv(nn,4, &i);
	if((i== 1) || (i== 2)){
		parity = 0;
		prod = 1.;
	}
	if(i== 3)
		parity = 1;
	sdiv(nn, 8, &junk);
	if(junk == 3) prod = 2./3.;
	if(junk == 7) prod = 2.;
	fcount = 0;
	while(1){
		if((m = getpr(0)) < 0)
			break;
		lastpr = m;
		if(lastpr > sqroot){
			printf("Prime factor: %.0f\n", nn.low);
			exit(0);
		}
		if(lastpr > 10.*fifth)
			break;
		modf(nn.high/m, &temp);
		n = nn.high - m*temp;
		modf(e16/m, &temp);
		temp = e16 - m*temp;
		n = n*temp;
		modf(nn.low/m, &temp);
		temp = nn.low - m*temp;
		n = n + temp;
		j = jacobi(-n,m);
		if((j==0) && (nofact==0)){
			printf("Prime factor: %.0f\n", m);
			mtemp.high = 0;
			mtemp.low = m;
			nn = mdiv(nn, mtemp, &mtemp);
			if(mcmp(mtemp,zero)!=0) abort();
			goto retry;
		}
		a = m;
		if((j==1) && (fcount<fmax)){
			det = mchs(nn);
			sdiv(det, 4, &i);
			if((i== -1) || (i== -2))
				det = smult(det, 4);
			for(b=parity; b<=a; b = b+2){
				ma.high = 0.;
				ma.low = a;
				mb.high = 0.;
				mb.low = b;
				mc.high = 0.;
				mc.low = b*b;
				mc = msub(mc, det);
				mtemp.high = 0.;
				mtemp.low = 4.*a;
				mc = mdiv(mc, mtemp, &mtemp);
				if(mcmp(mtemp,zero) == 0){
					stockf[fcount].a = ma;
					stockf[fcount].b = mb;
					stockf[fcount].c = mc;
#if testing
					if(verbose){
						formout(stockf[fcount]);
						printf("\n");
					}
#endif
					fcount++;
				}
			}
		}
		prod *= m/(m-j);
	}
	printf("Trying to factor: n = ");
	mout(nn);
	printf("Factor limit = %.0f\n", a);
	flimit = a;
	mtemp1 = mcbrt(nn, &mjunk);
	if(mjunk.low == 0){
		mout1(nn);
		printf(" is a cube.\n");
		exit(0);
	}
	mtemp1 = msqrt(nn, &mjunk);
	if(mjunk.low == 0){
		mout1(nn);
		printf(" is a square.\n");
		exit(0);
	}

	prime = pseudo(nn, itom(2));
	if(prime == 0) prime = pseudo(nn, itom(3));
	if(prime == 0) prime = pseudo(nn, itom(5));
	if(prime == 0) prime = pseudo(nn, itom(7));
	if(prime == 0){
		printf("n is pseudoprime (mod 2,3,5,7).\n");
	}else{
		printf("n is composite.\n");
	}

	if(parity==0) prod *= 2.;
	modf(prod*mtemp1.low/pi, &hbar);
#if testing
	printf("h~~ %.0f\n", hbar);
#endif

	ugly.grida[0].a = 0;
	ugly.grida[0].b = 1.;
	if(parity == 1){
		baby = 1;
		kbaby = 0;
	}else{
		baby = 2;
		kbaby = 0;
	}

	sdiv(nn, 8, &i);
	if(i == 1){
		baby = 4;
		kbaby = 0;
	}else if((i == 5) && (prime != 0)){
		baby = 4;
		kbaby = 0;
	}
	if((parity == 1) && (prime != 0)){
		baby = 2;
		kbaby = 0;
	}

	modf(hbar/baby, &temp);
	hbar = baby*temp;
	hbar = hbar + kbaby;

	smallf = formpow(stockf[0], (double)baby);
	ugly.grida[1].a = 1;
	ugly.grida[1].b = smallf.a.low;
	formt1 = formpow(stockf[0], hbar);

	if(mcmp(formt1.a,one) == 0){
		h = hbar;
		goto found;
	}
	formtt = smallf;
	ttn = 1;
	temp = sqrt(0.1*hbar/(sqrt(fifth)*baby));
	if(temp > gridmax) temp = gridmax;
	giant = temp;
	printf("Grid = %d by %d\n", giant, baby);
	while(ttn <= giant){
		if(formtt.a.low == formt1.a.low){
			if(formtt.b.low == formt1.b.low){
				h = hbar - ttn*baby;
				goto found;
			}
			if(formtt.b.low == -formt1.b.low){
				h = hbar + ttn*baby;
				goto found;
			}
		}
		formtt = compos(formtt, smallf);
		ttn += 1;
		ugly.grida[ttn].a = ttn;
		ugly.grida[ttn].b = formtt.a.low;
	}
	qsort(&ugly.grida[0].a, giant+1, sizeof(ugly.grida[0]), compare);
	bigf = formpow(smallf, (double)(2*giant));
	bigfi = bigf;
	bigfi.b.low = -bigfi.b.low;
	formtt = compos(bigf, formt1);
	formttt = compos(bigfi, formt1);
	for(k=1; k<fifth/2.; k=k+1){
		tt = formtt.a.low;
		ttt = formttt.a.low;
		ftt = formtt.a.low;
		fttt = formttt.a.low;
		if(search(&ugly.grida[0].a, giant+1, sizeof(ugly.grida[0]), compar1, &ftt, &j1, &j2) >=0){
b1:
			j = j1->a;
			regen = formpow(smallf, (double)j);
			if(tt == regen.a.low){
				if(formtt.b.low == regen.b.low){
					h = hbar + (2.*giant*k - j)*baby;
					goto found;
				}else
				if(formtt.b.low == -regen.b.low){
					h = hbar + (2.*giant*k + j)*baby;
					goto found;
				}
			}
			if(j2 > j1+1){
				j1 = j1 + 1;
				goto b1;
			}
		}
		if(search(&ugly.grida[0].a, giant+1, sizeof(ugly.grida[0]), compar1, &fttt, &j1, &j2) >=0){
b2:
			j = j1->a;
			regen = formpow(smallf, (double)j);
			if(ttt == regen.a.low){
				if(formttt.b.low == regen.b.low){
					h = hbar - (2.*giant*k + j)*baby;
					goto found;
				}else
				if(formttt.b.low == -regen.b.low){
					h = hbar - (2.*giant*k - j)*baby;
					goto found;
				}
			}
			if(j2 > j1 + 1){
				j1 = j1 + 1;
				goto b2;
			}
		}
		formtt = compos(formtt, bigf);
		formttt = compos(formttt, bigfi);
	}
	printf("Search abandoned at %.0f.\n", 2.*giant*k*baby);
	exit(1);
found:
	printf("h = %.0f\n", h);
	temp = hbar - h;
	printf("Search succeeded at %.0f (%.3f) ", temp, 1000.*temp/h);
	printf("(%.3f)\n",flimit*temp*temp/(h*h));

	ht = 0;
	hr =h;
	while(1){
		modf(hr/2, &temp);
		if(hr == 2.*temp){
			hr = temp;
			ht += 1;
		}else{
			break;
		}
	}
/*
 * h = hr * 2^ht
 */
	if(ht == 0){
		printf("Class number is odd!\n");
		mout1(nn);
		printf(" is a prime.\n");
		exit(0);
	}
#if testing
	printf("Sylow 2-subgroup of order 2^%.0f\n", ht);
#endif
	for(i=0; i<fmax; i++){
		stockf[i] = formpow(stockf[i], hr);
	}

	for(i=0; i<fmax; i++){
		if(mcmp(stockf[i].b,zero) < 0){
			stockf[i].b = mchs(stockf[i].b);
		}
	}

	for(i=1,jmax=1; i<fmax; i++){
		for(j=0; j<jmax; j++){
			uneq = 0;
			if(mcmp(stockf[i].a,stockf[j].a) != 0) uneq = 1;
			if(mcmp(stockf[i].b,stockf[j].b) != 0) uneq = 1;
			if(mcmp(stockf[i].c,stockf[j].c) != 0) uneq = 1;
			if(uneq == 0) break;
		}
		if(uneq == 1){
			stockf[jmax++] = stockf[i];
		}
	}

	for(i=0; i<jmax; i++){
		it = 0;
		formt1 = stockf[i];
		if(mcmp(formt1.a,one) == 0) continue;
		while(1){
			formtt = formpow(formt1, (double)2);
			it = it+1;
			if(it>ht){
				printf("main: error no. 1\n");
				abort();
			}
			if(mcmp(formtt.a,one) == 0) break;
			formt1 = formtt;
		}
		order[i] = it;
/*
		printf("f%3d: ", i);
		formout(stockf[i]);
		printf(" order = 2^%.0f: ", it);
		formout(formt1);
		printf("\n");
*/
	}
	logord = 0;
	ordind = 0;
	for(i=0; i<jmax; i++){
		if(order[i] > logord){
			ordind = i;
			logord = order[i];
		}
	}

	if(logord == ht){
		printf("Sylow 2-subgroup is cyclic.\n");
		formtt = stockf[ordind];
		for(i=0; i<order[ordind]-1; i++){
			formtt = formpow(formtt, (double)2);
		}
		if(mcmp(formtt.a,two) == 0){
			mout1(nn);
			printf(" is a prime.\n");
			exit(0);
		}
		if(mcmp(formtt.b,zero) == 0){
			sqtest(formtt.a, flimit);
			sqtest(formtt.c, flimit);
			exit(0);
		}
		if(mcmp(formtt.a, formtt.b) == 0){
			sqtest(formtt.a, flimit);
			sqtest(msub(mult(itom(4),formtt.c),formtt.a), flimit);
			exit(0);
		}
		if(mcmp(formtt.a, formtt.c) == 0){
			sqtest(msub(mult(itom(2),formtt.a),formtt.b), flimit);
			sqtest(madd(mult(itom(2),formtt.a),formtt.b), flimit);
			exit(0);
		}
		printf("main: error no. 4\n");
		exit(1);
	}
	if(logord > 10){
		printf("Sylow 2-subgroup too big.\n");
		exit(1);
	}

	for(i=0,j=1;i<logord;i++){
		j = j*2;
	}
	maxord = j;
	printf("\n");

	ugly.powers[0] = stockf[ordind];
	for(i=1;i<maxord/2;i++){
		ugly.powers[i] = compos(ugly.powers[i-1],stockf[ordind]);
	}

	for(i=0; i<jmax; i++){
		if(i == ordind) continue;
		gotit = 0;
		formt1 = stockf[i];
		for(j=0; j<=order[i]; j++){
			for(junk=0; junk<maxord/2; junk++){
				if(mcmp(formt1.a,ugly.powers[junk].a) != 0)
					continue;
				if(mcmp(formt1.c,ugly.powers[junk].c) != 0)
					continue;
				if(mcmp(formt1.b,ugly.powers[junk].b) != 0){
					goti = junk + 1;
				}else{
					goti = maxord - junk - 1;
				}
				for(junk=0,gotj=1; junk<j; junk++){
					gotj *= 2;
				}
				if(goti%gotj != 0){
					printf("main: error no. 2\n");
				}
				goti = goti/gotj;
				if(goti > maxord/2){
					goti = maxord - goti;
				}
				stockf[i] = compos(stockf[i],ugly.powers[goti-1]);
				gotit = 1;
				break;
			}
			if(gotit == 1) break;
			formt1 = compos(formt1,formt1);
		}
	}

	for(i=0; i<jmax; i++){
		if(mcmp(stockf[i].b,zero) < 0){
			stockf[i].b = mchs(stockf[i].b);
		}
	}

	junk = jmax;
	for(i=1,jmax=1; i<junk; i++){
		for(j=0; j<jmax; j++){
			uneq = 0;
			if(mcmp(stockf[i].a,stockf[j].a) != 0) uneq = 1;
			if(mcmp(stockf[i].b,stockf[j].b) != 0) uneq = 1;
			if(mcmp(stockf[i].c,stockf[j].c) != 0) uneq = 1;
			if(uneq == 0) break;
		}
		if(uneq == 1){
			stockf[jmax++] = stockf[i];
		}
	}

	for(i=0; i<jmax; i++){
		it = 0;
		formt1 = stockf[i];
		if(mcmp(formt1.a,one) == 0) continue;
		while(1){
			formtt = formpow(formt1, (double)2);
			it = it+1;
			if(it>ht){
				printf("main: error no. 3\n");
				abort();
			}
			if(mcmp(formtt.a,one) == 0) break;
			formt1 = formtt;
		}
		order[i] = it;
		if(verbose){
			printf("f%3d: ", i);
			formout(stockf[i]);
			printf(" order = 2^%.0f: ", it);
			formout(formt1);
			printf("\n");
		}
		if(mcmp(formt1.b,zero) == 0){
			putfact(formt1.a);
			putfact(formt1.c);
		}else if(mcmp(formt1.a,formt1.b) == 0){
			putfact(formt1.a);
			putfact(msub(mult(four,formt1.c),formt1.a));
		}else if(mcmp(formt1.a,formt1.c) == 0){
			putfact(madd(mult(two,formt1.a),formt1.b));
			putfact(msub(mult(two,formt1.a),formt1.b));
		}else{
			printf("main: error no. 5\n");
		}
	}
	nugget = nn;
	for(i=0;i<facptr;i++){
		if(mcmp(factab[i],one) == 0) continue;
		if(mcmp(factab[i],nn) >= 0) continue;
		if(mcmp(factab[i],nugget) > 0) continue;
		prtest(factab[i], flimit);
		nugget = mdiv(nugget, factab[i], &mjunk);
		if(mcmp(mjunk,zero) != 0){
			printf("main: error no. 6\n");
		}
	}
	if(mcmp(nugget,one) > 0){
		prtest(nugget, flimit);
	}
	exit(0);
}

/*
 * m must be positive and odd.
 */
int
jacobi(n,m)
double n,m;
{
	int mr2, mrm1, i, j, sign;
	double temp;
	double modf();

	sign = 1;
	if(n==0) return(0);
	if(n==1) return(1);
	if(m==1) return(1);

	mr2 = 1;
	mrm1 = 1;
	modf(m/8, &temp);
	i = m - 8*temp;
	switch(i){
	case 3:
		mrm1 = -1;
	case 5:
		mr2 = -1;
	case 1:
		break;
	case 7:
		mrm1 = -1;
		break;
	default:
		abort();
	}
	
	modf(n/m, &temp);
	n = n - m*temp;
	if(n<0) n += m;
	if(n==0) return(0);
	if(n+n>m){
		n = m-n;
		sign = sign*mrm1;
	}
	while(1){
		if(modf(.5*n, &temp) != 0) break;
		n = temp;
		sign = sign * mr2;
	}
	if(m>=32768.)
		return(sign * jacobi(mrm1*m,n));
	else{
		i = mrm1*m;
		j = n;
		return(sign * ijac(i,j));
	}
}
/*
 * m must be positive and odd.
 */
int
ijac(n,m)
int n,m;
{
	int mr2, mrm1, sign;

	sign = 1;
	if(n==0) return(0);
	if(n==1) return(1);
	if(m==1) return(1);

	mr2 = mrm1 = 1;
	switch(m&07){
	case 3:
		mrm1 = -1;
	case 5:
		mr2 = -1;
	case 1:
		break;
	case 7:
		mrm1 = -1;
		break;
	default:
		abort();
	}
	
	n = n%m;
	if(n<0) n += m;
	if(n==0) return(0);
	if((n&01)==1){
		n = m-n;
		sign = sign*mrm1;
	}
	while((n&03) == 0)
		n = n>>2;
	if((n&01) == 0){
		n = n>>1;
		sign = sign * mr2;
	}
	return(sign * ijac(mrm1*m,n));
}

int sieve[] = {
	 1,  7, 11, 13, 17, 19, 23, 29,
	31, 37, 41, 43, 47, 49, 53, 59,
	61, 67, 71, 73, 77, 79, 83, 89,
	91, 97,101,103,107,109,113,119,
};
double
getpr(arg)
int arg;
{
	static int i;
	static unsigned word;
	static double base;

	if(arg<0){
		rewind(ptab);
		word = getw(ptab);
		if(feof(ptab)) exit(0);
		if(ferror(ptab)) exit(0);
		base = 0;
		i = -2;
		return(0.);
	}
	if((base == 0) && (i < 0)){
		if(i == -2){
			i++;
			return(3);
		}
		if(i == -1){
			i++;
			return(5);
		}
	}
	while(1){
		i++;
		if(i>(8*sizeof(int))){
			word = getw(ptab);
			if(ferror(ptab)) abort();
			if(feof(ptab)) return((double)-1);
			i -= 8*sizeof(int);
			base += 30*sizeof(int);
		}
		if((word & (1<<(i-1))) != 0)
			return(base+sieve[i-1]);
	}
}

int
compare(a,b)
struct grid *a, *b;
{
	if(a->b > b->b)
		return(1);
	if(a->b < b->b)
		return(-1);
	return(0);
}
int
compar1(a,b)
float *a;
struct grid *b;
{
	if(*a > b->b)
		return(1);
	if(*a < b->b)
		return(-1);
	return(0);
}

void
sqtest(nn, flimit)
mint nn;
double flimit;
{

	mint temp1, temp2;
	double dtemp;

	temp1.low = flimit;
	temp1.high = 0;
	if(mcmp(temp1,nn) > 0){
		printf("sqtest: error no. 1\n");
		exit(1);
	}

	dtemp = log(nn.high+nn.low)/log(flimit);
	if(dtemp >= 5.0){
		printf("sqtest: error no. 2");
		exit(1);
	}

	temp1 = msqrt(nn, &temp2);
	if(temp2.low == 0){
		printf("Square factor: ");
		mout(temp1);
		return;
	}
	temp1 = mcbrt(nn, &temp2);
	if(temp2.low == 0){
		printf("Cubic factor: ");
		mout(temp1);
		return;
	}
		printf("Prime factor: ");
		mout(nn);
	return;
}
putfact(arg)
mint arg;
{
	mint mtemp;
	int i;


	while(1){
		mtemp = sdiv(arg,2,&i);
		if(i==0){
			arg = mtemp;
		}
		else break;
	}
	if(facptr == 0){
		factab[facptr++] = arg;
		return(0);
	}
	for(i=0; i<facptr; i++){
		if(mcmp(factab[i],arg) < 0) continue;
		if(mcmp(factab[i],arg) == 0) return(0);
		mtemp = factab[i];
		factab[i] = arg;
		arg = mtemp;
		continue;
	}
	factab[facptr++] = arg;
	return;

}

prtest(arg)
mint arg;
{
	double dtemp;

	dtemp = log(arg.high+arg.low)/log(flimit);
	if(dtemp < 2.0){
		printf("Prime factor: ");
	}else{
		printf("Factor: ");
	}
	mout(arg);
	return;
}
