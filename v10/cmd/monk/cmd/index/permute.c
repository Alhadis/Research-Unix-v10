#include <stdio.h>
#define	MAXLINE	1000

/*
 * permute - convert input lines like
	a b c \t n
   into
	a b c	n
	b c, a	n
	c, a b	n
 */

main(argc, argv)
char *argv[];
{
	FILE *fp;

	if (argc <= 1)
		permute(stdin);
	else
		while (--argc > 0) {
			if ((fp = fopen(*++argv, "r")) == NULL) {
				fprintf(stderr, "permute: can't open %s\n", *argv);
				exit(1);
			}
			permute(fp);
			fclose(fp);
		}
	exit(0);
}

permute(fp)	/* permute file fp */
FILE *fp;
{
	char line[MAXLINE];
	int i, j, k, l;

	while (fgets(line, MAXLINE, fp) != NULL) {
		for (i = 0; ; ) {
			for (j = i; line[j] == ' '; j++)
				;	/* skip blanks */
			if (line[j] == '\t' || line[j] == '\n')
				break;
			for (k = j; line[k] != '\t' && line[k] != '\n'; k++)
				putchar(line[k]);	/* put head */
			if (i > 0) {	/* put ", old head" */
				putchar(',');
				putchar(' ');
				for (l = 0; l < i; l++)
					putchar(line[l]);
			}
			if (line[k] != '\n')	/* put tail */
				while (putchar(line[k++]) != '\n')
					;
			for (i = j; line[i] != ' ' && line[i] != '\t' && line[i] != '\n'; i++)
				if (line[i] == '\\')	/* find next word */
					i++;
		}
	}
}
