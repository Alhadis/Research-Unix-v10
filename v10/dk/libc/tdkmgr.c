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
extern	int dkp_ld ;
extern	int rdk_ld ;



struct mgrmsg *
dkmgr(mgrname, traffic)
char * mgrname ;
{
	register fd ;
	extern int errno ;
	register char *cp ;
	char *savcp ;
	int i;

	ntries = 0;
again:
	if (dkmgrreply <= 0) {
		dkmgrreply = dkctlchan(traffic) ;
 		if (dkmgrreply < 0) {
			fprintf(stderr, "MGR %s: cannot open control chan\n",
			    mgrname) ;
			if (ntries==0)
				exit(1);
			nwait(ntries);
			ntries++;
			goto again;
		}
	}
	if ((fd = dkmgropen) == 0) {
		extern char *dkerror;
		fd = tdkserv(mgrname, traffic) ;
		if (fd < 0) {
			fprintf(stderr, "dkmgr: can't create server %s (%s)\n",
			     mgrname, dkerror) ;
			nwait(ntries);
			ntries++;
			goto again ;
		}
		dkmgropen = fd ;
		fprintf(stderr, "MGR %s:  successful initialization\r\n",
		     mgrname) ;
		dkproto(fd, dkp_ld);
		ntries = 0;
	}
	errno = 0 ;
	if ((i= read(fd, dkmgribuf, 128)) <= 0) {
		if (errno == EINTR)
			return NULL ;
		close(fd) ;		/* a host restart */
		dkmgropen = 0 ;
		close(dkmgrreply);
		dkmgrreply = 0;
		fprintf(stderr, "MGR %s:  dk-eof,  restarting\r\n",
		     mgrname) ;
		ntries = 1;
		sleep(30);
		goto again ;
	}
	dkmgribuf[i] = '\0';
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
		fprintf(stderr, "bad char %o: ", cp[-1]);
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
	dkmgrmsg.m_param1 = NULL ;
	dkmgrmsg.m_param2 = NULL ;
	dkmgrmsg.m_uid = NULL ;
	dkmgrmsg.m_source = NULL ;
	dkmgrmsg.m_origin = NULL ;
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
	if (*cp == '\n') {	/* 3-liner from generic 2 VCS node */
		dkmgrmsg.m_param2 = dkmgrmsg.m_param1 ;
		dkmgrmsg.m_param1 = dkmgrmsg.m_uid ;
		dkmgrmsg.m_uid = dkmgrmsg.m_source ;
		*cp++ = '\0' ;
		dkmgrmsg.m_source = cp ;
		while (*cp != '.' && *cp != '\0' && *cp != '\n')  cp++ ;
		if (*cp == '.') {
			*cp++ = 0 ;
			dkmgrmsg.m_origin = cp ;
			while (*cp != '.' && *cp != '\0' && *cp != '\n')  cp++ ;
			if (*cp == '.')  cp++ ;		/* end of node name */
			while (*cp != '.' && *cp != '\0' && *cp != '\n')  cp++ ;
			if (*cp == '.')  cp++ ;		/* end of mod number */
			while (*cp != '.' && *cp != '\0' && *cp != '\n')  cp++ ;
							/* end of chan number */
		}
	}
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
