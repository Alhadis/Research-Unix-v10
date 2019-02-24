#include "master.pri"
#include "process.pri"
SRCFILE("master.c")

char *Master.kbd(char*s)		{ return PadRcv::kbd(s); }
char *Master.help()			{ return PadRcv::help(); }

Master.Master()
{
	trace( "%d.Master()", this );
	child = 0;
	core = 0;
}

Process *Master.makeproc(char* proc, char *stab, char *comment)
{
	Process *p;

	for( p = child; p; p = p->sibling )
		if( !p->isdead
		 && eqstr(proc,p->procpath)
		 && eqstr(stab,p->stabpath) ){
			p->comment = sf("%s", comment);
			insert(p);
			return p;
		}
	child = domakeproc(proc, stab, comment);
	child->master = this;
	insert(child);
	return child;
}

void Master.insert(Process *p)
{
	trace( "%d.insert(%d)", this, p ); VOK;
	IF_LIVE( !p || p->disc()!=U_PROCESS ) return;
	if( p->isdead ){
		pad->removeline( (long)p );
		return;
	}
	char *pp = p->procpath;
	char *sp = p->stabpath;
	char *ct = p->comment;
	pad->insert( (long)p, SELECTLINE, (PadRcv*)p, p->carte(),
		"%s %s %s", pp?pp:"", sp?sp:"", ct?ct:"");
}

Process *Master.search(char *path)
{
	Process *p;
	OK(0);
	for( p = child; p; p = p->sibling )
		if( eqstr(path, p->procpath) && p->core ) return p;
	return 0;
}
