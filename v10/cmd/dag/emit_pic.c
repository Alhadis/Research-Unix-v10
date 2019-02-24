/*
 *	emit pic.
 */

#include "draw_dag.h"
#include "dag.h"
#include "parsedag.h"
#include "paths.h"

static char* escape(char *s) {
	static char buf[BUFSIZ];
	char *p = buf;
	while (*s)  {
		if (*s == '\"') *p++ = '\\';
		*p++ = *s++;
	}
	*p = '\0';
	return buf;
}

char *expandesc(char*d, int node) {
	static char buf[BUFSIZ];
	char * p = buf;
	char *printname;
	if (Node[node]->label.type == STRING)
		printname = Node[node]->label.value;
	else
		printname = Node[node]->name;
	
	while (*p = *d) switch(*p) {
		case '\\':
			if (!(*p++ = *++d)) return buf;
			d++;
			break;
		case '$':
			d++;
			int m = 0;
			if (!strncmp(d,"NAMELEN",m=7)) sprintf(p,"%d",strlen(printname));
			else if (!strncmp(d,"NAME",m=4)) {
				if (Node[node]->pointsize != Default_node.pointsize)
					sprintf(p,"\\s%d\"%s\"\\s0",Node[node]->pointsize,
						escape(printname));
				else sprintf(p,"\"%s\"",escape(printname));
			}
			else if (!strncmp(d,"WIDTH",m=5)) sprintf(p,"%f",inchof(Node[node]->xsize));
			else if (!strncmp(d,"HEIGHT",m=6)) sprintf(p,"%f",inchof(Node[node]->ysize));
			else {p++; break;}
			while (*p) p++;
			d += m;
			break;
		default:
			p++; d++; break;
	}
	return buf;
}

char *pic_inkstr(dag_ink_t ink) {
	switch(ink) {
		case solid_ink:		return "";	// Sys V pic doesn't like "solid".
		case dashed_ink:	return "dashed";
		case dotted_ink:	return "dotted";
		case invis_ink:		return "invis";
	}
}

char* find_cip_shape(shape_id_t s) {
	switch (s) {
		case Box:
		case Square:
			return "box wid %f ht %f";
		case Diamond:
			return "d = %f\ne=%f\nmove down e/2; line up e/2 right d/2 then up e/2 left d/2 then down e/2 left d/2 then down e/2 right d/2";
		case Circle:
			return "circle rad %f/2";
		case Doublecircle:
			return "circe rad %f/2; circle rad .9*%f/2 at last circle.c";
		case User_defined:
		case Ellipse:
		default:
			return "ellipse wid %f ht %f";
		case Plaintext:
			return "box invis wid %f ht %f";
	}
}

void emit_pic_pt(Point p) {
	printf("(%f,%f)",inchof(p.x),inchof(p.y));
}

void emit_pic_header() {

	static int ordinal = 0;
	double sf = 1.;
	if (User.width <= 0) printf(".PS\n");	// no args
	else {
		sf =  min((User.width*Resolution)/Xmax,1.);
		if (User.height <= 0) User.height = (double)Ymax/Resolution;
		else sf = min((User.height*Resolution)/Ymax,sf);
		if (sf == 1.) printf(".PS\n");
		else {
			User.width  = min(User.width,sf*(double)Xmax/Resolution);
			User.height = min(User.height,sf*(double)Ymax/Resolution);
			Default_node.pointsize = (int)(Default_node.pointsize * sf);
			for (int i = 0; i < N; i++)
				Node[i]->pointsize = (int)(Node[i]->pointsize * sf);
			printf(".PS %f %f\n",User.width,User.height);
		}
	}

	printf(".lf %d\n",Current_file.line_number + 1);
	printf("arrowht = %f;\n", inchof((int)(Default_node.xsize*.15)));
	printf("arrowwid  = %f;\n", inchof((int)(Default_node.xsize*.075)));
	if (!ordinal++ && (Output_type != Cip))
		cat_libfile(DAGLIB_PIC);
	unsquirrel();
}

void emit_pic_node_header() {
	printf(".ps %d\n",Default_node.pointsize);
}

void emit_pic_edge_header() {
	if (Default_edge.pointsize != Default_node.pointsize)
		printf(".ps\n.ps %d\n",Default_edge.pointsize);
}

void emit_pic_node(int node) {
	shape_t shape = Node[node]->shape;
	char *printname;
	if (Node[node]->label.type == DESC) printname = "";
	else if (Node[node]->label.type == STRING) printname = Node[node]->label.value;
	else printname = Node[node]->name;

	if (shape.type == DESC) {
		printf("Node%d: [ %s ] with .c at ",node,expandesc(Node[node]->shape.value,node));
		emit_pic_pt(Node[node]->pos);
		printf(";\n");
	}
	else {
		if (Output_type == Cip)  {
			printf("[ ");
			printf(find_cip_shape(Node[node]->shape.shape_id),
				inchof(Node[node]->xsize),inchof(Node[node]->ysize));
			printf("] with .c at ");
			emit_pic_pt(Node[node]->pos);
			printf(";\nmove to ");
			emit_pic_pt(Node[node]->pos);
			printf("; \"%s\";\n",escape(printname));
		}
		else {
			printf("Node%d: %s(\"%s\",%f,%f) at ",node,Node[node]->shape.value,
			escape(printname),inchof(Node[node]->xsize),inchof(Node[node]->ysize));
			emit_pic_pt(Node[node]->pos);
			printf(";\n");
		}
	}
	if (Node[node]->label.type == DESC) {
		printf("move to ");
		emit_pic_pt(Node[node]->pos);
		printf("%s;\n",Node[node]->label.value);
	}
}

void emit_pic_edgelabel(DAG_edge_t *e) {
	if (!e->label.type) return;
	printf("move to ");
	emit_pic_pt(find_edge_midpoint(e));
	printf("; ");
	if (e->label.type == DESC) printf("%s;\n",e->label.value);
	else printf("\"%s\";\n",escape(e->label.value));
}

void emit_pic_edge(int node, DAG_edge_t *e) {
	if (e->ink == invis_ink) return;
	Point intersection;
	int fromnode = node, tonode = e->node;
	intersection = find_nodeport(fromnode,e->top,e->splinept[0]);
	printf("spline %s %s from ",e->flipped?"<-":"->",
		(Output_type != Cip? pic_inkstr(e->ink) : "") );
	emit_pic_pt(intersection);

	for (int i = 1; e->splinept[i+1].x >= 0; i++) {
		printf(" to ");
		emit_pic_pt(e->splinept[i]);
	}
	intersection = find_nodeport(tonode,e->bottom,e->splinept[i]);
	printf(" to ");
	emit_pic_pt(intersection);
	printf(";\n");
	emit_pic_edgelabel(e);
}

void emit_pic_trailer() {
	printf(".ps\n.PE\n");
	printf(".lf %d\n",Current_file.line_number + 1);
}

void emit_pic() {
	int node;
	DAG_edge_t *e;

	emit_pic_header();
	emit_pic_node_header();
	for (node = 0; node < N; node++) emit_pic_node(node);
	emit_pic_edge_header();
	for (node = 0; node < N; node++)
		for (e = Edge[node]; e; e = e->nextof())
			emit_pic_edge(node,e);
	emit_pic_trailer();
}
