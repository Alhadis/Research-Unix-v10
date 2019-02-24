/*%cc -go # %
 * Tom Duff, U of Toronto 1976, BTL 1984
 */
#include <stdio.h>
int blankcount;
/*
 * Simulated <jerq.h>, <font.h>
 * The jerq font format is not very portable.  Neither is this code.
 */
struct font{
	short n;
	char height, ascent;
	long unused;
}head;
struct fontchar{
	short x;
	unsigned char top, bottom;
	char left;
	unsigned char width;
	short slack;
}info[257];
int width;
int *bits;
getfont(fname)
char *fname;
{
	register font, size, i, j, v;
	char *malloc();
	if((font=open(fname, 0))<0){
		chdir("/usr/jerq/font");
		if((font=open(fname,0))<0)
			err("Font open error");
	}
	read(font, &head, sizeof head);
	head.n=((head.n&255)<<8)|((head.n>>8)&255);
	if(head.n>256)
		err("Font too big");
	read(font, info, sizeof info[0]*(head.n+1));
	for(i=0;i<=head.n;i++)
		info[i].x=((info[i].x&255)<<8)|((info[i].x>>8)&255);
	width=(info[head.n].x+31)/32;
	size=width*head.height*sizeof(int);
	if((bits=(int *)malloc(size))==NULL)
		err("Can't alloc");
	if(read(font, bits, size)!=size)
		err("Bad font");
	size/=sizeof(int);
	for(i=0;i!=size;i++){
		v=0;
		for(j=0;j!=4;j++){
			v=(v<<8)|(bits[i]&255);
			bits[i]>>=8;
		}
		bits[i]=v;
	}
	close(font);
}
main(argc, argv)
char *argv[];
{
	register i;
	register char *k;
	char buf[BUFSIZ];
	--argc;
	argv++;
	if(argc>1 && **argv=='-'){
		getfont(*argv+1);
		--argc;
		argv++;
	}
	else
		getfont("tiny");
	setbuf(stdout, buf);
	while(argc){
		for(i=0;i!=head.height;i++){
			for(k = *argv;*k;k++)
				scanchar(*k, i);
			blankcount=0;
			putchar('\n');
		}
		if(--argc!=0)
			putchar('\n');
		argv++;
	}
	exit(0);
}
scanchar(c, line){
	register char *bitp;
	register i;
	register x=info[c].x;
	register dx=info[c+1].x-x;
	register w=info[c].width;
	register l=info[c].left;
	for(i=0;i!=w;i++){
		if(i<l || dx+l<=i)
			blankcount++;
		else if(fontbit(x+i-l, line)){
			while(blankcount){
				putchar(' ');
				--blankcount;
			}
			putchar('#');
		}
		else
			blankcount++;
	}
}
fontbit(x, y){
	return(bits[y*width+x/32]&(1<<(31-(x&31))));
}
err(m){
	fprintf(stderr, "%s.\n", m);
	exit(1);
}
