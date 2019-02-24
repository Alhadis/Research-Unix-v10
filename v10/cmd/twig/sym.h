/* symbol table definitions */
extern int symbol_undefined;

/*
 * A LabelData structures are associated with label symbols.  They
 * indicate that a tree is labelled with the symbol
 */
typedef struct LabelData	LabelData;

struct LabelData {
	Node *tree;
	int treeIndex;
	int lineno;
	SymbolEntry *label;	/* back pointer */
	LabelData *next;
};

struct treeassoc {
	int tree;
	struct treeassoc *next;
};

struct symbol_entry {
	int hash;
	char *name;
	short attr;
#		define  A_UNDEFINED		0
#		define	A_NODE			1
#		define  A_LABEL			2
#		define	A_COST			3
#		define	A_ACTION		4
#						define MAXCHAINS     A_CONST	
#						define HAS_UNIQUE(x) (x<MAXCHAINS)
#		define	A_CONST			5
#						define HAS_LIST(x) (x<=A_CONST)
#		define	A_ERROR			10
#		define	A_KEYWORD		11
	short unique;
	struct symbol_entry *link;
	struct symbol_entry *next;
	union {
		int keyword;
		int cvalue;	/* a constant's value */
		int arity;	/* information stored for A_NODE type */
		LabelData *lp;
		struct {
			int arity;
			Code *code;
		} predData;
		struct {
			struct treeassoc *assoc;
			Code *code;
		} ca;				/* data for cost/action symbols */
	} sd;
};

extern void SymbolInit();
extern void SymbolEnter();
extern SymbolEntry *SymbolLookup();
extern SymbolEntry *SymbolAllocate();
extern void SymbolFree();
extern void SymbolEnterList();
extern char *SymbolGenUnique();

extern SymbolEntry ErrorSymbol;

/*
 * In order for the walker to access the labelled leaves of a pattern,
 * a table (mistakenly) called the path table is generated (see sym.c).
 * This table contains the following possible values:
 *
 * ePush	follow the children link in the walker skeleton.
 * ePop		Pop up to parent.
 * eNext	follow  the siblings link.
 * eEval <arg>	The current node is a labelled leaf whose label id
 *		is the integer <arg>.
 * eStop	All done. stop!
 *
 * The table is interpreted by the _getleaves routine in the walker.
 */
#define	eSTOP	0
#define	ePOP	-1
#define eEVAL	-2
#define eNEXT	-3
#define ePUSH	-4
