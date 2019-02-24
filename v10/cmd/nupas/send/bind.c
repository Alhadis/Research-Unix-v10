#include <regexp.h>
#include "mail.h"
#include "string.h"
#include <stdio.h>
#include "message.h"
#include "dest.h"

/* imported */
extern char *thissys;
extern int debug;

/* Return TRUE if a forwarding loop exists, i.e., the string `system'
 * is found more than 4 times in the return address.
 */
static int
forward_loop(addr, system)
	char *addr;
	char *system;
{
	int len = strlen(system), found = 0;

	while (addr = strchr(addr, '!'))
		if (!strncmp(++addr, system, len)
		 && addr[len] == '!' && ++found == 4)
			return 1;
	return 0;
}

/* bind the destinations to the commands to be executed */
extern dest *
up_bind(destp, mp, checkforward)
	dest *destp;
	message *mp;
{
	dest *list[2];		/* lists of unbound destinations */
	int li;			/* index into list[2] */
	dest *bound=NULL;	/* bound destinations */
	dest *dp;
	int i;

	list[0] = destp;
	list[1] = NULL;

	/*
	 *  loop once to check for:
	 *	- forwarding rights
	 *	- addressing loops
	 *	- illegal characters
	 */
	for (dp = d_rm(&list[0]); dp != NULL; dp = d_rm(&list[0])) {
		if (!checkforward)
			dp->authorized = 1;
		if (forward_loop(s_to_c(dp->addr), thissys)) {
			dp->status = d_eloop;
			d_same_insert(&bound, dp);
		} else if(forward_loop(s_to_c(mp->sender), thissys)) {
			dp->status = d_eloop;
			d_same_insert(&bound, dp);
		} else if(shellchars(s_to_c(dp->addr))) {
			dp->status = d_syntax;
			d_same_insert(&bound, dp);
		} else
			d_insert(&list[1], dp);
	}
	li = 1;

	/* Loop until all addresses are bound or address loop detected */
	for (i=0; list[li]!=NULL && i<32; ++i, li ^= 1) {
		/* Traverse the current list.  Bound items are put on the
		 * `bound' list.  Unbound items are put on the next list to
		 * traverse, `list[li^1]'.
		 */
		for (dp = d_rm(&list[li]); dp != NULL; dp = d_rm(&list[li])){
			dest *newlist;
			extern dest *expand_local();
			extern dest *translate();

			rewrite(dp, s_to_c(mp->replyaddr));
			if(debug)
				fprintf(stderr, "%s -> %s\n", s_to_c(dp->addr),
					s_to_c(dp->repl1));
			switch (dp->status) {
			case d_auth:
				/* authorize address if not already authorized */
				if(!dp->authorized){
					authorize(dp);
					if(dp->status==d_auth)
						d_insert(&list[li^1], dp);
					else
						d_insert(&bound, dp);
				}
				break;
			case d_cat:
				/* address -> local */
				newlist = expand_local(dp);
				if (newlist == NULL) {
					/* append to mailbox (or error) */
					d_same_insert(&bound, dp);
				} else if (newlist->status == d_undefined) {
					/* Forward to ... */
					d_insert(&list[li^1], newlist);
				} else {
					/* Pipe to ... */
					d_same_insert(&bound, newlist);
				}
				break;
			case d_pipe:
				/* address -> command */
				d_same_insert(&bound, dp);
				break;
			case d_alias:
				/* address -> rewritten address */
				newlist = s_to_dest(dp->repl1, dp);
				if(newlist != NULL)
					d_insert(&list[li^1], newlist);
				else
					d_same_insert(&bound, dp);
				break;
			case d_translate:
				/* pipe to a translator */
				newlist = translate(dp);
				if (newlist != NULL)
					d_insert(&list[li^1], newlist);
				else
					d_same_insert(&bound, dp);
				break;
			default:
				/* error */
				d_same_insert(&bound, dp);
				break;
			}
		}
	}

	/* mark remaining comands as "forwarding loops" */
	for (dp = d_rm(&list[li]); dp != NULL; dp = d_rm(&list[li])) {
		dp->status = d_loop;
		d_same_insert(&bound, dp);
	}

	return bound;
}

