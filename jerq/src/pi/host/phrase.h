#ifndef PHRASE_H
#define PHRASE_H
#ifndef UNIV_H
#include "univ.h"
#endif

class Phrase : PadRcv {
>pub
	char	pub_filler[16];
>pri
	friend	Frame;
	long	key;
	Expr	*expr;
	Frame	*frame;
	Pad	*pad();
	Phrase	*sib;
	void	applybinary(enum Op, Expr*);
	void	derive(Expr*);
	void	plugqindex(Expr*);
	void	plugqtype(Expr*);
>
	char	*help();
	char	*kbd(char*);
	void	numeric(long);
PUBLIC(Phrase,U_PHRASE)
		Phrase(Frame*, Phrase*, Expr*,long);
	void	applyarrow(Var*);
	void	applydot(Var*);
	void	allstar(UType*);
	void	alleval(UType*);
	void	applyunary(enum Op);
	void	evaluate();
	int	changed(Bls&);
	void	memory();
	void	reformat(int);
	void	setspy(long);
	void	applycast(DType*);
	void	strcast(long);
	void	enumcast(long);
	void	soretycast(long,short);
	void	increfcast(long);
	int	iscast();
};
#endif
