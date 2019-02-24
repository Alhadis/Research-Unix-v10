	static char	SCCSID[] = "@(#)dkxenviron.c	2.1 DKHOST 84/08/10";

#include <stdio.h>
#include "string.h"

	char		*getenv();

/*
 * Transmit environment variables to a remote host
 *
 * The environment variables to be sent are listed, separated by commas,
 * in the environment variable DKEXPORT.  They are all sent as a series of
 * null-terminated strings using the dkxqt protocol.
 *
 *	netfd	= file descriptor of an open connection to the remote host
 *	return	= >0 : write successful
 *		| <0 : write failed
 */

dkxenviron(netfd)
{
	register char	*envlist, *envnext, *ap, *ep;
	short		len;
	char		envarray[2048];

	if(!(envlist = getenv("DKEXPORT")))
		envlist = "";

	ap = envarray;

	do{
		if(envnext = strchr(envlist, ','))
			*envnext = '\0';

		if(ep = getenv(envlist)){
			sprintf(ap, "%s=%s", envlist, ep);

			ap += strlen(ap) + 1;
		}

		if(envnext)
			*envnext++ = ',';
	}while(envlist = envnext);

	len = ap - envarray;

	return(dkxlwrite(netfd, envarray, len));
}
