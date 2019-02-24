#include	"draw_dag.h"


/*
	Routines to start and end draw_dag.
	In the starting phase, make in/out edge lists with no self-loops,
	multiple edges and cycles. This phase also makes sure that 
	constraints of the form 'this set of nodes must be at the same level'
	can be later satisfied.
*/
static int	Clean_up;	// 1 if things need cleaned up before starting

static int	multiple(int, edge_t*, int),
		findroot(int);
static void	dfs(int, edge_t**, edge_t**, int*, int*, int**),
		setunion(int*);



void dag_start(int n_nodes, node_t **nodes, edge_t **outedges, options_t options)
{
	// clean up previous space
	if(Clean_up)
	{
		if(Verbose)
			fprintf(stderr,"Clean up space from last call\n");
		Clean_up = 0;

		for(int v = 0; v < N_real_nodes; v++)
		{
			for(edge_t *e = Outedges[v]; e; e = e->next)
				if(e->splinept)
					delete e->splinept;
			if(Noedges)
				for(e = Noedges[v]; e; e = e->next)
					if(e->splinept)
						delete e->splinept;
		}

		deledges(N_nodes,Inedges);
		deledges(N_nodes,Outedges);
		if(Noedges)
			deledges(N_real_nodes,Noedges);
		Outedges = Inedges = Noedges = NULL;
		if(Istems)
		{
			delete Istems;
			delete Ostems;
			delete Stems;
			Istems = Ostems = NULL;
			Stems = NULL;
		}

		for(v = 0; v <= Maxlevel; v++)
			delete Rank[v];
		delete Rank; Rank = NULL;

		delete Root; Root = NULL;
		delete Level; Level = NULL;
		delete N_level; N_level = NULL;
		delete Width; Width = NULL;
		delete Height; Height = NULL;
		delete Levelheight; Levelheight = NULL;
		delete Levelpos; Levelpos = NULL;
		delete Nodepos; Nodepos = NULL;
		delete Deleted; Deleted = NULL;
	}

	if (n_nodes <= 0)
		return;

	// copy node Widths/Heights
	Width = new int[n_nodes];
	Height = new int[n_nodes];
	for(int n = 0; n < n_nodes; ++n) {
		Width[n] = nodes[n]->width;
		Height[n] = nodes[n]->height;
	}
	Nodesep = options.nodesep;
	Levelsep = options.ranksep;
	Xbound = options.xbound;
	Ybound = options.ybound;

	// construct equivalence class indices for vertices at same levels
	int *srcs = options.source_nodes;
	int *sinks = options.sink_nodes;
	int **same = options.same_nodes;
	Root = new int[n_nodes];
	for(n = 0; n < n_nodes; ++n)
		Root[n] = n;
	if(srcs && srcs[0] != -1)
		setunion(srcs);
	if(sinks && sinks[0] != -1)
		setunion(sinks);
	if(same)
		for(n = 0; same[n]; ++n)
			if(same[n][0] != -1)
				setunion(same[n]);
	// compress union trees
	for(n = 0; n < n_nodes; ++n)
		(void)findroot(n);

	// construct auxilliary lists of edges turned around by min/max nodes
	int source = (srcs && srcs[0] != -1) ? Root[srcs[0]] : -1;
	int sink = (sinks && sinks[0] != -1) ? Root[sinks[0]] : -1;
	if(sink == source)
		sink = -1;
	edge_t **aux = (edge_t **)0;
	if(source >= 0 || sink >= 0)
	{
		aux = new edge_t*[n_nodes];
		for(n = 0; n < n_nodes; ++n)
		{
			if(Root[n] != source)
			{
				for(edge_t *e = outedges[n]; e; e = e->next)
				if(Root[e->node] == source)
				{
					aux[e->node] = new edge_t(n,e->weight,
						e->width,e->minlen,aux[e->node]);
					aux[e->node]->chain = e;
					e->chain = aux[e->node];
				}
			}

			if(Root[n] == sink)
			{
				for(edge_t *e = outedges[n]; e; e = e->next)
				if(Root[e->node] != sink)
				{
					aux[e->node] = new edge_t(n,e->weight,
						e->width,e->minlen,aux[e->node]);
					aux[e->node]->chain = e;
					e->chain = aux[e->node];
				}
			}
		}
	}

	// make space for edges
	N_edges = N_self_edges = N_repeat_edges = N_revert_edges = 0;
	N_nodes = N_real_nodes = n_nodes;
	Inedges = new edge_t*[n_nodes];
	Outedges = new edge_t*[n_nodes];

	// use depth-first search to construct edge lists
	int *active = new int[n_nodes];
	int *marked = new int[n_nodes];

	// compute the squashed trees of equi-level nodes
	int **sets = new int*[N_nodes];
	for(n = 0; n < N_nodes; ++n)
		active[Root[n]]++;
	for(n = 0; n < N_nodes; ++n)
		if(active[n] > 0) {
			sets[n] = new int[active[n]+1];
			sets[n][active[n]] = -1;
			active[n] = 0;
		}
	for(n = 0; n < N_nodes; ++n)
		sets[Root[n]][active[Root[n]]++] = n;
	for(n = 0; n < N_nodes; ++n)
		active[n] = 0;

	for(n = 0; n < n_nodes; ++n)
		if(n == Root[n] && !marked[n])
			dfs(n,outedges,aux,active,marked,sets);
	delete active;
	delete marked;
	for(n = 0; n < n_nodes; ++n)
		if(sets[n])
			delete sets[n];
	delete sets;

	// restore space used by auxilliary edges
	if(source >= 0 || sink >= 0)
	{
		for(n = 0; n < n_nodes; ++n)
		{
			edge_t *nexte;
			for(edge_t *e = aux[n]; e; e = nexte)
			{
				nexte = e->next;
				delete e;
			}
		}
		delete aux;
	}

	if (Verbose)
	{
#ifdef TIMING
		fprintf(stderr,"%d\t%d\t",N_nodes,N_edges);
#else
		fprintf(stderr,"N_nodes = %d, N_edges=%d\n",N_nodes,N_edges);
		fprintf(stderr,"N_self_edges = %d\n", N_self_edges);
		fprintf(stderr,"N_flat_edges = %d\n", N_flat_edges);
		fprintf(stderr,"N_repeat_edges = %d\n", N_repeat_edges);
		fprintf(stderr,"N_revert_edges = %d\n", N_revert_edges);
#endif
	}
}


void dag_end(node_t **nodes, edge_t **outedges)
{
	// output node positions
	for(int v = 0; v < N_real_nodes; v++)
	{
		nodes[v]->pos.x = Nodepos[v];
		nodes[v]->pos.y = Levelpos[Level[v]];
	}

	// output edge splines
	for(v = 0; v < N_real_nodes; ++v)
	for(edge_t *e = outedges[v]; e; e = e->next)
	{
		// copy the points
		Point *sp = e->chain->splinept;
		for(int n_points = 0; sp[n_points].x >= 0; ++n_points)
			;

		Point *pt;
		if(e->chain->count > 1)
		{
			pt = new Point[n_points+1];
			for(int n = 0; n <= n_points; ++n)
			{
				pt[n].x = sp[n].x;
				pt[n].y = sp[n].y;
			}
		}
		else
		{
			pt = sp;
			e->chain->splinept = NULL;
		}

		// reverse the order of inverted edges
		if(Level[v] > Level[e->node])
		{
			int n = 0, m = n_points-1;
			for(; n < m; ++n, --m)
			{
				swap(pt[n].x,pt[m].x);
				swap(pt[n].y,pt[m].y);
			}
		}

		e->top.x = pt[1].x;
		e->top.y = pt[1].y;
		e->bottom.x = pt[n_points-2].x;
		e->bottom.y = pt[n_points-2].y;

		// adjust for multiple edges
		if(e->chain->count > 1)
		{
			int place = e->place;
			int width = e->chain->width;
			if(v == e->node)
			{
				place -= width/2;
				pt[2].x += place;
				pt[1].y -= place/2;
				pt[3].y += place/2;
			}
			else if(Level[v] == Level[e->node])
			{
				if(e->chain->weight > 0)
					place = -place;
				int mx = iabs(pt[n_points/2].x - pt[0].x);
				for(int n = 1; n < n_points-1; ++n)
				{
					int d = iabs(pt[n].x - pt[0].x);
					d = min(d,iabs(pt[n].x - pt[n_points-1].x));
					pt[n].y += (int)(place*(((double)d)/mx));
				}
			}
			else
			{
				place -= width/2;
				for(int n = 1; n < n_points-1; ++n)
					pt[n].x += place;
			}
		}

		e->splinept = pt;
	}

	// so we'll remember to clean up next time around
	Clean_up = 1;
}


// search for the root of the union tree containing this vertex
static int findroot(int v)
{
	// if this is root, return it
	if(v == Root[v])
		return v;
	// search for root and compress the path
	return (Root[v] = findroot(Root[v]));
}

// union a set of nodes
static void setunion(int *set)
{
	int root = findroot(set[0]);
	for(int i = 0; set[i] != -1; ++i)
	{
		int r = findroot(set[i]);
		if(r == set[i])
			Root[set[i]] = root;
		else
		{
			Root[root] = r;
			root = r;
		}
	}
}



// check for multiple edges
static int multiple(int node, edge_t *edge, int isaux)
{
	for(int dohead = 0; dohead < 2; ++dohead)
	{
		/* check for forward multiple edges */
		edge_t *e = dohead ? Outedges[edge->node] : Outedges[node];
		int	match = dohead ? node : edge->node;

		for(; e; e = e->next)
		if(e->node == match)
		{
			e->count += 1;
			e->weight += edge->weight;
			e->width += 3*Nodesep/4 + edge->width;

			// identify user's edge with our edge and set its order
			if(isaux)
			{
				edge->chain->chain = e;
				edge->chain->place = e->width - edge->width;
			}
			else
			{
				edge->chain = e;
				edge->place = e->width - edge->width;
			}

			// update the corresponding version in Inedges
			e = e->link;
			e->count += 1;
			e->weight += edge->weight;
			e->width += Nodesep + edge->width;

			return 1;
		}
	}

	return 0;
} 


// keep track of self and flat edges for position assignment
static void noedge(int tail, edge_t *edge)
{
	if(!Noedges)
		Noedges = new edge_t*[N_real_nodes];

	// check for multiple edge
	for(edge_t *e = Noedges[tail]; e; e = e->next)
		if(e->node == edge->node)
		{
			e->count += 1;
			e->weight += edge->weight;
			e->width += Nodesep/2 + edge->width;
			edge->chain = e;
			edge->place = e->width - edge->width;
			return;
		}

	// making a new edge
	Noedges[tail] = new edge_t(edge->node,edge->weight,edge->width,0,Noedges[tail]);
	edge->chain = Noedges[tail];
}



// construct new edge lists ensuring no cycles, multiple edges and self edges.
static void dfs(int node, edge_t **outedges, edge_t **auxedges,
		 int *active, int *marked, int **sets)
{
	if(marked[node])
		return;
	marked[node] = 1;

	// mark this node as being on the search stack
	active[node] = 1;

	for(int *set = sets[node]; *set != -1; ++set)
	{
		int tail = *set;

		for(int isaux = 0; isaux < 2; ++isaux)
		{
			if(isaux && !auxedges)
				continue;

			edge_t *e = isaux ? auxedges[tail] : outedges[tail];
			for(; e; e = e->next)
			{
				// there is a corresponding aux edge
				if(!isaux && e->chain)
					continue;

				// ignore self-edges or squashed edges
				if(e->node == tail || Root[e->node] == node)
				{
					if(e->node == tail)
						N_self_edges++;
					else	N_flat_edges++;
					noedge(tail,e);
					continue;
				}

				// check for multiple edges
				if(multiple(tail,e,isaux))
				{
					N_repeat_edges++;
					continue;
				}

				// a new edge is to be constructed
				N_edges++;

				// a cycle is detected, reverse the edge
				if(active[Root[e->node]])
				{
					N_revert_edges++;

					Outedges[e->node] =
						new edge_t(tail,e->weight,e->width,e->minlen,
							   Outedges[e->node]);
					Inedges[tail] =
						new edge_t(e->node,e->weight,e->width,e->minlen,
							   Inedges[tail]);
					Outedges[e->node]->link = Inedges[tail];
					Inedges[tail]->link = Outedges[e->node];

					// identify user's edge with our edge
					if(isaux)
					{
						e->chain->chain = Outedges[e->node];
						e->chain->place = 0;
					}
					else
					{
						e->chain = Outedges[e->node];
						e->place = 0;
					}

					continue;
				}

				// forward or cross-edges
				Outedges[tail] = new edge_t(e->node,e->weight,e->width,e->minlen,
							Outedges[tail]);
				Inedges[e->node] = new edge_t(tail,e->weight,e->width,e->minlen,
							Inedges[e->node]);
				Outedges[tail]->link = Inedges[e->node];
				Inedges[e->node]->link = Outedges[tail];

				// identify user's edge with our edge
				if(isaux)
				{
					N_revert_edges++;
					e->chain->chain = Outedges[tail];
					e->chain->place = 0;
				}
				else
				{
					e->chain = Outedges[tail];
					e->place = 0;
				}

				// recursion
				if(!marked[Root[e->node]])
					dfs(Root[e->node],outedges,auxedges,
					    active,marked,sets);
			}
		}
	}

	// this node is now off the search stack
	active[node] = 0;
}
