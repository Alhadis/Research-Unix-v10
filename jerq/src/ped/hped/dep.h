#define	NULL	0
#define	EPSILON	1.0e-6
#define	abs(x)	(((x)>0)?(x):-(x))

/* definitions related to dependency lists */

typedef struct _deplist {
	struct _deplist *next;
	double coeff;
	struct _variable *var;
} deplist;

#define	NEXT(p)	(p->next)
#define	COEFF(p)	(p->coeff)
#define	VAR(p)	(p->var)

/* functions in dep.c */
extern void depinit ();
extern deplist *depgen ();
extern void depfree ();
extern void depclean ();
extern deplist *depadd ();
extern deplist *depsubst ();

/* definitions related to variables */

typedef struct _variable {
	deplist *rep, *nrep;
	struct _variable *nextdep; /* for hooking together the depvarlist */
} variable;

extern void removedeps ();
extern void resetreps ();

/* functions in solve.c */
extern void depprint ();
