/* local.c */
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
	
extern char *scnames();
typedef long OFFSZ;
struct symtab {
	char *sname;
	TWORD stype;  
	char sclass;  
	char slevel;  
	char sflags;  
	int offset;  
	short dimoff; 
	short sizoff; 
	short suse;  
	struct symtab *scopelink;	
};
struct sw {
	CONSZ sval;
	int slab;
};
extern struct sw swtab[];
extern struct sw *swp;
extern int swx;
extern int ftnno;
extern int blevel;
extern int instruct, stwart;
extern int lineno, nerrors;
typedef union {
	int intval;
	NODE * nodep;
} YYSTYPE;
extern YYSTYPE yylval;
extern CONSZ lastcon;
extern CONSZ ccast();
extern double dcon;
extern char ftitle[];
extern struct symtab stab[];
extern int curftn;
extern int strftn;
extern char *locnames[];
extern int curloc;
extern int curclass;
extern int curdim;
extern int dimtab[];
extern int paramstk[];
extern int paramno;
extern int argstk[];
extern int argsoff[];
extern TWORD argty[];
extern int argno;
extern int autooff, argoff, strucoff;
extern int regvar;
extern int nextrvar;
extern int brkflag;
extern char yytext[];
extern int strflg;
extern OFFSZ inoff;
extern int reached;
extern int idname;
extern NODE node[];
extern NODE *lastfree;
extern int cflag, hflag, pflag;
extern int brklab;
extern int contlab;
extern int flostat;
extern int retlab;
extern int retstat;
extern int asavbc[], *psavbc;
	
extern NODE
	*aadjust(),
	*bcon(),
	*bdty(),
	*block(),
	*bpsize(),
	*buildtree(),
	*clocal(),
	*conval(),
	*convert(),
	*dclstruct(),
	*doszof(),
	*getstr(),
	*makety(),
	*mkty(),
	*oconvert(),
	*offcon(),
	*optim(),
	*pconvert(),
	*ptmatch(),
	*pvconvert(),
	*rstruct(),
	*sconvert(),
	*strargs(),
	*stref(),
	*talloc(),
	*tymatch(),
	*tymerge(),
	*unconvert(),
	*xicolon();
OFFSZ	tsize(),
	psize();
TWORD	types(),
	ctype();
char *exname(), *exdcon();
int minrvar = 11;
int wloop_level = 1	;
int floop_level = 1	;
int maxboff;
int maxtemp;
extern int gdebug;
main( argc, argv ) char *argv[];
{
	extern int proflag;  


	int r;
	r = mainp1( argc, argv );
	return( r );
}
beg_file()
{
	
	register char *p, *s;
	char *tempnam();
			
	p = ftitle + strlen( ftitle ) - 2;
	s = p - 14;	
	while ( p > s && *p != '"' && *p != '/' )
		--p;
	regvar = minrvar;
	if (gdebug)
		dbline();
}
NODE *
treecpy(p)		
	register NODE *p; 
{
	
	register NODE *q;
	q = talloc();
	*q = *p;
	switch ( (dope[q->in.op]&016))
	{
	case 010:
		q->in.right = treecpy(p->in.right);
	case 04:
		q->in.left = treecpy(p->in.left);
	}
	return (q);
}
NODE *
clocal(p) NODE *p;
{
	register NODE *l,*ll,*temp;
	if( p->in.op == (2+ 11) )
	{	
		
		l = p->in.left;
		if( l->in.op == 6 )
		{
			ll = l->in.left;
			if( ll->in.op != 11 && ll->in.op != 2+ 14 )
			{
				if( (l->in.right)->in.op == 11 )
				{
					temp = l->in.right;
					l->in.right = l->in.left;
					l->in.left = temp;
				}
			}
		}
	}
	return(p);
}
cisreg( t ) TWORD t;
{ 
	if( t==4 || t==14 ||  ((t&(03<<5 ))== 040) || t==2 || t==12
		   || t==3 || t==13 )
	{
		if( regvar >= 6 )
		{
			nextrvar = regvar--;
			if( regvar < minrvar ) minrvar = regvar;
			return(1);
		}
	}
	return(0);
}
opbigsz( op )
{
	
	switch( op )
	{
	default:
		return( 32 );
	case 6:
	case 8:
	case 17:
	case 14:
	case 19:
	case 77:
	case 2+ 8:
		return( 8 );
	}
}
branch(n)			
int n;
{
	if (!reached)			
		return;
	dbline();
	genubr(n);
}
	
static int tablelabel;
struct sw heapsw[250 ];	
	
	
genswitch(p,n) register struct sw *p;
{
	
	register i;
	register CONSZ j, range;
	register dlab, swlab;
	range = p[n].sval-p[1].sval;
	if(  ( range>0 &&  range<=(3* n ) &&  n >=4) )
	{	
		swlab = getlab();
		dlab = ((p->slab >= 0) ? p->slab : getlab());
		dswbegin( n, p[1].sval, range, swlab, dlab );
		for( i=1,j=p[1].sval; i<=n; j++)
		{
			if( j == p[i].sval )
			{
				dswcase( p[i].slab );
				j = p[i++].sval;
			}
			else
			{
				dswcase( dlab );
			}
		}
		
		locctr( 0 );
		if( p->slab >= 0 ) genubr( dlab );
		else deflab( dlab );
		return;
	}
	if(  (n>8) )
	{	
		heapsw[0].slab = dlab = (p->slab >= 0 ? p->slab : getlab());
		makeheap(p, n, 1);	
		walkheap(1, n);		
		if( p->slab >= 0 )
			genubr( dlab );
		else
			deflab( dlab );
		return;
	}
	
	for( i=1; i<=n; ++i ) sswtest( p[i].sval, p[i].slab );
	if( p->slab>=0 ) genubr( p->slab );
	}
makeheap(p, m, n)
register struct sw *p;
{
	register int q;
	q = select(m);
	heapsw[n] = p[q];
	if( q>1 ) makeheap(p, q-1, 2*n);
	if( q<m ) makeheap(p+q, m-q, 2*n+1);
}
select(m) {
	register int l,i,k;
	for(i=1; ; i*=2)
		if( (i-1) > m ) break;
	l = ((k = i/2 - 1) + 1)/2;
	return( l + (m-k < l ? m-k : l));
}
walkheap(start, limit)
{
	int label;
	if( start > limit ) return;
	sswtest( heapsw[start].sval, heapsw[start].slab );
	if( (2*start) > limit ) {
		genubr( heapsw[0].slab );
		return;
	}
	if( (2*start+1) <= limit ) {
		label = getlab();
		hswelse( label );
	} else
		hswelse( heapsw[0].slab );
	walkheap( 2*start, limit);
	if( (2*start+1) <= limit ) {
		deflab( label );
		walkheap( 2*start+1, limit);
	}
}
dswbegin( numb, first, range, labl, dlab )
CONSZ first, range;
int numb, labl, dlab;
{
	printf("	casel	r0,$%ld,$%ld\n", first, range );
	printf("L%d:\n", labl );
	tablelabel = labl;
}
dswcase( l )
int l;
{
	printf("	.word	L%d-L%d\n", l, tablelabel );
}
sswtest( val, lab )
CONSZ val;
int lab;
{
	printf( "	cmpl	r0,$%ld\n	jeql	L%d\n", val, lab );
}
hswelse( lab )
int lab;
{
	printf("	jgtr	L%d\n", lab );
}
OFFSZ inoff;		
static inwd;		
static long word;	
zecode( n )
int n;
{
	
	if (n <= 0) return;
	printf( "	.space	%d\n", 4*n );
	inoff += n*32;
}
vfdzero( n ){ 
	
	sz_incode( (CONSZ)0, n );
	}
incode (p, sz)
NODE *p;
{
	sz_incode(p->tn.lval, sz);
}
sz_incode( val, sz )
CONSZ val;
{
	
	
	
	if((sz+inwd) > 32) cerror("incode: field > long");
	
	word |= ((unsigned)(val<<(32-sz))) >> (32-sz-inwd);
	inwd += sz;
	inoff += sz;
	
	if( inwd == 32 )
	{
		genlong( word );
		word = inwd = 0;
	}
}
fincode( d, sz )
double d;
int sz;
{
	
	
	
	union { float f; double d; int i[2]; } cheat;
	if (sz == 64)
	{
		cheat.d = d;
		printf("\t.long\t0x%x,0x%x\t# %.20e\n", cheat.i[0], cheat.i[1],
		    cheat.d);
	}
	else
	{
		cheat.f = d;
		printf("\t.long\t0x%x\t# %.20e\n", cheat.i[0], cheat.f);
	}
	inoff += sz;
}
int ftlab1, ftlab2;
int proflag;
int ent_mask[] = {
	0,0,0,0,0, 0xfc0, 0xf80, 0xf00, 0xe00, 0xc00, 0x800, 0};
efcode()
{
	
	long spoff;	
	genret( strftn, strftn, retlab );
	printf( "	.set	L.R%d,0x%x\n", ftnno, ent_mask[minrvar] );
	spoff = maxboff;
	if( spoff >=  ((0 )/8) ) spoff -=  ((0 )/8);
	spoff += maxtemp;
	spoff /= 8;
	deflab( ftlab1 );
	if( spoff!=0 )
		if( spoff < 64 )
			printf( "	subl2	$%ld,sp\n", spoff );
		else
			printf( "	movab	-%ld(sp),sp\n", spoff);
	genubr( ftlab2 );
	regvar = minrvar = 11;
	if (gdebug)
		dbfunend(getlab());
}
bfcode( a, n )
int a[], n;
{
	
	register i;
	
	printf( "	.word	L.R%d\n", ftnno);
	genubr( ftlab1 = getlab() );
	deflab( ftlab2 = getlab() );
	retlab = getlab();
	if( proflag )
	{	
		i = getlab();
		printf("	movab	L%d,r0\n", i);
		printf("	jsb 	mcount\n");
		printf("	.data\n");
		printf("	.align	2\n");
		printf("L%d:	.long	0\n", i);
		printf("	.text\n");
	}
	if (gdebug)
	{
		dbfunbeg(&stab[curftn]);
		dbnargs(n);
		for (i = 0; i < n; ++i)
			dbfunarg(&stab[a[i]]);
	}
}
defnam( psym )
register struct symtab *psym;
{
	
	
	
	if (psym->sclass == 5)
		printf( "	.globl	%s\n", exname(psym->sname) );
	printf("%s:\n", exname(psym->sname));
}
commdec(id)		
int id;
{
	register struct symtab *psym;
	OFFSZ n;
	psym = &stab[id];
	psym->sflags |= 0200;
	n = tsize(psym->stype, psym->dimoff, psym->sizoff) / 8;
	if (psym->sclass == 3)
		if (psym->slevel)
			printf("	.lcomm	L%d,%ld\n", psym->offset, n);
		else	
			printf("	.lcomm	%s,%ld\n", exname(psym->sname), n);
	else if (psym->sclass == 2)
		printf("	.comm	%s,%ld\n", exname(psym->sname), n);
	else
		cerror("Non-static/external in common");
}
