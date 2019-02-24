#include "jraw.h"
#include "thing.h"
#include "menu.h"

NMitem prims[] = {
	"box", "draw one box", 0, BOX,
	"macro", "enclose a macro", 0, MACRO,
/* let's forget about board for a while
	"dots", "guess!", 0, DOTS,
	"measure", "a box that tells its size", 0, MEASURE,
 */
	0
};
NMenu mprims = { prims };

NMitem objs[] = {
	"define",	"sweep a box to define a new object",	0, MASTER,
	"enter",	"edit the selected object",		0, ENTER,
	"exit",		"update edited master",			0, EXIT,
	0
};
NMenu mobjs = { objs };
NMitem item = {0, 0, 0, INST};

NMitem *instgen(int i)
{
	extern Dict *ml;
	item.text = (*ml)[i];
	return &item;
}

NMenu minst = { 0, &instgen};

NMitem toplevel[] = {
/*	"boxes",	"toggle drawing primitive",		0, BOX, */
	"onesies",	"draw one something",			&mprims, 0,
	"master",	"select object editing entry",		&mobjs, 0,
	"inst",		"deposit an instance",			&minst, 0,
	"sweep",	"selects items enclosed by a box",	0, SWEEP,
	"cut",		"put selected items in buffer",		0, CUT,
	"paste",	"paste copy of buffer",			0, PASTE,
	"snarf",	"cut without deleting original",	0, SNARF,
	"scroll",	"move viewing window",			0, SCROLL,
	0
};
NMenu mtop = { toplevel };

char *instname;		/* a global to put hints in */
meanyhit(int but)
{
	register NMitem *m = mhit(&mtop, but, 0);
	instname = m->text;
	return (m == 0) ? -1 : m->result;
}
