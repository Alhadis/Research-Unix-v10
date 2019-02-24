#include <stdio.h>
#include <ctype.h>
#include "mail.h"
#include "string.h"
#include "aux.h"

#define GFLIST "forwardlist"
#define LFLIST "forwardlist.lo"

/* predeclared */
static int ok();
static int okfile();

/* return true if ok to forward for this address.
 * `addr' is a list of ! delimited systems.
 */
extern int
ok_to_forward(addr)
	char *addr;
{
	char system[128];
	char *ap, *sp;
	static FILE *lfp;
	static FILE *gfp;

	for (ap = addr; *ap; ap++){
		for (sp = system; *ap && *ap!='!'; sp++, ap++)
			*sp = *ap;
		*sp = '\0';
		if (*ap != '!')
			return 1;
		switch(lookup(system, LFLIST, &lfp, GFLIST, &gfp)){
		case -1:
			/* no files */
			return 1;
		case 1:
			/* found */
			break;
		case 0:
			/* not found */
			return 0;
		default:
			/* call presotto */
			break;
		}
	}
	return 1;
}

/* return 1 if name found in one of the files
 *	  0 if name not found in one of the files
 *	  -1 if neither file exists
 */
int
lookup(cp, local, lfpp, global, gfpp)
	char *cp;
	char *local;
	FILE **lfpp;
	char *global;
	FILE **gfpp;
{
	static string *file = NULL;

	if (local) {
		if (file == NULL)
			file = s_new();
		abspath(local, UPASROOT, s_restart(file));
		if (*lfpp != NULL || (*lfpp = fopen(s_to_c(file), "r")) != NULL) {
			fseek(*lfpp, (long)0, 0);
			if (okfile(cp, *lfpp))
				return 1;
		} else
			local = 0;
	}
	if (global) {
		abspath(global, UPASROOT, s_restart(file));
		if (*gfpp != NULL || (*gfpp = fopen(s_to_c(file), "r")) != NULL) {
			fseek(*gfpp, (long)0, 0);
			if (okfile(cp, *gfpp))
				return 1;
		} else
			global = 0;
	}
	return (local || global)? 0 : -1;
}

static
okfile(cp, fp)
char *cp;
FILE *fp;
{
	char buf[128];
	int len;
	char *bp;
	int c;

	len = strlen(cp);
	
	/* one iteration per system name in the file */
	while (fgets (buf, sizeof buf, fp) != NULL) {
		buf[strlen(buf)-1] = '\0';
		for(bp=buf; *bp;){
			while(isspace(*bp) || *bp==',')
				bp++;
			if (strncmp (bp, cp, len) == 0) {
				c = *(bp+len);
				if(isspace(c) || c=='\0' || c==',')
					return 1;
			}
			while(*bp && (!isspace(*bp)) && *bp!=',')
				bp++;
		}
	}

	/* didn't find it, prohibit forwarding */
	return 0;
}
