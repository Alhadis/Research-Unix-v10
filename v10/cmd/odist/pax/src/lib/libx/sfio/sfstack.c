#include	"sfhdr.h"


/*	Push/pop streams
**
**	Written by Kiem-Phong Vo (07/10/90)
*/

#if __STDC__ || __cplusplus
Sfile_t *sfstack(reg Sfile_t* f1,reg Sfile_t* f2)
#else
Sfile_t *sfstack(f1,f2)
reg Sfile_t	*f1, *f2;
#endif
{
	Sfile_t		tmp;
	reg Sfile_t	*rf;

	/* can't push on something that is currently frozen */
	if(SFFROZEN(f1) || (f2 && SFFROZEN(f2)))
		return NIL(Sfile_t*);

	/* initializing std-streams */
	if((!f1->mode && _sfmode(f1,0) < 0) ||
	   (f2 && !f2->mode && _sfmode(f2,0) < 0))
		return NIL(Sfile_t*);

	/* give access to other internal functions */
	_Sfstack = sfstack;

	if(!f2)
	{	/* popping */
		if(!f1->push)
			return NIL(Sfile_t*);
		f2 = f1->push;
	}
	else if(_Sfpmove && f1->pool && f1->pool != &_Sfpool && !f1->back)
	{	/* get something else to the front since f1 will be locked */
		for(rf = f1->fore; rf; rf = rf->fore)
		{	if(SFFROZEN(rf))
				continue;
			(*_Sfpmove)(rf,0);
			break;
		}
	}

	SFLOCK(f1);
	SFLOCK(f2);

	/* swap image */
	tmp = *f2; *f2 = *f1; *f1 = tmp;

	if(f1->pool != f2->pool)
	{	/* reset pool pointers */
		if(f1->pool)
		{	if(f1->back)
				f1->back->fore = f1;
			else	f1->pool->head = f1;
			if(f1->fore)
				f1->fore->back = f1;
		}
		if(f2->pool)
		{	if(f2->back)
				f2->back->fore = f2;
			else	f2->pool->head = f2;
			if(f2->fore)
				f2->fore->back = f2;
		}
	}

	if(f2->push != f2)
	{	/* freeze the pushed stream */
		f2->mode |= SF_PUSH;
		f2->flags &= ~(SF_GETC|SF_PUTC);
		f1->push = f2;
		rf = f1;
	}
	else
	{	/* unfreeze the just exposed stream */
		f1->mode &= ~SF_PUSH;
		SFopen(f1); sfset(f1,f1->flags,1);
		f2->push = NIL(Sfile_t*);
		rf = f2;
	}

	SFOPEN(f1);
	SFOPEN(f2);
	return rf;
}
