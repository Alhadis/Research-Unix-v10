static char ID[] = "@(#) chstack.c: 1.1 3/16/82";
#include <stdio.h>
#include "sdp.h"
#include "define2.h"
#include "define3.h"

#define CS01	"CS01 "
#define CS02	"CS02 "
#define CS03	"CS03 "
#define CS04	"CS04 "
#define CS05	"CS05 "
#define CS06	"CS06 "


int
sdp_chstack(sp,c,v)
struct	SPACE	*sp ;
unsigned	c ;
int		v[] ;
{
	ITEMID			id ;
	struct	STAXINFO	*ptr ;
	struct	SPACE		tmp_sp ;
	int			i ;

	if ( c > 1 )
		for ( i=1 ; i<c ; i++ )
			if ( v[i] - v[i-1] < sizeof(ITEMID) )
				ERR_RET(CS01,ERROR)

	tmp_sp = *sp ;
	sp->staxinfo = (struct STAXINFO *)ALLOC((c+1)*sizeof(struct STAXINFO)) ;
	if ( sp->staxinfo == NULL )
		ERR_RET(CS02,ERROR)
	sp->numbstax = c+1 ;

	sp->staxinfo[0].top	= IDNULL ;
	sp->staxinfo[0].size	= 0 ;
	sp->staxinfo[0].depth = 0 ;
	for ( i=1 ; i<c+1 ; i++ )
	{
		sp->staxinfo[i].top   = IDNULL ;
		sp->staxinfo[i].size  = v[i-1] ;
		sp->staxinfo[i].depth = 0 ;
	}

	for ( i=0 ; i<tmp_sp.numbstax ; i++ )
		while ( (id=pop_id(&tmp_sp,tmp_sp.staxinfo[i].size)) != IDNOTFOUND )
			if ( id == IDNULL )
				ERR_RET(CS03,ERROR)
			else
			{
				ptr = (struct STAXINFO *)sdp_use(sp,id,RNLY) ;
				if ( ptr == NULL )
					ERR_RET(CS04,ERROR)
				if ( push_id(sp,id,ptr->size) == NULL )
					ERR_RET(CS05,ERROR)
				if ( sdp_unuse(sp,id,RNLY) == NULL )
					ERR_RET(CS06,ERROR)
			}
	return(SUCCESS) ;
}
