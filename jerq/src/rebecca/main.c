#include	<jerq.h>
#include	"instruments.h"
#include	"rebecca.h"

int *ALIVE, QUIT=0;
int res=16, magn=0, runlength=0;
int Gity, Gitres, Gitmag, Sem0=1, Sem1=1, Sem2=1, Sem3=1;
int Sem5=1, Sem6=1, Sem7=1, Sem9=1;
int nselect=0;
Rectangle dr, Gitrec, Nullrect;

unsigned char *photo[SZ];
unsigned char *pool[SZ]; short npool=0;
short have[SZ], r_side[SZ], l_side[SZ];

unsigned char get(), selected[512];
char anychange=0;

main()
{	int i, j, k; char c = '0';
	dr = inset(Drect, 4);
	request(RCV|SEND|MOUSE|KBD);

	ALIVE = &(P->state);
	if (!startdoit())
	{	disp("not enough memory", dr.origin);
		sleep(120); sendchar('q');
		exit();
	}
	reopen(1);
	Nullrect.origin = Nullrect.corner = PT(0, 0);

	Iinit();
	setreso(res);
	starters();

	while (1)
	{	if (P->state & (RESHAPED|MOVED))
		{	P->state &= ~(RESHAPED|MOVED);
			dr = inset(Drect, 4);
			clearinstruments();
			instruments();
			update();
		}
		if (!Sem0) { getline(Gity, Gitres, Gitmag); Sem0 = 1; }
		if (!Sem1) { Getrect(3, &Gitrec); Sem1 = 1; }
		if (!Sem2) { writefile(); Sem2 = 1; }
		if (!Sem3) { flushost(); Sem3 = 1; }
		if (!Sem5) { reopen(0); Sem5 = 1; }
		if (!Sem6) { rubbersheet(); Sem6 = 1; }
		if (!Sem7) { flushost(); sendchar('i'); Sem7 = 1; }
		if (!Sem9) { newxoff(); Sem9 = 1; }
		domouse();
		wait(CPU);
	}
}

reopen(how)
{	register int i;
	extern int Sem4;

	flushost();
	sendchar('o');
	if (get() == '1')
	{	for (i = 0; i < SZ; i++)
			invalidate(i);
		if (how) npool = 0;
		anychange = 0;
		Sem4  = 1;
	}
}

invalidate(i)
{
	if (have[i] < 2*SZ || l_side[i] < SZ)
	{	pool[npool++] = photo[i];
		have[i] = 2*SZ;
		l_side[i] = SZ;
		r_side[i] = 0;
	}
}

cutout(minx, maxx, miny, maxy)
{	int i;

	for (i = miny; i <= maxy; i++)
	{	if (have[i] == 2*SZ && l_side[i] == SZ)
			continue;

		have[i] = 2*SZ;
		if (maxx < l_side[i] || minx > r_side[i])
			continue;

		if (maxx-r_side[i] > l_side[i]-minx)
			l_side[i] = maxx;
		else
			r_side[i] = minx;
	}		
}

salvage()
{	extern int xoff;
	int i, r=res, ex=xoff+SZ/res;

	for (i = 0; i < SZ; i++)
	{	if ((i < xoff || i > ex) && i%r)
			invalidate(i);
	}
}

getline(i, re, ma)
{	int j=0, k, m, p, cnt, ores, run = runlength;
	int curoff=0;
	unsigned char c, baf[512];

	if (i < 0 || i >= SZ) return;

	flushost();
	if (have[i] == 2*SZ && l_side[i] == SZ)
	{	if (npool>0)
			photo[i] = pool[--npool];
		else
			photo[i] = (unsigned char *) alloc(SZ);
	}
	while (!photo[i])
	{	disp("--out of memory--", dr.origin);
		j = 1-j;
		sendchar('a');
		sendn(re);
		salvage();
		sleep(10);
		if (npool>0)
			photo[i] = pool[--npool];
		else
			photo[i] = (unsigned char *) alloc(SZ);
	}
	if (j) disp("--out of memory--", dr.origin);
	cnt = (ma)?1:re;

	sendchar((run)?'Y':'y');
	sendn(i);
	sendn(re);
	sendchar(ma);

	ores = have[i];

	if (cnt == 1)
	{	k = getn();	/* offset */
		curoff = k;
	} else
		k = 0;

	if (run)
	{	for (j = getn(), p = 0; j > 0; j--)
		{	m = (int)get(); c = get(); 
			while (m-- > 0) baf[p++] = c;
		}
	} else
	{	for (j = getn(), p = 0; j > 0; j--)
			baf[p++] = get(); 
	}

	for (m = 0; k < SZ && m < p; k += cnt)
	{	if (ores == 2*SZ || k%ores)
			photo[i][k] = baf[m++];
	}
	if (cnt > 1)
		have[i] = cnt;
	else
	{	r_side[i] = max(r_side[i], k);
		l_side[i] = min(l_side[i], curoff);
	}
}

domouse()
{
	if (!((own()&MOUSE) && button123() && ptinrect(mouse.xy, Drect)))
		return;

	if (button1() && button2())
		checkscreen();
}

quit()
{	int i;
	if (confirm(3))
	{	flushost();
		sendchar('q');
		QUIT = 1;
		exit();
	}
}

writefile()
{	flushost();
	sendchar('w');
	if (get() == '1')
		anychange=0;
}

flushost()
{	if (nselect > 0)
	{	sendnchars(nselect, selected);
		nselect = 0;
		sendchar('p');	/* refreshes ramtek display */
	}
}

valhost(val, how)
	unsigned char val;
	char how;
{
	if (nselect > 500) flushit();
	selected[nselect++] = how;
	selected[nselect++] = val;
}

newxoff()
{	extern int xoff;

	if (nselect > 500) flushost();
	selected[nselect++] = 'x';
	pushn(xoff);
}

pushn(n)
	short n;
{
	selected[nselect++] = n>>8;
	selected[nselect++] = n&255;
}

quadhost(x1,y1,x2,y2,x3,y3,x4,y4,X1,Y1,X2,Y2)
	short x1, y1, x2, y2, x3, y3, x4, y4;
	short X1, Y1, X2, Y2;
{
	flushost();
	selected[nselect++] = 'R';
	pushn(x1); pushn(y1);
	pushn(x2); pushn(y2);
	pushn(x3); pushn(y3);
	pushn(x4); pushn(y4);
	pushn(X1); pushn(Y1);
	pushn(X2); pushn(Y2);
	flushost();
}

ptohost(pt, c, how)
	Point pt;
{
	if (nselect > 500) 
	{	if (how) flushost(); else flushit();
	}
	selected[nselect++] = c;
	pushn(pt.y); pushn(pt.x);
}

rectohost(r, c)
	Rectangle r;
{
	if (nselect > 500) flushit();
	selected[nselect++] = c;
	pushn(r.origin.y); pushn(r.origin.x);
	pushn(r.corner.y); pushn(r.corner.x);
}

unsigned char
get()
{	int k;

	if ((k = rcvchar()) == -1)
	{	wait(RCV);
		k = rcvchar();
	}
	return (unsigned char) k;
}

getn()
{	int n = get();

	n <<= 8;
	n |= get();

	return(n&0xFFFF);
}

sendn(n)
{	char local[2];
	local[0] = n>>8;
	local[1] = n;
	sendnchars(2, local);
}


Psem(flag)
	int *flag;
{
	*flag = 0;
	while (OK && *flag==0) wait(CPU);
}

flushit()
{
	Psem(&Sem3);
}
