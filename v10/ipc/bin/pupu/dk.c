#include <sys/types.h>
#include <sys/stream.h>
#include	<sys/filio.h>
#include	<sys/ttyio.h>
#include	<stdio.h>
#include	"sysexits.h"
#include	<signal.h>
#include	"tdefs.h"
#include	"udefs.h"
#include	"pupu.h"

/*
 * To avoid race conditions, we remember if mesgld has been pushed.
 * This flag should become a table if there is ever a need for more
 * than one datakit fd open at a time
 */

static char msgpushed;

static unsigned char eofmsg[] = { M_CTL, MSGMAGIC, 1, 0, 106 };

dkeof(fd)
{
	write (fd, eofmsg, sizeof (eofmsg));
}

msgwrite (fd, buf, len)
	int fd;
	char *buf;
	int len;
{
	struct mesg m;
	static struct mesg delim = {M_DELIM, MSGMAGIC, 0, 0};
	int ret;

	if (!msgpushed) {
		extern int mesg_ld;

		if (ioctl (fd, FIOPUSHLD, &mesg_ld) < 0) {
			fprintf(stderr, "can't push message line discipline\n");
			exit(fd);
		}
		msgpushed = 1;
	}

	m.type = M_DATA;
	m.magic = MSGMAGIC;
	m.losize = len & 0xff;
	m.hisize = (len >> 8) & 0xff;

db(stderr, "msgwrite %d\n", len);
	if (write (fd, &m, MSGHLEN) != MSGHLEN) {
		perror ("header write");
		return -1;
	}

	ret = len? write (fd, buf, len): 0;

db(stderr, "msgwrite delim\n");
	if(write (fd, &delim, MSGHLEN) != MSGHLEN){
		perror ("delim write");
		return -1;
	}

	return ret;
}

msgread (fd, buf, len)
	int fd;
	char *buf;
	int len;
{
	struct mesg m;
	static int remain, tossdelim;
	int n, ret = 0;

	if (!msgpushed)
		return read (fd, buf, len);

	if (remain) {
		n = remain > len? len: remain;
		if (iread (fd, buf, n) != n)
			err ("Can't read trailer", NIL, -EX_PROTOCOL);
		remain -= n;
		return n;
	}

	while (len > 0) {

		do {
			if (iread (fd, &m, MSGHLEN) != MSGHLEN)
				err ("Stream msg header read error",
					NIL, -EX_PROTOCOL);
	
			if (m.magic != MSGMAGIC)
				err ("Bad msgmagic", NIL, -EX_PROTOCOL);

db(stderr, "msgread type %d len %d\n", m.type, (m.hisize << 8) + m.losize);
		} while (m.type != M_DATA && m.type != M_DELIM && m.type != M_CTL);

		if (m.type == M_DELIM) {
			if (!tossdelim)
				return ret;
			tossdelim = 0;
		} else {
			int r = (m.hisize << 8) + m.losize;

			if (m.type == M_CTL) {
				char c;

				if (r != 1)
					err ("Control message, cnt > 1", NIL,
						-EX_PROTOCOL);

				if (iread (fd, &c, r) != r)
					err ("Cannot read control char", NIL,
						-EX_PROTOCOL);

				if (c != 106)  /* should be symbolic */
					err ("Unknown control char", NIL,
						-EX_PROTOCOL);

				tossdelim = 0;
			} else {

				if(r)
					tossdelim = 0;
				if (r > len) {
					remain = r - len;
					r = len;
				}
		
				if (r && iread (fd, buf, r) != r)
					err ("Bad data read", NIL, -EX_PROTOCOL);

				buf += r;
				len -= r;
				ret += r;
			}
		}
	}

	tossdelim = 1;
	return ret;
}
