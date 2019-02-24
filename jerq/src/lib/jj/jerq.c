#include <jerq.h>
#include <jerqio.h>

Texture ones={
	0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
	0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

Point pt1, pt2;
Point pt[256];
int npts;

main(){
	do; while(input());
	scan();
	output();
}
input(){
	Point t;
	static int lineno=0;
	if(getline()==0)
		return 0;
	lineno++;
	if(npts==0){
		pt[npts++]=pt1;
		pt[npts++]=pt2;
		return 1;
	}
	if(eqpt(pt1, pt[npts-1])){
		pt[npts++]=pt2;
		return 1;
	}
	if(eqpt(pt2, pt[npts-1])){
		pt[npts++]=pt1;
		return 1;
	}
	if(npts!=2){
		printf("panic 1: line %d\n", lineno);
		exit();
	}
	t=pt[npts-2];
	pt[npts-2]=pt[npts-1];
	pt[npts-1]=t;
	if(eqpt(pt1, pt[npts-1])){
		pt[npts++]=pt2;
		return 1;
	}
	if(eqpt(pt2, pt[npts-1])){
		pt[npts++]=pt1;
		return 1;
	}
	printf("panic 2: line %d\n", lineno);
	exit();
	return 0;
}
char *
rin(p, ip)
	register char *p;
	register int *ip;
{
	register n=0, m=0, scale=1, sign=1;
	while(*p==' ')
		p++;
	if(*p=='-')
		p++, sign=-1;
	while('0'<=*p && *p<='9')
		n=10*n+*p++-'0';
	n*=100;
	if(*p=='.'){
		p++;
		while('0'<=*p && *p<='9'){
			m=10*m+*p++-'0';
			scale*=10;
		}
		n+=100*m/scale;
	}
	*ip=sign*n;
printf("read %d.%d\n", sign*n/100, n%100);
	return p;
}
getline(){
	char buf[128];
	int x;
	register char *p;
	if(fgets(buf, sizeof buf, stdin)==0)
		return 0;
	p=buf;
	while(*p!=' ')
		p++;
	p=rin(p, &x);
	pt1.x=x;
	p=rin(p, &x);
	pt1.y=x;
	p=rin(p, &x);
	pt2.x=x;
	p=rin(p, &x);
	pt2.y=x;
	return 1;
}
scan(){
	register i;
	for(i=0; i<npts; i++){
printf("pt %d %d %d\n", i, pt[i].x, pt[i].y);
		pt[i].x/=50, pt[i].y/=50;
		pt[i].x+=Drect.origin.x;
		pt[i].y+=(Drect.origin.y+Drect.corner.y)/2;
	}
	for(i=1; i<npts; i++)
		segment(&display, pt[i-1], pt[i], F_OR);
	polyture(&display, pt, npts, &ones, F_OR);
}
output(){
	request(KBD);
	do wait(KBD); while(kbdchar()!='q');
	exit();
}
pause(){
	request(KBD);
	rectf(&display, Drect, F_XOR); sleep(20); rectf(&display, Drect, F_XOR);
	do wait(KBD); while(kbdchar()!='q');
}
