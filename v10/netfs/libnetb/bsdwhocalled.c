/*
 * 4BSD sockets (only) who called me
 */

char *_rfbsdpeer();

char *
_rfwhocalled(cfd)
int cfd;
{
	static char buf[100];	/* arbitrary */
	char *p;

	if ((p = _rfbsdpeer(cfd)) != 0)
		strcpy(buf, p);
	return (buf);
}
