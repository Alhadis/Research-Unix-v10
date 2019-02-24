#include <pads.pri>

#define PI_VERSION 841027	/* YYMMDD */

#define	MUX_RUN		1
#define	MUX_BUSY	2
#define	MUX_BLOCKED	4
#define	MUX_USER	8
#define	MUX_KBDLOCAL	16
#define	MUX_MOUSELOCAL	32
#define	MUX_GOTMOUSE	64
#define	MUX_WAKEUP	128
#define	MUX_MOVED	256
#define	MUX_UNBLOCKED	512
#define	MUX_ZOMBIE	1024
#define	MUX_RESHAPED	2048
#define	MUX_ZOMBOOT	4096
#define	MUX_ALARMREQD	8192

#define FRAME_CACHE 16

enum CoreProto{
C_VERSION	= 0xA1,	/* L version S comment =>			*/
C_SPER		= 0xA2, /* L pid S cmd =>				*/
C_SPEE		= 0xA3, /* L pid =>					*/
C_JPS		= 0xA5, /*  => C NPROC (L P, S argv0)*			*/
C_DEBUG		= 0xA6, /*  => L P S argv0				*/

C_READ		= 0xB1,	/* L loc L ct => (C data)* S error		*/
C_WRITE		= 0xB2, /* L loc L ct, (C data)* => S error		*/
C_STRING	= 0xB3, /* L loc => S data S error			*/

CP_FLASH	= 0xC1,	/* L P => S error				*/
CP_RUN		= 0xC2,	/* L P => S error				*/
CP_STOP		= 0xC3,	/* L P => S error				*/
CP_STEP		= 0xC4, /* L P lo hi => L state pc psw S error		*/
CP_STACK	= 0xC5,	/* L P K => C lo hi (L fp L ap L pc)* S error	*/
CP_CONTROL	= 0xC6, /* L P => L reloc state pc psw S error		*/
CP_REGLOC	= 0xC7, /* L P C r => L loc S error			*/

C_LAYBPT	= 0xC8, /* L loc	=> C save S error		*/
C_LIFTBPT	= 0xC9, /* L loc C save => S error			*/

CG_POINT	= 0xD1,	/* L addr => S error				*/
CG_RECT		= 0xD2,	/* L addr => S error				*/
CG_TEXTURE	= 0xD3,	/* L addr => S error				*/
CG_BITMAP	= 0xD4,	/* L addr => S error				*/

CF_ALLOC	= 0xE1,	/* S bytes	=> L addr			*/
CF_FREE		= 0xE2,	/* L addr	=>				*/
CF_CALL		= 0xE3,	/* L fptr aptr  => L retloc S error		*/
};


#define	FT_RESET	0
#define FT_PROCESS	1
#define FT_STACK	2
#define FT_NORMAL	3

#define NORMAL_ZERODIV	(( 0 <<3) | FT_NORMAL)
#define NORMAL_TRACE	(( 1 <<3) | FT_NORMAL)
#define NORMAL_ILLOP	(( 2 <<3) | FT_NORMAL)
#define NORMAL_RESOP	(( 3 <<3) | FT_NORMAL)
#define NORMAL_INVDESC	(( 4 <<3) | FT_NORMAL)
#define NORMAL_EXTMEM	(( 5 <<3) | FT_NORMAL)
#define NORMAL_GATE	(( 6 <<3) | FT_NORMAL)
#define NORMAL_LEVEL	(( 7 <<3) | FT_NORMAL)
#define NORMAL_RESDATA	(( 8 <<3) | FT_NORMAL)
#define NORMAL_OFLOW	(( 9 <<3) | FT_NORMAL)
#define NORMAL_PRIVOP	((10 <<3) | FT_NORMAL)
#define NORMAL_BPT	((14 <<3) | FT_NORMAL)
#define NORMAL_PRIVREG	((15 <<3) | FT_NORMAL)

#define FT_MASK		0x3
#define ISC_MASK	(0x78|FT_MASK)

#ifdef IDTOCORE
struct IdToCORE {
	char	*id;
	int	op;
};
struct IdToCORE DisplayTable [] = {
	"Point",	(int) CG_POINT,
	"Rectangle",	(int) CG_RECT,
	"Rectangl",	(int) CG_RECT,	/* flex bugs */
	"Texture",	(int) CG_TEXTURE,
	"Bitmap",	(int) CG_BITMAP,
	"Layer",	(int) CG_BITMAP,
	0,		0
};
#endif
