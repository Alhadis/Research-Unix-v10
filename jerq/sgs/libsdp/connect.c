static char ID[] = "@(#) connect.c: 1.1 3/16/82";
#include <stdio.h>
#include "sdp.h"
#include "define3.h"

#ifdef SHORT
#define CO01	"CO01 "
#define CO02	"CO02 "
#define CO03	"CO03 "
#define CO04	"CO04 "
#define CO05	"CO05 "
#define CO06	"CO06 "
#define COAU	"COAU "
#else
#define CO01	"CO01 -- unknown permissions\n%d\n",perms
#define CO02	"CO02 -- error return from rdSPACE\n%s\n",name
#define CO03	"CO03 -- your address space (%ld) not compatible with this version (%ld)\n%s\n",sp->version,compatible,name
#define CO04	"CO04 -- environment not housed\n%o\t%s\n",environ,name
#define CO05	"CO05 -- framesize not compatible with file size\n%s\t%ld\t%d\n",name,sp->fileinfo[i].leng,environ->frame_size
#define CO06	"CO06 -- page/frame conflict\n%s\t%d\t%d\n",name,environ->frame_size,sp->page_size
#define COAU	"%s","COAU -- audit failed in sdp_connect\n"
#endif

#ifdef SWEET
#define PROLOG
#define EPILOG
#else
#define PROLOG		if( (aud_flag == AUDITON) && (aud_check() == ERROR) )\
				ERR_RET(COAU,NULL)
#define EPILOG		if(aud_flag == AUDITON)\
				aud_set() ;\
			if(hist_fp != NULL)\
				fprintf(hist_fp,"CO\t%s\t%o\t%o\n",name,environ,sp) ;
#endif

extern int aud_flag ;
extern struct LIBFILE *Glib_file ;
extern int hist_fp ;
extern struct LIBFILE lib_file ;
extern struct SPACE *first_space ;
extern ITEMID *known_idptr ;

extern aud_set() ;
extern int aud_check() ;
extern long compatible ;
extern int ensearch() ;
extern FATAL() ;
extern struct SPACE *rdSPACE() ;

struct SPACE *				/* pointer to configuration structure or NULL */
sdp_connect(name,environ,library,perms)
char		*name ;
struct ENVIRON	*environ ;
struct LIBFILE	*library ;
int		perms ;
{
	register struct SPACE	*sp ;
	register int		i ;
 
	PROLOG

	Glib_file = (library == NULL) ? &lib_file : library ;
	if( (perms != RNLY)  &&  (perms != WRTN) )
		ERR_RET(CO01,NULL)

	if( (sp = rdSPACE(name)) == NULL )
		ERR_RET(CO02,NULL)

	if(sp->version < compatible)
		ERR_RET(CO03,NULL)

	if( ensearch(environ) == NOTFOUND )
		ERR_RET(CO04,NULL)

	for(i=0; i < sp->numbfile; i++)
		if( sp->fileinfo[i].leng % environ->frame_size != 0 )
			ERR_RET(CO05,NULL)

	if(environ->frame_size % sp->page_size != 0)
		ERR_RET(CO06,NULL)

	sp->next_space = first_space ;
	first_space = sp ;
	environ->cnct_count++ ;
	sp->environ = environ ;
	sp->lib_file = Glib_file ;
	sp->perms = perms ;
	known_idptr = &(sp->known_id) ;
	EPILOG
	return(sp) ;
}
