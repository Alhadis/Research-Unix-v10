#include <stdio.h>
#include "mail.h"
#include "string.h"
#include "message.h"
#include "dest.h"

/*
 *  Translate the last component of the sender address.  If the translation
 *  yields the same address, replace the sender with its last component.
 */
extern int
gateway(mp)
	message *mp;
{
	char *base;
	dest *dp=NULL;
	static FILE *fp;
	extern dest *up_bind();
	char *sp;
	char system[128];
	extern char *thissys;

	/* first remove all systems equivalent to us */
	for (base = s_to_c(mp->sender); *base;){
		sp = strchr(base, '!');
		if(sp==NULL)
			break;
		*sp = '\0';
		if(lookup(base, "equivlist", &fp, 0, 0)==1){
			/* found or us, forget this system */
			*sp='!';
			base=sp+1;
		} else {
			/* no files or system is not found, and not us */
			*sp='!';
			break;
		}
	}

	/* punt if this is not a compound address */
	sp = strrchr(base, '!');
	if (sp==NULL)
		goto rebuild;
	sp++;

	/* bind the address to a command */
	d_insert(&dp, d_new(s_copy(sp)));
	dp->authorized = 1;
	dp = up_bind(dp, mp, 0);

	/* punt if translation did not succeed or resulted in multiple targets */
	if (dp==NULL || dp->next!=dp || dp->status!=d_pipe)
		goto rebuild;

	/* punt if the translation didn't result in the original address */
	if (strcmp(s_to_c(dp->addr), base)!=0)
		goto rebuild;
	base=sp;
rebuild:
	if(base!=s_to_c(mp->sender))
		mp->sender = s_copy(base);
}
