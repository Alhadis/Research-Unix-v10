typedef struct {
	char *name;	/* header name */
	int size;	/* size of name */
	string *line;	/* header line */
} header;

extern header hdrs[];		/* important headers */

/* some useful macros */
#define HEADER(s) { s, sizeof(s)-1, (string *)0 }
#define STRCMP(s, p) strncmp((s), (p)->name, (p)->size)
#define HCONTENT(p) (s_to_c(p.line) + p.size)
