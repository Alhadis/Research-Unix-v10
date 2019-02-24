/* the structure describing a sub-expression match */
typedef struct {
	char *sp;
	char *ep;
} regsubexp;

/* a compiled regular expression */
typedef char *regexp;

/* the routines */
extern regexp *regcomp();
extern int regexec();
extern void regsub();
