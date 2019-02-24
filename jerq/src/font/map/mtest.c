#include	<stdio.h>

main()
{
	char buf[256];

	while(gets(buf) != NULL)
	{
		printf("char '%s': imap=%d smap=%d\n", buf, imap(buf), smap(buf));
	}
}
