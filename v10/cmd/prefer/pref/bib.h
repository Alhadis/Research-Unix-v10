

/* file names */

#define BIBFILE "prefer.out"			/* default reference database */
#define COMFILE "/usr/lib/eign"		        /* common words to ignore */
#define IGNFILE "ignore"			/* %ref_fields to ignore */

/* size limits */
	
#define MAXCOMM 5000	/* maximum number of characters in common file */
#define MAXKLEN 6	/* default keyword length */

/* functions */

char *malloc();
char *strncat(), *strcat(), *strncpy(), *strcpy();
void exit();
