#include <jerq.h>
#include <msgs.h>
#define INSET 3
#define BORDER 8
int rflag;
main(argc,argv)
char **argv;
{
	int width = Drect.corner.x - Drect.origin.x;
	int height = Drect.corner.y - Drect.origin.y;
	int wd, ht, nw, nh;
	Layer *old, *new;
	Rectangle drect;
	if(argv[1][0]=='-') {
		rflag++;
		argv++;
		argc--;
	}
	if(argc<3) 
		exit();
	wd = atoi(argv[1]);
	ht = atoi(argv[2]);
	if(rflag) {
		nw = muldiv(wd,height,ht);
		nh = muldiv(ht,width,wd);
		wd = nw>width? width: nw;
		ht = nh>height? height: nh;
	}
	wd += 2*INSET;
	ht += 2*INSET;
	if(wd<100 || ht<40)
		exit();
	drect = inset(Drect,-INSET);
	drect.corner = add(drect.origin,Pt(wd,ht));
	if(drect.corner.x > XMAX-BORDER) {
		drect.origin.x = XMAX-BORDER-wd;
		drect.corner.x = XMAX-BORDER;
	}
	if(drect.corner.y > YMAX-BORDER) {
		drect.origin.y = YMAX-BORDER-ht;
		 drect.corner.y = YMAX-BORDER;
	}
	if(drect.origin.x < BORDER)
		drect.origin.x = BORDER;
	if(drect.origin.y < BORDER)
		drect.origin.y = BORDER;
	if(new = newlayer(drect)) {
		old = P->layer;
		P->layer = new;
		P->rect = inset(drect,INSET);
		muxnewwind(P,C_RESHAPE);
		dellayer(old);
	}
}
