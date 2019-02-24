#include "mail.h"
#include "string.h"
#include "dest.h"

/* imports */
extern char *malloc();
extern void exit();

/* exports */
dest *dlist;

extern dest*
d_new(addr)
	string *addr;
{
	dest *dp;

	dp = (dest *)malloc(sizeof(dest));
	if (dp == NULL) {
		perror("destnew");
		exit(1);
	}
	dp->same = dp;
	dp->nsame = 1;
	dp->nchar = 0;
	dp->next = dp;
	dp->addr = addr;
	dp->parent = NULL;
	dp->repl1 = dp->repl2 = NULL;
	dp->status = d_undefined;
	dp->uid = dp->gid = -1;
	return dp;
}

extern void
d_free(dp)
	dest *dp;
{
	if (dp != NULL) {
		s_free(dp->addr);
		s_free(dp->repl1);
		s_free(dp->repl2);
		free((char *)dp);
	}
}

/* The following routines manipulate an ordered list of items.  Insertions
 * are always to the end of the list.  Deletions are from the beginning.
 *
 * The list are circular witht the `head' of the list being the last item
 * added.
 */

/*  Get first element from a circular list linked via 'next'. */
extern dest *
d_rm(listp)
	dest **listp;
{
	dest *dp;

	if (*listp == NULL)
		return NULL;
	dp = (*listp)->next;
	if (dp == *listp)
		*listp = NULL;
	else
		(*listp)->next = dp->next;
	dp->next = dp;
	return dp;
}

/*  Insert a new entry at the end of the list linked via 'next'. */
extern void
d_insert(listp, new)
	dest **listp;	/* pointer to current list */
	dest *new;	/* list to be added */
{
	dest *head;

	if (*listp == NULL) {
		*listp = new;
		return;
	}
	if (new == NULL)
		return;
	head = new->next;
	new->next = (*listp)->next;
	(*listp)->next = head;
	return;
}

/*  Get first element from a circular list linked via 'same'. */
extern dest *
d_rm_same(listp)
	dest **listp;
{
	dest *dp;

	if (*listp == NULL)
		return NULL;
	dp = (*listp)->same;
	if (dp == *listp)
		*listp = NULL;
	else
		(*listp)->same = dp->same;
	dp->same = dp;
	return dp;
}

/* Insert an entry into the corresponding list linked by 'same'.  Note that
 * the basic structure is a list of lists.
 */
extern void
d_same_insert(listp, new)
	dest **listp;	/* pointer to current list */
	dest *new;	/* new dest (or null terminated list of dests) */
{
	dest *dp;
	int len;

	if(new->status == d_pipe) {
		len = new->repl2 ? strlen(s_to_c(new->repl2)) : 0;
		if(*listp != NULL){
			dp = (*listp)->next;
			do {
				if(dp->status == new->status
				&& dp->nsame < MAXSAME
				&& dp->nchar + len < MAXSAMECHAR
				&& strcmp(s_to_c(dp->repl1), s_to_c(new->repl1))==0
				&& dp->uid == new->uid && dp->gid == new->gid){
					new->same = dp->same;
					dp->same = new;
					dp->nchar += len + 1;
					dp->nsame++;
					return;
				}
				dp = dp->next;
			} while (dp != (*listp)->next);
		}
		new->nchar = strlen(s_to_c(new->repl1)) + len + 1;
	}
	new->next = new;
	d_insert(listp, new);
}

/* expand a string of destinations into a linked list of destiniations */
extern dest *
s_to_dest(sp, parent)
	string *sp;
	dest *parent;	/* parent of new destinations */
{
	string *addr;
	dest *list=NULL;
	dest *new;

	if (sp == NULL)
		return NULL;
	addr = s_new();
	while (s_parse(sp, addr)!=NULL) {
		if(shellchars(s_to_c(addr))){
			while(new = d_rm(&list))
				d_free(new);
			break;
		}
		new = d_new(addr);
		new->parent = parent;
		new->authorized = parent->authorized;
		d_insert(&list, new);
		addr = s_new();
	}
	s_free(addr);
	return list;
}
