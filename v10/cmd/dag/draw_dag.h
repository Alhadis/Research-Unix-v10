#include	<stdio.h>
#include	<string.h>
#include	<ctype.h>
#include	<math.h>
#include	<libc.h>
#include	<memory.h>

#define		Maxint	(((unsigned) 1 << 31) - 1)

typedef int (*qsortcmpfun)(void*,void*);
#ifndef QSORTDCL
#ifdef MICC
extern "C" {
	extern void qsort(char*,int, int, qsortcmpfun);
}
#else
	extern void qsort(char*,int, int, qsortcmpfun);
#endif  MICC
#endif QSORTDCL

struct Point {
	int	x;
	int	y;
};

// edge structure for input graphs
struct edge_t {
	int	node;		// head or tail node
	int	weight;		// cost of stretching this edge
	int	width;		// the width of the edge.  0 = invis.
	int	minlen;		// minimum length of the edge
	union {
	int	count;		// # of multiple edges in a class
				// 'count' is used in our Outedges and Inedges
	int	place;		// where it should be placed in its class
				// of multiple edges
	};
	union {
		Point	*splinept;	// spline control pts (last is (-1,-1))
		edge_t	*chain;		// chaining of broken edges
	};
	edge_t	*link;		// to link inedges and outedges
	edge_t	*next;
	Point	top;		// points from which the end-points can be
	Point	bottom;		// stretched to intersect weird node shapes

	edge_t(int innode, edge_t *innext)	// for boring linked lists
	{
		node = innode;
		next = innext;
	}
	edge_t(int innode, int inweight, int inwidth, int inminlen, edge_t* innext) {
		node	= innode;
		weight 	= inweight;
		width	= inwidth;
		minlen	= inminlen;
		next	= innext;
		count	= 1;
	}
	edge_t() {	
		/* this establishes the defaults for edges */
		node	= -1;
		weight	= 1;
		width	= 1;
		minlen	= 1;
		count	= 1;
		/* others are zero by operator new */
	}
};

struct node_t {
	char*	name;
	Point	pos;
	int		width;		// across the rank
	int		height;		// above&below the rank
};

struct options_t {
	int	quick;		// optimize for speed not drawing quality.
	int	verbose;	// say lots of things.
	int	rankadjust;	// 0 = uneven OK, 1 = all equal, 2 = exact 
	int	ranksep;	// default separation between adjacent ranks
	int	nodesep;	// minimum separation between adjacent nodes
	int xbound;		// if "fill" x limit of drawing area else 0
	int ybound;		// if "fill" y limit of drawing area else 0

	int	**same_nodes,n_same_nodes;
	int	*source_nodes;
	int	*sink_nodes;
};

overload max;
static inline double max(double a, double b) { return a > b ? a : b; }
static inline int max(int a, int b) { return a > b ? a : b; }

overload min;
static inline double min(double a, double b) { return a < b ? a : b; }
static inline int min(int a, int b) { return a < b ? a : b; }

static inline void swap(int& a, int& b) { int t; t = a; a = b; b = t;}

static inline int iabs(int a) { return a < 0 ? -a : a; }

extern void	draw_dag(int,node_t**,edge_t**,options_t);
extern void	dag_start(int,node_t**,edge_t**,options_t);
extern void	dag_delstem();
extern void	dag_insstem();
extern void	dag_levels(int,int,int);
extern void	dag_positions(int,int);
extern void	dag_ranks();
extern void	dag_unitedges();
extern void	dag_end(node_t**,edge_t**);
extern void	dag_simplex(int,int,long**,long*,long*,int,int*);
extern void	dag_spline();
extern void	deledges(int,edge_t**);
extern int	longedge(int,int);
extern void	panic(char*);

//	The following variables and storage space are available to users
//	after a call to draw_dag(). In particular, Nodepos and Levelpos
//	define the virtual locations assigned to nodes and levels.

extern int	N_real_nodes,	// actual number of input nodes
		N_nodes,	// number of nodes at various phases
		N_edges,	// total number of edges
		N_self_edges,	// # of loops detected
		N_flat_edges,	// # of edges whose ends are at the same level
		N_repeat_edges,	// # of multi-edges deleted
		N_revert_edges; // # of edges whose direction is reverted
extern edge_t	**Inedges,	// in-edges for each node
		**Outedges,	// out-edges
		**Noedges,	// self-edges and flat edges
		**Istems,	// in stem-edges
		**Ostems;	// out stem edges
extern int	*Stems;		// stem nodes
extern int	*Trueheight;	// save heights for when stem nodes are removed

extern int	*Root;		// union trees of vertices at same ranks

extern int	N_component;	// number of connected components
extern int	*Component;	// connected component numbers of nodes

extern int	*Level,		// levels assigned to nodes
		Maxlevel,	// maximum level assigned to any node
		*N_level,	// number of nodes at each level
		*Invert,	// indices of nodes in their ranks
		**Rank,		// array of ordered lists of nodes
		Levelsep,	// separation between levels
		*Levelheight,	// thickness of layers
		*Levelpos;	// position of levels

extern int	*Nodepos,	// node positions
		*Deleted,	// virtual nodes that were deleted
		*Width,		// widths of nodes
		Nodesep,	// minimum separation between nodes
		*Height;	// heights of nodes

extern int	Xbound,Ybound;	// if "fill" then size of box to fill, else 0

extern int	Verbose;	// say lots of things

