#include	"draw_dag.h"

#include	<sys/types.h>
#include	<sys/times.h>
#define TIC 60

/*
	Create an ordering of nodes on each level.
	The ordering attempts to minimize edge crossing
*/
static float	Convergence = .005;	// convergence rate
static int	Maxiter = 24;		// max number of iterations
static int	Minquit = 10;		// quit after this many iterations
					// if not improved
static int	**List;		// temp space for rank construction
static int	*Count,		// to aid rcross()	
		*Cross,		// current numbers of crossings between levels
		*Levelmarked;	// used in transpose()
static edge_t	**Flatedges;	// transitive closure of flat edges
static int	*Flatindeg;	// in-degree of nodes using Flatedges

static	long	*Median;
static int	*Medadj;

static void	decompose(),
		flatclosure(),
		removestem(),
		installstem(int**,int);
static int	cross_stat(int**), rcross(int*,int*),
		buildrank(int**,int**,int);
extern void	inversion(int**);

void dag_ranks()
{
	struct tms	begtm, endtm;
	int		startcross = 0;

	if(Verbose)
	{
#ifndef TIMING
		fprintf(stderr,"Minimize edge crossing\n");
#endif
		times(&begtm);
	}
#ifdef TIMING
	Maxiter = 20;
	Minquit = 20;
	Convergence = .0001;
#else
	// reset parameters for speed
	if(N_real_nodes > 500)
	{
		float density = ((float) N_edges)/N_nodes;
		if(density >= 2.)
		{
			Maxiter = N_real_nodes <= 1000 ? 8 : 4;
			Minquit = N_real_nodes <= 1000 ? 4 : 2;
			Convergence = N_real_nodes <= 1000 ? .01 : .1;
		}
	}
#endif

	// number of nodes in each level
	N_level = new int[Maxlevel+1];
	for(int i = 0; i < N_nodes; i++)
		N_level[Level[i]] += 1;

	// create space for node lists
	List = new int*[Maxlevel+1];
	Rank = new int*[Maxlevel+1];
	int **tryout = new int*[Maxlevel+1];
	int **target = new int*[Maxlevel+1];
	for(i = 0; i <= Maxlevel; ++i)
	{
		List[i] = new int[N_level[i]+1];
		tryout[i] = new int[N_level[i]+1];
		Rank[i] = new int[N_level[i]+1];
		Rank[i][0] = -1;
	}

	// space for intermediate calculations
	Invert = new int[N_nodes];
	Count = new int[N_nodes];
	Cross = new int[Maxlevel];
	Levelmarked = new int[Maxlevel+1];
	Median = new long[N_nodes];
	Medadj = new int[N_nodes+N_repeat_edges];

	// compute the transitive closure of the set of flat edges
	if(N_flat_edges > 0)
	{
		Flatedges = new edge_t*[N_real_nodes];
		Flatindeg = new int[N_real_nodes];
		flatclosure();
	}

	// detect connected components
	Component = new int[N_nodes];
	decompose();

	// build up the ordering component by component
	for(int component = 1; component <= N_component; ++component)
	{
		for(i = 0; i <= Maxlevel; ++i)
		{
			for(int k = 0; ; ++k)
				if(Rank[i][k] == -1)
					break;
			target[i] = Rank[i]+k;
		}
		startcross += buildrank(target,tryout,component);
	}
	inversion(Rank);

	if(Verbose)
	{
		times(&endtm);
		int total = (endtm.tms_utime - begtm.tms_utime)+
			    (endtm.tms_stime - begtm.tms_stime);
		int percent = (total - (total/TIC)*TIC)*100/TIC;
		int bestcross = cross_stat(Rank);
#ifdef TIMING
		fprintf(stderr,"%d\t%d\t%d.%02d\t",
			startcross,bestcross,total/TIC,percent);
#else
		fprintf(stderr,"Starting crossing number = %d\n",startcross);
		fprintf(stderr,"Crossings between ranks:\n");
		for(i = 0; i < Maxlevel; ++i)
			fprintf(stderr,"(%2d,%2d):\tx=%d\n",i,i+1,Cross[i]);
		fprintf(stderr,"Ending crossing number = %d\n",bestcross);
		fprintf(stderr,"Time in minimizing crossing: %d.%02ds\n",
				total/TIC, percent);
#endif
	}

	// release temp space
	delete Median;
	delete Medadj;
	delete Levelmarked;
	delete Cross;
	delete Count;
	for(i = 0; i <= Maxlevel; ++i)
	{
		delete List[i];
		delete tryout[i];
	}
	delete List;
	delete tryout;
	delete target;
	if(N_flat_edges > 0)
		deledges(N_real_nodes,Flatedges);
}

// compute the transitive closure of flat edges
// and remove resulting bidirectional edges.
// The final graph defines a partial order on the nodes.
static void flatedges(int root, int here, int *marked)
{
	marked[here] = 1;

	for(edge_t *e = Noedges[here]; e; e = e->next)
	if(e->weight > 0 && !marked[e->node])
	{
		// check for bi-directional edges
		edge_t *lastf = NULL, *f = Flatedges[e->node];
		for(; f != NULL; lastf = f, f = f->next)
			if(f->node == root)
				break;

		// remove bidirectional edges
		if(f != NULL)
		{
			if(lastf)
				lastf->next = f->next;
			else	Flatedges[e->node] = f->next;
			delete f;
		}
		// add a new edge
		else	Flatedges[root] =
				new edge_t(e->node,0,0,0,Flatedges[root]);

		flatedges(root,e->node,marked);
	}
}

static void flatclosure()
{
	int *marked = Count;
	for(int v = 0; v < N_real_nodes; ++v)
	{
		for(int i = 0; i < N_real_nodes; ++i)
			marked[i] = 0;
		flatedges(v,v,marked);
	}

	for(v = 0; v < N_real_nodes; ++v)
		for(edge_t *e = Flatedges[v]; e; e = e->next)
			Flatindeg[e->node]++;
}

// assign component numbers to nodes so that nodes in the same
// connected component get the same number.
static void decompose()
{
	// make a reverse flat edge list
	edge_t **noedges = NULL;
	if(N_flat_edges > 0)
	{
		noedges = new edge_t*[N_real_nodes];
		for(int v = 0; v < N_real_nodes; ++v)
		for(edge_t *e = Noedges[v]; e; e = e->next)
		{
			int w = e->node;
			if(w != v)
				noedges[w] = new edge_t(v,0,0,0,noedges[w]);
		}
	}

	// all adjacent edge lists
	edge_t **adjlist[5];
	adjlist[0] = Outedges;
	adjlist[1] = Inedges;
	adjlist[2] = N_flat_edges > 0 ? Noedges : NULL;
	adjlist[3] = N_flat_edges > 0 ? noedges : NULL;
	adjlist[4] = NULL;

	// space for the breadth-first search
	int *queue = new int[N_nodes];

	N_component = 0;
	for(int n = 0; n < N_nodes; ++n)
	{
		if(Component[n] > 0)
			continue;

		// initialize for component search
		N_component += 1;
		Component[n] = N_component;

		// breadth-first search
		int first = 0, last = 1;
		queue[0] = n;
		while(first < last)
		{
			int v = queue[first++];
			for(int i = 0; adjlist[i] != NULL; ++i)
			{
				if(v >= N_real_nodes && i >= 2)
					continue;
				for(edge_t *e = adjlist[i][v]; e; e = e->next)
				{
					int w = e->node;
					if(Component[w] != 0)
						continue;
					Component[w] = N_component;
					queue[last++] = w;
				}
			}
		}
	}

	delete queue;
	if(N_flat_edges > 0)
		deledges(N_real_nodes,noedges);
}

// copy a ranking to another
static void copyrank(int **from, int **to)
{
	for(int i = 0; i <= Maxlevel; ++i)
	{
		register int *frp = from[i], *top = to[i];
		while(*frp != -1)
			*top++ = *frp++;
		*top = -1;
	}
}

// see if one vertex must be left of another
static int left2right(int v, int w)
{
	if(v >= N_real_nodes || w >= N_real_nodes)
		return 0;

	for(edge_t *e = Flatedges[v]; e; e = e->next)
		if(e->node == w)
			return 1;
	return 0;
}

// construct nodes reachable from 'here' in post-order.
// This is the same as doing a topological sort in reverse order.
static void postorder(int here,int *marked,int *list, int &n_search)
{
	marked[here] = 1;
	for(edge_t *e = Flatedges[here]; e; e = e->next)
		if(!marked[e->node])
			postorder(e->node,marked,list,n_search);
	list[n_search++] = here;
}

static void reorder(int *list, int *marked, int *temp)
{
	for(int i = 0; list[i] >= 0; ++i)
		marked[list[i]] = 0;

	int n_temp = 0;
	for(i = 0; list[i] >= 0; ++i)
	{
		// dummy nodes
		if(list[i] >= N_real_nodes)
			temp[n_temp++] = list[i];
		// real nodes
		else if(!marked[list[i]] && Flatindeg[list[i]] == 0)
		{
			int n_search = 0;
			postorder(list[i],marked,temp+n_temp,n_search);
			int *left = temp+n_temp, *right = left+n_search-1;
			for(; left < right; ++left, --right)
			{
				int t = *left;
				*left = *right;
				*right = t;
			}
			n_temp += n_search;
		}
	}

	// recopy the list
	for(i = 0; list[i] >= 0; ++i)
		list[i] = temp[i];
}

// Compute an initial ordering of nodes belonging to some connected component.
static void initrank(int *rank[], int component, int isdown)
{
	// use breadth-first search to build the list
	int *marked = new int[N_nodes];
	int *queue = new int[N_nodes];
	int *n_lev = new int[Maxlevel+1];

	// now do the rank partitioning by components
	edge_t **those = isdown ? Inedges : Outedges;
	edge_t **these = isdown ? Outedges : Inedges;
	for(int n = 0; n < N_nodes; ++n)
	{
		if(marked[n] || those[n] || Component[n] != component ||
		   (n < N_real_nodes && N_flat_edges <= 0 && Stems[n]))
			continue;

		marked[n] = 1;
		queue[0] = n;
		int first = 0, last = 1;
		while(first < last)
		{
			int node = queue[first++];
			rank[Level[node]][n_lev[Level[node]]++] = node;
			for(edge_t *e = these[node]; e; e = e->next)
			{
				if(!marked[e->node])
				{
					marked[e->node] = 1;
					queue[last++] = e->node;
				}
			}
		}
	}

	for(n = 0; n <= Maxlevel; ++n)
	{
		rank[n][n_lev[n]] = -1;

		// make sure that implicit left-to-right orders are satisfied
		if(N_flat_edges > 0)
			reorder(rank[n],marked,queue);
	}

	delete marked;
	delete queue;
	delete n_lev;
}

// Compute the crossing of two levels.
// We'll assume that edges point from top to bot.
static int rcross(int *top, int *bot)
{
	// clear up work space
	for (int i = 0; bot[i] != -1; i++)
		Count[i] = 0;

	register int cross = 0;	// the crossing count
	int max = 0;	// index we have to sum to during counting
	for (i = 0; top[i] != -1; i++)
	{
		register edge_t *e;

		if (max > 0)
		{
			/* iterate over each edge */
			for (e = Outedges[top[i]]; e; e = e->next)
				for(int k = Invert[e->node]+1; k <= max; k++)
					cross += Count[k]*e->count;
		}

		/* update the counts */
		for (e = Outedges[top[i]]; e; e = e->next)
		{
			register int inv = Invert[e->node];
			if (inv > max)
				max = inv;
			Count[inv] += e->count;
		}
	}
	return cross;
}

// compute the inverted indices of all vertices in their rank lists
// and the crossing statistics between ranks.
static void inversion(int **ranks)
{
	for(int i = 0; i <= Maxlevel; ++i)
	{
		int *rank = ranks[i];
		for(int k = 0; rank[k] != -1; ++k)
			Invert[rank[k]] = k;
	}
}

static int cross_stat(int **ranks)
{
	int cross = 0;
	for(int i = 0; i < Maxlevel; ++i)
		cross += (Cross[i] = rcross(ranks[i],ranks[i+1]));
	return cross;
}

// Compute the crossing of incident edges of two vertices
static int vcross(edge_t *adj1, edge_t *adj2)
{
	register int cross = 0;
	for (register edge_t *e2 = adj2; e2; e2 = e2->next)
	{
		register int inv = Invert[e2->node];
		register int cnt = e2->count;
		for(register edge_t *e1 = adj1; e1; e1 = e1->next)
			if(Invert[e1->node] > inv)
				cross += e1->count * cnt;
	}
	return cross;
}

// Reduce crossings by adjacent tranpositions of vertices
static void transpose(int **ranks, int revert)
{
	// current number of crossings
	int curcross = 0;
	for(int l = 0; l < Maxlevel; ++l)
	{
		curcross += Cross[l];
		Levelmarked[l] = 1;
	}
	if(curcross == 0)
		return;
	else	Levelmarked[l] = 1;

	while(1)
	{
		int	delta = 0;
		for (int lev = 0; lev <= Maxlevel; lev++)
		if(Levelmarked[lev])
		{
			int *rank = ranks[lev];
			if(rank[0] == -1 || rank[1] == -1)
			{
				Levelmarked[lev] = 0;
				continue;
			}
			for(; rank[1] != -1; ++rank)
			{
				register int c0 = 0, c1 = 0;
				int v = rank[0], w = rank[1];

				if(N_flat_edges > 0 && left2right(v,w) != 0)
					continue;

				if(lev > 0)
				{
					edge_t *vadj = Inedges[v], *wadj = Inedges[w];
					c0 += vcross(vadj,wadj);
					c1 += vcross(wadj,vadj);
				}
				if(lev < Maxlevel)
				{
					edge_t *vadj = Outedges[v], *wadj = Outedges[w];
					c0 += vcross(vadj,wadj);
					c1 += vcross(wadj,vadj);
				}

				if (c1 < c0 || (c1 > 0 && c1 == c0 && revert))
				{
					Levelmarked[lev] = 2;
					Invert[v]++;
					Invert[w]--;
					rank[0] = w;
					rank[1] = v;
					delta += c0-c1;
				}
			}

			if(Levelmarked[lev] == 2)
			{
				Levelmarked[lev] = 1;
				if(lev > 0)
				{
					Levelmarked[lev-1] = 1;
					Cross[lev-1] = -1;
				}
				if(lev < Maxlevel)
				{
					Levelmarked[lev+1] = 1;
					Cross[lev] = -1;
				}
			}
			else	Levelmarked[lev] = 0;
		}

		curcross -= delta;
		float reduce = curcross <= 0 ? 0. : ((float)delta)/curcross;
		if(curcross == 0 || reduce <= Convergence)
			break;
	}

	// restore crossing statistics
	for(l = 0; l < Maxlevel; ++l)
		if(Cross[l] < 0)
			Cross[l] = rcross(ranks[l],ranks[l+1]);
}

// sort nodes by their median values.
// This is basically a bubble sort that respects fixed points
static int mediansort(int *array, int nelt, int revert, int hasfixed)
{
	int changed = 0;

	register int *ep = array+nelt, *lp, *rp;
	for(nelt -= 1; nelt > 0; --nelt)
	{
		lp = array;
		while(lp < ep)
		{
			// -1 nodes are FIXED points
			if(Median[*lp] < 0)
				for(; lp < ep; lp++)
					if(Median[*lp] >= 0)
						break;
			if(lp >= ep)
				break;

			// find the one that can be compared with
			int muststay = 0;
			for(rp = lp+1; rp < ep; ++rp)
			{
				if(N_flat_edges > 0 && left2right(*lp,*rp))
				{
					muststay = 1;
					break;
				}
				if(Median[*rp] >= 0)
					break;
			}
			if(rp >= ep)
				break;

			// switch if appropriate
			if(!muststay &&
			   (Median[*lp] > Median[*rp] ||
			    (Median[*lp] == Median[*rp] && revert)))
			{
				register int t = *lp;
				*lp = *rp;
				*rp = t;
				changed++;
			}
			lp = rp;
		}

		if(!hasfixed && !revert)
			--ep;
	}
	return changed;
}


// reduce crossing using weighted median sorting
static int intcmp(register int *i1, register int *i2)
{
	return *i1 - *i2;
}
static void median(int **ranks, int revert, int isup)
{
#define SCALE	16
	for (int lev = 1; lev <= Maxlevel; lev++)
	{
		int	thislev = isup ? Maxlevel-lev : lev;
		int	thatlev = isup ? thislev+1 : thislev-1;
		edge_t	**edges = isup ? Outedges : Inedges;
		int	*thislist = ranks[thislev];
		int	*thatlist = ranks[thatlev];

		// compute medians of thislist
		int hasfixed = 0;
		for(int n_this = 0; thislist[n_this] != -1; n_this++)
		{
			register int node = thislist[n_this];
			register int *endadj = Medadj;
			for(register edge_t *e = edges[node]; e; e = e->next)
				for(int k = e->count; k > 0; --k)
					*endadj++ = Invert[e->node];
			register int cnt = endadj-Medadj;
#ifdef BCENTER
	// barycenter method
			if(cnt > 0)
			{
				int ave = 0;
				for(int m = 0; m < cnt; ++m)
					ave += Medadj[m];
				Median[node] = (SCALE*ave)/cnt;
			}
			else
			{
				Median[node] = -1;
				hasfixed = 1;
			}
#else	/* a right median method */
#ifdef RMEDIAN
			if(cnt > 1)
				qsort((char*)Medadj,cnt,sizeof(Medadj[0]),(qsortcmpfun)intcmp);
			if(cnt == 0)
			{
				Median[node] = -1;
				hasfixed = 1;
			}
			else	Median[node] = SCALE*Medadj[cnt/2];
#else	/* weighted median */
			switch(cnt)
			{
			case 0 :
				Median[node] = -1;
				hasfixed = 1;
				break;
			case 1 :
				Median[node] = SCALE*Medadj[0];
				break;
			case 2 :
				Median[node] = (Medadj[0]+Medadj[1])*(SCALE/2);
				break;
			default :
				qsort((char*)Medadj,cnt,sizeof(Medadj[0]),(qsortcmpfun)intcmp);
				if(cnt%2)
				{
					Median[node] = SCALE*Medadj[cnt/2];
					break;
				}

				register int r = cnt/2;
				register int l = r-1;

				// compute left/right spans
				int lspan = 0, rspan = 0;
				int curinv = Medadj[l];
				int nextinv;
				for(k = l-1; k >= 0; --k)
				{
					nextinv = Medadj[k];
					lspan += curinv-nextinv;
					curinv = nextinv;
				}
				curinv = Medadj[r];
				for(k = r+1; k < cnt; ++k)
				{
					nextinv = Medadj[k];
					rspan += nextinv-curinv;
					curinv = nextinv;
				}
				if(lspan == rspan)
					Median[node] = (Medadj[l]+Medadj[r])*(SCALE/2);
				else
				{
					int w = Medadj[l]*rspan+ Medadj[r]*lspan;
					Median[node] = (w*SCALE)/(lspan+rspan);
				}
				break;
			}
#endif	/* RMEDIAN */
#endif	/* BCENTER */
		}

		// sort by medians
		if(mediansort(thislist,n_this,revert,hasfixed))
		{
			for(int i = 0; *thislist != -1; ++i)
				Invert[*thislist++] = i;
			if(thislev > 0)
				Cross[thislev-1] = -1;
			if(thislev < Maxlevel)
				Cross[thislev] = -1;
		}
	}
	for(lev = 0; lev < Maxlevel; ++lev)
		if(Cross[lev] < 0)
			Cross[lev] = rcross(ranks[lev],ranks[lev+1]);
}

// minimizing crossing using the median and transpose heuristics
static int mincross(int **ranks, int bestcross)
{
	// copy current rank to List to start the process 
	copyrank(ranks,List);

	inversion(List);
	int curcross = cross_stat(List);
	if(curcross < bestcross)
		bestcross = curcross;

	int counter = 1;
	for (int iter = 0; iter < Maxiter; iter++)
	{
		median(List,iter%4 < 2 ? 1 : 0,iter%2 ? 1 : 0);
#ifndef NOTRANSPOSE
		transpose(List,iter%4 < 2 ? 0 : 1);
#endif
		curcross = 0;
		for(int i = 0; i < Maxlevel; ++i)
			curcross += Cross[i];
#ifdef BIGGRAPH
		if(Verbose)
			fprintf(stderr,"Iteration %d, curcross=%d bestcross=%d\n",
				iter,curcross,bestcross);
#endif
		if(curcross < bestcross)
		{
			float reduce = curcross <= 0 ? 0. :
				 ((float)(bestcross-curcross))/curcross;

			copyrank(List,ranks);
			bestcross = curcross;

			if(bestcross == 0 || reduce < Convergence)
				break;

			counter = 1;
		}
		else
		{
			if(counter > Minquit)
				break;
			++counter;
		}
	}

	return bestcross;
}

// compute edge crossings
static int scross(int iw, int count, edge_t *adj, int dir)
{
	int cross = 0;
	if(dir > 0)
	{
		for(register edge_t *e = adj; e; e = e->next)
			if(Invert[e->node] < iw)
				cross += e->count*count;
	}
	else
	{
		for(register edge_t *e = adj; e; e = e->next)
			if(Invert[e->node] > iw)
				cross += e->count*count;
	}
	return cross;
}

// put stems in place where they'll be close to their adjacent nodes
static void fixstem(int **ranks, int *stem)
{
	for(int dir = 1;  dir >= -1; dir -= 2)
	for(int l = dir > 0 ? 1 : Maxlevel-1; l >= 0 && l <= Maxlevel; l += dir)
	{
		edge_t **these = dir > 0 ? Inedges : Outedges;
		edge_t **those = dir > 0 ? Outedges : Inedges;
		int *rank = ranks[l]; 

		// count the list size
		int n_rank = 0;
		int n_stem = 0;
		for(int k = 0; rank[k] >= 0; ++k, ++n_rank)
		{
			int v = rank[k];
			if(these[v] && !these[v]->next && !those[v])
				stem[n_stem++] = v;
		}
		if(n_stem <= 0)
			continue;

		for(k = 0; k < n_stem; ++k)
		{
			int v = stem[k], iv = Invert[v];
			int w = these[v]->node, iw = Invert[w];
			int count = these[v]->count;

			// find the left and right medians and bounds
			int n = 0;
			for(edge_t *e = those[w]; e; e = e->next)
				if(e->node != v)
					Medadj[n++] = Invert[e->node];
			if(n == 0)
				continue;
			if(n > 2)
				qsort((char*)Medadj,n,sizeof(Medadj[0]),(qsortcmpfun)intcmp);
			int rmed = Medadj[n/2];
			int lmed = n%2 ? rmed : Medadj[n/2-1];

			// check each position
			int best = iv, cross = 0;
			int dist = iabs(2*iv - (rmed+lmed));
			for(int d = -1; d <= 1; d += 2)
			{
				int c = 0, lm = lmed, rm = rmed;
				for(int p = iv+d; p >= 0 && p < n_rank; p += d)
				{
					int x = rank[p];
					if(p == lm)
						lm -= d;
					if(p == rm)
						rm -= d;

					int c0 = scross(iw,count,these[x],d);
					int c1 = scross(iw,count,these[x],-d);
					c += c1-c0;
					if(c == cross)
					{
						int di = iabs(2*p -(lm+rm));
						if(di < dist)
						{
							best = p;
							dist = di;
						}
					}
					else if(c < cross)
					{
						cross = c;
						best = p;
						dist = iabs(2*p - (lm+rm));
					}
				}
			}
			if(best == iv)
				continue;

			// move v to its best place
			d = best < iv ? -1 : 1;
			for(register int p = iv; p != best; p += d)
			{
				rank[p] = rank[p+d];
				Invert[rank[p]] = p;
			}
			rank[p] = v;
			Invert[v] = p;

		}
	}
}

// make node orderings for a connected component
static int buildrank(int **target, int **tryout, int component)
{
	// build order using downward depth-first search
	int start_target, start_tryout, bestcross, curcross;
	initrank(target,component,1);
	inversion(target);
	start_target = bestcross = cross_stat(target);
	start_tryout = 0;

	if(bestcross > 0)
	{
#ifndef NOTRANSPOSE
		transpose(target,0);
		bestcross = 0;
		for(int i = 0; i < Maxlevel; ++i)
			bestcross += Cross[i];
#endif
	}

	// rebuild order using upward depth-first search
	if(bestcross > 0)
	{
		initrank(tryout,component,0);
		inversion(tryout);
		start_tryout = curcross = cross_stat(tryout);

		if(curcross > 0)
		{
#ifndef NOTRANSPOSE
			transpose(tryout,0);
			curcross = 0;
			for(int i = 0; i < Maxlevel; ++i)
				curcross += Cross[i];
#endif
		}
		if(curcross == 0)
		{
			copyrank(tryout,target);
			bestcross = curcross;
		}
	}

	// iterations to improve crossing
	if(bestcross > 0)
		bestcross = mincross(target,Maxint);
	if(bestcross > 0 && (curcross = mincross(tryout,bestcross)) < bestcross)
		copyrank(tryout,target);

	// fix dangling stems
	if(N_flat_edges <= 0)
	{
		inversion(target);
		fixstem(target,Count);
	}

	return max(start_target,start_tryout);
}
