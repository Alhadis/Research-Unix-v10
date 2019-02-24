#include "univ.h"
Selection Selected;
Pad *Current;
long HostObject;
long HostParent;
short Scrolly;
Point Zpoint;
Rectangle ZRectangle;
Pad *DirtyPad;

Pad Sentinel = { 0, 0, 0, 0, 0, &Sentinel, &Sentinel };
Rectangle PadSpace;
Rectangle KBDrect;
Attrib NewFold;

#define ISPAD(p) ((p) != &Sentinel)
#define ISLINE(l,p) ((l) != &(p)->sentinel)	/* used how much? */

Point PickPoint(s)
char *s;
{
	char *pick = "pick window: ";
	Point p;
	cursswitch(&Bullseye);
	if( s ) InvertKBDrect( pick, s );
	while( !button123() ) waitMOUSE();
	p =  (butts==BUTT2 || butts==BUTT3) ? mouse.xy: Zpoint;
	cursswitch( Cursor );
	PaintKBD();
	return p;
}

#define MINWD 50
#define MINHT 50
PadSized( r )
Rectangle r;
{
	return r.corner.x-r.origin.x > MINWD && r.corner.y-r.origin.y > MINHT;
}

#define SWEEP 125
int SWEEPTIMEOUT = 60*15;
Rectangle clipgetrect(s)
register char *s;
{
	Rectangle r, r1;
	register long start;
	Point p1, p2;

	if( s ) InvertKBDrect( "sweep ", s );
	for( start = realtime(); ; sleep(2) ){
		if( own()&MOUSE && ptinrect(mouse.xy, inset(PadSpace,-SWEEP)) )
			break;
		if( realtime()>start+SWEEPTIMEOUT ){
			r = Drect;
			while( PadSized(r1= inset(r,1)) )
				r = r1;
			goto TimedOut;
		}
	}
	r = getrect23();
	if( r.corner.x && r.corner.x-r.origin.x<=5 && r.corner.y-r.origin.y<=5 )
		r = Drect;	/* from jim */
TimedOut:
	PaintKBD();
	if( !rectclip( &r, PadSpace ) ) return ZRectangle;
	return r;
}

DelLine( l )
register Line *l;
{
	assert( l && l->down && l->up );

	if( Selected.line == l ) Selected.line = 0;
	l->down->up = l->up;
	l->up->down = l->down;
	gcfree( l->text );
	free( l );
}

Line *InsAbove(l, t)
register Line *l;
register Line *t;
{
	register Line *n;

	assert( l && l->down && l->up );
	if( !t->text ) return 0;		/* || !t->text[0] allow */
	n = salloc(Line);
	*n = *t;
	GCString(&n->text, t->text);
	n->down = l;
	n->up = l->up;
	l->up->down = n;
	l->up = n;
	return n;
}

Line *InsPos(p, tk)
register Pad *p;
register Line *tk;
{
	register Line *l = &p->sentinel;	

	assert( p && tk );
	if( (p->attributes&SORTED) && tk->text ){
		while( ISLINE(l->up,p) && !dictorder(l->up->text,tk->text) )
			l = l->up;
	} else {
		while( ISLINE(l->up,p) && l->up->key > tk->key )
			l = l->up;
	}
	return l;
}

CreateLine(p)
register Pad *p;
{
	register long lo, hi, k;
	register Line *inspos, *l;
	Line fake;
	register char tilde;

	lo = RcvLong();
	hi = RcvLong();
	if( p->sentinel.key || p->attributes&SORTED ) return;
	tilde = p->attributes&NO_TILDE ? 0 : '~';
	for( k = lo; k <= hi; ++k ) if( k ){
		fake.key = k;
		fake.carte = 0;
		fake.attributes = FAKELINE;
		fake.text = itoa(k);
		*fake.text = tilde;
		inspos = InsPos(p, &fake);
		if( inspos->up->key == k )
			DelLine( inspos->up );
		InsAbove(inspos, &fake);
		p->attributes |= FAKELINE;
		Dirty(p);
	}
}	

PutLine(p,op)
register Pad *p;
Protocol op;
{
	static Line prevrcvd;
	Line rcvd;
	char text[256];			/* 256 ? */
	register Line *l, *inspos;

	rcvd = prevrcvd;
	rcvd.object = RcvLong();
	if( op == P_NEXTLINE )
		rcvd.key = ++prevrcvd.key;
	else {
		rcvd.key = RcvLong();
		rcvd.carte = RcvShort();
		rcvd.attributes = RcvShort();
		prevrcvd = rcvd;
	}
	RcvString(rcvd.text = text);
	if( !p ) return;
	if( p->sentinel.key && rcvd.key>p->sentinel.key ) return;
	inspos = InsPos(p, &rcvd);
	{
	extern Script ObjScript;
	rcvd.phit = ObjScript.prevhit;
	rcvd.ptop = ObjScript.prevtop;
	}
	if( rcvd.attributes&SELECTLINE ) MakeCurrent(p);
	for( l = p->sentinel.up; ISLINE(l,p); l = l->up ){
		if( l->key == rcvd.key ){
			if( l == Selected.line )
				rcvd.attributes |= SELECTLINE;
			if( !strcmp(rcvd.text, l->text)
			 && rcvd.object == l->object
			 && rcvd.carte == l->carte
			 && rcvd.attributes == l->attributes
			 && !(rcvd.attributes&SELECTLINE) )
				return;
			inspos = l->down;
			if( NewFold = l->attributes & (FOLD|TRUNCATE) )
				FoldToggle( &rcvd.attributes );
			rcvd.phit = l->phit;
			rcvd.ptop = l->ptop;
			DelLine( l );
			break;
		}
	}
	l = InsAbove(inspos, &rcvd);
	if( !(rcvd.attributes&DONT_DIRTY) ) Dirty(p);
	if( l && rcvd.attributes&SELECTLINE ){	/* selected <= "" !! */
		Selected.line = l;
		Selected.pad = p;
		Paint(p);
	}
}

SetCurrent( p )
register Pad *p;
{
	if( Current ) HeavyBorder(Current);
	if( Current = p ) HeavyBorder( p );
}

PaintCurrent()
{
	if( Current ) Paint(Current);
}

LineXOR( l )
Line *l;
{
	rectf( &display, l->rect, F_XOR );
}

Cover Covered( p )	/* should be smarter */
register Pad *p;
{
	register Pad *f;

	for( f = p->front; ISPAD(f); f = f->front )
		if( rectinrect( p->rect, f->rect ) ) return COMPLETE;
	for( f = p->front; ISPAD(f); f = f->front )
		if( rectXrect( p->rect, f->rect ) ) return PARTIAL;
	return CLEAR;
}

Dirty(p)
Pad *p;
{
	if( p == DirtyPad ) return;
	if( DirtyPad && Covered(DirtyPad)!=COMPLETE )
		if( !ClipPaint(DirtyPad->rect, DirtyPad) )
			PaintForward( DirtyPad->rect, DirtyPad->front );
	DirtyPad = p;
}

Linkin(p)
register Pad *p;
{
	SetCurrent( (Pad *) 0 );
	p->back = Sentinel.back;
	p->back->front = p;
	p->front = &Sentinel;
	Sentinel.back = p;
	if( PadSized(p->rect) ) SetCurrent(p);
}

Unlink(p)
register Pad *p;
{
	if( p == Current ) SetCurrent( (Pad *) 0 );
	p->back->front = p->front;
	p->front->back = p->back;
	p->front = p->back = 0;		/* redundant - but caught an mcc bug! */
}

FrontLink(p)
register Pad *p;
{
	if( !p ) return;
	Unlink(p);
	Linkin(p);
}

PaintForward( r, p )
Rectangle r;
register Pad *p;
{
	register busy = 0;
	for( ; p != &Sentinel; p = p->front ){
		if( rectXrect(r, p->rect) && Covered(p)!=COMPLETE ){
			if( !ClipPaint( r, p ) )
				r = boundrect(r,p->rect);
			if( ++busy%4 == 0 ) wait(CPU);
		}
	}
}

Refresh( r )
Rectangle r;
{
	rectf( &display, r, F_CLR );
	PaintForward( r, Sentinel.front );
}

char NewString[] = "<new>";

void P_Define( p, o )
register Pad *p;
long o;
{
	if( !p ){
		p = salloc(Pad);		/* zeros */
		p->sentinel.up = p->sentinel.down = &p->sentinel;
		p->sentinel.ptop = 255;
		Linkin(p);
		p->object = o;
		p->name = NewString;
		p->sentinel.text = NewString;
		p->tabs = 8;
		p->selkey = 0;
	}
	Dirty(p);
}

void P_Carte(p)
register Pad *p;
{
	register Index i = RcvShort();
	if( p && p->object ) p->carte = i;
}

void P_Lines(p)
register Pad *p;
{
	register long k = RcvLong();
	if( p ){
		if( k < p->sentinel.key ) DelLines(p);
		p->sentinel.key = k;
		Dirty(p);
	}
}

void P_Banner(p)
register Pad *p;
{
	char b[256];

	RcvString(b);
	if( p ){
		if( p->sentinel.text != NewString ) gcfree( p->sentinel.text );
		GCString( &p->sentinel.text, b );
		Dirty(p);
	}
}

void P_Name(p)
register Pad *p;
{
	char n[256];

	RcvString(n);
	if( p ){
		if( p->name != NewString ) gcfree( p->name );
		GCString( &p->name, n );
	}
}

void P_Attributes(p)
register Pad *p;
{
	register Attrib a = RcvShort();
	if( p ) p->attributes = a;
}

void P_Tabs(p)
register Pad *p;
{
	register short t = RcvShort();
	if( p && t>0 && t<128 ) p->tabs = t;
	Dirty(p);
}

void P_RemoveLine(p)
register Pad *p;
{
	register long k = RcvLong();
	register Line *l;

	if( !p ) return;
	for( l = p->sentinel.up; ISLINE(l,p); l = l->up ){
		if( l->key == k ){
			DelLine( l );
			Dirty(p);
			return;
		}
	}
	
}


Pad *ObjToPad(o)
register long o;
{
	register Pad *p;

	for( p = Sentinel.back; ISPAD(p); p = p->back )
		if( p->object == o ) return p;
	return 0;
}

Cycle()
{
	register Pad *p;

	for( p = Sentinel.back; ISPAD(p); p = p->back ){
		if( p->ticks>0 ){
			if( --p->ticks == 0 ){
				HostParent = HostObject = p->object;
				ToHost( P_CYCLE /*, garbage */ );
			}
		}
	}
}

MakeGap(p)
Pad *p;
{
	register Line *l, *lsent = &p->sentinel;
	register long k = RcvLong();
	register long gap = RcvLong();

	for( l = lsent->down; l!=lsent; l = l->down )
		if( l->key >= k ) l->key += gap;
}

PadOp(op)
Protocol op;
{
	static long LINEobj;
	register long obj;
	register Pad *p;
	register short t;

	obj = op == P_NEXTLINE ? LINEobj : RcvLong();
	p = ObjToPad( obj );
	switch( (int) op ){
	case P_PADDEF:
		P_Define(p,obj);
		break;
	case P_ATTRIBUTE:
		P_Attributes(p);
		break;
	case P_REMOVELINE:
		P_RemoveLine(p);
		break;
	case P_TABS:
		P_Tabs(p);
		break;
	case P_BANNER:
		P_Banner(p);
		break;
	case P_CARTE:
		P_Carte(p);
		break;
	case P_LINES:
		P_Lines(p);
		break;
	case P_NAME:
		P_Name(p);
		break;
	case P_CLEAR:
		DelLines(p);
		Dirty(p);
		break;
	case P_MAKECURRENT:
		MakeCurrent(p);
		break;
	case P_LINE:
		LINEobj = obj;
	case P_NEXTLINE:
		PutLine(p,op);
		break;
	case P_CREATELINE:
		CreateLine(p);
		break;
	case P_DELETE:
		if( p ) p->attributes |= USERCLOSE;
		DeletePad(p);
		break;
	case P_MAKEGAP:
		MakeGap(p);
		break;
	case P_ALARM:
		t = RcvShort();
		if(p){
			if( !(p->ticks = t) ){
				HostParent = HostObject = p->object;
				ToHost( P_CYCLE /*, garbage */ );
			}
		}
		break;
	default:
		ProtoErr( "PadOp(): " );
	}
}

PickOp()
{
	register Pad *p;
	Index i;

	i = RcvShort();
	p = PickPad(PickPoint(IndexToStr(i)));
	MakeCurrent(p);
	while( butts ) waitMOUSE();
	if( p && (HostParent = HostObject = p->object) ){
		FlashBorder(p);
		PutRemote(P_PICK);
		HostAction( &i );
	}
}

Pad *PickPad(pt)
Point pt;
{
	register Pad *p;

	for( p = Sentinel.back; ISPAD(p); p = p->back )
		if( PadSized(p->rect) && ptinrect(pt,p->rect) )
			return p;
	return 0;
}

DeletePick()
{
	DeletePad(PickPad(PickPoint(0L)));
}

DelLines(p)
register Pad *p;
{
	if( !p ) return;
	while( ISLINE(p->sentinel.up,p) )
		DelLine( p->sentinel.up );
}

DeletePad(p)
register Pad *p;
{
	Rectangle r;
	register Line *l, *lu;

	if( !p ) return;
	if( DirtyPad == p ) Dirty((Pad*) 0);
	if( p->attributes&USERCLOSE ){
		HostParent = HostObject = p->object;
		ToHost( P_USERCLOSE /*, garbage */ );
		if( p->attributes&DONT_CLOSE ) return;
		DelLines( p );
		Unlink( p );
		if( p->sentinel.text != NewString ) gcfree( p->sentinel.text );
		if( p->name != NewString ) gcfree( p->name );
		free(p);
	} else {
		if( p->attributes&DONT_CLOSE ) return;
		for( l = p->sentinel.up; ISLINE(l,p); l = lu ){
			lu = l->up;
			if( !(l->attributes&DONT_CUT) ) DelLine(l);
		}
	}
	if( Current == p ) SetCurrent((Pad *) 0);
	if( Selected.line && Selected.pad == p ) Selected.line = 0;
	r = p->rect;
	p->rect = ZRectangle;
	Refresh( r );
}

Select(l, p)
register Line *l;
register Pad *p;
{
	if( Selected.line == l ) return;
	if( Selected.line ){
		if( Selected.pad != p )
			Selected.pad->selkey = Selected.line->key;
		switch( (int) Covered(Selected.pad) ){
		case COMPLETE:
			break;
		case CLEAR:
			LineXOR(Selected.line);
			break;
		case PARTIAL:
			Selected.line = 0;
			Refresh(Selected.pad->rect);
		}
	}
	if( Selected.pad = p )
		p->selkey = 0;
	if( Selected.line = l )
		LineXOR(l);
}

MakeCurrent(p)
register Pad *p;
{
	register paint;
	register Line *l;	

	if( !p ) return;
	if( Selected.line && Selected.pad!=p ) Select((Line*)0, (Pad*)0);
	paint = Covered(p) != CLEAR;
	if( !PadSized(p->rect) ){
		if( Selected.line ) Select((Line*)0, (Pad*)0);
		if( !PadSized(p->rect = clipgetrect(p->sentinel.text))) return;
		paint = 1;
	}
	if( p == Current ) return;
	FrontLink(p);
	if( paint ) Paint(p);
	l = &p->sentinel;
	if( p->selkey )	
		while( ISLINE(l->up, p) ){
			l = l->up;
			if( l->key == p->selkey ){
				Select(l, p);
				break;
			}
		}
}
	
Relocate(p,r)
Pad *p;
Rectangle r;
{
	Rectangle bounding;

	if( !PadSized(r) ) return;
	MakeCurrent(p);			/* bug - used to be FrontLink(p); */
	bounding = boundrect( r, p->rect );
	p->rect = r;
	Refresh( bounding );
}

Move(){
	register Pad *p = PickPad(PickPoint(0L));

	if( !p ) return;
	cursinhibit();
	Relocate( p, moverect(p->rect, PadSpace) );
	cursallow();
}

Reshape()
{
	register Pad *p = PickPad(PickPoint(0L));

	if( !p ) return;
	while( button123() ) waitMOUSE();
	Relocate( p, clipgetrect(0L) );
}
	
Rectangle NewSpace;
Point Scale( p )
Point p;
{
#define		fo PadSpace.origin
#define		fc PadSpace.corner
#define		to NewSpace.origin
#define		tc NewSpace.corner
#define SCALE(xy) p.xy = to.xy + muldiv( p.xy-fo.xy, tc.xy-to.xy, fc.xy-fo.xy );

	SCALE(x);
	SCALE(y);
	return p;
}

#define KBDLEN 90
char KBDStr[KBDLEN]=  "\1";

PadClip()
{
	register Pad *p;

	KBDrect = NewSpace = P->rect;
	KBDrect.origin.y = NewSpace.corner.y -= defont.height;
	KBDrect.origin.x += 2;
	for( p = Sentinel.back; ISPAD(p); p = p->back ){
		p->rect.origin = Scale( p->rect.origin );
		p->rect.corner = Scale( p->rect.corner );
		if( !PadSized(p->rect) ){
			p->rect = ZRectangle;
			if( p == Selected.pad ){
				Selected.pad = 0;
				Selected.line = 0;
			}
		}
	}
	Refresh( PadSpace = NewSpace );
	PaintKBD();
}

InvertKBDrect(s1, s2)
char *s1, *s2;
{
	char buf[300];

	strcpy( buf, s1 );
	strcat( buf, s2 );
	rectf( &display, KBDrect, F_CLR );
	string(&defont, buf, &display, KBDrect.origin, F_XOR);
	rectf( &display, KBDrect, F_XOR );
}		

PaintKBD()
{
	rectf( &display, KBDrect, F_CLR );
	string(&defont, KBDStr, &display, KBDrect.origin, F_XOR);
}	
	
#define PAD_TO_SH (1L)
#define LINE_TO_SH (2L)
CarriageReturn(obj)
register long obj;
{
	register char *kbds = KBDStr;
	register Line *l, *lsent;
	register long ct;

	kbds[strlen(kbds)-1] = '\0';
	if( obj == LINE_TO_SH ){
		PutRemote(P_SHELL);
		SendLong(0L);			/* common protocol */
		SendLong(0L);
		SendString(kbds+1);
		SendLong(1L);
		SendString(Selected.line->text);
	} else if( obj == PAD_TO_SH ){
		PutRemote(P_SHELL);
		SendLong(0L);			/* common protocol */
		SendLong(0L);
		SendString(kbds+1);
		lsent = &Current->sentinel;
		ct = 0;
		for(l = lsent->down; l != lsent; l = l->down)
			++ct;
		SendLong(ct);
		for(l = lsent->down; l != lsent; l = l->down)
			SendString(l->text);
	} else {
		PutRemote(P_KBDSTR);
		SendLong(Current->object);
		SendLong(obj);
		SendString(kbds);
	}
	kbds[0] = 001;
	kbds[1] = 000;
}

LayerReshaped()
{
	if( P->state & (RESHAPED|MOVED) ){
		P->state &= ~(RESHAPED|MOVED);
		PadClip();
	}
}

long LiveKBD()
{
	register Line *sel = Selected.line;
	register Pad *cur = Current;

	if( KBDStr[0] == '>' && cur ){
		if( sel ){
			DoubleOutline(&display, sel->rect);
			return LINE_TO_SH;
		} else {
			HeavyBorder(cur);
			return PAD_TO_SH;
		}
	}
	if( sel && (sel->attributes&ACCEPT_KBD) ){
		DoubleOutline(&display, sel->rect);
		return sel->object;
	}
	if( cur && (cur->attributes&ACCEPT_KBD) ){
		HeavyBorder(cur);
		return cur->object;
	}
	return 0L;
}

#define CNTRL_U 025
KBDAppend(c)
register c;
{
	register char *t;
	register int len = strlen(t = KBDStr);

	if( c < 040 || (c&0200) || len >= KBDLEN ){
		if( c == CNTRL_U ){
			t[0] = 001;
			t[1] = 000;
		}
		if( c != '\t' )		/* bug: \t when len >= KBDLEN !! */
			return;
	}
	t[len-1] = c;
	t[len] = 001;
	t[len+1] = 000;
}

typedef struct String{
	char *s;	/* pointer to string */
	short n;	/* number used, no terminal null */
	short size;	/* size of allocated area */
} String;

MuxSnarf()
{
	String hold;
	register i, c;

	hold.s = 0;
	hold.n = hold.size = 0;
	getmuxbuf(&hold);
	for( i = 0; i < hold.n; ++i ){
		c = hold.s[i];
		if( c == '\n' ) break;
		KBDAppend(c);
	}
}

#define KBD_PAUSE 4
#define ESCAPE 033
KBDServe()
{
	register char c, *t;
	register live, lenmake, len;

	if( own()&KBD ){
		live = LiveKBD();
		while( own() & KBD ){
			c = kbdchar();
			if( c == ESCAPE ){
				MuxSnarf();
				break;
			}
			if( c == '\r' && live ){
				CarriageReturn(live);
				break;
			}
			len = strlen(t = KBDStr);
			if( c == '\b' && len > 1 ){
				t[len-2] = 001;
				t[len-1] = 000;
				continue;
			}
			KBDAppend(c);
		}
		PaintKBD();
		sleep( KBD_PAUSE );
		LiveKBD();
	}
}

FoldToggle(a)
Attrib *a;
{
	*a &= ~(TRUNCATE|FOLD);
	*a |= NewFold;
	Paint(Current);
}

Entry *FoldEntry(a)
register Attrib *a;
{
	static Entry e = { 0, FoldToggle, 0 };

	assert(Current);
	if( ( (*a&(TRUNCATE|FOLD)) ? *a : Current->attributes )&TRUNCATE )
		e.text = "fold",	  NewFold = FOLD;
	else
		e.text = "truncate", NewFold = TRUNCATE;
	e.opand = (long) a;
	return &e;
}
