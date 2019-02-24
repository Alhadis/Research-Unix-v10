static char ID[] = "@(#) special2.c: 1.12 9/23/83";

#include <stdio.h>

#include "system.h"
#include "aouthdr.h"
#include "structs.h"
#include "ldfcn.h"
#include "tv.h"
#include "slotvec.h"
#include "reloc.h"
#include "extrns.h"
#include "sgsmacros.h"
#include "list.h"
#include "sgs.h"

void
adjneed(need, osp, sap)

ADDRESS	*need;
OUTSECT	*osp;
ANODE	*sap;
{
	/*
	 * adjust amount of memory allocated for a particular
	 * section;  special processing step now used only
	 * for DMERT with TRVEC
	 */
}

void
undefine()
{
	/*
	 * for m32 version of ld01.c, the symbol "_start" does not
	 * need to be undefined
	 */
}

void
dfltsec()
{
/*
 * For the m32, the default action of nm32ld is to generate two m32
 * segments: one for the text and the other for the data (both
 * .data and .bss)
 */

	ADDRESS lint = 0x80000;
	ADDRESS *lptr = &lint;
	ACTITEM *aiptr, *grptr;

/*
 * If any SECTIONS directives have been input, they take priority,
 * and no default action is taken
 */

#if ONEPROC
	if( bldoutsc.head )
#else
	if( bldcnt)
#endif
		return;

#if PAGING
	bond_t_d = 1;	/* setting bond_t_d will tell pboslist to bind text
				and data to the needed addresses */
#endif

/*
 * Generate a series of action itmes, as if the following had been
 * input to m32ld:
 *
 *	SECTIONS {
 *		.text : {}
 *		GROUP ALIGN(0x80000) : {
 *					.data : {}
 *					.bss  : {}
 *		 }
 *
 *
 * Steps:
 *	1. Define background variables
 *	2. Process the .text definition
 *	3. Process the GROUP definition
 *	4. Process the .data definition
 *	5. Process the .bss definition
 *
 *
 * The logic used was obtained by simulating the action of the parser.
 */

	memorg = 0x80800000L;
	curfilnm = "*m32.default.file*";

	lineno = 2;
	aiptr = dfnscngrp( AIDFNSCN, NULL, NULL, NULL );
	aiptr->dfnscn.aisctype = STYP_REG;
	copy( aiptr->dfnscn.ainame, _TEXT, 8 );
	aiptr->dfnscn.aifill = 0;
	aiptr->dfnscn.aifillfg = 0;
	listadd( l_AI, &bldoutsc, aiptr );

	lineno = 3;
	grptr = dfnscngrp( AIDFNGRP, NULL, lptr, NULL );
	copy( grptr->dfnscn.ainame, "*group*", 7);

	lineno = 4;
	aiptr = dfnscngrp( AIDFNSCN, NULL, NULL, NULL );
	aiptr->dfnscn.aisctype = STYP_REG;
	copy( aiptr->dfnscn.ainame, _DATA, 8 );
	aiptr->dfnscn.aifill = 0;
	aiptr->dfnscn.aifillfg = 0;
	listadd( l_AI, &grptr->dfnscn.sectspec, aiptr );

	lineno = 5;
	aiptr = dfnscngrp( AIDFNSCN, NULL, NULL, NULL );
	aiptr->dfnscn.aisctype = STYP_REG;
	copy( aiptr->dfnscn.ainame, _BSS, 8 );
	aiptr->dfnscn.aifill = 0;
	aiptr->dfnscn.aifillfg = 0;
	listadd( l_AI, &grptr->dfnscn.sectspec, aiptr );
	 
	listadd( l_AI, &bldoutsc, grptr );

	return;
}

void
procspecobj(fdes, ap)
LDFILE *fdes;
ACTITEM *ap;
{
	/*
	 * No more additional files to check 
	 */

	lderror(1, ap->ldlbry.aiinlnno, ap->ldlbry.aiinflnm,
		"file %s is of unknown type: magic number = %06.1x",
		ap->ldlbry.aifilnam, TYPE(fdes));
}

void
adjsize(osp)
OUTSECT *osp;
{
	/*
	 * No special cases, no need to adjust size of output section
	 */
}

void
adjaout(aout)
AOUTHDR *aout;
{
	/*
	 * Indicate, via the MAGIC NUMBER, when the data segment starts
	 * on a m32 segment (512K bytes) boundary
	 */

	if( (aout->data_start & 0x7ffffL) == 0 )
		aout->magic = (short) AOUT2MAGIC;
}

void
relocate(ifd, rfd, infl, isp, osp, fdes, rdes, paddiff, relocfg, buf)
LDFILE *ifd, *rfd;
INFILE *infl;
INSECT *isp;
OUTSECT *osp;
FILE *fdes, *rdes;
long *paddiff;
register int *relocfg;
char *buf;
{
	/*
	 * Process the section
	 */

	int numbytes;
	long highaddr, vaddiff;
	register long rdif, ndx;
	union {
		long	 l;
		unsigned short u[2];
		short	 i[2];
		char	 c[4];
		} value;
	RELOC rentry;
	register SLOTVEC *svp;

#if RBO
	/*
	 * for reverse byte ordering, two longs must be read in at a time,
	 * as the field may span a word boundary, and the bytes will no longer
	 * be contiguous.
	 */
	union {
		long	l[2];
		char	c[8];
		} rbobuf;
	short buffered, pos, i;
	long nextbnd;
#endif

	highaddr = isp->ishdr.s_vaddr + isp->ishdr.s_size;
	vaddiff = isp->isnewvad - isp->ishdr.s_vaddr;
	*paddiff = isp->isnewpad - isp->ishdr.s_paddr;
#if RBO
	buffered = 0;
#endif

	for( ndx = isp->ishdr.s_vaddr; ndx < highaddr; ) {

		/*
		 * Read a relocation entry, if any left, and
		 * determine the size of the relocation field, in bytes
		 */
		if( *relocfg ) {
			if (fread(&rentry, RELSZ, 1, IOPTR(rfd)) != 1)
					lderror(2,0,NULL,"fail to read the reloc entries of section %.8s of %s",
				isp->ishdr.s_name,infl->flname);
			(*relocfg)--;
			switch (rentry.r_type) {

			case R_ABS:
				/*
				 * No relocation, so skip to the
				 * next entry
				 */
				if( rflag )
				    if( (svp = svread(rentry.r_symndx)) == NULL ) {
					lderror(1,0,NULL, "reloc entry (%10.1lx, %ld %d) found for non-relocatable symbol, in section %.8s of file %s",
						rentry, isp->ishdr.s_name, infl->flname);
					goto nextentry;
					}
				    else
					goto nullentry;
				goto nextentry;



			case R_DIR32:
			case R_DIR32S:
				break;


			default:
				lderror(2,0,NULL,
					"Illegal relocation type %d found in section %.8s in file %s",
					rentry.r_type, isp->ishdr.s_name, infl->flname);
			}
			}
		/*
		 * There are no (more) relocation entries, so 
		 * copy the rest of the section
		 */
		else {
#if RBO
			/*
			 * write out the contents of the reverse byte order
			 * buffer if necessary
			 */
			if (buffered) {
				buffered = 0;
				fwrite( &rbobuf.l[1], sizeof(long), 1, fdes);
				ndx += 4;
			}
#endif
			while( (rdif = highaddr - ndx) > 0 ) {
				rdif = min(BUFSIZ, rdif);
				if (FREAD(buf,(int) rdif,1,ifd) != 1 ||
				    fwrite(buf,(int) rdif,1,fdes) != 1)
					lderror(2,0,NULL,"fail to copy the rest of section %.8s of file %s",
					isp->ishdr.s_name,infl->flname);
				ndx += rdif;
				}
			continue;
			}

		/*
		 * Copy bytes that need no relocation
		 */
#if RBO
		pos = rentry.r_vaddr % 4;
		nextbnd = rentry.r_vaddr - pos;
		if ( ((nextbnd - ndx) > 0) && buffered) {
			buffered = 0;
			fwrite( &rbobuf.l[1], sizeof(long), 1, fdes );
			ndx += 4;
		}

		while ((rdif = nextbnd - ndx) > 0) {
#else
		while( (rdif = rentry.r_vaddr - ndx) > 0 ) {
#endif
			rdif = min(BUFSIZ, rdif);
			if (FREAD(buf,(int) rdif,1,ifd) != 1 ||
			    fwrite(buf,(int) rdif,1,fdes) != 1)
				lderror(2,0,NULL,"fail to copy the bytes that need no reloc of section %.8s of file %s",
				isp->ishdr.s_name,infl->flname);
			ndx += rdif;
			}

		if( rdif < 0 ) {
			lderror(2,0,NULL, "Reloc entries out of order in section %.8s of file %s",
				isp->ishdr.s_name, infl->flname);
			}

		if( (svp = svread(rentry.r_symndx)) == NULL ) {
			lderror(1,0,NULL, "No reloc entry found for symbol: index %ld, section %.8s, file %s",
				rentry.r_symndx, isp->ishdr.s_name, infl->flname);
				goto nextentry;
				}

		rdif = svp->svnvaddr - svp->svovaddr;

		/*
		 * Special processing for COPY sections:
		 *  do not touch the raw data (no relocation)
		 *  but process relocation entries and lineno
		 *  entries as usual
		 */

		if ( osp->oshdr.s_flags & STYP_COPY )
			goto nullentry;

		switch( rentry.r_type ) {

		case R_DIR32:
			if (FREAD( &value.l, sizeof(long), 1, ifd) != 1)
				lderror( 2,0,NULL, "fail to read the field to be relocated in section %.8s, file %s",
					isp->ishdr.s_name, infl->flname);
			value.l = swapb4( swapb4( value.l, 0 ) + rdif, 0);
			fwrite( &value.l, sizeof(long), 1, fdes );
			ndx += 4;
			break;

		case R_DIR32S:
#if RBO
			if ( !buffered )
				if (FREAD( &rbobuf.l[1], sizeof(long), 1, ifd ) != 1)
					lderror( 2,0,NULL, "fail to read the field to be relocated in section %.8s, file %s",
						isp->ishdr.s_name, infl->flname);
			if (FREAD( &rbobuf.l[0], sizeof(long), 1, ifd ) != 1)
				lderror( 2,0,NULL, "fail to read the field to be relocated in section %.8s, file %s",
					isp->ishdr.s_name, infl->flname);
			for ( i = 0; i <= 3; i ++ )
				value.c[i] = rbobuf.c[7 - (pos + i)];
#else
			if (FREAD( &value.l, sizeof(long), 1, ifd) != 1)
				lderror( 2,0,NULL, "fail to read the field to be relocated in section %.8s, file %s",
					isp->ishdr.s_name, infl->flname);
#endif

			value.l = swapb4( swapb4(value.l, 1) + rdif, 1 );

#if RBO
			for ( i = 0; i <= 3; i++ )
				rbobuf.c[7 - (pos + i)] = value.c[i];
			fwrite( &rbobuf.l[1], sizeof(long), 1, fdes );
			rbobuf.l[1] = rbobuf.l[0];
			buffered = 1;
#else
			fwrite( &value.l, sizeof(long), 1, fdes );
#endif
			ndx += 4;
			break;
		}

	nullentry:	/* 
		 	 * Preserve relocation entries
			 */
		if( rflag ) {
			rentry.r_vaddr += vaddiff;
			rentry.r_symndx = svp->svnewndx;
			fwrite(&rentry, RELSZ, 1, rdes);
			}

	nextentry:	;
	}

}

long
swapb4(val, type)
long val;
int type;
{
	register long result;


/*
 * Reorder the bytes of a long, converting to/from UNIX/M32 format
 *
 *	val	: w x y z
 *
 *	11/70	:0:	xwzy
 *		:1:	yzwx
 *	vax	:0:	zyxw
 *		:1:	no change
 *
 *	other	:0:	no change
 *		:1:	zyxw
 */

	if( type ) {
#if AR16WR || AR32W
		result = (val >> 16) & 0x0000FFFFL;
		result |= (val << 16) & 0xFFFF0000L;
#if AR32W
		val = result;
		result = (val >> 8) & 0x00FF00FFL;
		result |= (val << 8) & 0xFF00FF00L;
#endif
#else
		result = val;
#endif
		}
	else {
#if AR16WR || AR32WR
		result = (val >> 8) & 0x00FF00FFL;
		result |= (val << 8) & 0xFF00FF00L;
#if AR32WR
		val = result;
		result = (val >> 16) & 0x0000FFFFL;
		result |=(val << 16) & 0xFFFF0000L;
#endif
#else
		result = val;
#endif
		}

	return( result );

}

void
procext(tvndx, secnum, sltval1, sltval2, ovaddr, nvaddr, gsp, sm)
int *tvndx, secnum;
long *sltval1, *sltval2, *ovaddr, *nvaddr;
register SYMTAB *gsp;
register SYMENT *sm;
{
			if( secnum > 0 ) {
				*sltval1 = ovaddr[secnum];
				*sltval2 = nvaddr[secnum];
				}
			else  {
				*sltval1 = sm->n_value;
				*sltval2 = gsp->smnewvirt;
				}
}

void
proclocstat(sm, infd, segment, newscnum, tvndx, ovaddr, nvaddr, opaddr, npaddr, ndx, sltval1, sltval2, count)
register SYMENT *sm;
LDFILE *infd;
int *segment, *newscnum, *tvndx;
long *ovaddr, *nvaddr, *opaddr, *npaddr, ndx, *sltval1, *sltval2, count;
{

			*sltval1 = ovaddr[sm->n_scnum];
			*sltval2 = nvaddr[sm->n_scnum];
			svcreate(count, *sltval1, *sltval2, ndx, newscnum[sm->n_scnum],
				0);

}

void
setslot(pglob, psect, outslot)
SYMTAB *pglob;
SCNHDR *psect;
TVENTRY *outslot;
{
	/*
	 * no user defined transfer vector slot in tv.c for m32
	 */
}

void
slotassign(tvent, pglob)
TVENTRY *tvent;
SYMTAB *pglob;
{
	long	paddr;

	paddr = pglob->smnewval;

	/*
	 * do not need to assign value to transfer vector
	 * entry for global symbol in tv.c for m32
	 */
}

void
settventry(tvent, psym)
TVENTRY *tvent;
struct syment *psym;
{
	/*
	 * no values to assign to transfer vector entry for local
	 * symbol in tv.c for m32
	 */
}

void
filltvassign(psym)
SYMTAB *psym;
{
	/*
	 * no fill values to assign for transfer vector in tv.c for m32
	 */
}
