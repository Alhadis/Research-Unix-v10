

























	







	

	

	





	

	

	

	







	



















extern	struct	_iobuf {
	int	_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	short	_flag;
	char	_file;
} _iob[120];


























struct _iobuf	*fopen();
struct _iobuf	*fdopen();
struct _iobuf	*freopen();
struct _iobuf	*popen();
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
		char pad[8-sizeof(char *)];	
		NODE *left;
		NODE *right;
	}in;	
	struct {
		int op;
		int goal;
		TWORD type;
		int cst[(6+4)];
		char * name;
		char pad[8-sizeof(char *)];	
		NODE *left;
		NODE *right;
		int lineno;
	}ln;	
	
	struct {
		int op;
		int goal;
		TWORD type;
		int cst[(6+4)];
		char * name;
		char pad[8-sizeof(char *)];	
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

extern int lineno, slineno, nerrors;
typedef struct {
		union {
		int u_intval;
		NODE *u_nodep;
	} YY_un;
	int lineno;
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
extern int *dimtab;
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








unsigned int maxoffset;

struct instk 
{
	int in_sz;   
	int in_x;    
	int in_n;    
	int in_s;    
	int in_d;    
	TWORD in_t;    
	int in_id;   
	int in_fl;   
	OFFSZ in_off;  
}instack[10],*pstk;

struct symtab *relook();

int ddebug = 0;

struct symtab * mknonuniq();

defid( q, class )
register NODE *q; 
{
	register struct symtab *p;
 	extern struct symtab *scopestack[];
	int idp;
	register TWORD type;
	register TWORD stp;
	register scl;
	register dsym, ddef;
	register slev, temp;

	if( q == (NODE *)0 ) return;  
	if( q < node || q >= &node[1500] ) cerror( "defid call" );
	idp = q->tn.rval;
	if( idp < 0 ) cerror( "tyreduce" );
	p = &stab[idp];

	if( ddebug )
	{
		printf( "defid( %s (%d), ", p->sname, idp );
		tprint( q->in.type );
		printf( ", %s, (%d,%d) ), level %d\n", scnames(class),
		q->fn.cdim, q->fn.csiz, blevel );
	}

	fixtype( q, class );
	type = q->in.type;
	class = fixclass( class, type );
	stp = p->stype;
	slev = p->slevel;

	if( ddebug )
	{
		printf( "	modified to " );
		tprint( type );
		printf( ", %s\n", scnames(class) );
		printf( "	previous def'n: " );
		tprint( stp );
		printf( ", %s, (%d,%d) ), level %d\n", scnames(p->sclass),
		p->dimoff, p->sizoff, slev );
	}

	if( stp == 0100 && p->sclass == 0 )goto enter;
	
	
	if( stp == 17|| stp == 1 )
	{
		if( blevel==1 && stp!=1 ) switch( class )
		{
		default:
			if(!(class&0100))
				uerror("declared argument %s is missing", p->sname);
		case 8:
		case 10:
		case 11:
		case 12:
		case 16:
		case 15:
		case 13:
			;
		}
		goto enter;
	}
	if( type != stp ) goto mismatch;
	
	dsym = p->dimoff;
	ddef = q->fn.cdim;
	for( temp=type; temp&(03<<5); temp = (((temp>>2)&~037)|(temp&037)) )
	{
		if( ((temp&(03<<5))==0140) )
		{
			if( dimtab[dsym] == 0 ) dimtab[dsym] = dimtab[ddef];
			else if( dimtab[ddef]!=0 && dimtab[dsym] != dimtab[ddef] )
			{
				goto mismatch;
			}
			++dsym;
			++ddef;
		}
	}
	
	if( (temp==8||temp==9||temp==10) && p->sizoff != q->fn.csiz
	    && class!=10 && class!=12 && class!=15 )
	{
		goto mismatch;
	}
	scl = ( p->sclass );

	if( ddebug )
	{
		printf( "	previous class: %s\n", scnames(scl) );
	}

	if( class&0100 )
	{
		
		if( !falloc( p, class&077, 1, (NODE *)0 ) ) 
		{
			
			psave( idp );
			return;
		}
		
	}
	else switch( class )
	{
	case 2:
		switch( scl )
		{
		case 3:
		case 18:
			if( slev==0 ) return;
			break;
		case 5:
		case 2:
		case 14:
		case 17:
			return;
		}
		break;
	case 3:
		if( scl==18 || (scl==2 && blevel==0) )
		{
			p->sclass = 3;
			if( ((type&(03<<5))==0100) ) curftn = idp;
			return;
		}
		break;
	case 18:
		if( scl==3 || scl==18 ) return;
		break;
	case 6:
		if( scl == 7 )
		{
			p->sclass = 6;
			deflab( p->offset );
			return;
		}
		break;
	case 13:
		if( scl == class ) return;
		break;
	case 17:
		if( scl == 17 || scl == 14 ) return;
		break;
	case 14:
		if( scl == 17 )
		{
			p->sclass = 14;
			if( ((type&(03<<5))==0100) ) curftn = idp;
			return;
		}
		break;
	case 11:
	case 8:
		if( scl == class ) 
		{
			if( oalloc( p, &strucoff ) ) break;
			if( class == 11 ) strucoff = 0;
			psave( idp );
			return;
		}
		break;
	case 16:
		if( scl == class )
		{
			if( p->offset!= strucoff++ ) break;
			psave( idp );
		}
		break;
	case 5:
		if( scl == 2 ) 
		{
			p->sclass = 5;
			if( ((type&(03<<5))==0100) ) curftn = idp;
			return;
		}
		break;
	case 10:
	case 12:
	case 15:
		if( scl != class ) break;
		if( dimtab[p->sizoff] == 0 )
			return;  
		break;
	case 7:
		if( scl == 6 || scl == 7 ) return;
	case 9:
	case 1:
	case 4:
		;  
	}
mismatch:
	
	if( class==11 || class==8 || class & 0100 )
	{
		
		register * memp;
		p->sflags |= 040;  
		
		for( memp = &paramstk[paramno-1];
			 *memp>=0 && stab[*memp].sclass != 10
				&& stab[*memp].sclass != 12;
			 --memp)
		{
			if( stab[*memp].sflags & 040 )
			{	int i;
				if ( p->sname != stab[*memp].sname )
					continue;
				i = stab[*memp].suse;
				if(i < 0)
					i = -i;
				uerror("redeclaration of: %s from line %d",
					p->sname, i);
				break;
			}
		}
		p = mknonuniq( &idp ); 
		goto enter;
	}
	if( blevel > slev && class != 2 && class != 14 &&
	    class != 17 && !( class == 6 && slev >= 2 ) )
	{
		q->tn.rval = idp = hide( p );
		p = &stab[idp];
		goto enter;
	}
	uerror( "redeclaration of %s from some line %d", p->sname, p->suse);
	if( class==5 && ((type&(03<<5))==0100) ) curftn = idp;
	return;
enter:  

	if( ddebug ) printf( "	new entry made\n" );

	if( type == 17 ) uerror("void type for %s",p->sname);
	p->stype = type;
	p->sclass = class;
 	p->slevel = (class == 6 || class == 7) ? 2 : blevel;
	p->offset = (-10201);
	p->suse = lineno;
	if( class == 10 || class == 12 || class == 15 ) 
	{
		p->sizoff = curdim;
		dstash( 0 );  
		dstash( -1 ); 
		dstash( 8 );  
		dstash( idp );  
	}
	else 
	{
		switch( (type&037) )
		{
		case 8:
		case 9:
		case 10:
			p->sizoff = q->fn.csiz;
			break;
		default:
			p->sizoff = (type&037);
		}
	}
	
	p->dimoff = q->fn.cdim;
	
	if( class&0100 )
	{
		falloc( p, class&077, 0, (NODE *)0 );  
		psave( idp );
	}
	else switch( class )
	{
	case 1:
		oalloc( p, &autooff );
		break;
	case 3:
	case 5:
		p->offset = getlab();
		if( ((type&(03<<5))==0100) ) curftn = idp;
		break;
	case 7:
	case 6:
		p->offset = getlab();
		if( class == 6 )
		{
			locctr( 0 );
			deflab( p->offset );
		}
		break;
	case 2:
	case 17:
	case 14:
		p->offset = getlab();
		break;
	case 11:
	case 8:
		oalloc( p, &strucoff );
		if( class == 11 ) strucoff = 0;
		psave( idp );
		break;
	case 16:
		p->offset = strucoff++;
		psave( idp );
		break;
	case 4:
		
		p->offset = nextrvar;
		if( blevel == 1 ) p->sflags |= 010;
		break;
	}
	


 	if (blevel >= 100)
 	{
 		cerror("too many nesting levels (%d>=%d)", blevel, 100);
 		
 	}
 	p->scopelink = scopestack[slev = p->slevel];
 	scopestack[slev] = p;
	


	switch( class )
	{
 		case 7:
 		case 6:
 			p->slevel = 2;
 			break;
		case 2:
		case 17:
		case 14:
			p->slevel = 0;
			break;
	}

	if( ddebug )
		printf( "	dimoff, sizoff, offset: %d, %d, %d\n",
			p->dimoff, p->sizoff, p->offset );

}

asave( i )
{
	if( argno >= 50 )
	{
		cerror( "too many arguments (%d>=%d)", argno, 50);
	}
	argstk[ argno++ ] = i;
}

psave( i )
{
	if( paramno >= 650 )
	{
		cerror( "parameter stack overflow (%d>=%d)", paramno, 650);
	}
	paramstk[ paramno++ ] = i;
}


int maxarg;

ftnend()
{
	 
	if( retlab != (-1) )
	{
		

		efcode();

		p2bend();

	}
	;
	retstat = 0;
	tcheck();
	curclass = 0;
	brklab = contlab = retlab = (-1);
	flostat = 0;
	strftn = 0;
	argno = 0;
	if( nerrors == 0 )
	{
		if( psavbc != & asavbc[0] ) cerror("bcsave error");
		if( paramno != 0 ) cerror("parameter reset error");
		if( swx != 0 ) cerror( "switch error");
	}
	psavbc = &asavbc[0];
	paramno = 0;
	autooff = 0;
	maxarg = 0;
	reached = 1;
	swx = 0;
	swp = swtab;
	locctr(2);
}

dclargs()
{
	register i, j;
	register struct symtab *p;
	register NODE *q;
	register TWORD temp;
	extern TWORD simpstr();

	argoff = 32;


	if( ddebug > 2) printf("dclargs()\n");

	for( i=0; i<argno; ++i )
	{
		if( (j = argstk[i]) < 0 ) continue;
		p = &stab[j];

		if( ddebug > 2 )
		{
			printf("\t%s (%d) ",p->sname, j);
			tprint(p->stype);
			printf("\n");
		}

		if( p->stype == 1 ) 
		{
			q = block(97,(NODE *)0,(NODE *)0,4,0,4);
			q->tn.rval = j;
			defid( q, 9 );
		}
		if( p->sclass == 4 )
		{
			
			int tmp = p->offset;
			p->offset = (-10201);
			p->sclass = 9;
			oalloc( p, &argoff );
			argsoff[i] = p->offset;
			argty[i] = p->stype;
			p->sclass = 4;
			p->offset = tmp;
		}
		else 
		{
			oalloc( p, &argoff );  
			argsoff[i] = p->offset;
			argty[i] = p->stype;
		}
	}
	autooff = 0;

	locctr(0);
	defalign(32);
	++ftnno;
	p = &stab[curftn];

	if( p->slevel>1 && p->sclass == 3 ) deflab( p->offset );
	else defnam( p );
	temp = p->stype;
	temp = ((( temp >>2)&~037)|( temp &037));

	
	
	strftn = (temp==8) || (temp==9);
	if( strftn && simpstr( p->dimoff, p->sizoff ) != 8 )
		strftn = 0;

	bfcode(argstk, argno, p->sname);

	regargs();

	
	if( strftn ) 
	{

		  
		
		i = tsize( temp, p->dimoff, p->sizoff);

		sretname(i);



	}
}


regargs()
{
	register i;
	register NODE *p, *q;
	register struct symtab *s;
	
	for( i=0; i<argno; ++i )
	{
		s = &stab[argstk[i]];
		if( s->sclass == 4 )
		{
			int temp;
			idname = argstk[i];
			p = buildtree( 2, (NODE *)0, (NODE *)0 );
			temp = s->offset;
			s->offset = argsoff[i];
			s->sclass = 9;
			q = buildtree( 2, (NODE *)0, (NODE *)0 );
			p = buildtree( 58, p, q );
			ecomp( p );
			s->offset = temp;
			s->sclass = 4;
		}
	}
}


NODE *
rstruct( idn, soru )
register idn,soru;
{
	 
	register struct symtab *p;
	register NODE *q;
	p = &stab[idn];
	switch( p->stype )
	{
	case 17:
def:
		q = block( 97, (NODE *)0, (NODE *)0, 0, 0, 0 );
		q->tn.rval = idn;
		q->in.type = (soru&02) ? 8 : ( (soru&04) ? 9 : 10 );
		defid( q, (soru&02) ? 10 : ( (soru&04) ? 12 : 15 ) );
		break;
	case 8:
		if( soru & 02 ) break;
		goto def;
	case 9:
		if( soru & 04 ) break;
		goto def;
	case 10:
		if( !(soru&(04|02)) ) break;
		goto def;
	}
	stwart = instruct;
	return( mkty( p->stype, 0, p->sizoff ) );
}

moedef( idn )
register idn;
{
	register NODE *q;

	q = block( 97, (NODE *)0, (NODE *)0, 11, 0, 0 );
	q->tn.rval = idn;
	if( idn>=0 ) defid( q, 16 );
}

extern char *hash(), *index(), *rindex();
static int fakeprev, fakenum;
static char fakestub[32], fakename[64];

bstruct( idn, soru )
register idn,soru;
{
	 
	register NODE *q;

	if (idn < 0) {
		char *p;
		if ((p = rindex(ftitle, '/')) == 0)
			p = ftitle;
		strcpy(fakestub, ++p);
		if (p = index(fakestub, '"'))
			*p = '\0';
		sprintf(fakename, "%s$%d.%d", fakestub, lineno, ++fakenum);
		idn = lookup(hash(fakename), 0100);
	}
	psave( instruct );
	psave( curclass );
	psave( strucoff );
	strucoff = 0;
	instruct = soru;
	q = block( 97, (NODE *)0, (NODE *)0, 0, 0, 0 );
	q->tn.rval = idn;
	if( instruct==02 )
	{
		curclass = 8;
		q->in.type = 8;
		defid( q, 10 );
	}
	else if( instruct == 04 ) 
	{
		curclass = 11;
		q->in.type = 9;
		defid( q, 12 );
	}
	else 
	{
		 
		curclass = 16;
		q->in.type = 10;
		defid( q, 15 );
	}
	psave( idn = q->tn.rval );
	
	if( idn >= 0 ) stab[idn].suse = lineno;
	return( paramno-4 );
}





NODE *
dclstruct( oparam )
register oparam;
{
	register struct symtab *p;
	register i, al, sa, j, sz, szindex;
	register TWORD temp;
	register high, low;

	










	if( (i=paramstk[oparam+3]) < 0 )
	{
		szindex = curdim;
		dstash( 0 );  
		dstash( -1 );  
		dstash( 8 );  
		dstash( -lineno );  
	}
	else 
	{
		szindex = stab[i].sizoff;
	}



	if( ddebug )
	{
		printf( "dclstruct( %szindex = %d\n",
			(i>=0)? stab[i].sname : "??", szindex );
	}

	temp = (instruct&02)?8:((instruct&04)?9:10);
	stwart = instruct = paramstk[ oparam ];
	curclass = paramstk[ oparam+1 ];
	dimtab[ szindex+1 ] = curdim;
	al = 8;

	high = low = 0;

	for( i = oparam+4;  i< paramno; ++i )
	{
		dstash( j=paramstk[i] );
		if( j<0 || j>= 6000 ) cerror( "gummy structure member" );
		p = &stab[j];
		if( temp == 10 )
		{
			if( p->offset < low ) low = p->offset;
			if( p->offset > high ) high = p->offset;
			p->sizoff = szindex;
			continue;
		}
		sa = talign( p->stype, p->sizoff );
		if( p->sclass & 0100 )
		{
			sz = p->sclass&077;
		}
		else 
		{
			sz = tsize( p->stype, p->dimoff, p->sizoff );
		}
		if( sz == 0 )
		{
			werror( "structure member has size 0: %s", p->sname );
		}
		if( sz > strucoff ) strucoff = sz;  
		if( ( al)%( sa ) != 0 )  al = ( (( al)/( sa ) + 1) * ( sa ));
		
	}
	dstash( -1 );  
	if( ( strucoff)%( al ) != 0 )  strucoff = ( (( strucoff)/( al ) + 1) * ( al ));

	if( temp == 10 )
	{
		register TWORD ty;

		ty = 4;
		strucoff = tsize( ty, 0, (int)ty );
		dimtab[ szindex+2 ] = al = talign( ty, (int)ty );
	}

	if( strucoff == 0 ) uerror( "zero sized structure" );
	dimtab[ szindex ] = strucoff;
	dimtab[ szindex+2 ] = al;
	dimtab[ szindex+3 ] = paramstk[ oparam+3 ];  




	if( ddebug>1 )
	{
		printf( "\tdimtab[%d,%d,%d,%d] = %d,%d,%d,%d\n",
		szindex,szindex+1,szindex+2,szindex+3,
		dimtab[szindex],dimtab[szindex+1],dimtab[szindex+2],
		dimtab[szindex+3] );
		for( i = dimtab[szindex+1]; dimtab[i] >= 0; ++i )
		{
			printf( "\tmember %s(%d)\n", stab[dimtab[i]].sname, dimtab[i] );
		}
	}


	strucoff = paramstk[ oparam+2 ];
	paramno = oparam;

	return( mkty( temp, 0, szindex ) );
}


yyerror( s, a, b, c, d, e, f, g ) char *s; 

{
	 
	uerror( s, a, b, c, d, e, f, g );

}

yyaccpt()
{
	ftnend();
}

ftnarg( idn ) 
register idn;
{
	register struct symtab *p;
	p = &stab[idn];
	switch( p->stype )
	{

	case 17:
		
		break;
	case 1:
		uerror("redeclaration of formal parameter, %s",p->sname);
		
	case 0100:
		
		
	default:
		idn = hide(p);
		p = &stab[idn];
		break;
	case 0:
		
		;
	}
	p->stype = 1;
	p->sclass = 9;
	asave( idn );
}

talign( ty, s)
register unsigned ty; 
register s; 
{
	

	register i;


	for( i=0; i<=(32-5-1); i+=2 )
	{
		switch( (ty>>i)&(03<<5) )
		{

		case 0100:
			cerror( "compiler takes alignment of function");
		case 040:
			return( 32 );
		case 0140:
			continue;
		case 0:
			break;
		}
	}

	switch( (ty&037) )
	{

	case 9:
	case 10:
	case 8:
		return( dimtab[ s+2 ] );
	case 2:
	case 12:
		return( 8 );
	case 6:
		return( 32 );
	case 7:
		return( 32 );
	case 5:
	case 15:
		return( 32 );
	case 3:
	case 13:
		return( 16 );
	default:
		return( 32 );
	}
}

OFFSZ
tsize( ty, d, s )
register TWORD ty; 
{
	


	

	register i;
	register OFFSZ mult;

	mult = 1;

	for( i=0; i<=(32-5-1); i+=2 )
	{
		switch( (ty>>i)&(03<<5) )
		{

		case 0100:
			uerror("illegal use of function pointer");
		case 040:
			return( 32 * mult );
		case 0140:
			mult *= dimtab[ d++ ];
			continue;
		case 0:
			break;

		}
	}
	if(ty == 10)
		return(32);
	if(ty == 16)
		uerror("using a void value");
	if( dimtab[s]==0 ) {
		uerror( "unknown size");
		return( 32 );
	}
	return( dimtab[ s ] * mult );
}

inforce( n )
register OFFSZ n; 
{
	  
	
	register OFFSZ wb;
	register rest;
	

	if( inoff == n ) return;
	if( inoff > n ) 
	{
		uerror("too many initializers (%d>%d)", inoff, n);
		inoff = n;
		return;
	}

	wb = inoff;
	if( ( wb)%( 32 ) != 0 )  wb = ( (( wb)/( 32 ) + 1) * ( 32 ));

	
	if( wb >= n )
	{
		 
		rest = n - inoff;
		vfdzero( rest );
		return;
	}

	

	rest = wb - inoff;
	vfdzero( rest );

	

	rest = (n-inoff)/32;
	zecode( rest );

	

	rest = n-inoff;
	vfdzero( rest );
	if( inoff != n ) cerror( "inoff error");

}

vfdalign( n )
{
	 
	register OFFSZ m;

	m = inoff;
	if( ( m)%( n ) != 0 )  m = ( (( m)/( n ) + 1) * ( n ));
	inforce( m );
}



extern int idebug;


int ibseen = 0;  

int iclass;  

int ilocctr = 0;  

beginit(curid)
{
	
	register struct symtab *p;


	if( idebug >= 3 ) printf( "beginit(), curid = %d\n", curid );


	p = &stab[curid];

	iclass = p->sclass;
	if( curclass == 2 || curclass == 14 ) iclass = 2;
	switch( iclass )
	{

	case 12:
	case 2:
		return;
	case 1:
	case 4:
		break;
	case 5:
	case 3:
		ilocctr = ((p->stype&(03<<5))==0140)?1:2;
		locctr( ilocctr );
		defalign( talign( p->stype, p->sizoff ) );
		if( p->slevel>1 && p->sclass == 3 ) deflab( p->offset );
		else defnam( p );

	}

	inoff = 0;
	ibseen = 0;

	pstk = 0;

	instk( curid, p->stype, p->dimoff, p->sizoff, inoff );

}

instk( id, t, d, s, off )
register OFFSZ off; 
register TWORD t; 

{
	

	register struct symtab *p;

	for(;;)
	{

		if( idebug ) printf( "instk((%d, %o,%d,%d, %d)\n", id, t, d, s, off );


		

		if( !pstk ) pstk = instack;
		else ++pstk;

		pstk->in_fl = 0;	
		pstk->in_id =  id ;
		pstk->in_t =  t ;
		pstk->in_d =  d ;
		pstk->in_s =  s ;
		pstk->in_n = 0;  
		pstk->in_x =  t==8 ?dimtab[s+1] : 0 ;
		pstk->in_off = off;  
		
		
		if( ((t&(03<<5))==0140) )
		{
			pstk->in_sz = tsize( (((t>>2)&~037)|(t&037)), d+1, s );
		}
		else if( stab[id].sclass & 0100 )
		{
			pstk->in_sz = - ( stab[id].sclass & 077 );
		}
		else 
		{
			pstk->in_sz = 0;
		}

		if( (iclass==1 || iclass == 4 ) &&
		    (((t&(03<<5))==0140) || t==8) )
			uerror( "no automatic aggregate initialization" );

		

		if( ((t&(03<<5))==0140) )
		{
			t = (((t>>2)&~037)|(t&037));
			++d;
			continue;
		}
		else if( t == 8 )
		{
			id = dimtab[pstk->in_x];
			p = &stab[id];
			if( p->sclass != 8 && !(p->sclass&0100) )
				cerror( "insane structure member list" );
			t = p->stype;
			d = p->dimoff;
			s = p->sizoff;
			off += p->offset;
			continue;
		}
		else return;
	}
}

NODE *
getstr()
{
	 

	register l, temp;
	register NODE *p;

	if( (iclass==5||iclass==3) &&
	    (pstk->in_t == 2 || pstk->in_t == 12) &&
	    pstk!=instack && (( pstk[-1].in_t &(03<<5))==0140) )
	{
		
		strflg = 1;
		ilbrace();  
		inforce( pstk->in_off );

		



		
		lxstr((pstk-1)!=instack?dimtab[(pstk-1)->in_d]:0);
		irbrace();  
		return( (NODE *)0 );
	}
	else 
	{
		 
		if( iclass != 0 )
		{
			 
			
			vfdalign( 32 );
		}
		
		temp = locctr( blevel==0?3:4 );
		deflab( l = getlab() );
		strflg = 0;
		lxstr(0); 
		locctr( blevel==0?ilocctr:temp );
		p = buildtree( 3, (NODE *)0, (NODE *)0 );
		p->tn.rval = -l;
		return(p);
	}
}

putbyte( v )
{
	 
	register NODE *p;
	p = bcon(v);
	incode( p, 8 );
	tfree( p );
	gotscal();
}

endinit()
{
	register TWORD t;
	register d, s, n, d1;


	if( idebug ) printf( "endinit(), inoff = %d\n", inoff );


	switch( iclass )
	{

	case 2:
	case 1:
	case 4:
		iclass = 0;
		return;
	}

	pstk = instack;

	t = pstk->in_t;
	d = pstk->in_d;
	s = pstk->in_s;
	n = pstk->in_n;

	if( ((t&(03<<5))==0140) )
	{
		d1 = dimtab[d];

		
		vfdalign( pstk->in_sz );

		n = inoff/pstk->in_sz;  
		if( d1 >= n )
		{
			
			inforce( tsize( t, d, s ) );
			n = d1;
		}
		if( d1!=0 && d1!=n ) uerror( "too many initializers (%d>%d)",
			n, d1);
		if( n==0 ) werror( "empty array declaration");
		dimtab[d] = n;
	}

	else if( t == 8 || t == 9 )
	{
		
		inforce( tsize( t, d, s ) );
	}
	else if( n > 1 ) uerror("%d initializers for a scalar", n);
	
	else inforce( tsize(t,d,s) );

	paramno = 0;
	vfdalign( 32 );
	inoff = 0;
	iclass = 0;

}

doinit( p )
register NODE *p; 

{

	
	



	register sz, d, s;
	register TWORD t;

	

	if(nerrors)
		goto leave;
	if( iclass < 0 ) goto leave;
	if( iclass == 2 || iclass == 12 )
	{
		uerror( "cannot initialize extern or union" );
		iclass = -1;
		goto leave;
	}

	if( iclass == 1 || iclass == 4 )
	{
		


		bccode();
		idname = pstk->in_id;
		p = buildtree( 58, buildtree( 2, (NODE *)0, (NODE *)0 ), p );
		ecomp(p);
		return;
	}

	if( p == (NODE *)0 ) return;  

	if( ibseen )
	{
		uerror( "} expected");
		goto leave;
	}


	if( idebug > 1 ) printf( "doinit(%o)\n", p );


	t = pstk->in_t;  
	d = pstk->in_d;
	s = pstk->in_s;
	if( pstk->in_sz < 0 )
	{
		  
		sz = -pstk->in_sz;
	}
	else 
	{
		sz = tsize( t, d, s );
	}

	inforce( pstk->in_off );

	p = buildtree( 58, block( 2, (NODE *)0,(NODE *)0, t, d, s ), p );
	p->in.left->in.op = 97;
	p->in.left = p->in.right;
	p->in.right = (NODE *)0;
	p->in.left = optim( p->in.left );
	if( p->in.left->in.op == 2+ 14 )
	{
		p->in.left->in.op = 97;
		p->in.left = p->in.left->in.left;
	}
	p->in.op = 113;
	p = optim(p);


	if( p->in.left->in.op == 5 )
	{
		fincode( p->in.left->fpn.dval, sz );
	}
	else

		if( p->in.left->in.op != 4 )
		{
			uerror( "initialization by non-constant" );
			inoff += sz;
		}
		else 
		{
			
			if( sz < 32 )
			{
				 
			incode( p->in.left, sz );
			}
			else 
			{

				ecode( optim(p) );
				inoff += sz;

			}
		}

	gotscal();

leave:
	tfree(p);
}

gotscal()
{
	register t, ix;
	register n, id;
	register struct symtab *p;
	register OFFSZ temp;

	for( ; pstk > instack; ) 
	{

		if( pstk->in_fl ) ++ibseen;

		--pstk;

		t = pstk->in_t;

		if( t == 8 )
		{
			ix = ++pstk->in_x;
			if( (id=dimtab[ix]) < 0 ) continue;

			

			p = &stab[id];
			instk( id, p->stype, p->dimoff, p->sizoff, p->offset+pstk->in_off );
			return;
		}
		else if( ((t&(03<<5))==0140) )
		{
			n = ++pstk->in_n;
			if( n >= dimtab[pstk->in_d] && pstk > instack ) continue;

			

			temp = pstk->in_sz;
			instk( pstk->in_id, (TWORD)(((pstk->in_t>>2)&~037)|(pstk->in_t&037)), pstk->in_d+1, pstk->in_s,
			pstk->in_off+n*temp );
			return;
		}

	}

}

ilbrace()
{
	 
	register t;
	register struct instk *temp;

	temp = pstk;

	for( ; pstk > instack; --pstk )
	{

		t = pstk->in_t;
		if( t != 8 && !((t&(03<<5))==0140) ) continue; 
		if( pstk->in_fl )
		{
			 
			if( pstk->in_n ) uerror( "illegal {");
			continue;
		}

		
		pstk->in_fl = 1;
		break;
	}

	
	

	pstk = temp;
}

irbrace()
{
	


	if( idebug ) printf( "irbrace(): paramno = %d on entry\n", paramno );


	if( ibseen ) 
	{
		--ibseen;
		return;
	}

	for( ; pstk > instack; --pstk )
	{
		if( !pstk->in_fl ) continue;

		

		pstk->in_fl = 0;  
		gotscal();  
		return;
	}

	

}

upoff( size, alignment, poff )
register alignment, *poff; 
{
	




	register off;

	off = *poff;
	if( ( off)%( alignment ) != 0 )  off = ( (( off)/( alignment ) + 1) * ( alignment ));
	if( (maxoffset-off) < size )
	{
		if( instruct!=02 )cerror("local variables fill memory");
		else cerror("structure larger than address space");
	}
	*poff = off+size;
	return( off );
}

oalloc( p, poff )
register struct symtab *p; 
register *poff; 
{
	
	register al, off, tsz;
	int noff;

	al = talign( p->stype, p->sizoff );
	noff = off = *poff;
	tsz = tsize( p->stype, p->dimoff, p->sizoff );

	if( p->sclass == 1 )
	{
		if( (maxoffset-off) < tsz ) cerror("local variables fill memory");
		noff = off + tsz;
		if( ( noff)%( al ) != 0 )  noff = ( (( noff)/( al ) + 1) * ( al ));
		off = -noff;
	}
	else


			if( p->sclass == 9 && ( tsz % 32 ) )
			{	

				tsz = (tsz+32-1)/32;
				off = upoff(tsz*32, 32, &noff );

			}
			else
			{
				off = upoff( tsz, al, &noff );
			}

	if( p->sclass != 4 )
	{
		 
		if( p->offset == (-10201) ) p->offset = off;
			else if( off != p->offset ) return(1);
	}

	*poff = noff;
	return(0);
}

falloc( p, w, new, pty )
register struct symtab *p; 
register NODE *pty; 
{
	
	

	register al,sz,type;

	type = (new<0)? pty->in.type : p->stype;

	
	switch( new<0?pty->in.type:p->stype )
	{

	case 10:
		{
			register s;
			s = new<0 ? pty->fn.csiz : p->sizoff;
			al = dimtab[s+2];
			sz = dimtab[s];
			break;
		}

	case 2:
	case 12:
		al = 8;
		sz = 8;
		break;

	case 3:
	case 13:
		al = 16;
		sz = 16;
		break;

	case 4:
	case 14:
		al = 32;
		sz = 32;
		break;


	default:

			uerror( "illegal field type" );
			al = 32;

	}

	if( w > sz ) 
	{
		uerror( "field too big (%d>%d)", w, sz);
		w = sz;
	}

	if( w == 0 )
	{
		 
		if( ( strucoff)%( al ) != 0 )  strucoff = ( (( strucoff)/( al ) + 1) * ( al ));
		if( new >= 0 ) uerror( "zero size field");
		return(0);
	}

	if( strucoff%al + w > sz ) if( ( strucoff)%( al ) != 0 )  strucoff = ( (( strucoff)/( al ) + 1) * ( al ));
	if( new < 0 ) 
	{
		if( (maxoffset-strucoff) < w )
			cerror("structure fills memory");
		strucoff += w;  
		return(0);
	}

	

	if( new == 1 ) 
	{
		 
		if( p->offset != strucoff || p->sclass != (0100|w) ) return(1);
	}
	p->offset = strucoff;
	if( (maxoffset-strucoff) < w ) cerror("structure overflows memory");
	strucoff += w;
	p->stype = type;
	return(0);
}





nidcl( p )
register NODE *p; 
{
	 
	
	register class;
	register commflag;  

	commflag = 0;

	
	if( (class=curclass) == 0 )
	{
		if( blevel > 1 ) class = 1;
		else if( blevel != 0 || instruct ) cerror( "nidcl error" );
		else 
		{
			 
			if( (class = 2) == 2 ) commflag = 1;
		}
	}

	defid( p, class );

	if( class==5 || class==3 ) 
	{


		commflag = 1;


	}
	if( commflag ) commdec( p->tn.rval );
}

TWORD
types( t1, t2, t3 )
register TWORD t1, t2, t3; 
{
	

	TWORD t[3];
	register TWORD  noun, adj, unsg;
	register i;

	t[0] = t1;
	t[1] = t2;
	t[2] = t3;

	unsg = 4;  
	noun = 17;  
	adj = 4;  

	for( i=0; i<3; ++i )
	{
		switch( t[i] )
		{

		default:
bad:
			uerror( "illegal type combination" );
			return( 4 );

		case 17:
			continue;

		case 14:
			if( unsg != 4 ) goto bad;
			unsg = 14;
			continue;

		case 5:
		case 3:
			if( adj != 4 ) goto bad;
			adj = t[i];
			continue;

		case 4:
		case 2:
		case 6:
			if( noun != 17 ) goto bad;
			noun = t[i];
			continue;
		}
	}

	
	if( noun == 17 ) noun = 4;
	else if( noun == 6 )
	{
		if( unsg != 4 || adj == 3 ) goto bad;
		return( adj==5 ? 7 : 6 );
	}
	else if( noun == 2 && adj != 4 ) goto bad;

	
	if( adj != 4 ) noun = adj;
	if( unsg == 14 ) return( noun + (14-4) );
	else return( noun );
}

NODE *
tymerge( typ, idp )
register NODE *typ, *idp; 

{
	

	register unsigned t;
	register i;

	if( typ->in.op != 33 ) cerror( "tymerge: arg 1" );
	if(idp == (NODE *)0 ) return( (NODE *)0 );


	if( ddebug > 2 ) eprint(idp);


	idp->in.type = typ->in.type;
	idp->fn.cdim = curdim;
	tyreduce( idp );
	idp->fn.csiz = typ->fn.csiz;

	for( t=typ->in.type, i=typ->fn.cdim; t&(03<<5); t = (((t>>2)&~037)|(t&037)) )
	{
		if( ((t&(03<<5))==0140) ) dstash( dimtab[i++] );
	}

	

	idp->in.type = ctype( idp->in.type );

	if( (t = (idp->in.type&037)) != 8 && t != 9 && t != 10 )
	{
		idp->fn.csiz = t;  
	}

	return( idp );
}

tyreduce( p )
register NODE *p; 

{

	
	

	register o, temp;
	register unsigned t;

	o = p->in.op;
	p->in.op = 97;

	if( o == 2 ) return;

	t = ((( p->in.type &~037)<<2)|040|( p->in.type &037));
	if( o == 2+ 70 ) t += (0100-040);
	else if( o == 54 )
	{
		t += (0140-040);
		temp = p->in.right->tn.lval;
		p->in.right->in.op = 97;
	}

	p->in.left->in.type = t;
	tyreduce( p->in.left );

	if( o == 54 ) dstash( temp );

	p->tn.rval = p->in.left->tn.rval;
	p->in.type = p->in.left->in.type;

}

fixtype( p, class )
register NODE *p; 
register class;
{
	register unsigned t, type;
	register mod1, mod2;
	

	if( (type = p->in.type) == 17 ) return;
	if( mod2 = (type&(03<<5)) )
	{
		t = (((type>>2)&~037)|(type&037));
		while( mod1=mod2, mod2 = (t&(03<<5)) )
		{
			if( mod1 == 0140 && mod2 == 0100 )
			{
				uerror( "array of functions is illegal" );
				type = 0;
			}
			else if( mod1==0100 && ( mod2==0140 || mod2==0100 ) )
			{
				uerror( "function returns array or function" );
				type = 0;
			}
			t = (((t>>2)&~037)|(t&037));
		}
	}

	
	
	

	if( class==0 && blevel==1 && !(instruct&(02|04)) )
		class = 9;
	if( class == 9 || ( class==4 && blevel==1 ) )
	{
		if( type == 6 ) type = 7;
		else if( ((type&(03<<5))==0140) )
		{
			++p->fn.cdim;
			type += (040-0140);
		}
		else if( ((type&(03<<5))==0100) )
		{
			werror( "a function is declared as an argument" );
			type = (((type&~037)<<2)|040|(type&037));
		}
	}

	if( instruct && ((type&(03<<5))==0100) )
	{
		uerror( "function illegal in structure or union" );
		type = (((type&~037)<<2)|040|(type&037));
	}
	p->in.type = type;
}


TWORD
ctype( t )
register TWORD t; 
{
	register TWORD bt;
	bt = (t&037);







	if( bt==5 ) t = (t&(~037))|4;
	else if( bt==15) t = (t&(~037))|14;




	return( t );
}


uclass( class ) 
register class; 
{
	
	if( class == 0 ) return( 2 );
	else if( class == 3 ) return( 18 );
	else if( class == 14 ) return( 17 );
	else return( class );
}

fixclass( class, type )
register TWORD type; 
register class;

{

	

	if( class == 0 )
	{
		if( instruct&02 ) class = 8;
		else if( instruct&04 ) class = 11;
		else if( blevel == 0 ) class = 5;
		else if( blevel == 1 ) class = 9;
		else class = 1;

	}

	
	if(blevel == 1 && (class == 3 || class == 2))
		uerror("parameter declared with weird storage classs");
	if( (( type &(03<<5))==0100) )
	{
		switch( class ) 
		{
		default:
			uerror( "function has illegal storage class" );
		case 1:
			class = 2;
		case 2:
		case 5:
		case 14:
		case 13:
		case 3:
		case 17:
		case 18:
			;
		}
	}

	if( class&0100 )
	{
		if( !(instruct&02) ) uerror( "field not in structure" );
		return( class );
	}

	switch( class )
	{

	case 11:
		if( !(instruct&04) ) uerror( "illegal class" );
		return( class );

	case 8:
		if( !(instruct&02) ) uerror( "illegal class" );
		return( class );

	case 16:
		if( instruct & (02|04) ) uerror( "illegal class" );
		return( class );

	case 4:
		if( blevel == 0 ) uerror( "register declaration outside a fncn" );
		
		else if( cisreg( type ) ) return( class );
		if( blevel == 1 ) return( 9 );
		else return( 1 );

	case 1:
	case 6:
	case 7:
		if( blevel < 2 ) uerror( "auto outside function" );
		return( class );

	case 9:
		if( blevel != 1 ) uerror( "illegal class" );
		return( class );

	case 17:
	case 14:

		if( !((type&(03<<5))==0100) ) uerror( "fortran declaration must apply to function" );
		else 
		{
			type = (((type>>2)&~037)|(type&037));
			if( ((type&(03<<5))==0100) || ((type&(03<<5))==0140) || ((type&(03<<5))==040) ) 
			{
				uerror( "fortran function has wrong type" );
			}
		}
	case 10:
	case 12:
	case 15:
	case 2:
	case 3:
	case 5:
	case 13:
	case 18:
		return( class );

	default:
		cerror( "illegal class: %d", class );
		
	}
}

struct symtab *
mknonuniq(idindex)
register *idindex; 
{
	
	
	
	register i;
	register struct symtab * sp;

	sp = & stab[ i= *idindex ]; 
	while( sp->stype != 0 )
	{
		 
		if( ++i >= 6000 )
		{
			
			i = 0;
			sp = stab;
		}
		else ++sp;
		if( i == *idindex ) cerror("Symbol table full");
	}
	sp->sflags = 040 | 01;
	sp->sname = stab[*idindex].sname; 
	*idindex = i;

	if( ddebug )
	{
		printf( "\tnonunique entry for %s from %d to %d\n",
		sp->sname, *idindex, i );
	}

	return ( sp );
}



struct symtab *
relook(p)
register struct symtab *p; 

{
	  
	register struct symtab *q;

	
	q = &stab[lookup( p->sname, p->sflags&(0100|01|02) )];
	
	if( q->stype == 17 )
	{
		q->stype = 0;
		return(q);
	}
	while( q != p )
	{
		if( q->stype == 0 ) break;
		if( ++q >= &stab[6000] ) q=stab;
	}
	return(q);
}

uplevel()
{
	++blevel;

	dblbrac();

}

clearst( lev )
{
	 
	extern struct symtab *scopestack[];
	register struct symtab *p, *q, *r;


	


	aobeg();
	p = scopestack[blevel];
	while (p)
	{
		aocode(p);
		p = p->scopelink;
	}
	aoend();
	if (lev != 1)	
		dbrbrac();


	

	q = (struct symtab *) 0;
	



	p = scopestack[lev];
	while (p)
	{

		if (ddebug)
			printf("removing %s = stab[ %d], flags %o level %d\n",
			    p->sname,p-stab,p->sflags,p->slevel);

		
		if (p->stype == 17 ||
				( p->sclass == 7 && lev <=2) )  {
			uerror("%s undefined", p->sname);
		}
		if( p->sflags & 04 )
			unhide(p);
		if( (r=relook(p)) != p )
			*r = *p;
		if ( lev > p->slevel ) {
		

			r = p;
			p = p->scopelink;
			r->scopelink = q;
			q = r;
		} else {
			p->stype = 0;
			p = p->scopelink;
		}
	}
	scopestack[lev] = (struct symtab *) 0;
	
	while( q )
	{
	    r = &stab[ lookup( q->sname, q->sflags ) ];
	    if ( r != q )
	    {
		*r = *q;
		q->stype = 0;
	    }
	    q = q->scopelink;
	    r->scopelink = scopestack[ lev-1 ];
	    scopestack[ lev-1 ] = r;
	}
}

hide( p )
register struct symtab *p; 
{
	register struct symtab *q;
	for( q=p+1; ; ++q )
	{
		if( q >= &stab[6000] ) q = stab;
		if( q == p ) cerror( "symbol table full" );
		if( q->stype == 0 ) break;
	}
	*q = *p;
	p->sflags |= 02;
	q->sflags = (p->sflags&(01|0100)) | 04;
	if( hflag ) werror( "%s redefinition hides earlier one", p->sname );

	if( ddebug ) printf( "	%d hidden in %d\n", p-stab, q-stab );

	return( idname = q-stab );
}

unhide( p )
register struct symtab *p; 
{
	register struct symtab *q;
	register s;

	s = p->sflags & (01|0100);
	q = p;

	for(;;)
	{

		if( q == stab ) q = &stab[6000-1];
		else --q;

		if( q == p ) break;

		if( (q->sflags&(01|0100)) == s )
		{
			if ( p->sname == q->sname )
			{
				 
				q->sflags &= ~02;

				if( ddebug ) printf( "unhide uncovered %d from %d\n", q-stab,p-stab);

				return;
			}
		}

	}
	cerror( "unhide fails" );
}
