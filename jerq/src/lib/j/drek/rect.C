typedef struct {
	short x;
	short y;
} Point;
typedef struct {
	Point origin;
	Point corner;
} Rectangle;

Rectangle
Rect(a,b,c,d)
short a,b,c,d;
{
	Rectangle temp;
	temp.origin.x = a;
	temp.origin.y = b;
	temp.corner.x = c;
	temp.corner.y = d;
	return(temp);
}
Point
Pt(a,b)
short a,b;
{
	Point temp;

	temp.x = a;
	temp.y = b;
	return(temp);
}
Rectangle
Rpt(p,q)
Point p,q;
{
	Rectangle temp;

	temp.origin.x = p.x;
	temp.origin.y = p.y;
	temp.corner.x = q.x;
	temp.corner.y = q.y;
	return(temp);
}
main()
{	
	Point Pt();
	register Point p;

	int x,y;

	x=0;
	y=0;

	p = Pt(x,y);
}
