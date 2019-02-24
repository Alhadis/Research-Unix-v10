#include <pads.pri>
SRCFILE("menu.c")

Index ZIndex; /* = {0,0}; */
int Index.null() { return !(major|minor); }

Menu.Menu() { list = 0; size = 0; }

Menu.Menu(char *t, Action a, long o)
{
	trace( "%d.Menu(%s,%d,%d)", this, t, a, o );
	list = 0;
	size = 0;
	first(t, a, o);
}

Menu.~Menu()
{
	IList *l, *lnext;

	for( l = list; l; l = lnext ){
		lnext = l->next;		// new malloc()
		delete l;
	}
}

void Menu.first(char *t, Action a, long o)
{
	trace( "%d.first(%s,%d,%d)", this, t, a, o );
	first( ICache->place( Item(t,a,o) ) );
}

void Menu.last(char *t, Action a, long o)
{
	trace( "%d.last(%s,%d,%d)", this, t, a, o );
	last( ICache->place( Item(t,a,o) ) );
}

void Menu.first(Index i)
{
	trace( "%d.first(%u:%u)", this, i.major, i.minor );
	if( i.null() ) return;
	list = new IList(i,list);
	++size;
}

void Menu.last(Index i)
{
	IList *l;

	trace( "%d.last(%u:%u)", this, i.major, i.minor );
	if( i.null() ) return;
	++size;
	if( !list ){
		list = new IList(i,0);
		return;
	}
	for( l = list; l->next; l = l->next ) {}
	l->next = new IList(i,0);
}

int IndexTextCmp( Index a, Index b )
{
	trace( "IndexTextCmp(%u:%u,%u:%u)", a.major, a.minor, b.major, b.minor );
	return strcmp( ICache->take(a)->text, ICache->take(b)->text );
}

void Menu.sort(char *t, Action a, long o )
{
	int cmp;
	IList **p;

	trace( "%d.sort(%s,%d,%d)", this, t, a, o );
	Index i = ICache->place( Item(t,a,o) );
	if( !list ){
		first(i);
		return;
	}
	for( p = &list; *p; p = &((*p)->next) ){
		trace( "%u:%u", (*p)->index.major, (*p)->index.minor );
		cmp = IndexTextCmp( i, (*p)->index );
		if( cmp == 0 ) return;
		if( cmp < 0 ) break;
	}
	++size;
	*p = new IList(i,*p);
}

Index Menu.index(char *t, Action a, long o)
{
	IList *l;
	int i;
	Carte *c;
	
	trace( "%d.index() %d", this, size );
	if( size==0 ) return 0;
	c = (Carte *) new char [CARTESIZE(size)];
	c->attrib = 0;
	c->size = size;
	for( l = list, i = 1; l; l = l->next, ++i )
		c->bin[i] = l->index;
	if( t )
		c->bin[0] = ICache->place(Item(t,a,o));
	Index ix = CCache->place(c);
	delete c;
	return ix;
}

void Menu.dump()
{
	IList *l;

	trace( "%d.dump()", this );
	for( l = list; l; l = l->next ){
		trace( "%u:%u", l->index.major, l->index.minor );
	}
}
