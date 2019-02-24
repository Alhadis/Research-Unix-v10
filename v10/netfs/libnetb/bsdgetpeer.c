/*
 * who called me over a socket, 4BSD-style
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

char *
_rfbsdpeer(f)
int f;
{
	register struct hostent *hp;
	int len;
	struct sockaddr_in s;

	/*
	 * sleaze away, as UCB code usually does:
	 * assume it's internet family in various places
	 */
	len = sizeof(s);
	if (getpeername(f, &s, &len) < 0)
		return (0);
	if ((hp = gethostbyaddr(&s.sin_addr, sizeof(struct in_addr), s.sin_family)) == 0)
		return (0);
	return (hp->h_name);
}
