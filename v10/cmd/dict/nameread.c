#include <stdio.h>
#include <ctype.h>

main()
{
	int c;
	while((c=getchar())!=EOF) {
		switch(c) {
		case '%':
			percent();
			break;
		case ']':
			bracket();
			break;
		case '@':
			at();
			break;
		case '$':
			getchar();
			if(getchar()=='\n')
				putchar('\n');
			break;
		case '!':
			bang();
			break;
		default:
			putchar(c);
		}
	}
}

percent()
{
	int c = getchar();
	switch(c) {
		case '_':
			putchar('_');
			break;
		case '1':
		case '2':
		case '5':
		case '6':
		case '7':
		case '8':
			c = getchar();
			if(c!=' ')
				putchar(c); 	/* shouldn't happen */
			break;
		case '#':
			break;
		default:	/* shouldn't happen */
			putchar(c);
	}
}

at()
{
	int c = getchar();
	switch(c) {
	case 'A':
	case 'a':
		putchar('a');
		break;
	case 'e':
	default:	/* shouldn't happen */
		putchar(c);
		break;
	}
}

bracket()
{
	int c = getchar();
	if(c=='O')
		putchar('o');
	else
		putchar(c);
}

bang()
{
	int c = getchar();
	if(isdigit(c))
		return;
	else		/* shouldn't happen */
		putchar(c);
}

