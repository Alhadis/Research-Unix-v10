#include <sys/inet/in.h>
#include <ctype.h>

/*
 * Convert network-format internet address
 * to base 256 d.d.d.d representation.
 */
char *
in_ntoa(in)
	in_addr in;
{
	register char *p;
	static char b[18];
#define	UC(b)	(((int)b)&0xff)

	/*
	 *  number of network bytes depends on network class
	 */
	p = (char *)&in;
	if(IN_CLASSC(in)) {
		if((in & IN_CLASSC_HOST)==0){
			sprintf(b, "%d.%d.%d", UC(p[3]), UC(p[2]), UC(p[1]));
			return b;
		}
	} else if(IN_CLASSB(in)) {
		if((in & IN_CLASSB_HOST)==0){
			sprintf(b, "%d.%d", UC(p[3]), UC(p[2]));
			return b;
		}
	} else {
		if((in & IN_CLASSA_HOST)==0){
			sprintf(b, "%d", UC(p[3]));
			return b;
		}
	}
	sprintf(b, "%d.%d.%d.%d", UC(p[3]), UC(p[2]), UC(p[1]), UC(p[0]));
	return b;
}

/*
 *  Convert base 256 d.d.d.d representation
 *  to network-format internet address.
 */
in_addr
in_aton(s)
	register char *s;
{
	register int b;
	register int x, i;
	in_addr h, n;

	/*
	 *  first bytee determines class of network
	 */
	for(b = 0; *s; s++){
		if(*s=='.'){
			s++;
			break;
		} else if(isdigit(*s)) {
			b = b*10 + (*s - '0');
		} else
			return INADDR_ANY;
	}

	/*
	 *  number of network bytes depends on network class
	 */
	n = b<<24;
	if(IN_CLASSC(n))
		x = 0;		/* 2 more net bytes */
	else if(IN_CLASSB(n))
		x = 1;		/* 1 more net bytes */
	else
		x = 2;		/* no more net bytes */

	/*
	 *  get network bytes
	 */
	for(i = 2; i>x; i--){
		for(b = 0; *s; s++){
			if(*s=='.'){
				s++;
				break;
			}
			b = b*10 + (*s - '0');
		}
		n |= b<<(8*i);
	}

	/*
	 *  get host bytes
	 */
	h = 0;
	while(*s){
		for(b = 0; *s; s++){
			if(*s=='.'){
				s++;
				break;
			}
			b = b*10 + (*s - '0');
		}
		h = (h<<8) + b;
	}

	return h | n;
}
