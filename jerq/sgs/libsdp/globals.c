static char ID[] = "@(#) globals.c: 1.1 3/16/82";
#include <stdio.h>
#include "sdp.h"
#include "define2.h"
#include "define3.h"

#define COMPATIBLE	811221

extern int *ALLOC() ;
extern char *BUILDNAME() ;
extern int CLOSE() ;
extern int CREATE() ;
extern	   FATAL() ;
extern     FREE() ;
extern FD  OPEN() ;
extern int READ() ;
extern int SEEK() ;
extern int UNLINK() ;
extern int WRITE() ;

int aud_flag = AUDITOFF ;		/* flag for audits			*/
long compatible = COMPATIBLE ;		/* last version compatible with old SDP	*/

struct ENVIRON *first_environ = NULL ;	/* pointer to first environment		*/

struct SPACE *first_space = NULL ;	/* pointer to first space		*/

long Gln_file[] = FILEINIT ;		/* file lengths 			*/

long gl1_hash = HASHNULL ;		/* for audits				*/
long gl2_hash = HASHNULL ;		/* for audits				*/

int Gnumbfile = sizeof(Gln_file)/sizeof(Gln_file[0]) ;
					/* number of files with lengths		*/

int Gsz_stax[] = STAXINIT ;		/* stack sizes 				*/

int Gnumbstax = sizeof(Gsz_stax)/sizeof(Gsz_stax[0]) ;
					/* number of stacks			*/

FILE *hist_fp = NULL ;			/* for history file			*/

ITEMID *known_idptr = NULL ;		/* for user itemid			*/
struct LIBFILE lib_file =
{
	BUILDNAME,
	CLOSE,
	CREATE,
	OPEN,
	READ,
	SEEK,
	UNLINK,
	WRITE,
};

struct LIBMEM lib_mem =
{
	ALLOC,
	FREE,
} ;

struct LIBFILE *Glib_file ;		/* file library pointer used in macros	*/

struct LIBMEM *Glib_mem ;		/* memory library pointer used in macros*/
