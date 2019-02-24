#include	"draw_dag.h"

#include	<sys/types.h>
#include	<sys/times.h>
#define TIC 60


/*
	Assign horizontal positions to nodes
*/

// Edge factors depend on their adjacent nodes.
// The weight system chosen below encourages straight long edges and paths.
// The node naming scheme is:
//	s:	a real node with <=1 in-edge and <=1 out-edge
//	v:	a virtual node
//	e:	anything else.
static const int  C_ee = 1,
		  C_es = 1,
		  C_ev = 1,
		  C_vs = 2,
		  C_ss = 2,
		  C_vv = 8;

static int	*Selfwidth;	// width of nodes with self edges

static void	levelposition(int),
		bestposition(),
		goodposition(),
		smoothedges();
static int	rlength(int*, int*);
static inline int widthof(int);


#ifdef DEBUG
static void checkpos(int *nodepos)
{
	for(int l = 0; l <= Maxlevel; ++l)
	for(int *r = Rank[l]+1; *r != -1; ++r)
		if((nodepos[r[-1]]+widthof(r[-1])/2+Nodesep) >
		   (nodepos[r[0]]-widthof(r[0])/2))
			panic("Bad position assignment");
}
#endif


//	ranksepar = 0 for full flexibility
//		    1 for forcing all ranks be separate by a max amount
//			computed to avoid edge/node intersections
//		   >1 for fixing at user's requested separation

void dag_positions(int ranksepar, int opt_level)
{
	struct tms begtm, endtm;
	if(Verbose)
	{
#ifndef TIMING
		fprintf(stderr,"Assign positions\n");
#endif
		times(&begtm);
	}

	// additional widths for nodes with self edges
	if(N_self_edges > 0)
	{
		Selfwidth = new int[N_nodes];
		for(int i = 0; i < N_real_nodes; ++i)
		{
			if(Rank[Level[i]][Invert[i]+1] == -1)
				continue;

			for(edge_t *e = Noedges[i]; e; e = e->next)
				if(e->node == i)
					break;
			if(e)
				Selfwidth[i] = e->width + Nodesep; 
		}
	}

	// compute node positions
	Nodepos = new int[N_nodes];
	if(opt_level >= 1)
		bestposition();
	else	goodposition();

	// readjust node positions if there are self edges
	if(N_self_edges > 0)
	{
		for(int i = 0; i < N_real_nodes; ++i)
			if(Selfwidth[i] > 0)
				Nodepos[i] -= Selfwidth[i]/2;
		delete Selfwidth;
	}

	// compute layer position
	Levelpos = new int[Maxlevel+1];
	levelposition(ranksepar);
		
	// smooth out little turns in long edges
	Deleted = new int[N_nodes];
	smoothedges();

	// readjust positions to start from 0
	int minx = Maxint;
	for(int i = 0; i <= Maxlevel; ++i)
		if(Nodepos[Rank[i][0]] < minx)
			minx = Nodepos[Rank[i][0]];
	if(minx != 0)
		for(i = 0; i < N_nodes; ++i)
			Nodepos[i] -= minx;

	if(Verbose)
	{
		times(&endtm);
		int total = (endtm.tms_utime-begtm.tms_utime) +
				(endtm.tms_stime-begtm.tms_stime);
		int percent = (total - (total/TIC)*TIC)*100/TIC;
#ifdef TIMING
		fprintf(stderr,"%d.%02d\t",total/TIC, percent);
#else
		fprintf(stderr,"Time in coordinates assignment: %d.%02ds\n",
				total/TIC, percent);
#endif
	}
}



// for an edge top->bot and the horizontal assignment of nodes,
// compute the separation between respective layers that would
// guarantee that the edge does not intersect any drawings of other
// nodes.

static int vsepar(int top, int bot, int *toprank, int *botrank, int separ)
{
	int xtop = Nodepos[top];
	int xbot = Nodepos[bot];
	int minx = min(xtop,xbot);
	int maxx = max(xtop,xbot);
	int htop = top < N_real_nodes ? Height[top]/2 : Levelheight[Level[top]]/2;
	int hbot = bot < N_real_nodes ? Height[bot]/2 : Levelheight[Level[bot]]/2;

	if(maxx-minx < Nodesep)
		return separ;

	for(; *toprank != -1; ++toprank)
		if(Nodepos[*toprank] > minx && *toprank < N_real_nodes)
			break;
	for(; *toprank != -1; ++toprank)
	{
		if(Nodepos[*toprank] >= maxx)
			break;
		if(*toprank >= N_real_nodes)
			continue;

		// the side of the box that may intersect with the edge
		int x = Nodepos[*toprank] + (Width[*toprank]/2)*(xtop>xbot ? 1:-1);
		if(x == xtop)
			continue;

		// the height of that side
		int h = Height[*toprank]+Nodesep;

		// the separation required
		int s = (int)((double)((htop*(xbot-x)+h*(xtop-xbot)))/(xtop-x));
		if((s += hbot) >= separ)
			separ = s;
	}
	for(; *botrank != -1; ++botrank)
		if(Nodepos[*botrank] > minx && *botrank < N_real_nodes)
			break;
	for(; *botrank != -1; ++botrank)
	{
		if(Nodepos[*botrank] >= maxx)
			break;
		if(*botrank >= N_real_nodes)
			continue;
		int x = Nodepos[*botrank] + (Width[*botrank]/2)*(xbot>xtop ? 1:-1);
		if(x == xbot)
			continue;
		int h = Height[*botrank]+Nodesep;
		int s = (int)((double)((hbot*(x-xtop)+h*(xtop-xbot)))/(x-xbot));
		if((s += htop) > separ)
			separ = s;
	}

	return separ;
}



static void levelposition(int ranksepar)
{
	int maxsep = 0;
	Levelpos[0] = Levelheight[0]/2;
	for(int i = 1; i <= Maxlevel; ++i)
	{
		int h_min = Levelsep + (Levelheight[i-1]+Levelheight[i])/2;
		int h_max = h_min+Levelsep;

		// reduce edge/node intersections by raising level separations
		if(ranksepar < 2)
		{
			for(int *vp = Rank[i-1]; *vp != -1; ++vp)
			for(edge_t *e = Outedges[*vp]; e; e = e->next)
				h_min = vsepar(*vp,e->node,Rank[i-1],Rank[i],h_min);
			if(ranksepar == 1)
				maxsep = max(maxsep,min(h_max,h_min));
		}
		
		if(ranksepar != 1)
			Levelpos[i] = Levelpos[i-1] + min(h_max,h_min);
	}

	if(ranksepar == 1)
		for(i = 1; i <= Maxlevel; ++i)
			Levelpos[i] = Levelpos[i-1]+maxsep;
}

// see if a node is part of a path
static int pathdegree(int n)
{
	int deg = 0;
	if(Inedges[n])
	{
		deg += 1;
		if(Inedges[n]->next)
			deg += 3;
	}
	if(Outedges[n])
	{
		deg += 1;
		if(Outedges[n]->next)
			deg += 3;
	}
	return deg;
}

// multiplicative factor for different type of edges
static int edgefactor(int v, int w)
{
#define V_BIT	001
#define S_BIT	002
#define C_EE	000	/* 0000 */
#define C_VE	004	/* 0100 */
#define C_SE	010	/* 1000 */
#define C_EV	001	/* 0001 */
#define C_VV	005	/* 0101 */
#define C_SV	011	/* 1001 */
#define C_ES	002	/* 0010 */
#define C_VS	006	/* 0110 */
#define C_SS	012	/* 1010 */

	int v_type = 0;
	if(v >= N_real_nodes)
		v_type |= V_BIT;
	else if(pathdegree(v) <= 2)
		v_type |= S_BIT;

	int w_type = 0;
	if(w >= N_real_nodes)
		w_type |= V_BIT;
	else if(pathdegree(w) <= 2)
		w_type |= S_BIT;

	switch((v_type<<2)|w_type)
	{
	default:
	case C_EE:
		return C_ee;
	case C_VE:
		return C_ev;
	case C_EV:
		return C_ev;
	case C_SE:
		return C_es;
	case C_ES:
		return C_es;
	case C_VS:
		return C_vs;
	case C_SV:
		return C_vs;
	case C_SS:
		return C_ss;
	case C_VV:
		return C_vv;
	}
}

// define width of a node
static inline int widthof(int node)
{
	int width = max(Width[node],Nodesep);
	if(N_self_edges > 0)
		width += Selfwidth[node];
	return width;
}

// solve LP to get the positions
static void bestposition()
{
	// calculate # of rows and cols
	int jumpcnt = N_nodes - Maxlevel - 1;
	int rows = N_edges + jumpcnt;
	int cols = N_nodes + 2*N_edges + jumpcnt + 1;

	// allocate simplex arrays
	long *weight = new long[N_edges];
	long *objf = new long[cols];
	long *auxf = new long[cols];
	long **arrp = new long*[rows];
	for(int i = 0; i < rows; ++i)
		arrp[i] = new long[cols];

	// fill in first N_edges rows of the constraint array
	int k = 0;
	for(i = 0; i < N_nodes; i++)
	{
		for(edge_t *e = Outedges[i]; e; e = e->next)
		{
			arrp[k][N_nodes+k] = 1;
			arrp[k][N_nodes+N_edges+k] = -1;
			arrp[k][e->node] = -1;
			arrp[k][i] = 1;
			weight[k] = e->weight*edgefactor(i,e->node);
			k++;
		}
	}

	// fill in last jumpcnt rows of constraint array
	k = N_edges;
	for(i = 0; i <= Maxlevel; i++)
	{
		for(int j = 0; j < N_level[i]-1; j++)
		{
			int v = Rank[i][j];
			int w = Rank[i][j+1];
			int vwidth = widthof(v);
			int wwidth = widthof(w);
			
			arrp[k][cols-1] = (vwidth+wwidth)/2 + Nodesep;
			arrp[k][v] = -1;
			arrp[k][w] = 1;
			arrp[k][N_nodes+N_edges+k] = -1;

			k++;
		}
	}

	// fill in last columns of objective function.
	k = N_nodes+N_edges;
	for(i = 0; i < N_edges; i++, k++)
		objf[k] = 2*weight[i];

	// fill in last columns of auxiliary function
	for(i = 0; i < jumpcnt; i++)
		auxf[N_nodes + 2*N_edges + i] = 1;
	auxf[cols-1] = -jumpcnt;

	// fill in first N_nodes columns of objective and aux functions
	for(i = 0; i < N_nodes; i++)
	{
		long sum = 0;
		for(k = 0; k < N_edges; ++k)
			sum += arrp[k][i]*weight[k];
		objf[i] = -sum;

		sum = 0;
		for(int j = 0; j < jumpcnt; j++)
			sum += arrp[N_edges + j][i];
		auxf[i] = -sum;
	}

	dag_simplex(rows,cols,arrp,auxf,objf,N_nodes,Nodepos);

	// restore space used by simplex
	delete weight;
	delete objf;
	delete auxf;
	for(i = 0; i < rows; ++i)
		delete arrp[i];
	delete arrp;
}


// try to straighten parts of a long edge
// the main for loop works from outside in to preserve any inherent symmetry
// in the part of the edge being worked on.

static int straight(int n_nodes,int *nodes,int *minx,int *maxx,int *pos)
{
	int n_done = 0;
	if(n_nodes > 4)
	{
		int top = nodes[0], bot = nodes[n_nodes-1];
		int top_lx = Nodepos[top] - Width[top]/2;
		int top_rx = Nodepos[top] + Width[top]/2;
		int bot_lx = Nodepos[bot] - Width[bot]/2;
		int bot_rx = Nodepos[bot] + Width[bot]/2;
		int d_top = Nodepos[top] - Nodepos[nodes[1]];
		int d_bot = Nodepos[bot] - Nodepos[nodes[n_nodes-2]];
		if(d_top*d_bot > 0)
		{
			for(int t = 1; t < n_nodes-1; ++t)
				if(d_top*(Nodepos[nodes[t]]-Nodepos[nodes[t+1]]) <= 0)
					break;
			for(int b = n_nodes-2; b > t; --b)
				if(d_bot*(Nodepos[nodes[b]]-Nodepos[nodes[b-1]]) <= 0)
					break;
			if(Nodepos[nodes[t]] >= top_lx && Nodepos[nodes[t]] <= top_rx)
				t = 0;
			if(Nodepos[nodes[b]] >= bot_lx && Nodepos[nodes[b]] <= bot_rx)
				b = n_nodes-1;
			if(t > 0 || b < n_nodes-1)
			{
				if(t >= 2)
					n_done = straight(t+1,nodes,minx,maxx,pos);
				if(b <= n_nodes-3)
					n_done += straight(n_nodes-b,nodes+b,
							   minx+b,maxx+b,pos);
				if((b-t) >= 2)
					n_done += straight(b-t+1,nodes+t,
							   minx+t,maxx+t,pos);
				return n_done;
			}
		}
	}
	// try to straighten all sub-intervals
	for(int t = 0; t < n_nodes-2;)
	{
		for(int b = n_nodes-1; b > t+1; --b)
		{
			int vt = nodes[t];
			int vb = nodes[b];
			int xt = Nodepos[vt];
			int xb = Nodepos[vb];
			int yt = Levelpos[Level[vt]];
			int yb = Levelpos[Level[vb]];

			// interpolate top/bottom nodes and compute new positions
			int linear = 1;
			for(int i = t+1, lev = Level[nodes[i]]; i < b; ++i, ++lev)
			{
				// new position of point
				int cury = Levelpos[lev];
				int curx = Nodepos[nodes[i]];
				int newx = (xb-xt)*(cury-yb);
				newx = (int)(((double)newx)/(yb-yt) + xb + .5);

				// easy case
				if(newx == curx)
				{
					pos[i] = curx;
					continue;
				}

				// node width
				int w = widthof(nodes[i])/2;

				// check constraints
				int violated = 0;
				if(newx-w < minx[i] || newx+w > maxx[i])
					violated = 1;

				// make sure that yanking this straight
				// won't cause node intersections
				if(!violated)
				{
					int y = cury - Levelheight[lev]/2;
					int x = (xb-xt)*(y-yb);
					x = (int)(((double)x)/(yb-yt) + xb + .5);
					if(x-w < minx[i] || x+w > maxx[i])
						violated = 1;
				}
				if(!violated)
				{
					int y = cury + Levelheight[lev];
					int x = (xb-xt)*(y-yb);
					x = (int)(((double)x)/(yb-yt) + xb + .5);
					if(x-w < minx[i] || x+w > maxx[i])
						violated = 1;
				}
				if(violated && (b-t) > 2)
					break;
				else if(violated)
				{
					linear = 0;
					int midx = (minx[i]+maxx[i])/2;
					if((curx < midx && midx < newx) ||
					   (curx > midx && midx > newx))
						newx = midx;
					if(newx-w < minx[i] || newx+w > maxx[i])
						break;
				}
				pos[i] = newx;
			}

			// if the whole segment can be straightened
			if(i == b)
			{
				for(i = t+1; i < b; ++i)
				{
					Nodepos[nodes[i]] = pos[i]; 
					Deleted[nodes[i]] = linear ? 1 : 0;
				}
				Deleted[nodes[t]] = Deleted[nodes[b]] = 0;
				n_done += b-t;

				// back to main loop
				break;
			}
		}

		// start with the rest of the interval
		t = b > t+1 ? b-1 : b;
	}
	return n_done;
}


// run thru all the edges once and try to straighten them out if
// no constraints are violated
static void setbounds(int *nodes, int n_nodes, int *minx, int *maxx)
{
	for(int k = 1; k < n_nodes-1; ++k)
	{
		int v = nodes[k];
		int i = Invert[v];
		int *rank = Rank[Level[v]];
		int a = rank[i+1];
		maxx[k] = a < 0 ? Maxint : (Nodepos[a]-widthof(a)/2) - 5*Nodesep/4;
		a = i > 0 ? rank[i-1] : -1;
		minx[k] = a < 0 ? -Maxint : (Nodepos[a]+widthof(a)/2) + 5*Nodesep/4;
	}
}

static void smoothedges()
{
	int *nodes = new int[Maxlevel+1];
	int *pos = new int[Maxlevel+1];
	int *minx = new int[Maxlevel+1];
	int *maxx = new int[Maxlevel+1];

	int counter = 0;
	int improving = 1;
	while(improving)
	{
		improving = 0;
		int dir = counter%2 ? -1 : 1;
		for(int i = 0; i < Maxlevel; ++i)
		{
			int *begp = Rank[i];
			int *endp = Rank[i]+N_level[i]-1;
			int *vp = counter%2 ? endp : begp;
			for(; vp >= begp && vp <= endp; vp += dir)
			{
				if(*vp >= N_real_nodes)
					continue;
				for(edge_t *e = Outedges[*vp]; e; e = e->next)
				{
					if(e->node < N_real_nodes)
						continue;

					// get all the nodes
					int n_nodes = 0;
					nodes[n_nodes++] = *vp;
					for(edge_t *f = e; f; f = f->chain)
						nodes[n_nodes++] = f->node;

					// bounds for intermediate nodes
					setbounds(nodes,n_nodes,minx,maxx);
					int n = straight(n_nodes,nodes,minx,maxx,pos);
					if(n > 2*n_nodes/3)
					{
						improving++;
						e->node = -(e->node);
					}
					// need these to separate parallel edges
					if(n > 0 && e->count > 1)
						Deleted[nodes[1]] =
						Deleted[nodes[n_nodes-2]] = 0;
				}
			}
		}
	}

	// reset signs of nodes
	for(int v = 0; v < N_real_nodes; ++v)
	for(edge_t *e = Outedges[v]; e; e = e->next)
		if(e->node < 0)
			e->node = -(e->node);
	delete nodes;
	delete pos;
	delete minx;
	delete maxx;
}


/*
	Fast routines to assign positions.
	The basic idea is to iteratively assign node positions based
	on the median(s) of adjacent nodes. This heuristic is aided
	with other heuristics that pack nodes closer based on local
	improvements.
*/

// compute total edge length for a set of nodes
static int rlength(int *nodepos, int *node)
{
	int length = 0;
	for(; *node != -1; ++node)
	for(edge_t *e = Outedges[*node]; e; e = e->next)
		length += iabs(nodepos[*node]-nodepos[e->node])*e->weight;
	if(length < 0)
		panic("Integer overflow");
	return length;
}

// compute the length of all edges incident on a node givenits position
static int vlength(int v, int pos, int *nodepos)
{
	int length = 0;
	for(edge_t *e = Outedges[v]; e; e = e->next)
		length += iabs(pos-nodepos[e->node])*e->weight;
	for(e = Inedges[v]; e; e = e->next)
		length += iabs(pos-nodepos[e->node])*e->weight;
	return length;
}


// structures and functions for median computations
static struct MEDIAN
{
	int	position;
	int	weight;
	int	node;
}; 
static MEDIAN	*Median;

static struct PAIR
{
	int	left;
	int	right;
};

static int	*Priority;
static int prioritycmp(register int *one, register int *two)
{
	return Priority[*two]-Priority[*one];
}


// compute the weighted median of a bunch of nodes
static int mediancmp(register MEDIAN *one, register MEDIAN *two)
{
	return one->position - two->position;
}
static void wmedian(int n, PAIR	*p)
{
	if(n <= 0)
	{
		p->left = p->right = -1;
		return;
	}
	if(n == 1)
	{
		p->left = p->right = 0;
		return;
	}

	// median weight;
	int w_total = 0;
	for(int i = 0; i < n; ++i)
		w_total += Median[i].weight;
	int w_median = (w_total+1)/2;

	// sort and the median
	qsort((char*)Median,n,sizeof(Median[0]),(qsortcmpfun)mediancmp);
	int w_accum = 0;
	for(i = 0; i < n; ++i)
	{
		w_accum += Median[i].weight;
		if(w_accum >= w_median)
			break;
	}
	if(i == n)
		panic("Bad median");
	if(i < n-1 && (w_total%2) == 0 && w_accum == w_median)
	{
		p->left = i;
		p->right = i+1;
	}
	else	p->left = p->right = i;
}

// assign position for nodes in a rank relative to an adjacent rank
static void assignpos(int *pos, int *node, int *rank, PAIR *median, int *priority)
{
	for(; *node != -1; ++node)
	{
		// boundary in which the node must appear
		int maxx = Maxint, minx = -Maxint;
		int here = Invert[*node];
		for(register int k = here+1; rank[k] != -1; ++k)
			if(priority[rank[k]] > priority[*node])
				break;
		if(rank[k] != -1)
		{
			maxx = pos[rank[k]];
			for(int n = k-1; n > here; --n)
				maxx -= widthof(rank[n]);
			maxx -= (k-here)*Nodesep;
			maxx -= (widthof(*node)+widthof(rank[k]))/2;
		}
		for(k = here-1; k >= 0; --k)
			if(priority[rank[k]] > priority[*node])
				break;
		if(k >= 0)
		{
			minx = pos[rank[k]];
			for(int n = k+1; n < here; ++n)
				minx += widthof(rank[n]);
			minx += (here-k)*Nodesep;
			minx += (widthof(*node)+widthof(rank[k]))/2;
		}
		if(maxx <= minx)
		{
			pos[*node] = minx;
			continue;
		}

		// compute new position
		int set = 0;
		PAIR *p = median + *node;
		if(p->left >= 0)
		{
			int newx = (pos[p->left]+pos[p->right]+1)/2;
			if(newx >= minx && newx <= maxx)
			{
				pos[*node] = newx;
				++set;
			}
		}
		if(!set)
		{
			int curx = pos[*node];
			if(curx < minx)
				pos[*node] = minx;
			else if(curx > maxx)
				pos[*node] = maxx;
			//else	pos[*node] = curx;
		}
	}
}


// reassign positions using medians.
// Nodes are considered in decreasing priority
static void medianpos(int *pos, int **order, int *priority, PAIR *median, int isdown)
{
	for(int l = 1; l <= Maxlevel; ++l)
	{
		int lev = isdown ? l : Maxlevel-l;
		assignpos(pos,order[l],Rank[l],median,priority);
	}
}


// same as medianpos, but for edges
static void minedge(int *nodepos)
{
	for(int v = 0; v < N_real_nodes; ++v)
	for(edge_t *e = Outedges[v]; e; e = e->next)
	{
		int w = e->node;
		if(w >= N_real_nodes || nodepos[w] != nodepos[v])
			continue;

		// interval in which the edge can move
		int v_min, v_max;
		int i = Invert[v];
		int *rank = Rank[Level[v]];
		int a = i > 0 ? rank[i-1] : -1;
		v_min = a<0 ? -Maxint : nodepos[a]+(widthof(v)+widthof(a))/2 + Nodesep;
		a = rank[i+1];
		v_max = a<0 ?  Maxint : nodepos[a]-(widthof(v)+widthof(a))/2 - Nodesep;
		rank = Rank[Level[v]+1];

		int minx, maxx;
		i = Invert[w];
		a = i > 0 ? rank[i-1] : -1;
		minx = a<0 ? -Maxint : nodepos[a]+(widthof(w)+widthof(a))/2 + Nodesep;
		a = rank[i+1];
		maxx = a<0 ?  Maxint : nodepos[a]-(widthof(w)+widthof(a))/2 - Nodesep;
		minx = max(minx,v_min);
		maxx = min(maxx,v_max);
		if(maxx <= minx)
			continue;

		// make list of adjacent nodes
		MEDIAN *m = Median;
		int count = 0;
		for(int top = 0; top < 2; ++top)
		{
			int here = top ? v : w;
			int there = top ? w : v;
			for(int in = 0; in < 2; ++in)
			{
				edge_t *e = in ? Inedges[here] : Outedges[here];
				for(; e; e = e->next)
				{
					if(e->node == there)
						continue;
					m->position = nodepos[e->node];
					m->weight = e->weight;
					++m;
					++count;
				}
			}
		}
		if(count <= 0)
			continue;

		// get the median(s)
		PAIR p;
		wmedian(count,&p);
		int newx = (Median[p.left].position+Median[p.right].position+1)/2;
		if(newx >= minx && newx <= maxx)
		{
			nodepos[v] = newx;
			nodepos[w] = newx;
		}
	}
}


// local improvement for each node based on its entire adjacent nodes
// This is the same as doing a descent from the current assignment to
// improve the objective function. This is why the algorithm will
// terminate.
static void minnode(int *nodepos, int *marked, int *queue)
{
// local inline functions
#define ENQUEUE(x)	queue[tail] = x; if(++tail >= N_nodes+1) tail = 0;
#define DEQUEUE(x)	x = queue[head]; if(++head >= N_nodes+1) head = 0;
#define NOTNULL()	head != tail

	// marked[] and queue[] have the set of nodes to be considered
	for(int n = 0; n < N_nodes; ++n)
	{
		marked[n] = 1;
		queue[n] = n;
	}

	int head = 0, tail = N_nodes;
	while(NOTNULL())
	{
		int here;

		DEQUEUE(here);
		marked[here] = 0;

		// boundary in which the node must appear
		int here_w = widthof(here);
		n = Invert[here];
		int *rank = Rank[Level[here]];
		int l = n <= 0 ? -1 : rank[n-1];
		int r = rank[n+1];
		int minx = l < 0 ? -Maxint : nodepos[l]+Nodesep+(here_w+widthof(l))/2;
		int maxx = r < 0 ?  Maxint : nodepos[r]-Nodesep-(here_w+widthof(r))/2;
		if(maxx <= minx)
			continue;

		int cnt = 0;
		MEDIAN *median = Median;
		for(int down = 0; down < 2; ++down)
		{
			edge_t *e = down ? Outedges[here] : Inedges[here];
			for(; e; e = e->next)
			{
				median->position = nodepos[e->node];
				median->weight = e->weight;
				median->node = -1;
				cnt++;
				median++;
			}
		}
		if(cnt == 0)
			continue;

		PAIR p;
		wmedian(cnt,&p);
		int newx = (Median[p.left].position+Median[p.right].position+1)/2;
		int correction = 0;
		if(newx < minx || newx > maxx)
		{
			correction = 1;
			newx = newx < minx ? minx : maxx;
		}
		if(newx == nodepos[here])
			continue;

		int newlength = vlength(here,newx,nodepos);
		int oldlength = vlength(here,nodepos[here],nodepos);
		if(newlength > oldlength || (newlength == oldlength && correction))
			continue;

		// update position and mark neighbors for consideration
		nodepos[here] = newx;
		if(newlength < oldlength)
		{
			for(down = 0; down < 2; ++down)
			{
				int adj = down ? l : r;
				if(adj >= 0 && !marked[adj])
				{
					marked[adj] = 1;
					ENQUEUE(adj);
				}
				edge_t *e = down ? Outedges[here] : Inedges[here];
				for(; e; e = e->next)
				{
					adj = e->node;
					if(marked[adj])
						continue;
					marked[adj] = 1;
					ENQUEUE(adj);
				}
			}
		}
	}
#undef ENQUEUE
#undef DEQUEUE
#undef NOTNULL
}

// pack cuts that are not tight
static int *_nodepos;
static int rightcmp(int *p1, int *p2)
{
	int right1 = _nodepos[*p1] + widthof(*p1)/2;
	int right2 = _nodepos[*p2] + widthof(*p2)/2;
	return right1-right2;
}
static int leftcmp(int *p1, int *p2)
{
	int left1 = _nodepos[*p1] - widthof(*p1)/2;
	int left2 = _nodepos[*p2] - widthof(*p2)/2;
	return left1-left2;
}
static void verticalcut(int *nodepos, int *nodes, int maxwidth)
{
	// sort vertices by their positions
	_nodepos = nodepos;
	qsort((char*)nodes,N_nodes,sizeof(nodes[0]),(qsortcmpfun)rightcmp);

	// go thru each possible cut
	for(int i = 0; i+1 < N_nodes; ++i)
	{
		int v = nodes[i];
		int cut = nodepos[v] + widthof(v)/2 + Nodesep;

		int leftside = Maxint;
		for(int k = i+1; k < N_nodes; ++k)
		{
			int w = nodes[k];

			// don't need to check further
			if((nodepos[w]-2*(maxwidth+Nodesep)) > leftside)
				break;

			// define leftside
			int wleft = nodepos[w] - widthof(w)/2;
			if(wleft < leftside)
				leftside = wleft;
			if(leftside <= cut)
				break;
		}
		// can improve
		int shift = leftside-cut;
		if(shift > 0)
			for(k = i+1; k < N_nodes; ++k)
				nodepos[nodes[k]] -= shift;
	}
}

static void mincut(int *nodepos, int *marked, int *queue, int *nodes, int dir)
{
#define ENQUEUE(x)	queue[tail] = x; if(++tail >= N_nodes) tail = 0;
#define DEQUEUE(x)	x = queue[head]; if(++head >= N_nodes) head = 0;
#define NOTNULL()	head != tail

	_nodepos = nodepos;
	qsort((char*)nodes,N_nodes,sizeof(nodes[0]),(dir < 0 ? (qsortcmpfun)leftcmp : (qsortcmpfun)rightcmp));
	int cut, lastcut = Maxint;
	int n = dir > 0 ? 0 : N_nodes-1;
	for(; n >= 0 && n < N_nodes; n += dir, lastcut = cut)
	{
		if(nodes[n] >= N_real_nodes)
			continue;

		// where the graph is being cut
		cut = nodepos[nodes[n]]+dir*(widthof(nodes[n])/2+Nodesep);
		if((dir > 0 && cut <= lastcut) || (dir < 0 && cut >= lastcut))
			continue;

		// try to shift connected components in the right of the cut
		for(int i = 0; i < N_nodes; ++i)
			marked[i] = 0;
		int component = 1;
		for(int m = n+dir; m >= 0 && m < N_nodes; m += dir)
		{
			int v = nodes[m];
			if(marked[v])
				continue;
			int side = nodepos[v] - dir*widthof(v)/2;
			if((dir > 0 && side <= cut) || (dir < 0 && side >= cut))
				continue;

			// bf-search for all nodes in this component
			int head = 0, tail = 0;
			ENQUEUE(v);
			marked[v] = component;
			while(NOTNULL())
			{
				int here;
				DEQUEUE(here);
				for(int in = 0; in < 2; ++in)
				{
					edge_t *e = in ? Inedges[here]:Outedges[here];
					for(; e; e = e->next)
					{
						int w = e->node;
						if(marked[w])
							continue;
						int side = nodepos[w]-dir*widthof(w)/2;
						if((dir > 0 && side <= cut) ||
						   (dir < 0 && side >= cut))
							continue;
						ENQUEUE(w);
						marked[w] = component;
					}
				}
			}

			int shift = Maxint;
			for(i = 0; i < tail; ++i)
			{
				int v = queue[i];
				int a = Invert[v];
				if(dir > 0)
					a = a <= 0 ? -1 : Rank[Level[v]][a-1];
				else	a = Rank[Level[v]][a+1];
				if(a < 0 || marked[v] == marked[a])
					continue;
				int side = nodepos[a] + dir*(widthof(a)/2+Nodesep);
				if((dir > 0 && side < cut) || (dir < 0 && side > cut))
					side = cut;
				int slack = nodepos[v] - dir*(widthof(v)/2);
				slack = dir > 0 ? slack-side : side-slack;
				if(slack < shift)
					shift = slack;
				if(shift <= 0)
					break;
			}
			if(shift < Maxint && shift > 0)
				for(i = 0; i < tail; ++i)
					nodepos[queue[i]] -= dir*shift;
			component++;
		}
	}
}

// straighten paths
static void getminmax(int *nodepos, int v, int& minx, int& maxx)
{
	int *rank = Rank[Level[v]];
	int i = Invert[v];
	int l = i > 0 ? rank[i-1] : -1;
	int r = rank[i+1];
	minx = l < 0 ? -Maxint : nodepos[l] + (widthof(l)+widthof(v))/2 + Nodesep;
	maxx = r < 0 ?  Maxint : nodepos[r] - (widthof(r)+widthof(v))/2 - Nodesep;
}

static void minpath(int *nodepos, int *nodes)
{
	for(int lev = 0; lev < Maxlevel; ++lev)
	for(int *rank = Rank[lev]; *rank >= 0; ++rank)
	for(edge_t *e = Outedges[*rank]; e; e = e->next)
	{
		// find a straightenable path from v
		int n_nodes = 0;
		int minx = -Maxint, maxx = Maxint;
		int v = *rank, w = e->node;
		if(pathdegree(v) == 1 && !Inedges[v])
		{
			nodes[n_nodes++] = v;
			getminmax(nodepos,v,minx,maxx);
		}
		edge_t *f = e;
		while(1)
		{
			w = f->node;
			if(pathdegree(w) > 2)
				break;

			int lx, rx;
			getminmax(nodepos,w,lx,rx);
			if(lx > maxx || rx < minx)
				break;

			minx = max(minx,lx);
			maxx = min(maxx,rx);
			nodes[n_nodes++] = w;
			if(!Outedges[w])
				break;
			else	f = Outedges[w];
		}
		if(n_nodes <= 0 || minx > maxx)
			continue;

		// new position
		int vx = nodepos[v];
		int wx = nodepos[w];
		vx = vx < minx ? minx : (vx > maxx ? maxx : vx);
		wx = wx < minx ? minx : (wx > maxx ? maxx : wx);
		int newx;
		if(v == nodes[0] && w != nodes[n_nodes-1])
			newx = wx;
		else if(w == nodes[n_nodes-1] && v != nodes[0])
			newx = vx;
		else	newx = e->weight > f->weight ? vx :
				(f->weight > e->weight ? wx : (vx+wx+1)/2);
		for(int i = 0; i < n_nodes; ++i)
			nodepos[nodes[i]] = newx;
	}
}


// define median pointers to help medianpos().
// The observation here is that since the relative order of
// adjacent nodes of a node in the next or last level is always
// maintained, the median node is known regardless of its position.
// So we compute it once and avoid doing qsorts during position
// reassignment.

static void define_median(PAIR *median, edge_t **edges)
{
	for(int i = 0; i < N_nodes; ++i)
	{
		int cnt = 0;
		MEDIAN *mp = Median;
		for(edge_t *e = edges[i]; e; e = e->next)
		{
			mp->weight = e->weight;
			mp->position = Invert[e->node];
			mp->node = e->node;
			++mp;
			++cnt;
		}
		if(cnt == 0)
			median[i].left = median[i].right = -1;
		else
		{
			PAIR p;
			wmedian(cnt,&p);
			median[i].left = Median[p.left].node;
			median[i].right = Median[p.right].node;
		}
	}
}

// compute starting positions
static void startpos(int *pos,	int **u_order, PAIR *u_median, int *u_prior,
				int **d_order, PAIR *d_median, int *d_prior)
{
	int maxrank = -1, maxwidth = 0;
	for(int i = 0; i <= Maxlevel; ++i)
	{
		int wlast = 0;
		int xlast = -Nodesep;
		for(int *rank = Rank[i]; *rank >= 0; ++rank)
		{
			int wnow = widthof(*rank);
			xlast += Nodesep+(wlast+wnow)/2;
			pos[*rank] = xlast;
			wlast = wnow;
		}
		if(xlast+wlast/2 > maxwidth)
		{
			maxrank = i;
			maxwidth = xlast+wlast/2;
		}
	}
	for(i = maxrank-1; i >= 0; --i)
		assignpos(pos,u_order[i],Rank[i],u_median,u_prior);
	for(i = maxrank+1; i <= Maxlevel; ++i)
		assignpos(pos,d_order[i],Rank[i],d_median,d_prior);
}

// define orders
static void define_order(int *u_prior, int **u_order, int *d_prior, int **d_order)
{
	for(int i = 0; i <= Maxlevel; ++i)
	{
		int n_rank = N_level[i];
		int *rank = Rank[i];
		int *up = u_order[i];
		int *down = d_order[i];

		for(int k = n_rank; k >= 0; --k)
			up[k] = down[k] = rank[k];
		for(k = 0; rank[k] != -1;)
		{
			for(int ek = k+1; rank[ek] != -1; ++ek)
				if(Component[rank[ek]] != Component[rank[ek-1]])
					break;
			Priority = u_prior;
			qsort((char*)(up+k),ek-k,sizeof(up[0]),(qsortcmpfun)prioritycmp);
			Priority = d_prior;
			qsort((char*)(down+k),ek-k,sizeof(down[0]),(qsortcmpfun)prioritycmp);

			k = ek;
		}
		for(k = 0; rank[k] != -1; ++k, --n_rank)
		{
			u_prior[up[k]] = n_rank;
			d_prior[down[k]] = n_rank;
		}
	}
}

static void goodposition()
{
	const int Maxiter = 8, Minquit = 4;

	// set priorities of nodes
	int *d_prior = new int[N_nodes];
	int *u_prior = new int[N_nodes];
	for(int i = 0; i < N_nodes; ++i)
	{
		d_prior[i] = u_prior[i] = 0;
		for(edge_t *e = Inedges[i]; e; e = e->next)
			d_prior[i] += (e->weight *= edgefactor(e->node,i));
		for(e = Outedges[i]; e; e = e->next)
			u_prior[i] += (e->weight *= edgefactor(i,e->node));
	}

	// construct the order in which positions are assigned to nodes
	int **u_order = new int*[Maxlevel+1];
	int **d_order = new int*[Maxlevel+1];
	for(i = 0; i <= Maxlevel; ++i)
	{
		u_order[i] = new int[N_level[i]+1];
		d_order[i] = new int[N_level[i]+1];
	}
	define_order(u_prior,u_order,d_prior,d_order);

	// compute the median pointers
	Median = new MEDIAN[N_nodes];
	int *nodepos = new int[N_nodes];
	PAIR *u_median = new PAIR[N_nodes];
	PAIR *d_median = new PAIR[N_nodes];
	define_median(d_median,Inedges);
	define_median(u_median,Outedges);

	// temp space for graph search
	int *marked = new int[N_nodes];
	int *queue = new int[N_nodes+1];
	int *nodes = new int[N_nodes];
	int maxwidth = 0;
	for(i = 0; i < N_nodes; ++i)
	{
		nodes[i] = i;
		maxwidth = max(maxwidth,widthof(i));
	}

	long bestlength = -1;
	int improved = 0;
	for(int upfirst = 1; upfirst >= 0; --upfirst)
	{
		// compute starting positions
		startpos(nodepos,u_order,u_median,u_prior,d_order,d_median,d_prior);
		int counter = 1;
		for(int iter = 0; iter < Maxiter; ++iter)
		{
			if(upfirst)
			{
				if(iter%2 == 0)
					medianpos(nodepos,u_order,u_prior,u_median,0);
				else	medianpos(nodepos,d_order,d_prior,d_median,1);
			}
			else
			{
				if(iter%2 == 0)
					medianpos(nodepos,d_order,d_prior,d_median,1);
				else	medianpos(nodepos,u_order,u_prior,u_median,0);
			}

			// local optimization
			minedge(nodepos);
			minnode(nodepos,marked,queue);
			minpath(nodepos,queue);
			verticalcut(nodepos,nodes,maxwidth);

			long curlength = 0;
			for(i = 0; i < Maxlevel; ++i)
				curlength += rlength(nodepos,Rank[i]);
			if(bestlength < 0 || curlength < bestlength)
			{
				bestlength = curlength;
				for(i = 0; i < N_nodes; ++i)
					Nodepos[i] = nodepos[i];
				improved++;
				counter = 1;
			}
			else if(counter++ > Minquit)
				break;
		}
	}

	mincut(Nodepos,marked,queue,nodes,1);
	mincut(Nodepos,marked,queue,nodes,-1);
	minedge(Nodepos);
	minnode(Nodepos,marked,queue);
	minpath(Nodepos,queue);

	// reclaim space
	for(i = 0; i <= Maxlevel; ++i)
	{
		delete u_order[i];
		delete d_order[i];
	}
	delete u_order;
	delete d_order;
	delete u_prior;
	delete d_prior;
	delete u_median;
	delete d_median;
	delete marked;
	delete queue;
	delete nodes;
	delete nodepos;
	delete Median;
}
