#ifndef FORMAT_H
#define FORMAT_H
#ifndef UNIV_H
#include "univ.h"
#endif

#define F_EXT8		0x000001
#define F_EXT16		0x000002
#define F_MASK8		0x000004
#define F_MASK16	0x000008
#define F_HEX		0x000010
#define F_DECIMAL	0x000020
#define F_SIGNED	0x000040
#define F_OCTAL		0x000080
#define F_ASCII		0x000100
#define F_FLOAT		0x000200
#define F_DOUBLE	0x000400
#define F_DBLHEX	0x000800	
#define F_SHORTSTRING	0x001000
#define F_LONGSTRING	0x002000
#define F_SYMBOLIC	0x004000
#define F_ENUM		0x008000
#define F_UTYPE		0x010000
#define F_ARY		0x020000
#define F_NONE		0x040000
#define F_TURNOFF	0x080000
#define F_TIME		0x100000

#define F_STRING	(F_SHORTSTRING|F_LONGSTRING)
#define F_MASKEXT8	(F_MASK8|F_EXT8)
#define F_MASKEXT16	(F_MASK16|F_EXT16)
#define F_MASKEXT816	(F_MASKEXT8|F_MASKEXT16)
#define F_HOAD		(F_HEX|F_OCTAL|F_ASCII|F_DECIMAL)

#define TBLS 250
class Bls {
	char	*p;
public:
	char	text[TBLS+1];
	int	clear() 	{ *(p=text) = 0; return 1;}	// cfront bug
		Bls()		{ clear(); }
		Bls(char* ...);
		Bls(Bls&);
	char	*af(char* ...);
};

class Format {
class	SymTab	*stab;
	Bls	accum;
	char	*sep;
	void	grow(char*);
	void	grow(char*, long);
	void	grow(char*, double);
	void	growtime(long);
public:
	char	*f(long, double=0.0);
		Format(long=0, SymTab* =0);
	long	format;
};

char *FmtByte(int), *FmtName(int);
#endif
