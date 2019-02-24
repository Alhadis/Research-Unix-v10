#include	"sfhdr.h"

/*	Management of pools of streams.
**	If pf is not nil, f is pooled with pf and f becomes current;
**	otherwise, f is isolated from its pool. flag can be any of:
**	0, SF_READ, SF_WRITE or SF_READ|SFWRITE to indicate the type
**	of stream that gets synced when a stream becomes non-current.
**	If flag is 0, the pool type is unchanged.
**
**	Written by Kiem-Phong Vo (6/27/90).
*/

static Sfpool_t		*Pfree;	/* list of free pools */

#if __STDC__ || __cplusplus
static void delpool(reg Sfpool_t* p)
#else
static delpool(p)
reg Sfpool_t	*p;
#endif
{
	if(p == &_Sfpool)
		return;
	if(p->fore)
		p->fore->back = p->back;
	if(p->back)
		p->back->fore = p->fore;
	p->fore = Pfree;
	Pfree = p;
}

#if __STDC__ || __cplusplus
static Sfpool_t *newpool(reg int mode)
#else
static Sfpool_t *newpool(mode)
reg int	mode;
#endif
{
	reg Sfpool_t	*p;

	if(Pfree)
	{	/* from the free list */
		p = Pfree;
		Pfree = p->fore;
	}
	else	p = (Sfpool_t*) malloc(sizeof(Sfpool_t));

	p->mode = mode;
	p->head = NIL(Sfile_t*);

	/* insert into chain of pools */
	if((p->fore = _Sfpool.fore) != NIL(Sfpool_t*))
		p->fore->back = p;
	p->back = &_Sfpool;
	_Sfpool.fore = p;
	return p;
}

#if __STDC__ || __cplusplus
static sfpmove(reg Sfile_t* f, reg int type)
#else
static sfpmove(f,type)
reg Sfile_t	*f;
reg int		type;	/* <0 : deleting, 0: move-to-front, >0: inserting */
#endif
{
	reg Sfpool_t	*p;
	reg Sfile_t	*head;

	SFLOCK(f);
	p = f->pool;
	if(type < 0)
	{	/* deleting from pool */
		if(f->fore)
			f->fore->back = f->back;
		if(f->back)
			f->back->fore = f->fore;
		else	p->head = f->fore;
		f->pool = NIL(Sfpool_t*);

		if(p != &_Sfpool)
		{	if(!(head = p->head))
				delpool(p);
			else if(!head->fore)
			{	/* remove the last one */
				sfpmove(head,-1);
				head->pool = &_Sfpool;
				sfpmove(head,1);
			}
		}
	}
	else if(type > 0)
	{	/* insertion */
		if((head = p->head) != NIL(Sfile_t*))
		{	/* right after current head */
			if((f->fore = head->fore) != NIL(Sfile_t*))
				f->fore->back = f;
			f->back = head;
			head->fore = f;
			f->mode |= SF_POOL;
			SFopen(f);
			sfset(f,f->flags,1);
		}
		else	
		{	f->fore = f->back = NIL(Sfile_t*);
			p->head = f;
		}
	}
	else if(p != &_Sfpool && (head = p->head) && head != f && !SFFROZEN(head))
	{	/* syn head stream before moving it */
		if(head->mode&p->mode)
			sfsync(head);
		head->mode |= SF_POOL;
		sfset(head,head->flags,1);

		if(f->fore)
			f->fore->back = f->back;
		f->back->fore = f->fore;
		f->back = NIL(Sfile_t*);
		f->fore = head;
		head->back = f;
		p->head = f;
		f->mode &= ~SF_POOL;
	}

	SFOPEN(f);
	return 0;
}

#if __STDC__ || __cplusplus
sfpool(reg Sfile_t* f, reg Sfile_t* pf, reg int mode)
#else
sfpool(f,pf,mode)
reg Sfile_t	*f;
reg Sfile_t	*pf;
reg int		mode;
#endif
{
	reg Sfpool_t	*p;

	/* function to move a stream in a pool */
	_Sfpmove = sfpmove;

	if(!f || f == pf ||			/* nothing to do */
	   (!pf && f->pool == &_Sfpool) ||	/* already isolated */
						/* new pool but type is unknown */
	   (pf && pf->pool == &_Sfpool && (mode&SF_RDWR) == 0))
		return -1;

	if(SFFROZEN(f) || (!f->mode && _sfmode(f,0) < 0))
		return -1;
	if(pf && (SFFROZEN(pf) || (!pf->mode && _sfmode(pf,0) < 0)))
		return -1;

	/* isolate it from current pool */
	p = f->pool;
	sfpmove(f,-1);

	if(!pf)
	{	/* add to the discrete pool */	
		f->pool = &_Sfpool;
		sfpmove(f,1);
		return 0;
	}

	if((p = pf->pool) == &_Sfpool)
	{	/* making a new pool */
		sfpmove(pf,-1);
		p = newpool(mode&SF_RDWR);
		pf->pool = p;
		sfpmove(pf,1);
	}

	if(f->pool != p)
	{	/* insert f into the pool */
		f->pool = p;
		sfpmove(f,1);
	}

	return 0;
}
