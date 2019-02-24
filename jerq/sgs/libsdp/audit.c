static char ID[] = "@(#) audit.c: 1.1 3/16/82";
#include <stdio.h>
#include "sdp.h"
#include "define3.h"

#ifdef SHORT
#define	AU01	"AU01 "
#define	AU02	"AU02 "
#define	AU03	"AU03 "
#define	AU04	"AU04 "
#define	AUAU	"AUAU "
#else
#define AU01	"%s","AU01 -- audit failure in global hash\n"
#define AU02	"%s","AU02 -- audit failure in space hash\n"
#define AU03	"%s","AU03 -- audit failure in environment hash\n"
#define AU04	"%s","AU04 -- audit failure in manager hash\n"
#define AUAU	"%s","AUAU -- audit failed in sdp_audit\n"
#endif

#define PROLOG		if( (aud_flag == AUDITON) && (aud_check() == ERROR) )\
				ERR_RET(AUAU,ERROR)
#define EPILOG		if(aud_flag == AUDITON)\
				aud_set() ;\
			if(hist_fp != NULL)\
				fprintf(hist_fp,"AU\t%d\n",mode) ;
#define RADIX	109

extern int aud_flag ;
extern long compatible ;
extern long gl1_hash ;
extern long gl2_hash ;
extern struct ENVIRON *first_environ ;
extern struct SPACE *first_space ;
extern int Gnumbfile ;
extern int Gln_file[] ;
extern int Gnumbstax ;
extern int Gsz_stax[] ;
extern FILE *hist_fp ;
extern ITEMID *known_idptr ;
extern struct LIBFILE lib_file ;
extern struct LIBMEM lib_mem ;

int					/* SUCCESS or ERROR */
sdp_audit(mode)
int mode ;
{
#ifndef SWEET

	PROLOG
	switch(mode)
	{
		case AUDITNOW:
			break ;

		case AUDITOFF:
			aud_flag = AUDITOFF ;
			aud_reset() ;
			break ;

		case AUDITON:
			aud_flag = AUDITON ;
			break ;
	}
	EPILOG
	return(SUCCESS) ;
#endif
}
aud_set()
{
#ifndef SWEET
	struct SPACE *i ;
	struct ENVIRON *j ;
	int k ;
	long hash1_gl(), hash2_gl(), hash1_sp(), hash2_sp(), hash_en(), hash_mn() ;

	gl1_hash = hash1_gl() ;
	gl2_hash = hash2_gl() ;

	for(i=first_space; i!=NULL; i=i->next_space)
	{
		i->sp1_hash = hash1_sp(i) ;
		i->sp2_hash = hash2_sp(i) ;
	}

	for(j=first_environ; j!=NULL; j=j->next_environ)
	{
		j->en_hash = hash_en(j) ;
		for(k=0; k<=j->num_frames; k++)
			(j->manp[k]).mn_hash = hash_mn(&(j->manp[k])) ;
	}
#endif
}
int					/* SUCCESS or ERROR */
aud_check()
{
#ifndef SWEET
	struct SPACE *i ;
	struct ENVIRON *j ;
	int k ;
	long hash1_gl(), hash2_gl(), hash1_sp(), hash2_sp(), hash_en(), hash_mn() ;

	if( gl1_hash != hash1_gl()
	 || gl2_hash != hash2_gl() )
		ERR_RET(AU01,ERROR)

	for(i=first_space; i!=NULL; i=i->next_space)
		if( i->sp1_hash != hash1_sp(i)
		 || i->sp2_hash != hash2_sp(i) )
			ERR_RET(AU02,ERROR)

	for(j=first_environ; j!=NULL; j=j->next_environ)
	{
		if( j->en_hash != hash_en(j) )
			ERR_RET(AU03,ERROR)
		for(k=0; k<=j->num_frames; k++)
			if( (j->manp[k]).mn_hash != hash_mn(&(j->manp[k])) )
				ERR_RET(AU04,ERROR)
	}
	return(SUCCESS) ;
#endif
}

#ifndef SWEET
aud_reset()
{

	struct SPACE *i ;
	struct ENVIRON *j ;
	int k ;

	gl1_hash = HASHNULL ;
	gl2_hash = HASHNULL ;

	for(i=first_space; i!=NULL; i=i->next_space)
	{
		i->sp1_hash = HASHNULL ;
		i->sp2_hash = HASHNULL ;
	}

	for(j=first_environ; j!=NULL; j=j->next_environ)
	{
		j->en_hash = HASHNULL ;
		for(k=0; k<j->num_frames; k++)
			(j->manp)[k].mn_hash = HASHNULL ;
	}
}
long					/* hash value */
hash1_gl()
{
	register long	value ;

	value = (long)first_environ ;
	value *= RADIX ;
	value += (long)first_space ;
	value *= RADIX ;
	value += compatible ;
	value *= RADIX ;
	value += (long)Gnumbfile ;
	value *= RADIX ;
	value += (long)Gnumbstax ;
	value *= RADIX ;
	value += (long)hist_fp ;
	value *= RADIX ;
	value += (long)aud_flag ;
	value *= RADIX ;
	value += (long)known_idptr ;
	value *= RADIX ;
	value += (long)lib_file.libbuildname ;
	value *= RADIX ;
	value += (long)lib_file.libclose ;
	value *= RADIX ;
	value += (long)lib_file.libcreate ;
	value *= RADIX ;
	value += (long)lib_file.libopen ;
	value *= RADIX ;
	value += (long)lib_file.libread ;
	value *= RADIX ;
	value += (long)lib_file.libseek ;
	value *= RADIX ;
	value += (long)lib_file.libunlink ;
	value *= RADIX ;
	value += (long)lib_file.libwrite ;
	value *= RADIX ;
	value += (long)lib_mem.liballoc ;
	value *= RADIX ;
	value += (long)lib_mem.libfree ;
	return(value) ;
}

long					/* hash value */
hash2_gl()
{
	register long	value ;
	register int	i ;

	value = 0 ;
	for(i=0; i<Gnumbfile; i++)
	{
		value *= RADIX ;
		value += Gln_file[i] ;
	}
	for(i=0; i<Gnumbstax; i++)
	{
		value *= RADIX ;
		value += Gsz_stax[i] ;
	}
	return(value) ;
}
long					/* hash value */
hash_en(env)
struct ENVIRON *env ;
{
	register long	value ;

	value = (long)env->num_frames ;
	value *= RADIX ;
	value += (long)env->frame_size ;
	value *= RADIX ;
	value += (long)env->cnct_count ;
	value *= RADIX ;
	value += (long)env->manp ;
	value *= RADIX ;
	value += (long)env->empties ;
	value *= RADIX ;
	value += (long)env->next_environ ;
	value *= RADIX ;
	value += env->stats.uses ;
	value *= RADIX ;
	value += env->stats.faults ;
	value *= RADIX ;
	value += env->stats.writes ;
	value *= RADIX ;
	value += (long)env->stats.maxuse ;
	value *= RADIX ;
	value += (long)env->stats.curuse ;
	return(value) ;
}
long					/* hash value */
hash1_sp(sp)
struct SPACE *sp ;
{
	register long	value ;

	value = sp->version ;
	value *= RADIX ;
	value += (long)sp->page_size ;
	value *= RADIX ;
	value += sp->maxpage ;
	value *= RADIX ;
	value += sp->curpage ;
	value *= RADIX ;
	value += (long)sp->curleft ;
	value *= RADIX ;
	value += (long)sp->numbfile ;
	value *= RADIX ;
	value += (long)sp->fileinfo ;
	value *= RADIX ;
	value += (long)sp->numbstax ;
	value *= RADIX ;
	value += (long)sp->staxinfo ;
	value *= RADIX ;
	value += (long)sp->head_name ;
	value *= RADIX ;
	value += (long)sp->environ ;
	value *= RADIX ;
	value += (long)sp->next_space ;
	return(value) ;
}
long					/* hash value */
hash2_sp(sp)
struct SPACE *sp ;
{
	register long	value ;
	register int	i ;

	value = 0 ;
	for(i=0; i<sp->numbfile; i++)
	{
		value *= RADIX ;
		value += sp->fileinfo[i].leng ;
		value *= RADIX ;
		value += (long)sp->fileinfo[i].desc ;
	}
	for(i=0; i<sp->numbstax; i++)
	{
		value *= RADIX ;
		value += sp->staxinfo[i].top ;
		value *= RADIX ;
		value += (long)sp->staxinfo[i].size ;
		value *= RADIX ;
		value += sp->staxinfo[i].depth ;
	}
	return(value) ;
}
long					/* hash value */
hash_mn(man)
struct MANAGER *man ;
{
	register long	value ;

	value = (long)man->space ;
	value *= RADIX ;
	value += man->page_id ;
	value *= RADIX ;
	value += man->usecount ;
	value *= RADIX ;
	value += (long)man->ref_chng ;
	value *= RADIX ;
	value += (long)man->forward ;
	value *= RADIX ;
	value += (long)man->backward ;
	value *= RADIX ;
	value += (long)man->frame_pntr ;
	return(value) ;
}
#endif
