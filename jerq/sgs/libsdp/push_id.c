static char ID[] = "@(#) push_id.c: 1.1 3/16/82";
#include "sdp.h"
#include "define3.h"

#ifdef SHORT
#define PU01	"PU01 "
#define PU02	"PU02 "
#else
#define PU01	"%s","PU01 -- error return from stk2search\n"
#define PU02	"%s","PU02 -- error return from fault\n"
#endif

extern FATAL() ;
extern struct LIBFILE *Glib_file ;
extern fault() ;
extern framesearch() ;
extern stk2search() ;

int					/* SUCCESS or ERROR */
push_id(sp,id,howmuch)
struct SPACE	*sp ;
ITEMID		id ;
unsigned	howmuch ;
{
	register int		frame ;
	register int		offset ;
	register int		stack ;
	register struct MANAGER	*manager ;
	register struct STAXINFO *pntr ;

	if(howmuch < sizeof(ITEMID)+sizeof(unsigned))
		return(SUCCESS) ;
	manager = (sp->environ)->manp ;

	if( (stack = stk2search(sp,howmuch)) == ERROR )
		ERR_RET(PU01,ERROR)

	if( (frame = framesearch(sp,id)) == NOTFOUND )
		frame = fault(sp,id) ;

	if(frame == ERROR)
		ERR_RET(PU02,ERROR)

	offset = id % (sp->environ)->frame_size ;
	pntr = (struct STAXINFO *)(manager[frame].frame_pntr + offset) ;
	pntr->top = sp->staxinfo[stack].top ;
	pntr->size = (long)howmuch ;
	sp->staxinfo[stack].top = id ;
	sp->staxinfo[stack].depth++  ;
	manager[frame].ref_chng = WRTN ;
	return(SUCCESS) ;
}
