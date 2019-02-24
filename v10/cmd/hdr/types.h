/*
 *	Sydney C Compiler.
 *
 *	Copyright 1984, Bruce Ellis.
 *
 *	Unauthorised possesion, sale or use prohibited.
 */

/*
 *	Type flags.
 */
#define	TYPE_OTHERS	(GLOBAL)
#define	TYPE_TYPES	(TYPEDEF | TYPEDEF_USE)
#define	TYPE_SCOPE	(ARGUMENT | AUTO | EXTERN | REGISTER | STATIC)
#define	TYPE_CLASS	(TYPE_OTHERS | TYPE_TYPES | TYPE_SCOPE)
#define	TYPE_BASE	(CHAR | SHORT | INT | FLOAT | VOID)
#define	TYPE_ADJECTIVE	(UNSIGNED | LONG)
#define	TYPE_COMPLEX	(ENUM | STRUCT | UNION | MOENUM)
#define	TYPE_PROPER	(TYPE_BASE | TYPE_COMPLEX)

/*
 *	If these are changed the type initialisation table in init.c
 *	must be tweeked.
 */

/*
 *	Adjectives.
 */
#define	UNSIGNED	0x000001L
#define	LONG		0x000002L
/*
 *	Base types.
 */
#define	CHAR		0x000004L
#define	SHORT		0x000008L
#define	INT		0x000010L
#define	FLOAT		0x000020L
#define	VOID		0x000040L

/*
 *	Classes and class modifiers.
 */
#define	GLOBAL		0x000100L

#define	TYPEDEF		0x000200L

#define	ARGUMENT	0x000400L
#define	AUTO		0x000800L
#define	BLOCKSTATIC	0x001000L
#define	EXTERN		0x002000L
#define	REGISTER	0x004000L
#define	STATIC		0x008000L

/*
 *	Complex types.
 */
#define	ENUM		0x010000L
#define	STRUCT		0x020000L
#define	UNION		0x040000L
#define	MOENUM		0x080000L

/*
 *	Other type flags.
 */
#define	PURE		0x100000L
#define	TYPEDEF_USE	0x200000L

#define	ID_SCOPES	(AUTO | BLOCKSTATIC | STATIC | GLOBAL | TYPEDEF)
