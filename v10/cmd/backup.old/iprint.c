#include	<sys/types.h>
#include	<sys/stat.h>
#include	<utsname.h>
#include	<fio.h>
#include	<libc.h>
#include	<errno.h>

main()
{
	struct stat buf;
	register char *name;
	int ret;
	struct utsname mach;
	register char *last;
	char me[256];

	uname(&mach);
	sprint(me, "/n/%s", mach.nodename);
	while(name = Frdline(0)){
		if(*name == 0) continue;
		last = name + FIOLINELEN(0)-1;
		if((name[0] == '/') && (name[1] == '/'))
			ret = stat(++name, &buf);
		else
			ret = lstat(name, &buf);
		if(ret < 0){
			if(errno != ENOENT)	/* don't bitch about missing files */
				perror(name);
			continue;
		}
		if(*last == '/') *last = 0;
		printf("%s%s//%d %d\n", strncmp(name, "/n/", 3)?me:"", name,
			buf.st_ctime, buf.st_size+1024);
	}
	exit(0);
}
