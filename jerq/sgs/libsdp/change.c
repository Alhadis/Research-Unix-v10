static char ID[] = "@(#) change.c: 1.1 3/16/82";
#include <stdio.h>
#include "sdp.h"
#include "define3.h"

#ifdef SHORT
#define CH01	"CH01 "
#define CH02	"CH02 "
#define CHAU	"CHAU "
#else
#define CH01	"CH01 -- no permission to change\n%o\n",i->manp[j].space
#define CH02	"CH02 -- frame not found\n%o\n",ptr
#define CHAU	"%s","CHAU -- audit failed in sdp_change\n"
#endif

#ifdef SWEET
#define PROLOG
#define EPILOG
#else
#define PROLOG		if( (aud_flag == AUDITON) && (aud_check() == ERROR) )\
				ERR_RET(CHAU,ERROR)
#define EPILOG		if(aud_flag == AUDITON)\
				aud_set() ;\
			if(hist_fp != NULL)\
				fprintf(hist_fp,"CH\t%o\t%ld\t%o\n",(i->manp)[j].space,(i->manp)[j].page_id,ptr) ;
#endif

extern int aud_flag ;
extern FILE *hist_fp ;
extern struct ENVIRON *first_environ ;
extern struct LIBFILE *Glib_file ;

extern aud_set() ;
extern int aud_check() ;
extern fatal() ;

int					/* SUCCESS or ERROR */
sdp_change(ptr)
char	*ptr ;
{
	register struct ENVIRON	*i ;
	register int		j ;
	register unsigned	f ;
	register unsigned	p ;
	register int		status ;

	PROLOG

	p = (unsigned)ptr ;
	status = 0 ;
	for(i=first_environ; i!=NULL; i=i->next_environ)
	{
		for(j=0; j<i->num_frames; j++)
		{
			f = (unsigned)(i->manp)[j].frame_pntr ;
			if ( f <= p  &&  p < f + (unsigned)i->frame_size )
			{
				if(i->manp[j].space->perms != WRTN)
					ERR_RET(CH01,ERROR)
				i->manp[j].ref_chng = WRTN ;
				status = 1 ;
				break ;
			}
		}
		if(status == 1)
			break ;
	}

	if(status == 0)
		ERR_RET(CH02,ERROR)

	EPILOG
	return(SUCCESS) ;
}
