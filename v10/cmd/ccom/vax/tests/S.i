extern	struct	_iobuf {
	int	_cnt;
	char	*_ptr;
	char	*_base;
	short	_flag;
	char	_file;
} _iob[20];
struct _iobuf	*fopen();
struct _iobuf	*fdopen();
struct _iobuf	*freopen();
long	ftell();
char	*fgets();
  struct _Point {
	int	x;
	int	y;
};
struct _Rectangle {
	  struct _Point	ll;
	  struct _Point	ur;
};
  struct _Object {
	  struct _Object	*link;
};
  struct _Pool {
	  struct _Object	*freelist;
	  struct _Object	*arealist;
	int	objsize;
};
  struct _Vpool {
	  struct _Object	*next;
	  struct _Object	*arealist;
	int	left;
	int	quantum;
};
	  struct _SEQFILE {
		int	hidden;
	};
extern char *myname;
extern int nullstring;
extern char *stringtab;
extern char *execname();
extern struct _iobuf *chkfopen();
extern   struct _Pool *newpool();
extern   struct _Vpool *newvpool();
extern char *maketemp();
extern char *permspace();
extern   struct _SEQFILE *seqopen();
extern char *strchr();
extern char *strrchr();
extern char debug;
	struct _Bsim_hd {
	int	magic;
	int	vdd;
	int	vss;
	int	offtrans;	
	int	offcap;		
	int	ntrans;		
	int	maxnode;	
	int	offarrays;	
	int	ngroups;	
	int	nnodes;		
	int	nsws;		
	int	nargs;		
	int	nops;
	int	nctrls;
	char	clean;		
};
	struct _Bsim_tr {
	char	type;		
	char	selfgate;
	int	group;
	int	numb;
	float	length;
	float	width;
	int	src;
	int	drn;
	int	gate;
};
union  _Objgroup {
	struct {
		char	type;
		char	powstr;
		short	nnodes;
		int	nodeoffset;
		int	domoffset;
		int	vssoffset;
		int	vddoffset;
	} gen;
	struct {
		char	type;
		int	opoffset;
		int	argoffset;
		int	nodeoffset;
	} pol;
	struct {
		char	type;
		int	inoffset;
		int	outoffset;
		int	nodeoffset;
	} inv;
};
struct _Objnode {
	int	orignum;
	int	size;
	int	ctrloffset;
	int	swoffset;
	char	selfgate;
};
	struct _Objsw {
	char	flags;
	char	str;
	short	relnode;
	int	gate;
};
struct _Objctrl {
	int	group;
	int	gate;
};
extern 	struct _Bsim_tr *firsttrans ();
extern 	struct _Bsim_tr *nexttrans ();
union  _Group {
	struct {
		union  _Group	*qlink;
		struct _Node	*nodelist;
		char	type;
		char	powstr;
		short	nnodes;
		char	*domtab;
		struct _Sw	*vsslist;
		struct _Sw	*vddlist;
	} gen;
	struct {
		union  _Group	*qlink;
		struct _Node	*nodelist;
		char	type;
		char	*oplist;
		struct _Node	**arglist;
	} pol;
	struct {
		union  _Group	*qlink;
		struct _Node	*nodelist;
		char	type;
		struct _Node	*input;
		struct _Node	*output;
	} inv;
};
struct _Delay {
	struct _Delay	*forw;
	struct _Delay	*back;
	union  _Group	**ctrllist;
	struct _Node	*node;
	char	delta[3];
	int	when;
} ;
struct _Node {
	char	val;
	char	nextval;
	char	flags;
	char	size;
	int	orignum;
	union	{
		union  _Group	**ctrllist;
		struct _Delay	*delay;
	} 	u;
	struct _Sw	*swlist;
};
struct _Sw {
	char	flags;
	char	str;
	short	relnode;
	struct _Node	*gate;
};
struct _Tnode {
	struct _Tnode	*forw;
	struct _Tnode	*back;
	char	str0;
	char	str1;
	char	xstr0;
	char	xstr1;
	char	val;
	struct _Sw	*swlist;
};
extern char nottab [];
extern char *nettoname();
extern char valstring[];
extern char dqueue;
extern char devalgen;
extern char ddelay;
extern struct _Node *findnode();
extern struct _Node *nettonode();
extern union  _Group *nodetogroup();
extern union  _Group **getctrllist();
extern char debug;
extern   struct _SEQFILE *objfile;
extern 	struct _Bsim_hd header;
extern union  _Group *group;
extern union  _Group **ctrl;
extern struct _Node *node;
extern struct _Sw *sw;
extern struct _Node **arg;
extern char *op;
extern struct _Tnode *tnode;
extern struct _Tnode *list;
extern union  _Group endofq;
extern union  _Group *firstonq;
extern union  _Group *lastonq;
static struct _Node *firstnode, *lastnode;
static struct _Tnode *pasttnode;
static struct _Tnode *toplist;
static char anyxs;
static char *tnodename (this)
register struct _Tnode *this;
{
	register int n;
	switch (n = this - tnode) {
	case 0:
		return "vss";
	case 1:
		return "vdd";
	default:
		return nettoname (firstnode[n-2].orignum);
	}
}
prop_on (domtab)
register char *domtab;
{
	register struct _Tnode *this, *that, *newl;
	register struct _Sw *s;
	struct _Tnode *l;
	char v;
	for (l = toplist; l > &list[0]; l--) {
		if (devalgen)
			printf ("prop_on level %d\n", l-list);
		for (this = l->forw; this != l; this = this->forw) {
			if (this->str0 == l->str0) {
				if ( (domtab[this->str1] >= ( this->str0)))
					continue;
				if (this->str1 == this->str0) {
												{											if (devalgen)										printf ("\tonprop from %s, myval_%d\n", tnodename(this), 2);	s = this->swlist;								if (s != 0) {									for (;;) {										switch (s->gate->val) {								default:										fatal ("bad val");							case 2:											anyxs = 1;									break;									case 0:											if ( ! (s->flags & 0x02)) break;						that = &tnode[s->relnode];							if (s->flags & 0x08 &&							    this->val == 0 && that->val == 0)							break;									goto  labx;								case 1:											if (s->flags & 0x02) break;						that = &tnode[s->relnode];							if (s->flags & 0x08 &&							    this->val == 1 && that->val == 1)							break;					 labx:				if (2 != 1) {										{					(v) = ( this->str0);			if (( this->str0) > ( s->str))				(v) = ( s->str);	};						if (v > that->str0) {									that->str0 = v;									if (v > that->str1) {													{							(that)->forw->back = (that)->back;		(that)->back->forw = (that)->forw;	};									newl = &list[v];											{				 		(that)->back = ( newl)->back;		( newl)->back = (that);			(that)->forw = ( newl);			(that)->back->forw = (that);	};							}									}									}										if (2 != 0) {										{					(v) = ( this->str1);			if (( this->str1) > ( s->str))				(v) = ( s->str);	};						if (v > that->str1) {									that->str1 = v;									if (v > that->str0) {													{							(that)->forw->back = (that)->back;		(that)->back->forw = (that)->forw;	};									newl = &list[v];											{				 		(that)->back = ( newl)->back;		( newl)->back = (that);			(that)->forw = ( newl);			(that)->back->forw = (that);	};							}									}									}										break;									}										if (s->flags & 	0x04)									break;									s++;									}									}									};
					continue;
				} else {
												{											if (devalgen)										printf ("\tonprop from %s, myval_%d\n", tnodename(this), 0);	s = this->swlist;								if (s != 0) {									for (;;) {										switch (s->gate->val) {								default:										fatal ("bad val");							case 2:											anyxs = 1;									break;									case 0:											if ( ! (s->flags & 0x02)) break;						that = &tnode[s->relnode];							if (s->flags & 0x08 &&							    this->val == 0 && that->val == 0)							break;									goto  lab0;								case 1:											if (s->flags & 0x02) break;						that = &tnode[s->relnode];							if (s->flags & 0x08 &&							    this->val == 1 && that->val == 1)							break;					 lab0:				if (0 != 1) {										{					(v) = ( this->str0);			if (( this->str0) > ( s->str))				(v) = ( s->str);	};						if (v > that->str0) {									that->str0 = v;									if (v > that->str1) {													{							(that)->forw->back = (that)->back;		(that)->back->forw = (that)->forw;	};									newl = &list[v];											{				 		(that)->back = ( newl)->back;		( newl)->back = (that);			(that)->forw = ( newl);			(that)->back->forw = (that);	};							}									}									}										if (0 != 0) {										{					(v) = ( this->str1);			if (( this->str1) > ( s->str))				(v) = ( s->str);	};						if (v > that->str1) {									that->str1 = v;									if (v > that->str0) {													{							(that)->forw->back = (that)->back;		(that)->back->forw = (that)->forw;	};									newl = &list[v];											{				 		(that)->back = ( newl)->back;		( newl)->back = (that);			(that)->forw = ( newl);			(that)->back->forw = (that);	};							}									}									}										break;									}										if (s->flags & 	0x04)									break;									s++;									}									}									};
					if ( (domtab[this->str0] >= ( this->str1)))
						continue;
					if (this->str0 < this->str1)
						continue;
					newl = &list[this->str1];
				}
			} else {
				if ( (domtab[this->str0] >= ( this->str1)))
					continue;
											{											if (devalgen)										printf ("\tonprop from %s, myval_%d\n", tnodename(this), 1);	s = this->swlist;								if (s != 0) {									for (;;) {										switch (s->gate->val) {								default:										fatal ("bad val");							case 2:											anyxs = 1;									break;									case 0:											if ( ! (s->flags & 0x02)) break;						that = &tnode[s->relnode];							if (s->flags & 0x08 &&							    this->val == 0 && that->val == 0)							break;									goto  lab1;								case 1:											if (s->flags & 0x02) break;						that = &tnode[s->relnode];							if (s->flags & 0x08 &&							    this->val == 1 && that->val == 1)							break;					 lab1:				if (1 != 1) {										{					(v) = ( this->str0);			if (( this->str0) > ( s->str))				(v) = ( s->str);	};						if (v > that->str0) {									that->str0 = v;									if (v > that->str1) {													{							(that)->forw->back = (that)->back;		(that)->back->forw = (that)->forw;	};									newl = &list[v];											{				 		(that)->back = ( newl)->back;		( newl)->back = (that);			(that)->forw = ( newl);			(that)->back->forw = (that);	};							}									}									}										if (1 != 0) {										{					(v) = ( this->str1);			if (( this->str1) > ( s->str))				(v) = ( s->str);	};						if (v > that->str1) {									that->str1 = v;									if (v > that->str0) {													{							(that)->forw->back = (that)->back;		(that)->back->forw = (that)->forw;	};									newl = &list[v];											{				 		(that)->back = ( newl)->back;		( newl)->back = (that);			(that)->forw = ( newl);			(that)->back->forw = (that);	};							}									}									}										break;									}										if (s->flags & 	0x04)									break;									s++;									}									}									};
				if ( (domtab[this->str1] >= ( this->str0)))
					continue;
				if (this->str1 < this->str0)
					continue;
				newl = &list[this->str0];
			}
			that = this->forw;
							{							(this)->forw->back = (this)->back;		(this)->back->forw = (this)->forw;	};
						{				 		(this)->back = ( newl)->back;		( newl)->back = (this);			(this)->forw = ( newl);			(this)->back->forw = (this);	};
			this = that->back;
		}
	}
}
prop_xon (domtab)
register char *domtab;
{
	register struct _Tnode *this, *that, *newl;
	register struct _Sw *s;
	struct _Tnode *l;
	char v;
	for (l = toplist; l > &list[0]; l--) {
		if (devalgen)
			printf ("prop_xon level %d\n", l-list);
		for (this = l->forw; this != l; this = this->forw) {
			if (this->xstr0 == l->str0) {
				if ( (domtab[this->xstr1] >= ( this->xstr0)))
					continue;
				if (this->xstr1 == this->xstr0) {
												{											if (devalgen)										printf ("\txonprop from %s, myval_%d\n", tnodename(this), 2);	s = this->swlist;								if (s != 0) {									for (;;) {										switch (s->gate->val) {								default:										fatal ("bad val");							case 0:											if ( ! (s->flags & 0x02)) break;						that = &tnode[s->relnode];							if (s->flags & 0x08 &&							    this->val == 0 && that->val == 0)							break;									goto  labx;								case 2:											that = &tnode[s->relnode];							if (s->flags & 0x02) {								if (s->flags & 0x08 &&							    this->val == 0 && that->val == 0)							break;								} else {										if (s->flags & 0x08 &&							    this->val == 1 && that->val == 1)							break;								}										goto  labx;								case 1:											if (s->flags & 0x02) break;						that = &tnode[s->relnode];							if (s->flags & 0x08 &&							    this->val == 1 && that->val == 1)							break;					 labx:				if (2 != 1) {										{					(v) = ( this->xstr0);			if (( this->xstr0) > ( s->str))				(v) = ( s->str);	};						if (v > that->xstr0) {									that->xstr0 = v;								if (v > that->xstr1) {													{							(that)->forw->back = (that)->back;		(that)->back->forw = (that)->forw;	};									newl = &list[v];											{				 		(that)->back = ( newl)->back;		( newl)->back = (that);			(that)->forw = ( newl);			(that)->back->forw = (that);	};							}									}									}										if (2 != 0) {										{					(v) = ( this->xstr1);			if (( this->xstr1) > ( s->str))				(v) = ( s->str);	};						if (v > that->xstr1) {									that->xstr1 = v;								if (v > that->xstr0) {													{							(that)->forw->back = (that)->back;		(that)->back->forw = (that)->forw;	};									newl = &list[v];											{				 		(that)->back = ( newl)->back;		( newl)->back = (that);			(that)->forw = ( newl);			(that)->back->forw = (that);	};							}									}									}										break;									}										if (s->flags & 	0x04)									break;									s++;									}									}									};
					continue;
				} else {
												{											if (devalgen)										printf ("\txonprop from %s, myval_%d\n", tnodename(this), 0);	s = this->swlist;								if (s != 0) {									for (;;) {										switch (s->gate->val) {								default:										fatal ("bad val");							case 0:											if ( ! (s->flags & 0x02)) break;						that = &tnode[s->relnode];							if (s->flags & 0x08 &&							    this->val == 0 && that->val == 0)							break;									goto  lab0;								case 2:											that = &tnode[s->relnode];							if (s->flags & 0x02) {								if (s->flags & 0x08 &&							    this->val == 0 && that->val == 0)							break;								} else {										if (s->flags & 0x08 &&							    this->val == 1 && that->val == 1)							break;								}										goto  lab0;								case 1:											if (s->flags & 0x02) break;						that = &tnode[s->relnode];							if (s->flags & 0x08 &&							    this->val == 1 && that->val == 1)							break;					 lab0:				if (0 != 1) {										{					(v) = ( this->xstr0);			if (( this->xstr0) > ( s->str))				(v) = ( s->str);	};						if (v > that->xstr0) {									that->xstr0 = v;								if (v > that->xstr1) {													{							(that)->forw->back = (that)->back;		(that)->back->forw = (that)->forw;	};									newl = &list[v];											{				 		(that)->back = ( newl)->back;		( newl)->back = (that);			(that)->forw = ( newl);			(that)->back->forw = (that);	};							}									}									}										if (0 != 0) {										{					(v) = ( this->xstr1);			if (( this->xstr1) > ( s->str))				(v) = ( s->str);	};						if (v > that->xstr1) {									that->xstr1 = v;								if (v > that->xstr0) {													{							(that)->forw->back = (that)->back;		(that)->back->forw = (that)->forw;	};									newl = &list[v];											{				 		(that)->back = ( newl)->back;		( newl)->back = (that);			(that)->forw = ( newl);			(that)->back->forw = (that);	};							}									}									}										break;									}										if (s->flags & 	0x04)									break;									s++;									}									}									};
					if ( (domtab[this->xstr0] >= ( this->xstr1)))
						continue;
					if (this->xstr0 < this->xstr1)
						continue;
					newl = &list[this->xstr1];
				}
			} else {
				if ( (domtab[this->xstr0] >= ( this->xstr1)))
					continue;
											{											if (devalgen)										printf ("\txonprop from %s, myval_%d\n", tnodename(this), 1);	s = this->swlist;								if (s != 0) {									for (;;) {										switch (s->gate->val) {								default:										fatal ("bad val");							case 0:											if ( ! (s->flags & 0x02)) break;						that = &tnode[s->relnode];							if (s->flags & 0x08 &&							    this->val == 0 && that->val == 0)							break;									goto  lab1;								case 2:											that = &tnode[s->relnode];							if (s->flags & 0x02) {								if (s->flags & 0x08 &&							    this->val == 0 && that->val == 0)							break;								} else {										if (s->flags & 0x08 &&							    this->val == 1 && that->val == 1)							break;								}										goto  lab1;								case 1:											if (s->flags & 0x02) break;						that = &tnode[s->relnode];							if (s->flags & 0x08 &&							    this->val == 1 && that->val == 1)							break;					 lab1:				if (1 != 1) {										{					(v) = ( this->xstr0);			if (( this->xstr0) > ( s->str))				(v) = ( s->str);	};						if (v > that->xstr0) {									that->xstr0 = v;								if (v > that->xstr1) {													{							(that)->forw->back = (that)->back;		(that)->back->forw = (that)->forw;	};									newl = &list[v];											{				 		(that)->back = ( newl)->back;		( newl)->back = (that);			(that)->forw = ( newl);			(that)->back->forw = (that);	};							}									}									}										if (1 != 0) {										{					(v) = ( this->xstr1);			if (( this->xstr1) > ( s->str))				(v) = ( s->str);	};						if (v > that->xstr1) {									that->xstr1 = v;								if (v > that->xstr0) {													{							(that)->forw->back = (that)->back;		(that)->back->forw = (that)->forw;	};									newl = &list[v];											{				 		(that)->back = ( newl)->back;		( newl)->back = (that);			(that)->forw = ( newl);			(that)->back->forw = (that);	};							}									}									}										break;									}										if (s->flags & 	0x04)									break;									s++;									}									}									};
				if ( (domtab[this->xstr1] >= ( this->xstr0)))
					continue;
				if (this->xstr1 < this->xstr0)
					continue;
				newl = &list[this->xstr0];
			}
			that = this->forw;
							{							(this)->forw->back = (this)->back;		(this)->back->forw = (this)->forw;	};
						{				 		(this)->back = ( newl)->back;		( newl)->back = (this);			(this)->forw = ( newl);			(this)->back->forw = (this);	};
			this = that->back;
		}
	}
}
dumpstr (s, domtab)
char *s;
char *domtab;
{
	register struct _Tnode *this;
	puts (s);
	for (this = &tnode[2]; this < pasttnode; this++)
		printf ("\ttnode[%d] %s str0=%d>%d str1=%d>%d\n",
			this-tnode, nettoname(firstnode[this-tnode-2].orignum),
			this->str0, domtab[this->str0],
			this->str1, domtab[this->str1]
		);
}
dumpxstr (s, domtab)
char *s;
char *domtab;
{
	register struct _Tnode *this;
	puts (s);
	for (this = &tnode[2]; this < pasttnode; this++) {
		printf ("\ttnode[%d] %s ",
			this-tnode, nettoname(firstnode[this-tnode-2].orignum)
		);
		printf ("str0=%d str1=%d xstr0=%d xstr1=%d\n",
			this->str0, domtab[this->str0],
			this->str1, domtab[this->str1],
			this->xstr0, domtab[this->xstr0],
			this->xstr1, domtab[this->xstr1]
		);
	}
}
evalgen (g)
register union  _Group *g;
{
	register struct _Node *n;
	register struct _Tnode *this, *l;
	register char *domtab;
	char v;
	if (devalgen)
		printf ("evalgen group %d\n", g - group);
	domtab = g->gen.domtab;
	firstnode = g->gen.nodelist;
	lastnode = &firstnode[g->gen.nnodes - 3];
	tnode[0].str0 = tnode[1].str1 = g->gen.powstr;
	tnode[0].str1 = tnode[0].str1 = 0;
	toplist = &list[g->gen.powstr];
	v = 0;
	for (l = &list[0]; l <= toplist; l++) {
		l->forw = l->back = l;
		l->str0 = v++;
	}
	tnode[0].swlist = g->gen.vsslist;
	tnode[1].swlist = g->gen.vddlist;
				{				 		(&tnode[0])->back = ( toplist)->back;		( toplist)->back = (&tnode[0]);			(&tnode[0])->forw = ( toplist);			(&tnode[0])->back->forw = (&tnode[0]);	};
				{				 		(&tnode[1])->back = ( toplist)->back;		( toplist)->back = (&tnode[1]);			(&tnode[1])->forw = ( toplist);			(&tnode[1])->back->forw = (&tnode[1]);	};
	
	this = &tnode[2];
	for (n = firstnode; n <= lastnode; this++, n++) {
		this->swlist = n->swlist;
		this->val = n->val;
		v = n->size;
		if (n->flags & 0x08)
			v = g->gen.powstr;
		switch (n->val) {
		case 0:
			this->str0 = v;
			this->str1 = 0;
			break;
		case 1:
			this->str0 = 0;
			this->str1 = v;
			break;
		case 2:
			this->str0 = v;
			this->str1 = v;
			break;
		}
		l = &list[v];
					{				 		(this)->back = ( l)->back;		( l)->back = (this);			(this)->forw = ( l);			(this)->back->forw = (this);	};
	}
	pasttnode = this;
	
	anyxs = 0;
	if (devalgen)
		dumpstr ("before prop over 1's", domtab);
	prop_on (domtab);
	if (devalgen)
		dumpstr ("after prop over 1's", domtab);
	
	if (! anyxs) {
		this = &tnode[2];
		for (n = firstnode; n <= lastnode; n++, this++) {
			if ( (domtab[this->str0] >= ( this->str1))) {
				if (n->nextval != 0)
					assign (n, 0, 1);
			} else if ( (domtab[this->str1] >= ( this->str0))) {
				if (n->nextval != 1)
					assign (n, 1, 1);
			} else {
				if (n->nextval != 2)
					assign (n, 2, 1);
			}
		}
		return;
	}
	
	for (l = &list[0]; l <= toplist; l++)
		l->forw = l->back = l;
	for (this = tnode; this < pasttnode; this++) {
		this->xstr0 = this->str0;
		this->xstr1 = this->str1;
		if (this->xstr0 > this->xstr1)
			l = &list[this->xstr0];
		else
			l = &list[this->xstr1];
					{				 		(this)->back = ( l)->back;		( l)->back = (this);			(this)->forw = ( l);			(this)->back->forw = (this);	};
	}
	prop_xon (domtab);
	if (devalgen)
		dumpxstr ("after prop over X's", domtab);
	
	this = &tnode[2];
	for (n = firstnode; n <= lastnode; n++, this++) {
		if ( (domtab[this->str0] >= ( this->str1)) &&  (domtab[this->str0] >= ( this->xstr1))) {
			if (n->nextval != 0)
				assign (n, 0, 1);
		} else if ( (domtab[this->str1] >= ( this->str0)) &&  (domtab[this->str1] >= ( this->xstr0))) {
			if (n->nextval != 1)
				assign (n, 1, 1);
		} else {
			if (n->nextval != 2)
				assign (n, 2, 1);
		}
	}
}
