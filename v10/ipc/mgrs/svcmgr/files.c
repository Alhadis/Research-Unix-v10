/*
 * read in the control files;
 * deal with the tables
 */

#include "mgr.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <setjmp.h>

typedef int *(*PTOI)();
struct afile {
	char *file;
	int *(*new)();
	int (*add)();
	long last;
} files[] = {
 {"/usr/ipc/lib/serv.local",	(PTOI)newservice, addservice},
 {"/usr/ipc/lib/serv", 		(PTOI)newservice, addservice},
 {"/usr/ipc/lib/auth.local", 	(PTOI)newmap, addmap},
 {"/usr/ipc/lib/auth", 		(PTOI)newmap, addmap},
 {NULL}
};

/*
 * stuff concerning the service tables
 */
statfiles()
{
	register int i;
	struct stat st;

	for (i = 0; files[i].file; i++)
		if (stat(files[i].file, &st) >= 0)
			files[i].last = st.st_mtime;
}

checkfiles()
{
	register int i;
	struct stat st;

	for(i=0; files[i].file; i++)
		if (stat(files[i].file, &st) >= 0
		&&  st.st_mtime > files[i].last)
			return (1);
	return (0);
}

readfiles()
{
	FILE *fp;
	int i;
	struct stat st;
	char	buf[64];

	resetmaps();
	for(i=0; files[i].file; i++) {
		if((fp=fopen(files[i].file, "r"))==NULL)
			continue;
		stat(files[i].file, &st);
		files[i].last = st.st_mtime;
		readfile(fp, &files[i]);
		fclose(fp);
	}
	strcpy(buf, "readfiles	readfiles");
	addservice(newservice(buf));
	startsvcs();
}

/*
 *  Read and parse a file
 */
readfile(fp, ap)
	FILE *fp;
	struct afile *ap;
{
	char buf[256];
	char *cp;
	int *op;

	while(fgets(buf, sizeof(buf), fp)!=NULL) {
		cp = strchr(buf, '\n');
		if(cp==NULL)
			logevent("%s: line too long\n", ap->file);
		else
			*cp = '\0';
		if(buf[0]=='#')
			continue;	/* comment */
		op = (*(ap->new))(buf);
		if(op!=NULL)
			(*(ap->add))(op);
	}
}
