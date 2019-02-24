#include	"draw_dag.h"

#include	<sys/types.h>
#include	<sys/times.h>
#define TIC 60

int	N_real_nodes,	// actual number of input nodes
	N_nodes,	// number of nodes at various phases
	N_edges,	// total number of edges
	N_self_edges,	// # of loops detected
	N_flat_edges,	// # of edges whose ends are at the same level
	N_repeat_edges,	// # of multi-edges deleted
	N_revert_edges, // # of edges whose direction is reverted
	N_component;	// # of connected components
edge_t	**Inedges,	// in-edges for each node
	**Outedges,	// out-edges
	**Noedges,	// self-edges and flat edges
	**Istems,	// stem edges
	**Ostems;
int	*Stems;
int	*Root,		// space to store union trees of vertices at same ranks
	*Component,	// connected component numbers
	*Invert,	// inversion of nodes in their ranks
	*Level,		// level assignment of nodes
	Maxlevel,	// maximum level assigned to any node
	*N_level,	// number of nodes at each level
	**Rank;		// array of ordered lists of nodes
int	*Nodepos,	// node positions
	*Deleted,	// virtual nodes that were deleted
	*Width,		// widths of nodes
	Nodesep,	// minimum separation between nodes
	*Height,	// height of nodes
	Levelsep,	// separation between levels
	*Levelheight,	// height of layers
	*Levelpos;	// position of levels

int	Xbound,Ybound;	// if "fill" then size of box to fill, else 0

int	Verbose;	// say lots of things

/*
	Draw any directed graphs.
	This function works best with directed acyclic graphs.
	Cycles are broken using a depth-first search.
*/

void draw_dag(int n_nodes, node_t **nodes, edge_t **edges, options_t options)
{
	struct tms begtm, endtm;
	Verbose = options.verbose;

	if(options.verbose)
	{
#ifndef TIMING
		fprintf(stderr,"Start graph drawing\n");
#endif
		times(&begtm);
	}

	// Create Inedges[], Outedges[], Width[], N_nodes, and N_edges
	dag_start(n_nodes,nodes,edges,options);

	// remove stem edges
	if(N_flat_edges <= 0)
		dag_delstem();

	// Create Level[], Maxlevel
	int *srcs = options.source_nodes;
	int source = (srcs && srcs[0] != -1) ? Root[srcs[0]] : -1;
	int *sinks = options.sink_nodes;
	int sink = (sinks && sinks[0] != -1) ? Root[sinks[0]] : -1;
	if(sink == source)
		sink = -1;
	int **same = options.same_nodes;
	dag_levels(source,sink,same ? 1 : 0);

	// Make long edges into short edges by inserting dummy nodes
	dag_unitedges();

	// Create N_level[], Rank[][]
	dag_ranks();

	// reinsert stems
	if(N_flat_edges <= 0)
		dag_insstem();

	// Create Positions[], Maxpos 
	dag_positions(options.ranksep,!options.quick);

	// Create splines
	dag_spline();

	// Return values to user
	dag_end(nodes,edges);

	if(Verbose)
	{
		times(&endtm);
		int total = (endtm.tms_utime-begtm.tms_utime) +
				(endtm.tms_stime-begtm.tms_stime);
		int percent = (total - (total/TIC)*TIC)*100/TIC;
#ifdef TIMING
		fprintf(stderr,"%d.%02d\n",total/TIC, percent);
#else
		fprintf(stderr,"Total time in graph drawing: %d.%02ds\n",
				total/TIC, percent);
#endif
	}
}
