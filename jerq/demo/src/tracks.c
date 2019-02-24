
#include <jerq.h>

#define TRACKSNUM 4
#define SLEEPTIME 40

#undef	bitblt
#define bitblt(s, r, d, p, c)	(*((void(*)())0x7a24))(s,r,d,p,c)

Bitmap *test;

Bitmap **trackshape;
Point *track, *velocity, rounded;
int *side;
int tracksnum, sleeptime;

short Feet[] = {
0x1668,0x0000,
0x7E7E,0x700E,
0x7E7E,0x7E7E,
0x7C3E,0xFFFF,
0x3C3C,0x7FFE,
0x3C3C,0xFE7F,
0x3C3C,0xE007,
0x1818,0x0000,
0x1818,0x0000,
0x3C3C,0xE007,
0x3C3C,0xFE7F,
0x3C3C,0x7FFE,
0x7C3E,0xFFFF,
0x7E7E,0x7E7E,
0x7E7E,0x700E,
0x1668,0x0000,
};

short Bird[] = {
0x0810,0x0000,
0x4992,0x4002,
0x2A54,0x2004,
0x1C38,0x1008,
0x0810,0xFFFF,
0x0810,0x1008,
0x0810,0x2004,
0x0810,0x4002,
0x0810,0x4002,
0x0810,0x2004,
0x0810,0x1008,
0x0810,0xFFFF,
0x1C38,0x1008,
0x2A54,0x2004,
0x4992,0x4002,
0x0810,0x0000,
};
 
short Bear[] = {
0x0000,0x300C,
0x1818,0x366C,
0xD81B,0x0FF0,
0xC3C3,0x6FF6,
0x3BDC,0x6FF6,
0x7C3E,0x0660,
0x7C3E,0x1818,
0x381C,0x1818,
0x381C,0x1818,
0x7C3E,0x1818,
0x7C3E,0x0660,
0x3BDC,0x6FF6,
0xC3C3,0x6FF6,
0xD81B,0x0FF0,
0x1818,0x366C,
0x0000,0x300C,
};

short Horse[] = {
0x3E7C,0x0000,
0x7FFE,0x781E,
0x63C6,0xE667,
0x4182,0xC003,
0x4182,0xC003,
0x2244,0xC003,
0x2244,0xE667,
0x0000,0x781E,
0x0000,0x781E,
0x2244,0xE667,
0x2244,0xC003,
0x4182,0xC003,
0x4182,0xC003,
0x63C6,0xE667,
0x7FFE,0x781E,
0x3E7C,0x0000,
};

Bitmap feet  = {(Word *)Feet, 32/WORDSIZE,{{0,0},{32,16}}};
Bitmap bird  = {(Word *)Bird, 32/WORDSIZE,{{0,0},{32,16}}};
Bitmap bear  = {(Word *)Bear, 32/WORDSIZE,{{0,0},{32,16}}};
Bitmap horse = {(Word *)Horse,32/WORDSIZE,{{0,0},{32,16}}};

Rectangle ltrackup = {{0,0},{8,8}};
Rectangle rtrackup = {{8,0},{16,8}};
Rectangle ltrackdown = {{8,8},{16,16}};
Rectangle rtrackdown = {{0,8},{8,16}};
Rectangle ltrackleft = {{16,8},{24,16}};
Rectangle rtrackleft = {{16,0},{24,8}};
Rectangle ltrackright = {{24,0},{32,8}};
Rectangle rtrackright = {{24,8},{32,16}};

Texture grey = {
   0x1111,0x4444,0x1111,0x4444,0x1111,0x4444,0x1111,0x4444,
   0x1111,0x4444,0x1111,0x4444,0x1111,0x4444,0x1111,0x4444,
 };

Bitmap screen;

small(i)
register i;
{
	return((rand() % i) - (i >> 1));
}

PickVel(velocity)
Point *velocity;
{
  do {
    velocity->x = small(31);
    velocity->y = small(31);
  } while ((abs(velocity->x) < 9) && (abs(velocity->y) < 9));
}

ChangeVel(velocity)
Point *velocity;
{
  do {
     velocity->x += small(7);
     velocity->y += small(7);
  } while ((abs(velocity->x) < 9) && (abs(velocity->y) < 9));
}

ImpBlit(trackshape,p,v,s)
Bitmap *trackshape;
Point p,v;
int *s;
{
   Rectangle which;
   if (abs(v.x)>=abs(v.y)) {
      if (v.x>0) {
        if ((*s)==0) {
          which = rtrackright;
          p.y += 4;
        } else {
          which = ltrackright;
          p.y -= 4;
        }
      } else if ((*s)==0) {
          which = rtrackleft;
          p.y -= 4;
        } else {
          which = ltrackleft;
          p.y += 4;
        }
   } else {
     if (v.y>0) {
        if ((*s)==0) {
          which = rtrackdown;
          p.x -= 4;
        } else  {
          which = ltrackdown;
          p.x += 4;
        }
      } else if ((*s)==0) {
          which = rtrackup;
          p.x += 4;
        } else {
          which = ltrackup;
          p.x -= 4;
        }
   }
   bitblt(trackshape,which,&screen,p,F_OR);
   *s = !(*s);
}

main(argc, argv)
	char *argv[];
{
        Rectangle insetscreen;
        int i;
        srand(*XMOUSE);
/*
	dellayer(P->layer);
	P->layer=newlayer(Rect(0, 0, 0, 0));
*/
	tracksnum = 0;
        sleeptime = 0;
	while(argc>1){
		if(strcmp(argv[1], "-s")==0){
			--argc; argv++;
			for(i=0; i<atoi(argv[1]); i++)
				sleep(3600);
		}else
		if(strcmp(argv[1], "-v")==0){
			--argc; argv++;
			sleeptime = atoi(argv[1]);
		}else
			tracksnum=atoi(argv[1]);
		--argc; argv++;
	}
	if(tracksnum<=0)
		tracksnum=TRACKSNUM;
        if(sleeptime<=0)
		sleeptime=SLEEPTIME;
        sleeptime = sleeptime/tracksnum;
	track=(Point *)alloc(tracksnum*sizeof(Point));
	velocity=(Point *)alloc(tracksnum*sizeof(Point));
        side=(int *)alloc(tracksnum*sizeof(int));
        trackshape=(Bitmap **)alloc(tracksnum*sizeof(long));
	if(track==0 || velocity==0 || side==0 || trackshape==0) exit();
	screen.base = addr(&display,Pt(0,0));
	screen.width = display.width;
	screen.rect = Jrect;
        insetscreen = inset(screen.rect,9);
        test = balloc(Rect(0,0,16,4));
        rectf(test,test->rect,F_CLR);
        for (i=0; i<tracksnum; i++) {
          track[i].x = muldiv(XMAX-20,i,tracksnum)+6; track[i].y = 6;
          PickVel(&(velocity[i]));
          side[i]=0;
          if ((i%4)==0) trackshape[i] = &bear;
          if ((i%4)==1) trackshape[i] = &bird;
          if ((i%4)==2) trackshape[i] = &feet;
          if ((i%4)==3) trackshape[i] = &horse;
        }

	request(KBD);
	while (kbdchar()!='q') {
          for (i=0; i<tracksnum; i++) {
            sleep(sleeptime);
            wait(CPU);
            if (small(5)==0) ChangeVel(&(velocity[i]));
            track[i] = add(track[i],velocity[i]);
            if (!ptinrect(track[i],insetscreen)) {
              track[i] = sub(track[i],velocity[i]);
              PickVel(&velocity[i]);
            } else {
              rounded.x = (~3) & track[i].x;
              rounded.y = (~3) & track[i].y;
              texture(test,Rect(0,0,4,4),&grey,F_STORE);
              bitblt(&screen,raddp(Rect(0,0,4,4),rounded),test,Pt(0,0),F_XOR);
              if (*addr(test,Pt(0,0))||*addr(test,Pt(0,1))||
                  *addr(test,Pt(0,2))||*addr(test,Pt(0,3)))
                ImpBlit(trackshape[i],sub(track[i],Pt(4,4)),velocity[i],&(side[i]));
            }
          }
        }
}

