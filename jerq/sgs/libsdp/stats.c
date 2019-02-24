static char ID[] = "@(#) stats.c: 1.1 3/16/82";

#include <stdio.h>
#include "sdp.h"
#include "define3.h"

#ifdef SHORT
#define ST01	"ST01 "
#define STAU	"STAU "
#else
#define ST01	"ST01 -- attempt to take statistics on an unhoused enviroment\n%o\n",environ
#define STAU	"%s","STAU -- audit failed in sdp_stats\n"
#endif

#ifdef SWEET
#define PROLOG
#define EPILOG
#else
#define PROLOG	if(aud_flag == AUDITON)\
			aud_set() ;
#define EPILOG	if( (aud_flag == AUDITON) && (aud_check() == ERROR) )\
		{\
			stats.uses =   ERROR ;\
			stats.faults = ERROR ;\
			stats.writes = ERROR ;\
			stats.maxuse = ERROR ;\
			ERR_RET(STAU,stats) ;  \
		}\
		if(hist_fp != NULL)\
			fprintf(hist_fp,"ST\t%o\t%ld\t%ld\t%ld\t%d\n",environ,stats.uses,stats.faults,stats.writes,stats.maxuse) ;
#endif

extern int aud_flag ;
extern FILE *hist_fp ;
extern struct ENVIRON *first_environ ;
extern FATAL() ;
extern struct LIBFILE *Glib_file ;

struct STATS					/* contains statistics */
sdp_stats(environ)
struct ENVIRON *environ ;
{
	register struct ENVIRON	*curr ;
	register struct ENVIRON	*next ;
	struct STATS		stats ;

	PROLOG
	stats.uses   = 0l ;
	stats.faults = 0l ;
	stats.writes = 0l ;
	stats.maxuse = 0 ;

	for(curr=first_environ; curr!=NULL; curr=next)
	{
		next = curr->next_environ ;
		if( (curr==environ) || (environ==NULL) )
		{
			stats.uses   += (curr->stats).uses ;
			stats.faults += (curr->stats).faults ;
			stats.writes += (curr->stats).writes ;
			stats.maxuse += (curr->stats).maxuse ;
			stats.curuse += (curr->stats).curuse ;

			if(environ == curr)
				break ;
		}
	}

	if( (curr == NULL) && (environ != NULL) )
	{
		stats.uses   = ERROR ;
		stats.faults = ERROR ;
		stats.writes = ERROR ;
		stats.maxuse = ERROR ;
		ERR_RET(ST01,stats)
	}

	EPILOG
	return(stats) ;
}
