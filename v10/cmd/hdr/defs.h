/*
 *	Sydney C Compiler.
 *
 *	Copyright 1984, Bruce Ellis.
 *
 *	Unauthorised possesion, sale or use prohibited.
 */

#include	"sets.h"

#define	SYSERROR	(-1)

#define	NULL		0
#define	NODE_GRABZ	64
#define	BUFFZ		4096
#define	OUTZ		4096
#define	SBUFFZ		32
#define	PERM_CHUNKZ	2048
#define	PUSHBACKZ	10
#define	EOF_CHAR	'\0'
#define	STDIN		0
#define	STDOUT		1
#define	NO_FID		(-1)

#define	loop		for (;;)
#define	readonly
#define	nels(x)		(sizeof x / sizeof x[0])

#define ctack(c, d)	((c)->c_tail->c_next = (d)->c_head, (c)->c_tail = (d)->c_tail)
#define cadd(c, w)	((c)->c_next = new_cnode(), (c) = (c)->c_next, (c)->c_what = (w))

#define	pack(x)		((pack_t)x)

typedef unsigned char	uchar;
typedef unsigned char	pack_t;

#include	"xnodes.h"
#include	"places.h"

/*
 *	Forward declarations of general purpose data types.
 */
typedef	set char	chset;
typedef set xnodes	xtset;

#include	"data.h"
#include	"xnode.h"
#include	"ident.h"

struct st_node
{
	st_node	*st_left;
	st_node	*st_right;
	char	*st_name;
	long	st_index;
	ident	*st_idlist;
};

struct	tkdope
{
	uchar	equiv_xt;
	char	prec_lev;
};

struct	level
{
	ident	*lv_idents;
};

struct choice
{
	cnode	*ch_def;
	cnode	*ch_case;
};

typedef union
{
	char	*m;
	double	*f;
	long	i;
	token	*t;
	xnode	*x;
}
	formarg;

#include	"alloc.h"
#include	"extern.h"
