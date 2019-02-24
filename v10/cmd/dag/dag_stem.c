#include	"draw_dag.h"


// Merge all stems from the same node into a single one
// The width of the remaining node is appropriately widened while
// its height should be largest of the whole bunch.

void dag_delstem()
{
	Istems = new edge_t*[N_real_nodes];
	Ostems = new edge_t*[N_real_nodes];
	Stems = new int[N_real_nodes];

	int n_remove = 0;
	int n_retain = 0;
	for(int in = 0; in < 2; ++in)
	{
		edge_t **these = in ? Inedges : Outedges;
		edge_t **those = in ? Outedges : Inedges;
		edge_t **edges = in ? Istems : Ostems;
		for(int v = 0; v < N_real_nodes; ++v)
		{
			// remove all stems from the edge list
			edge_t *e, *elast, *stem;
			edges[v] = NULL;
			elast = NULL;
			stem = NULL;
			for(e = these[v]; e; e = elast ? elast->next : these[v])
			{
				if(these[e->node] || those[e->node]->next)
					elast = e;
				else if(!stem)
				{
					stem = e;
					elast = e;
					n_retain++;
				}
				else
				{
					if(elast)
						elast->next = e->next;
					else	these[v] = e->next;
					e->next = edges[v];
					edges[v] = e;
					n_remove++;
				}
			}

			if(edges[v])
			{
				// merge edges
				int w = stem->node;
				for(e = edges[v]; e; e = e->next)
				{
					stem->count += e->count;
					stem->weight += e->weight;
					those[e->node] = NULL;
					Stems[e->node] = 1;
				}
				stem->link->count = stem->count;
				stem->link->weight = stem->weight;
				edges[v]->chain = stem;
			}
		}
	}
}


// reinsert the stems
void dag_insstem()
{
	for(int down = 0; down < 2; ++down)
	{
		edge_t **edges = down ? Ostems : Istems;
		edge_t **these = down ? Outedges : Inedges;
		edge_t **those = down ? Inedges : Outedges;
		for(int v = 0; v < N_real_nodes; ++v)
		{
			if(!edges[v])
				continue;

			// our representative
			edge_t *stem = edges[v]->chain;
			edges[v]->chain = NULL;

			// number of friends
			int n_edges = 0;
			for(edge_t *e = edges[v]; e; e = e->next)
				n_edges += 1;

			// insert the nodes in the rank listing
			int w = stem->node;
			int *rank = Rank[Level[w]];
			for(int n_rank = 0; rank[n_rank] != -1; ++n_rank)
				;
			rank[n_rank+n_edges] = -1;
			int pos = Invert[stem->node];
			for(int i = n_rank+n_edges-1; i > pos+n_edges; --i)
			{
				rank[i] = rank[i-n_edges];
				Invert[rank[i]] = i;
			}

			edge_t *elast;
			for(e = edges[v], i = pos+1; e; i++, elast = e, e = e->next)
			{
				int n = e->node;
				those[n] = e->link;
				stem->count -= e->count;
				stem->weight -= e->weight;
				Component[n] = Component[w];
				rank[i] = n;
				Invert[n] = i;
			}
			stem->link->count = stem->count;
			stem->link->weight = stem->weight;

			// put these edges back in the proper edge list
			elast->next = these[v];
			these[v] = edges[v];
			edges[v] = NULL;
		}
	}
}
