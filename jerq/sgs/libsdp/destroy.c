static char ID[] = "@(#) destroy.c: 1.2 12/8/82";
#include <stdio.h>
#include "sdp.h"
#include "define2.h"
#include "define3.h"

#ifdef SHORT
#define DE01	"DE01 "
#define DE02	"DE02 "
#define DE03	"DE03 "
#define DE04	"DE04 "
#define DE05	"DE05 "
#define DEAU	"DEAU"
#else
#define DE01	"DE01 -- error return from rdSPACE\n%s\n",name
#define DE02	"DE02 -- fail to unlink header file\n%s\n",sp->head_name
#define DE03	"DE03 -- fail to build name\n%s%d\n",sp->head_name,i
#define DE04	"DE04 -- fail to unlink file\n%s\n",file_name
#define DE05	"DE05 -- unknown descriptor value\n%d\n",sp->fileinfo[i].desc
#define DEAU	"%s","DEAU -- audit failed in sdp_destroy\n"
#endif

#ifdef SWEET
#define PROLOG
#define EPILOG
#else
#define PROLOG		if( (aud_flag == AUDITON) && (aud_check() == ERROR) )\
				ERR_RET(DEAU,ERROR)
#define EPILOG		if(aud_flag == AUDITON)\
				aud_set() ;\
			if(hist_fp != NULL)\
				fprintf(hist_fp,"DE\t%s\n",name) ;
#endif


extern int aud_flag ;
extern struct LIBFILE *Glib_file ;
extern FILE *hist_fp ;
extern struct LIBFILE lib_file ;

extern char *BUILDNAME() ;
extern aud_set() ;
extern int aud_check() ;
extern FATAL() ;
extern FREE() ;
extern struct SPACE *rdSPACE() ;
extern UNLINK() ;

int					/* SUCCESS or ERROR */
sdp_destroy(name,library)
char		*name ;
struct LIBFILE	*library ;
{
	register int		i ;
	register struct SPACE 	*sp ;
	register char		*file_name ;

	PROLOG

	Glib_file = (library == NULL) ? &lib_file : library ;
	if( (sp = rdSPACE(name)) == NULL )
		ERR_RET(DE01,ERROR)

	if(MUNLINK(sp->head_name) == ERROR)
		ERR_RET(DE02,ERROR)

	for(i=0 ; i<sp->numbfile; i++)
		switch(sp->fileinfo[i].desc)
		{
			case CLOSED:
				if( (file_name = MBUILDNAME(sp->head_name,i)) == NULL )
					ERR_RET(DE03,ERROR)
				if(MUNLINK(file_name) == ERROR)
					ERR_RET(DE04,ERROR)
				FREE(file_name) ;
				break ;

			case NOTMADE:
				break ;

			default:
				ERR_RET(DE05,ERROR)
				break ;
		}

	FREE(sp->head_name) ;
	sp->head_name = NULL ;
	FREE((char *)sp->fileinfo) ;
	sp->fileinfo = NULL ;
	FREE((char *)sp->staxinfo) ;
	sp->staxinfo = NULL ;
	FREE((char *)sp) ;
	sp = NULL ;

	EPILOG
	return(SUCCESS) ;
}
