/* usage: connect hostname (answer|send|notice) (in|out) program [ args ... ] */
/* TODO: should librarify the bit for opening an rsh-authenticated connection.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include "../paths.h"

int
main(int argc, char *argv[])
{
	int fd;
	struct hostent *host;
	struct servent *serv;
	struct passwd *pw;
	int lport;
	struct sockaddr_in sin;
	char byte;

	if (argc < 5)
		exit(1);

	host = gethostbyname(argv[1]);
	if (!host) {
		fprintf(stderr, "connect: can't get host '%s'\n", argv[1]);
		exit(1);
	}

	serv = getservbyname("dist", "tcp");
	if (!serv) {
		fprintf(stderr, "connect: can't get 'dist' port number\n");
		exit(1);
	}

	pw = getpwuid(getuid());	/* we assume unique uid/name map */
	if (!pw) {
		fprintf(stderr, "connect: can't get user name\n");
		exit(1);
	}

	chdir(SDIR);

	lport = 1023;
	fd = rresvport(&lport);
	if (fd < 0) {
		perror("connect: getting socket");
		exit(1);
	}

	sin.sin_family = host->h_addrtype;
	memcpy(&sin.sin_addr, host->h_addr, host->h_length);
	sin.sin_port = serv->s_port;
	if (connect(fd, &sin, sizeof sin) < 0) {
		perror("connect: opening connection");
		exit(1);
	}

	write(fd, "", 1);	/* a NUL byte */
	write(fd, pw->pw_name, strlen(pw->pw_name) + 1);
	write(fd, pw->pw_name, strlen(pw->pw_name) + 1);
	if (read(fd, &byte, 1) != 1 || byte != '\0') {
		fprintf(stderr, "connect: rsh authentication failed\n");
		exit(1);
	}

	write(fd, argv[2], strlen(argv[2]) + 1);

	if (strchr(argv[3], 'i'))
		dup2(fd, 0);
	if (strchr(argv[3], 'o'))
		dup2(fd, 1);
	close(fd);
	execv(argv[4], &argv[4]);
}
