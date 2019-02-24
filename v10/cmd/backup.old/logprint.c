#include	"backup.h"
#include	<libc.h>

static int connect();
char *doprint();

logprint(fmt, args)
	char *fmt;
{
	char buf[4096], *out;
	long t;
	int ifd, ofd;
	char *ctime();

	if(connect(&ifd, &ofd))
		return;
	(void)time(&t);
	out = ctime(&t);
	out[strlen(out)-1] = 0;
	out = doprint(buf, "%s ", (char *)&out);
	out = doprint(out, fmt, (char *)&args);
	if(write(ofd, buf, (int)(out-buf)) != out-buf)
		perror("write");
	buf[0] = 'X';
	switch(read(ifd, buf, 1))
	{
	case -1:
		perror("read");
		break;
	case 1:
		if(buf[0] != 'O')
			fprint(2, "unexpected return '%c' (0%o) from backup logger\n", buf[0], buf[0]&0xFF);
		break;
	case 0:
		fprint(2, "unexpected eof from backup logger\n");
		break;
	}
	close(ifd);
	close(ofd);
}

static
connect(ifd, ofd)
	int *ifd, *ofd;
{
	char bdir[FNAMELEN], *b;

	backupdir(b = bdir);
	while(*b++)
		;
	return(rx(b, "backuplog", "bkplog", "light", ifd, ofd));
}
