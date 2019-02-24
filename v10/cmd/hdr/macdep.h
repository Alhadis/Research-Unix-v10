/*
 *	Sydney C Compiler.
 *
 *	Copyright 1984, Bruce Ellis.
 *
 *	Unauthorised possesion, sale or use prohibited.
 */

/*
 *  The null machine (cyntax checker or compiler debugger).
 */
#define	BITS_PER_BYTE	8
#define	MAXINTBITS	0xFFFFFFFFL
#define	MAXOF_LONG	0x7FFFFFFFL
#define	MINOF_LONG	0x80000000L
#define	MAXOF_INT	0x7FFFFFFFL
#define	MINOF_INT	0x80000000L
#define	MAXOF_SHORT	0x7FFFL
#define	MINOF_SHORT	(-0x8000L)

#define	ZOF_CHAR	1
#define	ZOF_SHORT	2
#define	ZOF_INT		4
#define	ZOF_LONG	4
#define	ZOF_PTR		4
#define	ZOF_FLOAT	4
#define	ZOF_DOUBLE	8

#define	CHAR_SIGN	0x80
#define	CHAR_SXT(i)	(-((~(i) + 1) & 0x7F))
