/* C compiler: symbolic configuration parameters */

typedef int Xinterface;
typedef int Env;
typedef struct {
	short lev;		/* node nesting level */
	short id;		/* node identification number */
	int state;
	Node next;		/* next node on output list */
} Xnode;

typedef struct {
	char *name;		/* name for back end */
	int offset;		/* locals: frame offset */
} Xsymbol;
