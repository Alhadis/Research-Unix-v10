dcls1:	  dcl1
	| dcls1 EOS
	| dcls1 EOS dcl1
		{ $$ = (int *)hookup($1,$3); }
	;

dcl1:	  dcl
	| varlist
	;

dcl:	  attrs vars
		{ attvars($1,$2); $$ = $2; }
	| attrs LBRACK dcls1 RBRACK
		{ attvars($1,$3); $$ = $3; }
	| INITIAL initlist
		{ $$ = 0; }
	| IMPLICIT letton implist lettoff
		{ $$ = 0; }
	| EQUIVALENCE equivsets
		{ $$ = 0; }
	| EQUIVALENCE equivlist
		{ mkequiv($2); $$ = 0; }
	;

dcls:	  dcl
	| dcls EOS
	| dcls EOS dcl
		{ $$ = (int *)hookup($1,$3); }
	;

initlist:	init
	| initlist COMMA init
	;

init:	  lhs ASGNOP  {ininit = YES; }   expr
		= { ininit = NO;  mkinit($1,$4);  frexpr($1); }
	;

implist:  impgroup
	| implist COMMA impgroup;
	;

impgroup:  impspec
		{ setimpl(imptype, 'a', 'z'); }
	| impspec LPAR impsets RPAR
	;

impspec:  specs
		{ imptype = ((struct atblock *)$1)->attype; cfree($1); }
	;

impsets:  impset
	| impsets COMMA impset
	;

impset:	  LETTER
		{ setimpl(imptype, $1, $1); }
	| LETTER ADDOP LETTER
		{ setimpl(imptype, $1, $3); }
	;

equivsets:	equivset
	| equivsets COMMA equivset
	;

equivset:  LPAR equivlist RPAR
		{ mkequiv($2); }
	;

equivlist:  lhs COMMA lhs
		{ $$ = (int *)mkchain($1, mkchain($3,CHNULL)); }
	| equivlist COMMA lhs
		{ $$ = (int *)hookup($1, mkchain($3,CHNULL)); }
	;

attrs:	  attr
	| attrs attr	{ attatt($1,$2); }
	;

attr:	  spec dim	{ ((struct atblock *)$1)->atdim = $2; }
	| array dim	{ $$ = (int *)ALLOC(atblock); ((struct atblock *)$$)->atdim = $2; }
	;

dim:		{ $$ = 0; }
	| dimbound
	;

dimbound:  LPAR { inbound = 1; }  bounds RPAR
			{ inbound = 0;  $$ = arrays = (int *)mkchain($3,arrays); }
	;

bounds:	  bound
	| bounds COMMA bound	{ hookup($1,$3); }
	;

bound:	  ubound
		{
		$$ = (int *)ALLOC(dimblock);
		((struct dimblock *)$$)->lowerb = 0;
		((struct dimblock *)$$)->upperb = $1;
		}
	| expr COLON ubound
		{
		$$ = (int *)ALLOC(dimblock);
		((struct dimblock *)$$)->lowerb = $1;
		((struct dimblock *)$$)->upperb = $3;
		}
	;

ubound:	  expr
	| MULTOP  { $$ = 0; }
	;

vars:		{ $$ = 0; }
	| varlist
	;

varlist:  var
	| varlist COMMA var	{ hookup($1,$3); }
	;

var:	  varname dim
		{
		if($2!=0)
			if(((struct exprblock /*|| struct varblock */ *)$1)->vdim==0)
				((struct exprblock /*|| struct varblock */ *)$1)->vdim = $2;
			else if(!eqdim($2,((struct exprblock /*|| struct varblock */ *)$1)->vdim))
				dclerr("multiple dimension", ((struct stentry *)$1)->namep);
		$$ = (int *)mkchain($1,CHNULL);
		}
	| varname dim ASGNOP  { ininit = YES; }   expr
		{
		ininit = NO;
		if($3!=OPASGN)
			dclerr("illegal initialization operator", ((struct stentry *)((struct defblock /*|| struct labelblock|| struct varblock|| struct keyblock|| struct typeblock */ *)$1)->sthead)->namep);
		if($2!=0)
			if(((struct exprblock /*|| struct varblock */ *)$1)->vdim==0)
				((struct exprblock /*|| struct varblock */ *)$1)->vdim = $2;
			else if(!eqdim($2,((struct exprblock /*|| struct varblock */ *)$1)->vdim))
				dclerr("multiple dimension", ((struct stentry *)((struct defblock /*|| struct labelblock|| struct varblock|| struct keyblock|| struct typeblock */ *)$1)->sthead)->namep);
		if($5!=0 && ((struct varblock *)$1)->vinit!=0)
			dclerr("multiple initialization", ((struct stentry *)((struct defblock /*|| struct labelblock|| struct varblock|| struct keyblock|| struct typeblock */ *)$1)->sthead)->namep);
		((struct varblock *)$1)->vinit = $5;
		$$ = (int *)mkchain($1,CHNULL);
		}
	;

varname:  NAME
		{ $$ = mkvar($1); }
	;


specs:	  specarray
	| specs specarray	{ attatt($1,$2); }
	;

specarray:  spec
	| array dimbound
		{ $$ = (int *)ALLOC(atblock); ((struct atblock *)$$)->atdim = $2; }
	;

spec:	  sclass 
		{
		$$ = (int *)ALLOC(atblock);
		if($1 == CLEXT)
			((struct atblock *)$$)->atext = 1;
		((struct atblock *)$$)->atclass = $1;
		}
	| comclass contnu
		{
		$$ = (int *)ALLOC(atblock);
		((struct atblock *)$$)->atclass = CLCOMMON;
		((struct atblock *)$$)->atcommon = $1;
		}
	| stype
		{ $$ = (int *)ALLOC(atblock); ((struct atblock *)$$)->attype = $1; }
	| CHARACTER LPAR expr RPAR
		{ $$ = (int *)ALLOC(atblock); ((struct atblock *)$$)->attype = TYCHAR; ((struct atblock *)$$)->attypep = $3; }
	| FIELD LPAR bound RPAR
		{ $$ = (int *)ALLOC(atblock); ((struct atblock *)$$)->attype = TYFIELD;
		  ((struct atblock *)$$)->attypep = mkfield($3); }
	| deftype
		{ $$ = (int *)ALLOC(atblock); ((struct atblock *)$$)->attype = TYSTRUCT;
		  ((struct atblock *)$$)->attypep = $1; }
	| prec
		{ $$ = (int *)ALLOC(atblock); ((struct atblock *)$$)->atprec = $1; }
	;

sclass:	  AUTOMATIC	{ $$ = CLAUTO;
			  fprintf(diagfile,"AUTOMATIC not yet implemented\n"); }
	| STATIC	{ $$ = CLSTAT; }
	| INTERNAL	{ $$ = CLSTAT; }
	| VALUE		{ $$ = CLVALUE;
			  fprintf(diagfile, "VALUE not yet implemented\n");  }
	| EXTERNAL	{ $$ = CLEXT; }
	;

comclass:  COMMON LPAR comneed comname RPAR
			{ $$ = $4; }
	| COMMON MULTOP comneed comname MULTOP
			{ $$ = $4; }
	;

comneed:	{ comneed = 1; }
	;

comname:		{ $$ = mkcomm(""); }
	| COMNAME
	;

stype:	  INTEGER	{ $$ = TYINT; }
	| REAL		{ $$ = TYREAL; }
	| COMPLEX	{ $$ = TYCOMPLEX; }
	| LOGICAL	{ $$ = TYLOG; }
	| DOUBLE PRECISION
			{ $$ = TYLREAL; /* holdover from Fortran */ }
	| DOUBLEPRECISION
			{ $$ = TYLREAL; /* holdover from Fortran */ }
	;

deftype:  STRUCTNAME
		{ $$ = ((struct stentry *)$1)->varp; }
	| STRUCT structname contnu Struct
		  { $$ = mkstruct($2,$4); }
	| STRUCT Struct
		{ $$ = mkstruct(PNULL,$2); }
	;

structname:  NAME
		{ if(((struct stentry *)$1)->varp && ((struct headbits *)((struct stentry *)$1)->varp)->blklevel<blklevel)
			hide($1);
		  ((struct headbits *)$1)->tag = TSTRUCT;
		}
	| STRUCTNAME
		{ if(((struct stentry *)$1)->varp)
			if(((struct headbits *)((struct stentry *)$1)->varp)->blklevel<blklevel)
				hide($1);
			else dclerr("multiple declaration for type %s", ((struct stentry *)$1)->namep);
		}
	;

Struct:	  LBRACK  { ++instruct; }   dcls  { --instruct; }   RBRACK EOS
			{ $$ = $3; prevv = -1; }
	;

array:	  ARRAY
	| DIMENSION
	;

prec:	  LONG	{ $$ = 1; }
	| SHORT	{ $$ = 0; }
	;
