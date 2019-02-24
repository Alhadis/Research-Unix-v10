#include <jerq.h>
#include <font.h>
#include "event.h"
#include "rock.h"
#define	NSTART	4
#define	MAXSTART 12
int	kbdcheck(), quickcheck(), longcheck(), pause(), doscore();
int	mouseset();
int	death=1;
int	randx;
int	score;
int	oldscore;
int	notdrawn;	/* number of time ticks without moving rock */
int	xflag;
int	holdon;		/* don't do anything */
extern int computer, screens;
main()
{
	int nstart;
	register short i;
	extern long allocated;
	request(KBD|MOUSE);
	while((i= *XMOUSE)==0177777)
		;
	randx=i&0377;
	while((i= *YMOUSE)==0177777)
		;
	randx*=i&0377;
	holdon = 0;
Loop:
	rectf(&display, display.rect, F_OR);
	if(death){
		shipfree();
		initship();
		death=0;
		nstart=NSTART;
		score=0;
	}else
		drawship();
	oldscore=score;
	drawscore(score);
	drawstats();
	initrock(nstart);
	if(nstart<MAXSTART)
		nstart+=2;
	initevent();
	initexplosion();
	initbomb();
	/* The times between these events are tunable and somewhat arbitrary */
	addevent(newevent(), 32, kbdcheck, EKEYBOARD);	/* 32==NROCK, but that's not important */
	addevent(newevent(), 7, quickcheck, EMOUSE);
	addevent(newevent(), 31, longcheck, EMOUSE);
	addevent(newevent(), 10, pause, EPAUSE);
	addevent(newevent(), 500, doscore, ESCORE);
	addevent(newevent(), 1000, mouseset, EMOUSE);
	while(!death && allocated){
		moverock();
		tick();
		while(holdon) kbdcheck(), wait(CPU);
	}
	for(i=0; i<1000; i++){
		moverock();
		tick();
		while(holdon) kbdcheck(), wait(CPU);
	}
	exfree();
	rockfree();
	if(death){
		do
			wait(CPU);
		while(button123()==0);
	}
	goto Loop;
}
mouseset(){
	if(ptinrect(mouse.xy, Drect))
		cursset(transform(Pt(XMAX/2, 3*YMAX/4)));
	return 1;
}
quit()
{
	exfree();
	rockfree();
	shipfree();
	exit();
}
char status[] = "   ";
kbdcheck()
{	int c;
	if((c = kbdchar())=='q')
		quit();
	else if(c == 'c')
		computer++;
	else if(c == 'r')
		screens = computer = 0;
	else if(c == 's')
		screens = 1;
	else if(c == 'p')
		holdon = 1-holdon;
	else
		return(1);
	drawstats();
	status[0] = (computer? 'C': ' ');
	status[1] = (screens? 'S': ' ');
	status[2] = (holdon? 'P': ' ');
	drawstats();
	return 1;
}
drawstats()
{
	/*jmoveto(Pt(400,5));
	jstring(status);*/
	string(&defont, status, &display, transform(Pt(400,5)), F_XOR);
}
/* Number of rocks that can be moved in a clock tick, roughly */
#define	ROCKSPERTICK	4
pause()
{
	if(notdrawn>ROCKSPERTICK){
		sleep(notdrawn/ROCKSPERTICK);
		notdrawn%=ROCKSPERTICK;
	}else
		wait(CPU);
	return 1;
}
allover(p)
	Point p;
{
	register i;
	explode(p);
	drawship();	/* remove ship from screen */
	death=1;
}
rnd(x)
{
	register i;
	return rand()%x;
}
drawscore(sc)
{
	static char str[]="000000 Points";
	register char *p;
	register long s;
	s=sc;
	p= &str[6];
	do
		*--p=s%10+'0';
	while(s/=10);
	/*jmoveto(Pt(20, 5));
	jstring(p);*/
	string(&defont, p, &display, transform(Pt(20,5)), F_XOR);
}
doscore()
{
	drawscore(oldscore);
	drawscore(oldscore=score);
	return 1;
}
