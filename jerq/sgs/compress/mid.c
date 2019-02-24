/*
*     static char ID_mid[] = "@(#) mid.c: 1.3 6/29/82";
*/

#include <stdio.h>
#include "sdp.h"
#include "sdp1.h"
#include <signal.h>
#include "syms.h"
#include "tagitem.h"


/*	before starting pass 2 of utility, a table for index of
	tag must be built.  The tagitem and storage area for tags 
	saved in pass 1 are freed
 */

#define YES 	1
#define NO	0
struct table{
	long	oldlcn;
	long	newlcn;
	long	real_loc;
} *tbl_head;


extern	long	szecnt;
extern  char   *malloc();
extern long	tag_sum;
extern int	vflag;
extern char      tagname[];
extern int      framsz;
extern int        framect;

extern struct SPACE    *TAGSPACE;
extern struct ENVIRON  *ENV;
extern struct SPACE    *SYMSPACE;


build()
{


	extern ITEMID		ti_head;

	TAGITEM *tag_ptr, *tag_ptr2;
	ITEMID  tagid;
	struct table	*tbl_ptr;

	long		i;
	char           *p;


	if (symfini()!=YES)
		{
		fprintf(stderr,"could not close SDP\n");
		return (NO);
		}
	if (sdp_disconnect(TAGSPACE) == SDPERROR)	
		{
		fprintf(stderr,"Failed to close SDP for tag");
		return(NO);
		}
	if (sdp_vacate(ENV) == SDPERROR)
		{
		return (NO);
		}
	tbl_head = (struct table *) calloc((unsigned)tag_sum, sizeof(struct table));
	if ( tbl_head == NULL ) {
		fprintf(stderr,"No space for table\n");
	    return(NO);
	    }
	tbl_ptr = tbl_head;

	tagid = ti_head;
/*
	printf("framect=%d framsz=%d tagsum=%ld tab=%d\n",framect,framsz,
	tag_sum,sizeof(struct table));
*/
	framect=((long) framect * framsz -  tag_sum * sizeof(struct table)) / framsz;

	if (framect < 2)
		{
		fprintf(stderr,"nospace for secondary house\n");
		return (NO);
		}
	ENV = sdp_house(framect,framsz,NULL);
	if (ENV == NULL)
		{
		fprintf(stderr,"secondary house failed\n");
		return (NO);
		}
	if ((int)(TAGSPACE=sdp_connect(tagname,ENV,NULL,RNLY)) == SDPERROR)
		{
		fprintf(stderr,"Failed to reinitialized SDP symbol space\n");
		return (NO);
		}
	tag_ptr = (TAGITEM *) sdp_use(TAGSPACE,tagid,RNLY);

	/*
	 * build table
	 */

	for ( i=0L; i < tag_sum; i++) {
		tbl_ptr->oldlcn = tag_ptr->oldloc;
		tbl_ptr->newlcn = tag_ptr->newloc;
		if ( tag_ptr->p_realtag == 0L)
			tbl_ptr->real_loc = tbl_ptr->newlcn;
		else {
			tag_ptr2 = (TAGITEM *)sdp_use(TAGSPACE,tag_ptr->p_realtag,RNLY);
			tbl_ptr->real_loc = tag_ptr2->newloc;
			sdp_unuse(TAGSPACE,tag_ptr->p_realtag,RNLY);
			}

		++tbl_ptr;
		sdp_unuse(TAGSPACE,tagid,RNLY);
		tagid = tag_ptr->next_ti;
		if ( (i+1) < tag_sum )
			tag_ptr = (TAGITEM *) sdp_use(TAGSPACE,tagid,RNLY);
	}

	return( YES );
}
