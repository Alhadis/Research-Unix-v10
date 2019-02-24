#include	"mk.h"

#define	NMATCH	10

gen(buf)
	char *buf;
{
	char buf1[BIGBLOCK];
	register char *src, *dest;
	register char *s1, *s2, *s3, *s4;
	regexp *prog;
	regsubexp match[NMATCH];
	char c;

	if((s1 = strchr(buf, '@')) == 0)
			return;
	patrule = 0;
	src = buf;
	dest = buf1;
	do {
		while(src != s1)
			*dest++ = *src++;
		if((s2 = strchr(s1+1, '@')) && (s3 = strchr(s2+1, '@'))
		&& (s4 = strchr(s3+1, '@')))
			;
		else {
			SYNERR(-1); Fprint(2, "expected four @\n");
			break;
		}
		src = s4+1;
		*s2++ = *s3++ = *s4 = 0;
		prog = regcomp(s2);
		for(s2 = ++s1; ; s2 = s1){
			while((*s2 == ' ')||(*s2 == '\t'))
				s2++;
			if(s1 != s2) *dest++ = ' ';
			if(*s2 == 0) break;
			for(s1 = s2+1; *s1 && (*s1 != ' ') && (*s1 != '\t');)
				s1++;
			c = *s1;
			*s1 = 0;
			if(regexec(prog, s2, match, NMATCH)){
				regsub(s3, dest, match, NMATCH);
				while(*dest) dest++;
			} else
				while(*dest = *s2++)
					dest++;
			*s1 = c;
		}
	} while(s1 = strchr(src, '@'));
	while(*dest++ = *src++)
		;
	memcpy(buf, buf1, (int)(dest-buf1));
}
