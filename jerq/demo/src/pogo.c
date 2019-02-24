
#include <jerq.h>

#define LINENUM 3
#define SLEEPTIME 4

#undef	bitblt
/* #undef	texture */
#define bitblt(s, r, d, p, c)	(*((void(*)())0x7a24))(s, r, d, p, c)
/* #define texture(s, r, d, c)	(*((void(*)())0x8d84))(s, r, d, c) */

Point *dot, *velocity, rounded;
Bitmap *test;
int visible,perm,linenum,sleeptime;


Bitmap screen;
Rectangle rr,r = {100,100,200,200};
Point dp,p;

Texture grey = {
	0x1111,	0x4444,	0x1111,	0x4444,	0x1111,	0x4444,	0x1111,	0x4444,
	0x1111,	0x4444,	0x1111,	0x4444,	0x1111,	0x4444,	0x1111,	0x4444,
};

Texture black = {
	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,
	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,
};

small(i)
register i;
{
	return((rand() % i) - (i >> 1));
}

bias(r,d) {
	return r - (d/64);
}

PickVel(p1,p2,velocity)
Point p1,p2,*velocity;
{
	do {
		velocity->x = bias(small(29),p1.x-p2.x);
		velocity->y = bias(small(29),p1.y-p2.y);
	} while ((velocity->x == 0) && (velocity->y == 0));
}

ChangeVel(velocity)
Point *velocity;
{
	velocity->x += small(3);
	velocity->y += small(3);
}

drawit(where)
Point *where;
{
	rectf(&screen,Rect(where[0].x-2,where[0].y-2,where[0].x+3,where[0].y+3),F_XOR);
	rectf(&screen,Rect(where[1].x-2,where[1].y-2,where[1].x+3,where[1].y+3),F_XOR);
	if (visible) segment(&screen,where[0],where[1],F_XOR);
}

main(argc, argv)
	char *argv[];
{
	Rectangle insetscreen;
	int i,j;
	request(KBD);
	srand(*XMOUSE);
	perm = 0;
	visible = 1;
	linenum = 0;
        sleeptime = 0;
	while(argc>1){
		if(strcmp(argv[1], "-i")==0){
			visible = 0;
		}else
		if(strcmp(argv[1], "-p")==0){
			perm = 1;
		}else
		if(strcmp(argv[1], "-s")==0){
			--argc; argv++;
			for(i=0; i<atoi(argv[1]); i++)
				sleep(3600);
		}else
		if(strcmp(argv[1], "-v")==0){
			--argc; argv++;
			sleeptime = atoi(argv[1]);
		}else
			linenum=atoi(argv[1]);
		--argc; argv++;
	}
	if(linenum<=0)
		linenum=LINENUM;
        if(sleeptime<=0)
		sleeptime=SLEEPTIME;
	if (perm) {
		dellayer(P->layer);
		P->layer=newlayer(Rect(0, 0, 0, 0));
		request();
	}
  	screen.base = addr(&display,Pt(0,0));
  	screen.width = display.width;
  	screen.rect = Jrect;
	dot=(Point *)alloc(2*linenum*sizeof(Point));
	velocity=(Point *)alloc(2*linenum*sizeof(Point));
	if(dot==0 || velocity==0)
		exit();
	insetscreen = inset(screen.rect,5);
	test = balloc(Rect(0,0,16,4));
	rectf(test,test->rect,F_CLR);
	for (i=0; i<2*linenum; i++) {
		dot[i].x = muldiv(XMAX-20,i,2*linenum)+6; dot[i].y = 6;
		PickVel(dot[i],dot[i],&velocity[i]);
	}
	for (i=0; i<2*linenum; i+=2) {
		drawit(&dot[i]);
	}
	while (!(own()&KBD)) {
		sleep(sleeptime);
		for (i=0; i<2*linenum; i++) {
			if ((i&1)==0) drawit(&dot[i]);
			if (small(7)==0) ChangeVel(&(velocity[i]));
			dot[i] = add(dot[i],velocity[i]);
			if (!ptinrect(dot[i],insetscreen)) {
				dot[i] = sub(dot[i],velocity[i]);
				PickVel(dot[i],dot[1^i],&velocity[i]);
			} else {
				rounded.x = (~3) & dot[i].x;
				rounded.y = (~1) & dot[i].y;
				texture(test,Rect(0,0,4,2),&grey,F_STORE);
				bitblt(&screen,raddp(Rect(0,0,4,2),rounded),test,Pt(0,0),F_XOR);
				if (!(*addr(test,Pt(0,0))||*addr(test,Pt(0,1)))) {
					texture(&screen,raddp(Rect(0,0,4,2),rounded),&black,F_STORE);
					PickVel(dot[i],dot[1^i],&velocity[i]);
				} else {
/*
					texture(test,Rect(0,0,4,2),&grey,F_XOR);
					if ((*addr(test,Pt(0,0))&*addr(test,Pt(0,1))&0xF000)
							!=0xF000) {
						dot[i] = sub(dot[i],velocity[i]);
						PickVel(dot[i],dot[1^i],&velocity[i]);
					}
*/
				}
			}
			if (i&1) {
				drawit(&dot[i^1]);
				wait(CPU);
			}
		}
	}
	for (i=0;i<2*linenum;i+=2)
		drawit(&dot[i]);
	regrey();
}

regrey()
{
	Layer *l;
	l = P->layer;
	while (l->front)
		l = l->front;
	layerblt(l, Rect(0,0,XMAX,YMAX));
}

#define rc (r.corner)
#define ro (r.origin)
#define lc (l->rect.corner)
#define lo (l->rect.origin)

layerblt(l,r)
Layer *l;
Rectangle r;
{
	if (l)
		if (rectXrect(l->rect, r)) {
			if (ro.y < lo.y) {
				layerblt(l->back, Rpt(ro,Pt(rc.x,lo.y)));
				ro.y = lo.y;
			}
			if (rc.y > lc.y) {
				layerblt(l->back, Rpt(Pt(ro.x,lc.y),rc));
				rc.y = lc.y;
			}
			if (ro.x < lo.x) {
				layerblt(l->back, Rpt(ro,Pt(lo.x,rc.y)));
				ro.x = lo.x;
			}
			if (rc.x > lc.x) {
				layerblt(l->back, Rpt(Pt(lc.x,ro.y),rc));
				rc.x = lc.x;
			}
		} else
			layerblt(l->back, r);
	else
		texture(&screen, r, &grey, F_STORE);
}
