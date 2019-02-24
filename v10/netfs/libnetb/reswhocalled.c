/* 
 * who called me;
 * research connection server (only) version
 */

char *_rfcallsource();

char *
_rfwhocalled(cfd)
int cfd;
{

	return (_rfcallsource());
}
