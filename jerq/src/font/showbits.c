/*
	display some bytes as a bitmap

	usage:	jx showbits < bytes
	kbd input: width of the bitmap in words as n<cr>, terminate by q<cr>
*/

#include <jerq.h>
#include <jerqio.h>
#include <font.h>
#define	SIZE	4000
char	f[SIZE];
int nbytes;
main(){
	register char *f, *p;
	register i,c;
	char buf[64];
	for(i=0,p=f; i<SIZE && (c=getchar())!=EOF; i++)
		*p++ = c;
	nbytes=i;
	sprintf(buf, "%d bytes", nbytes);
	string(&defont, buf, &display, add(Drect.origin, Pt(5, 5)), F_XOR);
	request(KBD);
	for(;;)
		show(rin());
}
rin(){
	register n=0,c;
	string(&defont, "where?", &display, add(Drect.origin, Pt(5, 22)), F_XOR);
	for(;;){
		wait(KBD);
		c=kbdchar();
		if(c=='\r' || c=='\n')
			break;
		if(c=='q')
			exit();
		n=n*10+c-'0';
	}
	if(n==0)
		exit();
	cursinhibit();
	rectf(&display, Drect, F_CLR);
	cursallow();
	return n*2;
}
show(width){
	register i, j;
	register char *p, *q=f;
	register n=0;
	while(n<nbytes){
		if(n%width==0)
			p=(char *)addr(&display, add(Drect.origin,Pt(16, 40+n/width)));
		*p++= *q++;
		n++;
	}
}
