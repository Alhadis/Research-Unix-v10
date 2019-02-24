static char ID[] = "@(#) house.c: 1.1 3/16/82";
#include <stdio.h>
#include "sdp.h"
#include "define3.h"

#ifdef SHORT
#define HO01	"HO01 "
#define HO02	"HO02 "
#define HO03	"HO03 "
#define HO04	"HO04 "
#define HOAU	"HOAU "
#else
#define HO01	"HO01 -- illegal value of first parameter\n%d\n",num_frames
#define HO02	"%s","HO02 -- fail to allocate environment\n"
#define HO03	"%s","HO03 -- fail to allocate managers\n"
#define HO04	"HO04 -- fail to allocate frame\n%d\n",i
#define HOAU	"%s","HOAU -- audit failed in sdp_house\n"
#endif

#ifdef SWEET
#define PROLOG
#define EPILOG
#else
#define PROLOG		if( (aud_flag == AUDITON) && (aud_check() == ERROR) )\
				ERR_RET(HOAU,NULL)
#define EPILOG		if(aud_flag == AUDITON)\
				aud_set() ;\
			if(hist_fp != NULL)\
				fprintf(hist_fp,"HO\t%d\t%u\t%o\t%o\n",num_frames,frame_size,library,environ) ;
#endif

extern int aud_flag ;
extern struct ENVIRON *first_environ ;
extern struct LIBFILE *Glib_file ;
extern struct LIBMEM *Glib_mem ;
extern FILE *hist_fp ;
extern struct LIBMEM lib_mem ;

extern int *ALLOC() ;
extern int aud_check() ;
extern aud_set() ;
extern FATAL() ;

struct ENVIRON *				/* pointer to environment or NULL */
sdp_house(num_frames,frame_size,library)
int		num_frames ;
unsigned	frame_size ;
struct LIBMEM	*library ;
{
	register struct ENVIRON	*environ ;
	register struct MANAGER	*manager ;
	register int		i ;

	PROLOG
	Glib_mem = (library == NULL) ? &lib_mem : library ;

	if(num_frames <= 0)
		ERR_RET(HO01,NULL)

	if( (environ = (struct ENVIRON *)MALLOC(sizeof(struct ENVIRON))) == NULL )
		ERR_RET(HO02,NULL)

	environ->num_frames   = num_frames ;
	environ->frame_size    = frame_size ;
	environ->cnct_count   = 0 ;
	environ->lib_mem      = Glib_mem ;

	if(  (environ->manp = (struct MANAGER *)MALLOC((num_frames+1)*sizeof(struct MANAGER))) == NULL )
		ERR_RET(HO03,NULL)

	manager = environ->manp ;
	environ->empties = 0 ;
	for(i=0; i<num_frames; i++)
	{
		manager[i].space      = NULL ;
		manager[i].page_id    = IDNULL ;
		manager[i].forward    = i+1 ;
		manager[i].backward   = END ;
		manager[i].ref_chng   = RCNULL ;
		manager[i].usecount   = 0l ;
		if( (manager[i].frame_pntr = (char *)MALLOC(frame_size)) == NULL )
			ERR_RET(HO04,NULL)
	}

	manager[num_frames-1].forward   = END ;
	manager[num_frames].forward     = manager[num_frames].backward = num_frames ;
	manager[num_frames].space       = NULL ;
	manager[num_frames].page_id     = IDNULL ;
	manager[num_frames].ref_chng    = RCNULL ;
	manager[num_frames].usecount    = 0l ;
	manager[num_frames].frame_pntr  = NULL ;

	environ->stats.uses   = 0l ;
	environ->stats.faults = 0l ;
	environ->stats.writes = 0l ;
	environ->stats.maxuse = 0  ;
	environ->stats.curuse = 0  ;

	environ->next_environ = first_environ ;
	first_environ         = environ ;
	EPILOG
	return(environ) ;
}
