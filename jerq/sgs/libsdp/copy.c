static char ID[] = "@(#) copy.c: 1.1 3/16/82";
#include <stdio.h>
#include <fcntl.h>
#include "define2.h"
#include "define3.h"
#include "sdp.h"

#define CP01	"CP01 "
#define CP02	"CP02 "
#define CP03	"CP03 "
#define CP04	"CP04 "
#define CP05	"CP05 "
#define CP06	"CP06 "
#define CP07	"CP07 "
#define CP08	"CP08 "
#define CP09	"CP09 "
#define CP10	"CP10 "
#define CP11	"CP11 "
#define CP12	"CP12 "
#define CP13	"CP13 "
extern	struct	LIBFILE	lib_file ;
extern	struct	LIBFILE	*Glib_file ;
extern	struct	LIBMEM	*Glib_mem ;
extern	struct	SPACE	*rdSPACE() ;
extern	int		wtSPACE() ;
extern			freeSPACE() ;

sdp_copy(in_name,out_name,library)
char		*in_name ;
char		*out_name ;
struct	LIBFILE	*library ;
{
	struct SPACE	*sp ;
	int		in_fd ;
	int		out_fd ;
	int		n ;
	int		i ;
	char		*namebuf ;
	char		*databuf ;
	int		nameleng ;

	Glib_file = (library == NULL ) ? &lib_file : library ;
	if ( (sp = rdSPACE(in_name)) == NULL )
		ERR_RET(CP01,ERROR)
	if ( (databuf = (char *)ALLOC(sp->page_size)) == NULL )
		ERR_RET(CP02,ERROR)
	nameleng = strlen(in_name) ;
	if ( strlen(out_name) > strlen(in_name) )
		nameleng = strlen(out_name) ;
	if ( (namebuf = (char *)ALLOC(nameleng+strlen(SUFFIX)+COUNT+1)) == NULL )
		ERR_RET(CP03,ERROR)
	for(i=0; i<sp->numbfile; i++)
		if(sp->fileinfo[i].desc != NOTMADE)
		{
			sprintf(namebuf,"%s%d",sp->head_name,i) ;
			if((in_fd=MOPEN(namebuf,O_RDONLY)) == ERROR)
				ERR_RET(CP04,ERROR)
			sprintf(namebuf,"%s%s%d",out_name,SUFFIX,i) ;
			if ( MCREATE(namebuf,sp->mode) == ERROR )
				ERR_RET(CP05,ERROR)
			if ( (out_fd=MOPEN(namebuf,O_RDWR)) == ERROR )
				ERR_RET(CP06,ERROR)
			while(1)
			{
				switch(n=MREAD(in_fd,databuf,sp->page_size))
				{
					case 0 :
						break ;
		
					case ERROR :
						ERR_RET(CP07,ERROR)
		
					default :
						if ( n != sp->page_size )
							ERR_RET(CP08,ERROR)
						if ( MWRITE(out_fd,databuf,sp->page_size) == ERROR )
							ERR_RET(CP09,ERROR)
						continue ;
				}
				break ;
			}
			if(MCLOSE(in_fd) == ERROR)
				ERR_RET(CP10,ERROR)
			if(MCLOSE(out_fd) == ERROR)
				ERR_RET(CP11,ERROR)
		}
	sp->head_name = namebuf ;
	sprintf(sp->head_name,"%s%s",out_name,SUFFIX) ;
	sp->perms = WRTN ;
	if ( MCREATE(sp->head_name,sp->mode) == ERROR )
		ERR_RET(CP12,ERROR)
	if ( wtSPACE(sp) == ERROR )
		ERR_RET(CP13,ERROR)
	freeSPACE(sp) ;
	return(SUCCESS) ;
}
