#include <ctype.h>
#include "defs"

char * copys(s)
register char *s;
{
register char *t;
char *k;
ptr calloc();

for(t=s; *t++ ; );
if( (k = (char *)calloc( t-s , sizeof(char))) == NULL)
	fatal("Cannot allocate memory");

for(t=k ; *t++ = *s++ ; );
return(k);
}



equals(a,b)
register char *a,*b;
{
if(a==b) return(YES);

while(*a == *b)
	if(*a == '\0') return(YES);
	else {++a; ++b;}

return(NO);
}


char *concat(a,b,c)   /* c = concatenation of a and b */
register char *a,*b;
char *c;
{
register char *t;
t = c;

while(*t = *a++) t++;
while(*t++ = *b++);
return(c);
}





ptr conrep(a,b)
char *a, *b;
{
char *s;

s = (char *)intalloc( strlen(a)+strlen(b)+1 );
concat(a,b,s);
cfree(a);
return((int *)s);
}


eqcon(p,q)
register ptr p, q;
{
int pt, qt;

if(p==q) return(YES);
if(p==NULL || q==NULL) return(NO);
pt = p->tag;
qt = q->tag;
if(pt==TNEGOP && qt==TNEGOP)
	return( eqcon(p->leftp, q->leftp) );
if(pt==TCONST && qt==TNEGOP)
	return(NO);
if(pt==TNEGOP && qt==TCONST)
	return(NO);
if(p->tag==TCONST && q->tag==TCONST)
	return( equals(p->leftp,q->leftp) );

fatal("eqcon: nonconstant argument");
/* NOTREACHED */ return 0;
}



char *convic(n)
register int n;
{
static char s[20];
register char *t;

s[19] = '\0';
t = s+19;

do	{
	*--t = '0' + n%10;
	n /= 10;
	} while(n > 0);

return(t);
}



conval(p)
register ptr p;
{
int val;
if(isicon(p, &val))
	return(val);
fatal("bad conval");
return 0;
}



isicon(p, valp)
ptr p;
int *valp;
{
int val1;

if(p)
    switch(p->tag)
	{
	case TNEGOP:
		if(isicon(p->leftp, &val1))
			{
			*valp = - val1;
			return(1);
			}
		break;

	case TCONST:
		if(p->vtype == TYINT)
			{
			*valp = convci(p->leftp);
			return(YES);
			}
	default:
		break;
	}
return(NO);
}



isconst(p)
ptr p;
{
return(p->tag==TCONST  ||  (p->tag==TNEGOP && isconst(p->leftp)) );
}



iszero(s)
register char *s;
{
if(s == NULL)
	return(YES);
while( *s=='+' || *s=='-' || *s==' ' )
	++s;
while( *s=='0' || *s=='.' )
	++s;
switch( *s )
	{
	case 'd':
	case 'e':
	case 'D':
	case 'E':
	case ' ':
	case '\0':
		return(YES);
	default:
		return(NO);
	}
}




convci(p)
register char *p;
{
register int n;
register int sgn;

n = 0;
sgn = 1;
for( ; *p ; ++p)
	if(*p == '-')
		sgn = -1;
	else if( isdigit(*p) )
		n = 10*n + (*p - '0');

return(sgn * n);
}



chainp hookup(x,y)
register chainp x, y;
{
register chainp p;

if(x == NULL)
	return(y);
for(p=x ; p->nextp ; p = (chainp)p->nextp)
	;

p->nextp = (int *)y;
return(x);
}


ptr cpexpr(p)
register ptr p;
{
register ptr e;
ptr q, q1;

if(p == NULL)
	return(NULL);

e = allexpblock();
cpblock(p, e, sizeof(struct exprblock));

switch(p->tag)
	{
	case TAROP:
	case TRELOP:
	case TLOGOP:
	case TASGNOP:
	case TCALL:
		e->rightp = cpexpr(p->rightp);

	case TNOTOP:
	case TNEGOP:
		e->leftp = cpexpr(p->leftp);
		break;

	case TCONST:
		e->leftp = (int *)copys(p->leftp);
		if(p->rightp)
			e->rightp = (int *)copys(p->rightp);
		if(p->vtype == TYCHAR)
			e->vtypep = cpexpr(p->vtypep);
		break;

	case TLIST:
		q1 = (int *)&(e->leftp);
		for(q = p->leftp ; q ; q = q->nextp)
			q1 = q1->nextp = (int *)mkchain( cpexpr(q->datap), CHNULL);
		break;

	case TTEMP:
	case TNAME:
	case TFTNBLOCK:
		if(p->vsubs)
			e->vsubs = cpexpr(p->vsubs);
		if(p->voffset)
			e->voffset = cpexpr(p->voffset);
		break;

	case TERROR:
		break;

	default:
		badtag("cpexpr", p->tag);
	}
return(e);
}


mvexpr(p,q)
char *p, *q;
{
cpblock(p,q, sizeof(struct exprblock) );
frexpblock(p);
}


cpblock(p,q,n)
register char *p, *q;
int n;
{
register int i;

for(i=0; i<n; ++i)
	*q++ = *p++;
}



strlen(s)
register char *s;
{
register char *t;
for(t=s ; *t ; t++ ) ;
return(t-s);
}


char *procnm()	/* name of the current procedure */
{
return( procname ? ((struct stentry *)procname->sthead)->namep : "" );
}





ptr arg1(a)		/* make an argument list of one value */
ptr a;
{
return( mknode(TLIST,0, mkchain(a,CHNULL), PNULL) );
}



ptr arg2(a,b)	/* make an argumentlist (a,b) */
ptr a,b;
{
register ptr p;

p = (int *)mkchain(a, mkchain(b,CHNULL) );
return( mknode(TLIST,0, p,0) );
}




ptr arg4(a,b)	/* make an argument list of  (a,len(a), b,len(b)) */
ptr a,b;
{
register ptr p;
p = (int *)mkchain(b, mkchain(cpexpr(b->vtypep), CHNULL));
p = (int *)mkchain(a, mkchain(cpexpr(a->vtypep), p));
return( mknode(TLIST,0,p,PNULL));
}



ptr builtin(type,s)
int type;
char *s;
{
register ptr p, q = 0;
ptr mkvar(), mkname();

if(p = (int *)name(s,1))
	{
	if(p->blklevel>1 || (p->tag!=TNAME && p->tag!=TKEYWORD) 
	    || (q=p->varp)==0 || q->vext
	    || (q->vtype!=type && q->vtype!=TYUNDEFINED) )
		{
		exprerr("error involving builtin %s", s);
		return(errnode());
		}
	if(q && q->vtype != TYUNDEFINED)
		return( cpexpr(q) );
	}
else	{
	q = mkvar( mkname(s) );
	if(blklevel > 1)
		{
		q->blklevel = 1;
		((struct headbits *)q->sthead)->blklevel = 1;
		--ndecl[blklevel];
		++ndecl[1];
		}
	}

q->vtype = type;
q->vdclstart = 1;
mkftnp(q);
return( cpexpr(q) );
}



ptr errnode()
{
register struct exprblock * p;

p = (struct exprblock *)allexpblock();
p->tag = TERROR;
p->vtype = TYINT;
return((int *)p);
}



min(a,b)
int a,b;
{
return( a<b ? a : b);
}



setvproc(p, v)
register ptr p;
register int v;
{
ptr q;
register int k;

q = ((struct stentry *)p->sthead)->varp;
k = q->vproc;
/*debug printf("setvproc(%s ,%d)\n", ((struct stentry *)q->sthead)->namep, v); */
if(p != q)
	p->vproc = k;
if(k == v)
	return;

if(k==PROCUNKNOWN || (k==PROCYES && v==PROCINTRINSIC) )
	p->vproc = q->vproc = v;
else if( !(k==PROCINTRINSIC && v==PROCYES)  && ((struct stentry *)p->sthead)->varp!=procname)
	execerr("attempt to use %s as variable and procedure",
		((struct stentry *)p->sthead)->namep);
}
