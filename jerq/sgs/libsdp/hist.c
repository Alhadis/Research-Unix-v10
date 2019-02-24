static char ID[] = "@(#) hist.c: 1.2 6/29/82";
#include <stdio.h>
#include "sdp.h"
#include "define3.h"


#ifdef SHORT
#define HI01	"HI01 "
#define HI02	"HI02 "
#define HI03	"HI03 "
#define HIAU	"HIAU "
#else
#define HI01	"HI01 -- unable to close stream\n%o\n",hist_fp
#define HI02	"HI02 -- unable to close previous stream\n%o\n%s\n",hist_fp,name
#define HI03	"HI03 -- unable to open history file\n%s\n",name
#define HIAU	"%s","HIAU -- audit failed in sdp_hist\n"
#endif

#ifdef SWEET
#define PROLOG
#define EPILOG
#else
#define PROLOG		if( (aud_flag == AUDITON) && (aud_check() == ERROR) )\
				ERR_RET(HIAU,NULL)
#define EPILOG		if(aud_flag == AUDITON)\
				aud_set() ;\
			if(hist_fp != NULL)\
				fprintf(hist_fp,"HI\t%s\n",name) ;
#endif

extern int aud_flag ;
extern struct LIBFILE *Glib_file ;
extern FILE *hist_fp ;

extern aud_set() ;
extern int aud_check() ;
extern FATAL() ;

MY_FILE *				/* FILE pointer */
sdp_hist(name)
char *name ;
{

	PROLOG

		if(name == NULL)
		{
			if(fclose(hist_fp) == EOF)
				ERR_RET(HI01,NULL)
			hist_fp = NULL ;
		}
		else
		{
			if( (hist_fp != NULL) && (fclose(hist_fp) == EOF) )
				ERR_RET(HI02,NULL)
			if( (hist_fp = fopen(name,"w")) == NULL )
				ERR_RET(HI03,NULL)
		}

	EPILOG
	return((MY_FILE *)hist_fp) ;
}
