#include <fio.h>
main()
{
	char *hp, *np, *ap;
	unsigned long x;

	print(">");
	while(hp = Frdline(0)){
		np = (char *)strchr(hp, ' ');
		if(np) {
			*np++ = '\0';
			x = in_address(np);
			print("%s %ux\n", np, x);
			ap = (char *)qvalue("in", np, (char *)0);
			x = in_aton(ap);
			print("%s %ux\n", ap, x);
			x = (unsigned long)in_addronnet(hp, x, 0);
			print("%ux\n", x);
		} else {
			x = (unsigned long)in_address(hp);
			print("%ux\n", x);
		}
		print(">");
	}
}
