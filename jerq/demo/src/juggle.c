#include <jerq.h>
#define r16 Rect( 0, 0, 16, 16 )
struct segmnt {
	int	ts, xs, ys;
	int	tm, xm, ym;
	int	te, xe, ye;
};

quadratic( sts, sxs, stm, sxm, ste, sxe, st )
{
	long at2, bt;
	long tm = stm-sts, te = ste-sts, t = st-sts, xm = sxm-sxs, xe = sxe-sxs;

	at2 = (xm*t*t/tm - xe*t*t/te)/(tm-te);
	 bt = (xm*t/tm - (xm*t - xe*t*tm/te)/(tm-te));
	
	return at2 + bt + sxs;
}


int  period, high, hand, low, right, left, balls, air, hold, move;
struct Bitmap *ballbits;

#define BMAX 55
Point Pos[BMAX+1];
struct segmnt Seg[10];
int Segct;

update( t, i )
{
	struct segmnt s;
	Point new;
	int j;

	if( i <= balls ){ 
		t += period*i/balls;
		t %= period;
		for( j  = 0; t > Seg[j+1].ts; ++j ){};
		s = Seg[j];
		new.x = quadratic( s.ts, s.xs, s.tm, s.xm, s.te, s.xe, t );
		new.y = quadratic( s.ts, s.ys, s.tm, s.ym, s.te, s.ye, t );
	}else
		new.x = new.y = 0;
	if( !eqpt( new, Pos[i] ) ){
		bitblt( ballbits, r16, &display, Pos[i], F_XOR ); 
		Pos[i] = new;
		bitblt( ballbits, r16, &display, Pos[i], F_XOR );
	}
}
setsegs(){
	int s, sum, t;

	Segct = 0;
	seg( right-move, hand, air,  (left*2+right)/3, high, air,  left,  hand );
	seg( left,  hand, hold, left,             hand, hold, left+move,  hand );
	seg( left+move,  hand, air,  (left+right*2)/3, high, air,  right, hand );
	seg( right, hand, hold, right,            hand, hold, right-move, hand );
	for( sum = s = 0; s < Segct; ++s ) sum += Seg[s].tm + Seg[s].te;
	for( t = s = 0; s < Segct; ++s ){
		Seg[s].ts = muldiv( t, period, sum );
		t += Seg[s].tm;
		Seg[s].tm = muldiv( t, period, sum );
		t += Seg[s].te;
		Seg[s].te = muldiv( t, period, sum );
	}
	Seg[Segct].ts = period+1;
}	

seg( xs, ys, tm, xm, ym, te, xe, ye )
{
	register struct segmnt *s = &Seg[Segct++];

	s->xs = xs;
	s->ys = ys;
	s->tm = tm;
	s->xm = xm;
	s->ym = ym;
	s->te = te;
	s->xe = xe;
	s->ye = ye;
}

shape()
{
	high = Drect.origin.y+50;
	hand = Drect.corner.y-50;
	right = Drect.corner.x-50-8;
	left = Drect.origin.x+50-8;
}

main()
{
	static Point Pt0;
	int t, b, c, s = 1;

	request( KBD );
	period = (4*60);
	balls = 3;
	air = 5;
	hold = 2;
	move = 32;
	shape();

	while( !(ballbits = balloc( r16 ) ) ) sleep( 60*10 );
	rectf( ballbits, r16, F_CLR );
	disc( ballbits, Pt(8,8), 7, F_STORE );
	setsegs();
	t = 0;
	while( 1 ){
		if( P->state & RESHAPED ){
			P->state &= ~RESHAPED;
			for( b = 1; b <= BMAX; ++b ) Pos[b] = Pt0;
			rectf( &display, Drect, F_CLR );
			shape();
			setsegs();
		}
		if( (c = kbdchar()) != -1 ){
			if( c >= '0' && c <= '9' ) s = c - '0';
			else switch( c ){
				case 'a' : air += 1; break;
				case 'd' : if( air>1 ) air -= 1; break;
				case '+' : if( balls<BMAX-1) balls += 2; break;
				case '-' : if( balls>2) balls -= 2; break;
				case 'h' : high -= 10; break;
				case 'l' : high += 10; break;
				case 'w' : left -= 5; right += 5; break;
				case 'n' : left += 5; right -= 5; break;
				case 'f' : if( period > 20 ) period -= 10; break;
				case 's' : period += 10; break;
				case 'q' : exit();
				case 'i' : 
				case 'j' : rectf( &display, Drect, F_XOR );
					   break;
			}
			setsegs();
		}
		t = (t+s)%period;
		sleep( s );
		for( b = 1; b <= BMAX; ++b ) update( t, b );
	}
}
