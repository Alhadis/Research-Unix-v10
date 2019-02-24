#include "draw_dag.h"
#include "dag.h"
#include "parsedag.h"
#include "defaults.h"

boolean shape_t::is_fixed_aspect_ratio() {
	switch(shape_id) {
		case Square:
		case Circle:
		case Doublecircle:
			return true;
		default:
			return false;
	}
}

DAG_node_t::DAG_node_t() : shape(STRING,"Ellipse") {
	xsize 		= Default_node_xsize;
	ysize		= Default_node_ysize;
	pointsize	= Default_node_pointsize;
	color		= Default_node_color;
	name		= "";
	label.type	= 0;
	sethw();
}

void DAG_node_t::setname(char *invalue){
	name 	=	invalue;
	autosize();
}

void DAG_node_t::setshape(int intype, char* invalue) {
	shape = shape_t(intype,invalue);
	autosize();
}

void DAG_node_t::setcolor(char *incolor) {
	color	=	incolor;
}

void DAG_node_t::setlabel(int intype, char* invalue) {
	label.type	=	intype;
	label.value	=	invalue;
	autosize();
}

void DAG_node_t::setxsize(int inxsize) {
	xsize		=	inxsize;
	autosize();
}

void DAG_node_t::setysize(int inysize) {
	ysize		=	inysize;
	autosize();
}

void DAG_node_t::setpointsize(int inpointsize) {
	pointsize	=	inpointsize;
	autosize();
}

void DAG_node_t::autosize() {
	char *printname;
	int spaceneeded = 0;
	if (label.type != DESC) {
		if (label.type == STRING) printname = label.value;
		else printname = name;
		if (printname) {
			if (*printname)
				spaceneeded = (int)((2 + strlen(printname)) * pointsize * IPP * Resolution);
			/* would be better if we kept default node size at time of its creation */
			xsize = max(spaceneeded,xsize);
			spaceneeded = (int) (pointsize * IPP * 2 * Resolution);
			ysize = max(spaceneeded,ysize);
		}
	}
	if (shape.is_fixed_aspect_ratio()) ysize = xsize;
	sethw();
}

void DAG_node_t::sethw() {
	if (!User.rotated) {
		width = xsize;
		height = ysize;
	}
	else {
		width = ysize;
		height = xsize;
	}
}
