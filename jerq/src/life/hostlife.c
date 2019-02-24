/*
 * host life: read and write files
 */

#include <stdio.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <ctype.h>

int	bugout();
struct	sgttyb stbuf;

main(argc, argv)
char **argv;
{
	char file[128];
	register c;
	FILE *fp;
	struct sgttyb nstbuf;

	setbuf(stdout, NULL);
	ioctl(0, TIOCGETP, &stbuf);
	signal(SIGHUP, bugout);
	nstbuf = stbuf;
	nstbuf.sg_flags &= ~(ECHO|CRMOD);
	nstbuf.sg_flags |= CBREAK;
	ioctl(0, TIOCSETP, &nstbuf);
	for (;;) {
		switch (getchar()) {

		case 'r':
			getfile(file);
			if ((fp = fopen(file, "r")) == NULL) {
				putchar('n');
			} else {
				int result = 'Y';

				putchar('y');
				while ((c = getc(fp)) != EOF) {
					if (isdigit(c) || c==' ' || c=='\t'
					  || c=='\n' | c=='-')
						putchar(c);
					else {
						result = 'N';
						break;
					}
				}
				putchar(result);
				fclose(fp);
			}
			continue;

		case 'w':
			getfile(file);
			if ((fp = fopen(file, "w")) == NULL) {
				putchar('n');
			} else {
				putchar('y');
				while((c = getchar()) != EOF && c != 'X')
					putc(c, fp);
				fclose(fp);
			}
			continue;

		default:
			break;
		}
		break;
	}
	bugout();
}

getfile(f)
register char *f;
{
	register c;

	while ((c = getchar()) != EOF && c != '\n')
		*f++ = c;
	*f++ = '\0';
}

bugout()
{
	ioctl(0, TIOCSETP, &stbuf);
	exit(0);
}
