static char ID[] = "@(#) unuse.c: 1.1 3/16/82";

#include <stdio.h>
#include "sdp.h"
#include "define3.h"

#ifdef SHORT
#define UN01	"UN01 "
#define UN02	"UN02 "
#define UN03	"UN03 "
#define UN04	"UN04 "
#define UN05	"UN05 "
#define UN06	"UN06 "
#define UN07	"UN07 "
#define UN08	"UN08 "
#define UN09	"UN09 "
#define UNAU	"UNAU "
#else
#define UN01	"UN01 -- space not connected\n%o\n",sp
#define UN02	"UN02 -- id out of range\n%lo\t%lo\n",id,sp->maxpage
#define UN03	"UN03 -- attempt to unuse an item not core resident\n%o\t%lo\n",sp,id
#define UN04	"UN04 -- frame search failed\n%o\n",sp
#define UN05	"UN05 -- whichframe returned an impossible value\n%o\t%d\t%d\n",sp,whichframe,(sp->environ)->num_frames
#define UN06	"UN06 -- no permissions for WRTN\n%o\n",sp
#define UN07	"UN07 -- unknown permissions\n%o\t%d\n",sp,permissions
#define UN08	"UN08 -- attempt to unuse an unused item\n%o\n%O\n",sp,id
#define UN09	"%s","UN09 -- compilation made without either LFU or LRU defined\n"
#define UNAU	"%s","UNAU -- audit failed in sdp_unuse\n"
#endif

#ifdef SWEET
#define PROLOG
#define EPILOG
#else
#define PROLOG		if( (aud_flag == AUDITON) && (aud_check() == ERROR) )\
				ERR_RET(UNAU,ERROR)
#define EPILOG		if(aud_flag == AUDITON)\
				aud_set() ;\
			if(hist_fp != NULL)\
				fprintf(hist_fp,"UN\t%o\t%ld\t%d\n",sp,id,permissions) ;
#endif

extern int aud_flag ;
extern struct LIBFILE *Glib_file ;
extern FILE *hist_fp ;

extern int aud_check() ;
extern aud_set() ;
extern FATAL() ;
extern framesearch() ;
extern spsearch() ;
extern int rem_frame() ;
extern ins_frame() ;

int					/* SUCCESS or ERROR */
sdp_unuse(sp,id,permissions)
register struct SPACE	*sp ;
ITEMID		id ;
int		permissions ;
{
	register struct MANAGER	*manager ;
	register int		whichframe ;
	register long		usecount ;

	PROLOG
	if(spsearch(sp) == NOTFOUND)
		ERR_RET(UN01,ERROR)

	if(INVALID(sp,id))
		ERR_RET(UN02,ERROR)

	Glib_file = sp->lib_file ;

	switch(whichframe = framesearch(sp,id))
	{
		case NOTFOUND:
			ERR_RET(UN03,ERROR)
			break ;

		case ERROR:
			ERR_RET(UN04,ERROR)
			break ;

		default:
			if( (whichframe<0) || (whichframe >= (sp->environ)->num_frames) )
				ERR_RET(UN05,ERROR)
			break ;
	}


	manager = (sp->environ)->manp ;
	switch(permissions)
	{
		case WRTN:
			if(sp->perms != WRTN)
				ERR_RET(UN06,ERROR)
			manager[whichframe].ref_chng = WRTN ;
			break ;

		case RNLY:
			break ;

		default:
			ERR_RET(UN07,ERROR)
	}

	usecount = --manager[whichframe].usecount ;
	if(usecount < 0)
		ERR_RET(UN08,ERROR)

	if(usecount == 0)
		(sp->environ)->stats.curuse-- ;
#ifdef LRU
	rem_frame(sp->environ,whichframe) ;
	ins_frame(sp->environ,whichframe,(sp->environ)->num_frames) ;
#else
#ifdef LFU
	if(manager[whichframe].forward != (sp->environ)->num_frames)
		ins_frame(sp->environ,rem_frame(sp->environ,manager[whichframe].forward),whichframe) ;
#else
	ERR_RET(UN09,ERROR)
#endif
#endif
	EPILOG
	return(SUCCESS) ;
}
