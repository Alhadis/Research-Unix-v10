#include	"backup.h"
#include	<libc.h>

backupdir(buf)
	char *buf;
{
	int fd, n;
	char *s, *strchr();

	if((fd = open(BACKUP, 0)) < 0){
		perror(BACKUP);
		exit(2);
	}
	if((n = read(fd, buf, FNAMELEN)) <= 0){
		fprint(2, "read error in %s\n", BDBM);
		exit(2);
	}
	buf[n+1] = 0;
	buf[n] = 0;
	buf[n-1] = 0;	/* ensure three strings */
	if(s = strchr(buf, '\n')){
		*s++ = 0;
		if(s = strchr(s, '\n'))
			*s = 0;
	}
	close(fd);
}
