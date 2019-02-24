static char ID[] = "@(#) ld1.c: 1.21 12/10/83";
#include "system.h"

#include <stdio.h>

#if TS || RT
#if PORTAR || PORT5AR
#define TABSZ	2000
#endif
#include <ar.h>
#else
#include <archive.h>
#endif

#include "structs.h"
#include "extrns.h"
#include "list.h"
#include "ldfcn.h"
#include "params.h"
#include "sgsmacros.h"
#include "reloc.h"
#if DMERT || XL
#include "aouthdr.h"
#include "magic.h"
#endif
#include "ldmacros.h"

#if AR32W && !UNIX
#define MAXSCNBS	20
#define MAXSYMBS	120
#else
#define	MAXSCNBS	4
#if AR16WR
#define	MAXSYMBS	30
#else
#define MAXSYMBS	60
#endif
#endif

#if TRVEC
#define OKMAGIC(x)	((unsigned short) x == (unsigned short) magic)
#else
#include "sgs.h"
#define OKMAGIC		ISMAGIC
#endif

#if M32
unsigned short macflags = 0;
#endif

int lo_scnh, hi_scnh;

/*eject*/
psymdef()
{

/*
 * Process the symlist, to define/undefine global symbols
 */

#if !ONEPROC
	ACTITEM a;
#endif
	ACTITEM *ap;

#if DEBUG
	if( dflag > 2 )
		fprintf( stderr, "\nDFNSYM data structures:" );
#endif

#if ONEPROC
	ap = (ACTITEM *) symlist.head;
	while( ap )  {
#else
	ap = &a;
	while( symcnt-- ) {
		fread( &a, 1, sizeof(ACTITEM), trnfdes );
		ap->dfnsym.aiinflnm = (char *) ((int) ap->dfnsym.aiinflnm + (int) strbase);
		ap->dfnsym.aisymbol = (char *) ((int) ap->dfnsym.aisymbol + (int) strbase);

#endif
#if DEBUG
		if( dflag > 2 )
			fprintf( stderr, "\n\t%04x AIDFNSYM (%s) %d (%s) %s %08lx",
				ap->dfnsym.ainext, ap->dfnsym.aiinflnm,
				ap->dfnsym.aiinlnno, ap->dfnsym.aisymbol,
				(ap->dfnsym.aideflag ? "DEF" : "UNDEF"),
				ap->dfnsym.aidefval );
#endif

		if( ap->dfnsym.aideflag ) {
			lderror(1, ap->dfnsym.aiinlnno, ap->dfnsym.aiinflnm,
				"inimplemented feature: value definition for symbol %.8s",
				ap->dfnsym.aisymbol );
			}
		else
			undefsm( ap->dfnsym.aisymbol );
#if ONEPROC
		ap = ap->ldlbry.ainext;
#endif
		}
}
/*eject*/
pmemreg()
{

/*
 * Input the MEMLIST and REGLIST built during PASS 1
 */

#if USEREGIONS
	register REGION *rptr;
#endif
	register MEMTYPE *mptr;
	ADDRESS length;

/*
 * If no MEMORY directives are supplied, use a default, otherwise check
 * for proper usage
 */

#if ONEPROC
	if (memlist.head == NULL )  {
#else
	if( memcnt == 0 ) {
#endif
		mptr = (MEMTYPE *) myalloc(sizeof(MEMTYPE));
		copy(mptr->mtname, "*dfltmem", 8);
#if PAGING
		if (zflag)
			memorg = max(memorg, NONULLORG);
#endif
		mptr->mtorig = memorg;
		mptr->mtlength = (MEMSIZE - memorg);
		mptr->mtattr=15;		/* all attrs set */
		listadd( l_MEM, &memlist, mptr );
		}
	else {
#if DEBUG
		if( dflag > 2 )
			fprintf( stderr, "\nMEMTYPE data structures:" );
#endif
#if !ONEPROC
		while( memcnt-- ) {
			mptr = (MEMTYPE *) myalloc(sizeof(MEMTYPE));
			fread( mptr, 1, sizeof(MEMTYPE), trnfdes );
			mptr->mtnext = NULL;
#if DEBUG
			if( dflag > 2 )
				fprintf( stderr, "\n\t%04x (%8.8s) %08lx %08lx %d %04x %04x",
					mptr->mtnext, mptr->mtname, mptr->mtorig, mptr->mtlength, mptr->mtattr,
					mptr->mtaddrhd, mptr->mtaddrtl );
#endif
			listadd( l_MEM, &memlist, mptr );
			}
#endif
		mptr = (MEMTYPE *) memlist.head;
		while ( mptr->mtnext ) {
			length = mptr->mtorig + mptr->mtlength;
			if( length > mptr->mtnext->mtorig ) {
				lderror(1,0,NULL, "memory types %.8s and %.8s overlap",
					mptr->mtname, mptr->mtnext->mtname);
				free( mptr->mtnext );
				mptr->mtnext = mptr->mtnext->mtnext;
				}
			mptr = mptr->mtnext;
			}
		}

#if USEREGIONS
/*
 * If no REGIONS directives are supplied, use a default, otherwise check
 * for proper usage
 */

#if ONEPROC
	if( reglist.head == NULL )  {
#else
	if( regcnt == 0 ) {
#endif
		if( (! iflag)  &&  (! tvflag) ) {
			/*
			 * No separate I and D space. No tv
			 */
			rptr = (REGION *) myalloc(sizeof(REGION));
			copy(rptr->rgname, "*dfltrgn", 8);
			rptr->rgorig = 0L;
			rptr->rglength = REGSIZE;
			rptr->rgvaddr = 0L;
			listadd(l_REG, &reglist, rptr);
			}
		else if( ! tvflag ) {
			/*
			 * Separate I and D space. No tv
			 */
			rptr = (REGION *) myalloc(sizeof(REGION));
			copy(rptr->rgname, "RTEXT", 8);
			rptr->rgorig = 0L;
			rptr->rglength = REGSIZE;
			rptr->rgvaddr = 0L;
			listadd(l_REG, &reglist, rptr);
			rptr = (REGION *) myalloc(sizeof(REGION));
			copy(rptr->rgname, "RDATA", 8);
			rptr->rgorig = REGSIZE + 2;
			rptr->rglength = REGSIZE - 2;
			rptr->rgvaddr = 2L;
			listadd(l_REG, &reglist, rptr);
			}
		}
	else {
#if DEBUG
		if( dflag > 2 )
			fprintf( stderr, "\nREGION data structures:" );
#endif
#if !ONEPROC
		while( regcnt-- ) {
			rptr = (REGION *) myalloc(sizeof(REGION));
			fread( rptr, 1, sizeof(REGION), trnfdes );
			rptr->rgnext = NULL;
#if DEBUG
			if( dflag > 2 )
				fprintf( stderr, "\n\t%04x (%8.8s) %08lx %08lx %08lx %04x %04x",
					rptr->rgnext, rptr->rgname, rptr->rgorig,
					rptr->rglength, rptr->rgvaddr,
					rptr->rgaddrhd, rptr->rgaddrtl );
#endif
			listadd( l_REG, &reglist, rptr );
			}
#endif
		rptr = (REGION *) reglist.head;
		while( rptr->rgnext ) {
			length = rptr->rgorig + rptr->rglength;
			if( length > rptr->rgnext->rgorig ) {
				lderror(2,0,NULL,"Can't allocate space for overlap regions %.8s and %.8s",
					rptr->rgname, rptr->rgnext->rgname);
				}
			rptr = rptr->rgnext;
			}
		}
#endif

}
/*eject*/
ploadfil()
{

/*
 * Process the LDFILE list
 */

	extern	LDFILE	*myldopen();
	extern long	sgetl();
#if !ONEPROC
	ACTITEM a;
#endif
	ACTITEM *ap;
	ARCHDR	ahdr;
	SCNHDR	scnbuf[MAXSCNBS];
	SYMENT	symbuf[MAXSYMBS];
	LDFILE	fdesbuf;
	register LDFILE *fdes;
#if AR16WR
	char	fbuf[BUFSIZ];
#endif
	register int done, count;

/*
 * For each separate file to go into some output section:
 *	1. Make sure the file is of valid type (i.e., "good magic")
 *	2. Input the file and section headers
 *	3. Scan the symbol table, counting globals, etc.
 */

#if DEBUG
	if( dflag > 2 )
		fprintf( stderr, "\nLDFILE data structures:" );
#endif
#if ONEPROC
	ap = (ACTITEM *) ldfilist.head;
	while ( ap )  {
#else
	ap = &a;
	while( ldfcnt-- ) {
		fread( &a, 1, sizeof(ACTITEM), trnfdes );
		ap->ldlbry.aiinflnm = (char *) ((int) ap->ldlbry.aiinflnm + (int) strbase);
		ap->ldlbry.aifilnam = (char *) ((int) ap->ldlbry.aifilnam + (int) strbase);
#endif
#if DEBUG
		if( dflag > 2 )
			fprintf( stderr, "\n\t%04x AILDFILE (%s) %d (%s)",
				ap->ldlbry.ainext, ap->ldlbry.aiinflnm,
				ap->ldlbry.aiinlnno, ap->ldlbry.aifilnam );
#endif
		/*
		 * The same file can appear many times on the ldlist
		 * sequence, since the input sections from it can go
		 * into many different output sections
		 *
		 * Process a NON-ARCHIVE file exactly ONCE; but,
		 * process an ARCHIVE file each time it is given
		 */
		if( (ap->ldlbry.aitype == AILDFILE)  &&  fndinfil(ap->ldlbry.aifilnam) )  {
#if ONEPROC
			ap = ap->ldlbry.ainext;
#endif
			continue;
			}

#if AR16WR
		if( (fdes = myldopen(ap->ldlbry.aifilnam,&ahdr,&fdesbuf,fbuf)) == NULL )
#else
		if( (fdes = myldopen(ap->ldlbry.aifilnam,&ahdr,&fdesbuf))  ==  NULL )
#endif
			lderror(1, ap->ldlbry.aiinlnno, ap->ldlbry.aiinflnm, "Can't open file %s",
				ap->ldlbry.aifilnam);
		else if( OKMAGIC(TYPE(fdes))) {
#if DMERT || XL
			if (HEADER(fdes).f_opthdr >= sizeof(AOUTHDR))  {
				short o_magic;
				if (FREAD(&o_magic,sizeof(o_magic),1,fdes) != 1)
					lderror(2,0,NULL,"fail to read magic number of pfile %s",
					ap->ldlbry.aifilnam);
				if (o_magic == (short) PLIBMAGIC)
					dmertplib = 1;
				}
#endif
			lo_scnh = 1;
			if ((hi_scnh = getrec2(scnbuf,fdes,1)) == 0)
				lderror(2,0,NULL,"fail to read section headers of file %s",
				ap->ldlbry.aifilnam);
			loadobjf(fdes,ap->ldlbry.aifilnam,-1,scnbuf,NULL);
#if DMERT || XL
			dmertplib = 0;
#endif
			fclose(IOPTR(fdes));
			}
		else if( TYPE(fdes) == USH_ARTYPE ) {
#if PORTAR
			/* Read in the archive symbol table and search it
			 * for unresolved symbol references.  Keep passing
			 * over the symbol table, loading in archive members
			 * as you go until one complete pass is made with no
			 * additional loads of an archive member.
			 */
			
			typedef union {	long l;
				char c[sizeof(long)/sizeof(char)];
			     } longchar;
			longchar arsymtab[TABSZ], sym_count;
			char * arsymnmptr[TABSZ];
			char found_one;
			long  sym_ar_size;
			int strtabsz;
			char *j,*strtabptr;
			int i;
			struct ar_hdr arheader;

			/* read archive header */
			if (fseek(IOPTR(fdes),(long)SARMAG,0) != OKFSEEK
			   || FREAD(&arheader,sizeof(struct ar_hdr),1,fdes) != 1
			   || strncmp(arheader.ar_fmag,ARFMAG,sizeof(arheader.ar_fmag))
			   || sscanf(arheader.ar_size, "%ld", &sym_ar_size) !=1)
				lderror(2,0,NULL,
				"can't read archive header from archive %s",
				ap->ldlbry.aifilnam);
			if (arheader.ar_name[0] != '/')
				lderror(2,0,NULL,
				"archive symbol directory is missing from archive %s\nexecute 'ar ts %s' to restore archive symbol directory",
				ap->ldlbry.aifilnam,ap->ldlbry.aifilnam);
			
			/* figure out number of archive symbols */
			if (FREAD(sym_count.c,sizeof(sym_count.c),1,fdes) != 1)
				lderror(2,0,NULL,
				"can't read archive symbol directory number of symbols from archive %s",
				ap->ldlbry.aifilnam);
			sym_count.l = sgetl( sym_count.c );
			/* check to make sure that you have a symbol table */
			if ( sym_count.l <= 0 )
				lderror(2,0,NULL,
				"archive symbol table is empty in archive %s\nexecute 'ar ts %s' to restore archive symbol table",
				ap->ldlbry.aifilnam,ap->ldlbry.aifilnam);
			if ( sym_count.l > TABSZ )
				lderror(2,0,NULL,
				"archive symbol directory in archive %s is too large",
				ap->ldlbry.aifilnam);

			/* read in the archive symbol directory */

			if (FREAD(arsymtab,sizeof(longchar),sym_count.l,fdes)
				!= sym_count.l)
				lderror(2,0,NULL,
				"can't read archive symbol directory of archive %s",
				ap->ldlbry.aifilnam);
			/* read in the symbol names string table */

			strtabsz = sym_ar_size - (sym_count.l + 1) * 
					sizeof(longchar);
			strtabptr = myalloc(strtabsz);
			if ((FREAD(strtabptr,strtabsz,1,fdes) != 1) ||
				strtabptr[strtabsz - 1] != '\0')
				lderror(2,0,NULL,
				"can't read archive string table of archive %s",
				ap->ldlbry.aifilnam);
			j = strtabptr; /* ptr for walking thru string table */
			for (i=0; i<sym_count.l; i++) {
				if (j > (strtabptr + strtabsz - 1))
					lderror(2,0,NULL,
					"too few symbol names in string table for archive %s",
					ap->ldlbry.aifilnam);
				arsymtab[i].l = sgetl(arsymtab[i].c);
				arsymnmptr[i] = j;
				while (*++j != '\0');
				j++;
				}
			
			/* loop thru symbol table until you can't find any more
			   unresolved symbols which are satisfied by this 
			   archive */
			found_one = 1;
			while (found_one) {
				struct ar_hdr memberhdr;
				SYMTAB *symptr;
				int i;
				found_one = 0;
				for (i=0; i < sym_count.l; i++) {
					symptr = findsym(arsymnmptr[i]);
					if (symptr != NULL) /* found one ?? */
						if ( symptr->sment.n_value == 0
						 && symptr->sment.n_scnum==0) {
							PUTSYM(symptr,0);
							/* seek to the archive location */
							OFFSET(fdes) = arsymtab[i].l
								+ sizeof(struct ar_hdr);
							/* read this member's archive header */
							if (FSEEK(fdes,-(int)(sizeof(struct ar_hdr)),BEGINNING)
								!= OKFSEEK ||
								FREAD(&memberhdr,sizeof(struct ar_hdr),1,fdes) != 1 ||
								strncmp(memberhdr.ar_fmag,ARFMAG,
								sizeof(memberhdr.ar_fmag)))
								lderror(2,0,NULL,
									"can't read archive header of archive %s",
									ap->ldlbry.aifilnam);
							if (sscanf(memberhdr.ar_size,"%ld",&ar_size) != 1)
								lderror(2,0,NULL,
									"invalid archive size for file %s",
									ap->ldlbry.aifilnam);
							/* read this member's header */
							if (FREAD(&(HEADER(fdes)),FILHSZ, 1, fdes) != 1)
							lderror(2,0,NULL,
								"can't read file header of archive %s",
								ap->ldlbry.aifilnam);
							if (OKMAGIC(HEADER(fdes).f_magic)) {
								/* bind this member */
								loadobjf(fdes,ap->ldlbry.aifilnam,-1,NULL,NULL);
								found_one = 1;
							}
						}
#if !NODSP
						else
							PUTSYM(symptr,0);
#endif
				} /* end of for */
			} /* end of while */

			/* we are done with this archive */
			fclose(IOPTR(fdes));
#endif
#if PORT5AR
			/* Read in the archive symbol table and search it
			 * for unresolved symbol references.  Keep passing
			 * over the symbol table, loading in archive members
			 * as you go until one complete pass is made with no
			 * additional loads of an archive member.
			 */
#if ONEPROC	
			struct ar_sym arsymtab[TABSZ];
#else
			struct ar_sym *arsymtab;
#endif
			struct ar_hdr arheader;
			long sym_count;
			char found_one;

			/* read archive header */
			fseek(IOPTR(fdes),0L,0);
			if (FREAD(&arheader,sizeof(struct ar_hdr),1,fdes)
			   != 1)
				lderror(2,0,NULL,
				"can't read archive header from archive %s",
				ap->ldlbry.aifilnam);
			
			/* figure out number of archive symbols */
			sym_count = sgetl( arheader.ar_syms );
#if !ONEPROC
			arsymtab = (struct ar_sym *) myalloc(sym_count * sizeof(struct ar_sym));
#endif
			/* check to make sure that you have a symbol table */
			if (sym_count <= 0)
				lderror(2,0,NULL,
			"archive symbol table is empty in archive %s\nexecute 'ar ts %s' to restore archive symbol table",
				ap->ldlbry.aifilnam,ap->ldlbry.aifilnam);

			/* read in the archive symbol table */
			if (FREAD(arsymtab,sizeof(struct ar_sym),sym_count,fdes)
				!= sym_count)
				lderror(2,0,NULL,
				"can't read archive symbol table of archive %s",
				ap->ldlbry.aifilnam);

			found_one = 1; /* loop thru symbol table until you can't
					  find any more unresolved symbols which
					  are satisfied by this archive */
			while (found_one) {
				SYMTAB *symptr;
				int i;
				short j;
				long location;
				char sym_name[sizeof(arsymtab->sym_name)+1];

				found_one = 0;
				for (i=0; i<=sym_count; i++) {
					strncpy(sym_name,arsymtab[i].sym_name,
						sizeof(arsymtab->sym_name));
					j = sizeof(arsymtab->sym_name);
					while (sym_name[j-1] == ' ' &&
						j >= 1) j--;
					sym_name[j] = '\0';
					symptr = findsym(sym_name);
					if (symptr != NULL)  /* found one ?? */
						if (symptr->sment.n_value == 0 && symptr->sment.n_scnum == 0) {
							PUTSYM(symptr,0);
							/* seek to the archive location */
							location = sgetl( arsymtab[i].sym_ptr );
							OFFSET(fdes) = location +
								sizeof(struct arf_hdr);
							/* read this member's header */
							FSEEK(fdes,0L,BEGINNING);
							if (FREAD(&(HEADER(fdes)),
								FILHSZ,1,fdes) != 1)
							lderror(2,0,NULL,
								"can't read file header of archive %s",
							ap->ldlbry.aifilnam);

							if (OKMAGIC(HEADER(fdes).f_magic)) {
								/* bind this member */
								loadobjf(fdes,ap->ldlbry.aifilnam,-1,
									NULL,NULL);
								found_one = 1;
							}
						} 
#if !NOSDP
						else
							PUTSYM(symptr, 0);
#endif
				};
			};

			/* we are done with this archive */
			fclose(IOPTR(fdes));
#endif
#if !PORTAR && !PORT5AR
			/*
			 * Search each member of the archive library
			 * to see if it will resolve any unsatisfied
			 * references
			 */
			done = count = 0;
			while( ! done ) {
				if ( OKMAGIC(HEADER(fdes).f_magic)) {
					lo_scnh = 1;
					if ((hi_scnh = getrec2(scnbuf,fdes,1)) == 0)
						lderror(2,0,NULL,"fail to read section headers of library %s member %d",
						ap->ldlbry.aifilnam,count);
					if (HEADER(fdes).f_nsyms > MAXSYMBS ||
					    getrec3(symbuf,fdes) == FAILURE)  {
						if( libsrch(fdes,NULL) )
						     loadobjf(fdes, ap->ldlbry.aifilnam,count,scnbuf,NULL);
						}
					else  {
						if( libsrch(fdes,symbuf) )
						     loadobjf(fdes, ap->ldlbry.aifilnam,count,scnbuf,symbuf);
						}
					}
				else  {
					if (FSEEK(fdes,HEADER(fdes).f_symptr+
						(HEADER(fdes).f_nsyms * SYMESZ),BEGINNING) != OKFSEEK)
						lderror(2,0,NULL,"fail to seek to the end of library %s member %d",
						ap->ldlbry.aifilnam,count);
					}
				done = myldclose(&ahdr,fdes);
				count++;
				}
#endif
			}
		else
			procspecobj(fdes, ap);
#if ONEPROC
		ap = ap->ldlbry.ainext;
#endif
		}
}
#if !PORTAR && !PORT5AR
/*eject*/
libsrch(fdes,symbuf)
LDFILE	*fdes;
SYMENT	*symbuf;
{

/*
 * Return true if current member of archive file defines an
 *  undefined symbol
 */

	register int i;
	register SYMTAB *symp;
	SYMENT	*symbol;
	SYMENT	symbolbuf;

	if ( !OKMAGIC(HEADER(fdes).f_magic) )
		return (0);

	if (symbuf == NULL)  {
		if (FSEEK(fdes,HEADER(fdes).f_symptr,0) != OKFSEEK)
			lderror(2,0,NULL,"fail to seek to symbol table when search libraries");
		symbol = &symbolbuf;
		}

	for( i = 0; i < HEADER(fdes).f_nsyms; i++ ) {
		if (symbuf == NULL)  {
			if (FREAD(symbol,SYMESZ,1,fdes) != 1)
				lderror(2,0,NULL,"fail to read symbol table when search libraries");
			}
		else  {
			symbol = symbuf++;
			}
		if( (symbol->n_sclass == C_EXT)  &&  (symbol->n_scnum > 0) ) {
			if( (symp = findsym(PTRNAME(symbol))) != NULL ) {
				if( symp->sment.n_scnum == 0 ) {  /*undefined*/
					PUTSYM(symp, 0);
					return(1);
					}
				PUTSYM(symp, 0);
				}
			}
		/*
		 * skip auxilliary entries for efficiency and
		 * to avoid misinterpreting garbage sment
		 */

		if ( (int) symbol->n_numaux > 0 ) {
			if (symbuf == NULL)  {
				if (fseek(IOPTR(fdes),(long) (SYMESZ * (int) symbol->n_numaux), 1) != OKFSEEK)
					lderror(2,0,NULL,"fail to skip aux entries when search libraries");
				}
			else  {
				symbuf++;
				}
			i += (int) symbol->n_numaux;
			}
		}

	return(0);
}
#endif
/*eject*/
loadobjf(fdes,fnamp,filndx,scnbuf,symbuf)
LDFILE	*fdes;			/* file descriptor		    */
char	*fnamp;			/* name of the file		    */
int	filndx;			/* member number within library    */
SCNHDR	*scnbuf;
SYMENT	*symbuf;
{

/*
 * Given a file descriptor which has been "seeked" to the beginning
 * of the file header, build the INFILE and INSECT structures
 * and process the symbol table.
 */

#if FLEXNAMES
	char *strtabptr;
	int strtabsz;
#endif
	register int s;
	register INSECT *sptr;
	register INFILE *filptr;

	filptr = (INFILE *) myalloc(sizeof(INFILE));

	curfilnm = fnamp;
	filptr->flname = fnamp;
	filptr->flfilndx = filndx;
	filptr->flfiloff = OFFSET(fdes);
#if DMERT || XL
	if (dmertplib)
		filptr->flplib = 1;
	else
		filptr->flplib = 0;
#endif

	/*
	 * Build an INSECT data structure for every section in the
	 * input file.  Check for two special cases:
	 *
	 *	1. Sections with illegal names - names that conflict
	 *		predefined ld section names - are not processed,
	 *		and yield an error message
	 *	2. Sections which are "padding sections" (see add_pad())
	 *		are skipped. They exist only to provide certain
	 *		physical alignment in the file
	 */

	for( s = 1; s <= HEADER(fdes).f_nscns; s++ ) {
		sptr = (INSECT *) myalloc(sizeof(INSECT));
		if (scnbuf == NULL)  {
			if (FSEEK(fdes,(long) (FILHSZ+HEADER(fdes).f_opthdr+
				(s-1) * SCNHSZ),BEGINNING) != OKFSEEK ||
			    FREAD(&sptr->ishdr,SCNHSZ,1,fdes) != 1)
				lderror(2,0,NULL,"fail to read section headers of file %s",fnamp);
			}
		else  {
			if (s < lo_scnh || s > hi_scnh)  {
				lo_scnh = s;
				if ((hi_scnh = getrec2(scnbuf,fdes,s)) == 0)
					lderror(2,0,NULL,"fail to read section header of file %s",
					fnamp);
				hi_scnh = hi_scnh + lo_scnh - 1;
				}
			sptr->ishdr = scnbuf[s-lo_scnh];
			}
		if( ! OKSCNNAME(sptr->ishdr.s_name) ) {
			lderror(1,0,NULL, "file %s has a section name which is a reserved ld identifier: %.8s",
				fnamp, sptr->ishdr.s_name);
			continue;
			}
                if ( equal(sptr->ishdr.s_name, _TV, 8) )
			++usrdeftv;

		if( sptr->ishdr.s_flags & (STYP_COPY | STYP_INFO))
			sptr->ishdr.s_flags |= STYP_DSECT;
			/* COPY and INFO sections require the same treatment
			   as DSECT sections */

		if( sptr->ishdr.s_flags & STYP_PAD ) {
			free( sptr );
			continue;
			}
		sptr->isecnum = (short) s;
#if DMERT || XL
		if (dmertplib)  {
			sptr->ishdr.s_relptr = 0;
			sptr->ishdr.s_nreloc = 0;
			sptr->ishdr.s_lnnoptr = 0;
			sptr->ishdr.s_nlnno = 0;
			}
#endif
		listadd(l_IS,filptr,sptr);
		}
#if FLEXNAMES
	strtabsz = 0L;
	strtabptr = strtabread(fdes, fnamp, &strtabsz);
	if (strtabptr && (strtabsz <= MAXKEEPSIZE))
		filptr->flstrings = strtabptr;
	else
		filptr->flstrings = NULL;
#endif

	if (symbuf == NULL)
		if (FSEEK(fdes,HEADER(fdes).f_symptr,BEGINNING) != OKFSEEK)
			lderror(2,0,NULL,"fail to seek to symbol table of file %s",fnamp);
#if FLEXNAMES
	filptr->flnlsyms = psymtab(IOPTR(fdes),HEADER(fdes).f_nsyms,filptr,symbuf,strtabptr);
#else
	filptr->flnlsyms = psymtab(IOPTR(fdes),HEADER(fdes).f_nsyms,filptr,symbuf);
#endif
	listadd(l_IF,&infilist,filptr);
	
#if FLEXNAMES
	if (strtabptr && (strtabsz > MAXKEEPSIZE))
		free( strtabptr );
#endif

/*
 * Error check: warn the user about files which have no relocation
 * information in them
 *
 * Such files are, however, permitted
 */

	if( HEADER(fdes).f_flags & F_RELFLG ) {
			if( TYPE(fdes) == ARTYPE )
			lderror(0,0,NULL, "library %s, member has no relocation information",
				fnamp);
		else
			lderror(0,0,NULL, "file %s has no relocation information",
				fnamp);
			}

#if M32
	/* macflags is used to remember if any input file had F_BM32B on */
	macflags |= HEADER(fdes).f_flags & F_BM32B ;
#endif

}

#if FLEXNAMES
char *
strtabread(fdes,fnamp,sizeptr)
LDFILE *fdes;			/* file descriptor		*/
char   *fnamp;			/* name of the file		*/
int   *sizeptr;			/* ptr to size of string table read */
{
/* 
 * Given the file descriptor and the name of the file read the string table
 * if there is one and return a pointer to it, otherwise return null.
 */
	long symtabend;
	char *strtabptr;
	symtabend = HEADER(fdes).f_symptr + SYMESZ * HEADER(fdes).f_nsyms;
	if (TYPE(fdes) == USH_ARTYPE ) {
		if (ar_size > symtabend) {
			if (FSEEK(fdes, symtabend, BEGINNING) != OKFSEEK ||
				FREAD(sizeptr,sizeof(*sizeptr),1,fdes) != 1)
				lderror(2,0,NULL,
				"fail to read string table of file %s", fnamp);
			if (*sizeptr > 4) {
				strtabptr = myalloc(*sizeptr);
				if (FREAD(&strtabptr[4], *sizeptr - 4, 1, fdes) != 1)
					lderror(2, 0, NULL,
					"fail to read string table of file %s",
					fnamp);
				else
					return(strtabptr);
				}
			else
				return(NULL);
			}
		else 
			return(NULL);
		}
	else {
		/* not a member of an archive so it must be a
		 * regular object file or a DMERT public library
		 * (both of which are processed the same way
		 */

		if (FSEEK(fdes,symtabend,BEGINNING) == OKFSEEK &&
			FREAD(sizeptr,sizeof(*sizeptr),1,fdes) == 1) {
			/* read string table */
			if (*sizeptr > 4) {
				strtabptr = myalloc(*sizeptr);
				if (FREAD(&strtabptr[4], *sizeptr - 4, 1, fdes) != 1)
					lderror(2,0,NULL,
					"fail to read string table of file %s",
					fnamp);
				else
					return(strtabptr);
				}
			else
				return(NULL);
			}
		else
			return(NULL);
		}
}
#endif
#if COMMON
/*eject*/
pf77comm()
{
/* process F77 common data declarations, undefined (i.e., unitialized
 * common) data is placed into the bss section.  common data is indicated
 * by an undefined external variable with a non zero value.  the value
 * indicates the size of the common data region.
 */

	register SYMTAB *symp;
	long symsize;
	register INSECT *sptr;
	INFILE *fptr;

	/* look thru the symbol table to see if there are some symbols
	 * which are undefined and have non-zero values.  allocate all
	 * such symbols to an imaginary bss segment defined in the 
	 * COMF77 file.
	 */

	/* initialize the dummy section to which common data is allocated */

	fptr = (INFILE *)myalloc(sizeof(INFILE));
	fptr->flname = COMF77;
	fptr->flfiloff = 0L;
	
	sptr = (INSECT *)myalloc(sizeof(INSECT));

	listadd(l_IS,fptr,sptr);
	listadd(l_IF,&infilist,fptr);
	
	strcpy(sptr->ishdr.s_name,".bss");
	sptr->ishdr.s_paddr = 0;
	sptr->ishdr.s_vaddr = 0;
	sptr->ishdr.s_size = 0;
	sptr->ishdr.s_scnptr = NULL;
	sptr->ishdr.s_relptr = NULL;
	sptr->ishdr.s_lnnoptr = NULL;
	sptr->ishdr.s_nreloc = 0;
	sptr->ishdr.s_nlnno = 0;
	sptr->ishdr.s_flags = 0;

	sptr->isoutsec = NULL;
	sptr->isincnxt = NULL;
	sptr->isdispl = 0;
	sptr->isrldisp = 0;
	sptr->islndisp = 0;
	sptr->isnewvad = 0;
	sptr->isnewpad = 0;
	sptr->isecnum = 1;
	sptr->isfillfg = 0;


	symp = NULL;
	while ((symp = loopsym(symp,1)) != NULL) {
		if (symp->sment.n_sclass == C_EXT &&
		    symp->sment.n_scnum == N_UNDEF &&
		    symp->sment.n_value > 0) { /* we have a common symbol */
			
			/* assign this common symbol to the dummy section */

			symp->smscnptr = sptr;
			symp->sment.n_scnum = 1;

			/* align common symbol on optimal boundary */

			symsize = symp->sment.n_value;
			if (symsize == 1) {}
			else if (symsize == 2) {
				sptr->ishdr.s_size += 1;
				sptr->ishdr.s_size &= (~1);
				}
#if XL
			else if (symsize <= 4) {
				sptr->ishdr.s_size += 3;
				sptr->ishdr.s_size &= (~3);
				}
			else if (symsize <= 8) {
				sptr->ishdr.s_size += 7;
				sptr->ishdr.s_size &= (~7);
				}
#endif
			else 
				{
				sptr->ishdr.s_size += COM_ALIGN;
				sptr->ishdr.s_size &= (~COM_ALIGN);
				}

			/* increase size of dummy bss section */

			symp->sment.n_value = sptr->ishdr.s_size;
			sptr->ishdr.s_size += symsize;
		}
	}
 	/* round up the section size to be a multiple of 4 bytes */
 	sptr->ishdr.s_size += 03L;
 	sptr->ishdr.s_size &= (~03L);
}
#endif




LDFILE *
#if AR16WR
myldopen(filename,arhdr,ldptr,fbuf)
#else
myldopen(filename,arhdr,ldptr)
#endif
char	*filename;
ARCHDR	*arhdr;
LDFILE	*ldptr; 
#if AR16WR
char	*fbuf;
#endif
{

/*
 * Open the file.  If it is an arch file, read in the arch header
 * and file header.  Otherwise just read in the file header.
 */

#if PORTAR || PORT5AR
	unsigned short	type;
	char		atype[SARMAG];
#else
	unsigned short 	type,
			atype;
#endif
	int		count;

	if ((IOPTR(ldptr)=fopen(filename, "r")) == NULL) 
	    return(NULL);

#if AR16WR
	setbuf( IOPTR(ldptr), fbuf );
#endif

#if PORTAR || PORT5AR
	if (FREAD(atype,SARMAG,1,ldptr) != 1)
		atype[0] = '\0';
	OFFSET(ldptr) = 0L;
	FSEEK(ldptr,0L,0);
#endif

	if (FREAD(&type, sizeof(type), 1, ldptr) != 1) {
	    fclose(IOPTR(ldptr));
	    return(NULL);
	}
	count = sizeof(type);

#if PORT5AR || PORTAR
	if (strncmp(atype,ARMAG,SARMAG) == 0) {
		TYPE(ldptr) = USH_ARTYPE;
		OFFSET(ldptr) = 0L;
		FSEEK(ldptr,0L,0);
#else
	if ( sizeof(ARTYPE) != sizeof(type) ) {
		if ( FREAD(&atype, sizeof(atype), 1, ldptr) != 1)
			atype = type;
		else
			count += sizeof(atype);
	} else {
		atype = type;
		}

	if ( (atype == USH_ARTYPE) ||
			(type == USH_ARTYPE) ) {
		TYPE(ldptr) = USH_ARTYPE;
		OFFSET(ldptr) = ARCHSZ + sizeof(ARMAG);
		if (getrec1(arhdr,ldptr) == FAILURE)
			return(NULL);
#endif
	} else {
		TYPE(ldptr) = type;
		OFFSET(ldptr) = 0L;
		HEADER(ldptr).f_magic = type;
#if !PORTAR && !PORT5AR
		if (count > sizeof(type))
			HEADER(ldptr).f_nscns = atype;
#endif
		if ( FREAD( (char *)  (&(HEADER(ldptr)))+count,FILHSZ-count,1,ldptr ) != 1)
			return(NULL);
	}

	return(ldptr);
}
#if !PORTAR && !PORT5AR
/*eject*/
myldclose(arhdr,fdes)
ARCHDR *arhdr;
LDFILE *fdes;
{
	OFFSET(fdes) += arhdr->ar_size + ARCHSZ;

	if (arhdr->ar_size & 01)  {
		OFFSET(fdes) += 1;
		if (fseek(IOPTR(fdes),1L,1) != 0)  {
			fclose(IOPTR(fdes));
			return(SUCCESS);
			}
		}

	if (getrec1(arhdr,fdes) == SUCCESS)
		return(FAILURE);

	fclose(IOPTR(fdes));
	return(SUCCESS);
}
/*eject*/
getrec1(arhdr,fdes)
ARCHDR *arhdr;
LDFILE *fdes;
{

/*
 * if the file is an arch file, read in arch header and file
 * header.  Otherwise just read in file header.
 */

	if (TYPE(fdes) == USH_ARTYPE)
		if (FREAD(arhdr,ARCHSZ,1,fdes) != 1)
			return(FAILURE);

	if (FREAD(&(HEADER(fdes)),FILHSZ,1,fdes) != 1)
		return(FAILURE);

	return(SUCCESS);
}
#endif
/*eject*/
getrec2(shdrbuf,fdes,start_scnh)
SCNHDR	*shdrbuf;
LDFILE	*fdes;
int	start_scnh;
{

/*
 * read in section headers and store them in the buffer
 */

	int	i, lim;

	if (start_scnh != 1 || HEADER(fdes).f_opthdr > 0)
		if (FSEEK(fdes,(long) (FILHSZ+HEADER(fdes).f_opthdr+(SCNHSZ*(start_scnh-1))),BEGINNING) != OKFSEEK)
			return(FAILURE);

	lim = HEADER(fdes).f_nscns - start_scnh + 1;
	if (lim > MAXSCNBS) lim = MAXSCNBS;
	for (i = 0; i < lim; i++)  {
		if (FREAD(shdrbuf++,SCNHSZ,1,fdes) != 1)
			return(FAILURE);
		}


	return(lim);
}
#if !PORT5AR && !PORTAR
/*eject*/
getrec3(symbuf,fdes)
SYMENT *symbuf;
LDFILE *fdes;
{

/*
 * read in the symbol table and store it in the buffer.
 */

	int i, lim;

	if (FSEEK(fdes,HEADER(fdes).f_symptr,BEGINNING) != OKFSEEK)
		return(FAILURE);

	lim = (HEADER(fdes).f_nsyms < MAXSYMBS ? HEADER(fdes).f_nsyms : MAXSYMBS);
	for (i = 0; i < lim; i++)  {
		if (FREAD(symbuf++,SYMESZ,1,fdes) != 1)
			return(FAILURE);
		}

	return(SUCCESS);
}
#endif
