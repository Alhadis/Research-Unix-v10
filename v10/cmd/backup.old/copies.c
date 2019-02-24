#include	"backup.h"
#include	<fio.h>

char bdir[FNAMELEN];

main()
{
	char *s;

	backupdir(bdir);
	while(s = Frdline(0))
		process(s);
	exit(0);
}

process(s)
	char *s;
{
	char buf[512];
	int fd;
	struct backfile h;
	struct stat sb;

	if((fd = open(s, 0)) < 0){
		perror(s);
		return;
	}
	if(fstat(fd, &sb) < 0)
		goto err;
	if((sb.st_mode&0777) != 0400){
		close(fd);
		return;
	}
	if(read(fd, (char *)&h, sizeof h) != sizeof h){
err:
		perror(s);
		close(fd);
		return;
	}
	Fprint(1, "%s\t%s/%s\n", s, bdir, h.bname);
	close(fd);
}
