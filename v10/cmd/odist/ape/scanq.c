#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "dist.h"

static char *
copy(char *s)
{
	char *r;

	r = malloc(strlen(s) + 1);
	if (r)
		strcpy(r, s);
	return r;
}

static void
removeq(char *name)
{
	DIR *dp;
	struct dirent *d;
	char file[2 * MAXNAME + 1];

	name = copy(name);	/* readdir bug?!? */
	if (fork() == 0) {
		setuid(getuid());
		execl(LDIR "/genmail", "genmail", "-f", name, (char *) 0);
		exit(0);
	} else
		wait(0);
	if (!(dp = opendir(name)))
		return;
	while (d = readdir(dp)) {
		if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, ".."))
			continue;
		sprintf(file, "%s/%.*s", name, MAXNAME, d->d_name);
		remove(file);
	}
	closedir(dp);
	rmdir(name);
	free(name);
}

void
scanq(char *remsys, void (*func)(char *, int, char *))
{
	DIR *dp;
	struct dirent *d;
	FILE *fp;
	char sys[MAXLINE], cansys[MAXLINE], file[MAXNAME];
	int num, zapq;

	if (remsys)
		canon(remsys, cansys);

	if (chdir(SDIR) < 0) {
		eprintf("can't chdir %s", SDIR);
		exit(1);
	}

	if (!(dp = opendir("."))) {
		eprintf("can't read spool directory!");
		exit(1);
	}

	while (d = readdir(dp))
		if (strncmp(d->d_name, QPREFIX, strlen(QPREFIX)) == 0) {
			zapq = 1;
			if (chdir(d->d_name) < 0) {
				eprintf("can't chdir %s/%s", SDIR, d->d_name);
				continue;
			}
			if (fp = fopen(CTL, "r")) {
				while (fscanf(fp, "%d%s", &num, sys) == 2) {
					sprintf(file, "%d.%s", num, DONE);
					if (access(file, F_OK) == 0)
						continue;
					if (remsys && strcmp(sys, cansys) != 0) {
						zapq = 0;
						continue;
					}
					if (func)
						(*func)(d->d_name, num, sys);
					if (access(file, F_OK) != 0)
						zapq = 0;
				}
				fclose(fp);
			} else
				eprintf("can't read %s/%s", d->d_name, CTL);
			chdir("..");
			if (zapq)
				removeq(d->d_name);
		}

	closedir(dp);
}
