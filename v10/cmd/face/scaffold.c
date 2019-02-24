#include <stdio.h>
#include "faces.h"
#include "faceproto.h"

main(argc, argv)
int argc;
char **argv;
{
	register File *f;
	register int i;
	char *a[10];
	int na;
	char buf[100];

	while (fgets(buf, sizeof(buf), stdin)) {
		buf[strlen(buf)-1] = 0;	/* stupid newline */
		if ((na = getmfields(buf, a, 10)) <= 0)
			continue;
		switch (a[0][0]) {
		case 'i':
			if (na < 3) {
				printf("i pfile mfile\n");
				continue;
			}
			maketree(a[1], a[2]);
			printf("%ld\n", (long)sbrk(0));
			continue;

		case 'f':
			if (na < 2) {
				printf("f file\n");
				continue;
			}
			if ((f = lookfile(a[1])) == NULL) {
				printf("%s: not found\n", a[1]);
				continue;
			}
			if (isdir(f))
				printf("ino %d links %d dir nalloc %d nfiles %d\n",
					f->ino, f->nlinks, f->nalloc, f->nfiles);
			else
				printf("ino %d links %d file %s\n",
					f->ino, f->nlinks, f->data);
			continue;

		case 'd':
		case 'l':
			if (na < 2) {
				printf("%c dir\n", a[0][0]);
				continue;
			}
			if ((f = lookfile(a[1])) == NULL) {
				printf("%s: not found\n", a[1]);
				continue;
			}
			if (!isdir(f)) {
				printf("%s: not directory\n", a[1]);
				continue;
			}
			if (a[0][0] == 'd')
				dirdata(f);
			for (i = 0; i < f->nfiles; i++)
				if (a[0][0] == 'd')
					printf("%s\n", f->data+(i*FDLEN)+FD_NAME);
				else
					printf("%s\n", f->dir[i].name);
			continue;

		case 'D':
			if (na < 2) {
				printf("D dir\n");
				continue;
			}
			if ((f = lookfile(a[1])) == NULL) {
				printf("%s: not found\n", a[1]);
				continue;
			}
			if (!isdir(f)) {
				printf("%s: not directory\n", a[1]);
				continue;
			}
			dirdata(f);
			printf("%ld\n", sbrk(0));
			continue;

		default:
			printf("?\n");
			continue;
		}
	}
	exit(0);
}
