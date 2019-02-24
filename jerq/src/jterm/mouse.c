# include <jerq.h>

extern void put(), send(), sendint();

#define abs(A)  ((A)<0 ? -(A) : (A))

#define minMove 5
#define mQsize 20
struct Mouse mQ[mQsize];
int Qstart = 0;
int Qnext = 0;

void sendMouse(m) struct Mouse m;
{	register int i, c;
	for(c=m.xy.x, i=0; i<3; i++, c>>=4) put(c & 15);
	for(c=m.xy.y; i<6; i++, c>>=4) put(c & 15);
	put(m.buttons);
}

void sendQ()
{
	updateMouseQ();
	if (Qnext >= Qstart) sendint(Qnext - Qstart);
	else sendint(mQsize - Qstart + Qnext);
	for ( ; Qstart != Qnext; ) {
		sendMouse(mQ[Qstart]);
		if (++Qstart == mQsize) Qstart = 0;
	}
	put('\n');
	send();
}

insertQ(m) struct Mouse m;
{
	mQ[Qnext++] = m;
	if (Qnext == mQsize) Qnext = 0;
	if (Qnext == Qstart) Qstart++;
	if (Qstart == mQsize) Qstart = 0;
}

updateMouseQ()
{	int last;
	Point p;

	if (Qnext == Qstart) insertQ(mouse);
	else {
		last = Qnext - 1;
		if (last < 0) last = mQsize - 1;
		p = sub(mQ[last].xy,mouse.xy);
		if (abs(p.x)>=minMove || abs(p.y)>=minMove 
			|| mQ[last].buttons!=mouse.buttons) insertQ(mouse);
	}
}

clearMouseQ()
{
	Qnext=Qstart=0;
}
