static char ID[] = "@(#) writeframe.c: 1.1 3/16/82";
#include <stdio.h>
#include "sdp.h"
#include "define3.h"

#ifdef SHORT
#define WF01	"WF01 "
#define WF02	"WF02 "
#define WF03	"WF03 "
#define WF04	"WF04 "
#define WF05	"WF05 "
#define WF06	"WF06 "
#define WF07	"WF07 "
#else
#define WF01	"WF01 -- found id beyond file limit\n%lo\t%d\t%d\n",id,i,sp->numbfile
#define WF02	"WF02 -- fail to build file name for creation\n%s%d\n",sp->head_name,i
#define WF03	"WF03 -- fail to create file\n%s\n",file_name
#define WF04	"WF04 -- fail to build file name for opening\n%s%d\n",sp->head_name,i
#define WF05	"WF05 -- fail to open file\n%s\n",file_name
#define WF06	"WF06 -- fail to position file\n%d\n",fd
#define WF07	"WF07 -- fail to write\n%d\n",fd
#endif

extern struct LIBFILE *Glib_file ;
extern char *BUILDNAME() ;
extern FATAL() ;
extern FREE() ;

int					/* SUCCESS or ERROR */
writeframe(sp,frame)
register struct SPACE	*sp ;
int			frame;
{
	long			offset ;
	register long		floor ;
	register struct MANAGER *manager ;
	register int		i ;
	int			bytewt ;
	ITEMID			id ;
	register FD		fd ;
	char			*file_name ;

	manager = (sp->environ)->manp ;
	id = manager[frame].page_id ;

	for(floor=i=0; id >= (floor += sp->fileinfo[i].leng); )
		if(++i == sp->numbfile)
			ERR_RET(WF01,ERROR)
	offset = id - (floor - sp->fileinfo[i].leng) ;

	switch(sp->fileinfo[i].desc)
	{
		case NOTMADE:
			if( (file_name = MBUILDNAME(sp->head_name,i)) == NULL )
				ERR_RET(WF02,ERROR)
			if(MCREATE(file_name,sp->mode) == ERROR)
				ERR_RET(WF03,ERROR)
			FREE(file_name) ;

		case CLOSED:
			if( (file_name = MBUILDNAME(sp->head_name,i)) == NULL )
				ERR_RET(WF04,ERROR)
			if( (fd = sp->fileinfo[i].desc = MOPEN(file_name,sp->perms)) == ERROR )
				ERR_RET(WF05,ERROR)
			FREE(file_name) ;
			break ;

		default:
			fd = sp->fileinfo[i].desc ;
			break ;
	}
	if(MSEEK(fd,offset) == ERROR)
		ERR_RET(WF06,ERROR)

	if(sp->maxpage-id < sp->environ->frame_size)
		bytewt = sp->maxpage-id ;
	else
		bytewt = sp->environ->frame_size ;

	if(MWRITE(fd,manager[frame].frame_pntr,bytewt) == ERROR )
		ERR_RET(WF07,ERROR)
	manager[frame].ref_chng = RNLY ;
	(sp->environ)->stats.writes++ ;

	return(SUCCESS) ;
}
