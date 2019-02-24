static char ID[] = "@(#) use.c: 1.1 3/16/82";
#include <stdio.h>
#include "sdp.h"
#include "define3.h"

#ifdef SHORT
#define US01	"US01 "
#define US02	"US02 "
#define US03	"US03 "
#define US04	"US04 "
#define US05	"US05 "
#define US06	"US06 "
#define US07	"US07 "
#define USAU	"USAU "
#else
#define US01	"US01 -- space not connected\n%o\n",sp
#define US02	"US02 -- no permissions for WRTN\n%o\n",sp
#define US03	"US03 -- unknown permissions\n%o\t%d\n",sp,permissions
#define US04	"US04 -- id out of range\n%o\t%lo\t%lo\n",sp,id,sp->maxpage
#define US05	"US05 -- id not aligned to %d-byte boundary\n%o\t%lo\n",sizeof(BOUNDARY),sp,id
#define US06	"US06 -- framesearch returns impossible value\n%o\t%d\t%d\n",sp,whichframe,(sp->environ)->num_frames
#define US07	"US07 -- fail to place item in frame\n%o\t%lo\n",sp,id
#define USAU	"%s","USAU -- audit failed in sdp_use\n"
#endif

#ifdef SWEET
#define PROLOG
#define EPILOG
#else
#define PROLOG		if( (aud_flag == AUDITON) && (aud_check() == ERROR) )\
				ERR_RET(USAU,NULL)
#define EPILOG		if(aud_flag == AUDITON)\
				aud_set() ;\
			if(hist_fp != NULL)\
				fprintf(hist_fp,"US\t%o\t%ld\t%d\t%o\n",sp,id,permissions,manager[whichframe].frame_pntr+byte_offset) ;
#endif

extern int aud_flag ;
extern struct LIBFILE *Glib_file ;
extern FILE *hist_fp ;

extern int aud_check() ;
extern aud_set() ;
extern FATAL() ;
extern fault() ;
extern framesearch() ;
extern spearch() ;

BOUNDARY *				/* pointer to item or NULL */
sdp_use(sp,id,permissions)
register struct SPACE	*sp ;
ITEMID			id ;
int			permissions ;
{
	register struct MANAGER	*manager ;
	register int		whichframe ;
	register int		byte_offset ;

	PROLOG
	if(spsearch(sp) == NOTFOUND)
		ERR_RET(US01,NULL)

	switch(permissions)
	{
		case WRTN:
			if(sp->perms != WRTN)
				ERR_RET(US02,NULL)

		case RNLY:
			break ;

		default:
			ERR_RET(US03,NULL)
	}

	if(INVALID(sp,id))
		ERR_RET(US04,NULL)
	if(id != ALIGNID(id))
		ERR_RET(US05,NULL)

	Glib_file = sp->lib_file ;

	manager = (sp->environ)->manp ;
	switch(whichframe = framesearch(sp,id))
	{
		case NOTFOUND:
			whichframe = fault(sp,id) ;
		default:
			if( (whichframe<0) || (whichframe >= (sp->environ)->num_frames) )
				ERR_RET(US06,NULL)
			break ;

		case ERROR:
			ERR_RET(US07,NULL)
			break ;
	}

	if( (manager[whichframe].ref_chng == RCNULL) || (permissions == WRTN) )
		manager[whichframe].ref_chng = permissions ;
	if(manager[whichframe].usecount == 0)
	{
		(sp->environ)->stats.curuse++ ;
		if( (sp->environ)->stats.curuse > (sp->environ)->stats.maxuse )
			(sp->environ)->stats.maxuse = (sp->environ)->stats.curuse ;
	}
	manager[whichframe].usecount++ ;
	(sp->environ)->stats.uses++ ;
	byte_offset = id % (sp->environ)->frame_size ;
	
	EPILOG
	return((BOUNDARY *)(manager[whichframe].frame_pntr + byte_offset)) ;
}
