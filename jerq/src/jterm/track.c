#include <jerq.h>
#include <font.h>

#define o origin
#define c corner

#define dist(P,Q) norm(P.x-Q.x, P.y-Q.y, 0)
#define abs(A) (((A)>=0)?(A):(-(A)))
#define min(A,B) (((A)<(B))?(A):(B))
#define max(A,B) (((A)>(B))?(A):(B))

Point growarc(but, c, s, e)
int but; 
Point c, s, e;
{	Point p, op, ip;
	Point a[3];
	int g, i,j,k;

	a[0] = c;  a[1] = s;  a[2] = e;
	ip = op = mouse.xy;
	i = dist(ip, a[0]);
	j = dist(ip, a[1]);
	k = dist(ip, a[2]);
	if (k<=j && k<=i) g = 2;
	else if (j<=i && j<=k) g = 1;
	else g = 0;
	do {
		p = mouse.xy;
		if ( !eqpt(p, op) ) {
			cursinhibit();
			arc(&display, a[0], a[1], a[2], F_XOR);
			a[g] = add(a[g], sub(p,op));
			arc(&display, a[0], a[1], a[2], F_XOR);
			cursallow();
			op = p;
		}
		nap(2);
	} while (button(but));
	arc(&display, a[0], a[1], a[2], F_XOR);
	return( sub(p, ip) );		
}

Point movearc(but, center, start, End) 
int but; 
Point center, start, End;
{	Point p, op, v;
	Point end;
	op = mouse.xy;
	end = End;
	do {
		p = mouse.xy;
		if ( !eqpt(p, op) ) {
			cursinhibit();
			arc(&display,center,start,end,F_XOR);
			v = sub(p,op);
			center = add(center,v);
			start = add(start,v);
			end = add(end,v);
			arc(&display,center,start,end,F_XOR);
			cursallow();
			op = p;
		}
		nap(2);
	} while (button(but));
	arc(&display,center,start,end,F_XOR);
	return( sub(end,End) );		
}

drawbox(p,q) Point p,q;
{
	segment(&display,p,Pt(q.x,p.y),F_XOR);
	segment(&display,Pt(q.x,p.y),q,F_XOR);
	segment(&display,q,Pt(p.x,q.y),F_XOR);
	segment(&display,Pt(p.x,q.y),p,F_XOR);
}

Rectangle makerect(p,q) Point p,q;
{	Rectangle r;
	r.origin.x = min(p.x,q.x);
	r.origin.y = min(p.y,q.y);
	r.corner.x = max(p.x,q.x);
	r.corner.y = max(p.y,q.y);
	return(r);
}
	
Point growbox(but, r, fill) 
int but, fill; 
Rectangle r;
{	Point p, op, p1, p2, ip;

	ip = op = mouse.xy;
	if (abs(op.x-r.o.x) < abs(op.x-r.c.x)) {
		p1.x = r.o.x;
		p2.x = r.c.x;
	}
	else {
		p1.x = r.c.x;
		p2.x = r.o.x;
	}	
	if (abs(op.y-r.o.y) < abs(op.y-r.c.y)) {
		p1.y = r.o.y;
		p2.y = r.c.y;
	}
	else {
		p1.y = r.c.y;
		p2.y = r.o.y;
	}
	do {
		p = mouse.xy;
		if ( !eqpt(p, op) ) {
			cursinhibit();
			if (fill) rectf(&display,makerect(p1,p2),F_XOR);
			else drawbox(p1,p2);
			p1 = add(p1,sub(p, op));
			if (fill) rectf(&display,makerect(p1,p2),F_XOR);
			else drawbox(p1,p2);
			cursallow();
			op = p;
		}
		nap(2);
	} while (button(but));
	if (fill) rectf(&display,makerect(p1,p2),F_XOR);
	else drawbox(p1,p2);
	return(sub(p,ip));
}

Point movebox(but, R, fill) 
int but, fill; 
Rectangle R;
{	Point p, op;
	Rectangle r;

	op = mouse.xy;
	r = R;
	do {
		p = mouse.xy;
		if ( !eqpt(p, op) ) {
			cursinhibit();
			if (fill) rectf(&display,r,F_XOR);
			else {
				segment(&display,r.o,Pt(r.c.x,r.o.y),F_XOR);
				segment(&display,Pt(r.c.x,r.o.y),r.c,F_XOR);
				segment(&display,r.c,Pt(r.o.x,r.c.y),F_XOR);
				segment(&display,Pt(r.o.x,r.c.y),r.o,F_XOR);
			}
			r = raddp(r,sub(p, op));
			if (fill) rectf(&display,r,F_XOR);
			else {
				segment(&display,r.o,Pt(r.c.x,r.o.y),F_XOR);
				segment(&display,Pt(r.c.x,r.o.y),r.c,F_XOR);
				segment(&display,r.c,Pt(r.o.x,r.c.y),F_XOR);
				segment(&display,Pt(r.o.x,r.c.y),r.o,F_XOR);
			}
			cursallow();
			op = p;
		}
		nap(2);
	} while (button(but));
	if (fill) rectf(&display,r,F_XOR);
	else {
		segment(&display,r.o,Pt(r.c.x,r.o.y),F_XOR);
		segment(&display,Pt(r.c.x,r.o.y),r.c,F_XOR);
		segment(&display,r.c,Pt(r.o.x,r.c.y),F_XOR);
		segment(&display,Pt(r.o.x,r.c.y),r.o,F_XOR);
	}
	return( sub(r.origin, R.origin) );		
}

Point growcircle(but, center, radius, fill)
int but, fill; 
Point center;
register int radius;
{	Point p, op, ip;
	int rad, d;
	ip = op = mouse.xy;
	d = dist(ip, center);
	rad = radius;
	do {
		p = mouse.xy;
		if ( !eqpt(p, op) ) {
			cursinhibit();
			if (fill) disc(&display,center,rad,F_XOR);
			else circle(&display,center,rad,F_XOR);
			rad = abs(radius + dist(p,center) - d);
			if (fill) disc(&display,center,rad,F_XOR);
			else circle(&display,center,rad,F_XOR);
			cursallow();
			op = p;
		}
		nap(2);
	} while (button(but));
	if (fill) disc(&display,center,rad,F_XOR);
	else circle(&display,center,rad,F_XOR);
	return( sub(p,ip) );		
}

Point movecircle(but, Center, radius, fill)
int but, fill; 
Point Center;
register int radius;
{	Point p, op;
	Point center;
	op = mouse.xy;
	center = Center;
	do {
		p = mouse.xy;
		if ( !eqpt(p, op) ) {
			cursinhibit();
			if (fill) disc(&display,center,radius,F_XOR);
			else circle(&display,center,radius,F_XOR);
			center = add(center, sub(p,op));
			if (fill) disc(&display,center,radius,F_XOR);
			else circle(&display,center,radius,F_XOR);
			cursallow();
			op = p;
		}
		nap(2);
	} while (button(but));
	if (fill) disc(&display,center,radius,F_XOR);
	else circle(&display,center,radius,F_XOR);
	return( sub(center,Center) );		
}

Point growelarc(but, center, start, end, a, b)
int but;
Point center, start, end;
register int a, b;
{	Point p, op, ip;
	Point q[3];
	int g, i, j, k;

	q[0] = center;  q[1] = start;  q[2] = end;
	ip = op = mouse.xy;
	i = dist(ip, q[0]);
	j = dist(ip, q[1]);
	k = dist(ip, q[2]);
	if (i<=j && i<=k) g = 0;
	else if (j<=i && j<=k) g = 1;
	else g = 2;
	do {
		p = mouse.xy;
		if ( !eqpt(p, op) ) {
			cursinhibit();
			elarc(&display,q[0],a,b,q[1],q[2],F_XOR);
			q[g] = add(q[g], sub(p, op));
			elarc(&display,q[0],a,b,q[1],q[2],F_XOR);
			cursallow();
			op = p;
		}
		nap(2);
	} while (button(but));
	elarc(&display,q[0],a,b,q[1],q[2],F_XOR);
	return( sub(p, ip) );		
}

Point moveelarc(but, center, start, End, a, b)
int but;
Point center, start, End;
register int a, b;
{	Point p, op, v;
	Point end;
	op = mouse.xy;
	end = End;
	do {
		p = mouse.xy;
		if ( !eqpt(p, op) ) {
			cursinhibit();
			elarc(&display,center,a,b,start,end,F_XOR);
			v = sub(p,op);
			center = add(center,v);
			start = add(start,v);
			end = add(end,v);
			elarc(&display,center,a,b,start,end,F_XOR);
			cursallow();
			op = p;
		}
		nap(2);
	} while (button(but));
	elarc(&display,center,a,b,start,end,F_XOR);
	return( sub(end,End) );		
}

Point growellipse(but, center, A, B, fill)
int but, fill;
Point center;
int A, B;
{	Point p, op;
	Point ip, q;
	register int a = A, b = B, od, d;

	ip = op = mouse.xy;
	od = dist(op, center);
	do {
		p = mouse.xy;
		if ( !eqpt(p, op) ) {
			cursinhibit();
			if (fill) eldisc(&display,center,a,b,F_XOR);
			else ellipse(&display,center,a,b,F_XOR);
			d = dist(p, center);
			q = sub(p,op);
			if (d>od) {
				a += abs(q.x);
				b += abs(q.y);
			}
			else if (d<od) {
				a -= abs(q.x);
				b -= abs(q.y);
			}
			a = abs(a);
			b = abs(b);
			if (fill) eldisc(&display,center,a,b,F_XOR);
			else ellipse(&display,center,a,b,F_XOR);
			cursallow();
			op = p;
			od = d;
		}
		nap(2);
	} while (button(but));
	if (fill) eldisc(&display,center,a,b,F_XOR);
	else ellipse(&display,center,a,b,F_XOR);
	return( sub(p, ip) );		
}

Point moveellipse(but, Center, a, b, fill)
int but, fill;
Point Center;
register int a, b;
{	Point p, op;
	Point center;
	op = mouse.xy;
	center = Center;
	do {
		p = mouse.xy;
		if ( !eqpt(p, op) ) {
			cursinhibit();
			if (fill) eldisc(&display,center,a,b,F_XOR);
			else ellipse(&display,center,a,b,F_XOR);
			center = add(center, sub(p,op));
			if (fill) eldisc(&display,center,a,b,F_XOR);
			else ellipse(&display,center,a,b,F_XOR);
			cursallow();
			op = p;
		}
		nap(2);
	} while (button(but));
	if (fill) eldisc(&display,center,a,b,F_XOR);
	else ellipse(&display,center,a,b,F_XOR);
	return( sub(center,Center) );		
}

Point growline(but, a, b)
int but;
Point a, b;
{	Point p, op, ip;
	int reverse = 0;

	ip = op = mouse.xy;
	if (dist(ip,a) > dist(ip,b)) reverse = 1;
	do {
		p = mouse.xy;
		if ( !eqpt(p, op) ) {
			cursinhibit();
			segment(&display,a,b,F_XOR);
			if (reverse) b = add(b,sub(p, op));
			else a = add(a,sub(p, op));
			segment(&display,a,b,F_XOR);
			cursallow();
			op = p;
		}
		nap(2);
	} while (button(but));
	segment(&display,a,b,F_XOR);
	return( sub(p,ip) );		
}

Point moveline(but, a, b)
int but;
Point a, b;
{	Point p, op;
	Point  b0;
	op = mouse.xy;
	b0 = b;
	do {
		p = mouse.xy;
		if ( !eqpt(p, op) ) {
			cursinhibit();
			segment(&display,a,b,F_XOR);
			a = add(a,sub(p, op));
			b = add(b,sub(p, op));
			segment(&display,a,b,F_XOR);
			cursallow();
			op = p;
		}
		nap(2);
	} while (button(but));
	segment(&display,a,b,F_XOR);
	return( sub(b,b0) );		
}

Point growstring(but, pt, str, ft)
int but; 
Point pt; 
char *str; 
Font *ft;
{	Point p, op, ip;
	ip = op = mouse.xy;
	do {
		p = mouse.xy;
		if ( !eqpt(p, op) ) {
			cursinhibit();
			string(ft,str,&display,pt,F_XOR);
			pt = add(pt, sub(p,op));
			string(ft,str,&display,pt,F_XOR);
			cursallow();
			op = p;
		}
		nap(2);
	} while (button(but));
	string(ft,str,&display,pt,F_XOR);
	return( sub(p, ip) );		
}

Point movestring(but, pt, str, ft)
int but;
Point pt;
char *str;
Font *ft;
{	Point p, op, ip;
	ip = op = mouse.xy;
	do {
		p = mouse.xy;
		if ( !eqpt(p, op) ) {
			cursinhibit();
			string(ft,str,&display,pt,F_XOR);
			pt = add(pt, sub(p,op));
			string(ft,str,&display,pt,F_XOR);
			cursallow();
			op = p;
		}
		nap(2);
	} while (button(but));
	string(ft,str,&display,pt,F_XOR);
	return( sub(p, ip) );		
}
