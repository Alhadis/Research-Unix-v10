/*
 *	Sydney C Compiler.
 *
 *	Copyright 1984, Bruce Ellis.
 *
 *	Unauthorised possesion, sale or use prohibited.
 */

struct xnode
{
	union
	{
		xnode	*xu_xnd;
		ident	*xu_id;
	}
		x_u0;

	union
	{
		struct
		{
			xnode	*x_x0;
			xnode	*x_x1;
		}
			x_s0;

		long	x_l;
	}
		x_u1;

	xnodes	x_what;
	data	x_value;

	union
	{
		struct
		{
			uchar	x_c0;
			uchar	x_c1;
		}
			x_s1;

		short	x_s;
	}
		x_u2;
};

#define	x_left		x_u0.xu_xnd
#define	x_subtype	x_u0.xu_xnd
#define	x_var		x_u0.xu_id

#define	x_right		x_u1.x_s0.x_x0
#define	x_type		x_u1.x_s0.x_x1
#define	x_index		x_u1.x_l

#define	x_flags		x_u2.x_s
#define	x_su		x_u2.x_s1.x_c1

#define	XIS_FORVAL	0x0001
#define	XIS_FORREF	0x0002
#define	XIS_TEST	0x0004
#define	XIS_HARDASSOP	0x0008

#define	XIS_LVAL	0x0010
#define	XWAS_LVAL	0x0020
#define	XIS_LTRAV	0x0040
#define	XIS_RTRAV	0x0080

#define	XIS_IMPLICIT	0x0100
