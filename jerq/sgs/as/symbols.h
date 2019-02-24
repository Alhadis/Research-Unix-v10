/*
 * static char ID_smblsh_[] = "@(#) symbols.h: 1.4 6/3/83";
 */

#include	"systems.h"
typedef char BYTE;

#define UNDEF	000
#define ABS	001
#define TXT	002
#define DAT	003
#define BSS	004
#define TYPE	(UNDEF|ABS|TXT|DAT|BSS)
#define TVDEF	020
#define EXTERN	040

typedef	union
	{
		char	name[9];
		struct
		{
			long	zeroes;
			long	offset;
		} tabentry;
	} name_union;

typedef	struct
	{
		name_union	_name;
		BYTE	tag;
		short	styp;
		long	value;
		short	maxval;
		short	sectnum;
	} symbol;

#define SYMBOLL	sizeof(symbol)

typedef	struct
	{
		char	name[sizeof(name_union)];
		BYTE	tag;
		BYTE	val;
		BYTE	nbits;
		long	opcode;
		symbol	*snext;
	} instr;

#define INSTR sizeof(instr);

#define INSTALL	1
#define N_INSTALL	0
#define USRNAME	0
#define MNEMON	1

typedef	union
	{
		symbol	*stp;
		instr	*itp;
	} upsymins;
