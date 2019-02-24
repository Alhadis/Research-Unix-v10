#include "univ.h"

extern Script ObjScript;
Script Generic;
Index CIndex;
Entry *CarteEntry();

Entry *ObjectEntry(i)
{
	return CIndex ? CarteEntry(CIndex,i): (Entry*)0;
}

ObjectLimits(s)
Script *s;
{
	register Carte *c;

	if( !CIndex ) return;
	assert( MJR(CIndex)&CARTE );
	c = IndexToCarte(CIndex);
	s->items = c->size;
	s->width = c->width;
}

Entry *CarteEntry(ix, i)
Index ix;
register short i;
{
	static Entry e;
	static Script subscript[4];
	extern int MenuNest;

	register Carte *nest, *c;
	register int j;
	register Script *s;
	Index nix;

	assert( MJR(ix)&CARTE );
	c = IndexToCarte(ix);
	if( i >= c->size ) return 0;
	e.script = 0;
	if( c->attrib&NUMERIC ){
		e.action = HostNumeric;
		e.text = itoa( (int) (e.opand = i + (short) c->bin[1]) );
		return &e;
	}
	for( j = 1; ; ++j ){
		assert( i>=0 && j<=c->size );
		if( nix=c->bin[j], MJR(nix)&CARTE ){
			nest = IndexToCarte(nix);
			if( nest->bin[0] ){
				if( i == 0 ){
					e.action = HostAction;
					e.opand = (long) &nest->bin[0];
					e.text = IndexToStr(nest->bin[0]);
					e.script = s = &subscript[MenuNest];
					s->cindex = nix;
					s->generator = ObjectEntry;
					s->limits = ObjectLimits;
					return &e;
				}
				--i;
			} else {
				if( i < nest->size )
					return CarteEntry(nix,i);
				i -=  nest->size;
			}
		} else {
			if( i == 0 ){
				e.action = HostAction;
				e.opand = (long) &c->bin[j];
				e.text = IndexToStr(c->bin[j]);
				return &e;
			}
			--i;
		}
	}
}

Entry *SubEntry(i)
register i;
{
	int MakeCurrent(); extern Pad Sentinel;
	register Pad *p;
	static Entry e  = { 0, MakeCurrent, 0 };

	for( p = Sentinel.back; p != &Sentinel && i-- > 0; p = p->back ){}
	if( p == &Sentinel ) return (Entry *) 0;
	e.text = p->name;
	e.opand = (long) p;
	return &e;
}

SubLimits(s)
register Script *s;
{
	extern Pad Sentinel;
	register Pad *p;
	register l;

	s->items = 0;
	s->width = 1;
	for( p = Sentinel.back; p != &Sentinel ; p = p->back ){
		++s->items;
		l = strlen(p->name);
		if( l > s->width ) s->width = l;
	}
}

Script SubScript = { SubEntry, SubLimits };

Entry *PadEntry(i)
register i;
{
	static Entry e[] = {
		{ "\376",	0,		0 },
	  	{ "reshape",	Reshape,	0 },
	   	{ "move",	Move,		0 },
		{ "close",	DeletePick,	0 },
		{ "(fold)",	0,		0 },
		{ "top",	0,		0,	&SubScript}
	};

	if( i > 5 ) return (Entry *) 0;
	if( i == 4 && Current ) return FoldEntry(&Current->attributes);
	return e+i;
}

Entry *LineEntry(i)
{
	static Entry e [] = {	{ "\376",	0,		0 },
				{ "cut",	CutLine,	0 },
				{ "sever",	Sever,		0 }, 	};

	if( i<3 ) return e+i;
	if( i==3 && Selected.line)
		return FoldEntry(&Selected.line->attributes);
	return 0;
}
	
Script	ObjScript = { ObjectEntry, ObjectLimits, &Generic };

MOUSEServe()
{
	register int but;
	register Line *lop;	/* Line or Pad! */
	register Pad *p;
	register Entry *e = 0;
	register Script *s;
	register char *confirm = "confirm: ";

	if( !own(MOUSE) || !ptinrect(mouse.xy,Drect) ) return;
Again:
	switch( butts ){
	case BUTT1:
		Pointing();
		if( !button23() ) goto AllDone;
		while( button1() ) nap(2);
		goto Again;
	case BUTT2:
		but = 2;
		Generic.generator = LineEntry;
		if( lop = Selected.line )
			MakeCurrent(Selected.pad);
		break;
	case BUTT3:
		if( (p = PickPad(mouse.xy)) && (p != Current) ){
			MakeCurrent(p);
			goto Again;
		}
		Generic.generator = PadEntry;
		lop = (Line*) Current;
	}
	switch( butts ){
	case BUTT3:
		but = 3;
	case BUTT2:
		ObjScript.cindex = 0;
		if( lop ){
			if( lop->ptop != 255 ){
				ObjScript.prevtop = lop->ptop;
				ObjScript.prevhit = lop->phit;
			}
			ObjScript.cindex = lop->carte;
			HostObject = lop->object;
			HostParent = Current ? Current->object : (long)0;
		}
		if( e = ScriptHit(&ObjScript, but, (RectList*)0) ){
			if( lop ){
				lop->ptop = ObjScript.prevtop;
				lop->phit = ObjScript.prevhit;
			}
			if( e->text[strlen(e->text)-1] == '?' ){
				cursswitch(&Danger);
				if( Configuration&NOVICEUSER )
					InvertKBDrect( confirm, e->text );
				while( !butts ) waitMOUSE();
				if( !button(but) ) e = 0;
				while( butts ) waitMOUSE();
				if( Configuration&NOVICEUSER )
					PaintKBD();
				cursswitch( Cursor );
			}
		}
		if( e && e->action ){
			if( but == 3 ) FlashBorder(lop);
			(e->action)(e->opand);
			if( but == 2 ) Select((Line*)0, (Pad*) 0);
		}
	}
AllDone:
	while( butts ) waitMOUSE();
}

FlashBorder(p)
Pad *p;
{
	HeavyBorder(p);
	sleep(5);
	HeavyBorder(p);
}
