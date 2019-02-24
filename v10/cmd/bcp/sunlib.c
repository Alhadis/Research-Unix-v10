/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     T. Thompson - ATT-BL HO - first versions        */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <pixrect/pixrect.h>
#include <pixrect/pixfont.h>
#include <suntool/sunview.h>
#include <suntool/canvas.h>
#include "met.h"
#include "boole.h"
#include "Coord.h"

#define XYSCALE(x,y) (x=(x-obotx)*scx+botx),(y=(y-oboty)*scy+boty)

struct pixrect *source_pixrect;

Frame Mainframe;
Canvas Win;
int Winfd;
Pixwin *Pw;
int Key = -1;
int Mousex;
int Mousey;
int Colorval;

/* CMAPSIZE MUST BE A POWER OF 2 (restriction in sun colormap stuff)*/
#define CMAPSIZE 8

#define DEFAULTCOLOR (CMAPSIZE-1)

#define S_WHITE 0
#define S_RED 1
#define S_GREEN 2
#define S_BLUE 3
#define S_GREY 4
#define S_AQUA 5
#define S_YELLOW 6
#define S_BLACK 7

int mycolors[CMAPSIZE][3] = {
	255, 255, 255,	/* white */
	230, 0, 75,	/* red */
	0, 200, 0,	/* green */
	0, 0, 200,	/* blue */
	200, 200, 200,	/* grey */
	0, 160, 170,	/* aqua */
	250, 220, 0,	/* yellow */
	0,0,0		/* foreground (black) */
};


void domouse();

extern int Debug;

extern event_proc();

static int
	boty = 32767,		/* screen bottom y */
	botx = 0,			/* screen bottom x */
	topx = 32767,		/* screen top x */
	topy = 0,			/* screen top y */
	oboty = 0,			/* user's bottom y */
	obotx = 0,			/* user's bottom x */
	otopy = 32767,		/* user's top y */
	otopx = 32767;		/* user's top x */

static double
	scx = 1.0,			/* scale factor x */
	scy = 1.0			/* scale factor y */
;
static int lastx = 0;
static int lasty = 0;
static int lmode = 3;		/* drawing mode (default is xor)*/

int Prevlmode = 3;

#define signof(x) ((x)>=0.0?(1):(-1))
#define absof(x) ((x)>=0.0?(x):(-x))

void flushout() { }

short bsy_pixrect_data[] = {
/* Format_version=1, Width=16, Height=16, Depth=1, Valid_bits_per_item=16
 */
	0x7FFE,0x4002,0x200C,0x1A38,0x0FF0,0x07E0,0x03C0,0x0180,
	0x0180,0x0240,0x0520,0x0810,0x1108,0x23C4,0x47E2,0x7FFE
};
mpr_static(busy_pixrect, 16, 16, 1, bsy_pixrect_data);

int childpid;

Rect sweeparect();

om_open()
{
	Rect r;

	r = sweeparect();

	notify_errno = NOTIFY_OK;

	Mainframe = window_create((Window)NULL, FRAME,
		FRAME_LABEL, "ocr",
		0 );
	Win = window_create(Mainframe, CANVAS, 0);
        Pw = canvas_pixwin(Win);

	window_set(Mainframe, WIN_X, r.r_left,
			WIN_Y, r.r_top, 0);
	window_set(Win, WIN_WIDTH, r.r_width,
			WIN_HEIGHT, r.r_height, 0);

	window_fit(Win);
	window_fit(Mainframe);

	window_set(Mainframe, WIN_SHOW, TRUE, 0);

	Winfd = (int) window_get(Win, WIN_FD);

	if ( notify_errno != NOTIFY_OK ) {
		notify_perror("ocr");
		exit(1);
	}

	setcolormap();

	set_color(DEFAULTCOLOR);
	linemod("solid");

	window_set(Win,
		WIN_CONSUME_PICK_EVENTS,
			WIN_NO_EVENTS,
			WIN_ASCII_EVENTS,
			WIN_MOUSE_BUTTONS,
			LOC_MOVE,
			/* LOC_DRAG, */
			LOC_WINEXIT, LOC_WINENTER,
			LOC_RGNEXIT, LOC_RGNENTER, 0,
		0);

	window_set(Win, WIN_EVENT_PROC, event_proc, 0);

	space(0,0,M_WID,M_HGT);
	interact();
}

/***********************************************************/
/* The code below was taken from the 'dumpregion' program. */
/***********************************************************/

/*
 * Copyright Richard Tobin 1987.  You may freely copy, modify and distribute
 * this program in source form provided this comment remains intact.
 *
 * Richard Tobin,                    JANET: R.Tobin@uk.ac.ed             
 * AI Applications Institute,        ARPA:  R.Tobin%uk.ac.ed@nss.cs.ucl.ac.uk
 * Edinburgh University.             UUCP:  ...!ukc!ed.ac.uk!R.Tobin
 */

#include <sys/file.h>
#include <suntool/fullscreen.h>

short nwdata[] = {
#include </usr/include/images/stretchNW.cursor>
};
mpr_static(nwpixrect, 16, 16, 1, nwdata);

short sedata[] = {
#include </usr/include/images/stretchSE.cursor>
};
mpr_static(sepixrect, 16, 16, 1, sedata);

Rect rect = {0,0,0,0};

Rect
sweeparect()
{
    int wfd, n;
    struct fullscreen *fs;
    Event event;
    Pixwin *pw;
    int left, right, top, bottom, temp;
    Window win;
    double w;
    Rect r;

    win = window_create(0, FRAME,
			FRAME_OPEN_RECT, &rect,
			WIN_CONSUME_PICK_EVENTS,
			WIN_ASCII_EVENTS,
			 LOC_MOVE, WIN_MOUSE_BUTTONS, 0,
			0);

    wfd = (int)window_get(win, WIN_FD);
    fs = fullscreen_init(wfd);
    pw = fs->fs_pixwin;

    window_set(win,
	       WIN_CURSOR, cursor_create(CURSOR_IMAGE, &nwpixrect,
					 CURSOR_XHOT, 0, CURSOR_YHOT, 0,
					 0),
	       0);

    for ( ;; ) {
	n = window_read_event(win, &event);
	if ( n < 0 )
		continue;
	if ( event_is_ascii(&event) ) {
		if ( event_id(&event) == 'F' ) {
			r.r_left = 10;
			r.r_top = 10;
			r.r_width = 900;
			r.r_height = 900;
		}
		else {
			r.r_left = 542;
			r.r_top = 0;
			r.r_width = 540;
			r.r_height = 512;
		}
		goto getout;
	}
	if ( event_is_down(&event) && event_is_button(&event) )
		break;
    }

    right = left = event_x(&event);
    bottom = top = event_y(&event);

    window_set(win,
	       WIN_CURSOR, cursor_create(CURSOR_IMAGE, &sepixrect,
					 CURSOR_XHOT, 15, CURSOR_YHOT, 15,
					 0),
	       0);

    drawbox(pw, left, top, right, bottom);

    while(window_read_event(win, &event) == -1 ||
	  !event_is_up(&event) ||
	  !event_is_button(&event))
    {
	drawbox(pw, left, top, right, bottom);
	right = event_x(&event);
	bottom = event_y(&event);
	drawbox(pw, left, top, right, bottom);
    }

    drawbox(pw, left, top, right, bottom);

    if(right < left)
    {
	temp = right;
	right = left;
	left = temp;
    }
    if(bottom < top)
    {
	temp = bottom;
	bottom = top;
	top = temp;
    }
    r.r_left = left;
    r.r_top = top;
    r.r_width = (right-left);
    r.r_height = (bottom-top);
getout:
    fullscreen_destroy(fs);

    window_set(win, FRAME_NO_CONFIRM, TRUE, 0);
    window_destroy(win);
    return r;
}

drawbox(pw, left, top, right, bottom)
Pixwin *pw;
int left, top, right, bottom;
{
    fullscreen_pw_vector(pw, left, top, right, top, PIX_NOT(PIX_DST), 0);
    fullscreen_pw_vector(pw, right, top, right, bottom, PIX_NOT(PIX_DST), 0);
    fullscreen_pw_vector(pw, right, bottom, left, bottom, PIX_NOT(PIX_DST), 0);
    fullscreen_pw_vector(pw, left, bottom, left, top, PIX_NOT(PIX_DST), 0);
}

ttygets(buff,len)
char *buff;
{
	int x, y, n, key;
	char *p = buff;

	for ( n=0; n<(len-1); n++ ) {
		while ( (key=mouseorkey(&x,&y)) < 0 )
			;
		if ( key == '\r' )	/* just in case */
			key = '\n';
		*p++ = key;
		if ( key == '\n' || key == '\0' )
			break;
	}
	*p++ = '\0';
}

int mouseorkey(x,y)
int* x;
int* y;
{
	Event anevent;
	char c;
	int n;

	Key = -1;

	notify_do_dispatch();
	n = read(0,&c,1);
	notify_no_dispatch();
	if ( n == 1 ) {
		/* character was typed in text (original) window */
		/* ie. stdin */
		Key = c;
		*x = Mousex;
		*y = Mousey;
		xyunscale(x,y);
	}
	else if ( Key >= 0 ) {
		/* character was typed in graphics window */
		/* so it must be echoed. */
		putchar(Key);
		if ( Key == '\r' )
			putchar('\n');
		fflush(stdout);
		*x = Mousex;
		*y = Mousey;
		xyunscale(x,y);
	}
	return(Key);
}

interact()
{
	(void) notify_dispatch();
}

setcolormap()
{
	u_char red[CMAPSIZE], green[CMAPSIZE], blue[CMAPSIZE];
	int n;

	for(n=0;n<CMAPSIZE;n++) {
		red[n] = mycolors[n][0];
		green[n] = mycolors[n][1];
		blue[n] = mycolors[n][2];
	}
        pw_setcmsname(Pw,"showcolor");
        pw_putcolormap(Pw, 0, CMAPSIZE, red, green, blue);
}

/*ARGSUSED*/
wait_proc(window,event,arg)
Window window;
Event *event;
caddr_t arg;
{
	int id = event_id(event);

	if ( event_is_ascii(event) && id == 'q' )
		notify_stop(0);
}

om_close()
{
	window_set(Win, WIN_EVENT_PROC, wait_proc, 0);
	if ( fork() == 0 ) {
		close(0);
		close(1);
		close(2);
		/* This stuff doesn't seem to work.  I'm trying to */
		/* get the window to be able to repaint itself when */
		/* uncovered.  */
		sleep(1);
		window_main_loop(Mainframe);
		exit(0);
	}
}

space(x0,y0,x1,y1)
int x0, y0, x1, y1;
{
	int tx, ty;

	obotx = x0;
	oboty = y0;
	otopx = x1;
	otopy = y1;

        boty = 0;
        botx = 0;
        tx = (int) window_get(Win,WIN_WIDTH);
        ty = (int) window_get(Win,WIN_HEIGHT); 
        topx = tx;
        topy = ty;
        scx = (double)(topx-botx)/(otopx-obotx);
        scy = (double)(topy-boty)/(otopy-oboty);
}

#ifdef OLDSTUFF
xyscale(x, y)
int *x, *y;
{
        *x = (*x-obotx)*scx+botx;
        *y = (*y-oboty)*scy+boty;
}
#endif

xyunscale(x, y)
int *x, *y;
{
	/* map from device coordinates to space() coordinates */

	*x = ( (*x - botx)/scx + obotx );
	*y = ( (*y - boty)/scy + oboty );
}

wline(x1,y1,x2,y2)
int x1, y1, x2, y2;
{
	lastx = x2;
	lasty = y2;

	XYSCALE(x1, y1);
	XYSCALE(x2, y2);

	arawline(x1,y1,x2,y2);
}

wrect(x1,y1,x2,y2)
int x1, y1, x2, y2;
{
	lastx = x2;
	lasty = y2;

	XYSCALE(x1, y1);
	XYSCALE(x2, y2);

	arawbox(x1,y1,x2,y2);
}

arawbox(x1,y1,x2,y2)
int x1, y1, x2, y2;
{
	arawline(x1,y1,x2,y1);
	arawline(x2,y1,x2,y2);
	arawline(x2,y2,x1,y2);
	arawline(x1,y2,x1,y1);
}

arawline(x1,y1,x2,y2)
int x1, y1, x2, y2;
{
	int op, col;

	currentopcol(&op,&col);
/* fprintf(stderr,"calling pw_vector, %d,%d  %d,%d  currentop=%d  Colorval=%d  Pw=%ld\n",x1,y1,x2,y2,op,col,Pw); */
	pw_vector(Pw, (int)x1,(int)y1, (int)x2,(int)y2, op, col);
	interact();
}

currentopcol(aop,acol)
int *aop, *acol;
{
	if ( lmode == 1 && Colorval == S_WHITE ) {
		*aop = (PIX_NOT(PIX_SRC) & PIX_DST);
		*acol = S_BLACK;
	}
	else {
		*acol = Colorval;
		switch (lmode) {
		case 0:
			*aop = (PIX_NOT(PIX_SRC) & PIX_DST);
			break;
		case 1:
		case 2:
			/* *aop = (PIX_SRC | PIX_DST); */
			*aop = PIX_SRC;
			break;
		case 3:
			*aop = (PIX_SRC ^ PIX_DST);
			break;
		default:
			*aop = (PIX_SRC | PIX_DST);
			break;
		}
	}
}

wt(s,scl,left,top,right,bot)
char *s;
{
	int op, col;
/* fprintf(stderr,"wt() called, s=%s  lastxy=%d,%d  scl=%d  left-top=%d,%d\n",
s,left,top,scl,left,top); */
	currentopcol(&op,&col);
	XYSCALE(left, top);
	pw_ttext(Pw,(int)left,(int)top,op,NULL,s);
}

Sp *
set_font(n)
{
	static Sp chsz;
	struct pr_size prs;
	int x, y;

	prs = pf_textwidth(1,pf_default(),"x");
	x = prs.x;
	y = prs.y;
	xyunscale(&x,&y);
	chsz.x = ++x;
	chsz.y = y;
	return &chsz;
}

#ifdef OLDSTUFF
textwidth(s)
{
	struct pr_size prs;

	prs = pf_textwidth(bound,strlen(s),pf_default(),s);

	
	prs = pf_textwidth(1,pf_default(),"x");
	*ax = prs.x;
	*ay = prs.y;
	xyunscale(ax,ay);
}
#endif

amove(x,y)
int x,y;
{
	lastx = x;
	lasty = y;
}

cont(x,y)
int x,y;
{
	wline(lastx,lasty,x,y);
	/* last[xy] are then updated by line */
}


set_color(n)
int n;
{
	switch(n){
	case M_WHITE:
		n = S_WHITE; break;
	case M_PALE_GREY:
	case M_GREY_7:
		n = S_GREY; break;
	case M_BLACK:
		n = S_BLACK; break;
	case M_YELLOW_13:
	case M_YELLOW_11:
		n = S_YELLOW; break;
	case M_MAGENTA:
	case M_CYAN:
	case M_CYAN_14:
		n = S_AQUA; break;
	case M_GREEN:
	case M_GREEN_7:
		n = S_GREEN; break;
	case M_BLUE:
		n = S_BLUE; break;
	case M_RED:
		n = S_RED; break;
	default:
		fprintf(stderr,"Unknown color in set_color (%d)\n",n);
		n = S_WHITE;
		break;
	}
	Colorval = n;
/* fprintf(stderr,"set_color = (sun#) %d\n",n); */
}

linemod(s)
char *s;
{
	if ( strcmp(s,"erase")==0 )
		lmode = 0;
	else if ( strcmp(s,"dotted")==0 || strcmp(s,"hilight")==0 )
		lmode = 2;
	else if ( strcmp(s,"xor")==0 )
		lmode = 3;
	else { /* everything, including "solid" defaults to this */
		lmode = 1;
	}
}

aerase()
{
	pw_writebackground(Pw,0,0,(int)topx,(int)topy,PIX_SRC);
}

wfrect(x1,y1,x2,y2)
int x1, y1, x2, y2;
{
	int op, col;
	int t;

	XYSCALE(x1, y1);
	XYSCALE(x2, y2);

	currentopcol(&op,&col);

	if ( x1 > x2 ) {
		t = x1;
		x1 = x2;
		x2 = t;
	}
	if ( y1 > y2 ) {
		t = y1;
		y1 = y2;
		y2 = t;
	}
/* fprintf(stderr,"wfrect(%d,%d to %d,%d  lmode=%d op=%d col=%d)\n",
x1,y1,x2,y2,lmode,op,col); */
	for ( t=y1; t<=y2; t++ ) {
		pw_vector(Pw, x1,t, x2,t, op, col);
	}
	interact();
}

/*ARGSUSED*/
event_proc(window,event,arg)
Window window;
Event *event;
caddr_t arg;
{
	int id = event_id(event);

	if ( event_is_ascii(event) ) {
		Key = id;
		notify_stop(0);
		return;
	}
}

/* trim a Metheus X coordinate to fit on screen */
int M_trimX(x)
	int x;
{	if(x<0) return(0);
	else if(x>(topx-1)) return((topx-1));
	return(x);
	}

/* trim a Metheus Y coordinate to fit on screen */
int M_trimY(y)
	int y;
{	if(y<0) return(0);
	else if(y>(topy-1)) return((topy-1));
	return(y);
	}

/* show Sp (Metheus coordinates) on Metheus,
   enlarged to a `dot'-sided square */
sh_Mp(mpp,dot)
	Sp *mpp;
	int dot;
{	int half_dot;
	if(dot==1)
	   wfrect(mpp->x,mpp->y,mpp->x,mpp->y);
	else {
		half_dot=dot/2;
		if((dot%2)==0)
	   		wfrect( M_trimX(mpp->x-half_dot+1),
				M_trimY(mpp->y-half_dot+1),
				M_trimX(mpp->x+half_dot),
				M_trimY(mpp->y+half_dot) );
		else	wfrect( M_trimX(mpp->x-half_dot),
				M_trimY(mpp->y-half_dot),
				M_trimX(mpp->x+half_dot),
				M_trimY(mpp->y+half_dot) );
		};
	}


om_force(){}

/* write "thick" line, two pixels wide */
wtline(x1,y1,x2,y2)
{   int absdx, absdy;
	wline(x1,y1,x2,y2);
	absdx = (x1-x2)>=0 ? x1-x2 : x2-x1;
	absdy = (y1-y2)>=0 ? y1-y2 : y2-y1;
	if(absdx>absdy)
		/* more horizontal -- thicken vertically */
		wline(x1,M_trimY(y1+1),x2,M_trimY(y2+1));
	else
		/* more vertical -- thicken horizontally */
		wline(M_trimX(x1+1),y1,M_trimX(x2+1),y2);
}

/* write "fat" line, three pixels wide */
wfline(x1,y1,x2,y2)
{	register int absdx, absdy;

	wline(x1,y1,x2,y2);
	absdx = (x1-x2)>=0 ? x1-x2 : x2-x1;
	absdy = (y1-y2)>=0 ? y1-y2 : y2-y1;
	if(absdx>absdy) {
		/* more horizontal -- thicken vertically */
		wline(x1,M_trimY(y1+1),x2,M_trimY(y2+1));
		wline(x1,M_trimY(y1-1),x2,M_trimY(y2-1));
		}
	else	{
		/* more vertical -- thicken horizontally */
		wline(M_trimX(x1+1),y1,M_trimX(x2+1),y2);
		wline(M_trimX(x1-1),y1,M_trimX(x2-1),y2);
		};
}


om_wrpix()
{
        printf("om_wrpix called\n");
}

om_buffer(n) { }

rreset() { }
