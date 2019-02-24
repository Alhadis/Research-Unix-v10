/*
*     static char ID_mn[] = "@(#) tstc.c: 1.2 3/17/82";
*/

/*	main program for the compress utility is used as a bridge
 *	between the 3 main functions: frstpass, build, and secpass.
 */


#include <stdio.h>
#include "sdp.h"
#include "sdp1.h"
#include <signal.h>
#include "filehdr.h"
#include "syms.h"
#include "ldfcn.h"

long 	tag_sum = 0;
int	vflag = 0;
int	pflag = 0;
extern  struct ENVIRON *ENV;


main(argc, argv)

int	argc;
char	*argv[];
{
	extern int	frstpass();
	extern int	secpass();
	extern int	build();
	extern int	tagfini(),
			symfini();
	extern long	delsum;
	char		*c;
	int		nargs;

	nargs = argc;
	while (--argc > 0 && **++argv == '-' ) {
		--nargs;
		for ( c =argv[0]+1; *c != '\0'; c++)
				switch(*c) {
					case 'v': vflag++; break;
					case 'p': pflag++; break;
					default:
						fprintf(stderr, "invalid option '%c' ignored \n", *c);
						break;
				}  /* end of switch  */
	}  /* end of othermost while  */

	if ( nargs < 3 ) {
		fprintf(stderr,"usage: cprs [-v] [-p] infile outfile\n");
		exit (1);
	}

	if(frstpass(*argv) != SUCCESS) { 
		fprintf(stderr, "error condition: no compression\n");
		exit (1);
	}
	if ( pflag > 0)
		printf("number of tags read %ld\n",tag_sum);
	if ( delsum == 0) {
		if ( pflag > 0)
			fprintf(stderr, "no duplicate tags\n");
		if ( cpyfile(argv[0], argv[1]) != SUCCESS)
			fprintf(stderr,"error in copying file\n");
		kill_sdp();
		exit (2);
	}
	if ( build() != SUCCESS) {
		if ( vflag > 0)
			fprintf(stderr,"unable to build table for compression\n");
		exit (3);
	}
	kill_sdp();
	if(secpass(argv[0], argv[1]) != SUCCESS) {
		fprintf(stderr,"couldn't compress\n");
	}

	exit(0);
}

kill_sdp()
{
	if (tagfini() != SUCCESS || symfini() != SUCCESS)
		return(FAILURE);
	else
		{
		if (sdp_vacate(ENV) == SDPERROR)
			{
			fprintf(stderr,"Failed to close SDP");
			return(FAILURE);
			}
		return(SUCCESS);
		}
}
