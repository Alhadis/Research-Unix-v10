#ifndef EXPR_H
#define EXPR_H
#ifndef UNIV_H
#include "univ.h"
#endif
#include "dtype.pub"
#include "op.h"
#include "format.pub"
>pri

enum EDisc{
	E_UNARY		= 1,
	E_BINARY	= 2,
	E_LCONST	= 3,
	E_DCONST	= 4,
	E_ID		= 5,
	E_SYMNODE	= 6,
};

char *OpName(Op);
>

class Spy{
	friend	Expr;
	friend	Phrase;
	Bls	b;
		Spy()	{}
};

class Expr : PadRcv {
>pub
	char	pub_filler[4];
>pri
	friend	Phrase;
	EDisc	edisc;
	char	*under(Expr*);
	char	*left(Expr*);
	char	*evalunary(Frame*);
	char	*evalbinary(Frame*);
	char	*evalindex(Frame*,Expr*,long);
	char	*evalcast();
	char	*ascii(Frame*,int limit);
	char	*enumformat();
	char	*utypeformat(Frame*, Bls&);
	char	*getval(Frame*);
	char	*evaldotarrow(Frame*);
	char	*evalcall(Frame*);
	char	*evalassign(Frame*);
	char	*invalidoperands(char* =0);
	char	*evaltextcomma(Frame*, Bls&);
	char	*doevaltext(Frame*, Bls&);
	Expr	*actual(int i);
	char	*eval(Frame*);
	char	*evalenv(Frame*);
	Index	castcarte();
	char	*textunary();
	char	*textbinary();
	char	*evalrange();
	char	*floaterror();
	char	*evalflop();
	void	catchfpe();
	char	*enumid(Frame*);
static	double	*fp1;
static	double	*fp2;
static	char	*fpe;
>
PUBLIC(Expr,U_EXPR)
		Expr();	
		Expr(EDisc, Op, Expr*, Expr*, Cslfd, char*, int, Symbol*);
	char	*id;
	Symbol	*sym;
	Expr	*sub1;
	Expr	*sub2;
	DType	type;
	Cslfd	val;
	long	addr;
	Spy	*spy;
	short	bitaddr;
	char	op;
	char	evalerr;
	char	*evaltext(Frame*, Bls&);
	char	*text();
	Index	carte(Frame*);
	int	format();
	void	setspy(long);
	void	reformat(int,int=0);
};

Expr	*E_Id(char*),
	*E_Sym(Symbol*),
	*E_Unary(Op,Expr*),
	*E_Binary(Expr*,Op,Expr*),
	*E_IConst(long);
#endif
