#ifndef DTYPE_H
#define DTYPE_H
#ifndef UNIV_H
#include "univ.h"
#endif
class DType : public PadRcv {
	PadRcv	*univ;
>pri
	friend CoffSymTab; friend Ed8SymTab; friend MccSymTab; friend Block; friend Func; friend SymTab; friend Var;
	int	formatset();
	void	free();
>
PUBLIC(DType,U_DTYPE)
		DType();
	int	dim;
	short	pcc;
	int	over;
class	UType	*utype();
	DType	*ref();
	DType	incref();
	DType	*decref();
	char	*text();
	int	format();
struct	Index	carte();
	int	size_of();
	int	isary();
	int	isaryorptr();
	int	isftn();
	int	isintegral();
	int	isptr();
	int	isreal();
	int	isscalar();
	int	isstrun();
	void	reformat(int,int=0);
};
char *PccName(int);
#endif
