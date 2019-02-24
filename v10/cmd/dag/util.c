#include "draw_dag.h"
#include "dag.h"
#include "paths.h"
#include "parsedag.h"
#include "defaults.h"
#include "unistd.h"

/*** utility functions ***/

char* Infile::gets(char *buf,int n) {
	if (!fp) return 0;
	line_number++;
	return fgets(buf,n,fp);
}

Infile nextfile(char** &argv)
{
	argv++;
	while (**argv == '-') argv++;
	Infile rv = Infile(*argv);
	if (!rv.fp) fprintf(stderr,"%s: can't open %s\n",Cmd_name,*argv);
	return rv;
}

/*** shared strings ***/

shared_string_t *shared_string_pool;
char *newstring(char *init) {
	shared_string_t *it
		= (shared_string_t*) malloc(sizeof(shared_string_t) + strlen(init));
	it->next = shared_string_pool;
	shared_string_pool = it;
	strcpy(it->data,init);
	return it->data;
}

void freestrings() {
	shared_string_t *q,*p = shared_string_pool;
	while (p) {
		q = p->next;
		free((char*)p);
		p = q;
	}
	shared_string_pool = 0;
}

/*** hash table ***/

int hash(register char* s, int size) {
	register unsigned int	h;
	register int	c;

	h = 0;
	while (c = *s++) {
		if ((h <<= 1) < 0) h ^= c ^ ((~(((unsigned int)~0) >> 1)) | 1);
		else h ^= c;
	}
	return(h % size);
}

const int Hash_extent = 517;
struct hashrec {
	int			node;
	hashrec		*next;

	hashrec(int argnode, hashrec *argnext) {
		node  = argnode;
		next  = argnext;
	};
} *Hashtab[Hash_extent];

boolean expand_graph(int new_size) {
		if (! (Node = (DAG_node_t**)realloc((char*)Node,sizeof(DAG_node_t*)*new_size)))
			return false;
		else
			if (! (Edge = (DAG_edge_t**)realloc((char*)Edge,sizeof(DAG_edge_t*)*new_size)))
				return false;
		return true;
}

/* look name and insert in symbol table if not found. return new node's number. */
int node_lookup(char *name) {
	int h = hash(name,Hash_extent);
	for (hashrec *hp = Hashtab[h]; hp; hp = hp->next)
		if (!strcmp(name,Node[hp->node]->name)) break;
	if (!hp) {
		Hashtab[h] = hp =  new hashrec(N,Hashtab[h]);

		if (N > Extent - 1) {
			Extent += Extent;
			if (!expand_graph(Extent)) {
				yyerror("too many nodes, out of memory");
				exit(1);
			}
		}
		Node[N] = new DAG_node_t();
		*Node[N] = Default_node;
		Node[N]->setname(name);
		Edge[N] = (DAG_edge_t*)0;
		N++;
	}
	return hp->node;
}

/*** utility functions for code gen */

void cat_libfile(char* name) {
	char buf[BUFSIZ];
	if (!Uselib) return;
	if (!Lib_path) {
#ifdef EXPTOOLS
		char *tools = getenv("TOOLS");
		if (tools) {
			Lib_path = new char[strlen(Lib_path)+strlen(EXPTOOLS_SUBDIR)+2];
			sprintf(Lib_path,"%s/%s",tools,EXPTOOLS_SUBDIR);
			sprintf(buf,"%s/%s",Lib_path,name);
			if (access(buf,R_OK))
				Lib_path = LIB_PATH;
		}
		else
#endif EXPTOOLS
			Lib_path = LIB_PATH;
	}
	fflush(stdout);
	sprintf(buf,"cat %s/%s",Lib_path,name);
	system(buf);
}

Point find_edge_midpoint(DAG_edge_t *e) {
	int x,y;
	for (int npts = 0; e->splinept[npts].y >= 0; npts++);
	if (iabs(e->splinept[0].y - e->splinept[npts-1].y) >
	    iabs(e->splinept[0].x - e->splinept[npts-1].x)) { // "vertical" edge
		int i,y1 = 0;
		y1 = (e->splinept[0].y + e->splinept[npts-1].y)/2;
		for (i = 0; e->splinept[i].y >= 0; i++)
			if (between(e->splinept[i].y,y1,e->splinept[i+1].y)) break;
		y = y1;
		x = (int) (e->splinept[i].x +
			(double)(y1 - e->splinept[i].y)/(e->splinept[i+1].y-e->splinept[i].y)
			* (e->splinept[i+1].x - e->splinept[i].x));
	}
	else {												// "horizontal" edge
		int i,x1 = 0;
		x1 = (e->splinept[0].x + e->splinept[npts-1].x)/2;
		for (i = 0; e->splinept[i].y >= 0; i++)
			if (between(e->splinept[i].x,x1,e->splinept[i+1].x)) break;
		x = x1;
		y = (int) (e->splinept[i].y +
			(double)(x1 - e->splinept[i].x)/(e->splinept[i+1].x-e->splinept[i].x)
			* (e->splinept[i+1].y - e->splinept[i].y));
	}
	Point rv;
	rv .x = x; rv .y = y;
	return rv;
}

static boolean seginter(int x0,int y0, int x1, int y1, int x2, int y2, int x3, int y3,
int& xinter, int& yinter) {
	double m0,b0,m1,b1,x;

	if ((x0 == x1) && (x2 == x3)) return false;
	if (x2 == x3) {swap(x0,x2); swap(x1,x3); swap(y0,y2); swap(y1,y3);}
	if (x0 == x1) {
		x = x0;
	}	
	else {
		m0 = (y1 - y0)/(double)(x1 - x0);
		b0 = y0 - m0*x0;
		m1 = (y3 - y2)/(double)(x3 - x2);
		b1 = y2 - m1*x2;
		if (m1 == m0) {
			if (b0 != b1) return false;
			int l0lowx = min(x0,x1);
			int l0highx = max(x0,x1);
			int l1lowx = min(x2,x3);
			int l1highx = max(x2,x3);
			if (between(l0lowx,l1lowx,l0highx))
				x = l1lowx;
			else if (between(l0lowx,l1highx,l0highx))
				x = l1highx;
			else if (between(l1lowx,l0lowx,l1highx))
				x = l0lowx;
			else return false;
		}
		else x = (b1 - b0)/(m0 - m1);
	}

	if (between(x2,round(x),x3)) {
		if (between(y2,round(m1 * x + b1),y3)) {
			yinter = round(m1 * x + b1);
			xinter = round(x);
			return true;
		}
	}
	return false;
}

/*
 *  return point of intersection of the node 
 *  with the ray from (x0,y0) through (x1,y1).
 *  p1 must be on the bounding box of the node.
 *  assume User_defined nodes are like Box.
 */
Point find_nodeport(int node, Point p0, Point p1) {
	Point rv;
	if ((p0.x == p1.x) && (p0.y == p1.y)) {return p0;}
	int x0 = p0.x; int y0 = p0.y; int x1 = p1.x; int y1 = p1.y;

	/* handle degenerate case of vertical line segment */
	if (x0 == x1) return p1;

	while (1) {
		/* normal case */
		double m = (double)(y1 - y0) / (double)(x1 - x0); 
		double b = y0 - m * x0;
		switch(Node[node]->shape.shape_id) {
			case Circle:
			case Doublecircle:
			case Ellipse:
				double s0,s1;	// two solutions (x values)
				//double b1 = m*Node[node]->pos.x + b - Node[node]->pos.y;
				double b1 = (y0 - Node[node]->pos.y) - m * (x0 - Node[node]->pos.x);
				double rx = Node[node]->xsize/2.;
				double ry = Node[node]->ysize/2.;
				double aa = 1/(rx*rx) + (m*m)/(ry*ry);
				double bb = 2.*m*b1/(ry*ry);
				double cc = (b1*b1)/(ry*ry) - 1;
				if (m == 0.) {
					s0 = rx;
					s1 = -rx;
				}
				else {
					double t = bb*bb - 4.*aa*cc;
					if (t < 0) {
						if (x1 == Node[node]->pos.x && y1 == Node[node]->pos.y) 
							panic("can't find node intersection");
						x1 = Node[node]->pos.x;
						y1 = Node[node]->pos.y;
						continue;
					}
					s0 = (-bb + sqrt(t))/(2.*aa);
					s1 = (-bb - sqrt(t))/(2.*aa);
				}
				s0 += Node[node]->pos.x;
				s1 += Node[node]->pos.x;
				if (distance(s0,m*s0+b,x0,y0) <= distance(s1,m*s1+b,x0,y0)) {
					rv.x = round(s0);
					rv.y = round(s0 * m + b);
				}
				else {
					rv.x = round(s1);
					rv.y = round(s1 * m + b);
				}
				return rv;
			case Diamond:
				x1 = Node[node]->pos.x;
				y1 = (int)(m*x1 + b);
				switch((sign(x0 - Node[node]->pos.x)== -1) + 2*(sign(y0 - Node[node]->pos.y)== -1)) {
					case 0:
						if (seginter(x0,y0,x1,y1,Node[node]->pos.x+Node[node]->xsize/2,Node[node]->pos.y,Node[node]->pos.x,Node[node]->pos.y+Node[node]->ysize/2,rv.x,rv.y)) return rv;
						break;
					case 1:
						if (seginter(x0,y0,x1,y1,Node[node]->pos.x,Node[node]->pos.y+Node[node]->ysize/2,Node[node]->pos.x-Node[node]->xsize/2,Node[node]->pos.y,rv.x,rv.y)) return rv;
						break;
					case 2:
						if (seginter(x0,y0,x1,y1,Node[node]->pos.x,Node[node]->pos.y-Node[node]->ysize/2,Node[node]->pos.x+Node[node]->xsize/2,Node[node]->pos.y,rv.x,rv.y)) return rv;
						break;
					case 3:
						if (seginter(x0,y0,x1,y1,Node[node]->pos.x-Node[node]->xsize/2,Node[node]->pos.y,Node[node]->pos.x,Node[node]->pos.y-Node[node]->ysize/2,rv.x,rv.y)) return rv;
						break;
				}
			case Box:
			case Square:
			case Plaintext:
			case User_defined:
			default:
				return p1;
		}
	}
}

void reclaim_hashtable() {
	for (int i = 0; i < Hash_extent; i++) {
		hashrec *h,*h1;
		if (Hashtab[i])
			for (h = Hashtab[i], h1 = h->next; h1; h1=h1->next){
				delete h; h = h1;
			}
		Hashtab[i] = 0;
	}
}

void set_output_type(char *name) {
	const int n_output_types = 4;
	static struct {
		char*			name;
		output_lang_t	lang;
	} map[] = {	
		"pic",		Pic,
		"ps",		Postscript,
		"simple",	Graphdraw,
		"cip",		Cip
	};
	for (int i = 0; i < n_output_types; i++)
		if (!strcmp(name,map[i].name)) {
			Output_type = map[i].lang;
			return;
		}
	fprintf(stderr,"%s: warning, no output driver for %s, option ignored.\n",
		Cmd_name,name);
}

void set_page_size(char *arg) {
	double	w,h,mw,mh;	/* width, height, margin width, margin height */
	int		nargs;
	nargs = sscanf(arg,"%lfx%lf,%lfx%lf",&w,&h,&mw,&mh);
	if (nargs > 1) {
		Page_size.x = (int)(w*Resolution);
		Page_size.y = (int)(h*Resolution);
	}
	if (nargs > 2) {
		Margin.x = (int)(mw * Resolution);
		if (nargs > 3) Margin.y = (int)(mh * Resolution);
		else Margin.y = Margin.x;
	}
	Page_size.x -= 2*Margin.x;
	Page_size.y -= 2*Margin.y;
	if (Page_size.x <= 0 || Page_size.y <= 0) {
		fprintf(stderr,"%s: warning, bad page or margin size options (ignored)\n",Cmd_name);
		Page_size.x = 0; Page_size.y = 0;
	}
}

options_t reset_options() {
	options_t rv;

	rv.quick		= 	Default_options_quick;
	rv.verbose		=	Default_options_verbose;
	rv.rankadjust 	=	Default_options_rankadjust;
	rv.ranksep		=	Default_options_ranksep;
	rv.nodesep		=	Default_options_nodesep;
	rv.xbound		=	0;
	rv.ybound		=	0;

	rv.n_same_nodes=	0;
	rv.same_nodes 	=	0;
	rv.source_nodes=	0;
	rv.sink_nodes	=	0;
	return	rv;
}

void set_bounds(double xdimen, double ydimen) {
	if ((xdimen <= 0.0) || (ydimen <= 0.0)) return;
	/* the following because the picture will be rotated down in draw_dag */
	if (User.rotated) {double t = xdimen; xdimen = ydimen; ydimen = t;}
	Options.xbound = round(xdimen * Resolution);
	Options.ybound = round(ydimen * Resolution);
}

shape_t::shape_t(int intype, char* invalue) {
	const int n_predefined_shapes = 7;
	static struct {
		char *		n;
		shape_id_t	s;
	} map[] = {
		"Circle", Circle,
		"Doublecircle",	Doublecircle,
		"Box",	Box,
		"Ellipse",	Ellipse,
		"Square",	Square,
		"Plaintext",	Plaintext,
		"Diamond",	Diamond
	};
	type = intype;
	value = invalue;
	if (type == STRING) {
		for (int i = 0; i < n_predefined_shapes; i++) {
			if (!strcmp(invalue,map[i].n)) {
				type	=	intype;
				predefined = true;
				shape_id = map[i].s;
				return;
			}
		}
	}
	predefined = false;
	shape_id = User_defined;
}
