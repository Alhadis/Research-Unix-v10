#include <stdio.h>
#include <sys/types.h>
#include <ndir.h>
#include <errno.h>

int fflag;
int iflag;
int rflag;
int qflag;
int errcnt;

extern char *strcpy(), *malloc(), *parent(), *child();

main(argc, argv)
char **argv;
{
	for( ; argc>1 && *argv[1]=='-'; argc--, argv++) {
		for(;;) {
			switch(*++argv[1]) {
			case 'f':
				fflag++;
				continue;
			case 'i':
				iflag++;
				continue;
			case 'r':
				rflag++;
				continue;
			default:
				usage();
			case 0: ;
			}
			break;
		}
	}
	if(argc <= 1)
		usage();
	qflag = !fflag && !iflag && isatty(0);
	while(--argc>0)
		rm(*++argv);
	return errcnt;
}

usage()
{

	fprintf(stderr, "usage: rm [-fri] file ...\n");
	exit(1);
}

/* To avoid unnecessarily raising the process label in secure unix,
   rm() and recurse() are coded without stat().  Except when 
   questions are asked, it takes between 1 and 6, typically 3,
   system calls to remove a file.
*/
rm(s)
register char *s;
{
	if(strcmp(s, "..") == 0) {
		errno = EINVAL;
		error(s);
		return;
	}
	if(iflag && access(parent(s),2)==0 && !query("%s: ", s) ||
	   qflag && access(s,2)==-1 &&
	   access(s,0)==0 && access(parent(s),2)==0 &&
	   !query("rm: %s unwritable; remove? ", s))
		return;
	/* Try rmdir first, to guard against superuser process
	   doing unlink on a directory, which would be bad.
	   Race conditions in which a directory might be unlinked:
	   (1) rmdir gives EBUSY; other proc cd's out before unlink
	   (2) rmdir gives EACCES; other proc changes parent's mode
	   (3) rmdir gives EPERM; other proc changes mode along path
	*/
	if(rmdir(s) == 0)
		return;
	switch(errno) {
	case EBUSY:	/* text file or working directory */
	case EPERM:
	case EACCES:	/* parent(s) unwritable; s may not be dir */
		if(access(child(s),0) == 0)	/* s searchable? */
			goto case_ISDIR;
	case ENOTDIR:
		if(unlink(s) == 0)
			return;
		break;
	case_ISDIR:
	case EHASF:
	case EINVAL:	/* file name "." */
		if(rflag) {
			recurse(s);
			return;
		}
	}
	error(s);
}

recurse(s)
char *s;
{
	register struct direct *entryp;
	register DIR *dirp = opendir(s);
	char *path;

	if(dirp == 0) {
		error(s);
		return;
	}
	path = malloc(strlen(s)+MAXNAMLEN+2);
	while(entryp = readdir(dirp)) {
		char *ep = entryp->d_name;
		if(strcmp(ep,".")==0 || strcmp(ep,"..")==0)
			continue;
		sprintf(path, "%s/%s", s, ep);
		rm(path);
	}
	free(path);
	closedir(dirp);
	if(rmdir(s) == -1)
		error(s);
}

error(s)
char *s;
{
	if(fflag) return;
	fprintf(stderr, "rm: ");
	perror(s);
	errcnt++;
}

query(f, s)
char *f, *s;
{
	int c, reply;

	printf(f, s);
	reply = c = getchar();
	while(c != '\n') {
		if(c == EOF)
			return 0;
		c = getchar();
	}
	return reply == 'y';
}

char *
parent(s)
register char *s;
{
	register n;
	static char *p;
	register char *t = s;
	
	while(*t)
		t++;
	while(t>s && t[-1]=='/')
		t--;
	while(t>s && t[-1]!='/')
		t--;
	if(t == s)
		return ".";
	while(t>s && t[-1]=='/')
		t--;
	if(t == s)
		return "/";
	n = t - s;
	if(p) free(p);
	p = malloc(n + 1);
	strncpy(p, s, n);
	p[n] = 0;
	return p;
}

char *
child(s)
char *s;
{
	static char *p;
	if(p) free(p);
	p = malloc(strlen(s)+MAXNAMLEN+2);
	strcat(strcpy(p, s), "/.");
	return p;
}
