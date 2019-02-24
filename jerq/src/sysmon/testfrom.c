#include <stdio.h>
main()
{
	char a[128];
	char m[128];
	char u[128];
	int i;

	while(1) {
		putchar('>');
		fflush(stdout);
		gets(a);
		i = from(a, m, u);
		if (i >= 0)
			printf("From = %s\nMachine = %s\nUser = %s\n", a, m, u);
	}
}
