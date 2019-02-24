#include "draw_dag.h"
#include "dag.h"
#include "parsedag.h"
#include "defaults.h"

DAG_edge_t::DAG_edge_t() : () {
	color	=	Default_edge_color;
	ink		=	Default_edge_ink;
	pointsize	=	Default_edge_pointsize;
}

void DAG_edge_t::setlabel(int intype, char *invalue) {
	label.type 		= intype;
	label.value 	= invalue;
}

void DAG_edge_t::setcolor(char* incolor) {
	color			= incolor;
}

void DAG_edge_t::setweight(int inweight) {
	weight			= inweight;
}

void DAG_edge_t::setink(dag_ink_t inink) {
	ink				= inink;
}

void DAG_edge_t::setpointsize(int inpointsize) {
	pointsize = inpointsize;
}
