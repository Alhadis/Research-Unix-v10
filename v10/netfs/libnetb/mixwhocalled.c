/* 
 * who called me;
 * 4BSD socket-using machines
 * that also run research connection server
 */

#define	NULL	0

char *_rfcallsource();
char *_rfbsdpeer();

char *
_rfwhocalled(cfd)
int cfd;
{
	static char buf[100];	/* arbitrary */
	char *p;

	if ((p = _rfcallsource()) != NULL)
		return (p);
	if ((p = _rfbsdpeer(cfd)) != NULL) {
		strcpy(buf, "tcp!");
		strcat(buf, p);
		return (buf);
	}
	return (NULL);
}
