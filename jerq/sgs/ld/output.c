static char ID[] = "@(#) output.c: 1.30 12/10/83";
#include <stdio.h>

#if PORTAR
#include <ar.h>
#endif

#include "system.h"
#include "reloc.h"
#include "linenum.h"
#include "structs.h"
#include "extrns.h"
#include "ldfcn.h"

#if TRVEC
#include "tv.h"
#include "ldtv.h"
#endif

#include "aouthdr.h"
#include "slotvec.h"
#include "sgs.h"
#include "patch.h"
#include "sgsmacros.h"
#include "instr.h"
#include "params.h"
#include "ldmacros.h"


#if AR32W && !UNIX
#define MAXSCNBS 20
#else
#define MAXSCNBS 4
#endif

/*
#if NATIVE || UNIX
#define MODE 0777
#else
#define MODE 0666
#endif
*/
#define	MODE	0666	/* rob - we always want no execute for 5620s */

static	long	lastfile = -1L;	/* symtab index of previous .file entry */
static	long	nextfile = -1L; /* symtab index of next .file entry */
static	char	undefmsg;	/* has the undefined message been printed? */

#if FLEXNAMES
static	long	string_buf_length = 0;
static	char	*last_string_ptr = NULL;
#endif

extern unsigned svhighslt;
extern unsigned short macflags; 	/* defined in ld1.c */
extern unsigned short swapb2();

/*eject*/
output()
{

/*
 * Produce the output object file
 */


	FILE *fdes, *sdes, *rdes;
#if FLEXNAMES
	FILE *strdes;
	long strtborg;
#endif
	int fd;
	FILHDR outflhdr;
	unsigned *buf;
	ANODE *ap;
	OUTSECT *osp;
	INSECT *isp;
	register SYMTAB *symp;
	short fill;
	long indx;
	extern long time();
#if AR16WR
	char rdesbuf[BUFSIZ], fdesbuf[BUFSIZ], sdesbuf[BUFSIZ];
#endif

/*
 * Only generate output if there were no fatal errors,
 * and the output file can be opened
 *
 * NOTE: there is no return from these two lderror calls
 */

	if( (errlev > 1) || (errlev && (!rflag || !aflag)) )
		lderror(2,0,NULL, "Error(s). No output written to %s",
			outfilnm);

	buf = (unsigned *) myalloc(BUFSIZ);

	if( cflag != 4 ) {
		if( (fd = creat(outfilnm, MODE)) < 0 )
			lderror(2,0,NULL, "Can't create output file %s", outfilnm);
		close(fd);
		fdes = fopen(outfilnm, "w");
#if AR16WR
		setbuf( fdes, fdesbuf );
#endif
		zerofile(fdes, buf, BUFSIZ);
		}
	else {
		 copy(outfilnm, "/dev/null", 10);
		 fdes = fopen(outfilnm, "w");
		 }

/*
 * Prepare to generate relocation information
 */

	if( rflag ) {
		rdes = fopen(outfilnm, "w");
#if AR16WR
		setbuf( rdes, rdesbuf );
#endif
	} else
		rdes = NULL;

/*
 * Prepare to generate a symbol table
 */

	if( ! sflag ) {
		/*
		 * Seek to beginning of symbol table
		 */
		sdes = fopen(outfilnm, "w");
#if AR16WR
		setbuf( sdes, sdesbuf );
#endif
		fseek(sdes, symtborg, 0);

#if FLEXNAMES
		/* prepare to generate a string table */

		strdes = fopen(outfilnm, "w");
		strtborg = symtborg + SYMESZ * tnumsyms + 4;
		fseek(strdes,strtborg,0);
#endif
		}
	else {
		sdes = NULL;
#if FLEXNAMES
		strdes = NULL;
#endif
		}

	zero((char *) &outflhdr, FILHSZ);
	fseek(fdes, (long) (FILHSZ + hflag), 0);

/*
 * Output the section headers
 *
 * This is done in two steps:
 *	1. All allocated sections
 *	2. All unallocated sections (e.g., DSECTS)
 */

	for( ap = (ANODE *) avlist.head; ap; ap = ap->adnext ) {

		if( ap->adtype == ADSECT ) {
#if UNIX || XL
			if( sflag || xflag ) 
#else
			if( sflag )
#endif
				ap->adscnptr->oshdr.s_nlnno = 0;
			/*
			 * Reduce "physical" size of .patch section (as far
			 * as ldp is concerned) but leave "true" size
			 * available for patch list construction in wrtplst()
			 */
			if( (pflag > 0)  &&  equal(ap->adscnptr->oshdr.s_name,".patch",6) ) {
				indx = ap->adscnptr->oshdr.s_size;
				ap->adscnptr->oshdr.s_size = (long) pflag;
				fwrite(ap->adscnptr, SCNHSZ, 1, fdes);
				ap->adscnptr->oshdr.s_size = indx;
				}
			else
				fwrite(ap->adscnptr, SCNHSZ, 1, fdes);
			/* Issue a warning if a section starts on an odd byte
			   boundary! This is NEVER adviseable, although it is
			   possible that for certain unforseen special
			   applications, it may be necessary, so it is put
			   in as a warning vice a fatal error; this is
			   applicable in varying degrees of severity for
			   all SGSs */
			if(ap->adscnptr->oshdr.s_paddr & 0x01L)
				lderror(0,0,NULL,"Section %.8s starts on an odd byte boundary!",ap->adscnptr->oshdr.s_name);

			}

		if( ap->adtype == ADPAD )
			fwrite(ap->adscnptr, SCNHSZ, 1, fdes);

		}

	for( osp = (OUTSECT *) dsectlst.head; osp; osp = osp->osdsnext ) {
		/*
		 * Special processing for COPY and INFO sections:
		 *  1) remove DSECT (signifying no alloc) flag
		 *  2) leave section size intact
		 */

		if ( osp->oshdr.s_flags & (STYP_COPY | STYP_INFO)) {
			osp->oshdr.s_flags ^= STYP_DSECT;
			}
		else
			osp->oshdr.s_size = 0L;	/* DSECTS get 0 size */
		fwrite(osp, SCNHSZ, 1, fdes);
		}

#if ILDOPT
	if (ildflag) {
		SCNHDR shdr;
		copy( shdr.s_name, ".history", 8 );
		shdr.s_flags = STYP_DSECT;
		shdr.s_paddr = 0l;
		shdr.s_vaddr = 0l;
		shdr.s_size = 0l;
		shdr.s_scnptr = 0l;
		shdr.s_relptr = 0l;
		shdr.s_lnnoptr = 0l;
		shdr.s_nreloc = 0;
		shdr.s_nlnno = 0;
		fwrite( &shdr, SCNHSZ, 1, fdes );
	}
#endif

#if TRVEC 
/*
 * Now do whatever is necessary to initialize tv.
 */

	if (tvflag && aflag)
		tvinit();

#endif

/*
 * Close transfer file
 */

#if !ONEPROC
	if (ferror(trnfdes))
		lderror(2,0,NULL,"I/O error on transfer file");
	fclose( trnfdes );
#if AR16WR
	free( trnfbuf );
#endif
#endif

/* 
 * Allocate buffer for slot vectors used by plocsyms 
 */

	svinit(maxrelocsms);

/*
 * Process each input file, in order
 *
 * The act of processing a file can result in the detection of fatal
 * errors. These are indicated by setting "errlev" to 2
 *
 * Currently known fatal errors:
 *	1. Undefined symbols when "-r" flag is not set
 */

#if FLEXNAMES
	wrtinfl(fdes, sdes, strdes, rdes, buf);
#else
	wrtinfl(fdes, sdes, rdes, buf);
#endif

	if ( errlev > 1 )	/* unresolved refs in absolute load */
		lderror(2,0,NULL, "Symbol referencing errors. No output written to %s",
				outfilnm);

	if ( aflag && errlev )
		lderror(2,0,NULL, "Output file %s not executable", outfilnm);

	fflush(fdes);

#if TRVEC
/*
 * Complete tv processing
 */

	if (tvflag && aflag)
		tvfinish();

#endif

/* 
 * Fill any necessary holes between sections, in .bss
 */

	for( osp = (OUTSECT *) outsclst.head; osp; osp = osp->osnext ) {

		if( osp->oshdr.s_scnptr != 0L ) {
			long disp;
			long offset;

#if TRVEC
			/*
			 *  Don't fill the _TV section.
			 *  It is built by the link editor.
			 */
			if( equal(osp->oshdr.s_name, _TV, 8) )
				continue;
#endif

			indx = 0;
			fill = (osp->osflags & FILL) ? osp->osfill : globfill;
#if AR16WR || AR32WR
#if SWABFMT
			/*
			 * Interchange bytes of the fill constant 
			 */
			fill = ((fill >> 8) & 0xff) + ((fill & 0xff) << 8);
#endif
#endif
			isp = osp->osinclhd;
			while( indx < osp->oshdr.s_size ) {
				if( isp == NULL )
					disp = osp->oshdr.s_size;
				else
					disp = isp->isdispl;
				if( disp > indx ) {	/* fill needed */
					offset = osp->oshdr.s_scnptr + indx;
					fseek(fdes, offset, 0);
					for( ; indx < disp-1; indx += sizeof(short) )
						fwrite(&fill, sizeof(short), 1, fdes);
					if( indx < disp ) {
						fwrite(&fill, sizeof(char), 1, fdes);
						indx++;
						}
					}
				if( isp != NULL ) {
					indx += isp->ishdr.s_size;
					isp = isp->isincnxt;
					}
				}
			}
		}

/* 
 * Write global symbols at end of symbol table 
 */

	undefmsg = 0;
	symp = NULL;
	while( (symp = loopsym(symp, 1)) != NULL ) {
		symp->sment.n_value = symp->smnewval;
		if( (symp->smlocflg == 0)  &&  ! sflag ) {
			AUXTAB *auxp;
			char	*save_ptr;
			/*
			 * set last .file entry to point to first
			 * global symbol entry
			 */
			if ( lastfile != -1L ) {
				setfile( sdes, noutsyms);
				lastfile = -1L;
				}

			if( Hflag  &&  (symp->sment.n_sclass == C_EXT) )
				symp->sment.n_sclass = C_STAT;
#if FLEXNAMES
			if (symp->sment.n_zeroes == 0L) {
				save_ptr = symp->sment.n_nptr;
				strwrite(strdes,&symp->sment, symp->smnamelength);
			}
#endif
			if ( fwrite(symp, SYMESZ, 1, sdes) != 1 )
				lderror(2,0,NULL,
				  "Cannot complete output file %s. Write error.", outfilnm);
#if FLEXNAMES
			if (symp->sment.n_zeroes == 0L)
				symp->sment.n_nptr = save_ptr;
#endif

			noutsyms++;
			auxp = NULL;
			while( (auxp = loopaux(symp, auxp, 0)) != NULL ) {
				if (auxp->axent.x_sym.x_tvndx == P_TV)
					auxp->axent.x_sym.x_tvndx = N_TV;
#if UNIX || XL || DMERT
				if ((BTYPE(symp->sment.n_type) == T_STRUCT ||
				    BTYPE(symp->sment.n_type) == T_UNION ||
				    BTYPE(symp->sment.n_type) == T_ENUM ||
				    symp->sment.n_sclass == C_EOS) && auxp->axent.x_sym.x_tagndx > 0)  {
#if UNIX
					if (xflag)
#endif
#if DMERT
					if (symp->smscnptr && symp->smscnptr->isfilptr->flplib)
#endif
#if XL
					if (xflag || (symp->smscnptr && symp->smscnptr->isfilptr->flplib))
#endif
						auxp->axent.x_sym.x_tagndx = 0;
					}
#endif
				fwrite(auxp, AUXESZ, 1, sdes);
				noutsyms++;
				}
			}
		}
#if FLEXNAMES
/*
 * Once all symbols have been written, write size of string table.
 * Symbol table file descriptor points to beginning of string table.
 * Final value of stroffset is size of string table.
 */
	if ( last_string_ptr )
		fwrite( last_string_ptr, string_buf_length, 1, strdes );
	if ( stroffset > 4L &&
		fwrite(&stroffset, sizeof(stroffset), 1, sdes) != 1)
		lderror(2,0,NULL,
			"fail to write size of string table for file %s",
			outfilnm);
#endif

/*
 * Output the file header
 */

	outflhdr.f_magic = magic;
	if( sflag ) {
		outflhdr.f_symptr = 0L;
		outflhdr.f_nsyms = 0L;
		outflhdr.f_flags |= (F_LNNO | F_LSYMS);
		}
#if UNIX || XL
	else if ( xflag ) {
			outflhdr.f_symptr = symtborg;
			outflhdr.f_nsyms = noutsyms;
			outflhdr.f_flags |= (F_LNNO | F_LSYMS);
			}
#endif
	else  {
		outflhdr.f_symptr = symtborg;
		outflhdr.f_nsyms = noutsyms;
		}
	if( ! rflag )
		outflhdr.f_flags |= F_RELFLG;
	if ( ! errlev && aflag )
		outflhdr.f_flags |= F_EXEC;
#if AR16WR
	outflhdr.f_flags |= F_AR16WR;
#endif
#if AR32WR
	outflhdr.f_flags |= F_AR32WR;
#endif
#if AR32W
	outflhdr.f_flags |= F_AR32W;
#endif
#if M32
	/* macflags is used to remember if any input files have F_BM32B on */
	outflhdr.f_flags |= macflags;
#endif
	if ( pflag )
		outflhdr.f_flags |= F_PATCH;
	outflhdr.f_nscns = numoutsc;
	outflhdr.f_timdat = (long) time(0);
	outflhdr.f_opthdr = hflag;
	fseek(fdes, 0L, 0);		/* seek to beginning of file */
	fwrite(&outflhdr, FILHSZ, 1, fdes);

/*
 * If requested, supply an old a.out file header in the optional
 * header of the output file
 */

	if( Xflag )
		wrtaout(fdes, &outflhdr);

/*
 * Zero the unused portion of the optional header
 */

	fill = hflag - (Xflag ? sizeof(AOUTHDR) : 0) - psize();
	if( fill > 0 ) {
		zero((char *) buf, BUFSIZ);
		for( ; fill > 0; fill -= BUFSIZ ) 
			fwrite(buf, min(fill, BUFSIZ), 1, fdes);
		}

/*
 * If requested, construct a patch list. This list is the last thing 
 * built in the optional header
 */

	if( pflag )
		wrtplst(fdes);

/*
 * Close and clean up
 */

	if (ferror(fdes))
		lderror(2,0,NULL,"I/O error on output file %s", outfilnm);
	fclose(fdes);
	if ( ! sflag ) {
		if (ferror(sdes))
			lderror(2,0,NULL,"I/O error on output file %s",
				outfilnm);
		fclose(sdes);
#if FLEXNAMES
		if (ferror(strdes))
			lderror(2,0,NULL,"I/O error on output file %s",
				outfilnm);
		fclose(strdes);
#endif
		}
	if( rflag )  {
		if (ferror(rdes))
			lderror(2,0,NULL,"I/O error on output file %s",
				outfilnm);
		fclose(rdes);
		}

	free( buf );
/*
 *	Release slot vector buffers or frames
 */

#if !NOSDP
	svfini();
#endif

	if ( outflhdr.f_flags & F_EXEC ) {
		int mask;
		mask = umask(0);
		umask(mask);
		chmod(outfilnm, MODE & ~mask);
		}
}
/*eject*/
#if FLEXNAMES
wrtinfl(fdes, sdes, strdes, rdes, buf)
FILE *fdes, *sdes, *strdes, *rdes;
#else
wrtinfl(fdes, sdes, rdes, buf)
FILE *fdes, *sdes, *rdes;
#endif
char *buf;
{

/*
 * This routine is called once per run to relocate the
 * text, relocation information, and line number entries.
 */

	SCNHDR scnbuf[MAXSCNBS];
	LDFILE *ifd, *rfd, ifdbuf, rfdbuf;
	short fill;
	int lo_scnh, hi_scnh, relocfg;
	long ndx, offset, oldoffst, paddiff;

	INFILE	*infl,		/* current input file */
		*pnfl;		/* previous input file*/
	register INSECT *isp;
	OUTSECT *osp;
	LINENO *lentry;
	LINENO lentrybuf;
	SLOTVEC *svp;
	SYMTAB *gsp;

lentry = &lentrybuf;

ifd = &ifdbuf;
rfd = &rfdbuf;
IOPTR(ifd) = NULL;
pnfl = NULL;
infl = (INFILE *)infilist.head;

while ( infl != NULL ) {

	OFFSET(ifd) = OFFSET(rfd) = infl->flfiloff;
	if ( IOPTR(ifd) == NULL ) {	/* start new input file */
#if COMMON
		if (strcmp(infl->flname,COMF77) == 0) {
			/* skip the dummy file for F77 common data */
			infl = infl->flnext;
			continue;
		};
#endif
		if( (IOPTR(ifd) = fopen(infl->flname,"r")) == NULL ||
		    (IOPTR(rfd) = fopen(infl->flname,"r")) == NULL )
			lderror(2,0,NULL, "Can't open %s", infl->flname);
		if( infl->flfiloff == 0L)
			TYPE(ifd) = TYPE(rfd) = 0;
		else
			TYPE(ifd) = TYPE(rfd) = USH_ARTYPE;
		}
	if( TYPE(ifd) == ARTYPE )  {
		if (FSEEK(ifd,0L,BEGINNING) != OKFSEEK)
			lderror(2,0,NULL,"internal error: fail to seek to the member of %s",
			infl->flname);
		}
	if( FREAD(&(HEADER(ifd)),FILHSZ,1,ifd) != 1 )
		lderror(2,0,NULL,"fail to read file header of %s",infl->flname);
	HEADER(rfd) = HEADER(ifd);
	lo_scnh = 1;
	if ((hi_scnh = getrec2(scnbuf,ifd,1)) == 0)  {
		lderror(2,0,NULL,"fail to read section headers of %s",
		infl->flname);
		}

/*
 * Process the local symbol table. Build the slot vector entries, to
 * permit easier relocation of symbol references
 */

#if FLEXNAMES
	noutsyms += plocsyms(sdes, strdes, ifd, infl);
#else
	noutsyms += plocsyms(sdes, ifd, infl);
#endif

/*
 * Process each section in this file which is to be used for link
 * editing
 */

	for( isp = infl->flishead; isp; isp = isp->isnext ) {

		osp = isp->isoutsec;
		offset = osp->oshdr.s_scnptr + isp->isdispl;

		/*
		 * Ignore zero-length input sections, and output section
		 * which are all zeros
		 */
		if( (isp->ishdr.s_size == 0L) || (osp->oshdr.s_scnptr == 0L) )
			continue;

		/*
		 * If the input section was not initialized, but the output
		 * section is, "fill" the section with the appropriate value
		 */
		if( isp->ishdr.s_scnptr == 0L ) {
			if( isp->isfillfg )
				fill = isp->isfill;
			else if( osp->osflags & FILL )
				fill = osp->osfill;
			else
				fill = globfill;
#if AR16WR || AR32WR
#if SWABFMT
			/*
			 * Interchange bytes of the fill constant 
			 */
			fill = ((fill >> 8) & 0x00ff) + ((fill << 8) & 0xff00);
#endif
#endif
			fseek(fdes, offset, 0);
			for( ndx=0; ndx < isp->ishdr.s_size-1; ndx +=2 )
				fwrite(&fill, sizeof(short), 1, fdes);
			if( ndx < isp->ishdr.s_size )
				fwrite(&fill, sizeof(char), 1, fdes);
			continue;
			}

		/*
		 * Process an initialized input section
		 */

		if( isp->isecnum < lo_scnh || isp->isecnum > hi_scnh)  {
			lo_scnh = isp->isecnum;
			if ((hi_scnh = getrec2(scnbuf,ifd,lo_scnh)) == 0)
				lderror(2,0,NULL,"fail to read section header %d of %s",
				isp->isecnum,infl->flname);
			hi_scnh += lo_scnh;
			}
		if( FSEEK(ifd,scnbuf[isp->isecnum - lo_scnh].s_scnptr,
		    BEGINNING) != OKFSEEK)
			lderror(2,0,NULL, "Seek to %s section %.8s failed",
				infl->flname, isp->ishdr.s_name);
		/* 
		 * Prepare to process relocation entries, if any
		 */
		if( (relocfg = isp->ishdr.s_nreloc) > 0 ) {
			if( FSEEK(rfd,scnbuf[isp->isecnum - lo_scnh].s_relptr,
			    BEGINNING) != OKFSEEK)  {
				lderror(1,0,NULL, "Seek to %s section %.8s reloc entries failed",
					infl->flname, isp->ishdr.s_name);
				relocfg = 0;
				}
			}
		fseek(fdes, offset, 0);
		if( rflag  &&  relocfg ) {
			offset = osp->oshdr.s_relptr + isp->isrldisp;
			fseek(rdes, offset, 0);
			}

		relocate(ifd, rfd, infl, isp, osp, fdes, rdes, &paddiff, &relocfg, buf);

/*
 * Process line numbers, if present
 */

#if UNIX || XL
		if( !sflag && !xflag 
#else
		if( !sflag 
#endif
		    && scnbuf[isp->isecnum - lo_scnh].s_nlnno > 0)  {
			long lnptr;

			if( FSEEK(ifd,scnbuf[isp->isecnum - lo_scnh].s_lnnoptr,
			    BEGINNING) != OKFSEEK)
				lderror(2,0,NULL, "Seek to %s section %.8s lnno failed",
				infl->flname, isp->ishdr.s_name);
			oldoffst = ftell(sdes);
			offset = osp->oshdr.s_lnnoptr + isp->islndisp;
			fseek(fdes, offset, 0);
			lnptr = offset;
			for( ndx = 0; ndx < isp->ishdr.s_nlnno; ndx++ ) {
				AUXENT ax;
				AUXENT *axp;

				if (FREAD(lentry,LINESZ,1,ifd) != 1)
					lderror(2,0,NULL,"fail to read lnno of section %.8s of file %s",
					isp->ishdr.s_name,infl->flname);
				if( lentry->l_lnno == 0 ) {
					if( (svp = svread(lentry->l_addr.l_symndx)) == NULL ) {
						lderror(1,0,NULL, "line nbr entry (%ld %d) found for non-relocatable symbol: section %.8s, file %s",
							lentry->l_addr.l_symndx,lentry->l_lnno, isp->ishdr.s_name, infl->flname);
						continue;
						}
					lentry->l_addr.l_symndx = svp->svnewndx;
					offset = symtborg + (1+lentry->l_addr.l_symndx)*SYMESZ
						+ ((char *) &ax.x_sym.x_fcnary.x_fcn.x_lnnoptr - (char *) &ax);
					fseek(sdes, offset, 0);
					fwrite(&lnptr, sizeof(long), 1, sdes);
					}
				else
					lentry->l_addr.l_paddr += paddiff;
				fwrite(lentry, LINESZ, 1, fdes);
				lnptr += LINESZ;
				} /* for */
			fseek(sdes, oldoffst, 0);
			} /* if */
		} /* for isp */

	pnfl = infl;
	infl = infl->flnext;

	/* see if next input file is from same archive */
	if ( (pnfl->flfiloff > 0L) && (infl != NULL) )
		if ( strcmp(pnfl->flname, infl->flname) == 0 &&
		     pnfl->flfiloff < infl->flfiloff             )
			continue;

	fclose(IOPTR(ifd));
	fclose(IOPTR(rfd));
	IOPTR(ifd) = NULL;
	}
}

long
#if FLEXNAMES
plocsyms(sdes, strdes, infd, infl)
FILE *strdes;	/* current place in output string table */
#else
plocsyms(sdes, infd, infl)
#endif
FILE *sdes;	/* current place in output symbol table */
INFILE *infl;	/* input file control block */
LDFILE *infd;	/* input file I/O descriptor */
{

/*
 * This routine is called once per input file to process its local
 * symbol table
 *
 * Each symbol is relocated and written to the output file
 *
 * In addition, a "slot vector" entry is built so that wrtinfl
 * can easily get at relocation info for all symbols in the file.
 *
 * The "slot" contains the old and new symbol values, old and new symbol
 * table index, and new section number
 */

	register SYMTAB *gsp;
	register SYMENT	*sm;
	register long    count;
	register AUXTAB *auxp;

	int *newscnum, *segment;
	long *ovaddr, *nvaddr, *opaddr, *npaddr;
	SLOTVEC *svp;
	int	i,
		secnum,		/* section in which defined */
		reloctag,	/* needs x_tagndx relocated */
		relocend,	/* needs x_endndx relocated */
		tvndx;		/* tv slot index or return
				   code from tvslot()	    */
	long sltval1, sltval2;
	long ndx;
	AUXENT	*ax;
#if FLEXNAMES
	char *strtabptr;
	int strtabsz;
#endif
	SYMENT	sm_buf;
	AUXENT	ax_buf;
	INSECT *isp;
	int swrite_flag, awrite_flag;
	char tmp_numaux;
	long		nsymstrip;
	char		stripflag;
#if PORTAR
	struct ar_hdr memberhdr;
#endif
	char	*symname;

/*
 * Initialize:
 *	1. Set slot vector starting slot number to zero
 *	2. Get the number and position of the symbols in the input file
 *	3. Allocate space for six arrays
 *	4. Position the input file to the symbol table
 */

	svhighslt = 0;

	i = HEADER(infd).f_nscns + 1;
	nsymstrip = 0L;
	ndx = infl->flsymndx;

	newscnum = (int *) myalloc( 2*(i*sizeof(int)) + 4*(i*sizeof(long)) );
	segment = newscnum + i;
	ovaddr = (long *) (segment + i);
	nvaddr = ovaddr + i;
	opaddr = nvaddr + i;
	npaddr = opaddr + i;

#if FLEXNAMES && PORTAR
	/* read string table if one exists */

	if (TYPE(infd) == USH_ARTYPE){
		if (FSEEK(infd,-(int)(sizeof(struct ar_hdr)),BEGINNING) != OKFSEEK ||
			FREAD(&memberhdr,sizeof(struct ar_hdr), 1, infd) != 1 ||
			strncmp(memberhdr.ar_fmag,ARFMAG, sizeof(memberhdr.ar_fmag)))
				lderror(2,0,NULL,"fail to read archive header of file %s",
				infl->flname);
		if (sscanf(memberhdr.ar_size,"%ld",&ar_size) != 1)
			lderror(2,0,NULL,"invalid archive size for file %s", infl->flname);
		}
	strtabsz = 0L;
	if (infl->flstrings)
		strtabptr = infl->flstrings;
	else
		strtabptr = strtabread(infd,infl->flname, &strtabsz);
#endif

	if( HEADER(infd).f_symptr <= 0L ||
	    FSEEK(infd,HEADER(infd).f_symptr,BEGINNING) != OKFSEEK )  {
		free( newscnum );
		return(0);
		}

/*
 * Define the contents of the six arrays
 */

	i = 0;
	for( isp = infl->flishead; isp; isp = isp->isnext ) {
		OUTSECT *os;
		newscnum[++i] = isp->isoutsec->ossecnum;
		ovaddr[i] = isp->ishdr.s_vaddr;
		opaddr[i] = isp->ishdr.s_paddr;
		nvaddr[i] = isp->isnewvad;
		npaddr[i] = isp->isnewpad;
		os = isp->isoutsec;
		segment[i] = (int) ((os->oshdr.s_paddr - os->oshdr.s_vaddr) >> 4);
		}

/*
 * Process each symbol in the symbol table
 */

	sm = &sm_buf;
	ax = &ax_buf;
	for( count = 0; count < HEADER(infd).f_nsyms; count++)  {

#if TRVEC
		tvndx = 0;
#else
		tvndx = -1;
#endif
		reloctag = 0;
		relocend = 0;

		if (fread(sm, SYMESZ, 1, IOPTR(infd)) != 1)
			lderror(2,0,NULL,"fail to read symbol table of file %s",
			infl->flname);

#if FLEXNAMES
		if (sm->n_zeroes == 0L)
		{
			if (strtabptr == NULL)
				lderror( 2, 0, NULL, "no string table in file %s", infl->flname );
			sm->n_nptr = strtabptr + sm->n_offset;
		}
#endif
		symname = PTRNAME(sm);

		switch(sm->n_sclass) {
		case C_EXT:
		case C_EXTDEF:
			gsp = findsym( symname );
			swrite_flag = 0;
			secnum = sm->n_scnum;
			/*
			 * Determine what will be stored in the slot vector
			 */

			procext(&tvndx, secnum, &sltval1, &sltval2, ovaddr, nvaddr, gsp, sm);

			/*
			 * Output defined local symbols
			 */
			if( gsp->smlocflg  &&  (secnum > 0) ) {
				svcreate(count, sltval1, sltval2, gsp->smoutndx, gsp->sment.n_scnum,
					(tvndx != -1) ? SV_TV : 0);
				if( ! sflag ) {
					char	*save_ptr;

					sltval1 = gsp->sment.n_value;
					gsp->sment.n_value = gsp->smnewval;
					if( Hflag ) {
						gsp->sment.n_sclass = C_STAT;
						swrite_flag = 1;
						}
#if FLEXNAMES
					if (gsp->sment.n_zeroes == 0L) {
						save_ptr = gsp->sment.n_nptr;
						strwrite(strdes,&gsp->sment, gsp->smnamelength);
					}
#endif
					fwrite(gsp, SYMESZ, 1, sdes);
#if FLEXNAMES
					if (gsp->sment.n_zeroes == 0L)
						gsp->sment.n_nptr = save_ptr;
#endif

					gsp->sment.n_value = sltval1;
					if( gsp->sment.n_numaux > 0 ) {
						auxp = getaux(gsp->smauxid);
						awrite_flag = 0;
						if( auxp->axent.x_sym.x_tagndx > 0 ) {
#if UNIX || DMERT || XL
#if UNIX
							if (xflag)
#endif
#if DMERT
							if (infl->flplib)
#endif
#if XL
							if (infl->flplib || xflag)
#endif
								auxp->axent.x_sym.x_tagndx = 0;
							else
#endif
								auxp->axent.x_sym.x_tagndx += infl->flsymndx - nsymstrip;
							awrite_flag = 1;
							}
						if( auxp->axent.x_sym.x_fcnary.x_fcn.x_endndx > 0 ) {
#if UNIX || XL
							if (xflag)
								auxp->axent.x_sym.x_fcnary.x_fcn.x_endndx = ndx + 2;
							else
#endif
								auxp->axent.x_sym.x_fcnary.x_fcn.x_endndx += infl->flsymndx - nsymstrip;
							awrite_flag = 1;
							}
						if( auxp->axent.x_sym.x_tvndx == P_TV)
							auxp->axent.x_sym.x_tvndx = N_TV;
#if DMERT || XL
						if (infl->flplib) {
							auxp->axent.x_sym.x_fcnary.x_fcn.x_lnnoptr = 0L;
							awrite_flag = 1;
							}
#endif

						fwrite(auxp, AUXESZ, 1, sdes);
						PUTAUX(auxp, awrite_flag);
						}
					}
				if( sm->n_numaux > 0 )  {
					if (fseek(IOPTR(infd), (long) (sm->n_numaux * AUXESZ), 1) != OKFSEEK)
						lderror(2,0,NULL,"fail to skip the aux entry of %s",
						infl->flname);
					}
				ndx += 1 + sm->n_numaux;
				count += sm->n_numaux;
				}
			else {
				/*
				 * Symbol is not defined (in this run)
				 */
				if ( (gsp->sment.n_scnum == 0) && aflag &&
					(tvndx == -1)) {
					/*
					 * Undefined symbol in an absolute run
					 * (ignore undefined tv symbols)
					 */
					svcreate(count, sltval1, sltval2, gsp->smoutndx,
						gsp->sment.n_scnum, SV_ERR);
					if( ! undefmsg ) {
						fprintf( stderr, "undefined\t\tfirst referenced\n" );
						fprintf( stderr, " symbol  \t\t    in file\n" );
						undefmsg = 1;
						errlev = (rflag) ? 1 : 2;
						}
					if( ! gsp->smundmsg ) {
						fprintf( stderr, "%-20.20s\t\t%s",
				      			symname, infl->flname);
						fprintf( stderr, "\n" );
						gsp->smundmsg = 1;
						swrite_flag = 1;
						}
			 		}
	 			else
					/*
					 * Undefine symbol, but run not absoute
					 * (or it has a tv slot)
					 */
					svcreate(count, sltval1, sltval2, gsp->smoutndx,
						gsp->sment.n_scnum,
						(tvndx != -1) ? SV_TV : 0);
			 	if( sm->n_numaux > 0 ) {
					if (fseek(IOPTR(infd), (long) (sm->n_numaux * SYMESZ), 1) != OKFSEEK)
						lderror(2,0,NULL,"fail to skip the aux entries of %s",
						infl->flname);
					count += sm->n_numaux;
					if (!sflag && (sectnum > 0 || (secnum == 0 && sm->n_value > 0 ))) {
						/*
						 * If symbol is defined here but not
						 * written here, relocate the tag index
						 * in the aux entry
						 */
						auxp = getaux(gsp->smauxid);
						if( auxp->axent.x_sym.x_tagndx > 0 ) {
#if UNIX || XL || DMERT
#if UNIX
							if (xflag)
#endif
#if DMERT
							if (infl->flplib)
#endif
#if XL
							if (infl->flplib || xflag)
#endif
								auxp->axent.x_sym.x_tagndx = 0;
							else
#endif
								auxp->axent.x_sym.x_tagndx += infl->flsymndx;
							PUTAUX(auxp, 1);
							}
						else
							PUTAUX(auxp, 0);
						}
					}
				}
			PUTSYM(gsp, swrite_flag);
			break;
		case C_STAT:
		case C_HIDDEN:
			/*
			 * Absolute statics are generated by the assembler
			 */
			if( sm->n_scnum <= 0 )
				goto wrtsym;
			sm->n_value += (npaddr[sm->n_scnum] - opaddr[sm->n_scnum]);

			proclocstat(sm, infd, segment, newscnum, &tvndx, ovaddr, nvaddr, opaddr, npaddr, ndx, &sltval1, &sltval2, count);

			if( ISFCN(sm->n_type) )
				relocend = 1;

		case C_FIELD:
		case C_REGPARM:
			/*
			 * .text, .data, and .bss are C_STATs with
			 * x_tagndx > 0. They also have secnum > 0,
			 * but they must not have x_tagndx relocated
			 * but static structs, unions and enums should
			 * so differentiate on BTYPE(fundamental type)
			 */

			if ( (BTYPE(sm->n_type)==T_ENUM) || (BTYPE(sm->n_type)==T_UNION) ||
					(BTYPE(sm->n_type)==T_STRUCT) )
				reloctag = 1;

			goto wrtsym;
		case C_BLOCK:
			relocend = 1;
		case C_LABEL:
		case C_FCN:
			sm->n_value += (npaddr[sm->n_scnum] - opaddr[sm->n_scnum]);
	    wrtsym:
#if UNIX
			if (sm->n_sclass != C_STAT && sm->n_sclass != C_FILE && xflag)  {
#endif
#if XL
			if (sm->n_sclass != C_STAT && sm->n_sclass != C_FILE && xflag || infl->flplib) {
#endif
#if DMERT
			if (infl->flplib) {
#endif
#if UNIX || DMERT || XL
				stripflag = 1;
				if (!sflag)  {
						nsymstrip++;
						}
				}
			else 
#endif
				stripflag = 0;

			if( !sflag && !stripflag ) {
				if( sm->n_scnum > 0 )
					sm->n_scnum = newscnum[sm->n_scnum];
#if FLEXNAMES
				if (sm->n_zeroes == 0L)
					strwrite(strdes,sm, strlen( sm->n_nptr ) + 1);
#endif
				fwrite(sm, SYMESZ, 1, sdes);
				ndx++;
				}
			tmp_numaux = sm->n_numaux;
			for( i = 0; i < tmp_numaux; i++ ) {
				if (FREAD(ax,AUXESZ,1,infd) != 1)
					lderror(2,0,NULL,"fail to read aux entries of file %s",
					infl->flname);
#if UNIX || DMERT || XL
				if (!sflag && stripflag)  {
					nsymstrip++;
					}
#endif

				if( !sflag && !stripflag)  {
					if( reloctag && ax->x_sym.x_tagndx > 0 )
#if UNIX || XL || DMERT
#if UNIX
						if (xflag)
#endif
#if DMERT
						if (infl->flplib)
#endif
#if XL
						if (xflag || infl->flplib)
#endif
							ax->x_sym.x_tagndx = 0;
						else
#endif
							ax->x_sym.x_tagndx +=infl->flsymndx - nsymstrip;
					if( relocend && ax->x_sym.x_fcnary.x_fcn.x_endndx > 0 )
#if UNIX || XL
						if (xflag)
							ax->x_sym.x_fcnary.x_fcn.x_endndx += ndx + 1;
						else
#endif
							ax->x_sym.x_fcnary.x_fcn.x_endndx += infl->flsymndx - nsymstrip;
#if TRVEC
					if( (tvndx != -1)  &&  aflag )
						ax->x_sym.x_tvndx = tvndx;
					if (ax->x_sym.x_tvndx == P_TV)
						ax->x_sym.x_tvndx = N_TV;
#endif
					fwrite(ax, AUXESZ, 1, sdes);
					ndx++;
					}
				count++;
				}
			break;
		case C_STRTAG:
		case C_UNTAG:
		case C_ENTAG:
			/*
			 * Relocate x_tagndx and x_endndx
			 */
			relocend = 1;
		case C_ARG:
		case C_AUTO:
		case C_EOS:
		case C_MOS:
		case C_MOU:
		case C_REG:
		case C_TPDEF:
			/*
			 * Relocate only x_tagndx
			 */
			reloctag = 1;
			goto wrtsym;
		case C_FILE:
			/*
			 * Don't relocate since it is not a symbol name
			 *
			 * Update previous .file entry (if any) and
			 * set new value for later update
			 * This should chain .file entries together,
			 * for field update, but it is assumed that
			 * each input *.o file contains a .file entry
			 * as the first entry in its symbol table
			 */
#if DMERT || XL
			if (!sflag && !infl->flplib && (lastfile != -1L) && (nextfile != ndx))
#else
			if (!sflag && (lastfile != -1L) && (nextfile != ndx))
#endif
				setfile( sdes, ndx );
			if (infl->flnext)
			{
				nextfile = infl->flnext->flsymndx;
				sm->n_value = nextfile;
			}
			lastfile = ndx;


		default:
			/*
			 * no relocation 
			 */
			goto wrtsym;
		}
		}

	i = HEADER(infd).f_nscns + 1;
	free( newscnum );

#if FLEXNAMES
	if (last_string_ptr)
	{
		fwrite( last_string_ptr, string_buf_length, 1, strdes );
		last_string_ptr = NULL;
		string_buf_length = 0;
	}
	if (strtabptr && (strtabsz > MAXKEEPSIZE))
		free( strtabptr );
#endif
	
	return(ndx - infl->flsymndx);
}




wrtaout(fdes, fhdr)
FILE *fdes;		/* file descriptor seeked to start of optional hdr */
FILHDR *fhdr;
{

/*
 * Write UNIX aout header into optional header block, if -X flag set.
 * This function assumes that the sections .text, .data, and .bss
 * exist by name.
 */

	AOUTHDR aout;
	OUTSECT *osptr;
	SYMTAB *symp;
	char found;

#if M32
	aout.magic = ( Nflag ? AOUT1MAGIC : AOUT2MAGIC );
#else
	aout.magic = ( Nflag ? AOUT2MAGIC : AOUT1MAGIC );
#endif
#if PAGING
	if ( Fflag && !Nflag )
		aout.magic = PAGEMAGIC;
#endif
	found = 0;
	for( osptr = (OUTSECT *) outsclst.head; osptr; osptr=osptr->osnext ) {
		if( equal(osptr->oshdr.s_name, _TEXT, 8) ) {
			aout.tsize = osptr->oshdr.s_size;
			aout.text_start = osptr->oshdr.s_paddr;
			found++;
			}
		else if( equal(osptr->oshdr.s_name, _DATA, 8) ) {
			aout.dsize = osptr->oshdr.s_size;
			aout.data_start = osptr->oshdr.s_paddr;
			found++;
			}
		else if( equal(osptr->oshdr.s_name, _BSS, 8) ) {
			aout.bsize = osptr->oshdr.s_size;
			found++;
			}
		}

	adjaout(&aout);

	if( found != 3 )
		lderror(0,0,NULL,
		   "Sections .text .data or .bss not found. Optional header may be useless");

#if OLDAOUT
	aout.ssize = fhdr->f_nsyms * SYMESZ;
	aout.nsize = 0L;
	aout.flags = 0;
	if( ! rflag )
		aout.flags |= RELFLG;
	if( aout.text_start != 0L )
		aout.flags |= SETTEXT;
	if( aout.text_start + aout.tsize != aout.data_start )
		aout.flags |= SETDATA;
	if( sflag )
		aout.flags |= NSYMFLG;
#else
	aout.vstamp = VSflag;
#endif

/*
 * The entry point is obtained from the following sources, in order:
 *
 *	1. value of the symbol given in the "-e" flag
 *	2. value of the symbol _CSTART
 *	3. value of the symbol MAIN
 */

	if ( (symp = findsym(epsymbol))  ||
	     (symp = findsym(_CSTART)) ||
	     (symp = findsym(_MAIN)) ) {
		aout.entry = symp->sment.n_value;
		PUTSYM(symp, 0);
	} else
		aout.entry = 0L;

	fwrite(&aout, sizeof(AOUTHDR), 1, fdes);
}
/*eject*/
wrtplst(fdes)
FILE *fdes;
{
/*
 * Output the patch list
 */

	register int index;
	register OUTSECT *osptr;
	long	count;
	char pname[10];
	PENTRY block;
	OUTSECT *fndoutsec();

/*
 * The file pointer fdes is assumed positioned at the start of the
 * patch list
 *
 * Step 1: Output the block count
 */

	count = ( (long)((psize() - sizeof(long)) / PESIZE));
	fwrite(&count, sizeof(long), 1, fdes);

/*
 * Step 2: Output a PENTRY structure for each patch segment
 */

	for( index = 0; index < count; index++ ) {
		if (index < 10)
			sprintf( pname, ".patch0%d", index);
		else
			sprintf(pname, ".patch%d", index);
		osptr = fndoutsec(pname);
		block.type01.blk_type = (char) FREE_patch;
		block.type01.blk_addr = osptr->oshdr.s_paddr;
		block.type01.blk_size = osptr->oshdr.s_size;
		block.type01.blk_scnum = (unsigned short) osptr->ossecnum;
		fwrite(&block, PESIZE, 1, fdes);
		}
}


setfile( symdes, cursym )

FILE	*symdes;
long	cursym;
{

	/*
	 * Update the previous .file entry in the symbol table
	 *
	 *	symdes = symbol table output file
	 *	cursym = index of symbol table entry about to be written
	 *
	 *	static lastfile = index of previous .file entry
	 */

	long	here,		/* where we are now */
		there;		/* where to write update value */

	here = symtborg + cursym * (long)SYMESZ;
	there= symtborg + (lastfile * (long) SYMESZ) + (long)SYMNMLEN;
	fseek(symdes, there-here, 1);
	fwrite(&cursym, sizeof(long), 1, symdes);
	fseek(symdes, here-there-sizeof(long), 1);
	return(1);
}

#if FLEXNAMES
strwrite(strdes,sm, strlength)
FILE *strdes;	/* pointer to current position in string table */
SYMENT *sm; 	/* ptr to symbol whose name is to be written in string table */
int strlength;
{
	if ((last_string_ptr + string_buf_length) == sm->n_nptr)
		string_buf_length += strlength;
	else
	{
		if (last_string_ptr && fwrite(last_string_ptr, string_buf_length, 1, strdes ) != 1)
			lderror(2,0,NULL, "fail to write symbol name %s in string table for file %s",
				last_string_ptr, outfilnm);
		last_string_ptr = sm->n_nptr;
		string_buf_length = strlength;
	}

	sm->n_offset = stroffset;
	stroffset += strlength;
}
#endif
