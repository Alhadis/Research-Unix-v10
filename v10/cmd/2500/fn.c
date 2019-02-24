#include	"hdr.h"

static Time clock;
extern debug;

two(s, a, b)
	char *s;
{
	cmd(0, s, (char *)0, "%c%c", a, b);
}

three(s, a, b, c)
	char *s;
{
	cmd(0, s, (char *)0, "%c%c%c", a, b, c);
}

four(s, a, b, c, d)
	char *s;
{
	cmd(0, s, (char *)0, "%c%c%c%c", a, b, c, d);
}

swait()
{
	if(debug)
		return;
	while((status1(2)&2) == 0)
		sleep(1);
}

frame(n)
{
	three("playback ff/sel", 0x41, 0x38, n);
	three("rec ff/sel", 0x41, 0x39, n);
}

snap(n)
{
	upclock(n);
	printclock();
	if(debug)
		return;
	while(n-- > 0){
		two("snap", 0x20, 0x65);
		while(status1(1)&2)
			sleep(1);
		while((status1(0xA)&8) == 0)
			sleep(1);
		two("step fwd", 0x20, 0x14);
		while((status1(0xA)&0x8) == 0)
			sleep(1);
	}
}

cue(t)
	Time t;
{
	cmd(0, "cue", (char *)0, "%c%c%t%c", 0x25, 0x31, t, 0x01);
	clock = t;
	printclock();
}

prerollt(t)
	Time t;
{
	cmd(0, "preroll", (char *)0, "%c%c%t%c", 0x44, 0x31, t, 0x01);
}

loop(t0, t1, fps)
	Time t0, t1;
{
	int n, n1;
        double speed, p, p1;
        extern double log10(), pow();

        if(fps > 1500) fps = 1500;
        speed = fps/30.0;
        n = 32*(2+log10(speed));
	p = pow(10.,n/32.-2);
	p1 = pow(10.,(n+1)/32.-2);
	n1 = 256 * (speed - p) / ( p1 - p );
	if(n1<0)
		Fprint(2,"can't happen! loop: fps=%d n=%d n1=%d\n",fps,n,n1);
	cue(t0);
	sleep(1);
	{
		swait();
		four("var fwd", 0x21, 0x12, n, n1);
		while(timecmp(gettime(), t1) < 0)
			sleep(1);
		two("stop", 0x20, 0x00);
		four("var rev", 0x22, 0x22, n, n1);
		while(timecmp(t0, gettime()) < 0)
			sleep(1);
		two("stop", 0x20, 0x00);
	}
}

view(t0, t1)
	Time t0, t1;
{
	int fps=64.;

	cue(t0);
	sleep(1);
	{
		swait();
		three("var fwd", 0x21, 0x12, fps);
		while(timecmp(gettime(), t1) < 0)
			sleep(1);
		two("stop", 0x20, 0x00);
	}
	cue(t0);
	sleep(1);
}

locenable()
{
	if(two("local enable", 0x00, 0x1D) == 0)
		print("local enabled\n");
}

insert(t, n)
	Time t;
{
	two("", 0x40, 0x41); 		/* AUTO ON */
	three("", 0x41, 0x30, 0x58);	/* INS, SYNC/VIDEO */
	three("", 0x41, 0x36, 0);		/* timer mode to TCR */
	cmd(0, "", (char *)0, "%c%c%t", 0x44, 0x14, t);
	taddfr(&t, n);
	cmd(0, "", (char *)0, "%c%c%t", 0x44, 0x15, t);
	two("", 0x20, 0x30);		/* PREROLL */
	while((status1(1)&0x20) == 0)
		;			/* loop until stopped */
	two("", 0x20, 0x42);		/* AUTO EDIT */
	while((status1(1)&0x20) == 0)
		;			/* loop until stopped */
	three("", 0x41, 0x30, 0x00);	/* INS, SYNC/VIDEO off */
	two("", 0x40, 0x40);		/* AUTO OFF */
}

editset(t0, t1, chmask)
	Time t0, t1;
{
	two("", 0x40, 0x41); 		/* AUTO ON */
	three("", 0x41, 0x30, 0x40|chmask);	/* INS chmask */
	three("", 0x41, 0x36, 0);		/* timer mode to TCR */
	cmd(0, "", (char *)0, "%c%c%t", 0x44, 0x14, t0);
	cmd(0, "", (char *)0, "%c%c%t", 0x44, 0x15, t1);
	two("", 0x20, 0x30);		/* PREROLL */
	while((status1(1)&0x20) == 0)
		;			/* loop until stopped */
}

editgo()
{
	two("", 0x20, 0x42);		/* AUTO EDIT */
	while((status1(1)&0x20) == 0)
		;			/* loop until stopped */
	three("", 0x41, 0x30, 0x00);	/* INS off */
	two("", 0x40, 0x40);		/* AUTO OFF */
}

still(t)
	Time t;
{
	three("still mode", 0x41, 0x3F, 0x01);	/* STILL */
	cue(t);
	swait();
}

upclock(n)
	int n;
{
	/* this won't work if n is big, since Time uses shorts */
	clock.f += n;
	while( clock.f >= 30 ){
		clock.f -= 30;
		clock.s += 1;
	}
	while( clock.s >= 60 ){
		clock.s -= 60;
		clock.m += 1;
	}
	while( clock.m >= 60 ){
		clock.m -= 60;
		clock.h += 1;
	}
}

printclock()
{
	Fprint(logfd,"# time ");
	if( clock.h != 0 )
		Fprint(logfd,"%d.",clock.h);
	Fprint(logfd,"%d.%d.%d\n",clock.m,clock.s,clock.f);
}
