/*	display scheduling availability on jerq/5620 terminals
 */	
#include <jerq.h>
#include <font.h>

int	blotht = 2, blotwid = 2, halfht = 1, halfwid = 1;
int	hit;
char	*mainlist[]= {"sec/60/sample", "time dvision", "intensity",
		      "teeny", "hold line", "stop", "go", NULL};
Menu	mainmenu = { mainlist };
char	*samples[] = {"1", "2", "3", "4", "6", "10", NULL};
Menu	sammenu = { samples };
char	*pixsam[]   = {"1", "2", "3", "4", "6", "10", NULL};
Menu	pixmenu = { pixsam };
char	*htch[]   = {"1", "2", "3", "4", "6", "10", NULL};
Menu	htmenu = { htch };
int	samsize[] = {3, 1, 2, 3, 4, 6, 10};
int	pixsize[] = {2, 1, 2, 3, 4, 6, 10};
int	intensity[] = {2, 1, 2, 3, 4, 6, 10};

Texture	grey = {
	0xDDDD, 0x7777, 0xDDDD, 0x7777, 0xDDDD, 0x7777, 0xDDDD, 0x7777,
	0xDDDD, 0x7777, 0xDDDD, 0x7777, 0xDDDD, 0x7777, 0xDDDD, 0x7777
};

main()	{
long	now, nextdisp, base, disptime;
int	interval, schedcnt, actinterval, schperint, tick;
int	maxperint;
int	ht, wid; /* height and width of scope
		    in useful units */
Rectangle	scope;
int	teeny = 0;
char	*str[5], str2[30];
int	*history;
int	fadedisp, scdisp,i,j; /* scdisp is current display location on scope */

request(KBD|MOUSE);
interval = 3; /* display busyness every <interval> ticks */
history = (int *) alloc((17) * sizeof(int));

reshaped:
/* set up graphics */
scope = inset(Drect,5);
scope.origin.x += 55;
scope.corner.y -= 15;
if(teeny) scope = Drect;;
rectf( &display, Drect, F_CLR);
rectf( &display, scope, F_STORE);
scope = inset(scope,6);
ht  = (scope.corner.y - scope.origin.y) - 3*blotht/*  / blotht */;
wid = (scope.corner.x - scope.origin.x) / blotwid;

for(i=0; i< wid; i = i+60/interval) {
	texture(&display, 
		Rect(scope.origin.x+i*blotwid,   scope.origin.y,
		     scope.origin.x+i*blotwid+2, scope.corner.y),
		&grey, F_STORE);
	};
for(i=0; i<=4; i++) {
	j = i*(scope.corner.y-scope.origin.y)/4 + 1;
	texture(&display,
		Rect(scope.origin.x, scope.corner.y - j,
		     scope.corner.x, scope.corner.y - j+2),
		&grey, F_STORE);
	};
free(history);
history = (int *) alloc((wid+1) * sizeof(int));
for(i=0; i<wid; i++) {blot(i, 5, scope); history[i] = 5;};
for(fadedisp=1; fadedisp<20; fadedisp++)
	blot(fadedisp, history[fadedisp], scope);
fadedisp--;
if(! teeny) {	
	string(&defont, "  0", &display,
		Pt(scope.origin.x-40,scope.corner.y-10), F_STORE);
	sprintf(str2, "sample = %d/60 sec", interval);
	string(&defont, str2, &display,
		Pt((scope.origin.x+scope.corner.x)/2-80,scope.corner.y+7),
		F_STORE);
	i = Drect.origin.x+3;
	j = (scope.corner.y + scope.origin.y) / 2;
	string(&defont, "SCHED", &display, Pt(i,j-13), F_STORE);
	string(&defont, "PER", &display, Pt(i,j), F_STORE);
	string(&defont, "SEC.", &display, Pt(i,j+13), F_STORE);
};

maxperint = muldiv(1300, interval, 60);
scdisp = 0;
bar(scdisp, history[1], scope);
go:
now = realtime();
disptime = now;
schedcnt = 0;

for( ; ; ) {
	base = disptime;
	schedcnt++;
	wait(CPU);  /* be fair -  do it once */
	nextdisp = disptime + interval;
	while( realtime() < nextdisp ) {
		schedcnt++;
		wait(CPU);
	};
	now = realtime();
	actinterval = now - base;

	/* listen to the user ... */
	if('q'==kbdchar() ) exit();
	if(P->state&RESHAPED) {
		P->state&=~RESHAPED;
		goto reshaped;
	};
menus:	if(button3()) { /* Do It Yourselfer's ... */
		switch(menuhit(&mainmenu, 3))	{
		case 0: waitbut3();
			hit = menuhit(&sammenu, 3)+1;
			interval = samsize[hit];
			samsize[0] = samsize[hit];
			if (hit == 0) break;
			goto reshaped;
		case 1: waitbut3();
			hit = menuhit(&pixmenu, 3)+1;
			blotwid = pixsize[hit];
			pixsize[0] = pixsize[hit];
			if (hit == 0) break;
			goto reshaped;
			break;
		case 2: waitbut3();
			hit = menuhit(&htmenu, 3)+1;
			blotht = intensity[hit];
			intensity[0] = intensity[hit];
			if (hit == 0) break;
			goto reshaped;
		case 3: teeny = 1 - teeny;
			goto reshaped;
		case 4: /* hold onto sample line: fix it and don't erase */
			for(i=1; i<20; i++) 
				blot((scdisp+i)%wid,history[(scdisp+i)%wid], scope);
			bar(scdisp, history[scdisp], scope);
			for(i=0; i<wid; i++) history[i] = -50;
			break;
		case 5: /* stop */
			waitbut3(); goto menus;
		case 6: goto go;
		case -1: break;
		};
	};
		
	/* update the scope */
	schperint = muldiv(interval, schedcnt, actinterval);
	if(schperint>maxperint) {
		sprintf(str, " %d", muldiv(schperint,60,interval));
		string(&defont, str, &display,
			Pt(scope.origin.x-60,scope.origin.y), F_STORE);
		maxperint = schperint;
	};

	tick = muldiv(ht, schperint, maxperint);
	while( (disptime+interval) <= now ) {
		disptime += interval;
		schedcnt -= schperint;
		bar(scdisp, history[scdisp], scope);
		scdisp = (++scdisp) % wid;
		fadedisp = (++fadedisp) % wid;

		blot(fadedisp, history[fadedisp], scope);
		history[scdisp] = tick;
		bar(scdisp, history[scdisp], scope);
		blot(scdisp, history[scdisp], scope);
	};
	};

}


blot(x,y, scope)
int	x,y;
Rectangle	scope; 
{
int 	cx,cy;
cx = scope.origin.x + (x * blotwid);
cy = scope.corner.y - ( y + blotht );
/* draw the point */

rectf(&display, 
	Rect(cx, cy-blotht, cx+blotwid, cy),
	F_XOR);
}

bar(x,y, scope)
int	x,y;
Rectangle	scope; 
{
int 	cx,cy;
int	ex = 2;
cx = scope.origin.x + ( (x+1) * blotwid);
cy = scope.corner.y - (y + blotht);
/* draw the point */
	texture(&display, 
		Rect(cx-ex, cy-blotht-ex, cx+blotwid+ex, cy+ex),
		&grey, F_XOR);
}

/* wait until button 3 is pressed */
waitbut3()
{
waitloop:
	if(button3())
		return;
	else
		sleep(2);
goto waitloop;
}
