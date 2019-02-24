#include <ctype.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libc.h>

int mflag, aflag;
extern int errno;
extern char *sys_errlist[];
extern long timec();

main(argc, argv)
char **argv;
{
	char s[50];
	int bad = 0;
	long date;
	struct stat statb;
	time_t timeb[2];
	while(argv[1][0] == '-') {
		while(*(++argv[1])) {
			switch(argv[1][0]) {
			case 'm':
			case 'u':
				mflag++;
				break;
			case 'a':
				aflag++;
				break;
			}
		}
		argc--;
		argv++;
	}
	if(mflag==0 && aflag==0)
		mflag = aflag = 1;
	strcpy(s, argv[1]);
	strcat(s, " ");
	while(argc>2 && (isdigit(*argv[2])||*argv[2]=='-')) {
		strcat(s, argv[2]);
		strcat(s, " ");
		argc--;
		argv++;
	}
	date = timec(s);
	for( ; argc>2; argc--,argv++) {
		if(stat(argv[2],&statb) < 0) {
			fprintf(stderr,"chdate: %s: %s\n",
				argv[2],sys_errlist[errno]);
			bad++;
			continue;
		}
		timeb[0] = statb.st_atime;
		timeb[1] = statb.st_mtime;
		if(aflag) 
			timeb[0] = date;
		if(mflag)
			timeb[1] = date;
		if(utime(argv[2],timeb) < 0) {
			fprintf(stderr,"chdate: %s: %s\n",
				argv[2],sys_errlist[errno]);
			bad++;
		}
	}
	return bad;
}
