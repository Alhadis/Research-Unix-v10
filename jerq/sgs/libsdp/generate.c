static char ID[] = "@(#) generate.c: 1.2 12/8/82";
#include <stdio.h>
#include "sdp.h"

#define YYMMDD	811221

#include "define3.h"

#ifdef SHORT
#define GE01	"GE01 "
#define GE02	"GE02 "
#define GE03	"GE03 "
#define GE04	"GE04 "
#define GE05	"GE05 "
#define GE06	"GE06 "
#define GEAU	"GEAU "
#else
#define GE01	"GE01 -- fail to allocate header\n%s\n",name
#define GE02	"GE02 -- fail to allocate for file structure\n%s\n",name
#define GE03	"GE03 -- fail to allocate stacks structure\n%s\n",name
#define GE04	"GE04 -- fail to build name\n%s\n",name
#define GE05	"GE05 -- fail to create header file\n%s\n",sp->head_name
#define GE06	"GE06 -- error return from wtSPACE\n%s\n",name
#define GEAU	"%s","GEAU -- audit failed in sdp_generate\n"
#endif

#ifdef SWEET
#define PROLOG
#define EPILOG
#else
#define PROLOG		if( (aud_flag == AUDITON) && (aud_check() == ERROR) )\
				ERR_RET(GEAU,ERROR)
#define EPILOG		if(aud_flag == AUDITON)\
				aud_set() ;\
			if(hist_fp != NULL)\
				fprintf(hist_fp,"GE\t%s\t%u\t%o\n",name,page_size,library) ;
#endif


extern int aud_flag ;
extern struct LIBFILE *Glib_file ;
extern long Gln_file[] ;
extern int Gnumbfile ;
extern int Gnumbstax ;
extern int  Gsz_stax[] ;
extern FILE *hist_fp ;
extern struct LIBFILE lib_file ;

extern int *ALLOC() ;
extern char *BUILDNAME() ;
extern aud_set() ;
extern int aud_check() ;
extern struct LIBFILE lib_file ;
extern struct LIBFILE *Glib_file ;
extern FATAL() ;
extern int	wtSPACE() ;
extern		freeSPACE() ;

int					/* SUCCESS or ERROR */
sdp_generate(name,page_size,library,mode)
char		*name ;
unsigned	page_size ;
struct LIBFILE	*library ;
int		mode ;
{
	register struct SPACE	*sp ;
	register int		i ;

	PROLOG

	Glib_file = (library == NULL) ? &lib_file : library ;

	if( (sp = (struct SPACE *)ALLOC(sizeof(struct SPACE))) == NULL )
		ERR_RET(GE01,ERROR)

	sp->version    = YYMMDD ;
	sp->page_size  = page_size ;
	sp->maxpage    = 0l ;
	sp->curpage    = IDNULL ;
	sp->curleft    = 0 ;
	sp->perms      = WRTN ;
	sp->mode       = mode ;
	sp->numbfile   = Gnumbfile ;
	sp->numbstax   = Gnumbstax+1 ;
	sp->known_id   = IDNULL ;
	sp->environ    = NULL ;
	sp->lib_file       = NULL ;
	sp->next_space = NULL ;
	sp->sp1_hash   = HASHNULL ;
	sp->sp2_hash   = HASHNULL ;

	if( (sp->fileinfo = (struct FILEINFO *)ALLOC(sp->numbfile*sizeof(struct FILEINFO))) == NULL )
		ERR_RET(GE02,ERROR)

	for(i=0; i<sp->numbfile; i++)
	{
		sp->fileinfo[i].leng = Gln_file[i] ;
		sp->fileinfo[i].desc = NOTMADE ;
	}

	if( (sp->staxinfo = (struct STAXINFO *)ALLOC((sp->numbstax+1)*sizeof(struct STAXINFO))) == NULL )
		ERR_RET(GE03,ERROR)

	sp->staxinfo[0].top  = IDNULL ;
	sp->staxinfo[0].size = 0 ;
	sp->staxinfo[0].depth = 0 ;
	for(i=1; i<sp->numbstax; i++)
	{
		sp->staxinfo[i].top =  IDNULL ;
		sp->staxinfo[i].size = Gsz_stax[i-1] ;
		sp->staxinfo[i].depth = 0 ;
	}

	if( (sp->head_name = MBUILDNAME(name,-1)) == NULL )
		ERR_RET(GE04,ERROR)
	if(MCREATE(sp->head_name,mode) == ERROR)
		ERR_RET(GE05,ERROR)

	if(wtSPACE(sp) == ERROR)
		ERR_RET(GE06,ERROR)

	freeSPACE(sp) ;


	EPILOG
	return(SUCCESS) ;
}
