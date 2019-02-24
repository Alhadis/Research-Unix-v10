%{
#include "draw_dag.h"
#include "dag.h"
#define MIN_RANK_SEP	4

static pair_t*		nodelist;
static DAG_edge_t*	this_edge;
static boolean		is_ordered = false;

int					Syntax_error;
DAG_node_t			Reset_node,Default_node,Proto_node;
DAG_edge_t			Reset_edge,Default_edge;
static boolean		set_pointsize,set_label,set_shape,set_color,set_xsize,set_ysize;

static void teardown (pair_t *e) {
	pair_t *f;
	while (e) {
		f = e->next;
		delete e;
		e = f;
	}
}

static void init_proto() {
	Proto_node = Default_node;
	set_pointsize = set_label = set_shape = set_color = set_xsize = set_ysize = false;
}

static void apply_proto(DAG_node_t *np) {
	/* label setting has precedence over size! */
	if (set_pointsize) np->setpointsize(Proto_node.pointsize);
	if (set_label) np->setlabel(Proto_node.label.type,Proto_node.label.value);
	if (set_shape) np->setshape(Proto_node.shape.type,Proto_node.shape.value);
	if (set_color) np->setcolor(Proto_node.color);
	if (set_xsize) np->setxsize(Proto_node.xsize);
	if (set_ysize) np->setysize(Proto_node.ysize);
	np->autosize();
}

static void install_proto() {
	Proto_node.autosize();
	for (pair_t* p = nodelist; p; p = p->next)
		apply_proto(Node[p->node]);
}

/* append list1 to list2 */
static DAG_edge_t* append(DAG_edge_t *list1, DAG_edge_t *list2) {
if (!list2) panic("null list2");
	DAG_edge_t *e,*f;
	e = list2;
	while (f = e->nextof()) e = f;
	e->next = (edge_t*) list1;
	return list2;
}

/* create a new set of same rank nodes within the Options struct */
static int* &newset() {
	const int hunksize = 16;
	if (!Options.same_nodes)
		Options.same_nodes = new int*[hunksize];
	else if (!(Options.n_same_nodes % hunksize))
		Options.same_nodes = (int**)realloc((char*)Options.same_nodes,
			(Options.n_same_nodes + hunksize + 1)*sizeof(int*));
	Options.same_nodes[Options.n_same_nodes] = 0;
	return(Options.same_nodes[Options.n_same_nodes++]);
}

/* take the union of same rank nodes */
overload set_union;
static void set_union (int* &ptr, pair_t *nlist) {
	pair_t *e;
	int olen = 0, nlen = 0;
	for (e = nlist; e; e = e->next) nlen++;
	if (!nlen) return;
	if (ptr) {
		while (ptr[olen++] >= 0);
		ptr = (int*)realloc((char*)ptr,(nlen+olen)*sizeof(int));
	}
	else ptr = new int[nlen + 1];

	for (e = nlist; e; e = e->next)
		ptr[olen++] = e->node;
	ptr[olen] = -1;
}

static void set_union(int* &ptr, DAG_edge_t *elist) {
	pair_t *p = 0;
	while (elist) {
		p = new pair_t(elist->node,p);
		elist = elist->nextof();
	}
	set_union(ptr,p);
	teardown(p);
}

static void make_invis_edge(int fromnode,int tonode) {
	DAG_edge_t *e = new DAG_edge_t();
	e->ink = invis_ink;
	e->node = tonode;
	e->next = Edge[fromnode];
	Edge[fromnode] = e;
}

static void enter_edgelist(int tailnode, DAG_edge_t* elist) {
	DAG_edge_t *e;
	if (is_ordered) {
		set_union(newset(),elist);
		for (e = elist; e->nextof(); e = e->nextof())
			make_invis_edge(e->node,e->nextof()->node);
		is_ordered = false;
	}
	Edge[tailnode] = append(Edge[tailnode],elist);
}

static void enter_backedgelist(int headnode, DAG_edge_t* elist) {
	DAG_edge_t *e = elist;
	while (e) {
		DAG_edge_t *f = e->nextof();
		e->next = 0;
		e->flipped = true;
		int tailnode = e->node;
		e->node = headnode;
		Edge[tailnode] = append(Edge[tailnode],e);
		e = f;
	}
}	

static void enter_pathlist(int tailnode, DAG_edge_t* elist) {
	DAG_edge_t *e,*f;
	e = elist;
	while (e) {
		f = e;
		e = e->nextof();
		f->next = Edge[tailnode];
		Edge[tailnode] = f;
		tailnode = f->node;
	}
	is_ordered = false;
}

static void enter_backpathlist(int headnode, DAG_edge_t *elist) {
	DAG_edge_t *e = elist;
	while (e) {
		DAG_edge_t *f = e->nextof();
		e->next = 0;
		e->flipped = true;
		int tailnode = e->node;
		e->node = headnode;
		Edge[tailnode] = append(Edge[tailnode],e);
		e = f;
		headnode = tailnode;
	}
}

%}
%union {
	char		*s;
	int			i;
	pair_t		*p;	// for node lists
	DAG_edge_t	*e;	// for edge lists
}
%token	<i> AS BACKEDGE BACKPATH COLOR DASHED DOTTED DRAW EDGE EDGES EQUALLY
%token	<i> EXACTLY FROM HEIGHT INVIS LABEL MAXIMUM MINIMUM NODES ORDERED
%token	<i> PATH POINTSIZE RANK RANKS SAME SEPARATE SOLID TO WEIGHT WIDTH
%token	<s> STRING DESC
%type	<i> nitem eitem inkval nname intval tailnode
%type	<e>	head_list head tohead
%type	<p>	nlist 
%%
program		:   stmtlist
					{make_drawing();}
			|	error
			;

stmtlist	:  stmtlist stmt
			|	/*empty*/
			;

stmt		:	drawstmt
			|	edgestmt
			|	ctlstmt
			;

drawstmt	:	DRAW nlist {init_proto(); nodelist = $2;} ndesc ';' 
					{install_proto(); teardown($2);}
			|	DRAW NODES {init_proto(); nodelist = 0;}  ndesc ';'
					{apply_proto(&Default_node);}
			|	DRAW EDGES {this_edge = &Default_edge;} edesc ';'
			;

nlist		:	nlist nname
					{$$ = new pair_t($2,$1);}
			|	nlist ',' nname
					{$$ = new pair_t($3,$1);}
			|	nname
					{$$ = new pair_t($1,(pair_t*)0);}
			;
					

ndesc		:	nitem
			|	ndesc nitem
			;

nitem		:	WIDTH STRING
					{
						Proto_node.setxsize((int)(Resolution*atof($2)));
						set_xsize = true;
					}

			|	HEIGHT STRING
					{
						Proto_node.setysize((int)(Resolution*atof($2)));
						set_ysize = true;
					}
			|	POINTSIZE intval
					{
						Proto_node.setpointsize($2);
						set_pointsize = true;
					}
			|	LABEL STRING
					{
						Proto_node.setlabel(STRING,$2);
						set_label = true;
					}
			|	LABEL DESC
					{
						Proto_node.setlabel(DESC,$2);
						set_label = true;
					}
			|	AS DESC
					{
						Proto_node.setshape(DESC,$2);
						set_shape = true;
					}
			|	AS STRING
					{
						Proto_node.setshape(STRING,$2);
						set_shape = true;
					}
			|	COLOR STRING
					{
						Proto_node.setcolor($2);
						set_color = true;
					}
			;

edgestmt	:	ORDERED {is_ordered = true;} plainedgestmt
			|	plainedgestmt
			;

plainedgestmt	:	nname ';'
			|	nname head_list ';'
					{enter_edgelist($1,$2);}
			|	EDGE tailnode head_list ';'
					{enter_edgelist($2,$3);}
			|	BACKEDGE tailnode head_list ';'
					{enter_backedgelist($2,$3);}
			|	PATH tailnode head_list ';'
					{enter_pathlist($2,$3);}
			|	BACKPATH tailnode head_list ';'
					{enter_backpathlist($2,$3);}
			|	';'	/* empty statement */
			;

tailnode	:	nname
			|	FROM nname
					{$$ = $2;}

head_list	:	tohead
			|	head_list tohead
					{$$ = append($2,$1);}
			|	head_list ',' tohead
					{$$ = append($3,$1);}
			;

tohead		:	head
			|	TO head
					{$$ = $2;}
			;

head		:	nname
					{
						this_edge = new DAG_edge_t();
						*this_edge = Default_edge;
						this_edge->node = $1;
					}
				edesc
					{$$ = this_edge;}
			|	nname
					{
						this_edge = new DAG_edge_t();
						*this_edge = Default_edge;
						this_edge->node = $1;
						$$ = this_edge;
					}
			;

edesc		:	eitem
			|	edesc eitem
			;

eitem		:	WEIGHT intval
					{this_edge->setweight($2);}
			|	LABEL DESC
					{this_edge->setlabel(DESC,newstring($2));}
			|	LABEL STRING
					{this_edge->setlabel(STRING,newstring($2));}
			|	POINTSIZE intval
					{this_edge->setpointsize($2);}
			|	COLOR STRING
					{this_edge->setcolor(newstring($2));}
			|	inkval
					{this_edge->setink($1);}
			;
	
ctlstmt		:	SEPARATE seplist ';'
			|	MINIMUM RANK nlist ';'
					{set_union(Options.source_nodes,$3);teardown($3);}
			|	MAXIMUM RANK nlist ';'
					{set_union(Options.sink_nodes,$3);teardown($3);}
			|	SAME RANK nlist ';'
					{set_union(newset(),$3);teardown($3);}
			;

seplist		:	seplist sepitem
			|	/* empty */
			;

sepitem		:	NODES STRING
					{
						Options.nodesep = max(1,round(Resolution*atof($2)));
					}
			|	RANKS STRING EXACTLY
					{
						Options.ranksep = max(MIN_RANK_SEP,round(Resolution*atof($2)));
						Options.rankadjust = 2;
					}
						
			|	RANKS STRING EQUALLY
					{
						Options.ranksep = max(MIN_RANK_SEP,round(Resolution*atof($2)));
						Options.rankadjust = 1;
					}
			|	RANKS EQUALLY
					{
						Options.rankadjust = 1;
					}
			|	RANKS STRING
					{
						Options.ranksep = max(MIN_RANK_SEP,round(Resolution*atof($2)));
					}
			;

inkval		:	SOLID
					{$$ = solid_ink;}
			|	DASHED
					{$$ = dashed_ink;}
			|	DOTTED
					{$$ = dotted_ink;}
			|	INVIS
					{$$ = invis_ink;}
			;

nname		:	STRING
					{$$ = node_lookup($1);}
			;

intval		:	STRING
					{$$ = atoi($1);}
			;
