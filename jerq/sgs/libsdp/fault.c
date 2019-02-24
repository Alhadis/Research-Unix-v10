static char ID[] = "@(#) fault.c: 1.1 3/16/82";
#include "sdp.h"
#include "define3.h"

#ifdef SHORT
#define FA01	"FA01 "
#define FA02	"FA02 "
#define FA03	"FA03 "
#define FA04	"FA04 "
#else
#define FA01	"%s","FA01 -- error return from nextsearch\n"
#define FA02	"%s","FA02 -- error return from writeframe\n"
#define FA03	"FA03 -- unknown ref/change value\n%d\t%o\t%d\n",manager[frame].ref_chng,sp,frame
#define FA04	"%s","FA04 -- error return from readpage\n"
#endif

extern FATAL() ;
extern struct LIBFILE *Glib_file ;
extern nextsearch() ;
extern readpage() ;
extern writeframe() ;

int					/* frame number of ERROR */
fault(sp,id)
struct SPACE	*sp ;
ITEMID		id ;
{
	register int		frame ;
	register struct MANAGER	*manager ;

	id -= id % (sp->environ)->frame_size ;
	if( (frame = nextsearch(sp->environ)) == NOTFOUND)
		ERR_RET(FA01,ERROR)

	manager = (sp->environ)->manp ;
	switch(manager[frame].ref_chng)
	{
		case WRTN:
			if(writeframe(manager[frame].space,frame) == ERROR)
				ERR_RET(FA02,ERROR)

		case RCNULL:
		case RNLY:
			break ;

		default:
			ERR_RET(FA03,ERROR)
			break ;
	}

	if(readpage(sp,id,frame) == ERROR)
		ERR_RET(FA04,ERROR)

	manager[frame].space = sp ;
	manager[frame].page_id = id ;

	return(frame) ;
}
