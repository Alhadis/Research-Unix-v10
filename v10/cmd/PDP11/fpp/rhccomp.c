/*
 *  mjm: alloc(sz) -> own subroutine which calls malloc();
 *	HEADLEN: 4 -> (2*(sizeof(int))
 */
#define Q register struct obj
#define COMMAOPP 1
#define PUTOPP 2
#define AEOROPP 2
#define AIOROPP 2
#define AANDOPP 2
#define ASHROPP 2
#define ASHLOPP 2
#define AMODOPP 2
#define ADIVOPP 2
#define AMULOPP 2
#define ASUBOPP 2
#define APLUSOPP 2
#define QSTOPP 5
#define QSTOPP2 3
#define OROPP 7
#define ANDOPP 8
#define BITIORP 9
#define BITEORP 10
#define BITANDP 11
#define EQOPP 12
#define NEOPP 12
#define GTOPP 13
#define GEOPP 13
#define LTOPP 13
#define LEOPP 13
#define SHLOPP 14
#define SHROPP 14
#define ADDOPP 15
#define SUBOPP 15
#define MULOPP 16
#define DIVOPP 16
#define MODOPP 16
#define CASTP 17
#define UNMINUSP 17
#define UNPLUSP 17
#define UNSTARP 17
#define UNADDRP 17
#define UNNOTP 17
#define UNCOMP 17
#define UNINC1P 17
#define UNINC2P 17
#define UNDEC1P 17
#define UNDEC2P 17
#define SIZEOFP 17
#define DOTOPP 18
#define FCALLOPP 18
#define SUBSCRP 18
#define PTROPP 18

#define F 0 /*floating point type*/
#define N 1 /*norgen in4 or int*/
#define L 2 /*unix long*/
#define I 3 /*unix int*/
#define U 4 /*unix unsigned*/

#define MAXTYPE 5 /* "pointer to ..." adds MAXTYPE to type */

#define DFFUNCT 040000 /* declarator cannot be stored into */
#define FFSTRUCT 020000 /* structure prototype should follow */
#define DFLASTID 010000 /* item is identifier */
#define DFMOD 010000 /* bits below this are part of symbol defn */
#define DFTYPEDF 04000 /* this is a typedef typename */
#define DFFUNCT2 02000 /* this is a typedef typename which can't be */
                             /* stored into */
#define DFSTRUCT 01000 /* this is a structure tag or name */

#define NAMELEN 30
#define NUMBLEN 30
#define STRLEN 30
#define CHCONST 10

#define COMMAOP 1
#define AEOROP 2
#define AIOROP 3
#define AANDOP 4
#define ASHROP 5
#define ASHLOP 6
#define AMODOP 7
#define ADIVOP 8
#define AMULOP 9
#define ASUBOP 10
#define APLUSOP 11
#define QSTOP 12
#define OROP 14
#define ANDOP 15
#define BITIOR 16
#define BITEOR 17
#define BITAND 18
#define EQOP 19
#define NEOP 20
#define GTOP 21
#define GEOP 22
#define LTOP 23
#define LEOP 24
#define SHLOP 25
#define SHROP 26
#define ADDOP 27
#define SUBOP 28
#define MULOP 29
#define DIVOP 30
#define MODOP 31
#define UNMINUS 32
#define UNPLUS 33
#define UNSTAR 34
#define UNADDR 35
#define UNNOT 36
#define UNCOM 37
#define UNINC1 38
#define UNINC2 39
#define UNDEC1 40
#define UNDEC2 41
#define DOTOP 42
#define PTROP 43
#define LP 44
#define RP 45
#define LB 46
#define RB 47
#define LBC 48
#define RBC 49
#define SQ 50
#define DQ 51
#define SHARP 52
#define SC 53 
#define BACK 54
#define CONST 55
#define IDENT 56
#define SIZEOF 57
#define PUTOP 58
#define SUBSCROP 59
#define FCALLOP 60

#define HEADLEN (2*sizeof(int)) /* Q struct 'type' and 'size' fields */  /*mjm*/

#define CHAR 1
#define SHORT 2
#define INT 4
#define LONG 010
#define UNSIGNED 020
#define FLOAT 040
#define DOUBLE 0100
#define STRUCT 0200
#define UNION 0400
#define FLOATN 01000
#define INTN 02000
#define TYPEDF 04000

#define OUT (-1) /* out of storage in alloc() */

/* int outcore {OUT};	mjm removed */

/* if unixfmt is 1, floats are unix-format; else norgen-format */
int unixfmt = {0};

/* if unsign is 1, declare _ftou() and _ntou() unsigned */
int unsign = {1};

/* the value to be returned by gettype() when symbol is undefined 
   should be "function returning int" */
int undefin = MAXTYPE+I;

/* the current function return type */
static int frettype,lasttype;

/* the following flags flag that the sign bit should be set in symbol
	definitions and inquiries: an unforseen necessity to keep
	structure names and variable names separate in the symbol
	table. instruct  affects definitions, and dotid  affects
	inquires */
static int dotid; /* set by expr() after dot (. ->) for ident() */
static int instruct; /* set by structure statement for alt symbol table */

/* this is the {} nesting level for symbol defns, etc. */
int level;

/* a chain of prototype arguments to be defined as type "int" if not
   declared explicitly */
struct argstruct {
	struct argstruct *acnext;
	char *acstring;
	} *argchain;

static struct obj {
	int type; /* the type of an object */
	int size; /* the size of the following string */
	char text[2]; /* the object stored here. variable size */
	};

/* primative operations on Q items */

/* functions called here :
	prfx(string,o)	return object with prefixed string
	psfx(o,string)	return object with string postfixed
	unop(s1,s2,o)	unary minus rules
	unop2(s1,s2,o)	unary ! rules
	unop3(s1,s2,o)	unary ~ rules
	unop4(s1,s2,s3,o)	postfix ++ rules
	unop5(s1,s2,s3,o)	prefix ++ rules
	binop(ol,or,s1,s2)	+ rules
	binop2(ol,or,s)		>> rules
	binop3(ol,or,s1,s2)	> rules
	binop4(ol,or,s)		== rules
	binop5(ol,or,s)		& rules
	binop6(ol,or,s)		: rules
	binop7(ol,or,s)		? rules
	binop8(ol,or,s)		= rules
	binop9(ol,or,s)		, rules
	binop10(ol,or,s)	|| rules
	copy(o)		form a copy of o
	cvt(o,type)	do conversion from o-type to type
	seq(s1,s2)	return string equality
	qcat(ol,or)	cat two objects together
	bcat(s1,ol,s2,or,s3)	form obj from cat of s's and o's
*/

ptrop(ol,or)
Q *ol,*or;
{
/* used to be dotop(prfx("(",psfx(unstar(ol),")")),or), but "C" won't
   take (*0).smember, and will take 0->smember. */

	if((ol->type=ol->type-MAXTYPE)<0)warn("illegal indirection");
	if((ol->type&DFSTRUCT)==0)warn("non-structure used as structure name");
	ol->type=or->type;
	return(qcat(psfx(ol,"->\377"),or));
}
dotop(ol,or)
Q *ol,*or;
{
	if((ol->type&DFSTRUCT)==0) warn("non-structure used as structure name");
	ol->type=or->type;
	return(qcat(psfx(ol,"\377."),or));
}
unstar(op)
Q *op;
{
	if((op->type=op->type-MAXTYPE)<0)warn("illegal indirection");
	return(prfx("\377 *",op));
}
unaddr(op)
Q *op;
{
	op->type += MAXTYPE;
	return(prfx("\377 &",op));
}
unminus(op)
Q *op;
{
/* catch fltg point constants preceeded by minus signs and spaces by
   checking for op->text octal constant and fltg point type.
*/

        if(op->type==F&&op->text[0]=='0'&&op->size==13) {
		if(unixfmt) op->text[1] ^= 2; /* flip sign */
		else op->text[6] ^= 1;
		return(op);
		}
	return(unop("\377 -","\377_funmin(",op));
}
unplus(op)
Q *op;
{
	return(unop("\377 +","\377_funplus(",op));
}
unnot(op)
Q *op;
{
	return(unop2("\377 !","\377!(",op));
}
uncom(op)
Q *op;
{
	return(unop3("\377 ~","\377~(",op));
}
uninc1(op)
Q *op;
{
        return(unop5("\377 ++","\377_finc1(&","\377_ninc1(&",op));
}
uninc2(op)
Q *op;
{
        return(unop4("++\377 ","\377_finc2(&","\377_ninc2(&",op));
}
undec1(op)
Q *op;
{
        return(unop5("\377 --","\377_fdec1(&","\377_ndec1(&",op));
}
undec2(op)
Q *op;
{
        return(unop4("--\377 ","\377_fdec2(&","\377_ndec2(&",op));
}
mulop(ol,or)
Q *ol,*or;
{
	return(binop(ol,or,"*\377","\377_fmul("));
}
divop(ol,or)
Q *ol,*or;
{
	return(binop(ol,or,"/\377","\377_fdiv("));
}
modop(ol,or)
Q *ol,*or;
{
	return(binop(ol,or,"%\377","\377_fmod("));
}
addop(ol,or)
Q *ol,*or;
{
	if(ol->type>=MAXTYPE||or->type>=MAXTYPE) {
                if(ol->type<MAXTYPE) {
			ol=cvt(ol,I);
			ol->type=or->type;
			}
		else or=cvt(or,I);
		}
	return(binop(ol,or,"+\377","\377_fadd("));
}
subop(ol,or)
Q *ol,*or;
{
	if(ol->type>=MAXTYPE||or->type>=MAXTYPE) {
		if(ol->type>=MAXTYPE&&or->type>=MAXTYPE) {
			ol->type=or->type=I;
			}
		else {
			if(ol->type<MAXTYPE) {
				ol=cvt(ol,I);
				ol->type=or->type;
				}
			else or=cvt(or,I);
			}
		}
	return(binop(ol,or,"-\377","\377_fsub("));
}
shlop(ol,or)
Q *ol,*or;
{
	return(binop2(ol,or,"<<\377"));
}
shrop(ol,or)
Q *ol,*or;
{
	return(binop2(ol,or,">>\377"));
}
gtop(ol,or)
Q *ol,*or;
{
	return(binop3(ol,or,">\377","\377_fgt("));
}
geop(ol,or)
Q *ol,*or;
{
	return(binop3(ol,or,">=\377","\377_fge("));
}
ltop(ol,or)
Q *ol,*or;
{
	return(binop3(ol,or,"<\377","\377_flt("));
}
leop(ol,or)
Q *ol,*or;
{
	return(binop3(ol,or,"<=\377","\377_fle("));
}
eqop(ol,or)
Q *ol,*or;
{
	return(binop4(ol,or,"==\377"));
}
neop(ol,or)
Q *ol,*or;
{
	return(binop4(ol,or,"!=\377"));
}
bitand(ol,or)
Q *ol,*or;
{
	return(binop5(ol,or,"&\377"));
}
biteor(ol,or)
Q *ol,*or;
{
	return(binop5(ol,or,"^\377"));
}
bitior(ol,or)
Q *ol,*or;
{
	return(binop5(ol,or,"|\377"));
}
andop(ol,or)
Q *ol,*or;
{
	return(binop10(ol,or,"&&\377"));
}
orop(ol,or)
Q *ol,*or;
{
	return(binop10(ol,or,"||\377"));
}
altop(ol,or)
Q *ol,*or;
{
	return(binop6(ol,or,":\377"));
}
questop(ol,or)
Q *ol,*or;
{
	return(binop7(ol,or,"?\377"));
}
putop(ol,or)
Q *ol,*or;
{
	return(binop8(ol,or,"= \377"));
}
assignop(ol,or,op)
Q *ol,*or;
char *op;
{
char *s;
        if(ol->type!=F&&ol->type!=N) {
		return(binop8(ol,or,op));
		}
	or=cvt(or,ol->type);
	if(ol->type==N) or->type=ol->type=L; /* N returns L */
	s=ol->type==F?"\377_f":"\377_n";

/* if arguments to functions are evaluated left-to-right, each of these
	must be flipped to xxxx(& ol , or )  */

	if(seq(op,"+= ")) return(prfx(s,bcat("eqpl(\377",or,",\377&",ol,")\377")));
	if(seq(op,"-= ")) return(prfx(s,bcat("eqmi(\377",or,",\377&",ol,")\377")));
	if(seq(op,"*= ")) return(prfx(s,bcat("eqmu(\377",or,",\377&",ol,")\377")));
	if(seq(op,"/= ")) return(prfx(s,bcat("eqdv(\377",or,",\377&",ol,")\377")));
	if(seq(op,"&= ")) return(prfx(s,bcat("eqan(\377",or,",\377&",ol,")\377")));
        if(seq(op,"^= ")) return(prfx(s,bcat("eqer(\377",or,",\377&",ol,")\377")));
        if(seq(op,"%= ")) return(prfx(s,bcat("eqmo(\377",or,",\377&",ol,")\377")));
	if(seq(op,"|= ")) return(prfx(s,bcat("eqor(\377",or,",\377&",ol,")\377")));
	if(seq(op,">>= ")) return(prfx(s,bcat("eqsr(\377",or,",\377&",ol,")\377")));
	if(seq(op,"<<= ")) return(prfx(s,bcat("eqsl(\377",or,",\377&",ol,")\377")));
	abt("illegal assign operator");
}
commaop(ol,or)
Q *ol,*or;
{
	return(binop9(ol,or,",\377"));
}

/* primatives called above */

seq(s1,s2) /* are strings equal? */
char *s1,*s2;
{
	while(*s1)if(*s1++!= *s2++)return(0);
	return(*s2==0);
}

cvt(op,ty)	/*convert the type of an object*/
Q *op;
int ty;
{
if(op->type==ty)	return(op);
if(ty==F) {
	if(op->type==L) {
		op->type=F;
		return(prfx("\377_ffrl(",psfx(op,")")));
		}
	if(op->type==N) {
		op->type=F;
		return(prfx("\377_ffrn(",psfx(op,")")));
		}
	if(op->type==U) {
		op->type=F;
		return(prfx("\377_ffru(",psfx(op,")")));
		}
	op->type=F;
	return(prfx("\377_ffri(",psfx(op,")")));
	}
if(ty==L) {
	if(op->type==F) {
		op->type=L;
		return(prfx("\377_ftol(",psfx(op,")")));
		}
	if(op->type==N) {
		op->type=L;
		return(prfx("\377_ntol(",psfx(op,")")));
		}
	op->type=L;
	return(op);
	}
if(ty==N) {
	if(op->type==F) {
		op->type=N;
		return(prfx("\377_fton(",psfx(op,")")));
		}
	if(op->type==L) {
		op->type=N;
		return(prfx("\377_lton(",psfx(op,")")));
		}
	if(op->type==U) {
		op->type=N;
		return(prfx("\377_uton(",psfx(op,")")));
		}
	op->type=N;
	return(prfx("\377_iton(",psfx(op,")")));
	}
if(ty==U) {
	if(op->type==F) {
		op->type=ty;
		return(prfx("\377_ftou(",psfx(op,")")));
		}
	if(op->type==N) {
		op->type=ty;
		return(prfx("\377_ntou(",psfx(op,")")));
		}
	op->type=ty;
	return(op);
	}
else	{
	if(op->type==F) {
		op->type=ty;
		return(prfx("\377_ftoi(",psfx(op,")")));
		}
	if(op->type==N) {
		op->type=ty;
		return(prfx("\377_ntoi(",psfx(op,")")));
		}
	op->type=ty;
	return(op);
	}
}

copy(op)	/* make a copy of op */
Q *op;
{
Q *ans;
int i,k;
        ans=getq(k=op->size);
	ans->type=op->type;
        ans->size=k;
	for(i=0; i<k; i++) ans->text[i]=op->text[i];
	return(ans);
}

prfx(s,op)	/*prefix a string onto op*/
char *s;
Q *op;
{
int i,j,k;
Q *ans;
	for(i=0; s[i]; i++);
	ans=getq((k=op->size)+i);
	for(i=0; *s; i++)ans->text[i]= *s++;
	for(j=0; j<k; j++)ans->text[i++]=op->text[j];
	ans->size=i;
	ans->type=op->type;
	free(op);
	return(ans);
}

psfx(op,s)      /*postfix a string after op */
char *s;
Q *op;
{
int i,j,k;
Q *ans;
        for(i=0; s[i]; i++);
        ans=getq((k=op->size)+i);
        for(i=0; i<k; i++)ans->text[i]=op->text[i];
        while(*s)ans->text[i++]= *s++;
        ans->size=i;
        ans->type=op->type;
        free(op);
        return(ans);
}

getq(sz)	/* get a new Q with size=sz */
int sz;
{
Q *ans;
	if((ans=alloc(sz+HEADLEN))==OUT) {
		abt("out of storage in 'getq()'");
		}
	return(ans);
}
getqs(s)	/* get a new Q with a string in it */
char *s;
{
int i;
Q *ans;
	for(i=0; s[i]; i++);
	ans=getq(i);
	ans->type= -1;
	ans->size=i;
        while(--i>=0)ans->text[i]=s[i];
	return(ans);
}


unop(s1,s2,op)	/* unary minus rules */
char *s1,*s2;
Q *op;
{
	if(op->type==F) {
		return(prfx(s2,psfx(op,")")));
		}
	if(op->type==N) {
		return(prfx(s1,cvt(op,L)));
		}
	else	{
		return(prfx(s1,op));
		}
}

unop2(s1,s2,op)	/* unary ! rules */
char *s1,*s2;
Q *op;
{
	if(op->type==F) {
		op->type=I;
		return(prfx(s2,psfx(op,")")));
		}
	else {
		op->type=I;
		return(prfx(s1,op));
		}
}
unop3(s1,s2,op)	/* unary ~ rules */
char *s1,*s2;
Q *op;
{
	if(op->type==F) {
		return(prfx(s2,psfx(op,")")));
		}
	else {
		return(prfx(s1,op));
		}
}

unop4(s1,s2,s3,op)	/* postfix ++ rules */
char *s1,*s2,*s3;
Q *op;
{
	if(op->type==F) {
		return(prfx(s2,psfx(op,")")));
		}
	if(op->type==N) {
		op->type=L;
		return(prfx(s3,psfx(op,")")));
		}
	else {
		return(psfx(op,s1));
		}
}
unop5(s1,s2,s3,op)	/* prefix ++ rules */
char *s1,*s2,*s3;
Q *op;
{
	if(op->type==F) {
		return(prfx(s2,psfx(op,")")));
		}
	if(op->type==N) {
		op->type=L;
		return(prfx(s3,psfx(op,")")));
		}
	else {
		return(prfx(s1,op));
		}
}

binop(ol,or,s1,s2)	/* + rules */
char *s1,*s2;
Q *ol,*or;
{
	if(ol->type==F||or->type==F) {
		ol=cvt(ol,F); or=cvt(or,F);

/* if arguments are evaluated left-to-right, flip this to read
	bcat(s2,ol,",",or,")")		*/

		return(bcat(s2,or,",\377",ol,")"));
		}
        if(ol->type==N) ol=cvt(ol,L);
	if(or->type==N) or=cvt(or,L);
	if(ol->type==L||or->type==L) {
		ol=cvt(ol,L); or=cvt(or,L);
		return(qcat(psfx(ol,s1),or));
		}
	if(ol->type==U||or->type==U) {
		ol=cvt(ol,U); or=cvt(or,U);
		}
	return(qcat(psfx(ol,s1),or));
}

binop2(ol,or,s)	/* >> rules */
char *s;
Q *ol,*or;
{
	if(or->type!=I) or=cvt(or,I);
	if(ol->type==N) ol=cvt(ol,L);
	return(qcat(psfx(ol,s),or));
}

binop3(ol,or,s1,s2)	/* > rules */
char *s1,*s2;
Q *ol,*or;
{
	if(ol->type==F||or->type==F) {
		ol=cvt(ol,F); or=cvt(or,F);
		ol->type=I;

/* if arguments are evaluated left-to-right, flip this to read
	bcat(s2,ol,",",or,")")		*/

		return(bcat(s2,or,",",ol,")"));
		}
	if(ol->type==N) ol=cvt(ol,L);
	if(or->type==N) or=cvt(or,L);
	ol->type=I;
	return(qcat(psfx(ol,s1),or));
}

binop4(ol,or,s)	/* == rules */
char *s;
Q *ol,*or;
{
	if(ol->type==F||or->type==F) {
		ol=cvt(ol,F); or=cvt(or,F);
		}
	else if(ol->type==N||or->type==N) {
		ol=cvt(ol,N); or=cvt(or,N);
		}
	else if(ol->type==L||or->type==L) {
		ol=cvt(ol,L); or=cvt(or,L);
		}
	ol->type=I;
	return(qcat(psfx(ol,s),or));
}
binop5(ol,or,s)	/* bitwise & */
char *s;
Q *ol,*or;
{
	if(ol->type==N) ol=cvt(ol,L);
	if(or->type==N) or=cvt(or,L);
	if(ol->type!=F&&ol->type!=L) {
		if(or->type==F||or->type==L) ol->type=or->type;
		}
	return(qcat(psfx(ol,s),or));
}

binop6(ol,or,s)	/* : rules, as in ... ? ... : ... */
char *s;
Q *ol,*or;
{
	if(ol->type==F||or->type==F) {
		ol=cvt(ol,F); or=cvt(or,F);
		}
	if((ol->type==N||or->type==N)&&ol->type!=or->type) {
		ol=cvt(ol,L);
		or=cvt(or,L);
		}
	if(or->type==L) ol->type=L;
	if(or->type==U) ol->type=U;
	return(qcat(psfx(ol,s),or));
}

binop7(ol,or,s)	/* ? rules */
char *s;
Q *ol,*or;
{
	ol->type=or->type;
	return(qcat(psfx(ol,s),or));
}

binop8(ol,or,s)	/* = rules */
char *s;
Q *ol,*or;
{
int oltype;
	oltype=ol->type;
	return(qcat(psfx(ol,s),cvt(or,oltype)));
}

binop9(ol,or,s)	/* comma rules */
char *s;
Q *ol,*or;
{
	ol->type=or->type;
	return(qcat(psfx(ol,s),or));
}

binop10(ol,or,s)	/* logical or rules */
char *s;
Q *ol,*or;
{
	ol->type=I;
	return(qcat(psfx(ol,s),or));
}

qcat(ol,or)	/* cat two q's */
Q *ol,*or;
{
Q *ans;
int i,j,k;
	ans=getq(k=(j=ol->size)+or->size);
	ans->type=ol->type;
	ans->size=k;
	for(i=0; i<j; i++) ans->text[i]=ol->text[i];
	for(i=0; j<k; i++) ans->text[j++]=or->text[i];
	free(ol);
	free(or);
	return(ans);
}

bcat(s1,ol,s2,or,s3)	/* cat s's and o's */
char *s1,*s2,*s3;
Q *ol,*or;
{
	return(qcat(prfx(s1,psfx(ol,s2)),psfx(or,s3)));
}

/* parse an expression and return a Q pointer to it */

expr(pr) /* precedence that stuff must be below */
int pr;
{
int keynum;
int dummy;
Q *token,*op;
keynum=whatprf(); /* get operation index */
if(keynum==LP) {
	rmv(LP);
	if(op=declspec()) {
		dummy=0;
		op=decltor(op,&dummy);
                if(dummy)free(dummy);
		op->type %= DFMOD;
		skipbl();
		strip(")");
		token=expr(CASTP);
		if(op->type==F||op->type==N||token->type==F||token->type==N) {
			token=cvt(token,op->type);
			free(op);
			}
		else {
			token=cvt(token,op->type);
			token=bcat("(",op,")",token,"");
			}
		}
	else {
		token=expr(0);
		skipbl();
		strip(")");
		token=(prfx("(",psfx(token,")")));
		}
	}
else if(keynum==UNMINUS) {
	rmv(UNMINUS);
	token=(unminus(expr(UNMINUSP-1)));
	}
else if(keynum==UNPLUS) {
	rmv(UNPLUS);
	token=(unplus(expr(UNPLUSP-1)));
	}
else if(keynum==UNSTAR) {
	rmv(UNSTAR);
	token=(unstar(expr(UNSTARP-1)));
	}
else if(keynum==UNADDR) {
	rmv(UNADDR);
        token=(unaddr(expr(UNADDRP-1)));
	}
else if(keynum==UNNOT) {
	rmv(UNNOT);
	token=(unnot(expr(UNNOTP-1)));
	}
else if(keynum==UNCOM) {
	rmv(UNCOM);
	token=(uncom(expr(UNCOMP-1)));
	}
else if(keynum==UNINC1) {
	rmv(UNINC1);
	token=(uninc1(expr(UNINC1P-1)));
	}
else if(keynum==UNDEC1) {
	rmv(UNDEC1);
	token=(undec1(expr(UNDEC1P-1)));
	}
else if(keynum==SIZEOF) {
	strip("sizeof");
	if(look(skipbl())=='(') {
		gc();
		if((token=declspec())==0)token=expr(0);
		else {
			dummy=0;
			token=decltor(token,&dummy);
                        if(dummy)free(dummy);
			}
		token=prfx("sizeof(\377",psfx(token,")"));
		if(gc(skipbl())!=')')abt("expect ')' after sizeof(..");
		}
	else {
		token=prfx("sizeof \377",expr(SIZEOFP-1));
		}
	token->type=I;
	}

/* get a constant or variable name */

else if(keynum==CONST) {
	token=constant();
	}
else if(keynum==IDENT) {
	token=ident();
	}
else abt("expect expression, constant, or identifier");

/* do postfix op and binary ops until no more at precedence pr */

while(1) {
	keynum=whatpsf(); /* get unary postfix operator */

	if(keynum==UNINC2&&pr<UNINC2P) {
		rmv(UNINC2);
		token=uninc2(token);
		}
	else if(keynum==UNDEC2&&pr<UNDEC2P) {
		rmv(UNDEC2);
		token=undec2(token);
		}

	/* now try a binary operation */

	keynum=whatbin();
	
        if(keynum==SUBSCROP&&pr<SUBSCRP) {
		token=subscr(token);
		}
        else if(keynum==FCALLOP&&pr<FCALLOPP) {
		token=fcall(token);
		}
	else if(keynum==ADDOP&&pr<ADDOPP) {
		rmv(ADDOP);
		token=addop(token,expr(ADDOPP));
		}
	else if(keynum==SUBOP&&pr<SUBOPP) {
		rmv(SUBOP);
		token=subop(token,expr(SUBOPP));
		}
	else if(keynum==MULOP&&pr<MULOPP) {
		rmv(MULOP);
		token=mulop(token,expr(MULOPP));
		}
	else if(keynum==DIVOP&&pr<DIVOPP) {
		rmv(DIVOP);
		token=divop(token,expr(DIVOPP));
		}
	else if(keynum==GTOP&&pr<GTOPP) {
		rmv(GTOP);
		token=gtop(token,expr(GTOPP));
		}
	else if(keynum==GEOP&&pr<GEOPP) {
		rmv(GEOP);
		token=geop(token,expr(GEOPP));
		}
	else if(keynum==LEOP&&pr<LEOPP) {
		rmv(LEOP);
		token=leop(token,expr(LEOPP));
		}
	else if(keynum==LTOP&&pr<LTOPP) {
		rmv(LTOP);
		token=ltop(token,expr(LTOPP));
		}
	else if(keynum==EQOP&&pr<EQOPP) {
		rmv(EQOP);
		token=eqop(token,expr(EQOPP));
		}
	else if(keynum==NEOP&&pr<NEOPP) {
		rmv(NEOP);
		token=neop(token,expr(NEOPP));
		}
	else if(keynum==PUTOP&&pr<PUTOPP) {
		rmv(PUTOP);
		token=putop(token,expr(PUTOPP-1));
		}
	else if(keynum==OROP&&pr<OROPP) {
		rmv(OROP);
		token=orop(token,expr(OROPP));
		}
	else if(keynum==ANDOP&&pr<ANDOPP) {
		rmv(ANDOP);
		token=andop(token,expr(ANDOPP));
		}
	else if(keynum==MODOP&&pr<MODOPP) {
		rmv(MODOP);
		token=modop(token,expr(MODOPP));
		}
	else if(keynum==DOTOP&&pr<DOTOPP) {
		rmv(DOTOP);
		dotid=1; /* set id flag for alternate symbol table */
		token=dotop(token,expr(DOTOPP));
		}
	else if(keynum==PTROP&&pr<PTROPP) {
		rmv(PTROP);
		dotid=1; /* set id flag for alt s t */
		token=ptrop(token,expr(PTROPP));
		}
	else if(keynum==SHLOP&&pr<SHLOPP) {
		rmv(SHLOP);
		token=shlop(token,expr(SHLOPP));
		}
	else if(keynum==SHROP&&pr<SHROPP) {
		rmv(SHROP);
		token=shrop(token,expr(SHROPP));
		}
	else if(keynum==BITAND&&pr<BITANDP) {
		rmv(BITAND);
		token=bitand(token,expr(BITANDP));
		}
	else if(keynum==BITIOR&&pr<BITIORP) {
		rmv(BITIOR);
		token=bitior(token,expr(BITIORP));
		}
	else if(keynum==BITEOR&&pr<BITEORP) {
		rmv(BITEOR);
		token=biteor(token,expr(BITEORP));
		}
	else if(keynum==QSTOP&&pr<QSTOPP) {
		rmv(QSTOP);
		op=expr(QSTOPP2); /* need r to l grouping */
		if(gc(skipbl())!=':') abt("want ':' after '?'");
		token=questop(token,altop(op,expr(QSTOPP2)));
		}
	else if(keynum==APLUSOP&&pr<APLUSOPP) {
		rmv(APLUSOP);
		token=assignop(token,expr(APLUSOPP),"+= ");
		}
	else if(keynum==ASUBOP&&pr<ASUBOPP) {
		rmv(ASUBOP);
		token=assignop(token,expr(ASUBOPP),"-= ");
		}
	else if(keynum==AMULOP&&pr<AMULOPP) {
		rmv(AMULOP);
		token=assignop(token,expr(AMULOPP),"*= ");
		}
	else if(keynum==ADIVOP&&pr<ADIVOPP) {
		rmv(ADIVOP);
		token=assignop(token,expr(ADIVOPP),"/= ");
		}
	else if(keynum==AMODOP&&pr<AMODOPP) {
		rmv(AMODOP);
		token=assignop(token,expr(AMODOPP),"%= ");
		}
	else if(keynum==ASHLOP&&pr<ASHLOPP) {
		rmv(ASHLOP);
		token=assignop(token,expr(ASHLOPP),"<<= ");
		}
	else if(keynum==ASHROP&&pr<ASHROPP) {
		rmv(ASHROP);
		token=assignop(token,expr(ASHROPP),">>= ");
		}
	else if(keynum==AANDOP&&pr<AANDOPP) {
		rmv(AANDOP);
		token=assignop(token,expr(AANDOPP),"&= ");
		}
	else if(keynum==AIOROP&&pr<AIOROPP) {
		rmv(AIOROP);
		token=assignop(token,expr(AIOROPP),"|= ");
		}
	else if(keynum==AEOROP&&pr<AEOROPP) {
		rmv(AEOROP);
		token=assignop(token,expr(AEOROPP),"^= ");
		}
	else if(keynum==COMMAOP&&pr<COMMAOPP) {
		rmv(COMMAOP);
		token=commaop(token,expr(COMMAOPP));
		}
	else return(token);
	}
}

fcall(op)	/* return a function call */
Q *op;
{
	strip("(");
	op=psfx(op,"(\377");
	if((op->type=op->type-MAXTYPE)<0)warn("illegal function call");
	skipbl();
        while(!strip(")")) {
		op=qcat(op,expr(COMMAOPP));
		if(strip(","))op=psfx(op,",\377");
		}
	return(psfx(op,")"));
}

alphanum()	/* return a name string */
{
char *s;
int i,c;
	if((s=alloc(NAMELEN+1))==OUT)abt("out of space in 'alphanum'");
	i=0;
	do {
		c=gc();
		if((c>='a'&&c<='z')||
			(c>='0'&&c<='9')||
			(c>='A'&&c<='Z')||
			c=='_') {
			s[i++]=c;
			}
		else {
			ug(c);
			break;
			}
		} while(i<NAMELEN);
	s[i]=0;
	if(*s==0) {
		free(s);
		return(0);
		}
	return(s);
}

ugs(s)	/* unget a string */
char *s;
{
int i;
	for(i=0; s[i]; i++);
	while(i>0)ug(s[--i]);
	free(s);
}

digits()	/* strip a digit string */
{
char *s;
int i,c;
        if((s=alloc(NUMBLEN+1))==OUT)abt("out of space in 'digits'");
        i=0;
        do {
                c=gc();
		if(c<='9'&&c>='0') {
                        s[i++]=c;
                        }
                else {
                        ug(c);
                        break;
                        }
                } while(i<NUMBLEN);
        s[i]=0;
        if(*s==0) {
                free(s);
                return(0);
                }
        return(s);
}

anyof(s)	/* return a string of s's */
char *s;
{
int c,i,j;
char *ans;
	if((ans=alloc(STRLEN+1))==OUT)abt("out of room in 'anyof'");
	i=0;
	do {
		c=gc();
		for(j=0; s[j]; j++) if(c==s[j])break;
		if(s[j]==0) {
			ug(c);
			break;
			}
		ans[i++]=c;
		} while(i<STRLEN);
	ans[i]=0;
	if(*ans==0) {
		free(ans);
		return(0);
		}
	return(ans);
}

ident()	/* return a q-object identifier */
{
char *s;
Q *op;
int i;
	skipbl();
	if((s=alphanum())==0)abt("expect identifier");
	for(i=0; s[i]; i++);
	op=getq(i);
	op->size=i;
	for(i=0; s[i]; i++)op->text[i]=s[i];
	if(dotid) {
		*s |= 0200;
		dotid=0;
		}
	op->type=gettype(s);
	free(s);
	return(op);
}

subscr(op)	/* return added subscripting */
Q *op;
{
Q *or;
	skipbl();
	while(look()=='[') {
		gc();
		or=expr(0);
		if(op->type<MAXTYPE&&or->type>=MAXTYPE) {
			op=cvt(op,I);
			op->type=or->type;
			}
		else or=cvt(or,I);
		op=bcat("",op,"[\377",or,"]");
		if((op->type=op->type-MAXTYPE)<0)warn("too many subscripts");
		skipbl();
		if(gc()!=']')abt("expect close subscript");
		skipbl();
		}
	return(op);
}

look()	/* get and put back a character */
{
int c;
	ug(c=gc());
	return(c);
}

constant()	/* return a Q constant */
{
Q *op;
	skipbl();
	if(look()=='0') {
		gc();
		if(look()=='x'||look()=='X') {
			gc();
			op=hex();
			}
		else if(look()=='.') op=decimal();
		else op=octal();
		}
	else if(look()=='\'') op=charc();
	else if(look()=='"') op=stringc();
	else op=decimal();
	return(op);
}

octal()	/* return octal constant */
{
Q *op;
char *s;
long num;
int i;
        if((s=digits())==0) {
		op=getq(1);
		op->type=I;
		op->size=1;
		op->text[0]='0';
		if(look()=='l'||look()=='L') {
			gc();
			op=psfx(op,"L");
			op->type=L;
			}
		return(op);
		}
	for(num=i=0; s[i]; i++)num=num*8+s[i]-'0';
	op=getq(i);
	op->size=i;
	if(num>32767)op->type=L; else op->type=I;
	for(i=0; i<op->size; i++) op->text[i]=s[i];
	free(s);
	if(look()=='l'||look()=='L') {
		gc();
		op->type=L;
		op=psfx(op,"L");
		}
	return(prfx("0",op));
}

hex()	/* return hex constant */
{
Q *op;
char *s;
long num;
int i;
        while((s=anyof("0123456789ABCDEFabcdef"))==0) ug('0');
	for(num=i=0; s[i]; i++) {
		if(s[i]>='a'&&s[i]<='f')num=num*16+s[i]-'a'+10;
		else if(s[i]>='A'&&s[i]<='F')num=num*16+s[i]-'A'+10;
		else num=num*16+s[i]-'0';
		}
        op=getq(i);
        op->size=i;
        if(num>32767)op->type=L; else op->type=I;
        for(i=0; i<op->size; i++) op->text[i]=s[i];
        free(s);
        if(look()=='l'||look()=='L') {
                gc();
                op->type=L;
                op=psfx(op,"L");
                }
	return(prfx("0X",op));
}

decimal()	/* return decimal number */
{
Q *op;
char *s;
long num;
int i;
	if((s=digits())==0) {
		return(floatc());
		}
	if(look()=='.'||look()=='e'||look()=='E') {
		ugs(s);
		return(floatc());
		}
        for(num=i=0; s[i]; i++)num=num*10+s[i]-'0';
        op=getq(i);
        op->size=i;
        if(num>32767)op->type=L; else op->type=I;
        for(i=0; i<op->size; i++) op->text[i]=s[i];
        free(s);
        if(look()=='l'||look()=='L') {
                gc();
                op->type=L;
                op=psfx(op,"L");
                }
	return(op);
}

floatc()	/* return a float constant */
{
Q *op;
/**********this code redone avoiding floating point below.....
int i,c;
double fnum,xp;
long lnum;
	i=0;
	fnum=0.0;
	xp=1.0;
	while((c=gc())<='9'&&c>='0')fnum=fnum*10.0+c-'0';
	if(c=='.') {
		while((c=gc())<='9'&&c>='0')fnum=fnum+(c-'0')/(xp=xp*10.0);
		}
	if(c=='e'||c=='E') {
		if((c=gc())=='-') {
			while((c=gc())<='9'&&c>='0')i=i*10+c-'0';
			while(--i>=0)fnum=fnum/10.0;
			}
		else {
			ug(c);
			while((c=gc())<='9'&&c>='0')i=i*10+c-'0';
			while(--i>=0)fnum=fnum*10.0;
			}
		}
********this code replaced by below  */
#define FWH 256
#define FW (FWH*2)
#define DEC (c=gc())<='9'&&c>='0'
char q[FW+1];
int i,j,k,c;
long fnum;
long lnum;
	i=0; /* tens exponent */
	k=0; /* twos exponent */
	for(j=0; j<FW; j++)q[j]=0; /* set bit array to zero */
	while(DEC)multenadd(q,c-'0'); /* mul by 10 and add character */
	if(c=='.') {
		while(DEC) {
			multenadd(q,c-'0');
			i--; /* decr tens exponent */
			}
		}
	if(c=='e'||c=='E') {
		if((c=gc())=='-') {
			for(j=0; DEC; j=j*10+c-'0');
			i -= j;
			}
		else {
			ug(c);
			for(j=0; DEC; j=j*10+c-'0');
			i += j;
			}
		}
	if(i>0)while(i--)multenadd(q,0);
	else if(i<0)while(i++)divten(q,&k);
	for(j=FW-1; j>23; --j)if(q[j])break; /* find leading bit */
	i=j+k+(0201-FWH); /* exponent for fltg point number */
	if(i>0377)i=0377;
	if(i<0)i=0;
	fnum=i;
	fnum <<= 31-8;
	if(i) {
		for(k=31-9; --j&&k>=0; --k) {
			if(q[j])fnum |= 1l<<k;
			}
		}
/* **** end of replacement code, number now in fnum **** */
	ug(c);
	dblflt(&lnum,&fnum); /* copy two hi words */
        op=getq(13); /*0 00000000000 L*/
	op->size=13;
	op->type=F;
	i=13;
	op->text[--i]='L';
	while(i) {
		c=lnum;
		op->text[--i]=(c&7)+'0';
		lnum=lnum>>3;
		lnum=lnum&03777777777l;
		}
	return(op);
}

/* mul by 10 and div by 10 for replacement code above */
multenadd(q,ad)
int ad;
char *q;
{
int j;
	for(j=0; j<FW; j++)q[j] *= 10;
	q[FWH] += ad;
	for(j=0; j<FW; j++)
		while(q[j]>1) {
			q[j] -= 2;
			q[j+1]++;
			}
}
divten(q,xc)
int *xc;
char *q;
{
int i,j;
	for(j=FW; j>=4; --j) {
		i=q[j-1]*8+q[j-2]*4+q[j-3]*2+q[j-4];
		if(i>=5) {
			q[j-1]=1;
			i -= 5;
			q[j-2]=i>=4;
			q[j-3]=i%4>=2;
			q[j-4]=i%2;
			}
		}
	*xc -= 4;
}
/**** end of mul and div routines ****/
dblflt(lnump,fnump) /* reorder float words into a long */
int *lnump; /* called with long */
int *fnump; /* called with double */
{
if(unixfmt) {
	lnump[0]=fnump[0];
	lnump[1]=fnump[1];
	}
else {
	lnump[1]=fnump[0];
	lnump[0]=fnump[1];
	}
}

charc()	/* return a character constant */
{
Q *op;
int i;
int e;
	op=getq(CHCONST);
	for(e=1,i=0; i<CHCONST; i++) {
                if((op->text[i]=gc2())==0)abt("missing end \"'\"");
		if(e)e=0;
		else if(op->text[i]=='\\')e=1;
		else if(op->text[i]=='\'')break;
		}
	if(i==CHCONST)abt("character const too long");
	op->type=I;
	op->size=i+1;;
	return(op);
}

stringc()	/* return a string constant */
{
Q *op;
static char s[2]; /* ensure trailing null */
int e;
	op=getq(1);
        op->text[0]=gc();
	op->type=I+MAXTYPE;
	op->size=1;
	e=0;
	while(1) {
		if((*s=gc2())==0)abt("missing end '\"'");
		op=psfx(op,s);
		if(e)e=0;
		else if(*s=='\\')e=1;
		else if(*s=='"')break;
		}
	return(op);
}

skipbl()	/* skip 'blanks' */
{
int c;
	while((c=gc())==' '||c=='\n'||c=='\t');
	ug(c);
}

whatprf()	/* return keyword of unary-prefix operation/thing */
{
int r;
char *s;
	skipbl();
	r=0;
        if(s=anyof("*&-!~+(.")) {
		if(*s=='*')		r=UNSTAR;
		else if(*s=='&')	r=UNADDR;
		else if(*s=='-') {
			if(s[1]=='-')	r=UNDEC1;
			else		r=UNMINUS;
			}
		else if(*s=='.')	r=CONST;
		else if(*s=='!')	r=UNNOT;
		else if(*s=='~')	r=UNCOM;
		else if(*s=='+') {
			if(s[1]=='+')	r=UNINC1;
			else		r=UNPLUS;
			}
		else if(*s=='(')	r=LP;
		ugs(s);
		if(r)return(r);
		}
	if(look()=='"'||look()=='\'')return(CONST);
	if(s=alphanum()) {
		if(*s>='0'&&*s<='9') {
			ugs(s);
			return(CONST);
			}
		if(seq(s,"sizeof")) {
			ugs(s);
			return(SIZEOF);
			}
		ugs(s);
		return(IDENT);
		}
	abt("expect unary operator or identifier or constant");
}

whatpsf()	/* return postfix unary operator */
{
char *s;
int r;
	skipbl();
	r=0;
	if(s=anyof("+-")) {
                if(*s=='+'&&s[1]=='+')			r=UNINC2;
                else if(*s=='-'&&s[1]=='-')		r=UNDEC2;
		ugs(s);
		}
	return(r);
}

whatbin()	/* return binary operator */
{
char *s;
int r;
	skipbl();
	r=0;
        if(s=anyof("*/%+-<>=!&^|?,.([")) {
		if(*s=='(') 			r=FCALLOP;
		else if(*s=='[')		r=SUBSCROP;
		else if(*s=='-') {
			if(s[1]=='>')		r=PTROP;
			else if(s[1]=='=')	r=ASUBOP;
			else			r=SUBOP;
			}
		else if(*s=='/') {
			if(s[1]=='=')		r=ADIVOP;
			else			r=DIVOP;
			}
		else if(*s=='*') {
			if(s[1]=='=')		r=AMULOP;
			else			r=MULOP;
			}
		else if(*s=='.')		r=DOTOP;
		else if(*s=='%') {
			if(s[1]=='=')		r=AMODOP;
			else			r=MODOP;
			}
		else if(*s=='+') {
			if(s[1]=='=')		r=APLUSOP;
			else			r=ADDOP;
			}
		else if(*s=='<') {
			if(s[1]=='<') {
				if(s[2]=='=')	r=ASHLOP;
				else		r=SHLOP;
				}
			else if(s[1]=='=')	r=LEOP;
			else			r=LTOP;
			}
		else if(*s=='>') {
			if(s[1]=='>') {
				if(s[2]=='=')	r=ASHROP;
				else		r=SHROP;
				}
			else if(s[1]=='=')	r=GEOP;
			else			r=GTOP;
			}
		else if(*s=='=') {
			if(s[1]=='=')		r=EQOP;
			else if(s[1]=='>'&&s[2]=='>')
						r=ASHROP;
			else if(s[1]=='<'&&s[2]=='<')
						r=ASHLOP;
			else if(s[1]=='+')	r=APLUSOP;
			else if(s[1]=='-')	r=ASUBOP;
			else if(s[1]=='*')	r=AMULOP;
			else if(s[1]=='/')	r=ADIVOP;
			else if(s[1]=='%')	r=AMODOP;
			else if(s[1]=='&')	r=AANDOP;
			else if(s[1]=='^')	r=AEOROP;
			else if(s[1]=='|')	r=AIOROP;
			else 			r=PUTOP;
			}
                else if(*s=='!'&&s[1]=='=')	r=NEOP;
		else if(*s=='&') {
			if(s[1]=='&')		r=ANDOP;
			else if(s[1]=='=')	r=AANDOP;
			else			r=BITAND;
			}
		else if(*s=='^') {
			if(s[1]=='=')		r=AEOROP;
			else 			r=BITEOR;
			}
		else if(*s=='|') {
			if(s[1]=='|')		r=OROP;
			else if(s[1]=='=')	r=AIOROP;
			else			r=BITIOR;
			}
		else if(*s=='?')		r=QSTOP;
		else if(*s==',')		r=COMMAOP;
		ugs(s);
		}
	return(r);
}

rmv(r)	/* delete the r-type found above */
int r;
{
		if(r==UNSTAR||r==UNADDR||r==UNMINUS||
			r==UNNOT||r==UNCOM||r==UNPLUS||r==LP) {
				gc();
				}
		else if(r==UNDEC1||r==UNDEC2||r==UNINC1||r==UNINC2) {
				gc();
				gc();
				}
		else if(r==SUBOP||r==DIVOP||r==MODOP||r==ADDOP||r==LTOP||
			r==GTOP||r==PUTOP||r==BITAND||r==BITEOR||
                        r==BITIOR||r==QSTOP||
			r==MULOP||r==COMMAOP||r==DOTOP) {
				gc();
				}
		else if(r==PTROP||r==ASUBOP||r==ADIVOP||r==AMODOP||
			r==APLUSOP||r==SHLOP||r==LEOP||r==SHROP||
			r==GEOP||r==EQOP||r==AANDOP||r==AEOROP||
                        r==AIOROP||r==NEOP||r==ANDOP||r==OROP||
			r==AMULOP) {
				gc();
				gc();
				}
		else if(r==ASHLOP||r==ASHROP) {
				gc();
				gc();
				gc();
				}
}

strip(s)	/* remove a string */
char *s;
{
	while(*s++==look())gc();
	return(!*--s);
}
declspec()
{
Q *q;
int c;
char *s;
if(s=alphanum(skipbl())) {
	q=getq(0);
	q->size=0;
	q->type=0;
	q=sclass(q,s);
	if(q->size) {
		free(s);
		s=alphanum(skipbl());
		}
	if(s&&((c=gettype(s))&DFTYPEDF)) {
		if(c&DFFUNCT2)c=(c^DFFUNCT2)|DFFUNCT;
                q->type |= c^DFTYPEDF;
		q=psfx(psfx(q,s)," ");
		skipbl();
		free(s);
		return(q);
		}
	while(s) {
		if(seq(s,"char")) (q=psfx(q,"char "))->type |= CHAR;
		else if(seq(s,"short")) (q=psfx(q,"short "))->type |= SHORT;
		else if(seq(s,"int")) (q=psfx(q,"int "))->type |= INT;
		else if(seq(s,"long")) (q=psfx(q,"long "))->type |= LONG;
		else if(seq(s,"unsigned"))(q=psfx(q,"unsigned "))->type |=
								UNSIGNED;
		else if(seq(s,"float")) (q=psfx(q,"long "))->type |= FLOAT;
		else if(seq(s,"double")) (q=psfx(q,"long "))->type |= DOUBLE;
		else if(seq(s,"struct")) (q=psfx(q,"struct "))->type |= STRUCT;
		else if(seq(s,"union")) (q=psfx(q,"union "))->type |= UNION;
		else if(seq(s,"floatn")) (q=psfx(q,"long "))->type |= FLOATN;
		else if(seq(s,"intn")) (q=psfx(q,"long "))->type |= INTN;

/* can sc specifier appear anywhere but first? it appears that "C" 
   accepts it elsewhere, unlike the specification. skip it if found
   here. */

		else {
			if(seq(s,"auto"))q=psfx(q,"auto ");
			else if(seq(s,"static"))q=psfx(q,"static ");
			else if(seq(s,"extern"))q=psfx(q,"extern ");
			else if(seq(s,"register"))q=psfx(q,"register ");
			else if(seq(s,"typedef"))q=psfx(q,"typedef "); /*cmon*/
			else break;
			warn("misplaced sc specifier");
			}
		free(s);
		s=alphanum(skipbl());
		}
        if(s) ugs(s);
	if(q->size) {
		c=q->type;
                if(c&(CHAR|SHORT))q->type=I;
		else if(c&(FLOAT|FLOATN|DOUBLE))q->type=F;
		else if(c&(LONG))q->type=L;
		else if(c&(INTN))q->type=N;
		else if(c&(UNION|STRUCT))q->type=DFSTRUCT|FFSTRUCT;
		else if(c&(UNSIGNED))q->type=U;
		else q->type=I;
		if(c&TYPEDF)q->type |= DFTYPEDF;
		return(q);
		}
	free(q);
	}
return(0);
}

sclass(q,s)
Q *q;
char *s;
{
	if(seq(s,"auto"))return(psfx(q,"auto "));
	if(seq(s,"static"))return(psfx(q,"static "));
	if(seq(s,"extern"))return(psfx(q,"extern "));
	if(seq(s,"register"))return(psfx(q,"register "));
	if(seq(s,"typedef")) {
		q->type |= TYPEDF;
		return(psfx(q,"typedef "));
		}
	return(q);
}

/* parse a decraration list */
declist(op)
Q *op;
{
Q *q;
char *s;
	while(look(skipbl())!=';') {
		s=0;
		q=getq(0);
		q->size=0;
		q->type=op->type;
		q=decltor(q,&s);
		if((q->type&DFTYPEDF)&&(q->type&DFFUNCT))q->type |= DFFUNCT2;
		if(s) {
			if(instruct)*s |= 0200;
			definesy(s,lasttype=q->type%DFMOD);
			free(s);
			}
		else if(q->size==0)abt("can't parse declarator list");
		if(look(skipbl())!=';'&&look()!=','&&(q->type&DFFUNCT)==0) {
			if(look()=='=') {
				gc();
				q=psfx(q," =\377");
				}
			q=initdcl(q);
			}
		if(look(skipbl())==',') {
			gc();
			op=qcat(op,psfx(q,",\377"));
			}
		else {
			op=qcat(op,q);
			break;
			}
		}
	return(op);
}

/* parse a declarator eg.  **s[3][2] */
/*	This is messy. If sp is zero, the first alphanumeric encountered
	is stripped out and put into it (the symbol being declared). When
	this is done, DFLASTID is set. If a "(" is parsed with DFLASTID set,
	the identifier is a function, and DFFUNCT is set. This is used by
	caller to determine that an initialization is not allowed. If a
	"*" is encountered, DFLASTID is cleared. Thus,

		(*f)() {0

	cannot be the start of a function definition, but is a declaration
	of f as pointer to a function, which pointer is initialized to 0, and

		*f() {0

	is the start of a new function (there is no variable to initialize).

	The flag "skiptext" informs decltor that it is inside at least one
	"(", and must eat all text until balancing ")"'s are encountered
	(otherwise it stops at commas, for example). If, while skiptext is
	set, with DFFUNCT also set (and level==0, though it shouldn't matter),
	some more alphanumeric symbols are found, they are prototype arguments.
	Stash them in "argchain" for later definition (at higher level) as
	"int" in case there is no declaration for them.
*/
decltor(q,sp)	/* parse a declarator, return symbol in *sp,type in q */
Q *q;
char **sp;
{
char *s;
struct argchain *temp;
static int skiptext;
while(1) {
	if(look(skipbl())=='(') {
		skiptext++;
		gc();
                if(q->type&DFLASTID) {
			q->type += MAXTYPE;
			if(q->type&DFLASTID) q->type |= DFFUNCT;
			q->type &= ~DFLASTID;
			}
		else if(look(skipbl())==')')q->type += MAXTYPE;
		q=decltor(psfx(q,"("),sp);
		while(look(skipbl())==',') {
			gc();
			q=decltor(psfx(q,",\377"),sp);
			}
		if(gc()!=')')abt("expect ')' in declarator");
		skiptext--;
		q=psfx(q,")\377");
		}
	else if(look()=='*') {
		gc();
		q=decltor(psfx(q," *"),sp);
		q->type &= ~DFLASTID;
		q->type += MAXTYPE;
		}
	else if(look()=='[') {
		gc();
		q->type += MAXTYPE;
		if(look(skipbl())!=']')q=bcat("",q,"[",expr(0),"]");
		else q=psfx(q,"[]");
		if(gc(skipbl())!=']')abt("expect ']' in subscript");
		}
	else if((*sp==0||skiptext)&&(s=alphanum())) {
		q=psfx(psfx(q,"\377 "),s);
		if(*sp==0) {
			*sp=s;
			q->type |= DFLASTID;
			}
		/* A late adddition: if we are inside the argument list
		   of a function prototype, form a list of the arguments
		   chained off of "arglist" to define as integer type in
		   case not declared by explicit declaration. Check of level
		   should firewall the change's effects pretty well */

		else if(level==0&&skiptext&&(q->type&DFFUNCT)) {
			temp=argchain;
			(argchain=getq(0))->acnext=temp;
			argchain->acstring=s;
			}

		else free(s);
		}
	else if(look()==':') {
		gc();
		q=qcat(psfx(q,":"),expr(COMMAOPP));
		}
	else break;
	}
return(q);
}

/* do initialization */
initdcl(op)
Q *op;
{
	if(look(skipbl())=='{') {
		gc();
		op=psfx(op," \377{");
		while(look(skipbl())!=';'&&look()!='}') {
			op=initdcl(op);
			if(look(skipbl())==',') {
				gc();
					op=psfx(op,",\377");
				}
			}
		op=psfx(op,"}\377");
		if(gc(skipbl())!='}')abt("expect '}' in init");
		}
	else op=qcat(op,prfx(" \377",expr(COMMAOPP)));
return(op);
}

/* parse and output a statement */
statement()
{
Q *op;
int c;
char *s;
if(look(skipbl())=='{') {
	gc();
	definelv(++level);
	op=getqs("\n{\n"); outq(op); free(op);
	while(op=declarat()) {
		outq(op);
		free(op);
		}
	while(statement());
	if(gc(skipbl())!='}')abt("expect '}' for end of block");
	op=getqs("\n}\n"); outq(op); free(op);
	definelv(--level);
	return(1);
	}
else if(look()==';') {
	gc();
	op=getqs(";\n"); outq(op); free(op);
	return(1);
	}
else if(look()=='}')return(0);
else if(s=alphanum()) {
	if(seq(s,"if")) {	/* do if() [else] */
		free(s);
		if(gc(skipbl())!='(')abt("expect '(' after 'if'");
		op=getqs("if(\377");
                if(look(skipbl())!=')') op=qcat(op,expr(0));
		op=psfx(op,")\377");
		if(gc(skipbl())!=')') abt("expect ')' after 'if(...'");
		outq(op);
		free(op);
		statement();
		if(s=alphanum(skipbl())) {
			if(seq(s,"else")) {
				free(s);
				op=getqs("else \377");
				outq(op);
				free(op);
				statement();
				}
			else ugs(s);
			}
		return(1);
		}
	else if(seq(s,"while")) {
		free(s);
		if(gc(skipbl())!='(')abt("expect '(' after 'while'");
		op=getqs("while(\377");
                if(look(skipbl())!=')')op=qcat(op,expr(0));
		op=psfx(op,")\377");
		if(gc(skipbl())!=')') abt("expect ')' after 'while(...'");
		outq(op);
		free(op);
		statement();
		return(1);
		}
	else if(seq(s,"do")) {
		free(s);
		op=getqs("do \377");
		outq(op);
		free(op);
		statement();
		if((s=alphanum(skipbl()))&&seq(s,"while")) {
			free(s);
			if(gc(skipbl())!='(')
				abt("expect '(' after 'do ... while'");
			else op=getqs("\377while(\377");
			if(look(skipbl())!=')') {
				op=qcat(op,expr(0));
				}
			if(gc(skipbl())!=')')
				abt("expect ')' after 'do ... while('");
			if(gc(skipbl())!=';')
				abt("expect ';' after 'do  while()'");
			op=psfx(op,");\n");
			outq(op);
			free(op);
			return(1);
			}
		else abt("expect 'while' after 'do'");
		}
	else if(seq(s,"for")) {
		free(s);
		if(gc(skipbl())!='(') abt("expect '(' after 'for'");
		op=getqs("for(\377");
		if(look(skipbl())!=';') op=qcat(op,expr(0));
		if(gc(skipbl())!=';') abt("expect ';' after 'for( ...'");
		op=psfx(op,";\377");
		if(look(skipbl())!=';') op=qcat(op,expr(0));
		if(gc(skipbl())!=';') abt("expect ';' after 'for(...;...'");
		op=psfx(op,";\377");
		if(look(skipbl())!=')') op=qcat(op,expr(0));
		if(gc(skipbl())!=')') abt("expect ')' after 'for(..;..;..'");
		op=psfx(op,") \377");
		outq(op);
		free(op);
		statement();
		return(1);
		}
        else if(seq(s,"switch")) {
                free(s);
                if(gc(skipbl())!='(') abt("expect '(' after 'switch'");
                op=prfx("switch(\377",expr(0));
		if(gc(skipbl())!=')')abt("expect ')' after 'switch(...'");
		op=psfx(op,")\n");
		outq(op);
		free(op);
		return(statement());
		}
	else if(seq(s,"case")) {
		free(s);
		op=prfx("\377case ",expr(0));
		if(gc(skipbl())!=':') abt("expect ':' after 'case'");
		op=psfx(op,":\377");
		outq(op);
		free(op);
		return(statement());
		}
	else if(seq(s,"default")) {
		free(s);
                if(gc(skipbl())!=':') abt("expect ':' after 'default'");
		op=getqs("default:\377");
		outq(op);
		free(op);
		return(statement());
		}
	else if(seq(s,"break")) {
		free(s);
                if(gc(skipbl())!=';') abt("expect ';' after 'break'");
		op=getqs("break;\n");
		outq(op);
		free(op);
		return(1);
		}
	else if(seq(s,"continue")) {
		free(s);
		if(gc(skipbl())!=';') abt("expect ';' after 'continue'");
		op=getqs("continue;\n");
		outq(op);
		free(op);
		return(1);
		}
	else if(seq(s,"return")) {
		free(s);
		op=getqs("return ");
		if(look(skipbl())!=';')op=qcat(op,cvt(expr(0),frettype));
		if(gc(skipbl())!=';') abt("expect ';' after 'return ... '");
		op=psfx(op,";\n");
		outq(op);
		free(op);
		return(1);
		}
	else if(seq(s,"goto")) {
		free(s);
		if((s=alphanum(skipbl()))==0) abt("goto where?");
		op=psfx(getqs("goto "),s);	/*god*/
		if(gc(skipbl())!=';') abt("expect ';' after 'goto ...'");
		op=psfx(op,";\n");
		outq(op);
		free(op);
		return(1);
		}
	else if((c=look())==':'||look(skipbl())==':') {
		op=psfx(getqs(s),":\377");
		gc();
		free(s);
		outq(op);
		free(op);
		statement();
		return(1);
		}
	else {
		if(c!=look()) ug(c);
		ugs(s);
		}
	}
/* must be an expression statement! */
	{
	op=psfx(expr(0),";\n");
	outq(op);
	free(op);
	if(gc(skipbl())!=';')abt("expect ';' after expression");
	return(1);
	}
}

/* do a declaration statement */
declarat()
{
Q *op;
if(op=declspec()) {
	if((op->type&FFSTRUCT)) op=declstru(op);
	else op=declist(op);
	if(gc(skipbl())!=';')abt("expect ';' after declaration");
	return(psfx(op,";\n"));
	}
else return(0);
}

/* do a struct or union statement */
declstru(op)
Q *op;
{
char *s;
Q *q;
	s=alphanum(skipbl());
	if(look(skipbl())=='{') {
		gc();
		if(s) {
			op=psfx(op," ");
			op=psfx(op,s);
			free(s);
			}
		op=psfx(op," {\n");
		instruct++; /* alternate symbol table for defns */
		while(look(skipbl())!='}') {
			if(q=declarat()) {
				op=qcat(op,prfx(" ",q));
				}
			else if(look()==';') gc();
			else abt("bad statement inside struct");
			}
		instruct--; /* out of struct */
		op=psfx(op,"} \377");
		gc();
		}
	else if(s) {
		op=psfx(op," ");
		op=psfx(op,s);
		op=psfx(op," ");
		free(s);
		}
	op=declist(op);
	return(op);
}

file()		/* do a program file */
{
Q *op;
int c;
op=getqs("extern long _funmin(),_funplus(),_finc1(),_finc2(),\n");
outq(op); free(op);
op=getqs("_fdec1(),_fdec2(),_ninc1(),_ninc2(),_ndec1(),_ndec2(),\n");
outq(op); free(op);
op=getqs("_fmul(),_fdiv(),_fmod(),_fadd(),_fsub(),\n");
outq(op); free(op);
op=getqs("_ffrl(),_ffrn(),_ftol(),_ntol(),_fton(),\n");
outq(op); free(op);
op=getqs("_lton(),_iton(),_ffri(),_uton(),_ffru();\n");
outq(op); free(op);
op=getqs("extern int _ftoi(),_ntoi(),_fgt(),_fge(),_fle(),_flt();\n");
outq(op); free(op);
if(unsign==1) {
	op=getqs("extern unsigned _ftou(),_ntou();\n");
	outq(op); free(op);
	}
op=getqs("extern long _feqpl(),_neqpl(),_feqmi(),_neqmi(),\n");
outq(op); free(op);
op=getqs("_feqmu(),_neqmu(),_feqdv(),_neqdv(),_feqan(),_neqan(),\n");
outq(op); free(op);
op=getqs("_feqer(),_neqer(),_feqmo(),_neqmo(),_feqor(),_neqor(),\n");
outq(op); free(op);
op=getqs("_feqsr(),_neqsr(),_feqsl(),_neqsl();\n");
outq(op); free(op);
while(look(skipbl())) {
	while(1){
		if(look(skipbl())==0) return;
		if((op=declspec())==0) {
			op=getq(0);
			op->type=I;
			op->size=0;
			}
		if(op->type&FFSTRUCT) op=declstru(op);
		else op=declist(op);
		if((c=gc(skipbl()))!=';')break;
		op=psfx(op,";\n");
		outq(op);
		free(op);
		}
	frettype=(lasttype-MAXTYPE)%DFMOD;
	if(frettype<0)warn("illegal function type");
	op=psfx(op,"\n");
	outq(op);
	free(op);
	ug(c);
	definelv(++level);
	while(op=declarat()) {
		outq(op);
		free(op);
		}
/* now define all the prototype args as "int". symbol table takes first
   definition, so declaration preceeding will override. */
	while(argchain) {
		definesy(argchain->acstring,I);
		free(argchain->acstring);
		argchain=argchain->acnext;
		}
	if(look(skipbl())=='{') statement();
	else abt("expect '{' after function prototype");
	definelv(--level);
	}
}

/*
 *  mjm: own alloc() which calls malloc()
 */
static alloc(sz)
int sz;
{	/* mjm: made malloc arg always even */
if((sz=malloc((sz+1)&~01)) != 0)
	return(sz);
else return(OUT);
}
