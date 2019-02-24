static char ID[] = "@(#) vacate.c: 1.1 3/16/82";
#include <stdio.h>
#include "sdp.h"
#include "define3.h"

#ifdef SHORT
#define VA01	"VA01 "
#define VA02	"VA02 "
#define VAAU	"VAAU "
#else
#define VA01	"VA01 -- attempt to vacate a connected environment\n%o\n",environ
#define VA02	"VA02 -- attempt to vacate unhoused environment\n%o\n",environ
#define VAAU	"%s","VAAU -- audit failed in sdp_vacate\n"
#endif

#ifdef SWEET
#define PROLOG
#define EPILOG
#else
#define PROLOG		if( (aud_flag == AUDITON) && (aud_check() == ERROR) )\
				ERR_RET(VAAU,ERROR)
#define EPILOG		if(aud_flag == AUDITON)\
				aud_set() ;\
			if(hist_fp != NULL)\
				fprintf(hist_fp,"VA\t%o\n",environ) ;
#endif

extern int aud_flag ;
extern struct ENVIRON *first_environ ;
extern struct LIBFILE *Glib_file ;
extern struct LIBMEM *Glib_mem ;
extern FILE *hist_fp ;

extern int aud_check() ;
extern aud_set() ;
extern FATAL() ;

int					/* SUCCESS or ERROR */
sdp_vacate(environ)
struct ENVIRON	*environ ;
{
	register struct MANAGER	*manager ;
	register int		j ;
	register struct ENVIRON	*curr ;
	register struct ENVIRON	*next ;
	register struct ENVIRON	*prev ;

	PROLOG

	prev = NULL ;
	for(curr=first_environ; curr!=NULL; curr=next)
	{
		next = curr->next_environ ;
		if( (curr==environ) || (environ==NULL) )
		{
			if(curr->cnct_count != 0)
				ERR_RET(VA01,ERROR)

			Glib_mem = curr->lib_mem ;

			if(prev == NULL)
				first_environ = next ;
			else
				prev->next_environ = next ;

			manager = curr->manp ;
			for(j=0; j<curr->num_frames; j++)
			{
				MFREE(manager[j].frame_pntr) ;
				manager[j].frame_pntr = NULL ;
			}
			MFREE(curr->manp) ;
			curr->manp = NULL ;
			MFREE(curr) ;
			if(environ == curr)
				break ;
		}
		else
			prev = curr ;
	}

	if( (curr==NULL) && (environ!=NULL) )
		ERR_RET(VA02,ERROR)

	EPILOG
	return(SUCCESS) ;
}
