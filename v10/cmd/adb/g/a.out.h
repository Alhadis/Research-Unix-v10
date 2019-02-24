struct	exec {	/* a.out header */
	long	a_magic;	/* magic number */
	long	a_text; 	/* size of text segment */
	long	a_data; 	/* size of initialized data */
	long	a_bss;  	/* size of unitialized data */
	long	a_syms; 	/* size of symbol table */
	long	a_entry; 	/* entry point */
	long	a_unused;	/* not used */
	long	a_flag; 	/* relocation info stripped */
};

#define	A_MAGIC		0407

#define	PGSIZE	8192		/* page size for rounding text */

#define	NNAME	20

typedef struct
{
	long value;
	char type;
	char name[NNAME];
	char pad[3];
} Sym;
