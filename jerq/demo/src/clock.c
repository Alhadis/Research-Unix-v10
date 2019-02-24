#include <jerq.h>
#include <font.h>
Texture stip={
	0x1111,	0x4444,	0x1111,	0x4444,	0x1111,	0x4444,	0x1111,	0x4444,
	0x1111,	0x4444,	0x1111,	0x4444,	0x1111,	0x4444,	0x1111,	0x4444,
};

#define	atoi2(p)	((*(p)-'0')*10 + *((p)+1)-'0')
#define	itoa2(n, s)	{ (*(s) = (n)/10 + '0'); (*((s)+1) = n % 10 + '0'); }
#define NOALARM "no alarm"
Point	ctr, p, cur;
int	dx, dy;
int	h, m, s;	/* hour, min, sec */
int	rh, rm, rs;	/* radius of ... */
int	ah, am, as;	/* angle */
int	rad;
int	olds;
char	alarmstr[] = NOALARM;
char	alarmlen;
int 	alarmset;

main(argc, argv)
	char *argv[];
{
	char date[40], *p;
	register long oldtime;
	int stat, c, ds;
	int first = 1;

	rectf(&display, Drect, F_XOR);
	if(argc!=2){
		string(&defont,"Usage: 32ld clock \"`date`\"",
			&display,Drect.origin,F_XOR);
		sleep(600);
		exit();
	}
	request(KBD|ALARM);
	initface();
	strcpy(date, argv[1]);
	h = atoi2(date+11);
	m = atoi2(date+14);
	s = atoi2(date+17);

	oldtime=realtime();
	for ( ds = 0;; ) {
		while (realtime() <= oldtime) {
			if(own()&(KBD|ALARM))
				checkalarm();
			sleep(20);
		}
		ds += realtime()-oldtime;
		oldtime=realtime();
		s += ds / 60;
		ds %= 60;
		if (olds == s)
			continue;
		while (s >= 60) {
			s -= 60;
			m++;
		}
		olds = s;
		while (m >= 60) {
			m -= 60;
			h++;
			if (h >= 24)
				h = 0;
		}

		if (!first) {	/* zap previous rays */
			ray(rs, as);
			if (am != as)
				ray(rm, am);
			if (ah != am && ah != as)
				ray(rh, ah);
		}
		ah = (30 * (h%12) + 30 * m / 60);
		am = 6 * m;
		as = 6 * s;

		if (P->state & RESHAPED) {
			initface();
			first=1;
			P->state &= ~RESHAPED;
		}
		if (!first)
			string(&defont,date,&display,Drect.origin,F_XOR);
		sprintf(date, "00:00:00");
		itoa2(h, date);
		itoa2(m, date+3);
		itoa2(s, date+6);
		string(&defont,date,&display,Drect.origin,first?F_STORE:F_XOR);
		first = 0;
		ray(rs, as);	/* longest */
		if (am != as)
			ray(rm, am);
		if (ah != as && ah != am)
			ray(rh, ah);
	}
}

initface()	/* set up clock circle in window */
{
	rectf(&display, Drect, F_CLR);
	texture(&display, display.rect, &stip, F_STORE);
	ctr.x = (Drect.corner.x + Drect.origin.x) / 2;
	ctr.y = (Drect.corner.y + Drect.origin.y) / 2;
	rad = Drect.corner.x - Drect.origin.x;
	if (rad > Drect.corner.y - Drect.origin.y)
		rad = Drect.corner.y - Drect.origin.y;
	rad = rad/2 - 2;
	rh = 6 * rad / 10;
	rm = 9 * rad / 10;
	rs = rad - 1;
	circle(&display, ctr, rad, F_XOR);
/*****
	circle(&display, ctr, rad-1, F_XOR);
***/
	disc(&display, ctr, rad, F_STORE);
	string(&defont,alarmstr,&display,
		Pt(Drect.origin.x,Drect.corner.y-defont.height),F_STORE);
}

ray(r, ang)	/* draw ray r at angle ang */
	int r, ang;
{
	int dx, dy;

	dx = muldiv(r, sin(ang), 1024);
	dy = muldiv(-r, cos(ang), 1024);
	segment(&display, ctr, add(ctr, Pt(dx,dy)), F_XOR);
}

checkalarm()
{
	int c;
	if(alarmset && (own()&ALARM)) {
		ringbell();
		if(--alarmset<=0)
			noalarm();
	}
	if(own()&KBD) {;
		while((c=kbdchar())!=-1) {
			if(c=='\r')
				alarmlen = 0;
			else if(c=='\b') {
				if(alarmlen>0) {
					alarmstring();
					alarmstr[--alarmlen] = 0;
					alarmstring();
				}
			} else if(alarmlen<8 && (c==':'||c>='0'&&c<='9')) {
				alarmstring();
				alarmstr[alarmlen++] = c;
				alarmstr[alarmlen] = 0;
				alarmstring();
			}
		}
		setalarm();
	}
}

setalarm()
{
	extern char *strchr();
	char *p = alarmstr;
	int hr = atoi(p);
	int min = 0;
	int sec = 0;
	int dt;
	if((p=strchr(p,':'))!=0)
		min = atoi(++p);
	if((p=strchr(p,':'))!=0)
		sec = atoi(++p);
	dt = sec-s + 60*(min-m + 60*(hr-h));
	while(dt<0)
		dt += 24*60*60;
	alarm(dt*60);
	alarmset = 10;
}

noalarm()
{
	alarmset = alarmlen = 0;
	alarmstring();
	strcpy(alarmstr, NOALARM);
	alarmstring();
}

alarmstring()
{
	string(&defont,alarmstr,&display,
		Pt(Drect.origin.x,Drect.corner.y-defont.height),F_XOR);
}
