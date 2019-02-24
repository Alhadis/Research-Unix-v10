#include	<jerq.h>
#include	<font.h>

#define	STOP	((own()&MOUSE) && button123() && ptinrect(mouse.xy, Drect))

extern int maxframe, video;
char buf[256], *str;
int speed = 5, video=0;
extern char unwound[2];
extern char *genfn();
Menu menu  = { (char **)0,  genfn};

run()
{	int i=0;
	extern int nFrames;

	sendchar(103);
	while (getn() != -1 && i++ < nFrames)
	{	if (!getframe()) break;
		Blast(maxframe++);
		sendchar(103);
	}

	spr(1, "                   ");
	if (maxframe > 1) autoreverse();
	while (playit()) ;
	sendnchars(2, "x\n");
}

spr(how, s, n, m, k)
	char *s;
{
	int nr=0;
	str = buf;

	while (*s)
		if ((*str++ = *s++) == '%')
		{	str--;
			switch (nr) {
			case  0: digit(n); nr++; break;
			case  1: digit(m); nr++; break;
			default: digit(k); break;
			}
		}
	*str++ = 0;
	if (how) string(&defont, buf, &display, Drect.origin, F_STORE);
}

digit(i)
{	int base=1, n=i;
	char *h = str;

	if (n < 0)
	{	*str++ = '-';
		n = -n;
	}
	while (n/base > 0)
		base *= 10;

	for ( ; base > 0; base /= 10)
	{	if (n >= base)
		{	*str++ = n/base + '0';
			n %= base;
		} else
			if (h < str) *str++ = '0';
	}
	if (h == str || (*h == '-' && h == str+1))
		*str++ = '0';
}

play()		{ while (!STOP) slowonce(); }
autoreverse()	{ while (!STOP) { slowonce(); slowback(); } }

static long when;
static int nplay;

timed()
{	long n, m;

	slowonce();

	n = realtime()-when;
	m = (maxframe*6000)/n;
	if (m%100 < 10)
	spr(1, "%.0% frames/sec (% ticks) ", m/100, m%100, n);
	else
	spr(1, "%.% frames/sec (% ticks) ",  m/100, m%100, n);
}

slowonce()
{	int n;
	when = realtime();
	nplay = 0;
	for (n = 0; n < maxframe && (!STOP); n++)
		putframe(n);
}

slowback()
{	int n;
	for (n = maxframe-1; n >= 0 && (!STOP); n--)
		putframe(n);
}

putframe(n)
{	long target;

	nplay++;
	Blast(n);
	wait(CPU);
	if (!video)
	{	target = realtime() + speed;
		while (realtime() < target) sleep(1);
	} else
	{	if (nplay) target = ((realtime() - when)*10)/nplay;
		if (target < 25) sleep(1);
	}
}

step()
{	int n = 0;

	putframe(n);
	spr(1, "frame % ", n+1);
	for (;;)
	{	wait(CPU); wait(MOUSE);
		switch (button123()) {
		case 4:	 if (++n >= maxframe) n = 0;
			 while (button1()) ;
			 putframe(n);
			 spr(1, "frame % ", n+1);
			 break;
		case 2:  if (--n < 0) n = maxframe-1;
			 while (button2()) ;
			 putframe(n);
			 spr(1, "frame % ", n+1);
			 break;
		case 6:  n = 0;
			 break;
		case 1: case 3: case 5: case 7:
			 return;
		}
	}
}

rerecv()
{	int k;
	while ((k = rcvchar()) == -1) sleep(5);
	return k;
}

getn()
{	int n = rerecv();

	n <<= 8;
	n |= rerecv();
	return(n&0xFFFF);
}

char *
genfn(i)
{	int j = 2*unwound[1] + unwound[0];
	switch(i)
	{
	case  0: spr(0, "% frames", maxframe); break;
	case  1: spr(0, "once   "); break;
	case  2: spr(0, "repeat "); break;
	case  3: spr(0, "loop   "); break;
	case  4: spr(0, "step  (button 1/2)"); break;
	case  5: spr(0, "slower   (delay=%)", speed); break;
	case  6: spr(0, "faster   (delay=%)", speed); break;
	case  7: if (video)
			spr(0, "no  movie rate");
		 else
			spr(0, "set movie rate");
		 break;
	case  8: switch (j) {
		 case  3: spr(0, "exit"); break;
		 case  2: spr(0, "unwind even frames"); break;
		 default: spr(0, "unwind  odd frames"); break;
		 }
		 break;
	case  9: switch (j) {
		 case  3: return ((char *)0); break;
		 case  0: spr(0, "unwind even frames"); break;
		 default: spr(0, "exit"); break;
		 }
		 break;
	case 10: if (j == 0)
			spr(0, "exit");
		 else
			return ((char *)0);
		 break;
	default: return ((char *)0);
	}
	return(buf);
}

playit()
{	int j = 2*unwound[1] + unwound[0];
	wait(MOUSE);
	if(!button3()) return 1;
	spr(1, "                                      ");
	switch (menuhit(&menu, 3))
	{
	case -1:
	case  0: break;
	case  1: timed(); break;
	case  2: play(); break;
	case  3: autoreverse(); break;
	case  4: step(); break;
	case  5: speed++; break;
	case  6: speed = (speed <= 0) ? 0 : speed-1; break;
	case  7: video = 1 - video; break;
	case  8: switch (j) {
		 case  3: return (!confirm(3));
		 case  2: winder(0); break;
		 default: winder(1); break;
		 }
		 break;
	case  9: switch (j) {
		 case  3: break;
		 case  0: winder(0); break;
		 default: return (!confirm(3));
		 }
		 break;
	case 10: if (j == 0) return (!confirm(3));
	default: break;
	}
	return 1;
}
