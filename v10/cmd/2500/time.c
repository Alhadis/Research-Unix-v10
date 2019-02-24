#include	"hdr.h"

Time
gettime()
{
	char buf[64];
	Time t;

	if(cmd(4, "timer", buf, "%c%c%c", 0x61, 0x0C, 1)){
		t.h = t.m = t.s = t.f = 0;
		return(t);
	}
	return(atotime(buf+2));
}

settimer(t, who)
	Time t;
{
	cmd(0, "settimer", (char *)0, "%c%c%t", 0x44, who, t);
}

timer(type)
{
	char buf[64];
	Time t;

	if(cmd(4, "timer", buf, "%c%c%c", 0x61, 0x0C, type))
		return;
	t = atotime(buf+2);
	switch(buf[1])
	{
	case 0x04:	print("LTC: %T\n", t); break;
	case 0x14:	print("corrected LTC: %T\n", t); break;
	case 0x06:	print("VITC: %T\n", t); break;
	case 0x00:	print("TIMER-1: %T\n", t); break;
	case 0x01:	print("TIMER-2: %T\n", t); break;
	default:	print("unexpect timer return type %2.2ux\n", buf[1]);
	}
}

Time
atotime(b)
	char *b;
{
#define	M(x)	((((x)>>4)&0x7)*10 + ((x)&0xF))
	Time t;

	t.f = M(b[0]&0x3F);
	t.s = M(b[1]);
	t.m = M(b[2]);
	t.h = M(b[3]);
	return(t);
}

taddfr(t, n)
	register Time *t;
{
	t->f += n;
	if(t->f >= 30){
		t->s += t->f/30;
		t->f %= 30;
		if(t->s >= 60){
			t->m += t->s/60;
			t->s %= 60;
			if(t->m >= 60){
				t->h += t->m/60;
				t->m %= 60;
			}
		}
	}
}

timecmp(t0, t1)
	Time t0, t1;
{
	register k;

	if(k = t0.h - t1.h) return(k);
	if(k = t0.m - t1.m) return(k);
	if(k = t0.s - t1.s) return(k);
	return(t0.f - t1.f);
}
