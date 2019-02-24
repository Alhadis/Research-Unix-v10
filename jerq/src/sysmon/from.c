#include <stdio.h>
#include <ctype.h>
#include <regexp.h>

/* imports */
extern int strlen();
extern char *strrchr();
extern char *strchr();

extern int
from(addrp, src, sender)
	char *addrp;		/* address to rewrite */
	char *src;		/* where to source machine name */
	char *sender;		/* where to put sender name */
{
	char *nsender, *nsrc, *cp;

	/* very cruddy algorithm */
	nsender = strrchr(addrp, '!');
	if (nsender == NULL) {
		/* local mail */
		nsender = addrp;
		nsrc = "";
	} else {
		/* at least one ! */
		*nsender++ = '\0';
		nsrc = strrchr(addrp, '!');
		if (nsrc == NULL)
			nsrc = addrp;
		else
			*nsrc++ = '\0';
	}
	/* look for domain address in sender - note precedence of %, @, and . */
	if ((cp = strchr(nsender, '%'))!=NULL ||
	    (cp = strchr(nsender, '@'))!=NULL ||
	    (cp = strchr(nsender, '.'))!=NULL) {
		*cp = '\0';
		nsrc = ++cp;
	}
	(void)strcpy(sender, nsender);
	(void)strcpy(src, nsrc);
	return 0;
}
