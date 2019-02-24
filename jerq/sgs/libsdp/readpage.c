static char ID[] = "@(#) readpage.c: 1.1 3/16/82";
#include <stdio.h>
#include "sdp.h"
#include "define3.h"

#ifdef SHORT
#define RP01	"RP01 "
#define RP02	"RP02 "
#define RP03	"RP03 "
#define RP04	"RP04 "
#define RP05	"RP05 "
#define RP06	"RP06 "
#define RP07	"RP07 "
#else
#define RP01	"RP01 -- attempt to read id beyond maximum allottment\n%lo\t%lo\n",id,sp->maxpage
#define RP02	"RP02 -- attempt to read beyond file configuration\n%d\t%d\n",i,sp->numbfile
#define RP03	"RP03 -- fail to build file name\n%s%d\n",sp->head_name,i
#define RP04	"RP04 -- error return from open\n%s\n",file_name
#define RP05	"RP05 -- fail to seek\n%s\n",file_name
#define RP06	"RP06 -- fail to read\n%s\n",file_name
#define RP07	"RP07 -- unexpected number of bytes read\n%d\n",byterd
#endif

extern struct LIBFILE *Glib_file ;
extern char *BUILDNAME() ;
extern FATAL() ;

int					/* SUCCESS or ERROR */
readpage(sp,id,frame)
register struct SPACE	*sp ;
register ITEMID		id ;
int			frame;
{
	struct MANAGER		*manager ;
	register int		i ;
	register long		floor ;
	register int		byterd ;
	long			offset ;
	char			*file_name ;

	if(id >= sp->maxpage)
		ERR_RET(RP01,ERROR)

	id -=  id % (sp->environ)->frame_size ;

	for(floor=i=0; id >= (floor += sp->fileinfo[i].leng) ;)
		if(++i == sp->numbfile)
			ERR_RET(RP02,ERROR)
	offset = id - (floor - sp->fileinfo[i].leng);

	switch(sp->fileinfo[i].desc)
	{
		case NOTMADE:
			return(SUCCESS) ;

		case CLOSED:
			if( (file_name = MBUILDNAME(sp->head_name,i)) == NULL )
				ERR_RET(RP03,ERROR)
			if( (sp->fileinfo[i].desc = MOPEN(file_name,sp->perms)) == ERROR )
				ERR_RET(RP04,ERROR)
			FREE(file_name) ;
			break ;

		default:
			break ;
	}

	if(MSEEK(sp->fileinfo[i].desc,offset) == ERROR)
		ERR_RET(RP05,ERROR)

	manager = (sp->environ)->manp ;
	if( (byterd = MREAD(sp->fileinfo[i].desc,manager[frame].frame_pntr,(sp->environ)->frame_size)) == ERROR )
		ERR_RET(RP06,ERROR)

	if( (byterd > sp->environ->frame_size) || (byterd % sp->page_size != 0) )
		ERR_RET(RP07,ERROR)

	(sp->environ)->stats.faults++ ;
	return(SUCCESS) ;
}
