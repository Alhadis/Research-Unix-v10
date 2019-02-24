/*
 *	setup a datakit manager process     
 *	and receive requests for its service.
 */

#include <dk.h>
#include <dkmgr.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <errno.h>

char	dkmgribuf[128] ;
int	dkmgropen = 0 ;
int	dkmgrreply = 0 ;
int	ntries = 0;
struct 	mgrmsg	dkmgrmsg ;
extern int	dkp_ld ;
extern int	rdk_ld ;

#define MAXRETRIES 2

struct mgrmsg *
dkmgr(mgrname, traffic)
char * mgrname ;
{
	extern char *dkerror;
	register fd ;
	extern int errno ;
	register char *cp ;
	char *savcp ;

	ntries = 0;
again:
	if (dkmgrreply <= 0) {
		dkmgrreply = dkctlchan(0);
 		if (dkmgrreply < 0) {
			fprintf(stderr, "MGR %s: cannot open channel 1\r\n",
			    mgrname) ;
			if (ntries==0)
				exit(1);
			nwait(ntries);
			ntries++;
			goto again;
		}
	}
	if ((fd = dkmgropen) == 0) {
		fd = tdkserv(mgrname, traffic) ;
		if (fd < 0) {
			fprintf(stderr, "dkmgr: can't create server %s (%s)\r\n",
			     mgrname, dkerror) ;
			nwait(ntries);
			if (ntries++ > MAXRETRIES)
				exit (2);
			goto again ;
		}
		dkmgropen = fd ;
		fprintf(stderr, "MGR %s:  successful initialization\r\n",
		     mgrname) ;
		dkproto(fd, dkp_ld);
		ntries = 0;
		return ((struct mgrmsg *)0);
	}
	errno = 0 ;
	if (read(fd, dkmgribuf, 128) <= 0) {
		if (errno == EINTR)
			return NULL ;
		close(fd) ;		/* a host restart */
		dkmgropen = 0 ;
		close(dkmgrreply);
		dkmgrreply = 0;
		fprintf(stderr, "MGR %s:  dk-eof,  restarting\r\n",
		     mgrname) ;
		return ((struct mgrmsg *)0);
	}
	cp = dkmgribuf ;
	dkmgrmsg.m_chan = 0 ;
	while (*cp >= '0' && *cp <= '7')
		dkmgrmsg.m_chan = (dkmgrmsg.m_chan << 3) + (*cp++ & 07) ;
	dkmgrmsg.m_tstamp = 0 ;
	if (*cp == '.') {
		cp++ ;
		while (*cp >= '0' && *cp <= '7')
			dkmgrmsg.m_tstamp =
			       (dkmgrmsg.m_tstamp << 3) + (*cp++ & 07) ;
	}
	dkmgrmsg.m_traffic = 0 ;
	if (*cp == '.') {
		cp++ ;
		while (*cp >= '0' && *cp <= '7')
			dkmgrmsg.m_traffic =
			       (dkmgrmsg.m_traffic << 3) + (*cp++ & 07) ;
	}
	if (*cp++ != '\n') {
		fprintf(stderr, "mgr %s: bad incall message = %s\r\n", mgrname,
		    dkmgribuf) ;
		goto again ;
	}
	dkmgrmsg.m_dial = cp ;
	dkmgrmsg.m_service = NULL ;
	while (*cp != '\n' && *cp != '\0' && *cp != '.')
		cp++ ;
	if (*cp == '.') {
		*cp++ = '\0' ;
		dkmgrmsg.m_service = cp ;
		while (*cp != '\n' && *cp != '\0') cp++ ;
	}
	if (*cp == '\n')
		*cp++ = '\0' ;
	dkmgrmsg.m_traffic = 0 ;
	dkmgrmsg.m_param1 = NULL ;
	dkmgrmsg.m_param2 = NULL ;
	dkmgrmsg.m_uid = NULL ;
	dkmgrmsg.m_source = NULL ;
	if (*cp == '\0')
		return (&dkmgrmsg) ;
	dkmgrmsg.m_source = cp ;
	while (*cp != '.' && *cp != '\0' && *cp != '\n') cp++ ;
	if (*cp == '.') {
		*cp++ = '\0' ;
		dkmgrmsg.m_uid = cp ;
	}
	while (*cp != '.' && *cp != '\0' && *cp != '\n')  cp++ ;
	if (*cp == '.') {
		*cp++ = '\0' ;
		dkmgrmsg.m_param1 = cp ;
	}
	while (*cp != '.' && *cp != '\0' && *cp != '\n')  cp++ ;
	if (*cp == '.') {
		*cp++ = '\0' ;
		dkmgrmsg.m_param2 = cp ;
	}
	while (*cp != '.' && *cp != '\0' && *cp != '\n')  cp++ ;
	*cp++ = '\0' ;
	return (&dkmgrmsg) ;
}

dkmgrack(chan)
{
struct listenin d ;

	d.l_lchan = 1 ;
	d.l_type = T_REPLY ;
	d.l_srv = D_OPEN ;
	d.l_param0 = chan ;
	d.l_param1 = 0 ;
	d.l_param2 = chan ;
	d.l_param3 = dkmgrmsg.m_tstamp ;
	d.l_param4 = 0 ;
	d.l_param5 = 0 ;
	write(dkmgrreply, &d, sizeof(d)) ;
}


dkmgrnak(chan, error)
{
struct listenin d ;

	d.l_lchan = 1 ;
	d.l_type = T_REPLY ;
	d.l_srv = D_FAIL ;
	d.l_param0 = chan ;
	d.l_param1 = error ;
	d.l_param2 = chan ;
	d.l_param3 = dkmgrmsg.m_tstamp ;
	d.l_param4 = 0 ;
	d.l_param5 = 0 ;
	write(dkmgrreply, &d, sizeof(d)) ;
}

nwait(ntries)
{
	sleep(ntries<10?5:(ntries<20?30:600)) ;
}
