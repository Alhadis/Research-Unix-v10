#include "univ.h"

DoCut(l)
register Line *l;
{
	if( l->attributes & USERCUT ){
		HostParent = Selected.pad->object;
		HostObject = l->object;
		ToHost( P_USERCUT /*, garbage */ );
	}
	if( !(l->attributes&DONT_CUT) ) DelLine(l);
}

CutLine()
{
	if(!Selected.line) return;
	DoCut(Selected.line);
	Paint(Selected.pad);
}

Sever()
{
	register Line *l = Selected.line, *lu;

	if(!l) return;
	for( ; l != &Selected.pad->sentinel; l = lu ){	/* ISLINE */
		lu = l->up;
		DoCut(l);
	}
	Paint(Selected.pad);
}
