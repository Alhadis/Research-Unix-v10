static char ID[] = "@(#) wtSPACE.c: 1.2 12/8/82";
#include <stdio.h>
#include "sdp.h"
#include "define3.h"

#ifdef SHORT
#define WS01	"WS01 "
#define WS02	"WS02 "
#define WS03	"WS03 "
#define WS04	"WS04 "
#define WS05	"WS05 "
#define WS06	"WS06 "
#define WS07	"WS07 "
#define WS08	"WS08 "
#else
#define WS01	"WS01 -- fail to open header file\n%s\n",sp->head_name
#define WS02	"WS02 -- fail to position header file\n%s\n",sp->head_name
#define WS03	"WS03 -- fail to write header file\n%s\n",sp->head_name
#define WS04	"WS04 -- fail to position header file\n%s\n",sp->head_name
#define WS05	"WS05 -- fail to write header file\n%s\n",sp->head_name
#define WS06	"WS06 -- fail to position header file\n%s\n",sp->head_name
#define WS07	"WS07 -- fail to write header file\n%s\n",sp->head_name
#define WS08	"WS08 -- fail to close header file\n%s\n",sp->head_name
#endif

extern struct LIBFILE *Glib_file ;
extern FATAL() ;
extern FREE() ;

int					/* SUCCESS or ERROR */
wtSPACE(sp)
register struct SPACE	*sp ;
{
	register FD	fd ;

	if( (fd = MOPEN(sp->head_name,sp->perms)) == ERROR )
		ERR_RET(WS01,ERROR)

	if(MSEEK(fd,(long)sizeof(struct SPACE)) == ERROR)
		ERR_RET(WS02,ERROR)

	if(MWRITE(fd,sp->fileinfo,sp->numbfile*sizeof(struct FILEINFO)) == ERROR)
		ERR_RET(WS03,ERROR)

	if(MSEEK(fd,(long)(sp->numbfile*sizeof(struct FILEINFO)+sizeof(struct SPACE))) == ERROR)
		ERR_RET(WS04,ERROR)

	if(MWRITE(fd,sp->staxinfo,sp->numbstax*sizeof(struct STAXINFO)) == ERROR)
		ERR_RET(WS05,ERROR)

	if(MSEEK(fd,0l) == ERROR) 
		ERR_RET(WS06,ERROR)

	if(MWRITE(fd,sp,sizeof(struct SPACE)) == ERROR)
		ERR_RET(WS07,ERROR)


	if(MCLOSE(fd) == ERROR)
		ERR_RET(WS08,ERROR)

	return(SUCCESS) ;
}

freeSPACE(sp)
register struct SPACE	*sp ;
{
	FREE(sp->head_name) ;
	FREE(sp->fileinfo) ;
	FREE(sp->staxinfo) ;
	FREE(sp) ;
}
