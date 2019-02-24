#include <stdio.h>
#include <string.h>
#include "misc.h"

void
addsub(struct sub **sublistp, char *ostr, char *nstr)
{
	struct sub *nsub;

	nsub = xmalloc(sizeof (struct sub));
	nsub->ostr = xstrdup(ostr);
	nsub->nstr = xstrdup(nstr);
	nsub->next = *sublistp;
	*sublistp = nsub;
}

int
sub(struct sub **sublistp, int prefix, char str[], int maxlen)
{
	struct sub *sub;
	int olen, nlen;

	for (sub = *sublistp; sub; sub = sub->next) {
		olen = strlen(sub->ostr);
		if (prefix && strncmp(str, sub->ostr, olen) == 0)
			break;
		else if (!prefix && strcmp(str, sub->ostr))
			break;
	}
	if (!sub)
		return 1;
	nlen = strlen(sub->nstr);
	if (strlen(str) + nlen - olen > maxlen)
		return 0;
	memmove(str + nlen, str + olen, strlen(str + olen) + 1);
	strncpy(str, sub->nstr, nlen);
}
