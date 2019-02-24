/*
 * our internal idea of a symbol
 */

struct sym {
	char *y_name;
	struct sym *y_next;
	struct sym *y_locals;
	char y_type;	/* e.g. TEXT or DATA or STAB */
	char y_ltype;	/* e.g. RSYM or LSYM */
	WORD y_value;
};

/*
 * global types, found in y_type
 */

#define	S_STAB	0	/* see y_ltype for more info */
#define	S_TEXT	1
#define	S_DATA	2
#define	S_ABS	3

/*
 * things found in y_ltype
 * these map in some way to sdb-style stabs,
 * but the goddamn things vary between different alleged unixes
 */

#define	S_LSYM	0	/* local: offset from frame pointer */
#define	S_RSYM	1	/* register */
#define	S_PSYM	2	/* procedure parameter offset */
#define	S_STSYM	3	/* static symbol */

extern	struct	sym *symtab;
extern	struct	sym *cursym;
struct	sym *lookup();
WORD	findsym();
