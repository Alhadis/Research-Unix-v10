#include <stdio.h>

main(argc, argv)
char **argv;
{
	register char	*cp;
	register int	i, wd;
	int	j;
	int nflg = 0;
	int escflg = 0;

	while (argc > 1) {
		if (strcmp(argv[1], "-n")==0) {
			nflg++;
			argc--;
			argv++;
		} else if (strcmp(argv[1], "-e")==0) {
			escflg++;
			argc--;
			argv++;
		} else if (strcmp(argv[1], "-ne")==0 ||
			   strcmp(argv[1], "-en")==0) {
			escflg++;
			nflg++;
			argc--;
			argv++;
			break;
		} else
			break;
	}
	for (i = 1; i < argc; i++) {
		for (cp = argv[i]; *cp; cp++) {
			if (*cp == '\\' && escflg)
			switch (*++cp) {
				case 'b':
					putchar('\b');
					continue;

				case 'c':
					return 0;

				case 'f':
					putchar('\f');
					continue;

				case 'n':
					putchar('\n');
					continue;

				case 'r':
					putchar('\r');
					continue;

				case 't':
					putchar('\t');
					continue;

				case 'v':
					putchar('\v');
					continue;

				case '\\':
					putchar('\\');
					continue;

				case '0': case '1': case '2': case '3':
				case '4': case '5': case '6': case '7':
					wd = *cp&07;
					j = 0;
					while (*++cp>='0' && *cp<='7' && ++j<3){
						wd <<= 3;
						wd |= (*cp - '0');
					}
					putchar(wd);
					--cp;
					continue;

				default:
					cp--;
			}
			putchar(*cp);
		}
		if (i < argc-1)
			putchar(' ');
	}
	if (!nflg)
		putchar('\n');
	return 0;
}
