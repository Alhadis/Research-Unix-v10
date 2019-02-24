/* ../common/cgen.c */
extern	struct	_iobuf {
	int	_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	short	_flag;
	char	_file;
} _iob[20];
struct _iobuf	*fopen();
struct _iobuf	*fdopen();
struct _iobuf	*freopen();
long	ftell();
char	*fgets();
	char		*hash();
	char		*savestr();
	char		*tstr();
	extern int	tstrused;
	extern char	*tstrbuf[];
	extern char	**curtstr;
extern int nerrors;  
typedef union ndu NODE;
typedef unsigned int TWORD;
typedef long CONSZ;  
extern int dope[];  
extern char *opst[];  
	
union ndu {
	struct {
		int op;
		int goal;
		TWORD type;
		int cst[(6+4)];
		char * name;
		char pad[8  -sizeof(char *)];	
		NODE *left;
		NODE *right;
	}in;	
	
	struct {
		int op;
		int goal;
		TWORD type;
		int cst[(6+4)];
		char * name;
		char pad[8  -sizeof(char *)];	
		CONSZ lval;
		int rval;
	}tn;	
	
	struct {
		int op;
		int goal;
		TWORD type;
		int cst[(6+4)];
		int label;  
		int lop;  
	}bn;	
	struct {
		int op;
		int goal;
		TWORD type;
		int cst[(6+4)];
		int stsize;  
		short stalign;  
		short argsize;  
	}stn;	
	struct {
		int op;
		int goal;
		TWORD type;
		int cdim;
		int csiz;
	}fn;	
	
	struct {
		
		int op;
		int goal;
		TWORD type;
		int cdim;
		int csiz;
		double dval;
	}fpn;	
};
	
	
		
	
	
	
 
	
extern int busy[];
extern NODE node[];
typedef struct shape SHAPE;
	
extern struct shape {
	int	op;	
	SHAPE	*sl;	
	SHAPE	*sr;
	int	sh;	
	int	sc;	
} shapes[];
extern SHAPE	*pshape[];
typedef struct optab OPTAB;
struct optab {
	int	op;	
	int	tyop;	
	OPTAB	*nextop;
	SHAPE	**lshape;	
	int	ltype;		
	SHAPE	**rshape;	
	int	rtype;
	int	needs;
	int	rewrite;
	char	*cstring;
	int	cost;
	int	lcount;		
	int	rcount;		
	int	stinline;	
};
extern OPTAB
	*match(),
	*ophead[],
	table[];
extern NODE resc[];
extern int tmpoff;
extern int maxboff;
extern int maxtemp;
extern int maxarg;
extern int ftnno;
extern int sideff;
extern NODE
	*talloc(),
	*ind2type(),
	*tcopy(),
	*getadr(),
	*getlr();
extern CONSZ rdin();
extern char *rnames[];
extern int lineno;
extern char ftitle[];
extern int fldshf, fldsz;
extern int lflag, udebug, e2debug, odebug, rdebug, radebug, sdebug;
extern int fast;  
	
	
	
	
typedef struct inst INST;
struct inst {
	NODE	*p;
	OPTAB	*q;
	int	goal;
};
extern INST inst[300];
extern nins;
	
	
typedef SHAPE	*SHTABLE[2][20];
extern SHTABLE	sha;
rewcom( p, goal )
NODE *p; 
{
	
	
	
	int o, ty, g1, g2;
	NODE *l, *r, *ql, *qr;
	o = p->tn.op;
	g1 = g2 = 6;
	p->tn.goal = goal;
	
	switch( o )
	{
	case 97:
		cerror( "rewcom(%d) is FREE", p-node );
	case 110:
		g1 = (6+3);
		break;
	case 59:
		g1 = (6+1);
		g2 = goal;
		break;
	case 22:
	case 112:
	case 56:
	case 108:
		g1 = g2 = goal;
		break;
	case 70:
	case 100:
	case 73:
		g2 = (6+1);
		break;
	}
	switch( ty =  (dope[o]&016) )
	{
	case 010:
		rewcom( r = p->in.right, g2 );
	case 04:
		rewcom( l = p->in.left, g1 );
		break;
	case 02:
		return;
	}
	if( o==59 || o==22 || o==108 ) return;
	
	
	
	
	if( l->tn.op == 59 && l->in.left->tn.op != 110 ) 
	{
		
		
		
		ql = l->in.left;
		qr = l->in.right;
		*l = *p;  
		l->in.left = qr;
		p->in.right = l;
		p->in.left = ql;
		p->tn.op = 59;
		rewcom( p, p->tn.goal );
	}
	if( ty == 04 ) return;
	if( r->tn.op == 59 && r->in.right->tn.op != 110 ) 
	{
		
		
		
		ql = r->in.left;
		qr = r->in.right;
		*r = *p;
		p->tn.op = 59;
		p->in.left = ql;
		r->in.right = qr;
		rewcom( p, p->tn.goal );
	}
}
rewlhs(p)
NODE *p; 
{
	
	
	NODE *q, *t;
	q = talloc();
	*q = *p;
	t = tcopy( p->in.left, 0 );
	p->in.left = q;
	p->in.right = t;
	p->tn.op = 59;
	return;
}
rewsto(p)
NODE *p; 
{
	
	
	int o, ao;
	NODE *t, *q;
	
	while( (o=p->tn.op) == 59 ) p = p->in.right;
	if( o == 95 ) return(0);  
	if( o == 99 ) 
	{
		 		
		t = talloc();
		*t = *p->in.left;  
		q = talloc();
		*q = *t;
		t->tn.op = 95;
		t->tn.lval = freetemp(argsize(p)/32 );
		t->tn.lval =  ((t->tn.lval)/8);
		t->tn.name = (char *) 0;
		t->tn.type =      02000   ;
		q->tn.op = 2+ 14;
		q->in.left = t;
		
		t = talloc();
		*t = *p;
		t->in.left = q;
		t->in.right = p->in.left;
		t->tn.op = 98;
		
		p->in.left = talloc();
		p->in.left->tn.op = 59;
		p->in.left->in.left = t;
		p->in.left->in.right = t = talloc(); 
		*t = *q;
		t->in.left = talloc();
		*t->in.left = *q->in.left;
		
		
		if( odebug>1 ) e2print( p );
		return( 1 );
	}
	if( odebug>1 ) 
	{
		e2print( p );
		printf( "\nrewritten by rewsto as:\n" );
	}
	if(  (dope[o]&01) && o!=78 && o!=79 && lhsok( p->in.left ) ) {
		
		rewlhs( p );
		return( 1 );
	}
	ao = 1+ o;
	if(  (dope[ao]&020000) ) 
	{
		if( p->in.left->tn.op == 95 ) 
		{
			p->tn.op = ao;
			rewlhs( p );
			if( odebug>1 ) e2print( p );
			return( 1 );
		}
	}
	
	
	t = talloc();
	*t = *p;  
	q = talloc();
	*q = *p;
	t->tn.op = 95;
	t->tn.lval = freetemp(argsize(p)/32 );
	t->tn.lval =  ((t->tn.lval)/8);
	t->tn.name = (char *) 0;
	q->tn.op = 58;
	q->in.left = t;
	q->in.right = talloc();
	*(q->in.right) = *p;
		
	p->in.right = talloc();
	*(p->in.right) = *t;
	p->tn.op = 59;
	p->in.left = q;
	
	if( odebug>1 ) e2print( p );
	return( 1 );
}
iseff( p )
NODE *p; 
{
	
	int o;
	o = p->tn.op;
	if(  (dope[o]&02000) ||  (dope[o]&01) ) return( 1 );
	switch(  (dope[ o ]&016) )
	{
	case 010:
		if( iseff( p->in.right ) ) return( 1 );
	case 04:
		return( iseff( p->in.left ) );
	}
	return( 0 );
}
NODE *
lhsto( p )
NODE *p; 
{
	
	
	NODE *q;
	int o;
	for( q = p->in.left; (o=q->tn.op)!=(2+ 11); q=q->in.left )
	{
		if(  (dope[o]&016) == 02 ) return( (NODE *)0);
	}
	
	q = q->in.left;
	o = q->tn.op;
	if(  (dope[o]&016) == 02 ) return( (NODE *)0 );
	else return( q );
}
static int
c2bigger( p ) NODE *p; {
	
	register TWORD t, tl;
	t = p->tn.type;
	tl = p->in.left->tn.type;
	if( (t|tl)&      04000 ) return( 0 );  
	if( t&040 ) return( 1 );
	if( tl&040 ) return( 0 );
	if( t&	020 ) return( 1 );
	if( tl&	020 ) return( 0 );
	if( t&(	010|	01000) ) return( 1 );
	if( tl&(	010|	01000) ) return( 0 );
	if( t&(	(	04|	010)|(0400|	01000)) ) return( 1 );
	if( tl&(	(	04|	010)|(0400|	01000)) ) return( 0 );
	if( t &(	02|0200) ) return( 1 );
	return( 0 );
	}
NODE *
ind2type( p )
register NODE *p; 
{
	
	register TWORD t;
	NODE *q;
	t = p->tn.type;
	if( t == 	01 || t == 	02 ) t = 	(	04|	010);
	else if( t == 	0100 || t == 0200 ) t = (0400|	01000);
	else if( t == 	020 ) t = 040;
	else return( p );
	if( p->tn.op == 104 && c2bigger(p) ) 
	{
		p->tn.type = t;
		return( p );
	}
	q = talloc();
	q->tn.op = 104;
	q->in.left = p;
	q->in.right = 0;
	q->tn.name = (char *) 0;
	q->tn.type = t;
	q->tn.goal = 6;
	return( q );
}
NODE *
reweop( p )
register NODE *p; 
{
	
	
	
	
	
	
	register NODE *q, *t;
	register TWORD ty;
	if( odebug>1 ) 
	{
		e2print( p );
		printf( "\nrewritten by reweop as:\n" );
	}
	q = tcopy( p, 0 );
	if( p->in.left->tn.op == 104 ) 
	{
		
		
		
		t = p->in.left;
		ty = t->in.left->tn.type;
		p->in.left = t->in.left;
		tfree( p->in.right );
		p->in.right = t;
		t->in.left = q;
		
		t->tn.type = ty;
	}
	else 
	{
		tfree( p->in.right );
		p->in.right = q;
	}
	
	p->tn.op = 58;
	q->tn.op = (-1)+ q->tn.op;
	p->tn.type = p->in.left->tn.type;
	q->tn.type = q->in.right->tn.type;
	if( odebug>1 ) e2print( p );
}
rewass( p )
NODE *p; 
{
	NODE *q;
	int o;
	
	if( odebug ) 
	{
		printf( "rewass called with:\n" );
		e2print( p );
	}
	o = p->tn.op;
	if( o == 2+ 14 ) 
	{
		if( p->in.left->tn.op == 118 ) 
		{
			
			q = p->in.left;
			q->tn.op = 4;
			*p = *q;
			q->tn.op = 97;
			return(0);  
		}
		
		rewsto( p->in.left );
		
		if( odebug ) 
		{
			printf( "\nrewritten by rewass as:\n" );
			e2print( p );
		}
		return(1);
	}
	if( ! (dope[o]&01) || o==58 ) 
	{
		if( o==58 ) 
		{
			
			o = p->in.left->tn.op;
			if( o==118 || o==120 || o==119 ) 
			{
				
				p->in.left->tn.op = 94;
				p->in.left->tn.rval =  0;
				if( odebug ) 
				{
					printf( "funny node redone\n" );
					e2print(p);
				}
				return(0);
			}
		}
		else 
		{
			TWORD t = p->in.left->tn.type;
			
			
			p->in.left = ind2type( p->in.left );
			p->in.right = ind2type( p->in.right );
			if( odebug ) {
				printf( "conversions inserted" );
				e2print(p);
			}
			
			if( t != p->in.left->tn.type ) {
				
				return( 0 );
			}
		}
		e2print(p);
		cerror( "can't deal with op %s", opst[o] );
	}
	if( o == 78 || o == 79 ) 
	{
		
		if( odebug>1 ) 
		{
			e2print( p );
			printf( "\nrewritten by rewass as:\n" );
		}
		if( p->in.goal == (6+1) )
		{
			p->in.op = ((o==78)?1+ 6:1+ 8);
		}
		else
		{
			q = tcopy(p, 0);
			regrcl( p->in.left );
			tfree( p->in.left );
			p->in.left = q;
			q->tn.op = ((o==78)?1+ 6:1+ 8);
			p->tn.op = ((o==78)?8:6);
		}
		if( odebug ) 
		{
			printf( "\nrewritten by rewass as:\n" );
			e2print( p );
		}
		return(1);
	}
	
	if( q = lhsto(p) ) 
	{
		if( !rewsto( q ) ) cerror( "rewass0" );  
		rewcom( p, p->tn.goal );  
		if( p->tn.op != 59 ) cerror( "rewass1" );
		if( ! (dope[ p->in.right->tn.op ]&01) ) cerror( "rewass2" );
		(void)reweop( p->in.right );
	}
	else (void)reweop( p );  
	return(1);
}
outshp( pp )
SHAPE **pp; 
{
	SHAPE *p;
	if (pp == 0)
		return;
	for( ; p = *pp; ++pp )
	{
		printf("\t\t");
		shpr(p);
		printf( " (%d)\n", p->sc );
	}
}
tabpr()
{
	register	OPTAB	*p;
	for (p =table; ;p++)
	{
		printf("Dump of table[%d] (stinline %d)\n", p-table, p->stinline );
		printf("\top = %s\n", opst[p->op]);
		printf("\tnextop = %d\n", p->nextop?p->nextop-table:-1 );
		printf("\tlshape = %d\n", p->lshape-pshape);
		printf("\tltype = 0%o\n", p->ltype);
		printf("\trshape = %d\n", p->rshape-pshape);
		printf("\trtype = 0%o\n", p->rtype);
		printf("\tneeds = %d\n", p->needs);
		printf("\trewrite = %d\n", p->rewrite);
		printf("\tcstring = %s", p->cstring);
		printf("\tcost = %d\n", p->cost);
		printf("\tLeft:\n");
		outshp(p->lshape);
		printf("\tRight:\n");
		outshp(p->rshape);
		printf("\n");
	}
}
codgen( p )
NODE *p; 
{
	
	int i, flag;
	if (odebug > 5)
	{
		tabpr();
		
	}
	
	
	
	
again:
	
	rewcom( p, (6+1) );
	if( odebug ) 
	{
		printf( "After goals are computed:" );
		e2print( p );
	}
	
	if( costs( p ) ) goto again;  
	if( odebug ) 
	{
		printf( "After costs are computed:" );
		e2print( p );
	}
	
	nins = 0;
	insout( p, (6+1) );
	
	flag = 0;
	for( i=0; i<nins; ++i ) 
	{
		if( inst[i].goal == (6+2) ) 
		{
			if( odebug ) 
			{
				printf( "subtree is stored in temp:\n" );
				e2print( inst[i].p );
			}
			if( rewsto( inst[i].p ) ) {
				if( !fast ) goto again;
				
				flag = 1;
			}
		}
	}
	if( flag ) goto again;
	if( odebug ) e2print(p);
	
	insprt();
}
INST inst[300];
int nins;
insprt()
{
	int i;
	register INST *pi;
	register NODE *p;
	register OPTAB *q;
	register c, goal;
	for( pi=inst,i=0; i<nins; ++i,++pi )
	{
		p = pi->p;
		q = pi->q;
		c = pi->goal;
		if( c == (6+3) && (q->rewrite&040) ) goal = 04 ;
		else if( c == (6+1) ) goal = 01 ;
		else goal = 02 ;
		if(odebug > 4)
		{
			printf("INSOUT: %d c=",i);
			preff(c);
			printf(" goal=");
			prgoal(goal);
			printf("\n");
			e2print(p);
		}
		allo( p, q );
		expand( p, goal, q->cstring, q );
		reclaim( p, q->rewrite, goal );
		
		if( c == (6+3) && p->tn.op != 96 ) 
		{
			cfix( p, (6+3) );
			if( p->tn.op != 96 ) cerror( "ctest fails" );
		}
		if( c>=0 && c<=6 && ! ( p ->in.op==94 &&  (( p ->tn.rval)<6)) )
		{
			cfix( p, 6 );
		}
	}
}
SHTABLE sha;
int odebug = 0;
cfix( p, goal )
NODE *p; 
{
	
	OPTAB *q;
	NODE *pp;
	int r;
	if(odebug > 4)
	{
		printf("CFIX: goal=");
		prgoal(goal);
		printf("\n");
		e2print(p);
	}
	if( goal == (6+3) ) 
	{
		r = 040;
		p->tn.goal = (6+3);
	}
	else 
	{
		r = (04|010|020);
		pp =  ( ( (( (dope[ p ->tn.op]&016))==02? p : p ->in.left)));
		if(  ( pp ->in.op==94 &&  (( pp ->tn.rval)<6)) ) r |= 01;
		pp =  ( ( (( (dope[ p ->tn.op]&016))==010? p ->in.right: p )));
		if(  ( pp ->in.op==94 &&  (( pp ->tn.rval)<6)) ) r |= 02;
	}
	if( goal == (6+3) ) goal = 04 ;
	else goal = 02 ;
	for( q=0; q = match( p, q ); )
	{
		
		
		if( q->rewrite & r ) 
		{
			
			allo( p, q );
			expand( p, goal, q->cstring, q );
			reclaim( p, q->rewrite, goal );
			return;
		}
	}
	e2print(p);
	cerror( "cfix trouble" );
}
preff(c)
{
	char buf[20];
	register char *p;
	p = c==(6+3) ? "CCC" : c==(6+2) ? "CTEMP" : c==(6+1) ? "CEFF" : 0;
	if(!p)
	{
		sprintf(buf,"0%o",c);
		p = buf;
	}
	printf("%s",p);
}
