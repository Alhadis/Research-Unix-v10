#include "stdio.h"
#define	MAXLINE	1000

int	line;

main() {	/* index cruncher
			accumulates line numbers for adjacent
			identical lines */
	char line1[MAXLINE], line2[MAXLINE];
	int i, j;

	if (fgets(line1, MAXLINE, stdin) == NULL)
		return;
	line++;
	while (fgets(line2, MAXLINE, stdin) != NULL) {
		line++;
		for (i = 0; line1[i] == line2[i] &&
		  line1[i] != '\t' && line1[i] != '\n'; i++)
			/* compare thru tab */
			if (i >= MAXLINE)
				error("i = %d\n", i);
		if (line1[i] != line2[i]) {
			fputs(line1, stdout);	/* don't match */
			strcpy(line1, line2);
		}
		else if (line1[i] != '\n') {	/* match */
			for (j = ++i; line1[j] != '\n'; j++)
				if (j >= MAXLINE)
					error("j = %d\n", j);
				/* find end of line1 */
			line1[j++] = ',';
			line1[j++] = ' ';
			while ((line1[j++] = line2[i++]) != '\0')
				if (i >= MAXLINE || j >= MAXLINE)	/* copy tail */
					error("i = %d, j = %d\n", i, j);
		}
		else
			fprintf(stderr, "can't happen at line %s\n", line2);
	}
	fputs(line1, stdout);
}

error(s1, s2, s3, s4)
{
	fprintf(stderr, "error at line %d\n ", line);
	fprintf(stderr, s1, s2, s3, s4);
	fprintf(stderr, "\n");
	exit(1);
}
