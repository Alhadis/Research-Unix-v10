/* local2.c */
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
eobl2()			
{
}
char *
exname( ix )
char *ix;
{
	
	static char text[100];
	if( ix == 0 ) cerror("no name in exname");
	sprintf( text, "_%s", ix );
	return( text );
}
lineid( l, fn )
int l;
char *fn;
{
	extern int gdebug;
	
	printf( "#	line %d, file %s\n", l, fn );
}
deflab( n )
int n;
{
	printf( "L%d:\n", n );
}
genubr( n )
int n;
{
	
	printf( "	jbr	L%d\n", n );
}
genret( s, l, n )
int s, l, n;
{
	
	
	deflab(n);
	if( s ) printf( "	movab	L%d,r0\n", l );
	dbfunret();
	printf( "	ret\n" );
}
defalign(n)
int n;
{
	
	if( n % 8 ) cerror( "funny alignment: %d", n );
	else n /= 8;
	if( n == 1 ) return;
	else if( n==2 ) n=1;
	else if( n==4 ) n=2;
	else cerror( "funny alignment: %d", n );
	printf( "	.align	%d\n", n );
}
char *locnames[] = {
	
	"	.text\n",
	"	.data\n",
	"	.data\n",
	"	.data	2\n",
	"	.data	1\n",
	};
bycode( t, i )
int t, i;
{
	
	i &= 07;
	if( t < 0 )
	{ 
		if( i != 0 ) printf("\n");
	}
	else
	{ 
		if( i == 0 ) printf( "\n	.byte	" );
		else printf(",");
		printf("0x%x", t);
	}
}
genshort( s )
short s;
{
	
	printf( "	.short	%d\n", (short) s );
}
genlong( l )
long l;
{
	
	printf( "	.long	0x%lx\n", l );
}
char *
rnames[]= {  
	"r0", "r1",
	"r2", "r3", "r4",
	"r5", "r6", "r7", "r8", "r9", "r10", "r11",
	"ap", "fp", "sp", "pc" 
	};
zzzcode( p, ppc, q )
NODE *p; char **ppc; OPTAB *q;
{
	register c;
	switch( c= *++(*ppc) )
	{
	case 'I':
		cbgen( p->bn.lop, p->bn.label, c );
		return;
	case 'k':	
		printf("$%ld", ~p->in.right->tn.lval);
		return;
	case 'c':
		
		printf( "$%d", p->stn.argsize/32);
		return;
	case 'U':	
		printf("$%d", 32 - p->in.right->tn.lval );
		return;
	case 'T':	
		printf("$%d", p->stn.stsize/8);
		break;
	case 'M':	
		staradr( p->in.right );
		break;
	case 'S':  
		{
			register NODE *l, *r;
			register size;
			if( p->in.op == 98 )
			{
				l = p->in.left;
				r = p->in.right;
			}
			else if( p->in.op == 99 )
			{  
				r = p->in.left;
			}
			else cerror( "STASG bad" );
			size = p->stn.stsize/8;
			if( size <= 0 || size > 65535 )
				cerror("structure size <0=0 or >65535");
			switch(size)
			{
			case 1:
				printf("	movb	");
				break;
			case 2:
				printf("	movw	");
				break;
			case 4:
				printf("	movl	");
				break;
			case 8:
				printf("	movq	");
				break;
			default:
				printf("	movc3	$%d,", size);
				break;
			}
			staradr( r );
			printf(",");
			if( p->in.op == 99 ) printf( "(sp)" );
			else staradr( l );
			printf("\n");
		}
		break;
	default:
		cerror( "illegal zzzcode" );
	}
}
staradr( p )
NODE *p;
{
	NODE *pp;
	pp = talloc();
	pp->in.op = (2+ 11);
	pp->in.left = p;
	upput( pp );
	pp->in.op = 97;
}
conput( p )
register NODE *p;
{
	switch( p->in.op )
	{
	case 4:
		acon( p );
		return;
	case 94:
		printf( "%s", rnames[p->tn.rval] );
		return;
	default:
		cerror( "illegal conput" );
	}
}
insput( p )
NODE *p;
{
	cerror( "insput" );
}
upput( p )
NODE *p;
{
	register NODE *r, *l, *pp;
	register o;
	pp = ( (2+ 11) == p->in.op ) ? p->in.left : p ;
	o = pp->in.op;
	if( o==2 || o==(2+ 11) || o==95 || o==116 || o==117 )
	{
		printf( "*" );
		adrput(pp);
		return;
	}
	if( o==4 )
	{
		acon( pp );
		sideff = 0;
		return;
	}
	if( o == 6 )
	{
		r = pp->in.right;
		l = pp->in.left;
		if( l->in.op == 94 && r->in.op == 4 )
		{
			acon( r );
			pp = pp->in.left;
		}
		else
		{
			
			
			if( l->in.op == 2+ 14 )
			{ 
				adrput( l->in.left );
				l = r;
			}
			else
			{
				p->in.left = r;
				adrput( p );
				p->in.left = pp;  
			}
			if( l->in.op == 64 ) l = l->in.left;
			if( l->in.op != 94 ) cerror("illegal address");
			printf( "[%s]", rnames[l->tn.rval] );
			return;
		}
	}
	else if( o == 8 )
	{
		r = pp->in.right;
		if( r->tn.op != 4 || r->tn.name ) cerror("illegal address");
		r->tn.lval = -r->tn.lval;
		acon( r );
		r->tn.lval = -r->tn.lval;
		pp = pp->in.left;
	}
	else if( o == 1+ 8 )
	{
		r = pp->in.right;
		if( r->tn.op != 4 ) cerror("illegal address");
		r = pp->in.left;
		if( r->tn.op != 94 ) cerror("illegal address");
		
		printf( "-(%s)", rnames[r->tn.rval] );
		sideff = 1;  
		return;
	}
	else if( o == 78 )
	{
		r = pp->in.right;
		if( r->tn.op != 4 ) cerror("illegal address");
		r = pp->in.left;
		if( r->tn.op != 94 ) cerror("illegal address");
		if( sideff ) printf( "(%s)+", rnames[r->tn.rval] );
		else printf( "(%s)", rnames[r->tn.rval] );
		return;
	}
	if( pp->tn.op != 94 ) cerror("illegal address");
	printf( "(%s)", rnames[pp->tn.rval] );
	return;
}
adrput( p )
register NODE *p;
{
	
	register o;
	while( (o=p->in.op) == 103 || o==104 )
	{
		p = p->in.left;
		o = p->in.op;
	}
	switch( o )
	{
	case 2:
		acon( p );
		sideff = 0;
		return;
	case 4:
		
		printf( "$" );
		acon( p );
		sideff = 0;
		return;
	case 94:
		printf( "%s", rnames[p->tn.rval] );
		sideff = 0;
		return;
	case (2+ 11):
		upput( p );
		return;
	case 95:
		sideff = 0;
		printf( "%ld(fp)", p->tn.lval - maxboff );
		return;
	case 116:
		sideff = 0;
		printf( "%ld(fp)", p->tn.lval );
		return;
	case 117:
		sideff = 0;
		printf( "%ld(ap)", p->tn.lval );
		return;
	default:
		cerror( "illegal address" );
		return;
	}
}
acon(p)
NODE *p;
{ 
	if( p->tn.name == 0 )
	{	
		printf( "%ld", p->tn.lval);
	}
	else if( p->tn.lval == 0 )
	{	
		printf( "%s", p->tn.name );
	}
	else
	{				
		printf( "%s+%ld", p->tn.name, p->tn.lval );
	}
}
char *
ccbranches[] = {
	"	jeql	L%d\n",
	"	jneq	L%d\n",
	"	jleq	L%d\n",
	"	jlss	L%d\n",
	"	jgeq	L%d\n",
	"	jgtr	L%d\n",
	"	jlequ	L%d\n",
	"	jlssu	L%d\n",
	"	jgequ	L%d\n",
	"	jgtru	L%d\n",
	};
cbgen( o, lab, mode )
int o, lab, mode;
{ 
	if( !o ) printf( "	jbr	L%d\n", lab );
	else	if( o > 89 ) cerror( "bad conditional branch: %s", opst[o] );
	else printf( ccbranches[o-80], lab );
}
special()
{
	cerror("reached special");
}
