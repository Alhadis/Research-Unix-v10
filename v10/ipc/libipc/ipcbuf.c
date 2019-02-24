#include "defs.h"
/*
 *	stretching buffers
 */
_strinit(sp)
	stretch *sp;
{
	sp->ptr = NULL;
	sp->len = 0;
}

_strcat(sp, p1, p2, p3)
	stretch *sp;
	char *p1, *p2, *p3;
{
	char buf[2048];
	int len=0;
	char *malloc();

	if (p1) {
		len += strlen(p1);
		if (p2) {
			len += strlen(p2);
			if (p3)
				len += strlen(p3);
		}
	}
	if (len<sizeof(buf)) {
		buf[0] = '\0';
		if (p1) {
			strcat(buf, p1);
			if (p2) {
				strcat(buf, p2);
				if (p3)
					strcat(buf, p3);
			}
		}
	} else {
		strcpy(buf, "string_too_long");
		len = strlen(buf);
	}
	if (sp->len<=len) {
		if(sp->ptr)
			free(sp->ptr);
		sp->ptr = malloc(len+1);
		sp->len = len+1;
	}
	strcpy(sp->ptr, buf);
}

_strkill(sp)
	stretch *sp;
{
	if (sp->ptr)
		free(sp->ptr);
}
