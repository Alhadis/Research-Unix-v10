
#include "defs.h"
extern char *menutext[];

grid()
{

	if (gridstate == 1) {
		menutext[10] = "large grid";
		gridstate = 16;
		DrawGrid(16);
		}
	else if (gridstate == 16) {
		menutext[10] = "no grid";
		gridstate = 32;
		DrawGrid(16);
		DrawGrid(32);
		}
	else if (gridstate == 32) {
		menutext[10] = "small grid";
		gridstate = 1;
		DrawGrid(32);
		}
}

DrawGrid(size)
int size;
{
	Point p;
	int basey;

	p = grid_enforce(Dspace.origin,size);
	basey = p.y;
	for (; p.x<Dspace.corner.x; p.x+=size) {
		for (; p.y<Dspace.corner.y; p.y+=size) {
			point(&display,p,f2);
			}
		p.y = basey;
		}
}

Point grid_enforce(p,size)
Point p;
int size;
{	Point q;
	int half;

	if (size == 1) return(p);
	half = size/2;
	q.x = ((p.x+half)/size)*size;
	if (q.x <= Dspace.origin.x) q.x += size;
	if (q.x >= Dspace.corner.x) q.x -= size;
	q.y = ((p.y+half)/size)*size;
	if (q.y <= Dspace.origin.y) q.y += size;
	if (q.y >= Dspace.corner.y) q.y -= size;
	return(q);
}
