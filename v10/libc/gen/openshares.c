/*LINTLIBRARY*/

/*
**	Open ``shares'' file and optionally lock it.
*/ 

#include	<shares.h>
#include	<sys/filio.h>
#include	<errno.h>
#include	<libc.h>


int		ShareFd;
static char *	ShareFile	= SHAREFILE;
static int	ShareOpen;



int
openshares(lock)
	int	lock;	/* True if writing desired */
{
	if ( ShareOpen )
		return 1;

	if ( (ShareFd = open(ShareFile, 2)) == SYSERROR )
	{
		if ( !lock && (errno == EACCES) )
		{
			if ( (ShareFd = open(ShareFile, 0)) == SYSERROR )
				return 0;
		}
		else
			return 0;
	}

#	ifdef	FIOCLEX
	(void)ioctl(ShareFd, FIOCLEX, 0);	/* close on exec */
#	endif

	ShareOpen = 1;
	errno = 0;

	return 1;
}


void
closeshares()
{
	if ( !ShareOpen )
		return;

	(void)close(ShareFd);

	ShareOpen = 0;
}


void
sharesfile(file)
	char *	file;
{
	closeshares();
	ShareFile = file;
}
