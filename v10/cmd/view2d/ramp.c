/*% cyntax % && cc -go # %
 */
#include <stdio.h>
#include <libc.h>
#include "3610op.h"
main(argc, argv)
char *argv[];
{
	register r0=0, g0=0, b0=0, a0=255;
	register dr, dg, db, da;
	register x0=0, y0=0, x1=1280, y1=1024;
	register r1=255, g1=255, b1=255, a1=255;
	register i, n, d;
	register vert=0;
	while(argc>1 && argv[1][0]=='-') switch(argv[1][1]){
	default:
	Usage:
		fprintf(stderr,
"Usage: ramp [-w x0 y0 x1 y1] [-v] [[r0 [g0 b0 [a0]]] r1 [g1 b1 [a1]]]\n");
		exit(1);
	case 'w':
		if(argv[1][2])
			argv[1]+=2;
		else{
			--argc;
			argv++;
		}
		if(argc<5) goto Usage;
		x0=atoi(argv[1]);
		y0=atoi(argv[2]);
		x1=atoi(argv[3]);
		y1=atoi(argv[4]);
		argc-=4;
		argv+=4;
		break;
	case 'v':
		vert++;
		--argc;
		argv++;
		break;
	}
	switch(argc){
	case 1:
		break;
	case 2:
		r1=g1=b1=atoi(argv[1]);
		break;
	case 3:
		r0=g0=b0=atoi(argv[1]);
		r1=g1=b1=atoi(argv[2]);
		break;
	case 4:
		r1=atoi(argv[1]); g1=atoi(argv[2]); b1=atoi(argv[3]);
		break;
	case 5:
		r0=atoi(argv[1]); g0=atoi(argv[2]); b0=atoi(argv[3]);
		a0=atoi(argv[4]);
		break;
	case 7:
		r0=atoi(argv[1]); g0=atoi(argv[2]); b0=atoi(argv[3]);
		r1=atoi(argv[4]); g1=atoi(argv[5]); b1=atoi(argv[6]);
		break;
	case 9:
		r0=atoi(argv[1]); g0=atoi(argv[2]); b0=atoi(argv[3]);
		a0=atoi(argv[4]);
		r1=atoi(argv[5]); g1=atoi(argv[6]); b1=atoi(argv[7]);
		a1=atoi(argv[8]);
		break;
	default:
		goto Usage;
	}
	msnare();
	if(x0>x1){ d=x0; x0=x1; x1=d; }
	if(y0>y1){ d=y0; y0=y1; y1=d; }
	dr=r1-r0;
	dg=g1-g0;
	db=b1-b0;
	da=a1-a0;
	mcmd(WRMSKL); mlong(0xffffffff);
	mcmd(WRBANK); mbyte(15);
	if(vert){
		n=y1-y0;
		d=n>1?n-1:1;
		for(i=0;i!=n;i++)
			rect(x0, i+y0, x1, i+y0+1,
				r0+dr*i/d, g0+dg*i/d, b0+db*i/d, a0+da*i/d);
	}
	else{
		n=x1-x0;
		d=n>1?n-1:1;
		for(i=0;i!=n;i++)
			rect(i+x0, y0, i+x0+1, y1,
				r0+dr*i/d, g0+dg*i/d, b0+db*i/d, a0+da*i/d);
	}
	mflush();
	exit(0);
}
rect(x0, y0, x1, y1, r, g, b, a){
	mcmd(MOVP1); mword(x0); mword(y0);
	mcmd(MOVP2); mword(x1-1); mword(y1-1);
	mcmd(SETCLL); mbyte(a); mbyte(r); mbyte(g); mbyte(b);
	mcmd(FFILL);
}
