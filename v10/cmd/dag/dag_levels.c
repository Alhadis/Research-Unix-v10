#include	"draw_dag.h"

#include	<sys/types.h>
#include	<sys/times.h>
#define TIC 60

/*
	Assign levels to nodes of an acyclic digraph.

	Definition: the length of an edge is the difference
		in the levels of its adjacent nodes.

	Optimization objective: minimize the total edge length
		of all edges.
*/
static void	setlevels(int,int), balance();

#ifdef DEBUG
static void	verifylevel(), verifytree(), verifycycle(), fatal(),
		checklevel(int*), checkcycle(), printtree(int);
#endif

void dag_levels(int source, int sink, int hasequi)
{
	struct tms begtm, endtm;

	if(Verbose)
	{
#ifndef TIMING
		fprintf(stderr,"Assign levels\n");
#endif
		times(&begtm);
	}

#ifdef DEBUG
	verifycycle();
#endif

	// space to store level assignments
	Level = new int[N_nodes];

	// solve associated lp problems
	setlevels(source,sink);
#ifdef DEBUG
	verifylevel();
#endif

	// set levels for stems
	if(N_flat_edges <= 0)
	{
		for(int in = 0; in < 2; ++in)
		{
			edge_t **edges = in ? Istems : Ostems;
			for(int v = 0; v < N_nodes; ++v)
			{
				if(!edges[v])
					continue;
				int lev = Level[v] + (in ? -1 : 1);
				for(edge_t *e = edges[v]; e; e = e->next)
					Level[e->node] = lev;
			}
		}
	}

	// compute Maxlevel
	Maxlevel = 0;
	for(int v = 0; v < N_nodes; v++)
		if(Level[v] > Maxlevel)
			Maxlevel = Level[v];

	// balance lists of nodes
	if(!hasequi)
		balance();
#ifdef DEBUG
	verifylevel();
#endif

	if(Verbose)
	{
		times(&endtm);
		int total = (endtm.tms_utime-begtm.tms_utime) +
				(endtm.tms_stime-begtm.tms_stime);
		int percent = (total - (total/TIC)*TIC)*100/TIC;
#ifdef TIMING
		fprintf(stderr,"%d.%02d\t",total/TIC,percent);
#else
		fprintf(stderr,"Time in level assignment: %d.%02ds\n",
				total/TIC, percent);
		fprintf(stderr,"Number of levels = %d\n",Maxlevel+1);
#endif
	}
}


static void balance()
{
	int *size = new int[N_nodes];
	for(int v = 0; v < N_nodes; v++)
		size[Level[v]] += 1;
	for (v = 0; v < N_nodes; ++v)
	{
		int	inweight = 0, outweight = 0;
		int	low = 0, high = N_nodes;

		for (edge_t *e = Inedges[v]; e; e = e->next)
		{
			inweight += e->weight;
			if (Level[e->node] > low)
				low = Level[e->node];
		}

		for (e = Outedges[v]; e; e = e->next)
		{
			outweight += e->weight;
			if (Level[e->node] < high)
				high = Level[e->node];
		}

		if (inweight && (inweight == outweight) && (low+1 < high-1))
		{
			int	best = low+1;
			for(int n = low+2; n < high; ++n)
				if(size[n] < size[best])
					best = n;
			size[Level[v]] -= 1;
			size[best] += 1;
			Level[v] = best;
		}
	}
	delete size;
}



/*
	Below are data structures and code for the network simplex algorithm.
*/
static struct edge_l
{
	int	node;		// the other end
	int	weight;		// edge weight
	int	minlen;		// minimum edge length
	int	cutvalue;	// value of cut set if this is a tree edge
	edge_l	*next;		// doubly linked for constant time insert/delete
	edge_l	*last;
	edge_l	*link;		// links for identifying pairs of out/in edges

	// this routine assume edge insertions are done at the head of the list
	edge_l(int innode, int inweight, int inminlen, edge_l *innext)
	{
		node = innode;
		weight = inweight;
		minlen = inminlen;
		cutvalue = 0;
		last = NULL;
		next = innext;
		if(innext)
			innext->last = this;
	}
};

static edge_l	**Oedges,	// static in/out-edge lists. No multiple edges
		**Iedges,	// are in these lists.
		**Itree,	// Edges in a feasible spanning tree
		**Otree;

static int	n_nodes,	// number of mega-nodes
		*Merge;		// mapping from normal nodes to mega-nodes.
				// The mega-nodes are nodes resulted from
				// merging sets of nodes which are specified
				// to be on the same levels.


// add a new edge
static void addedge(int tail, int head, int weight, int minlen)
{
	// map real nodes to corresponding mega-nodes
	head = Merge[head];
	tail = Merge[tail];

	// check for multiple edges
	for(edge_l *e = Oedges[tail]; e; e = e->next)
		if(e->node == head)
		{
			e->weight += weight;
			e->link->weight += weight;
			e->minlen = max(e->minlen,minlen);
			return;
		}

	// a new edge
	Oedges[tail] = new edge_l(head,weight,minlen,Oedges[tail]);
	Iedges[head] = new edge_l(tail,weight,minlen,Iedges[head]);

	// link the two versions of the edge
	Oedges[tail]->link = Iedges[head];
	Iedges[head]->link = Oedges[tail];
} 


// delete an edge from its linked list
static void delete_edge(edge_l *e, edge_l **elist)
{
	if(e->last)
		e->last->next = e->next;
	else	elist[0] = e->next;
	if(e->next)
		e->next->last = e->last;
}


// insert an edge into a linked list
static void insert_edge(edge_l *e, edge_l **elist)
{
	e->last = NULL;
	e->next = elist[0];
	if(elist[0])
		elist[0]->last = e;
	elist[0] = e;
}


// breadth-first search for initial level assignment
static int bfs(int me,int *level,edge_l **edges,int *queue,int *marked,int iter)
{
#define ENQUEUE(x)	queue[tail] = x; if(++tail >= n_nodes) tail = 0;
#define DEQUEUE(x)	x = queue[head]; if(++head >= n_nodes) head = 0;
#define NOTNULL()	head != tail

	int head = 0, tail = 0;
	ENQUEUE(me);

	int level_inc = edges == Oedges ? 1 : -1;
	int n_bfs = 0;
	while(NOTNULL())
	{
		DEQUEUE(me);
		for(edge_l *e = edges[me]; e; e = e->next)
		{
			int it = e->node;
			int length = level_inc*(level[it]-level[me]);

			if(level[it] == Maxint || length < e->minlen)
			{
				ENQUEUE(it);
				marked[it] = iter;
				n_bfs++;
				level[it] = level[me] + level_inc*e->minlen;
			}
		}
	}
	return n_bfs;
}



// construct an initial level assignment
static void initlevel(int me,int *level,int *queue,int *marked)
{
	for(int n = 0; n < n_nodes; ++n)
		marked[n] = 0;
	level[me] = 0;
	marked[me] = 2;

	// level assignment
	for(int iter = 1;; ++iter)
	{
		// odd iter: search down, even iter: search up
		edge_l **edges = iter%2 ? Oedges : Iedges;

		int nextiter = iter+1;
		int alldone = 1;
		for(n = 0; n < n_nodes; ++n)
			if(marked[n] >= iter)
			{
				if(bfs(n,level,edges,queue,marked,nextiter) > 0)
					alldone = 0;
				if(marked[n] > iter && alldone)
					alldone = 0;
			}
		if(alldone)
			return;
	}
}


// compute a feasible spanning tree
static void spantree(int me, int *level, int *queue, int *marked)
{
	for(int v = 0; v < n_nodes; ++v)
		marked[v] = 0;

	int head = 0, tail = 0;
	ENQUEUE(me);
	marked[me] = 1;
	while(NOTNULL())
	{
		DEQUEUE(me);
		for(int isdown = 0; isdown < 2; ++isdown)
		{
			int dir = isdown ? 1 : -1;
			edge_l **these_edges = isdown ? Oedges : Iedges;
			edge_l **those_edges = isdown ? Iedges : Oedges;
			edge_l **these_tree = isdown ? Otree : Itree;
			edge_l **those_tree = isdown ? Itree : Otree;

			edge_l *e, *enext;
			for(e = these_edges[me]; e; e = enext)
			{
				enext = e->next;

				int it = e->node;
				if(marked[it])
					continue;

				if(dir*(level[it]-level[me]) == e->minlen)
				{
					delete_edge(e,these_edges+me);
					insert_edge(e,these_tree+me);
					delete_edge(e->link,those_edges+it);
					insert_edge(e->link,those_tree+it);

					marked[it] = 1;
					ENQUEUE(it);
				}
			}
		}
	}
}


// compute all nodes on one side of the cutset
static void cutset(int me, int *marked, int val)
{
	marked[me] = val;
	for(int isdown = 0; isdown < 2; ++isdown)
		for(edge_l *e = isdown ? Otree[me] : Itree[me]; e; e = e->next)
			if(marked[e->node] == 0)
				cutset(e->node,marked,val);
} 


// compute the initial values of edges in a spanning forest
static void treevalue(int *marked)
{
	for(int i = 0; i < n_nodes; ++i)
	for(edge_l *e = Otree[i]; e; e = e->next)
	{
		// compute the cutset partition
		for(int n = 0; n < n_nodes; ++n)
			marked[n] = 0;

		// break the edge
		marked[i] = 1;
		marked[e->node] = -1;

		// search the two sides
		cutset(i,marked,1);
		cutset(e->node,marked,-1);

		int cutvalue = e->weight;
		for(n = 0; n < n_nodes; ++n)
		{
			if(marked[n] == 0)
				continue;
			for(edge_l *f = Oedges[n]; f; f = f->next)
			{
				if(marked[n] != marked[f->node])
				{
					if(marked[n] > 0)
						cutvalue += f->weight;
					else	cutvalue -= f->weight;
				}
			}
		}

		e->cutvalue = e->link->cutvalue = cutvalue;
	}
}


// update values of tree edges on a fundamental cycle being altered
static int treeupdate(int here, int to_be, int cutvalue, int *marked)
{
	marked[here] = 1;

	for(int isdown = 0; isdown < 2; ++isdown)
	{
		for(edge_l *e = isdown ? Otree[here] : Itree[here]; e; e = e->next)
		{
			int found = e->node == to_be ? 1 : 0;
			if(!found && !marked[e->node])
				found = treeupdate(e->node,to_be,cutvalue,marked);
			if(found)
			{
				e->cutvalue += isdown ? cutvalue : -cutvalue;
				e->link->cutvalue = e->cutvalue;
				return 1;
			}
		}
	}
	return 0;
}


// the network simplex algorithm for computing levels
static void networksimplex(int *level)
{
	// construct a feasible level assignment
	int *queue = new int[n_nodes];
	int *marked = new int[n_nodes];
	for(int i = 0; i < n_nodes; ++i)
	{
		marked[i] = 0;
		level[i] = Maxint;
	}
	for(i = 0; i < n_nodes; ++i)
		if(level[i] == Maxint)
		{
			// assign levels to node in the component containing i
			initlevel(i,level,queue,marked);

			// now find a feasible spanning tree
			spantree(i,level,queue,marked);
		}

	// initial values of tree edges
	treevalue(marked);

	// network simplex loop
	for(int iter = 0;; ++iter)
	{
#ifdef DEBUG
		printtree(iter);
		verifytree();
		checklevel(level);
#endif
		// find a negative tree edge
		int	cutvalue = 0, treetail;
		edge_l	*treeedge;
		for(i = 0; i < n_nodes; ++i)
		for(edge_l *e = Otree[i]; e; e = e->next)
			if(e->cutvalue < cutvalue) 
			{
				cutvalue = e->cutvalue;
				treetail = i;
				treeedge = e;
			}
		// done; the tree is positive
		if(cutvalue >= 0)
			break;

		// compute the cutset partition for this tree edge
		for(i = 0; i < n_nodes; ++i)
			marked[i] = 0;

		marked[treetail] = 1;		// break the edge
		marked[treeedge->node] = -1;
		cutset(treetail,marked,1);
		cutset(treeedge->node,marked,-1);

		// find a non-tree edge to be switched in
		int	length = Maxint;
		int	nontail;
		edge_l	*nonedge = NULL;
		for(i = 0; i < n_nodes; ++i)
		{
			if(marked[i] >= 0)
				continue;
			int level_i = level[i];
			for(e = Oedges[i]; e; e = e->next)
			{
				if(marked[e->node] < 0)
					continue;
				int len = (level[e->node] - level_i) - e->minlen;
				if(len < length)
				{
					length = len;
					nontail = i;
					nonedge = e;
				}
			}
		}

		// update the value of edges on the involved basic cycle
		for(i = 0; i < n_nodes; ++i)
			queue[i] = 0;
		(void)treeupdate(nontail,nonedge->node,treeedge->cutvalue,queue);

		// now switch the edges
		treeedge->cutvalue = treeedge->link->cutvalue = 0;
		delete_edge(treeedge,Otree+treetail);
		delete_edge(treeedge->link,Itree+treeedge->node);
		insert_edge(treeedge,Oedges+treetail);
		insert_edge(treeedge->link,Iedges+treeedge->node);

		nonedge->cutvalue = nonedge->link->cutvalue = -cutvalue;
		delete_edge(nonedge,Oedges+nontail);
		delete_edge(nonedge->link,Iedges+nonedge->node);
		insert_edge(nonedge,Otree+nontail);
		insert_edge(nonedge->link,Itree+nonedge->node);

		// update level info
		if(length > 0)
		{
			for(i = 0; i < n_nodes; ++i)
				if(marked[i] > 0)
					level[i] -= length;
		}
#ifdef DEBUG
		printtree(iter);
		verifytree();
		checklevel(level);
#endif
	}

#ifndef TIMING
	if(Verbose)
		fprintf(stderr,"# of network simplex iterations = %d\n",iter);
#endif

	// make the level assignment starts from 0 for each connected component
	for(i = 0; i < n_nodes; ++i)
		marked[i] = 0;
	for(i = 0; i < n_nodes; ++i)
	{
		if(marked[i])
			continue;

		// use breadth-first search to get all nodes in this component.
		int n = 0, n_elt = 0;
		queue[n_elt++] = i;
		marked[i] = 1;
		while(n < n_elt)
		{
			int me = queue[n++];
			for(int isdown = 0; isdown < 2; ++isdown)
			{
				edge_l *e = isdown ? Otree[me] : Itree[me];
				for(; e; e = e->next)
					if(!marked[e->node])
					{
						queue[n_elt++] = e->node;
						marked[e->node] = 1;
					}
			}
		}

		int minlev = Maxint;
		for(n = 0; n < n_elt; ++n)
			if(level[queue[n]] < minlev)
				minlev = level[queue[n]];
		if(minlev != 0 && minlev != Maxint)
			for(n = 0; n < n_elt; ++n)
				level[queue[n]] -= minlev;
	}

	// restore space
	delete marked;
	delete queue;
}


static void setlevels(int source, int sink)
{
	// Merge[] contains new names of the mega-nodes that represent
	// true nodes that are specified to be at the same levels.
	Merge = new int[N_nodes];
	n_nodes = 0;
	for(int i = 0; i < N_nodes; ++i)
		if(i == Root[i])
			Merge[i] = n_nodes++;
	for(i = 0; i < N_nodes; ++i)
		Merge[i] = Merge[Root[i]];

	// construct the new edge lists.
	// Since no min_edge_length info is kept, this is 1
	Oedges = new edge_l*[n_nodes];
	Iedges = new edge_l*[n_nodes];
	for(i = 0; i < N_nodes; ++i)
		for(edge_t *e = Outedges[i]; e; e = e->next)
			addedge(i,e->node,e->weight,e->minlen);

	// add constraint edges for sources and sinks.
	// These edges have zero weight and zero min_edge_length
	if(source >= 0 || sink >= 0)
	{
		for(i = 0; i < N_nodes; ++i)
			if(i == Root[i])
			{
				if(source >= 0 && i != source)
					addedge(source,i,0,0);
				if(sink >= 0 && i != sink)
					addedge(i,sink,0,0);
			}
	}

	// get level assignment
	Otree = new edge_l*[n_nodes];
	Itree = new edge_l*[n_nodes];
	int *level = new int[n_nodes];
	networksimplex(level);
#ifdef DEBUG
	checklevel(level);
#endif

	// now reassign level assignment to real nodes
	for(i = 0; i < N_nodes; ++i)
		Level[i] = level[Merge[i]];
#ifdef DEBUG
	verifylevel();
#endif

	// restore space used
	for(i = 0; i < n_nodes; ++i)
	{
		edge_l *enext;
		for(edge_l *e = Oedges[i]; e; e = enext)
		{
			enext = e->next;
			delete e;
		}
		for(e = Iedges[i]; e; e = enext)
		{
			enext = e->next;
			delete e;
		}
		for(e = Otree[i]; e; e = enext)
		{
			enext = e->next;
			delete e;
		}
		for(e = Itree[i]; e; e = enext)
		{
			enext = e->next;
			delete e;
		}
	}
	delete Iedges;
	delete Oedges;
	delete Itree;
	delete Otree;
	delete Merge;
	delete level;
}


#ifdef DEBUG
static void fatal()
{
	(void) abort();
}

static void printtree(int iter)
{
	fprintf(stderr,"Iteration=%d\n",iter);
	for(int i = 0; i < n_nodes; ++i)
	for(edge_l *e = Otree[i]; e; e = e->next)
		fprintf(stderr,"%d -> %d : cutvalue=%d\n",i,e->node,e->cutvalue);
	for(i = 0; i < n_nodes; ++i)
	for(e = Oedges[i]; e; e = e->next)
		fprintf(stderr,"%d -> %d\n", i,e->node);
}

static void verifytree()
{
	int *marked = new int[n_nodes];

	for(int i = 0; i < n_nodes; ++i)
	for(edge_l *e = Otree[i]; e; e = e->next)
	{
		// compute the cutset partition
		for(int n = 0; n < n_nodes; ++n)
			marked[n] = 0;

		// break the edge
		marked[i] = 1;
		marked[e->node] = -1;
		cutset(i,marked,1);
		cutset(e->node,marked,-1);
			
		int cutvalue = e->weight;
		for(n = 0; n < n_nodes; ++n)
		{
			if(marked[n] == 0)
				continue;
			for(edge_l *f = Oedges[n]; f; f = f->next)
				if(marked[n] != marked[f->node])
				{
					if(marked[f->node] == 0)
						fatal();
					if(marked[n] > 0)
						cutvalue += f->weight;
					else	cutvalue -= f->weight;
				}
		}

		if(e->cutvalue != cutvalue)
			fatal();
	}

	delete marked;
}

static void checklevel(int *level)
{
	for(int v = 0; v < n_nodes; ++v)
	{
		for(edge_l *e = Oedges[v]; e; e = e->next)
			if(level[e->node]-level[v] < e->minlen)
				fatal();
		for(e = Otree[v]; e; e = e->next)
			if(level[e->node]-level[v] != e->minlen)
				fatal();
		for(e = Iedges[v]; e; e = e->next)
			if(level[v]-level[e->node] < e->minlen)
				fatal();
		for(e = Itree[v]; e; e = e->next)
			if(level[v]-level[e->node] != e->minlen)
				fatal();
	}
}

static void verifylevel()
{
	for(int v = 0; v < N_nodes; ++v)
	{
		for(edge_t *e = Outedges[v]; e; e = e->next)
			if(Level[v] >= Level[e->node])
				fatal();
		for(e = Inedges[v]; e; e = e->next)
			if(Level[v] <= Level[e->node])
				fatal();
	}
}

static void checksearch(int v, int *marked, int isdown)
{
	marked[v] = 1;
	for(edge_l *e = isdown ? Oedges[v] : Iedges[v]; e; e = e->next)
	{
		if(marked[e->node] == 1)
			fatal();
		if(!marked[e->node])
			checksearch(e->node,marked,isdown);
	}
	marked[v] = 2;
}
static void checkcycle()
{
	int *marked = new int[n_nodes];
	for(int v = 0; v < n_nodes; ++v)
		if(!marked[v])
			checksearch(v,marked,1);

	for(v = 0; v < n_nodes; ++v)
		marked[v] = 0;
	for(v = 0; v < n_nodes; ++v)
		if(!marked[v])
			checksearch(v,marked,0);

	delete marked;
}

static void verifysearch(int v, int *marked, int isdown)
{
	marked[v] = 1;
	for(edge_t *e = isdown ? Outedges[v] : Inedges[v]; e; e = e->next)
	{
		if(marked[e->node] == 1)
			fatal();
		if(!marked[e->node])
			verifysearch(e->node,marked,isdown);
	}
	marked[v] = 2;
}
static void verifycycle()
{
	int *marked = new int[N_nodes];
	for(int v = 0; v < N_nodes; ++v)
		if(!marked[v])
			verifysearch(v,marked,1);

	for(v = 0; v < N_nodes; ++v)
		marked[v] = 0;
	for(v = 0; v < N_nodes; ++v)
		if(!marked[v])
			verifysearch(v,marked,0);

	for(v = 0; v < N_nodes; ++v)
	{
		for(edge_t *e = Outedges[v]; e; e = e->next)
			if(e->minlen < 1)
				fatal();
		for(e = Inedges[v]; e; e = e->next)
			if(e->minlen < 1)
				fatal();
	}

	delete marked;
}
#endif
