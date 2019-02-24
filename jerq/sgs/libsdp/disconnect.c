static char ID[] = "@(#) disconnect.c: 1.2 12/8/82";
#include <stdio.h>
#include "sdp.h"
#include "define3.h"

#ifdef SHORT
#define DI01	"DI01 "
#define DI02	"DI02 "
#define DI03	"DI03 "
#define DI04	"DI04 "
#define DI05	"DI05 "
#define DI06	"DI06 "
#define DIAU	"DIAU "
#else
#define DI01	"DI01 -- space to be disconnected has item in use\n%o\n",sp
#define DI02	"DI02 -- permission conflict\n%d\t%d\t%d\n",sp->perms,j,manager[j].ref_chng
#define DI03	"DI03 -- error return from writeframe\n%o\n",sp
#define DI04	"DI04 -- fail to close open file\n%o\n",sp
#define DI05	"DI05 -- error return from wrtSPACE\n%o\n",sp
#define DI06	"DI06 -- space not connected\n%o\n",sp
#define DIAU	"%s","DIAU -- audit failed in sdp_disconnect\n"
#endif

#ifdef SWEET
#define PROLOG
#define EPILOG
#else
#define PROLOG		if( (aud_flag == AUDITON) && (aud_check() == ERROR) )\
				ERR_RET(DIAU,ERROR)
#define EPILOG		if(aud_flag == AUDITON)\
				aud_set() ;\
			if(hist_fp != NULL)\
				fprintf(hist_fp,"DI\t%o\n",sp) ;
#endif

extern	int aud_flag ;
extern	struct LIBFILE *Glib_file ;
extern	FILE *hist_fp ;
extern	struct SPACE *first_space ;

extern	aud_set() ;
extern	int aud_check() ;
extern	FATAL() ;
extern	rem_frame() ;
extern	writeframe() ;
extern int	wtSPACE() ;
extern		freeSPACE() ;

int					/* SUCCESS or ERROR */
sdp_disconnect(sp)
struct SPACE	*sp ;
{
	register int		j ;
	register int		k ;
	register struct MANAGER	*manager ;
	register struct SPACE	*curr ;
	register struct SPACE	*next ;
	register struct SPACE	*prev ;

	PROLOG
	prev = NULL ;
	for(curr=first_space; curr!=NULL; curr=next)
	{
		next = curr->next_space ;

		if( (sp==curr) || (sp==NULL) )
		{
			Glib_file = curr->lib_file ;

			manager = (curr->environ)->manp ;
			for(j=0; j<(curr->environ)->num_frames; j++)
				if(manager[j].space == curr)
				{
					if(manager[j].usecount != 0)
						ERR_RET(DI01,ERROR)

					if(manager[j].ref_chng == WRTN)
						if(curr->perms != WRTN)
							ERR_RET(DI02,ERROR)
						else
							if(writeframe(curr,j) == ERROR)
								ERR_RET(DI03,ERROR)
					manager[j].space    = NULL ;
					manager[j].page_id  = IDNULL ;
					manager[j].ref_chng = RCNULL ;
					rem_frame(curr->environ,j) ;
					manager[j].forward = (curr->environ)->empties ;
					(curr->environ)->empties = j ;
				}

			for(k=0; k<curr->numbfile; k++)
				switch(curr->fileinfo[k].desc)
				{
					case CLOSED:
					case NOTMADE:
						break ;

					default:
						if(MCLOSE(curr->fileinfo[k].desc) == ERROR)
							ERR_RET(DI04,ERROR)
						curr->fileinfo[k].desc = CLOSED ;
				}

			(curr->environ)->cnct_count-- ;
			if( (curr->perms == WRTN)  &&  (wtSPACE(curr) == ERROR) )
				ERR_RET(DI05,ERROR)

			freeSPACE(curr) ;

			if(prev == NULL)
				first_space = next ;
			else
				prev->next_space = next ;
			if(sp == curr)
				break ;
		}
		else
			prev = curr ;

	}

	if( (curr==NULL) && (sp!=NULL) )
		ERR_RET(DI06,ERROR)

	EPILOG
	return(SUCCESS) ;
}
