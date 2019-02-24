#include <sys/types.h>
#include <dir.h>
#include <sys/stat.h>
#include <stdio.h>
#include "defs.h"
#include "scanmail.h"

/* import */
extern char *strcat(), *strcpy(), *strncpy();

/* init the file */
void
sminit (smp, file, name)
	struct scanmail *smp;
	char *file;
	char *name;
{
	char *p;
	struct stat buf;
	char dir[128];
	char *strrchr();
	char islog = FALSE;
	int fd;

	/* look for a mail log */
	if ((fd=open(MAILLOG, 0)) >= 0)
		islog = TRUE;
	close(fd);

	/* defaults */
	strcpy (smp->sm_match, "From ");
	smp->sm_log = FALSE;

	/* change directory to avoid screwing with namei */
	strcpy(dir, MAILDIR);
	strcat(dir, "/");
	strcat(dir, name);
	if (file == NULL || strcmp(dir, file) == 0) {

		/* use the standard mail directory if none is specified */
		chdir (MAILDIR);
		if (islog) {
			smp->sm_log = TRUE;
			strcpy (smp->sm_file, MAILNAME);
			strcpy (smp->sm_match, "delivered ");
			strcat (smp->sm_match, name);
			strcat (smp->sm_match, " From ");
		} else {
			strcpy (smp->sm_file, name);
		}
	} else {

		/* use whatever was specified */
		if ((p = strrchr(file, '/')) != NULL) {
			strcpy (smp->sm_file, ++p);
			strncpy (dir, file, p - file);
			dir[p-file] = '\0';
			chdir (dir);
		} else {
			strcpy (smp->sm_file, file);
		}
	}

	/* get the end point */
	smp->sm_pos = (stat(smp->sm_file, &buf) < 0)? 0 : buf.st_size;
}

/* get the next header line (if the file has changed) */
char *
smnext (smp, fp)
	struct scanmail *smp;
	FILE *fp;
{
	struct stat buf;
	static char line[128];
	int len;
	char *sp=NULL, *mp, *ep;
	FILE *f;
	char *strchr();

	/* return if it hasn't gotten longer */
	if (stat(smp->sm_file, &buf) < 0)
		return 0;
	if (buf.st_size <= smp->sm_pos) {
		smp->sm_pos = buf.st_size;
		return 0;
	}

	/* it's grown, read it line by line */
	f = (fp != NULL && smp->sm_log) ? fp : fopen (smp->sm_file, "r");
	fseek (f, smp->sm_pos, 0);
	len = strlen(smp->sm_match);
	while (fgets(line, sizeof line, f) != NULL) {

		/* remember how far we've read */
		smp->sm_pos += strlen (line);

		/* look for a match */
		if (strncmp(line, smp->sm_match, strlen(smp->sm_match)) == 0) {

			/* get the sender's name and machine */
			ep = mp = line+len;
			while (1) {
				sp = mp;
				mp = ep;
				if ((ep = strchr(ep, '!')) == NULL)
					break;
				ep++;
			}
			for (ep = sp; *ep != ' ' && *ep != '\0'; ep++)
				;
			*ep = '\0';
			break;
		}
	}
	if (fp == NULL || !smp->sm_log)
		fclose (f);
	return sp;
}
