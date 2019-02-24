#include "graphtypes.h"
#include <stdio.h>

AddEdge (g,n,m,label,graphics)
GRAPH g;
int n,m;
char label[];
char graphics;
/* add edge to front of adjacency list */

{	edge_entry edge1,edge2;

	edge1 = (edge_entry) malloc(sizeof(struct edge_ent));
	edge2 = (edge_entry) malloc(sizeof(struct edge_ent));

	strcpy(edge1->label, label);
	edge1->endpoint = m;
	edge1->graphics = graphics;
	edge1->otheredge = edge2;
	edge1->prevedge = NULL;
	edge1->nextedge = g[n].adj_list;
	if (edge1->nextedge != NULL)
		edge1->nextedge->prevedge = edge1;
	g[n].adj_list = edge1;
	g[n].degree++;

	strcpy(edge2->label, label);
	edge2->endpoint = n;
	edge2->graphics = graphics;
	edge2->otheredge = edge1;
	edge2->prevedge = NULL;
	edge2->nextedge = g[m].adj_list;
	if (edge2->nextedge != NULL)
		edge2->nextedge->prevedge = edge2;
	g[m].adj_list = edge2;
	g[m].degree++;
}


GRAPH NewGraph(size)
int size;
{	GRAPH tmp;
	int i;

	tmp = (GRAPH) malloc((size+1)*sizeof(struct node_entry));
	for (i=1; i<=size; i++) {
		Degree(tmp,i) = 0;
		FirstEdge(tmp,i) = NULL;
		VertGraphics(tmp,i) = 'L';
		}
	return(tmp);
}


/* Graph I/O routines */

GRAPH Readgraph (size,file)
int *size;
char file[];

{	GRAPH graph;
	FILE *fp, *fopen();
 	char c, vgraphics[2], egraphics[2];
	char vlabel[MAXLABELSIZE], elabel[MAXLABELSIZE];
	int nodes, edges, degree, adj_node, i, j, k;
	int xcoord, ycoord;

	fp = fopen(file,"r");
	if (fp==NULL) {
		printf("Readgraph: file %s can't be opened\n",file);
		exit(0);
		}
	fscanf(fp,"%d%d",&nodes,&edges);
	while ((c=getc(fp))!='\n') ;

	*size = nodes;
	graph = NewGraph(nodes);
	for (i = 1; i <= nodes; ++i) {
		fscanf(fp,"%d%s%d%d%1s",&degree,vlabel,&xcoord,&ycoord,vgraphics);
		strcpy(NLabel(graph,i), vlabel);
		Xcoord(graph,i) = xcoord;
		Ycoord(graph,i) = ycoord;
		VertGraphics(graph,i) = vgraphics[0];

		while ((c=getc(fp))!='\n') ;
		for (j = 1; j <= degree; ++j) {
			fscanf(fp,"%d%s%1s", &adj_node, elabel, egraphics);
			while ((c=getc(fp))!='\n') ;
			if (i<adj_node)
				AddEdge (graph,i,adj_node,elabel,egraphics[0]);
			}
		}
	fclose(fp);
	return(graph);
}


Scalegraph (graph,size,xlim,ylim)
GRAPH graph;
int size;
float xlim,ylim;
/* scales the coordinates of the given graph to range from 0 to the x and y */
/* values given (xlim, Ylim) */
{	float xlow = 2000, ylow = 2000;
	float xhi = 0, yhi = 0;
	float xscale,yscale,scale;
	int i,x,y;

	for (i=1; i<=size; i++) { /* find hi and low corr. values */
		if ((x = Xcoord(graph,i)) > xhi)
			xhi = x;
		if (x < xlow)
			xlow = x;
		if ((y = Ycoord(graph,i)) > yhi)
			yhi = y;
		if (y < ylow)
			ylow = y;
	}

	if ( xlow == xhi) /* check for division by zero */
		xscale = 1;
	else
		xscale = xlim / (xhi - xlow);
	if (ylow == yhi)
		yscale = 1;
	else
		yscale = ylim / (yhi - ylow);

	if (xscale < yscale)
		scale = xscale;
	else
		scale = yscale;

	for (i=1; i<=size; i++) {
		Xcoord(graph,i) = (Xcoord(graph,i) - xlow) * xscale;
		Ycoord(graph,i) = (Ycoord(graph,i) - ylow) * yscale;
	}
}
