static char ID[] = "@(#) sync.c: 1.1 3/16/82";
#include <stdio.h>
#include "sdp.h"
#include "define3.h"

#ifdef SHORT
#define SY01	"SY01 "
#define SY02	"SY02 "
#define SY03	"SY03 "
#define SY04	"SY04 "
#define SY05	"SY05 "
#define SYAU	"SYAU "
#else
#define SY01	"SY01 -- error return from readpage\n%o\n",sp
#define SY02	"SY02 -- error return from writeframe\n%o\n",sp
#define SY03	"SY03 -- unknown ref/chng flag\n%o\t%d\n",sp,manager[i].ref_chng
#define SY04	"SY04 -- error return from wtSPACE\n%o\n",sp
#define SY05	"SY05 -- space not connected\n%o\n",sp
#define SYAU	"%s","SYAU -- audit failed in sdp_sync\n"
#endif

#ifdef SWEET
#define PROLOG
#define EPILOG
#else
#define PROLOG		if( (aud_flag == AUDITON) && (aud_check() == ERROR) )\
				ERR_RET(SYAU,ERROR)
#define EPILOG		if(aud_flag == AUDITON)\
				aud_set() ;\
			if(hist_fp != NULL)\
				fprintf(hist_fp,"SY\t%o\n",sp) ;
#endif

extern int aud_flag ;
extern struct SPACE *first_space ;
extern struct LIBFILE *Glib_file ;
extern FILE *hist_fp ;

extern int aud_check() ;
extern aud_set() ;
extern FATAL() ;
extern int	readpage() ;
extern int	wtSPACE() ;

int					/* SUCCESS or ERROR */
sdp_sync(sp)
struct SPACE	*sp ;
{
	register int		 i ;
	register struct SPACE	*curr ;
	register struct MANAGER	*manager ;

	PROLOG
	for(curr=first_space; curr!=NULL; curr=curr->next_space)
	{
		if( (curr==sp) || (sp==NULL) )
		{
			Glib_file = curr->lib_file ;

			manager = (curr->environ)->manp ;

			for(i=0; i<(curr->environ)->num_frames; i++)
				switch(manager[i].ref_chng)
				{
					case RNLY:
						if(readpage(curr,manager[i].page_id,i) == ERROR)
							ERR_RET(SY01,ERROR)
						break ;

					case WRTN:
						if( (curr->perms != WRTN)  ||  (writeframe(curr,i) == ERROR) )
							ERR_RET(SY02,ERROR)
						manager[i].ref_chng = RNLY ;
						break ;

					case RCNULL:
						break ;

					default:
						ERR_RET(SY03,ERROR)
						break ;

				}
			if ((curr->perms == WRTN) && (wtSPACE(curr) == ERROR))
				ERR_RET(SY04,ERROR)
			if(sp == curr)
				break ;
		}
	}
	if( (curr==NULL) && (sp!=NULL) )
		ERR_RET(SY04,ERROR)
	EPILOG
	return(SUCCESS) ;
}
