static char ID[] = "@(#) tv.c: 1.15 5/1/83";

#include <stdio.h>

#include "system.h"

#if TS || RT
#include <ar.h>
#else
#include <archive.h>
#endif

#include "reloc.h"
#include "sdpsrc/hd/define2.h"
#include "structs.h"
#include "tv.h"
#include "ldtv.h"
#include "ldfcn.h"
#include "extrns.h"
#include "list.h"
#include "sgsmacros.h"
#include "bool.h"
#include "ldmacros.h"

static FILE	*tvstream = NULL;
#if AR16WR
static char *tvbuf;
#endif
static FILE	*tvread = NULL;
static int	tvnxt;		/* next available tv slot */
long		gettvnxt();	/* finds next usable slot */
/*eject*/
tvspecdef()
{
#if TRVEC
	/*
	 * Complete definition of tvspec
	 *	called just after reading transfer file
	 */

	register unsigned spare;

#if !ONEPROC
	tvspec.tvinflnm = (char *) ((int) tvspec.tvinflnm + (int) strbase);
#endif
	spare = (unsigned) tvspec.tvosptr;
	tvspec.tvosptr = (OUTSECT *) myalloc(sizeof(OUTSECT));
	tvspec.tvosptr->oshdr.s_size = ((long) spare) * TVENTSZ;
	if ( aflag ) {
		/*
		 * Define the Output Section which will be used to 
		 * contain the tv
		 */
		tvspec.tvosptr->osflags = FILL;
		tvspec.tvosptr->osalign = 16;
		copy(tvspec.tvosptr->oshdr.s_name, _TV, 8);
		tvspec.tvosptr->oshdr.s_paddr = -1L;	/*none given */
		if (tvspec.tvrange[0]==0) /* leave slot 0 empty */
			tvspec.tvosptr->oshdr.s_size += TVENTSZ;
		}

#endif
}

#if TRVEC
tvreloc()
{

	register INSECT	*sptr;
	register int	numreloc;	/* nbr of relocation entries  */
	register SYMTAB *pglob;		/* ptr to ld symbol entry     */
	AUXTAB		*paux;		/* ptr to auxillary entry     */
	AUXENT		aux;		/* auxillary entry	      */
	SYMENT		lsym;		/* local symbol table entry   */
	SCNHDR		shdr;           /* section header	      */
	RELOC		rentry;		/* local relocation entry     */
	LDFILE		*rfd,rfdbuf, *ifd, ifdbuf;
	INFILE		*infl, *pnfl;
	char		*symname;

	/*
	 * the functions in DMERT's shared libraries have 0 in the tvndx
	 * field. To insure that all functions needing tv slots have P_TV
	 * in the tvndx field, scan all relocation entries for references
	 * to functions in shared libraries.
	 */

	rfd = &rfdbuf;
	ifd = &ifdbuf;
	IOPTR(ifd) = NULL;
	pnfl = NULL;
	infl = (INFILE *)infilist.head;
	while (infl != NULL) {

		OFFSET(ifd) = OFFSET(rfd) = infl->flfiloff;
		if ( IOPTR(ifd) == NULL) {
			if ((IOPTR(ifd) = fopen(infl->flname, "r")) == NULL ||
			   (IOPTR(rfd) = fopen(infl->flname, "r")) == NULL )
				lderror(2,0,NULL, "Can't open %s", infl->flname);
			if (infl->flfiloff == 0L)
				TYPE(ifd) = TYPE(rfd) = 0;
			else
				TYPE(ifd) = TYPE(rfd) = USH_ARTYPE;
			}
		if (TYPE(ifd) == ARTYPE) {
			if (FSEEK(ifd, 0L, BEGINNING) != OKFSEEK)
				lderror(2,0,NULL, "internal error: fail to seek to the member of %s",
				infl->flname);
			}
		if (FREAD(&(HEADER(ifd)),FILHSZ,1,ifd) != 1 )
			lderror(2,0,NULL, "fail to read file header of %s", infl->flname);
		if (HEADER(ifd).f_flags & F_RELFLG) {
			pnfl = infl;
			infl = infl->flnext;
			continue;
			}

		HEADER(rfd) = HEADER(ifd);

		for( sptr = infl->flishead; sptr != NULL; sptr = sptr->isnext ) {
			if( (numreloc = sptr->ishdr.s_nreloc) > 0 ) {
				if (FSEEK(rfd,(long) (FILHSZ+HEADER(rfd).f_opthdr+
					(sptr->isecnum-1) * SCNHSZ),BEGINNING) != OKFSEEK ||
			    	FREAD(&shdr,SCNHSZ,1,rfd) != 1 ||
			    	FSEEK(rfd,shdr.s_relptr,BEGINNING) != OKFSEEK)  {
					lderror(1,0,NULL, "Seek to relocation entries for section %.8s in file %s failed",
						sptr->ishdr.s_name, infl->flname);
					numreloc = 0;
					}
				}
			while( numreloc-- ) {
				if (fread(&rentry, RELSZ, 1, IOPTR(rfd)) != 1)
					lderror(2,0,NULL,"fail to read reloc entries of file %s",
					infl->flname);
				if( ! (ISTVRELOC(rentry.r_type)) )
					continue;
				if (FSEEK(ifd,HEADER(ifd).f_symptr+
					rentry.r_symndx * SYMESZ,BEGINNING) != OKFSEEK ||
			    	FREAD(&lsym,SYMESZ,1,ifd) != 1)
					lderror(2,0,NULL,"fail to read symtb entry %ld of file %s",
					rentry.r_symndx,infl->flname);
				if ( lsym.n_sclass != C_EXT )
					continue;	/* ignore statics */

				symname = SYMNAME( lsym );
				pglob = findsym( symname );

				if ( pglob->sment.n_scnum > 0) {
					if( pglob->sment.n_numaux > 0 ) {
						paux = getaux(pglob->smauxid);
						if  (paux->axent.x_sym.x_tvndx == 0) { 
							paux->axent.x_sym.x_tvndx = P_TV;
							tvspec.tvosptr->oshdr.s_size += TVENTSZ;
							PUTAUX(paux, 1);
							}
						else
							PUTAUX(paux, 0);
						PUTSYM( pglob, 0 );
					} else {
						zero(&aux, AUXESZ);
						aux.x_sym.x_tvndx = P_TV;
						tvspec.tvosptr->oshdr.s_size += TVENTSZ;
						putaux( makeaux(pglob, &aux, 0), 1);
						PUTSYM( pglob, 1 );
					}

				} else
					PUTSYM( pglob, 0 );
			}
		}

		pnfl = infl;
		infl = infl->flnext;

		/* see if next input is from the same archive */
		if ((pnfl->flfiloff > 0L) && (infl != NULL))
			if (strcmp(pnfl->flname, infl->flname) == 0 &&
					(pnfl->flfiloff < infl->flfiloff))
			continue;

		fclose(IOPTR(ifd));
		fclose(IOPTR(rfd));
		IOPTR(ifd) = NULL;
		}

}
#endif



tvupdat()
{
#if TRVEC
/*
 * If _TV section is not user-defined but needs to
 * be, add it to the list of output sections
 *
 *  Check if _TV section needs to be bonded. If so, add it to
 *  the bondlist
 */

	OUTSECT	*osptr;

	if ( (osptr = fndoutsec(_TV)) != NULL ) {
		++usrdeftv;
		/* redo work done in tvspecdef() */
		free( tvspec.tvosptr );
		tvspec.tvosptr = osptr;
		tvspec.tvosptr->osflags = FILL;
		tvspec.tvosptr->osalign  = 16;
		tvspec.tvosptr->oshdr.s_paddr = -1L;
	} else {
		/*
		 * User defined no .tv output section but .tv
		 * input sections may be present
		 */
		if ( ! aflag )
			return;
		if ( usrdeftv ) /* there are .tv input sections */
			if ( tvspec.tvrange[0] == 0 )
				tvspec.tvosptr->oshdr.s_size -= TVENTSZ;
		++numoutsc;
		listadd(l_OS, &outsclst, tvspec.tvosptr);

		if( tvspec.tvbndadr != -1L ) {
			register ACTITEM *aptr;

			aptr = (ACTITEM *) myalloc(sizeof(ACTITEM));
			aptr->bond.aitype = AIBOND;
			aptr->bond.aiinflnm = tvspec.tvinflnm;
			aptr->bond.aiinlnno = tvspec.tvinlnno;
			aptr->bond.aioutsec = tvspec.tvosptr;
			aptr->bond.aiadrbnd = tvspec.tvbndadr;
			listadd(l_AI, &bondlist, aptr);
			}
		}
#endif
}
/*eject*/
tvupdt2()
{
#if TRVEC
	OUTSECT	*osptr;
	long	*lptr;

	if ( usrdeftv ) {
		osptr = fndoutsec(_TV);
		/*
		 * Assume RANGE(0,N)
		 */
		tvspec.tvlength = osptr->oshdr.s_size / TVENTSZ;
		if ( tvspec.tvrange[1] == 0 ) /* no RANGE directive */
			tvspec.tvrange[1] = tvspec.tvlength - 1;
	} else {
		/*
		 * Make sure that TV directive is adequate
		 * and fill in missing spaces
		 */

		lptr = &tvspec.tvosptr->oshdr.s_size;
		if ((tvspec.tvrange[1]!=0)
		  &&((long)((tvspec.tvrange[1]-tvspec.tvrange[0]+(tvspec.tvrange[0]? 1 : 2))*TVENTSZ)<*lptr))
			lderror(2,0,tvspec.tvinflnm, "tv range allows %d entries; %d needed",
					tvspec.tvrange[1]-tvspec.tvrange[0]+1,
					(int) (*lptr/TVENTSZ));
		if( (((long) tvspec.tvlength) * TVENTSZ) >= *lptr )
			*lptr = ((long) tvspec.tvlength) * TVENTSZ;
		else	if( tvspec.tvlength == 0 )
				tvspec.tvlength = *lptr / TVENTSZ;
			else
				lderror(1,0,NULL, "tv needs %ld entries; only %d allowed",
					*lptr/TVENTSZ, tvspec.tvlength);
		if (tvspec.tvrange[1] == 0)
			tvspec.tvrange[1] = tvspec.tvlength - 1;
		}
#endif
}
/*eject*/
tvinit()
{
#if TRVEC

/*
 * Initialize for writing the transfer vector
 */

	register SYMTAB	*pglob;
	register AUXTAB	*paux;
	register int	indx;
	SCNHDR	*psect;
	TVENTRY	outslot;
	AUXENT  aux;
	long	offset;		/* where to write slot	*/
	int	changed;


	if( (tvstream = fopen(outfilnm, "r+")) == NULL )
		lderror(2,0,NULL, "can't open %s to write the tv",
			outfilnm);
#if AR16WR
	tvbuf = myalloc( BUFSIZ );
	setbuf( tvstream, tvbuf );
#endif

	fseek(tvstream, tvspec.tvosptr->oshdr.s_scnptr, 0);
	for ( indx = tvspec.tvrange[0]; indx <= tvspec.tvrange[1]; ++indx)
		fwrite(&tvspec.tvfill, TVENTSZ, 1, tvstream);

/*
 *	For each tv slot assignment directive, write out that slot
 *	if it is legal and update the ld symbol table appropriately.
 *
 *	TV slot assignment directives are of the form "func=n" where
 *	func is a function name and n is a transfer vector index. Another
 *	directive specifies a RANGE for the current load, meaning that
 *	all functions defined in this load are to be assigned slots within
 *	the given range. Slot assignment directives for functions outside
 *	the given range are for use in resolving references to functions
 *	defined in another subsystem. Ld depends on the veracity and
 *	competence of the user to insure that this is, after all, the
 *	slot index assigned to this function in another subsystem load.
 *
 *	Certain conditions result in errors.
 *	1) If the user specifies a LENGTH for the total transfer vector,
 *		a) if more slots are required than the given LENGTH allows,
 *	or	b) if an ASSIGN directive is issued which lies beyond the
 *		   specified LENGTH
 *	   then ERROR
 *	2) If the user ASSIGNs a slot S to a function which is undefined
 *	   in the current load, then
 *		if  R1 <= S <= R2
 *			then ERROR
 *		{where R1 is n in the directive RANGE(n,m) or 0;
 *		       R2 is m in the directive RANGE(n,m) or LENGTH; }
 *	3) If the user ASSIGNs a slot to a function which is defined
 *	   in the current load, then
 *		if NOT R1 <= S <= R2
 *			then ERROR
 *
 *	Otherwise the assigned function slot is either used to update
 *	the ld symbol table and/or written to the a.out file or, if the
 *	assigned function is unreferenced in the current load, ignored.
 */

#if ONEPROC
	while (tvslot1)  {
#else
	tvslot1 = (TVASSIGN *) myalloc(sizeof(TVASSIGN));

	while( tvslotcnt-- ) {
		fread( tvslot1, sizeof(TVASSIGN), 1, trnfdes);
#endif

		/*
		 * If the symbol has never been referenced, ignore 
		 * the ASSIGN command
		 */
		if( (pglob = findsym(tvslot1->funname)) == NULL ) {
#if ONEPROC
			tvslot1 = tvslot1->nxtslot;
#endif
			continue;
			}

		changed = 0;
		if( pglob->sment.n_scnum == 0 ) {
			/*
			 * The symbol is referenced but not defined
			 */
			if( (tvslot1->slot >= tvspec.tvrange[0]) &&
			    (tvslot1->slot <= tvspec.tvrange[1]) ) {
				PUTSYM(pglob, 0);
				lderror(2,0,tvspec.tvinflnm,
					"Undefined symbol assigned a tv slot within tv range");
				}
			else {
				if( pglob->sment.n_numaux == 0 ) {
					zero(&aux, AUXESZ);
					paux = makeaux(pglob, &aux, 0);
					changed = 1;
				} else {
					paux = getaux(pglob->smauxid);
					}
				paux->axent.x_sym.x_tvndx = P_TV;
				PUTAUX(paux, 1);
				}
			}
		else {
			/*
			 * The symbol is defined
			 */
			if( (tvslot1->slot < tvspec.tvrange[0]) ||
			    (tvslot1->slot > tvspec.tvrange[1]) ) {
				PUTSYM(pglob, 0);
				lderror(2,0,tvspec.tvinflnm,
					"Defined symbol assigned a tv slot outside tv range");
				}
			}

		/*
		 * Update ld symbol table aux entry
		 */

		if (pglob->sment.n_numaux <= 0)  {
			if (pglob->sment.n_type == 0) {
				zero( &aux, AUXESZ );
				paux = makeaux(pglob, &aux, 0);
				changed = 1;
				paux->axent.x_sym.x_tvndx = N_TV;
			} else
				lderror(2,0,tvspec.tvinflnm,
					"attempt to assign tv slot to illegal symbol");
		} else
			paux = getaux(pglob->smauxid);
		/*
		 * If a function has been defined but never referenced
		 * through a tv, then the tvndx is still N_TV, not P_TV
		 * thus we need to test for both as the user may be
		 * defining the function for use in another subsystem
		 * and this code would otherwise generate an error message
		 */
		if( paux->axent.x_sym.x_tvndx != P_TV && paux->axent.x_sym.x_tvndx != N_TV ) {
			PUTAUX(paux,0);
			PUTSYM(pglob, changed);
			lderror(2,0,tvspec.tvinflnm,
				"Two tv slot assignments for function %.8s: %d and %d",
				SYMNAME( pglob->sment ),
				paux->axent.x_sym.x_tvndx,
				tvslot1->slot);
		} else {
			paux->axent.x_sym.x_tvndx = tvslot1->slot;
			PUTAUX(paux, 1);
			}

		/*
		 * Determine if it is necessary to actually write the
		 * tv slot to the output file: does the slot fall
		 * within the specified tv range
		 */

		if( (tvslot1->slot < tvspec.tvrange[0])  ||
		    (tvslot1->slot > tvspec.tvrange[1]) ) {
			PUTSYM(pglob, changed);
#if ONEPROC
			tvslot1 = tvslot1->nxtslot;
#endif
			continue;
			}

		offset = (long) (tvspec.tvosptr->oshdr.s_scnptr + (TVENTSZ *
			  (tvslot1->slot - tvspec.tvrange[0])));
		fseek(tvstream, offset-ftell(tvstream), 1);

		setslot(pglob, psect, &outslot);

		PUTSYM(pglob, changed);
		fwrite(&outslot, TVENTSZ, 1, tvstream);
#if ONEPROC
		tvslot1 = tvslot1->nxtslot;
#endif
		}

	fflush(tvstream);
	
	/*
	 * Don't use slot 0 as it is illegal in C to have any
	 * type of valid pointer with a zero value
	 */

	tvnxt = (tvspec.tvrange[0] != 0) ? tvspec.tvrange[0] : 1;

	/*
	 * Position tv file pointers:
	 *	tvstream : start of the tv (range)
	 *	tvread   : start of first legal tv slot
	 */
	if( (tvread=fopen(outfilnm,"r")) == NULL )
		lderror(2,0,NULL,"Cannot open %s to read tv",outfilnm);
	fseek(tvstream, tvspec.tvosptr->oshdr.s_scnptr, 0);
	fseek(tvread, ((long)(tvspec.tvosptr->oshdr.s_scnptr + 
		(tvspec.tvrange[0] ? 0 : TVENTSZ))), 0);

	return;

#endif
}
/*eject*/
tvslot(pglob, psym, infile, cs)
SYMTAB		*pglob;		/* symbol is in ld symbol table entry */
struct syment	*psym;		/* symtab entry is from input file */
FILE		*infile;	/* input file stream */
unsigned short	cs;		/* segment value; used only for psym */
{

/*
 * Check whether the given symbol needs a slot
 * in the tv.  If it does and one has not be assigned,
 * assign the symbol a slot and write that entry to the
 * tv in the output file
 *
 * The symbol is passed in either a gsym or syment symbol table
 * entry data structure.  For a gsym, look in the aux entry; for a
 * syment, read the next syment from the input file.
 * In the latter case, reseek the input file pointer to the original
 * position
 *
 * The return value is:
 *
 *		-1	symbol doesn't need tv slot
 *		 0	symbol needs tv slot, but this is
 *			not an absolute load
 *		>0	tv slot needed and assigned
 */

	int		ret;
	long		offset;		/* where to write next */
	AUXTAB		*paux;
	TVENTRY		tvent;
	AUXENT		aux;

	if( ! tvflag )
		return ( -1 );

#if TRVEC
	if( pglob != NULL )
		psym = &pglob->sment;

/*
 * We are assured that all symbols needing tv slots have already
 * been detected in loadobjf() and their tvndx set to P_TV.
 * Except for static functions which are detected in psymtab
 * but their tvndx left at N_TV
 */

	if( psym->n_numaux == 0 )
		return ( -1 );

	if( pglob != NULL ) {
		/*
		 * This routine was given an ld symbol table entry
		 */
		paux = getaux(pglob->smauxid);
		if (paux->axent.x_sym.x_tvndx == 0 ) {
			PUTAUX(paux, 0);
			return ( -1 );
			}
		if( paux->axent.x_sym.x_tvndx == P_TV ) {
			/*
			 * This symbol is referenced through a tv:
			 *
			 *	tvndx == P_TV
			 */
			if( ! aflag ) {
				PUTAUX(paux, 0);
				return ( 0 );
				}
			if( pglob->smscnptr != NULL ) {

				slotassign(&tvent, pglob, &cs);

				}
			else {
				/*
				 * tv ref to undefined symbol
				 */
				PUTAUX(paux, 0);
				return( -1 );
				}
			if ( (offset = gettvnxt()) == 0L)
				lderror(2,0,NULL,"Internal error: out of tv slots");
			fseek(tvstream, offset-ftell(tvstream), 1);
			paux->axent.x_sym.x_tvndx = tvnxt;
			PUTAUX(paux, 1);
			}
		else {
			if( paux->axent.x_sym.x_tvndx == N_TV )
				ret = 0;
			else
			/*
			 * The symbol already has a tv assigned:
			 *
			 *	tvndx > 0
			 */
				ret = paux->axent.x_sym.x_tvndx;
			PUTAUX(paux, 0);
			return ( ret );
			}
		}
	else {
		/*
		 * This routine was given an input symbol table entry
		 */
		fread(&aux, AUXESZ, 1, infile);
		fseek(infile, (long) -AUXESZ, 1);
		if( aux.x_sym.x_tvndx != N_TV )
			return ( -1 );
		if( ! aflag )
			return ( 0 );
		if ( (offset = gettvnxt()) == 0L)
			lderror(2,0,NULL,"Internal error: out of tv slots");
		fseek(tvstream, offset-ftell(tvstream), 1);

		settventry(&tvent, psym, &cs);
		}

	fwrite(&tvent, TVENTSZ, 1, tvstream);

	return ( tvnxt++ );

#endif
}
/*eject*/
long
gettvnxt()
{
#if TRVEC

/*
 * This routine is called only when it is certain that we need to write
 * out a tv entry. tvinit() has written assigned slots and filled all
 * empty slots with 0. The first time this routine is called tvnxt equals
 * tvspec.tvrange[0], tvread is open for reading, the read/write
 * pointer is at the beginning of the transfer vector.
 *
 * It begins reading transfer vector entries until it finds
 * one which is unused. It then seeks TVENTSZ backwards and returns
 * the file offset of the available slot. It increments tvnxt whenever
 * it finds an unavailable slot. When next called, it will be in a position
 * to proceed reading slots, being positioned correctly and knowing
 * the value of tvnxt.
 */

	long	inslot;

	for( ; tvnxt <= tvspec.tvrange[1]; ++tvnxt ) {
		fread(&inslot, TVENTSZ, 1, tvread);
		if( inslot == -1L )
			return( (long)(ftell(tvread)-TVENTSZ) );
		}

	return(0);
#endif
}
/*eject*/
tvfinish()
{
#if TRVEC
	long	offset;
	SYMTAB	*psym;

	if( tvspec.tvfnfill[0] != '\0' ) {
		if( (psym = findsym(tvspec.tvfnfill)) == NULL )
			lderror(1, tvspec.tvinlnno, tvspec.tvinflnm,
				"tv fill symbol %s does not exist", tvspec.tvfnfill);
		else {

			filltvassign(psym);

			PUTSYM(psym, 0);

			/*
			 * update x_tvfill in .tv aux entry
			 */
			if ( (psym=findsym(_TV)) == NULL)
				lderror(0,0,NULL,"No .tv in symbol table");
			else {
				AUXTAB	*paux;
				union {
					TVENTRY ent;
					long	l;
				} fill;

				fill.ent = tvspec.tvfill;
				paux = getaux(psym->smauxid);
				paux->axent.x_tv.x_tvfill = fill.l;
				PUTAUX(paux, 1);
				PUTSYM(psym, 0);
				}

			if ( tvspec.tvrange[0] == 0 ) { /* fill slot 0 */
				fseek(tvstream, tvspec.tvosptr->oshdr.s_scnptr, 0);
				fwrite(&tvspec.tvfill, TVENTSZ, 1, tvstream);
				}

			for( offset = gettvnxt(); tvnxt <= tvspec.tvrange[1]; ) {
				fseek(tvstream, offset-ftell(tvstream), 1);
				fwrite(&tvspec.tvfill, TVENTSZ, 1, tvstream);
				++tvnxt;
				offset = gettvnxt();
				} /* for */
			} /* else */
		} /* if tvfnfill[0] */

	fclose(tvstream);
#if AR16WR
	free( tvbuf );
#endif
	fclose(tvread);

#endif
}
