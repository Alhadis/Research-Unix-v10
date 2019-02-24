static char ID[] = "@(#) pop_id.c: 1.1 3/16/82";
#include <stdio.h>
#include "sdp.h"
#include "define3.h"

#ifdef SHORT
#define PO01	"PO01 "
#else
#define PO01	"%s","PO01 -- error return from framesearch\n"
#endif

extern FATAL() ;
extern struct LIBFILE *Glib_file ;
extern fault() ;
extern framesearch() ;
extern stk1search() ;

ITEMID					/* itemid, IDNOTFOUND, IDERROR */
pop_id(sp,howmuch)
register struct SPACE	*sp ;
unsigned		howmuch ;
{
	register int		frame ;
	int			offset ;
	register int		stack ;
	struct MANAGER		*manager ;
	struct STAXINFO		*pntr ;
	register ITEMID		id ;


	if( (stack = stk1search(sp,howmuch)) == NOTFOUND)
		return(IDNOTFOUND) ;

	if( (id = sp->staxinfo[stack].top) == IDNULL )
		return(IDNOTFOUND) ;

	manager = (sp->environ)->manp ;

	if( (frame=framesearch(sp,id)) == NOTFOUND )
		frame = fault(sp,id) ;

	if(frame == ERROR)
		ERR_RET(PO01,IDERROR)

	offset = id % (sp->environ)->frame_size ;
	pntr = (struct STAXINFO *)(manager[frame].frame_pntr + offset) ;
	sp->staxinfo[stack].top = pntr->top ;
	sp->staxinfo[stack].depth-- ;
	pntr->top = IDNULL ;
	return(id) ;
}
