/*
 *	static char ID_sec[] = "@(#) pass2.c: 1.3 3/5/83";
*/

#define BSIZE 256

#include <stdio.h>
#include "filehdr.h"
#include "syms.h"
#include "scnhdr.h"
#include "linenum.h"
#include "reloc.h"
#include "ldfcn.h"
#include "tagitem.h"

/*  pass 2 of the compress utility compresses the symbol table and updates
    all pointers into the symbol table  */

 extern LDFILE *oldfile;
 extern FILE *compress;
extern	int	copy();

struct table{
	long 	oldlcn;
	long 	newlcn;
	long 	real_loc;
};


secpass(argv, argv2)

char *argv;
char *argv2;
{

	extern long 	delsum, tag_sum;
	extern int	vflag, pflag;
	extern long 	szecnt;
	extern struct table *tbl_head;
	long		newindx();
	long 		proln();
	long 		prorel();
	SCNHDR 		scptr;
	FILHDR 		fhead;
	extern LDFILE 	*ldopen();

	long		indx;
	long		start;
	long		datasze;
	int		sumrel;
	int		sumln;
	int		totsec;
	long		frstrel;
	long		frstln;
	long		onsyms;
	int 		i;


#ifdef FLEXNAMES
	reinit_strings();
#endif
	sumrel = sumln = 0;
	indx = 0;
	frstrel = frstln = 0;

	/* open files for reading and writing   */

	if ((oldfile=ldopen(argv, NULL)) == NULL) {
		fprintf(stderr, "unable to open %s \n", argv);
		return(ERROR);
	}

	if ( (compress = fopen(argv2, "w")) == NULL) {
		fprintf(stderr, "unable to create %s\n",argv2);
		return(ERROR);
	}


	/*  read and modify header information     */
	if ( ldfhread(oldfile, &fhead) == FAILURE) {
		if ( vflag > 0)
			fprintf(stderr, "unable to read header\n");
		return(FAILURE);
	}
	start = HEADER(oldfile).f_symptr;
	onsyms = HEADER(oldfile).f_nsyms;
	fhead.f_nsyms = HEADER(oldfile).f_nsyms - delsum;
	totsec = HEADER(oldfile).f_nscns;
	indx += FILHSZ;

	/*  copy adjusted header to compress file last   */

	if ( fseek(compress, (long)FILHSZ, 0) != 0) {
		if ( vflag > 0)
			fprintf(stderr,"seek past filheader failed\n");
		return(FAILURE);
	}

	/*  if there is an optional header   copy to compress  */

	if ( ldohseek(oldfile) == SUCCESS) {
		if (copy((long)HEADER(oldfile).f_opthdr) != SUCCESS) {
			if ( vflag > 0)
				fprintf(stderr, "ERROR; unable to copy opt header\n");
			return(FAILURE);
		}
	}

	indx += HEADER(oldfile).f_opthdr;

	/*  read section header information    */

	for( i=0; i<totsec; i++) {
		if (fread(&scptr, SCNHSZ, 1, IOPTR(oldfile)) != 1) {
			if ( vflag > 0)
				fprintf(stderr, "read on scnheader failed\n");
			return(FAILURE);
		}

		/*  save value for index of first reloc and line sections   */

		if (i==0 || frstrel == 0)
			frstrel = scptr.s_relptr;
		if (i == 0 || frstln == 0)
			frstln = scptr.s_lnnoptr;
		sumrel += scptr.s_nreloc;
		sumln += scptr.s_nlnno;

	/*  copy section information to compress file   */

		if ( fwrite(&scptr, SCNHSZ, 1, compress) != 1) {
			if( vflag > 0) 
				fprintf(stderr, "write on scnheader failed\n");
			return(FAILURE);
		}
		indx += SCNHSZ;
	}


	/*  read raw data for all sections and copy to compress   */

	if ( sumrel !=0)
		datasze = frstrel - indx;
	else
		if (sumln !=0)
	  		datasze = frstln - indx;
	else
		datasze = start - indx;
	if ( datasze != 0)
		if (copy((long)datasze) != SUCCESS) {
			if ( vflag > 0)
				fprintf(stderr, "ERROR; unable to copy raw data\n");
			return(FAILURE);
			}
	indx += datasze;

	/*  if header flag not set, update relocation section  */

	if ( !(HEADER(oldfile).f_flags & F_RELFLG) && sumrel != 0)
		if (( indx = prorel(indx, frstrel, sumrel)) == FAILURE) {
			if ( vflag > 0)
				fprintf(stderr, "indx at prorel wrong\n");
			return(FAILURE);
		}

	/* update linenumber section if flag not set    */

	if ( !(HEADER(oldfile).f_flags & F_LNNO) && sumln != 0L)
		if ((indx = proln(indx, frstln, sumln)) == FAILURE) {
			if ( vflag > 0)
				fprintf(stderr, "indx at proln wrong\n");
			return(FAILURE);
		}

	/*  call function to modify symbol table  */

	if (prosym(indx, onsyms, start) == FAILURE) {
		if ( vflag > 0)
			fprintf(stderr, "prosym returns failure\n");
		return(FAILURE);
	}

	if ( fseek(compress, 0L, 0) != 0)
		if ( vflag > 0)
			fprintf(stderr, "seek to start of object file failed");
	if (fwrite(&fhead, FILHSZ, 1, compress) != 1) {
		if ( vflag > 0)
			fprintf(stderr, "writing fileheader failed\n");
		return(FAILURE);
	}
	if ( pflag > 0)
		printf("compression reduces symbol table %ld%%\n", (delsum * 100L) / onsyms);
	/*  compression complete;  close both files  */
	fclose(compress);
	ldaclose(oldfile);
	return(SUCCESS);
}


/*eject*/
/* function "proln" adjust linenumber information for compressed file  */


long
proln(indx,lnindx,lnsum)

long indx, lnindx;
int lnsum;
{
	extern long	newindx();

	extern int	vflag;

	struct  lineno lnptr;
	int 	k;

	/* verify position of file ptr.  */

	if ( indx != lnindx)
		if ( vflag > 0)
			fprintf(stderr, "could not indx to line number \n");
	for( k=0; k < lnsum; k++) {

		if ( FREAD(&lnptr, LINESZ, 1, oldfile) != 1) {
			if ( vflag > 0)
				fprintf(stderr, "cannot read lineno infor\n");
			return(FAILURE);
		}
		if (lnptr.l_lnno == 0)
			lnptr.l_addr.l_symndx=newindx(lnptr.l_addr.l_symndx);
		if ( fwrite(&lnptr, LINESZ, 1, compress) != 1) {
			if ( vflag > 0)
				fprintf(stderr, "write to lineno failed\n");
			return(FAILURE);
		}
		indx += LINESZ;
	}
	return(indx);
}


/*eject*/
/*  function prorel, updates relocation entries for compression  *?
*/

long
prorel(indx,frstrel,sumrel)

long indx, frstrel; 
int sumrel;
{
	extern int	vflag;

	struct reloc 	relptr;
	long 		k;

	/*  verify position of file ptr to start of reloc section   */

	if ( indx != frstrel) {
		if ( vflag > 0)
			fprintf(stderr, "error in index computation at reloc\n");
		exit(1);
	}
	if ( vflag > 0)
		fprintf(stderr, "total number of rel entries = %d\n",sumrel);

	for( k=0; k<sumrel; k++) {
		if ( fread(&relptr, RELSZ, 1, IOPTR(oldfile)) < 1) {
			if ( vflag > 0)
				fprintf(stderr, "read rel failed\n");
			return(FAILURE);
		}

		relptr.r_symndx = newindx(relptr.r_symndx);
		if ( fwrite(&relptr, RELSZ, 1, compress) != 1) {
			if ( vflag > 0)
				fprintf(stderr, "write to rel failed\n");
			return(FAILURE);
		}

		indx = indx + RELSZ;
	}
	return(indx);
}



/*eject*/

/*  function "newindx" computes newloc for pointers into the symbol table  */

long
newindx(x)

long x;
{

	extern struct table *tbl_head;
	extern long 	tag_sum;
	extern long 	delsum;
	extern int	vflag;

	struct table 	*tbl_ptr;
	long 		i; 

	/*  compare number against old/new/real table,  if less than 1st entry
 	 *  return the same value         */

	tbl_ptr = tbl_head;
	for (i = 0L; i < tag_sum; i++) {
		if ( x == tbl_ptr->oldlcn) {
			return(tbl_ptr->newlcn);
			}
		if ( x < tbl_ptr->oldlcn) {
			if ( tbl_ptr == tbl_head)
				return(x);
			x -= (tbl_ptr->oldlcn - tbl_ptr->newlcn);
			return(x);
		}
		if ( i == tag_sum-1 && x > tbl_ptr->oldlcn) {
			x -= delsum;
			return(x);
		}
	if ( i != tag_sum-1 ) {
		++tbl_ptr;
		}
	}
	if ( vflag > 0)
		fprintf(stderr, "failed to compute indx\n");
	exit(1);
}


/*eject*/

/*  function "realindx" gives actual index of any "real" tag  */

/* given that newloc != oldloc in our table, tag is C_ALIAS  */

long
realindx(x)

long x;
{
	long 	i;
	extern long tag_sum;
	extern int	vflag;
	extern struct table *tbl_head;

	struct table *tbl_ptr;

	/*   search table til x lies between 2 tag indexes   */

	tbl_ptr = tbl_head;

	for( i=0L; i<tag_sum; i++) { 
		if (x != tbl_ptr->oldlcn) {
			if ( i != tag_sum-1 ) {
				++tbl_ptr;
				}
			}
		else {
			return(tbl_ptr->real_loc);
		}
	}
	if ( vflag > 0)
		fprintf(stderr, "failed to compute realindx at %ld\n", x);
	exit(2);
}

/*eject*/
/*  "prosym" processes entries in symbol table by actually deleting 
 *     duplicate structures and modifying pointer information   */

struct syment symbol;
union auxent auxil;

extern char *str_table, *str_next;

int
prosym(indx, nsyms, start)

long indx, nsyms, start;
{
	extern int	vflag,
			pflag;
	extern long	realindx(),
			newindx();
	long 	nextin, thisin;
	long 	duptag;
	char 	*s;
	long	slength;

	/*  verify that index is start of symbol table    */

	duptag = 0;
	if ( indx != start) {
		if ( vflag > 0)
			fprintf(stderr, "error in symbol table indexing \n");
		return(FAILURE);
	}

	/*  read each entry and/or aux entry        */

	nextin = 0;
	while (nextin < nsyms) {
		thisin = nextin;
		if ( ldtbread(oldfile, thisin, &symbol) == FAILURE) {
			if ( vflag > 0)
				fprintf(stderr, "unable to read symbol entry\n");
			return(ERROR);
		}
#ifdef FLEXNAMES
		if (symbol.n_zeroes == 0)	/* put name in string table */
		{
			extern char *keep_name();
			char *p;

			if ((p = keep_name(oldfile, &symbol)) == NULL)
				return (ERROR);
			symbol.n_offset = p - str_table;	/* new offset */
		}
#endif
		if ( symbol.n_numaux == 0)
			nextin = thisin + 1;
		else {
			nextin=thisin + 2;

		/*  read aux entry      */

			if ( fread(&auxil, AUXESZ, 1, IOPTR(oldfile))==FAILURE) {
				if ( vflag > 0)
					fprintf(stderr, "cannot read auxiliary entry\n");
				return(ERROR);
			}
		}
		if ( symbol.n_numaux == 0) {
			if ( symbol.n_sclass == C_ALIAS) {
				symbol.n_value = realindx(symbol.n_value);
			}
		}
		else {
			switch (symbol.n_sclass) {

				case C_BLOCK:
				  s = symbol.n_name;
				  if (*s=='.' && *(++s)=='b' && *(++s)=='b')
				      auxil.x_sym.x_fcnary.x_fcn.x_endndx=newindx(auxil.x_sym.x_fcnary.x_fcn.x_endndx);
				  break;

				case C_ENTAG:
				  auxil.x_sym.x_fcnary.x_fcn.x_endndx = newindx(auxil.x_sym.x_fcnary.x_fcn.x_endndx);
				  auxil.x_sym.x_tagndx=newindx(auxil.x_sym.x_tagndx);
				  break;

				case C_EOS:
				  auxil.x_sym.x_tagndx=newindx(auxil.x_sym.x_tagndx);
				  break;

				case C_FILE:
				  symbol.n_value=newindx(symbol.n_value);
				  break;

				case C_STRTAG:
				case C_UNTAG:
  				  if ( tagindx(thisin)==C_ALIAS) {
				    nextin = auxil.x_sym.x_fcnary.x_fcn.x_endndx;
				    duptag++;
				  } else
				    auxil.x_sym.x_fcnary.x_fcn.x_endndx=newindx(auxil.x_sym.x_fcnary.x_fcn.x_endndx);
				  break;

				default:
				  if (ISFCN(symbol.n_type)) {
				    auxil.x_sym.x_fcnary.x_fcn.x_endndx=newindx(auxil.x_sym.x_fcnary.x_fcn.x_endndx);
				  }

				  switch BTYPE(symbol.n_type) {

				  case T_ENUM:
				    if (auxil.x_sym.x_tagndx == 0L)
					break;
				    else
				    auxil.x_sym.x_tagndx = newindx(auxil.x_sym.x_tagndx);
				    break;

				  case T_STRUCT:
				  case T_UNION:
				    if (auxil.x_sym.x_tagndx == 0L)
					break;
				    else
				    auxil.x_sym.x_tagndx = realindx(auxil.x_sym.x_tagndx);
				    break;
				  }  /* end of second switch  */

				break;
			}  /*  end of main switch  */
		}  /*  end of else  */

		if ( fwrite(&symbol, SYMESZ, 1, compress) != 1) {
			if ( vflag > 0)
				fprintf(stderr, "failed to write symbol\n");
			return(FAILURE);
		}
		if ( symbol.n_numaux == 1)
			if ( fwrite(&auxil, AUXESZ, 1, compress) != 1) {
				if ( vflag > 0)
					fprintf(stderr, "failed to write aux\n");
				return(FAILURE);
			}
		}   /*   end of while loop  */

#ifdef FLEXNAMES
	/*
	* Write out modified string table.  Place string table length
	* at beginning.  Don't bother to check stuff at end of old file.
	*/
	slength = str_next - str_table;
	if (slength > sizeof(long) / sizeof(char))
	{
		str_table += sizeof(long) / sizeof(char);
		if (fwrite((char *)&slength, sizeof(long), 1, compress) != 1 ||
			fwrite(str_table, sizeof(char),
			slength - sizeof(long) / sizeof(char), compress) !=
			slength - sizeof(long) / sizeof(char))
		{
			if (vflag > 0)
				fprintf(stderr,
					"failed to write string table\n");
			return (FAILURE);
		}
	}
#else
	/*  verify that no more entries exist     */
	if ( ldtbread(oldfile, nextin, &symbol) != FAILURE) {
		if ( vflag > 0)
			fprintf(stderr, "error in count of symbols\n");
		exit(3);
	}
#endif
	if ( pflag > 0)
		printf("total number of duplicate tags = %ld\n", duptag);
	return(SUCCESS);
}



/*eject*/

/*  "tagindx" computes value for C_ALIAS entries   */


int
tagindx(symindx)

long symindx;
{
	extern long tag_sum;
	extern struct table *tbl_head;
	struct table *tbl_ptr;
	long 	i;

	/* search tag table for oldloc of given tag indx   */

	tbl_ptr = tbl_head;

	for( i=0L; i < tag_sum; i++) {
		if ( symindx == tbl_ptr->oldlcn) {
			if ( tbl_ptr->newlcn != tbl_ptr->real_loc) {
				symbol.n_sclass = C_ALIAS;
				symbol.n_numaux = 0;
				symbol.n_value = tbl_ptr->real_loc;
				return(C_ALIAS);
			}
		return(0);
		}
		if ( i != tag_sum-1 ) {
			++tbl_ptr;
			}
	}
	return(0);
}


/*eject*/
int
copy(size)

long	size;

{
    extern int		fread( ),
			fwrite( );

	extern FILE	*compress;
	extern LDFILE	*oldfile;

/* */    short		buffer[BSIZE];
    long		index;
/* */    short		remaindr;


    for (index = size/BSIZE; index > 0; --index) {
	if (fread(buffer, BSIZE, 1, IOPTR(oldfile)) < 1) {
	    return(FAILURE);
	}

	if (fwrite(buffer, BSIZE, 1, compress) != 1) {
	    return(FAILURE);
	}
    }

    if ((remaindr = size % BSIZE) != 0) {
	if (fread(buffer, remaindr, 1, IOPTR(oldfile)) != 1) {
	    return(FAILURE);
	}

	if (fwrite(buffer, remaindr, 1, compress) != 1) {
	    return(FAILURE);
	}
    }

    return(SUCCESS);
}

#ifdef FLEXNAMES
reinit_strings()
{
	register int i;

	str_next = str_table + (sizeof(long) / sizeof(char));
	for (i=0; i < sizeof(long) / sizeof(char); i++)
		str_table[i] = '\0';
}
#endif
