#include	<libc.h>
#include	<string.h>
#include	"crossbar.h"
#include	"switch.h"

extern int verbose;

Line::Line(char lab, char *n1, char *n2, Crossbar *cb, int l)
{
	name[0] = n1;
	name[1] = n2;
	c = cb;
	line = l;
	label = lab;
}

int
Line::pr(int label, char *buf, int len)
{
	int n;

	if(this == 0){
		for(n = 0; n < len; n++)
			buf[n] = ' ';
		buf[n] = 0;
		return(n);
	}
	sprint(buf, "%c: %s", inttolabel(label), name[verbose]);
	if((n = strlen(buf)) >= len)
		return(n);
	while(n < len)
		buf[n++] = ' ';
	buf[n] = 0;
	return(n);
}
