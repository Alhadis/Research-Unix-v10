#include <stdio.h>
#include <pwd.h>
struct passwd pwstruct;
struct passwd *pw = &pwstruct;
struct passwd *getpwent();

main(argc, argv)
char **argv;
{
	char cfile[100], cname[100];
	if(argc > 1)
		process(argv[1], "");
	else while(pw=getpwent()) {
		strcpy(cname, pw->pw_name);
		strcpy(cfile, pw->pw_dir);
		strcat(cfile, "/calendar");
		process(cfile, cname);
	}
	return(0);
}

process(file, name)
char *file, *name;
{
	char s[201];
	FILE *f;
	if(access(file,04)==0 && access(file,01)==-1)
		printf("!%s %s\n",file,name);
	f = fopen(file,"r");
	if(f!=NULL) {
		while(fgets(s,200,f))
			if(strncmp("#include",s,8)==0)
				include(s, name);
		fclose(f);
	}
}

include(s, name)
char *s, *name;
{
	char *t;
	for(t=s; *t; t++)
		continue;
	for(;;) {
		switch(t[-1]) {
		case '\n':
		case '"':
			*--t = 0;
			continue;
		}
		break;
	}
	for(t=s+8;;t++) {
		switch(*t) {
		case ' ':
		case '\t':
		case '"':
			continue;
		}
		break;
	}
	if(access(t,04)==0 && access(t,01)==-1)
		printf("!%s %s\n",t,name);
	else if(index(t,'!')!=0)
		printf("%s %s\n",t,name);
}

