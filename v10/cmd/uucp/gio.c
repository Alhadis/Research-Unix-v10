/*	/sccs/src/cmd/uucp/s.gio.c
	gio.c	1.2	8/30/84 17:37:24
*/
#include "uucp.h"
VERSION(@(#)gio.c	1.2);

#include "pk.h"

jmp_buf Gfailbuf;
struct pack *Pk;

pkfail()
{
	longjmp(Gfailbuf, 1);
}

gturnon()
{
	struct pack *pkopen();
	if (setjmp(Gfailbuf))
		return(FAIL);
	if (Debug > 4)
		pkdebug = 1;
	Pk = pkopen(Ifn, Ofn);
	if ((int) Pk == NULL)
		return(FAIL);
	return(0);
}


gturnoff()
{
	if(setjmp(Gfailbuf))
		return(FAIL);
	pkclose(Pk);
	return(0);
}


gwrmsg(type, str, fn)
char type, *str;
{
	char bufr[BUFSIZ], *s;
	int len, i;

	if(setjmp(Gfailbuf))
		return(FAIL);
	bufr[0] = type;
	s = &bufr[1];
	while (*str)
		*s++ = *str++;
	*s = '\0';
	if (*(--s) == '\n')
		*s = '\0';
	len = strlen(bufr) + 1;
	if ((i = len % PACKSIZE)) {
		len = len + PACKSIZE - i;
		bufr[len - 1] = '\0';
	}
	gwrblk(bufr, len, fn);
	return(0);
}


/*ARGSUSED*/
grdmsg(str, fn)
char *str;
{
	unsigned len;

	if(setjmp(Gfailbuf))
		return(FAIL);
	for (;;) {
		len = pkread(Pk, str, PACKSIZE);
		if (len == 0)
			continue;
		str += len;
		if (*(str - 1) == '\0')
			break;
	}
	return(0);
}


gwrdata(fp1, fn)
FILE *fp1;
{
	char bufr[BUFSIZ];
	int len;
	int ret;
	time_t ticks;
	long bytes;
	char text[BUFSIZ];

	if(setjmp(Gfailbuf))
		return(FAIL);
	bytes = 0L;
	(void) millitick();	/* set msec timer */
	while ((len = fread(bufr, sizeof (char), BUFSIZ, fp1)) > 0) {
		bytes += len;
		ret = gwrblk(bufr, len, fn);
		if (ret != len) {
			return(FAIL);
		}
		if (len != BUFSIZ)
			break;
	}
	ret = gwrblk(bufr, 0, fn);
	ticks = millitick();
	(void) sprintf(text, "-> %ld / %ld.%.3d secs", bytes, ticks / 1000,
		ticks % 1000);
	DEBUG(4, "%s\n", text);
	syslog(text);
	return(0);
}


grddata(fn, fp2)
FILE *fp2;
{
	int len;
	char bufr[BUFSIZ];
	time_t ticks;
	long bytes;
	char text[BUFSIZ];

	if(setjmp(Gfailbuf))
		return(FAIL);
	bytes = 0L;
	(void) millitick();	/* set msec timer */
	for (;;) {
		len = grdblk(bufr, BUFSIZ, fn);
		if (len < 0) {
			return(FAIL);
		}
		bytes += len;
		if (fwrite(bufr, sizeof (char), len, fp2) != len)
			return(FAIL);
		if (len < BUFSIZ)
			break;
	}
	ticks = millitick();
	(void) sprintf(text, "<- %ld / %ld.%.3d secs",
		bytes, ticks / 1000, ticks % 1000);
	DEBUG(4, "%s\n", text);
	syslog(text);
	return(0);
}


/*ARGSUSED*/
grdblk(blk, len,  fn)
char *blk;
{
	int i, ret;

	for (i = 0; i < len; i += ret) {
		ret = pkread(Pk, blk, len - i);
		if (ret < 0)
			return(FAIL);
		blk += ret;
		if (ret == 0)
			return(i);
	}
	return(i);
}


/*ARGSUSED*/
gwrblk(blk, len, fn)
char *blk;
{
	int ret;

	ret = pkwrite(Pk, blk, len);
	return(ret);
}
