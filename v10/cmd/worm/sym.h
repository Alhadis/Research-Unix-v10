typedef struct Symtab
{
	short space;
	char *name;
	void *value;
	struct Symtab *next;
} Symtab;
extern void *symlook();

#define		S_INODE		1
#define		S_UID		2
#define		S_GID		3
#define		S_FAIL		4	/* fetch */
#define		S_TOGO		5	/* wormy */

extern int sym_mem_fail;
