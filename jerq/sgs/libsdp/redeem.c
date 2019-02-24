static char ID[] = "@(#) redeem.c: 1.1 3/16/82";
#include <stdio.h>
#include "sdp.h"
#include "define3.h"

#ifdef SHORT
#define RE01	"RE01 "
#define RE02	"RE02 "
#define RE03	"RE03 "
#define RE04	"RE04 "
#define RE05	"RE05 "
#define REAU	"REAU "
#else
#define RE01	"RE01 -- space not connected\n%o\n",sp
#define RE02	"RE02 -- no permission to redeem\n%o\n",sp
#define RE03	"RE03 -- id out of range\n%lo\t%lo\n",id,sp->maxpage
#define RE04	"RE04 -- id not aligned\n%o\t%lo\n",sp,id
#define RE05	"RE05 -- fail to push\n%o\t%lo\n",sp,id
#define REAU	"%s","REAU -- audit failed in sdp_redeem\n"
#endif

#ifdef SWEET
#define PROLOG
#define EPILOG
#else
#define PROLOG		if( (aud_flag == AUDITON) && (aud_check() == ERROR) )\
				ERR_RET(REAU,ERROR)
#define EPILOG		if(aud_flag == AUDITON)\
				aud_set() ;\
			if(hist_fp != NULL)\
				fprintf(hist_fp,"RE\t%o\t%ld\t%u\n",sp,id,howmuch) ;
#endif

extern int aud_flag ;
extern struct LIBFILE *Glib_file ;
extern FILE *hist_fp ;

extern int aud_check() ;
extern aud_set() ;
extern FATAL() ;
extern push_id() ;
extern spsearch() ;

int					/* SUCCESS or ERROR */
sdp_redeem(sp,id,howmuch)
struct SPACE	*sp ;
ITEMID		id ;
unsigned	howmuch ;
{

	PROLOG

	if(spsearch(sp) == NOTFOUND)
		ERR_RET(RE01,ERROR)

	if(sp->perms != WRTN)
		ERR_RET(RE02,ERROR)

	if(INVALID(sp,id))
		ERR_RET(RE03,ERROR)

	howmuch = (unsigned)ALIGNID((ITEMID)howmuch) ;
	if(id != ALIGNID(id))
		ERR_RET(RE04,ERROR)

	Glib_file = sp->lib_file ;

	if(push_id(sp,id,howmuch) == ERROR)
		ERR_RET(RE05,ERROR)

	EPILOG
	return(SUCCESS) ;
}
