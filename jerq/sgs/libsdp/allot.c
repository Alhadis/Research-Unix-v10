static char ID[] = "@(#) allot.c: 1.1 3/16/82";
#include <stdio.h>
#include "sdp.h"
#include "define3.h"

#ifdef SHORT
#define AL01	"AL01 "
#define AL02	"AL02 "
#define AL03	"AL03 "
#define AL04	"AL04 "
#define AL05	"AL05 "
#define AL06	"AL06 "
#define ALAU	"ALAU "
#else
#define AL01	"AL01 -- space not connected\n%o\n",sp
#define AL02	"AL02 -- no permissions to allot\n"
#define AL03	"AL03 -- fail to pop stack\n%o\n",sp
#define AL04	"AL04 -- address space overflow\n%o\n",sp
#define AL05	"AL05 -- fail to redeem fragment from multipage item\n%o\n",sp
#define AL06	"AL06 -- fail to redeem fragment from old current page\n%o\n",sp
#define ALAU	"%s","ALAU -- audit failed in sdp_allot\n"
#endif

#ifdef SWEET
#define PROLOG
#define EPILOG
#else
#define PROLOG		if( (aud_flag == AUDITON) && (aud_check() == ERROR) )\
				ERR_RET(ALAU,IDERROR)\
			if(hist_fp != NULL)\
				fprintf(hist_fp,"AL\t%o\t%u\t",sp,howmuch) ;
#define EPILOG		if(aud_flag == AUDITON)\
				aud_set() ;\
			if(hist_fp != NULL)\
				fprintf(hist_fp,"%ld\n",id) ;
#endif

extern int aud_flag ;
extern struct LIBFILE *Glib_file ;
extern FILE *hist_fp ;
extern struct LIBFILE lib_file ;

extern aud_set() ;
extern int aud_check() ;
extern FATAL() ;
extern ITEMID pop_id() ;
extern push_id() ;
extern spsearch() ;

ITEMID				/* itemid, IDNULL, IDERROR */
sdp_allot(sp,howmuch)
register struct SPACE	*sp ;
register unsigned	howmuch ;
{
	register unsigned	page_size ;
	register ITEMID 	id ;
	ITEMID			increase ;
	ITEMID			redeem_id ;
	int			rationalize() ;

	PROLOG
	if(spsearch(sp) == NOTFOUND)
		ERR_RET(AL01,IDERROR)

	Glib_file = sp->lib_file ;

	if(sp->perms != WRTN)
		ERR_RET(AL02,IDERROR)

	howmuch = (unsigned)ALIGNID((ITEMID)howmuch) ;

	page_size = sp->page_size ;

	id = IDNULL ;
	if ( howmuch > 0 )
		id = pop_id(sp,howmuch) ;			/* fetch from stack */
	switch( rationalize(id) )	/* cannot switch on long:  see NOTES */
	{
		case ERROR:
			ERR_RET(AL03,IDERROR)
			break ;

		case NOTFOUND:

			if(howmuch == 0)
				id = IDNULL ;
			else if(howmuch >= page_size)		/* more than a page */
			{
				id = sp->maxpage ;
				increase = ((long)howmuch+page_size-1)/page_size * page_size ;
				sp->maxpage += increase ;
				if(sp->maxpage <= id)
					ERR_RET(AL04,IDERROR)
				if(push_id(sp,id+howmuch,increase-howmuch) == ERROR)
					ERR_RET(AL05,IDERROR)
			}
			else
				if(howmuch <= sp->curleft)	/* fits in current page */
				{
					id = (sp->curpage) + (page_size - sp->curleft) ;
					sp->curleft -= howmuch ;
				}
				else				/* need new current page */
				{
					redeem_id = (sp->curpage) + (page_size - sp->curleft) ;
					if(push_id(sp,redeem_id,sp->curleft) == ERROR)
						ERR_RET(AL06,IDERROR)
					id = sp->curpage = sp->maxpage ;
					sp->maxpage += page_size ;
					sp->curleft = page_size - howmuch ;
				}
			break ;

		default:
			break ;
	}


	EPILOG
	return(id) ;
}

int					/* SUCCESS, ERROR, NOTFOUND */
rationalize(id)
ITEMID	id ;
{

	if(id == IDERROR)
		return(ERROR) ;
	else if(id == IDNOTFOUND)
		return(NOTFOUND) ;
	else
		return(SUCCESS) ;
}
