/* from id.c */
extern int gidnum(char *);
extern char *gidstr(int);
extern int uidnum(char *);
extern char *uidstr(int);

/* from sub.c */
struct sub {
	char *ostr;
	char *nstr;
	struct sub *next;
};

extern void addsub(struct sub **, char *, char *);
extern int sub(struct sub **, int, char [], int);

/* from util.c */
extern char *prog;
extern void *xmalloc(size_t);
extern char *xstrdup(char *);
extern long fpcopy(FILE *, FILE *, long);
extern void discard(FILE *, long);
