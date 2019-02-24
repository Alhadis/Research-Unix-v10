#ifndef LDLIST

struct ldlist {
	LDFILE		ld_item;
	struct ldlist	*ld_next;
};

#define	LDLIST	struct ldlist
#define	LDLSZ	sizeof(LDLIST)

#endif
static char lddef_ID[ ] = "@(#) lddef.h: 1.1 1/7/82";
