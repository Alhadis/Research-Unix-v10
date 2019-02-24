
#
/*
 * Simulate a bouncing ball
 */
#include	<jerq.h>

#undef	XMAX
#undef	YMAX
#define HEIGHT  36
#define WIDTH   28
#define RAISE   3
#define HHeight (HEIGHT/2)
#define HWidth  (WIDTH/2)
#define XMIN    (Drect.origin.x+16)
#define YMIN    (Drect.origin.y+16)
#define XMAX    (Drect.corner.x-16)
#define YMAX    (Drect.corner.y-16)
#define TOP     YMIN
#define BOT     (YMAX-1)

int     GND[128];
int     xpos;
Bitmap	*ball;

short dijkstraBits[] = {
0x00FF,0xC000,
0x07FF,0xF800,
0x1FFF,0xFE00,
0x3FFF,0xFF00,
0x7FF8,0x7F80,
0x7FE0,0x7FC0,
0xFF80,0x3FE0,
0xFE00,0xBFE0,
0xF800,0x3FE0,
0xF000,0xBFE0,
0xE000,0x1FE0,
0xE000,0x9FC0,
0xE000,0x1FC0,
0xE00F,0x9F80,
0xBFFF,0xCF80,
0x5F9F,0xFE80,
0x5F96,0x4E80,
0x5699,0x4E80,
0x5990,0x4E80,
0x5090,0x9480,
0x318F,0x3A80,
0x1F90,0x5500,
0x1908,0x2F00,
0x18FC,0x3C00,
0x0852,0x2A00,
0x096D,0x9C00,
0x0EFD,0x2E00,
0x0B83,0xD400,
0x0E28,0xAA00,
0x0755,0xFC00,
0x07AA,0xA800,
0x03FF,0xD000,
0x03EB,0xE000,
0x03FD,0x4000,
0x00AB,0x8000,
0x0175,0x0000,
};

Bitmap Dijkstra  = {(Word *)dijkstraBits, 32/WORDSIZE, {{0,0},{WIDTH,HEIGHT}}};

main()
{      
	register i;
	register time;
	int x, xx, y, yy, xvel, yvel, xacc, yacc, ypos, offset;

	ball = balloc(Rect(0, 0, WIDTH, HEIGHT));
	rectf(ball, ball->rect, F_CLR);
        bitblt(&Dijkstra,Dijkstra.rect,ball,Pt(0,0),F_XOR);
Again:
	rectf(&display,Drect,F_CLR);
	segment(&display, Pt(XMIN, YMIN), Pt(XMIN, YMAX), F_XOR);
	segment(&display, Pt(XMIN, YMAX), Pt(XMAX, YMAX), F_XOR);
	segment(&display, Pt(XMAX, YMAX), Pt(XMAX, YMIN), F_XOR);

	for (i=0; i<128; i++) {
		x = i*HWidth;
		if ((x <= XMIN) || (x >= XMAX-WIDTH))
			GND[i] = 0;
		else {
			GND[i] = BOT - 16;
			if (i&01)
				if (GND[i-1]==0)
					GND[i] = 0;
				else
					GND[i] -= RAISE;
		}
	}
	request(KBD);
	while (kbdchar() != 'q') {
		xpos = rand() & 0177;
		if (GND[xpos] <= TOP) {
			sleep(1);
			if (GND[xpos] <= 0)
				continue;
			else
				goto Again;
		}
		x = xpos*HWidth;
		xvel = 4 - ((rand()|01)&07);
		yacc = 1;
		yvel = 0;
		y = TOP;
		for (time = 0;; time++) {
			if (kbdchar()=='q')
				return;
			drawit(x,y);
			xx = x; 
			yy = y;         /* save x and y */
			yvel += yacc;
			y += yvel;
			x += xvel;
			if (y > GND[x/HWidth]) {	/* bounce? */
				if (yvel>5) {
					drawit(xx, yy);
					drawit(x, y);
					sleep(1);
					drawit(x, y);
					drawit(xx, yy);
				}
				if (y <= GND[xx/HWidth]) { /* side collision? */
					x = xx;
					xvel = -xvel;
				} else if (yy <= GND[x/HWidth]) { /*bottom? */
					y = yy;
					yvel = -yvel;
				} else {	/* corner */
					x = xx;
					y = yy;
					xvel = -xvel;
					yvel = -yvel;
				}
				if ((time & 017)==0)
					xvel = decay(xvel);
				if (xvel == 0) {
					xpos = x/HWidth;
					if (GND[xpos-1]<GND[xpos]
					 && GND[xpos]<GND[xpos+1])
						xvel = 1;
					/* roll left */
					else if (GND[xpos-1]>GND[xpos]
					      && GND[xpos]>GND[xpos+1])
						xvel = -1;
					/* on hilltop */
					else if (GND[xpos-1]>GND[xpos]
					      && GND[xpos]<GND[xpos+1]) {
						if (rand() & 01)
							xvel = 1;
						else
							xvel = -1;
					}
				}
				yvel = decay(yvel);
			}
			sleep(1);
			if (P->state & RESHAPED) {
				P->state &= ~RESHAPED;
				goto Again;
			}
			drawit(xx,yy);
			if (xvel==0 && yvel==0 && y > GND[x/HWidth]-4) {
				drawit(x,GND[xpos]);
				while ((x>XMIN) && (x!=(xpos*HWidth))) {
					sleep(1);
					drawit(x,GND[xpos]);
					x--;
					drawit(x,GND[xpos]);
				}
				GND[xpos] -= HEIGHT;
				if (GND[xpos+1]>0) GND[xpos+1] = GND[xpos]+RAISE;
				if (GND[xpos-1]>0) GND[xpos-1] = GND[xpos]+RAISE;
				break;
			}
		}
	}
}

drawit(x,y)
int x,y;
{       
	int i;

	if (x<XMIN)
		x = XMIN;
	if (y<TOP)
		y = TOP+HHeight;
	if (x>XMAX)
		x = XMAX-WIDTH;
	xpos = x/HWidth;
	if (y>GND[xpos])
		y = GND[xpos];
	bitblt(ball, ball->rect, &display, Pt(x, y-HHeight), F_XOR);
}

decay(v)
register v;
{
	if (v==0)
		return(v);
	if (v > 0)
		return(v-1-(v/8));
	return(v+1-(v/8));
}
