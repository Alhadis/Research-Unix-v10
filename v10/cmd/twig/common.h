#include <stdio.h>
#include <assert.h>

#define MAXIDSIZE 100
#define HASHSIZE 1181
#define MAXPATH 32
#define MAXBRANCH 10
#define MAXACCEPTS 100
#define MAXSTATES 200
/* type definitions */
typedef char	byte;

/* forward and external type defs */
extern struct node *GetNode();
extern struct edges *GetEdge();
extern struct augmented_edges *GetAugEdges();
extern char *malloc();
extern char *strrchr();
extern FILE *outfile;
extern FILE *symfile;

extern int debug_flag;
#	define	DB_LEX	1	/* print out lexical analyser debugging info */
#	define	DB_MACH	2	/* print out machine information */
#	define	DB_SYM	4	/* print out symbol debugging info */
#	define	DB_TREE	8	/* print out trees */
#	define	DB_MEM	16	/* check memory usage */
extern int tflag;		/* generate tables only */
extern int ntrees;
extern int unique;

/* tree definitions */
typedef struct node	Node;
typedef struct symbol_entry SymbolEntry;

/* Nodes provide the backbone for the trees built by the parser */
struct node {
	SymbolEntry *sym;
	int nlleaves;	/* count of the labelled leaves */
	Node *children;
	Node *siblings;
};

extern Node *TreeBuild();

/* path defintions */
extern void path_setup();

/* lexical analyser */
extern int yyline;
extern char *inFileName;
extern char token_buffer[MAXIDSIZE+1];

extern void LexInit();
extern yylex();
