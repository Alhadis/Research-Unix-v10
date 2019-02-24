#include	"draw_dag.h"

#include	<sys/types.h>
#include	<sys/times.h>
#define TIC 60

// local storage for building splines
static const int max_cntrl_pts = 3;
static Point	*Pt;
static int	Pt_size;
static int	N_points;

static void addpoint(Point p)
{
	if(!Pt || N_points >= Pt_size)
	{
/*
		extern Point *realloc(...), *malloc(...);
*/
		if(Pt)
		{
			Pt_size += 4;
/*
			Pt = realloc(Pt,Pt_size*sizeof(Pt[0]));
*/
			Pt = (Point *)realloc((char *)Pt,Pt_size*sizeof(Pt[0]));
		}
		else
		{
			Pt_size = (Maxlevel+2)*max_cntrl_pts+1;
			Pt = new Point[Pt_size];
		}
		if(!Pt)
			panic("out of memory");
	}

	Pt[N_points].x = p.x;
	Pt[N_points].y = p.y;
	N_points++;
}


// copy the contructed spline points
static Point *copypoints()
{
	Point *pt = new Point[N_points];
	for(int n = 0; n < N_points; ++n)
	{
		pt[n].x = Pt[n].x;
		pt[n].y = Pt[n].y;
	}
	return pt;
}	


// see if a straight edge can be drawn for a flat edge
static int straightedge(int v, edge_t *e)
{
	int w = e->node;
	int left = Invert[v], right = Invert[w], node = v;
	if(left > right)
	{
		swap(left,right);
		node = w;
	}

	// if the left hand side node has a self edge, it can't be done
	for(edge_t *f = Noedges[node]; f; f = f->next)
		if(f->node == node)
			return 0;

	// if there is an intervening real node, it can't be done
	int *rank = Rank[Level[v]];
	for(left += 1; left < right; ++left)
		if(rank[left] < N_real_nodes)
			return 0;

	// if there is not enough separation to draw an edge
	if(iabs(Nodepos[v]-Nodepos[w])-(Width[v]+Width[w])/2 <
	   max(Levelsep/2,Levelsep-Nodesep))
		return 0;

	// see if there is an opposite edge
	int has_opposite = 0;
	for(f = Noedges[w]; f; f = f->next)
		if(f->node == v)
		{
			has_opposite = 1;
			break;
		}

	Point p0, p1;
	p0.y = p1.y = Levelpos[Level[v]];
	p0.x = Nodepos[v] + (Invert[v] < Invert[w] ? 1 : -1)*Width[v]/2;
	p1.x = Nodepos[w] + (Invert[w] < Invert[v] ? 1 : -1)*Width[w]/2;
	addpoint(p0);
	if(has_opposite)
	{
		Point mid;
		mid.x = (p0.x+p1.x)/2;
		mid.y = p0.y + (e->weight > 0 ? -1 : 1)*(e->width+Nodesep)/2;
		addpoint(mid);
	}
	addpoint(p1);

	p0.x = p0.y = -1;
	addpoint(p0);

	e->splinept = copypoints();
	return 1;
}



// make splines for flat edges.
// Here we compute a circle arc that joins v and w and has an appropriate
// height, then use a few points on it to define the spline control points.
static void flatedge(int v, edge_t *e)
{
	N_points = 0;

	// see if we can draw a straight line
	if(straightedge(v,e))
		return;

	// half the distance between v and w
	int w = e->node;
	double r = fabs((Nodepos[v]-Nodepos[w])/2.);

	// the height of the point half way between v and w
	double h = Levelsep/3.;

	// the height of the point 1/4 of the way between v and w
//	extern double sqrt(double);
	double h4 = (h*h - r*r + sqrt((r*r + h*h)*(r*r + h*h) - r*r*h*h))/(2*h);

	int level = Level[v];
	Point p0, p1, p2, p3, p4;
	p0.x = Nodepos[v];
	p1.x = (3*Nodepos[v] + Nodepos[w])/4;
	p2.x = (Nodepos[v] + Nodepos[w])/2;
	p3.x = (Nodepos[v] + 3*Nodepos[w])/4;
	p4.x = Nodepos[w];
	if(e->weight > 0)
	{
		p0.y = Levelpos[level] - Height[v]/2;
		p1.y = (int)(Levelpos[level] - Levelheight[level]/2. - h4);
		p2.y = (int)(Levelpos[level] - Levelheight[level]/2. - h);
		p3.y = p1.y;
		p4.y = Levelpos[level] - Height[w]/2;
	}
	else
	{
		p0.y = Levelpos[level] + Height[v]/2;
		p1.y = (int)(Levelpos[level] + Levelheight[level]/2. + h4);
		p2.y = (int)(Levelpos[level] + Levelheight[level]/2. + h);
		p3.y = p1.y;
		p4.y = Levelpos[level] + Height[w]/2;
	}

	addpoint(p0);
	addpoint(p1);
	addpoint(p2);
	addpoint(p3);
	addpoint(p4);
	p0.x = p0.y = -1;
	addpoint(p0);

	e->splinept = copypoints();
}



// spline for self edges
static void selfedge(int v, edge_t *e)
{
	N_points = 0;

	Point p0, p1, p2, p3, p4;
	p0.x = Nodepos[v]+Width[v]/2;
	p0.y = Levelpos[Level[v]] - Height[v]/8;
	p2.x = p0.x + Nodesep + e->width/2;
	p2.y = Levelpos[Level[v]];
	p1.x = (p2.x + p0.x)/2;
	p1.y = p0.y - Nodesep/4 - e->width/4;
	p4.x = Nodepos[v]+Width[v]/2;
	p4.y = Levelpos[Level[v]] + Height[v]/8;
	p3.x = p1.x;
	p3.y = p4.y + Nodesep/4 + e->width/4;

	addpoint(p0);
	addpoint(p1);
	addpoint(p2);
	addpoint(p3);
	addpoint(p4);

	p0.x = p0.y = -1;
	addpoint(p0);

	e->splinept = copypoints();
}


// compute a line aiming from v to w and confined to the box
// defined by 'tl' and 'br'.
// The line is guaranteed not to hit any adjacent boxes.
// Line equation is defined as x = my + b instead of y = mx + b to avoid
// the problem of infinite slopes with vertical lines.

static int defineline(Point v, Point w, Point tl, Point br,
	int thisnode, double &m, double &b)
{
	// horizontal line
	if(v.y == w.y)
	{
		m = HUGE;
		return 0;
	}

	// 1 if the defined ray meets w.
	// -1 if ray doesnot meet w because of a parallel edge
	// 0 if ray doesnot meet w because of intersection with nodes
	int meet = 1;

	// the ray that aims at w
	m = (v.x - w.x) / ((double)(v.y - w.y));
	b = v.x - m*v.y;

	edge_t *e = v.y < w.y ? Inedges[thisnode] : Outedges[thisnode];
	int ewidth = e->width/2;

	int dir = v.x < w.x ? -1 : 1;
	int level = Level[thisnode];
	int level_y = Levelpos[level];
	int *rank = Rank[level];
	for(int i = Invert[thisnode]+dir; i >= 0 && rank[i] >= 0; i += dir)
	{
		if(m == 0.)
			break;

		int node = rank[i];
		int x = Nodepos[node], y;

		// out of relevant bound
		if((dir < 0 && x < v.x) || (dir > 0 && x > v.x))
			break;

		if(node >= N_real_nodes)
		{
			// see if there is a parallel edge
			if(v.y < w.y)
			{
				int ix = Nodepos[Inedges[node]->node];
				if((ix-v.x)*(x-w.x) < 0)
					continue;
			}
			else
			{
				int ox = Nodepos[Outedges[node]->node];
				if((ox-v.x)*(x-w.x) < 0)
					continue;
			}
		}

		int height = Height[node]/2 + max(1,min(Levelsep/4,Height[node]/4));

		x += dir < 0 ? Width[node]/2 : -Width[node]/2;
		y = (int)(((x + (dir < 0 ? ewidth : -ewidth)) - b)/m);

		if((v.y < w.y && y < level_y-height) ||
		   (v.y > w.y && y > level_y+height))
			continue;

		if(thisnode >= N_real_nodes && Outedges[thisnode]->count > 1)
		{
			w.y = v.y < w.y ? tl.y : br.y;
			if(v.y == w.y)
				break;
			m = (v.x - w.x) / ((double)(v.y - w.y));
			b = v.x - m*v.y;
			break;
		}

		if(node >= N_real_nodes)
		{
			y = v.y < w.y ? level_y-height : level_y+height;
			if(meet == 1)
				meet = -1;
		}
		else
		{
			// the line from v to w-corner
			Point p;
			p.x = v.x < w.x ? tl.x : br.x;
			p.y = v.y > w.y ? br.y : tl.y;
			if(v.y == p.y)
				break;
			m = (v.x - p.x) / ((double)(v.y - p.y));
			b = v.x - m*v.y;
			if(m == 0.)
				break;

			y = (int)((x - b)/m);

			if(v.y > w.y)
			{
				int a_y = level_y+height;
				y = p.y < a_y ? a_y : min(y,a_y);
			}
			else
			{
				int a_y = level_y-height;
				y = p.y > a_y ? a_y : max(y,a_y);
			}

			meet = 0;
		}

		if(v.y == y)
			break;
		m = (v.x - x) / ((double)(v.y - y));
		b = v.x - m*v.y;
	}

	return meet;
}


// compute the point where a line enters a box.
static void lineXbox(Point v,int n,double m,double b,Point tl,Point br,Point &i)
{
	// center line of the box
	int mid_x = Nodepos[n];

	// try the top or bottom edge
	i.y = v.y < tl.y ? tl.y : br.y;
	i.x = m == HUGE ? mid_x : (int)(m*i.y + b + .5);
	if(m == HUGE)
		return;

	// miss the box, try the sides
	if(i.x < tl.x-1 || i.x > br.x+1)
	{
		i.x = v.x < mid_x ? tl.x : br.x;
		i.y = m == 0. ? (v.y < tl.y ? tl.y : br.y) : (int)((i.x - b)/m + .5);
	}

	// still miss it
	if(i.y > br.y+1 || i.y < tl.y-1 ||
	   (v.x < mid_x-1 && i.x > mid_x+1) ||
	   (v.x > mid_x+1 && i.x < mid_x-1))
	{
		if(v.x < mid_x)
		{
			if(Invert[n] > 0)
			{
				int a = Rank[Level[n]][Invert[n]-1];
				int x = max(v.x,Nodepos[a]+Width[a]/2);
				i.x = (x + tl.x + 1)/2;
			}
			else	i.x = tl.x;
		}
		else
		{
			int a = Rank[Level[n]][Invert[n]+1];
			if(a >= 0)
			{
				int x = min(v.x,Nodepos[a]-Width[a]/2);
				i.x = (x + br.x + 1)/2;
			}
			else	i.x = br.x;
		}

		i.y = m == 0. ? (v.y < tl.y ? tl.y : br.y) : (int)((i.x - b)/m + .5);
	}
}



// see if a line cross a self edge
static int lineXselfedge(int v, Point nextpt, double &m, double &b, int &right_y)
{
	if(N_self_edges <= 0 || m == 0. || m == HUGE)
		return 0;
	for(edge_t *e = Noedges[v]; e; e = e->next)
		if(e->node == v)
			break;
	if(!e)
		return 0;

	// location of the lowest point of self-edge
	int self_x = Nodepos[v] + Width[v]/2 + Nodesep/2;
	if(nextpt.x <= self_x)
		return 0;
	int self_y = Levelpos[Level[v]];
	if(nextpt.y > self_y)
	{
		self_y += Height[v]/8 + Nodesep/4 + e->width/2 + Height[v]/16;
		self_y = min(self_y,Levelpos[Level[v]]+Height[v]/2);
	}
	else
	{
		self_y -= Height[v]/8 + Nodesep/4 + e->width/2 + Height[v]/16;
		self_y = max(self_y,Levelpos[Level[v]]-Height[v]/2);
	}

	int y = (int)((self_x - b)/m);
	if((nextpt.y > self_y && y >= self_y) ||
	   (nextpt.y < self_y && y <= self_y))
		return 0;

	// compute the line that goes to self_x, self_y
	m = ((double)(nextpt.x-self_x))/(nextpt.y-self_y);
	b = self_x - m*self_y;

	// find where it intersects the middle of the box
	y = (int)((Nodepos[v]-b)/m);

	if(nextpt.y > self_y)
		right_y = max(right_y,y);
	else	right_y = min(right_y,y);

	return 1;
}


// compute the down part of a spline
static void downspline(int v, edge_t *edge, int &left_y, int &right_y)
{
	N_points = 0;

	Point	thispt, lastpt, nextpt,	// the points to spline thru
		tl, br,			// box containing thispt
		top_i, bot_i;		// intersections with box
	double	m, b;			// line equation coeffs

	int	level = Level[v];
	edge_t	*e = edge;
	thispt.x = Nodepos[v];
	thispt.y = Levelpos[level];
	nextpt.x = Nodepos[e->node];
	nextpt.y = Levelpos[level+1] - Height[e->node]/2;

	// box containing v to be aimed at from below
	tl.x = thispt.x - Width[v]/2;
	tl.y = thispt.y - Height[v]/2;
	br.x = thispt.x + Width[v]/2;
	br.y = thispt.y + Height[v]/2;

	// the point to be aimed at
	if(nextpt.x < thispt.x)
		thispt.y = max(left_y,thispt.y);
	else if(nextpt.x > thispt.x)
		thispt.y = max(right_y,thispt.y);

	defineline(nextpt,thispt,tl,br,v,m,b);
	if(lineXselfedge(v,nextpt,m,b,right_y))
		thispt.y = max(right_y,thispt.y);
	br.y = Levelpos[level] + Height[v]/2;
	lineXbox(nextpt,v,m,b,tl,br,top_i);

	// if outside of the actual bounding box of v
	if(top_i.y > br.y+1)
	{
		thispt.y = br.y;
		defineline(top_i,thispt,tl,br,v,m,b);
		lineXbox(top_i,v,m,b,tl,br,thispt);
		addpoint(thispt);
		if(nextpt.x < thispt.x)
			left_y = br.y;
		else if(nextpt.x > thispt.x)
			right_y = br.y;
	}
	else
	{
		// compute the intersection with the vertical line thru v
		int y = m == 0. ? br.y : (int)((thispt.x - b)/m);
		if(nextpt.x < thispt.x)
			left_y = max(y,left_y);
		else if(nextpt.x > thispt.x)
			right_y = max(y,right_y);
	}

	addpoint(top_i);
	lastpt.x = top_i.x;
	lastpt.y = top_i.y;

	// loop thru all virtual nodes
	int did_virtual = 0;
	for(; e->chain; e = e->chain)
	{
		int	thisnode = e->node;
		if(Deleted[thisnode])
			continue;

		level = Level[thisnode];
		thispt.x = Nodepos[thisnode];
		thispt.y = Levelpos[level];

		for(edge_t *f = e->chain; f; f = f->chain)
			if(f->node < N_real_nodes || !Deleted[f->node])
				break;
		int nextnode = f->node;
		nextpt.x = Nodepos[nextnode];
		nextpt.y = Levelpos[Level[nextnode]] - Height[nextnode]/2;

		// top and bottom corners of the box containing this node.
		int box_h = Height[thisnode];
		int box_w = Width[thisnode] + Nodesep/2;
		tl.y = thispt.y - box_h/2;
		tl.x = thispt.x - box_w/2;
		br.y = thispt.y + box_h/2;
		br.x = thispt.x + box_w/2;

		// define the top/bot rays by aiming them at the box center.
		double top_m, top_b, bot_m, bot_b;
		int top_meet, bot_meet;
		top_meet = defineline(lastpt,thispt,tl,br,thisnode,top_m,top_b);
		bot_meet = defineline(nextpt,thispt,tl,br,thisnode,bot_m,bot_b);

		// these are identical lines!
		if(top_m == bot_m && top_b == bot_b)
			continue;

		// so we know that some spline points was outputed
		did_virtual = 1;

		// increase box size if possible to smooth spline turns
		int *rank = Rank[level];
		int i = Invert[thisnode];
		int l = i > 0 ? rank[i-1] : -1;
		int r = rank[i+1];
		int lx = l >= 0 ? Nodepos[l]+Width[l]/2+Nodesep : -1;
		int rx = r >= 0 ? Nodepos[r]-Width[r]/2-Nodesep : -1;
		if(lastpt.x < lx && nextpt.x > rx)
			tl.x = min((lx+tl.x)/2,tl.x);
		if(lastpt.x > rx && nextpt.x < lx)
			br.x = max((rx+br.x)/2,br.x);

		if(lx >= 0)
			lx -= Nodesep;
		if(rx >= 0)
			rx += Nodesep;

		// points where the lines enter the box
		Point mid;
		lineXbox(lastpt,thisnode,top_m,top_b,tl,br,top_i);
		lineXbox(nextpt,thisnode,bot_m,bot_b,tl,br,bot_i);

		// horizontal line
		if(top_m == HUGE || bot_m == HUGE)
		{
			addpoint(top_i);
			addpoint(bot_i);
			continue;
		}

		if(top_m != bot_m || top_m == 0. || bot_m == 0.)
		{
			Point line_i;
			line_i.y = (int)((bot_b - top_b)/(top_m - bot_m));
			line_i.x = (int)(top_m*line_i.y + top_b);

			// see if the lines intersect inside the box
			if(top_m == 0. || bot_m == 0. ||
			   (line_i.x >= tl.x-1 && line_i.x <= br.x+1))
			{
				// add spline point to avoid node intersection
				mid.x = lastpt.x < thispt.x ? lx : rx;
				if((!top_meet || top_i.y < tl.y-1) &&
				   top_m != 0. && mid.x >= 0)
				{
					mid.y = (int)((mid.x-top_b)/top_m+.5);
					addpoint(mid);
				}
				addpoint(top_i);
				mid.y = (top_i.y+bot_i.y+1)/2;
				mid.x = (top_i.x+bot_i.x+1)/2;
				addpoint(mid);
				addpoint(bot_i);
				mid.x = nextpt.x < thispt.x ? lx : rx;
				if((!bot_meet || bot_i.y > br.y+1) &&
				   bot_m != 0. && mid.x >= 0)
				{
					mid.y = (int)((mid.x-bot_b)/bot_m+.5);
					addpoint(mid);
					bot_i.x = mid.x;
					bot_i.y = mid.y;
				}

				lastpt.x = bot_i.x;
				lastpt.y = bot_i.y;
				continue;
			}

			// the intersection is outside the box.
			// See if the two lines come from the same side
			if((lastpt.x - thispt.x)*(nextpt.x - thispt.x) >= 0)
			{
				// boundaries of immediate neighbors;
				// we don't want to overshoot them!
				int adj, min_x, max_x;
				adj = Invert[thisnode] <= 0 ? -1 :
					Rank[level][Invert[thisnode]-1];
				min_x = adj < 0 ? 0 :
					Nodepos[adj]+(Width[adj]+Nodesep)/2;
				adj = Rank[level][Invert[thisnode]+1];
				max_x = adj < 0 ? Maxint :
					Nodepos[adj]-(Width[adj]+Nodesep)/2;

				// find the points close to the intersection pt
				// and use their midpoint to make the turn smooth
				int x, top_y, bot_y;
				if(line_i.x > min_x && line_i.x < max_x)
					if(line_i.x < tl.x)
						x = (line_i.x + tl.x)/2;
					else	x = (line_i.x + br.x)/2;
				else	if(line_i.x < tl.x)
						x = (min_x + tl.x)/2;
					else	x = (max_x + br.x)/2;

				top_y = (int)((x - top_b)/top_m);
				bot_y = (int)((x - bot_b)/bot_m);

				mid.x = x;
				mid.y = (top_y + bot_y + 1)/2;

				addpoint(top_i);
				addpoint(mid);
				addpoint(bot_i);

				lastpt.x = bot_i.x;
				lastpt.y = bot_i.y;
				continue;
			}
		}

		// here we have lines that come from different sides
		// of the box and intersect outside it.
		// Move these lines as close to each other as possible.
		int meet;
		mid.x = thispt.x;
		mid.y = (int)((mid.x - bot_b)/bot_m);
		meet = defineline(lastpt,mid,tl,br,thisnode,top_m,top_b);
		if(meet <= 0)
		{
			mid.y = (int)((mid.x - top_b)/top_m);
			defineline(nextpt,mid,tl,br,thisnode,bot_m,bot_b);
		}
		lineXbox(lastpt,thisnode,top_m,top_b,tl,br,top_i);
		lineXbox(nextpt,thisnode,bot_m,bot_b,tl,br,bot_i);

		addpoint(top_i);
		mid.x = (top_i.x + bot_i.x + 1)/2;
		mid.y = (top_i.y + bot_i.y + 1)/2;
		addpoint(mid);
		addpoint(bot_i);

		lastpt.x = bot_i.x;
		lastpt.y = bot_i.y;
	}

	// so in upspline we'll know whether a spline point was output */
	thispt.y = did_virtual;

	thispt.x = -1;
	addpoint(thispt);

	edge->splinept = copypoints();
}



// now do the down end point of a spline
static void upspline(int w, edge_t *e, int &left_y, int &right_y)
{
	N_points = 0;

	// find the corresponding Outedge
	while(e->node >= N_real_nodes)
		e = e->chain;
	e = e->link;

	// current spline control points and their number
	Point *spline = e->splinept;
	for(int n_points = 0; spline[n_points].x >= 0; ++n_points)
		;

	Point lastpt, thispt, tl, br;

	// define the lastpt before routing to w
	lastpt.y = spline[n_points-1].y;
	lastpt.x = spline[n_points-1].x;

	// now define the point aiming to and the box around it
	int level = Level[w];
	thispt.x = Nodepos[w];
	thispt.y = Levelpos[level];
	tl.x = thispt.x - Width[w]/2;
	tl.y = thispt.y - Height[w]/2;
	br.x = thispt.x + Width[w]/2;
	br.y = thispt.y + Height[w]/2;
	if(lastpt.x < thispt.x)
		thispt.y = min(left_y,thispt.y);
	else if(lastpt.x > thispt.x)
		thispt.y = min(right_y,thispt.y);

	Point i;
	double m, b;
	defineline(lastpt,thispt,tl,br,w,m,b);
	if(lineXselfedge(w,lastpt,m,b,right_y))
		thispt.y = min(right_y,thispt.y);
	lineXbox(lastpt,w,m,b,tl,br,i);

	// see if a mid point is needed
	// note that spline[n_points].y contains the info on
	// whether a spline point had been output
	if(e->count > 1 && spline[n_points].y == 0)
	{
		Point mid;
		mid.x = (i.x+lastpt.x + 1)/2;
		mid.y = (i.y+lastpt.y + 1)/2;
		addpoint(mid);
	}

	// the intersection is outside the bounding box
	// add a few points to route it to the box
	if(i.y < tl.y-1)
	{
		if(lastpt.x < thispt.x)
			left_y = tl.y;
		else if(lastpt.x > thispt.x)
			right_y = tl.y;

		thispt.y = tl.y;
		defineline(i,thispt,tl,br,w,m,b);
		lineXbox(i,w,m,b,tl,br,thispt);
		addpoint(i);
		addpoint(thispt);
	}
	else
	{
		// compute the intersection with the vertical line thru w
		int y = m == 0. ? thispt.y : (int)((thispt.x-b)/m);
		if(lastpt.x < thispt.x)
			left_y = min(y,left_y);
		else if(lastpt.x > thispt.x)
			right_y = min(y,right_y);

		addpoint(i);
	}

//	extern Point *realloc(...);
	spline = (Point *)realloc((char *)spline,(n_points+N_points+1)*sizeof(spline[0]));

	// copy new points back in
	e->splinept = spline;
	spline += n_points;
	for(int n = 0; n < N_points; ++n, ++spline)
	{
		spline->x = Pt[n].x;
		spline->y = Pt[n].y;
	}
	spline->x = spline->y = -1;
}



// Given two nodes on the same rank, count how many edges
// go into the space between them.
static int between(int v, int w, edge_t **edges)
{
	int *rank = Rank[Level[v]];
	if((w = Invert[w]) < (v = Invert[v]))
		swap(v,w);

	int count = 0;
	for(v += 1; v < w; v++)
		for(edge_t *e = edges[rank[v]]; e; e = e->next)
			count += e->count;
	return count;
}



// partition flat edges below/above the rank to avoid edge crossings
static void assignside()
{
	for(int i = 0; i <= Maxlevel; ++i)
	{
		int side = 1;
		for(int *node = Rank[i]; *node != -1; ++node)
		{
			int v = *node;
			if(v >= N_real_nodes)
				continue;

			for(edge_t *e = Noedges[v]; e; e = e->next)
			{
				if(e->weight != 0)
					continue;

				int w = e->node;
				if (w >= N_real_nodes)
					continue;

				// self edge
				if(v == w)
					continue;

				// see if there is an opposite edge and
				// process it at the same time
				for(edge_t *f = Noedges[w]; f; f = f->next)
					if(f->node == v)
						break;

				// calculate # of intersections from top or bot
				int top_i = between(v,w,Inedges);
				int bot_i = between(v,w,Outedges);
				if(top_i-bot_i != 0)
				{
					if(!f || f->count < e->count)
						e->weight = top_i < bot_i ? 1 : -1;
					else	e->weight = top_i > bot_i ? 1 : -1;
				}
				else	e->weight = side;

				if(f)
					f->weight = -e->weight;
				else	side = e->weight;
			}
		}
	}
}


// dag_spline itself
static int adjcmp(edge_t **e1, edge_t **e2)
{
	return Invert[(*e1)->node]-Invert[(*e2)->node];
}

static void setmargins() {
	// reset the margin
	int xmax = 0, ymax = 0;
	int leftmargin = 0;
	int topmargin = Levelheight[0]/2;
	int rightmargin, bottommargin;
	int v;
	for(int i = 0; i <= Maxlevel; i++)
		leftmargin = max(leftmargin,Width[Rank[i][0]]);
	leftmargin = (leftmargin+1)/2;
	for(v = 0; v < N_nodes; ++v)
	    Nodepos[v] += leftmargin;

	/* here we adjust the node placement to fit user's desired aspect ratio */
	if ((Xbound > 0) && (Ybound > 0)) {
		for(v = 0; v < N_nodes; ++v) {
			int w2 = (Width[v] + 1) / 2;
			int h2 = (Height[v] + 1) / 2;
			if (xmax < Nodepos[v] + w2) {
				xmax = Nodepos[v] + w2;
				rightmargin = w2;
			}
			if (ymax < Levelpos[Level[v]] + h2) {
				ymax = Levelpos[Level[v]] + h2;
				bottommargin = h2;
			}
		}
		switch ((Xbound < xmax) + 2 * (!!(Ybound < ymax))) {
			case 0:
				break;
			case 1:
				Ybound = (int)(Ybound * (double)xmax/(double)Xbound);
				Xbound = xmax;
				break;
			case 2:
				Xbound = (int)(Xbound * (double)ymax/(double)Ybound);
				Ybound = ymax;
				break;
			case 3:
				double request_ratio = (double)Xbound/ (double)Ybound;
				double actual_ratio = (double)xmax/ (double)ymax;
				if (actual_ratio < request_ratio) {
					Ybound = ymax;
					Xbound = (int)(Ybound * request_ratio);
				}
				else {
					Xbound = xmax;
					Ybound = (int)(Xbound / request_ratio);
				}
				break;
			}

		if ((xmax < Xbound) && (xmax > (rightmargin + leftmargin))) {
			double t1 = Xbound - rightmargin - leftmargin;
			double t2 = xmax - rightmargin - leftmargin;
			double xstretch = t1 / t2;
			for (int v = 0; v < N_nodes; ++v)
				Nodepos[v] = (int)(leftmargin + (Nodepos[v] - leftmargin) * xstretch);
		}
		if ((ymax < Ybound) && (Maxlevel > 1)) {
			double ystretch = ((double)Ybound - topmargin - bottommargin)/((double)ymax - topmargin - bottommargin);
			for (int lev = 1; lev <= Maxlevel; lev++)
				Levelpos[lev] = (int)(topmargin + ((Levelpos[lev] - topmargin) * ystretch));
		}
	}
}

void dag_spline()
{
	struct tms begtm, endtm;
	if(Verbose)
	{
#ifndef TIMING
		fprintf(stderr,"Making splines\n");
#endif
		times(&begtm);
	}

	// reset margins, scale picture
	setmargins();

	// make splines for flat and self edges
	if(Noedges)
	{
		assignside();

		for(int v = 0; v < N_real_nodes; ++v)
		for(edge_t *e = Noedges[v]; e; e = e->next)
			if(e->node == v)
				selfedge(v,e);
			else	flatedge(v,e);
	}

	// space to sort edges
	edge_t **edges = new edge_t*[N_nodes];

	// compute splines for cross-level edges
	for(int v = 0; v < N_real_nodes; v++)
	{
		// these tell where we can aim without causing crossings
		int left_y = Levelpos[Level[v]], right_y = left_y;

		// make a sorted list of adjacent forward edges
		int n_forward = 0;
		for(edge_t *e = Outedges[v]; e; e = e->next)
			edges[n_forward++] = e;
		if(n_forward >= 2)
			qsort((char*)edges,n_forward,sizeof(edges[0]),(qsortcmpfun)adjcmp);

		// initialize the places in v that we can aim at
		left_y = Levelpos[Level[v]];
		right_y = left_y;

		for(int n = 0, m = n_forward-1; n <= m;)
		{
			if(Nodepos[edges[n]->node] <= Nodepos[v])
				downspline(v,edges[n++],left_y,right_y);
			if(m >= n && Nodepos[edges[m]->node] > Nodepos[v])
				downspline(v,edges[m--],left_y,right_y);
		}
	}

	// fix up the down end-points of edges so they won't intersect
	for(v = 0; v < N_real_nodes; ++v)
	{
		int n_forward = 0;
		for(edge_t *e = Inedges[v]; e; e = e->next)
			edges[n_forward++] = e;
		if(n_forward >= 2)
			qsort((char*)edges,n_forward,sizeof(edges[0]),(qsortcmpfun)adjcmp);

		// initialize the places in v that we can aim at
		int left_y = Levelpos[Level[v]], right_y;
		right_y = left_y;

		for(int n = 0, m = n_forward-1; n <= m;)
		{
			if(Nodepos[edges[n]->node] <= Nodepos[v])
				upspline(v,edges[n++],left_y,right_y);
			if(m >= n && Nodepos[edges[m]->node] > Nodepos[v])
				upspline(v,edges[m--],left_y,right_y);
		}
	}
	delete edges;

	if(Verbose)
	{
		times(&endtm);
		int total = (endtm.tms_utime-begtm.tms_utime) +
				(endtm.tms_stime-begtm.tms_stime);
		int percent = (total - (total/TIC)*TIC)*100/TIC;
#ifdef TIMING
		fprintf(stderr,"%d.%02d\t",total/TIC,percent);
#else
		fprintf(stderr,"Time in making splines: %d.%02ds\n",
				total/TIC, percent);
#endif
	}
}

