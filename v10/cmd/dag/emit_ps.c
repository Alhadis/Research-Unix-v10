/*
 *	emit PostScript.
 *
 */

#include "draw_dag.h"
#include "dag.h"
#include "parsedag.h"
#include "paths.h"

static double		PS_scalefactor;
static dag_ink_t	Current_PS_ink;

static Point Page_min,Page_max;
static Point PS_page_size,PS_Margin;
static int Pages_across, Pages_up;
static int Page_number = 0;

static char* escape(char *s) {
	static char buf[BUFSIZ];
	char *p = buf;
	while (*s)  {
		if ((*s == '(') || (*s == ')')) *p++ = '\\';
		*p++ = *s++;
	}
	*p = '\0';
	return buf;
}

void emit_ps_setink(dag_ink_t ink) {
	char *inkstr;
	switch(ink) {
		case solid_ink:
			inkstr = "[] 0"; break;
		case dashed_ink:
			inkstr = "[9 9 idtransform abs ceiling exch abs ceiling exch] 0"; break;
		case dotted_ink:
			inkstr = "[1 6 idtransform abs ceiling exch abs ceiling exch] 0"; break;
		case invis_ink:
			inkstr = "[0 1] 1"; break;
	}
	printf("%s setdash\n",inkstr);
	Current_PS_ink = ink;
}

void emit_ps_header() {
	if (!Page_number) {
		printf("%%!   PostScript\n%%%%EndComments\n");
		printf("/Reset_Node_Pointsize %d def\n",Reset_node.pointsize);
		printf("/Reset_Edge_Pointsize %d def\n",Reset_edge.pointsize);
		printf("/coordfont /Times-Roman findfont 6 scalefont def\n");
		cat_libfile(DAGLIB_PS);
	}

	PS_scalefactor = 1.;
	if (User.width > 0) {
		PS_scalefactor =  min((User.width*Resolution)/Xmax,1.);
		if (User.height <= 0) User.height = (double)Ymax/Resolution;
		else PS_scalefactor = min((User.height*Resolution)/Ymax,PS_scalefactor);
	}
	if ((Page_size.x == 0) && (Page_size.y == 0) && (User.width != 0)) {
		PS_page_size.x = round(User.width * Resolution / PS_scalefactor);
		PS_page_size.y = round(User.height * Resolution / PS_scalefactor);
	}
	else {
		if (Page_size.x) {
			PS_page_size.x = round(Page_size.x / PS_scalefactor);
			PS_page_size.y = round(Page_size.y / PS_scalefactor);
		}
		else {
			PS_page_size.x = Xmax;
			PS_page_size.y = Ymax;
		}
	}
	PS_Margin.x = (int)(Margin.x/PS_scalefactor);
	PS_Margin.y = (int)(Margin.x/PS_scalefactor);
	printf("save\n");

	if (Default_node.pointsize != Reset_node.pointsize)
		printf("/dagnodefont dagnodefont %.3f scalefont def\n",
		Default_node.pointsize/(double)Reset_node.pointsize);
	if (Default_edge.pointsize != Reset_edge.pointsize)
		printf("/dagedgefont dagedgefont %.3f scalefont def\n",
		Default_edge.pointsize/(double)Reset_edge.pointsize);
	printf("/coordfont /Times-Roman findfont %d scalefont def\n",(int)(6/PS_scalefactor));
	if (PS_scalefactor != 1.) printf("%.3f %.3f scale\n",PS_scalefactor,PS_scalefactor);
	printf("/arrowlength %d def\n",Default_node.ysize/5);
	printf("/arrowwidth arrowlength 2 div def\n");
	unsquirrel();	// inside save region, so will not affect global defs
	printf("%%%%EndProlog\n");
}

void emit_ps_node_header() {
	printf("dagnodefont setfont\n");
}

void emit_ps_edge_header() {
	emit_ps_setink(Default_edge.ink);
	Current_PS_ink = Default_edge.ink;
	printf("dagedgefont setfont\n");
}

/* emit drawing code for an edge label */
void emit_ps_edgelabel(DAG_edge_t *e) {
	if (!e->label.type) return;	// no label
	if (e->pointsize != Default_edge.pointsize)
		printf("dagedgefont %.3f scalefont setfont\n",
		PS_scalefactor * e->pointsize / Default_edge.pointsize);
	Point midpoint = find_edge_midpoint(e);
	printf("%d %d moveto\n",midpoint.x,midpoint.y);
	switch (e->label.type) {
		case DESC:
			printf("gsave\n%s\ngrestore\n",e->label.value);
			break;
		case STRING:
			printf("(%s) show\n",escape(e->label.value));
	}
	if (e->pointsize != Default_edge.pointsize)
		printf("dagedgefront setfont\n");
}

void emit_ps_node(int node) {
	char *printname;
	if (Node[node]->label.type == STRING)
		printname = Node[node]->label.value;
	else if (Node[node]->label.type == DESC)
		printname = "";
	else printname = Node[node]->name;
	printf("%d %d moveto\n",Node[node]->pos.x,Node[node]->pos.y);
	if (Node[node]->color)
		printf("gsave %s setdagcolor\n",Node[node]->color);
	if (Node[node]->pointsize != Default_node.pointsize)
		printf("dagnodefont %.3f scalefont setfont\n",
			(PS_scalefactor*Node[node]->pointsize)/Default_node.pointsize);
	if (Node[node]->shape.type == STRING)
		printf("(%s) %d %d %s\n",escape(printname),Node[node]->xsize,Node[node]->ysize,Node[node]->shape.value);
	else printf("%s\n",Node[node]->shape.value);
	if (Node[node]->label.type == DESC) {
		printf("%d %d moveto\n",Node[node]->pos.x,Node[node]->pos.y);
		printf("%s\n",Node[node]->label.value);
	}
	if (Node[node]->pointsize != Default_node.pointsize)
		printf("dagnodefont setfont\n");
	if (Node[node]->color)
		printf("grestore\n");
}

/*
 * generate PS code to place node intersection on stack.
 *  intersection is the ray from rayorg through rayinter
 */
void emit_ps_nodeport(int node,Point rayorg, Point rayinter) {
	printf(" %d %d %d %d %d %d ",Node[node]->xsize,Node[node]->ysize,rayorg.x - Node[node]->pos.x,rayorg.y - Node[node]->pos.y,rayinter.x - Node[node]->pos.x,rayinter.y - Node[node]->pos.y);
	printf("%s_clip\n",Node[node]->shape.type == STRING? Node[node]->shape.value : Default_node.shape.value);
	printf("%d add exch %d add exch\n",Node[node]->pos.y,Node[node]->pos.x);
}

Point find_seg_midpoint(Point p0, Point p1) {
	Point rv;
	rv.x = (p0.x + p1.x) / 2;
	rv.y = (p0.y + p1.y) / 2;
	return rv;
}

void emit_ps_edge(int node, DAG_edge_t *e) {
	if (e->ink == invis_ink) return;
	int fromnode = node, tonode = e->node;
	if (e->ink != Current_PS_ink) emit_ps_setink(e->ink);
	printf("%% %s %s edge\n",Node[fromnode]->name,Node[tonode]->name);
	if (e->color != 0) {printf("gsave %s setdagcolor\n",e->color);}

	printf("newpath ");
	emit_ps_nodeport(fromnode,e->top,e->splinept[0]); printf("moveto\n");
	Point mp = find_seg_midpoint(e->splinept[0],e->splinept[1]);
	if (e->flipped)
		printf("%d %d makearrow\n",e->top.x,e->top.y);
	printf(" %d %d lineto\n",mp.x,mp.y);

	for (int i = 1; e->splinept[i + 1].x >= 0; i++) {
		Point mp = find_seg_midpoint(e->splinept[i],e->splinept[i+1]);
		printf("%d %d 2 copy %d %d curveto\n",
			e->splinept[i].x,e->splinept[i].y,mp.x,mp.y);
	}
	emit_ps_nodeport(tonode,e->bottom,e->splinept[i]);
	printf("lineto currentpoint stroke moveto\n");
	if (!e->flipped) 
		printf("%d %d makearrow\n",e->bottom.x,e->bottom.y);
	printf("stroke\n");

	if (e->color != 0) {printf("grestore\n");}
	emit_ps_edgelabel(e);
}

void emit_ps_trailer() {
	printf("restore\n");
}

void emit_ps_page() {
	printf("showpage\ngrestore\n");
}

int n_ps_pages() {
	int rv; 
	Pages_across = Xmax / PS_page_size.x;
	if (Pages_across != (double)Xmax/PS_page_size.x) Pages_across++;
	Pages_up = Ymax / PS_page_size.y;
	if (Pages_up != (double)Ymax/PS_page_size.y) Pages_up++;
	rv = Pages_across * Pages_up;
	return rv;
}

void emit_ps_set_page(int page) {
	int row	= page % Pages_up;
	int col	= (page - row) / Pages_up;
	Page_min.x = col * PS_page_size.x;
	Page_min.y = row * PS_page_size.y;
	Page_max.x = Page_min.x + PS_page_size.x;
	Page_max.y = Page_min.y + PS_page_size.y;
	printf("%%%%Page: %s %d\n",Current_file.name,++Page_number);
	printf("%%%%BoundingBox: %d %d %d %d\n",
		PS_Margin.x-1,PS_Margin.y-1,PS_Margin.x+PS_page_size.x+1,PS_Margin.y+PS_page_size.y+1);
	printf("gsave\n");
	/* page coord before clip so it can be in the margin if ever change this */
	if ((Pages_up > 1) || (Pages_across > 1))
		printf("coordfont setfont %d %d moveto (%d,%d) show\n",
			PS_Margin.x,PS_Margin.y,col,row);
	printf("%d %d moveto %d 0 rlineto 0 %d rlineto %d 0 rlineto closepath clip newpath\n",
		PS_Margin.x-1,PS_Margin.y-1,PS_page_size.x+2,PS_page_size.y+2,-PS_page_size.x-2);	// off by one pixel?
	printf("%d %d translate\n",PS_Margin.x - Page_min.x,PS_Margin.y - Page_min.y);
}

int node_in_page(int node) {
	int halfx = Node[node]->xsize / 2;
	int halfy = Node[node]->ysize / 2;
	for (int xcorner = Node[node]->pos.x - halfx; xcorner <=  Node[node]->pos.x + halfx; xcorner += halfx) {
		for (int ycorner = Node[node]->pos.y - halfy; ycorner <=  Node[node]->pos.y + halfy; ycorner += halfy) {
			if ((ycorner >= Page_min.y) && (ycorner <= Page_max.y)
					&& (xcorner >= Page_min.x) && (xcorner <= Page_max.x))
					return 1;
		}
	}
	return 0;
}

int edge_in_page(DAG_edge_t* e) {
	for (int i = 0; e->splinept[i+1].x >= 0; i++) {
		Point pt0 = e->splinept[i];
		Point pt1 = e->splinept[i+1];
		if (!(((pt0.x < Page_min.x) && (pt1.x < Page_min.x)) ||
			((pt0.x > Page_max.x) && (pt1.x > Page_max.x)) ||
			((pt0.y < Page_min.y) && (pt1.y < Page_min.y)) ||
			((pt0.y > Page_max.y) && (pt1.y > Page_max.y)))) return 1;
	}
	return 0;
}

void emit_ps() {
	int		node;
	DAG_edge_t *e;
	int		maxpage;

	emit_ps_header();
	maxpage = n_ps_pages();
	for (int page = 0; page < maxpage; page++) {
		emit_ps_set_page(page);
		emit_ps_node_header();
		for (node = 0; node < N; node++)
			if (node_in_page(node)) emit_ps_node(node);
		emit_ps_edge_header();
		for (node = 0; node < N; node++)
			for (e = Edge[node]; e; e = e->nextof())
				if (edge_in_page(e)) emit_ps_edge(node,e);
		emit_ps_page();
	}
	emit_ps_trailer();
}
