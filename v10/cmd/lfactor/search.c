int
search(a, nn, es, fc, arg, j1, j2)
char *a, *arg, **j1, **j2;
unsigned nn;
int es;
int (*fc)();
{
	int c;
	char *lp, *ap;
	unsigned n;

	n = nn;
	lp = a;
	ap = a + n*es;
loop:
	if(n == 0){
		*j1 = lp;
		*j2 = ap;
		return(-1);
	}
	n = ap - lp;
	n = es * (n/(2*es));
	if((c=(*fc)(arg,lp+n)) == 0){
		lp = lp + n;
		ap = lp + es;
		goto gotit;
	}
	if(c<0){
		ap = lp + n;
		goto loop;
	}
	if(c>0){
		lp = lp + n;
		goto loop;
	}
gotit:
	while(1){
		if(lp==a) break;
		if((*fc)(arg,lp-es) ==0){
			lp = lp - es;
		}
		else
			break;
	}
	while(1){
		if(ap==a+nn*es) break;
		if((*fc)(arg,ap) == 0){
			ap = ap + es;
		}
		else
			break;
	}
	*j1 = lp;
	*j2 = ap;
	return(1);
}
