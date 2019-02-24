#ifndef _d_
#define _d_

#define MAXSAME 16
#define MAXSAMECHAR 128

/* status of a destination */
typedef enum {
	d_undefined,	/* address has not been matched */
	d_pipe,		/* repl1|repl2 == delivery command, rep */
	d_cat,		/* repl1 == mail file  */
	d_translate,	/* repl1 == translation command */
	d_alias,	/* repl1 == translation */
	d_auth,		/* repl1 == command to authorize */
	d_syntax,	/* addr contains illegal characters */
	d_unknown,	/* addr does not match a rewrite rule */
	d_loop,		/* addressing loop */
	d_eloop,	/* external addressing loop */
	d_noforward,	/* forwarding not allowed */
	d_badmbox,	/* mailbox badly formatted */
	d_resource,	/* ran out of something we needed */
	d_pipeto	/* pipe to from a mailbox */
} d_status;

/* a destination */
typedef struct dest dest;
struct dest {
	dest *next;		/* for chaining */
	dest *same;		/* dests with same cmd */
	dest *parent;		/* destination we're a translation of */
	string *addr;		/* destination address */
	string *repl1;		/* substitution field 1 */
	string *repl2;		/* substitution field 2 */
	int pstat;		/* process status */
	int uid;		/* authentication */
	int gid;
	d_status status;	/* delivery status */
	int authorized;		/* non-zero if we have been authorized */
	int nsame;		/* number of same dests chained to this entry */
	int nchar;		/* number of characters in the command */
};

/* dest routines */
extern dest *d_new();
extern void d_free();
extern dest *d_rm();
extern void d_insert();
extern dest *d_rm_same();
extern void d_same_insert();
extern dest *s_to_dest();

/* rewrite rule types */
#endif


