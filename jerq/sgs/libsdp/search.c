static char ID[] = "@(#) search.c: 1.1 3/16/82";
#include <stdio.h>
#include "sdp.h"
#include "define3.h"


#ifdef SHORT
#define S201	"S201 "
#else
#define S201	"%s","S201 -- stack sizes incorrectly configured\n"
#endif

extern struct LIBFILE *Glib_file ;
extern struct SPACE *first_space ;
extern struct ENVIRON *first_environ ;

int					/* frame number or NOTFOUND */
framesearch(sp,id)
register struct SPACE *sp ;
ITEMID id ;
{
	register int		i ;
	register struct MANAGER	*manager ;

	manager = (sp->environ)->manp ;

	id -= id % (sp->environ)->frame_size ;
	for(i=manager[(sp->environ)->num_frames].backward; i!=(sp->environ)->num_frames; i=manager[i].backward)
		if( (manager[i].space == sp) && (manager[i].page_id == id) )
			return(i) ;
	return(NOTFOUND) ;
}

int					/* frame number or NOTFOUND */
nextsearch(environ)
register struct ENVIRON *environ ;
{
	register int		i ;
	register struct MANAGER	*manager ;

	manager = environ->manp ;

	if( (i = environ->empties) != END )
	{
		environ->empties = manager[i].forward ;
		ins_frame(environ,i,manager[environ->num_frames].forward) ;
		return(i) ;
	}

	for(i=manager[environ->num_frames].forward; i!=environ->num_frames; i=manager[i].forward)
		if(manager[i].usecount == 0l)
			return(i) ;
	return(NOTFOUND) ;
}


int					/* stack array index or NOTFOUND */
stk1search(sp,howmuch)			/* for sdp_allot */
register struct SPACE *sp ;
unsigned howmuch ;
{
	register int	i ;

	for(i=0; i<sp->numbstax; i++)
		if(sp->staxinfo[i].size >= (long)howmuch)
			return(i) ;
	return(NOTFOUND) ;
}


int					/* stack array index or NOTFOUND */
stk2search(sp,howmuch)			/* for sdp_redeem) */
register struct SPACE *sp ;
unsigned howmuch ;
{
	register int	i ;

	for(i=sp->numbstax-1; i>=0; i--)
		if((long)howmuch >= sp->staxinfo[i].size)
			return(i) ;
	ERR_RET(S201,ERROR)
}



int					/* SUCCESS or NOTFOUND */
spsearch(sp)
struct SPACE *sp;
{

	register struct SPACE	*i;

	for(i=first_space; i!=NULL; i=i->next_space)
	{
		if(sp == i)
			return(SUCCESS);
	}
	return(NOTFOUND);

}
ensearch(en)
struct ENVIRON *en;
{

	register struct ENVIRON	*i;

	for(i=first_environ; i!=NULL; i=i->next_environ)
	{
		if(en == i)
			return(SUCCESS);
	}
	return(NOTFOUND);

}
