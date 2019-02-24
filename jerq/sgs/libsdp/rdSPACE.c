static char ID[] = "@(#) rdSPACE.c: 1.1 3/16/82";
#include <stdio.h>
#include "sdp.h"
#include "define2.h"
#include "define3.h"

#ifdef SHORT
#define RS01	"RS01 "
#define RS02	"RS02 "
#define RS03	"RS03 "
#define RS04	"RS04 "
#define RS05	"RS05 "
#define RS06	"RS06 "
#define RS07	"RS07 "
#define RS08	"RS08 "
#define RS09	"RS09 "
#define RS10	"RS10 "
#define RS11	"RS11 "
#define RS12	"RS12 "
#else
#define RS01	"RS01 -- fail to allocate for header file\n%s\n",sp->head_name
#define RS02	"RS02 -- error return from buildname\n%s\n",name
#define RS03	"RS03 -- fail to open header file\n%s\n",sp->head_name
#define RS04	"RS04 -- fail to position header file\n%s\n",sp->head_name
#define RS05	"RS05 -- fail to read header file\n%s\n",sp->head_name
#define RS06	"RS06 -- fail to allocate for file information\n%s\n",sp->head_name
#define RS07	"RS07 -- fail to position header file for file information\n%s\n",sp->head_name
#define RS08	"RS08 -- fail to read header file for file information\n%s\n",sp->head_name
#define RS09	"RS09 -- fail to allocate for stack information\n%s\n",sp->head_name
#define RS10	"RS10 -- fail to position header file for stack information\n%s\n",sp->head_name
#define RS11	"RS11 -- fail to read stack information\n%s\n",sp->head_name
#define RS12	"RS12 -- fail to close header file\n%s\n",sp->head_name
#endif

extern struct LIBFILE *Glib_file ;
extern int *ALLOC() ;
extern char *BUILDNAME() ;
extern FATAL() ;

struct SPACE *				/* pointer to configuration structure or NULL */
rdSPACE(name)
char	*name ;
{
	register FD		fd ;
	char			*head_name ;
	register struct SPACE	*sp ;

	if( (sp = (struct SPACE *)ALLOC(sizeof(struct SPACE))) == NULL )
		ERR_RET(RS01,NULL)

	if( (head_name = MBUILDNAME(name,-1)) == NULL )
		ERR_RET(RS02,NULL)

	if( (fd=MOPEN(head_name,RNLY)) == ERROR )
		ERR_RET(RS03,NULL)
	if(MSEEK(fd,0l) == ERROR)
		ERR_RET(RS04,NULL)

	if(MREAD(fd,sp,sizeof(struct SPACE)) == ERROR)
		ERR_RET(RS05,NULL)

	sp->head_name = head_name ;

	if( (sp->fileinfo = (struct FILEINFO *)ALLOC(sp->numbfile*sizeof(struct FILEINFO))) == NULL )
		ERR_RET(RS06,NULL)

	if(MSEEK(fd,(long)sizeof(struct SPACE)) == ERROR)
		ERR_RET(RS07,NULL)

	if(MREAD(fd,sp->fileinfo,sp->numbfile*sizeof(struct FILEINFO)) == ERROR)
		ERR_RET(RS08,NULL)

	if( (sp->staxinfo = (struct STAXINFO *)ALLOC(sp->numbstax*sizeof(struct STAXINFO))) == NULL )
		ERR_RET(RS09,NULL)

	if(MSEEK(fd,(long)(sp->numbfile*sizeof(struct FILEINFO)+sizeof(struct SPACE))) == ERROR)
		ERR_RET(RS10,NULL)

	if(MREAD(fd,sp->staxinfo,sp->numbstax*sizeof(struct STAXINFO)) == ERROR)
		ERR_RET(RS11,NULL)

	sp->next_space = NULL ;
	sp->environ = NULL ;

	if(MCLOSE(fd) == ERROR)
		ERR_RET(RS12,NULL)

	return(sp) ;
}
