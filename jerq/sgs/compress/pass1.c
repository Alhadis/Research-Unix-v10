/*
 *     static char ID_frs[] = "@(#)pass1.c	1.5 4/6/84";
 */

/*  program to remove duplicate structures and unions */
/*  2 pass process;  1st pass makes comparisons     2nd pass compresses,
 *  reading complete object file and redirecting pointers    
 */

#include <stdio.h>
#include "sdp.h"
#include "sdp1.h"
#include <signal.h>
#include "filehdr.h"
#include "syms.h"
#include "storclass.h"
#include "ldfcn.h"
#include "tagitem.h"

#define YES	1
#define NO	0
#define HASHSIZE	521
#define nmcmp(a,b)	!strcmp((a),(b))


	static int	sdp_call = 0; /* indicate if sdpinit called */
	static char	sdpname[sizeof(TMPDIR)+20] = "";
	static char buf[sizeof(sdpname)*2 + 20 ];
	static int		call = 0;
	char		tagname[sizeof(TMPDIR)+20] = "";
	static char  buf2[sizeof(tagname) * 2 + 20];

/*	TAGITEM is a chain containing information needed in 
 *	comparing various tags.  there are as many tagitems as there
 *	are union and structure 
 */
ITEMID	hashtab[HASHSIZE];




		ldclose(),
		ldfhread(),
		ldohseek(),
		ldtbread(),
		ldtbseek();

ITEMID		ti_head,
		ti_end;
TAGITEM		*p;


LDFILE	 *input;

/*  variable representing total # of entries removed    */


long 	delsum = 0;
int     sbrk();
#define	ulimit(a)	1000000L
long 	szecnt = 0;
struct SPACE *TAGSPACE;
struct  ENVIRON *ENV;
struct  SPACE *SYMSPACE;
int     pagesize=512;  /* the size of a page for sdp  */
int     framsz=4096;    /* the size of a frame for sdp  */
long	reserved;	/* the size of core excluded from sdp */
long    allocated;      /* amount of core used for sdp */
int     framect;

char	*str_table;	/* all names are copied to here for storage */
char	*str_next;	/* next available location in str_table */
char	*str_top;	/* last location in str_table */

/*  main opens object file and reads header info   
 */

int
frstpass(argv)

char *argv;
{
	extern char	*malloc();
	extern char	*keep_name();
	SYMENT 		symbuf;
	AUXENT 		auxbuf;
	extern long	tag_sum;
	extern int	vflag;
	extern int	pflag;
	long 		nthis, nnext;
	int 		aux;

	ti_head = 0L;
	if ( (input = ldopen(argv, NULL)) == NULL) {
		fprintf(stderr, "unable to open %s \n",argv);
		return(NO);
	}
	if (!(ISCOFF(HEADER(input).f_magic))) {
		fprintf(stderr, "%s has incorrect magic number\n", argv);
		return(NO);
	}

	if ( ldtbseek(input) == FAILURE) {
		if ( pflag > 0)
			fprintf(stderr, "could not find symbol table\n");
		return(FAILURE);
	}
	nnext = 0;
	/* call subroutine to initialize SDP for symtab and Tag      */
	allocated = ulimit(3) - sbrk(0);
	allocated = (allocated > 1048576) ? 1048576 : allocated;
	reserved = ((reserved = allocated/10) > 8192) ? reserved : 8192;
	framect =(allocated - reserved) / framsz;
	if (framect < 2)
		{
		fprintf(stderr,"Insufficient core for sdp\n");
		return(NO);
		}
	free(malloc((unsigned) framect*(unsigned) framsz));
	if ((ENV=sdp_house(framect,framsz,NULL)) == NULL)
		{
		return(NO);
		}
	if (syminit() && taginit() == NO)
		return(NO);
	/*
	* get initial string table set up
	*/
	if (init_strings() == FAILURE)
		return (FAILURE);
	while( nnext < HEADER(input).f_nsyms) {
		nthis = nnext;
		if ( ldtbread(input, nthis, &symbuf) == FAILURE){
			if ( vflag > 0)
				fprintf(stderr,"cannot read symbol with index %d\n", nthis);
			return(NO);
		}
		if ( symbuf.n_numaux == 0)
			nnext = nthis + 1;
		else {
			nnext = nthis + symbuf.n_numaux + 1;
			for ( aux = 0; aux < symbuf.n_numaux; aux++)
				if ( fread(&auxbuf, AUXESZ, 1, IOPTR(input)) < 1) { 
					if ( vflag > 0)
						fprintf(stderr, "unable to read auxiliary entry\n");
					return(NO);
				}
		}

		/*  after reading an entry check to see if it is a tag   */
		if (symbuf.n_sclass== C_STRTAG || symbuf.n_sclass == C_UNTAG) {
			if ( symbuf.n_numaux != 0)
				nnext = auxbuf.x_sym.x_fcnary.x_fcn.x_endndx;

			if ((symbuf.n_nptr = keep_name(input, &symbuf)) == NULL)
				return (FAILURE);
			/*  call subroutine to store all entries of tag       */
			if ( tagstor(symbuf, auxbuf, nthis) == NO)
				return(NO);
			tag_sum++;
		}
	}	/*  end of while loop  */

	/* at end of loop all symbol table entries have been read and compared 
 	 */

	if ( nnext != HEADER(input).f_nsyms ) {
		if ( vflag > 0)
			fprintf(stderr, "error in indexing symbol table\n");
		return(NO);
	}

	ldclose(input);

	return (SUCCESS);
}

/*eject*/

/*  function tagstor saves all tag entry info so comparison can be done */

int
tagstor(symbuf, auxbuf, nthis)

SYMENT 	symbuf;
AUXENT 	auxbuf;
long 	nthis;
{

	extern long	tag_sum;
	extern int	vflag;

	extern ITEMID	compar();

	TAGITEM	 *px, *p_comp;
	SYMENT		*storbuf;
	SYMENTRY	 *symbl;
	ITEMID		id,
			id2,
			realval;
	long		 last, ncur;
	SYMENTRY	*sym;
	AUXENT		*ax;
	int		hashval;
	int		aux_flag = 0;


	last = auxbuf.x_sym.x_fcnary.x_fcn.x_endndx;
	szecnt += (long)((last-nthis)*sizeof(SYMENT));
	id = sdp_allot(TAGSPACE, sizeof(TAGITEM));
        p = (TAGITEM *) sdp_use(TAGSPACE,id,RNLY);
	szecnt += (long)(sizeof(TAGITEM));
	p->oldloc = nthis;
	p->nentrys = (unsigned)((long)last - (long)nthis);
	p->tmpsym = (SYMENT *) calloc( p->nentrys, sizeof(SYMENT) );
	storbuf = p->tmpsym;

/*  initially ptr to next tagitem is NULL;  also, dupl. ptr. is NULL  */

	p->next_ti = 0L;
	p->p_realtag = 0L;
	p->newloc = 0;

	/*  ti_head = 1st data structure in chain        */
	if(ti_head == 0L){
		ti_head = id;
		ti_end = id;
	}
	else{

		/*  not the first tag found      */
		px = (TAGITEM *) sdp_use(TAGSPACE,ti_end,WRTN);

		/*    attach new tag to chain */

		px->next_ti = id;
		sdp_unuse(TAGSPACE,ti_end,WRTN);
		ti_end = id;
	}

	/*
	 * In addition to ordered list, create hash table access
	 * for speed
	 */

	hashval = hash(symbuf.n_nptr);
	p->next_col = hashtab[hashval];	/* may be 0L */
	hashtab[hashval] = id;

	/*  copy tag into allocated area of symbol     */

	*(p->tmpsym) = symbuf;
	++(p->tmpsym);
	*(AUXENT *)(p->tmpsym) = auxbuf;

	/*  proceed to read all members and .eos    */

	for ( ncur = nthis+symbuf.n_numaux+1; ncur < last; ncur++) {
		++(p->tmpsym);
		if( ldtbread( input, ncur, &symbuf) == FAILURE) {
			if ( vflag > 0)
				fprintf(stderr, "unable to read symbol at index %ld\n", ncur);
			return(NO);
		}
		/*
		* Check to see if this is an aux or not.  Only save the
		* name when it is not.  Assume that the first entry is a
		* non-aux entry and keep track from there.
		*/
		if (aux_flag == 0)
		{
			if ((symbuf.n_nptr = keep_name(input, &symbuf)) == NULL)
				return (FAILURE);
			aux_flag = symbuf.n_numaux;
		}
		else
			aux_flag--;
		*(p->tmpsym) = symbuf;
	}

	p->tmpsym = storbuf;
	p_comp = p;

	/*  compare for duplicate tags    */

	p->newloc = p->oldloc - delsum;
	if(( realval = compar(p_comp)) != 0L) {
		cfree(storbuf);
		p->tmpsym = NULL;
		p->tagbuf = 0L;
		delsum = delsum + (p->nentrys) - 1;
		p->p_realtag = realval;
		sdp_unuse(TAGSPACE,id, RNLY);
		return(SUCCESS);
	}

	/*
	 * store symbol table info permanently in SDP
	 */

	p->tmpsym = storbuf;
	p->tagbuf = sdp_allot(SYMSPACE,sizeof(SYMENTRY));
	symbl = (SYMENTRY *) sdp_use(SYMSPACE,p->tagbuf,WRTN);
	symbl->symbuf = *(p->tmpsym);
	symbl->auxbuf = sdp_allot(SYMSPACE,sizeof(AUXENT));
	ax = (AUXENT *) sdp_use(SYMSPACE,symbl->auxbuf,WRTN);
	++(p->tmpsym);
	*ax = *((AUXENT *)p->tmpsym);
	sdp_unuse(SYMSPACE,symbl->auxbuf, WRTN);
	id2 = p->tagbuf;
	for ( ncur = nthis+2; ncur < last; ncur++) {
                symbl->nxtsym = sdp_allot(SYMSPACE,sizeof(SYMENTRY));
		sym = (SYMENTRY *) sdp_use(SYMSPACE,symbl->nxtsym,WRTN);
		sym->symbuf = *(++(p->tmpsym));
		if ( sym->symbuf.n_numaux != 0) {
			ncur += (long) sym->symbuf.n_numaux;
                        sym->auxbuf = sdp_allot(SYMSPACE,sizeof(AUXENT));
			ax = (AUXENT *) sdp_use(SYMSPACE,sym->auxbuf,WRTN);
			*ax = *((AUXENT *) ++(p->tmpsym));
			sdp_unuse(SYMSPACE,sym->auxbuf, WRTN);
		}
	sdp_unuse(SYMSPACE,id2, WRTN);
	id2 = symbl->nxtsym;
	symbl = sym;
}

	sdp_unuse(SYMSPACE,id2, WRTN);
	sdp_unuse(TAGSPACE,id, RNLY);
	cfree(storbuf);
	p->tmpsym = NULL;
	return(SUCCESS);
}


/*eject*/

/*  function to compare requires last tag read in and compares it against
 *  all others       
 */

ITEMID
compar(p_test)
TAGITEM *p_test;
{
TAGITEM  *p_real;
ITEMID	tmpid;

	for ( tmpid = p_test->next_col; tmpid != 0L; tmpid = p_real->next_col) {
		p_real = (TAGITEM *) sdp_use(TAGSPACE,tmpid,RNLY);
		if( pair(p_real, p_test)==YES) {
			sdp_unuse(TAGSPACE,tmpid, RNLY);
			return(tmpid);
		} else {
			sdp_unuse(TAGSPACE,tmpid, RNLY);
			}
		}
	return(0L);
}
/*eject*/
/* the function pair, does actual comparison of 2 given data structures  */


int
pair(preal, ptest)
TAGITEM *preal, *ptest;
{

	SYMENTRY	*ptr1;
	SYMENT		*ptr2;
	AUXENT *aux1, *aux2;
	int 	i,
		aux;

	ITEMID		id;

/* pair takes the elements of 2 different data structures and compares each
 * element.  if any element varies, the 2 are different and function returns
 * zero to calling routine.          */
/* if all elements are equal, the function returns one     */

	aux1 = aux2 = NULL;

	if( preal->nentrys != ptest->nentrys)
		return(NO);
	if( preal->p_realtag == ptest->p_realtag && preal->p_realtag != 0L)
		return(YES);
	if( preal->p_realtag != 0L || ptest->p_realtag != 0L)
		return(NO);

	id = preal->tagbuf;
	ptr1 = (SYMENTRY *)sdp_use(SYMSPACE,id,RNLY);
	ptr2 = ptest->tmpsym;

	/* ptr1 & ptr2 point to symbol table infor. stored.  Compare tagname  
 	 * and auxentry of each tag.                
	 */

	if( nmcmp(ptr1->symbuf.n_nptr, ptr2->n_nptr) == NO)
		if( gen(ptr1->symbuf.n_nptr) == NO || gen(ptr2->n_nptr) == NO) 
			goto failure;
	if( ptr1->symbuf.n_scnum != ptr2->n_scnum) 
		goto failure;
	if( ptr1->symbuf.n_value != ptr2->n_value) 
		goto failure;
	if( ptr1->symbuf.n_sclass != ptr2->n_sclass) 
		goto failure;
	if( ptr1->symbuf.n_type != ptr2->n_type) 
		goto failure;
	if( ptr1->symbuf.n_numaux != ptr2->n_numaux) 
		goto failure;
	/*  tag names matched, now test the aux. entry for respective tags     
	 */
	if( ptr1->symbuf.n_numaux != 0) {
		aux1 = (AUXENT *) sdp_use(SYMSPACE,ptr1->auxbuf,RNLY);
		aux2 = (AUXENT *) ++(ptest->tmpsym);

		if (aux1->x_sym.x_misc.x_lnsz.x_size != aux2->x_sym.x_misc.x_lnsz.x_size)   
			goto failure;
		sdp_unuse(SYMSPACE,ptr1->auxbuf, RNLY);
		aux1 = aux2 = NULL;
	}
	aux = ptr1->symbuf.n_numaux;

	sdp_unuse(SYMSPACE,id, RNLY);
	id = ptr1->nxtsym;
	ptr1 = (SYMENTRY *)sdp_use(SYMSPACE,id,RNLY);
	ptr2 = ++(ptest->tmpsym);

	for ( i=0; i < preal->nentrys - (aux + 3); i++) {
		if( memcom(ptr1, ptr2, ptest, preal) == NO)  
			goto failure;
		if( ptr1->symbuf.n_numaux != 0) {
			i += ptr1->symbuf.n_numaux;
			++(ptest->tmpsym);
			}
		sdp_unuse(SYMSPACE,id, RNLY);
		id = ptr1->nxtsym;
		ptr1 = (SYMENTRY *)sdp_use(SYMSPACE,id,RNLY);
		ptr2 = ++(ptest->tmpsym);
	}

	/*  all members are equal; compare .eos and auxentry  */

	if( ptr1->symbuf.n_sclass != C_EOS && ptr2->n_sclass != C_EOS)   
		goto failure;
	if( ptr1->symbuf.n_value != ptr2->n_value)  
		goto failure;

	if( ptr1->symbuf.n_numaux != ptr2->n_numaux)  
		goto failure;

	if( ptr1->symbuf.n_numaux != 0) {
		for ( i = 0; i < ptr1->symbuf.n_numaux; i++) {
			aux1 = (AUXENT *) sdp_use(SYMSPACE,ptr1->auxbuf,RNLY);
			aux2 = (AUXENT *) ++(ptest->tmpsym);
			if(aux1->x_sym.x_misc.x_lnsz.x_size != aux2->x_sym.x_misc.x_lnsz.x_size) 
				goto failure;
			sdp_unuse(SYMSPACE,ptr1->auxbuf, RNLY);
			aux1 = aux2 = NULL;
		}
	}

	sdp_unuse(SYMSPACE,id, RNLY);
	return(YES);

failure:
	sdp_unuse(SYMSPACE,id, RNLY);
	if ( aux1 != NULL ) {
		sdp_unuse(SYMSPACE,ptr1->auxbuf, RNLY);
		}
	return(NO);
}


/*  memcom compares corresponding members of structures
 */

int
memcom(ptr1, ptr2, ptest, preal)

SYMENTRY *ptr1;
SYMENT	 *ptr2;
TAGITEM *ptest, *preal;
{

AUXENT *aux1, *aux2;

	if(nmcmp(ptr1->symbuf.n_nptr, ptr2->n_nptr) == NO)
		return(NO);
	if(ptr1->symbuf.n_value != ptr2->n_value)
		return(NO);
	if(ptr1->symbuf.n_scnum != ptr2->n_scnum)
		return(NO);
	if(ptr1->symbuf.n_type != ptr2->n_type)
		return(NO);
	if(ptr1->symbuf.n_sclass != ptr2->n_sclass)
		return(NO);
	if(ptr1->symbuf.n_numaux != ptr2->n_numaux)
		return(NO);
	if (ptr1->symbuf.n_numaux != 0) {
		if(BTYPE(ptr1->symbuf.n_type) == T_STRUCT || BTYPE(ptr1->symbuf.n_type) == T_UNION || ISARY(ptr1->symbuf.n_type) ) {
			aux1 = (AUXENT *) sdp_use(SYMSPACE,ptr1->auxbuf,RNLY);
			aux2 = (AUXENT *) ++ptr2;
			if(aux1->x_sym.x_misc.x_lnsz.x_size != aux2->x_sym.x_misc.x_lnsz.x_size)  {
				sdp_unuse(SYMSPACE,ptr1->auxbuf, RNLY);
				return(NO);
				}
			if ( auxcom(ptr1, aux1, aux2, ptest, preal)==NO){
				sdp_unuse(SYMSPACE,ptr1->auxbuf, RNLY);
				return(NO);
			}
			sdp_unuse(SYMSPACE,ptr1->auxbuf, RNLY);
		}
	}
	return(YES);
}


/*eject*/
/* gen checks to see if symbol name is compiler generated
*/


int
gen(px)

char *px;
{

int pos = 1;

	if(*px != '.')
		return(0);
	px++;
	pos++;
	while(*px >= '0' && *px <= '9') {
		px++;
		pos++;
	}
	if(pos== 1 || pos > 5)
		return(0);
	if(*px != 'f' || *(px+1) != 'a' || *(px+2) != 'k' || *(px+3) != 'e')
		return(0);
	return(1);
}


/*eject*/
/* auxcom compares corresponding aux.entries for members being tested    */

auxcom(ptr1, aux1, aux2, ptest, preal)

SYMENTRY *ptr1;
AUXENT *aux1, *aux2;
TAGITEM *ptest, *preal;
{
	SYMENTRY *check1, *check2;
	SYMENT	*check2x;
	int	useid;
	ITEMID	axid,axid2, tmpid, tmpid2;

	useid = 0;

    if(BTYPE(ptr1->symbuf.n_type) == T_STRUCT || BTYPE(ptr1->symbuf.n_type) == T_UNION) {  
	if(aux1->x_sym.x_tagndx ==preal->oldloc && aux2->x_sym.x_tagndx == ptest->oldloc) 
		return(YES);
	if(aux1->x_sym.x_tagndx ==preal->oldloc)    {
		axid = preal->tagbuf;
		check1 = (SYMENTRY *)sdp_use(SYMSPACE,axid,WRTN);
	} else {
		axid = ti_head;
                for (preal = (TAGITEM *)sdp_use(TAGSPACE,axid,WRTN); preal->next_ti != 0L; preal = (TAGITEM *)sdp_use(TAGSPACE,axid,WRTN)) {
			if(preal->oldloc == aux1->x_sym.x_tagndx) {
				if (preal->tagbuf == 0L)  {
					axid2 = preal->p_realtag;
					sdp_unuse(TAGSPACE,axid, WRTN);
					axid = axid2;
					preal = (TAGITEM *) sdp_use(TAGSPACE,axid,WRTN);
				}
				sdp_unuse (TAGSPACE,axid, WRTN);
				axid = preal->tagbuf;
				check1 = (SYMENTRY *)sdp_use(SYMSPACE,axid,WRTN);
				break;
			} else {
				axid2 = preal->next_ti;
				sdp_unuse (TAGSPACE,axid, WRTN);
				axid = axid2;
				}
			}
		}
	/* At this point axid is locked and check1 points to something resonable	 */
	if ( aux2->x_sym.x_tagndx == ptest->oldloc) {
		check2x = ptest->tmpsym;
	} else {
		useid = 1;
		tmpid = ti_head;
		for (ptest = (TAGITEM *)sdp_use(TAGSPACE,tmpid,WRTN); ptest->next_ti != 0L; ptest = (TAGITEM *)sdp_use(TAGSPACE,tmpid,WRTN)) {
			if(ptest->oldloc == aux2->x_sym.x_tagndx) {
				if(ptest->tagbuf == 0L) {
					tmpid2 = ptest->p_realtag;
					sdp_unuse(TAGSPACE,tmpid, WRTN);
					tmpid = tmpid2;
					ptest = (TAGITEM *) sdp_use(TAGSPACE,tmpid,WRTN);
					}
			sdp_unuse (TAGSPACE,tmpid, WRTN);
			tmpid  = ptest->tagbuf;
			check2 = (SYMENTRY *)sdp_use(SYMSPACE,tmpid,WRTN);
			break;
			} else {
				tmpid2 = ptest->next_ti;
				sdp_unuse (TAGSPACE,tmpid, WRTN);
				tmpid = tmpid2;
				}
		}
	}
		if (useid) {
			if( check1 == check2) {
	
				sdp_unuse (SYMSPACE,axid, WRTN);
				sdp_unuse(SYMSPACE,tmpid, WRTN);
				return(YES);
				}
			if(nmcmp(check1->symbuf.n_nptr, check2->symbuf.n_nptr)==NO)
				if(gen(check1->symbuf.n_nptr) == 0 && gen(check2->symbuf.n_nptr) == 0) {
					sdp_unuse(SYMSPACE,axid, WRTN);
					sdp_unuse(SYMSPACE,tmpid, WRTN);
					return(NO);
					}
			if(check1->symbuf.n_value != check2->symbuf.n_value)  {
					sdp_unuse(SYMSPACE,axid, WRTN);
					sdp_unuse(SYMSPACE,tmpid, WRTN);
					return(NO);
				}
			if(check1->symbuf.n_type != check2->symbuf.n_type) {
					sdp_unuse(SYMSPACE,axid,WRTN);
					sdp_unuse(SYMSPACE,tmpid, WRTN);
				return(NO);
				}
			if(check1->symbuf.n_sclass != check2->symbuf.n_sclass) {
					sdp_unuse(SYMSPACE,axid, WRTN);
					sdp_unuse(SYMSPACE,tmpid, WRTN);
				return(NO);
				}
	
			if(check1->symbuf.n_numaux != check2->symbuf.n_numaux) {
					sdp_unuse(SYMSPACE,axid, WRTN);
					sdp_unuse(SYMSPACE,tmpid, WRTN);
				return(NO);
				}
		sdp_unuse(SYMSPACE,axid, WRTN);
		sdp_unuse(SYMSPACE,tmpid, WRTN);
		return(YES);
		}	/* if useid */

		/*
		 * comparing SDP with internal storage
		 */
		if(nmcmp(check1->symbuf.n_nptr, check2x->n_nptr)==NO)
			if(gen(check1->symbuf.n_nptr) == 0 && gen(check2x->n_nptr) == 0) {
				sdp_unuse(SYMSPACE,axid, WRTN);
				return(NO);
				}
		if(check1->symbuf.n_value != check2x->n_value)  {
				sdp_unuse(SYMSPACE,axid, WRTN);
				return(NO);
			}
		if(check1->symbuf.n_type != check2x->n_type) {
			sdp_unuse(SYMSPACE,axid,WRTN);
			return(NO);
			}
		if(check1->symbuf.n_sclass != check2x->n_sclass) {
			sdp_unuse(SYMSPACE,axid, WRTN);
			return(NO);
			}

		if(check1->symbuf.n_numaux != check2x->n_numaux) {
			sdp_unuse(SYMSPACE,axid, WRTN);
			return(NO);
			}
		sdp_unuse(SYMSPACE,axid, WRTN);
		return(YES);

    }	/* if BTYPE */
    return(YES);
}

int
syminit()
{
	extern int	sprintf();
	extern int	mktemp();
	extern int	fork();

	if (sdp_call==2) {
		fprintf(stderr,"Attemp to re-initialize SDP\n");
		return(NO);
		}
	sdp_call++;
	sprintf(sdpname, "%s/%s", TMPDIR, "symXXXX");
	mktemp(sdpname);
	if ((sdp_generate(sdpname,pagesize,NULL,0777)) == SDPERROR)
			{
			return(NO);
			}

	if ((SYMSPACE = sdp_connect(sdpname,ENV,NULL,WRTN)) == NULL)
		{
		fprintf(stderr,"Failed to initialize SDP symbol space");
		return(NO);
		}
	else
		{
		sdp_call++;
		sdp_allot(SYMSPACE,4); /* so itemid never 0L */
		return(YES);
		}
}


int
symfini()
{


/*    Ignore BREAK, HANGUP and QUIT signals, to insure cleanup is finished
 */
	/*
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);

	*/
	if (sdp_call==2) {
		sdp_call--;
		if (sdp_disconnect(SYMSPACE) == SDPERROR)
			{
			fprintf(stderr,"Failed to close SDP");
			return(NO);
			}
		if (sdp_destroy(sdpname,NULL) == SDPERROR)
			{
			fprintf(stderr,"Failed to close SDP");
			return(NO);
			}
		}
	if (sdp_call != 1) {
		return(YES);
		}
	sdp_call--;

return(YES);
}

int
taginit()
{

	extern  int		sprintf(),
				mktemp();
	

	if (call) {
		fprintf(stderr,"Attempt to re-init SDP for Tag");
		return(NO);
		}

	sprintf(tagname, "%s/%s", TMPDIR, "tgXXXX");
	mktemp(tagname);

	call++;

	if ((sdp_generate(tagname,pagesize,NULL,0777)) == SDPERROR)
			{
			return(NO);
			}

	if ((TAGSPACE = sdp_connect(tagname,ENV,NULL,WRTN)) == NULL)
		{
		fprintf(stderr,"Failed to initialize SDP Tag space");
		return(NO);
		}
	else
		{
		sdp_allot(TAGSPACE,4); /* so that itemid never 0L */
		call++;
		return(YES);
		}
}

int
tagfini()
{

	/*
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	*/

	if (call==2) {
		call--;
		if (sdp_disconnect(TAGSPACE) == SDPERROR)
			{
			fprintf(stderr,"Failed to close SDP for tag");
			return(NO);
			}
		if (sdp_destroy(tagname,NULL) == SDPERROR)
			{
			fprintf(stderr,"Failed to close SDP for TAG");
			return(NO);
			}
		}

	if (call != 1)
		return(YES);
	call--;
return(YES);
}
hash(name)
register char *name;
{

/*
 * Compute hash value for a symbol name
 */

	register unsigned hashval;
	register int symlenth;
	int tmp;	/* VAX CC BUG KLUDGE */

	hashval = symlenth = 0;
	while( (*name != '\0')  &&  (symlenth < 8) ) {
		hashval = hashval*13 + *name++;
		symlenth++;
		}

/*
 * Because of a bug in the current VAX compiler, the following
 * line has temporarily been replaced by the KLUDGE following
 * this comment.  The define of abs() is also added above
 *
 *	return( (int) (hashval % HASHSIZE) );
 */

	tmp = (int) hashval;
	tmp = abs(tmp);
	return ( tmp % HASHSIZE );
}

int
init_strings()
{
	extern char *malloc();

	if ((str_table = malloc((unsigned)(BUFSIZ * 10))) == NULL)
	{
		fprintf(stderr, "Insufficient memory for string table\n");
		return (FAILURE);
	}
	str_next = str_table;
	str_top = str_table + BUFSIZ * 10;
	return (SUCCESS);
}

char *
keep_name(f, symptr)	/* place name into permanent string table */
	LDFILE *f;
	SYMENT *symptr;
{
	register char *p, *s;
	register int i, j;
	extern char *ldgetname();
	extern char *realloc();

	if ((p = ldgetname(f, symptr)) == NULL)
	{
		fprintf(stderr, "Name retrieval error\n");
		return (NULL);
	}
	i = strlen(p) + 1;
	if (str_next + i >= str_top)
	{
		str_top += BUFSIZ * 5;
		s = str_table;
		str_table = realloc(s, (unsigned)(str_top - s));
		if (str_table == NULL)
		{
			fprintf(stderr,
				"Insufficient memory to grow string table\n");
			return (NULL);
		}
		j = str_table - s;
		str_next += j;
		str_top += j;
	}
	(void)strcpy(str_next, p);
	s = str_next;
	str_next += i;
	return (s);
}
