/*
*     static char ID_cp[] = "@(#) copy.c: 1.4 10/7/83";

*/
#define	ERROR	0
#define BSIZE	256
#include 	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include 	"filehdr.h"
#include	"scnhdr.h"
#include	"reloc.h"
#include	"syms.h"
#include	"linenum.h"
#include	"ldfcn.h"

LDFILE 	*oldfile;
FILE	*compress;

int
cpyfile(argv,argv2)
char	*argv;
char	*argv2;
{

	extern int	copy();
	extern int	vflag;
	FILHDR 		fhead;
	LINENO		line;
	RELOC		rel;
	SCNHDR		sect;
	SYMENT		symbol;
	AUXENT		auxil;
	long		indx;
	long		datasze;
	int		sumrel;
	int		sumln;
	long		frstrel;
	long		frstln;
	int		i;
	long		k;
#ifdef FLEXNAMES
	char		buf[BUFSIZ];
#endif
	struct stat	statrec;
	unsigned short	fmode;

	indx = 0;
	sumrel = 0;
	sumln = 0;
	frstrel = 0;
	frstln = 0;

	if ((oldfile = ldopen(argv, NULL)) == NULL) {
	        fprintf(stderr,"unable to open %s \n", *argv);
		return(FAILURE);
	}
	if (stat(argv, &statrec) != 0) {
		fprintf(stderr,"unable to obtain mode of %s \n", *argv);
		return(FAILURE);
	}
	fmode=statrec.st_mode;
	if ((compress = fopen(argv2, "w")) == NULL) {
		fprintf(stderr,"unable to create %s\n", argv2);
		return(FAILURE);
	}
	chmod(argv2,fmode);

	if ( ldfhread(oldfile, &fhead) == FAILURE) {
		if (vflag > 0)
			fprintf(stderr,"cannot read header\n");
		return(ERROR);
	}

	indx += FILHSZ;
	if (fwrite(&fhead, FILHSZ, 1, compress) != 1)
		return(ERROR);

	if ( ldohseek(oldfile) == SUCCESS)
		if ( copy((long)HEADER(oldfile).f_opthdr) != SUCCESS) {
			if ( vflag > 0)
				fprintf(stderr,"error in copying opt header\n");
			return(ERROR);
		}
	indx += HEADER(oldfile).f_opthdr;

	for (i=1; i <= HEADER(oldfile).f_nscns; ++i) {
		if ( ldshread(oldfile, i, &sect) != SUCCESS) {
			if ( vflag > 0)
				fprintf(stderr,"cannot read sect head at %d\n", i);
			return(ERROR);
		}
		if ( i==0 || frstrel == 0)
			frstrel = sect.s_relptr;
		if ( i == 0 || frstln == 0)
			frstln = sect.s_lnnoptr;
		sumrel += sect.s_nreloc;
		sumln += sect.s_nlnno;

		if( fwrite(&sect, SCNHSZ, 1, compress) != 1)
			return(FAILURE);
		indx += SCNHSZ;
	}

	if (sumrel != 0)
		datasze = frstrel - indx;
	else
		if (sumln != 0)
			datasze = frstln - indx;
		else
			datasze = HEADER(oldfile).f_symptr - indx;

	if ( datasze != 0)
		if ( copy((long)datasze) != SUCCESS) {
			if ( vflag > 0)
				fprintf(stderr,"error in copying raw data\n");
			return(FAILURE);
		}
	indx = indx + datasze;

	if ( HEADER(oldfile).f_flags != F_RELFLG && sumrel != 0)
		for ( i=0; i < sumrel; i++) {
			if (fread(&rel, RELSZ, 1, IOPTR(oldfile)) < 1)
				return(FAILURE);
			if ( fwrite(&rel, RELSZ, 1, compress) != 1)
				return(FAILURE);
			indx += RELSZ;
		}

	if ( HEADER(oldfile).f_flags != F_LNNO && sumln != 0) {
		for ( i=0; i < sumln; i++) {
			if (fread(&line, LINESZ, 1, IOPTR(oldfile)) < 1)
				return(FAILURE);
			if (fwrite(&line, LINESZ, 1, compress) != 1)
				return(FAILURE);
			indx += LINESZ;
		}
	}

	if ( indx != HEADER(oldfile).f_symptr) {
		fprintf(stderr,"error in indexing\n");
		return(FAILURE);
	}

	for(k=0L; k < HEADER(oldfile).f_nsyms; k++) {
		ldtbread(oldfile, k, &symbol);
		fwrite(&symbol, SYMESZ, 1, compress); 
		for ( i=0; i < symbol.n_numaux; i++) {
			if (fread(&auxil, AUXESZ, 1, IOPTR(oldfile)) != 1)
				return(FAILURE);
			k++;
			if(fwrite(&auxil, AUXESZ, 1, compress) != 1)
				return(FAILURE);
		}
	}
#ifdef FLEXNAMES
	/*
	* Copy the string table (rest of the object file)
	*/
	while ((i = fread(buf, sizeof(char), BUFSIZ, IOPTR(oldfile))) > 0)
	{
		i -= fwrite(buf, sizeof(char), i, compress);
		if (i != 0)
			return (FAILURE);
	}
	if (i != 0)
		return (FAILURE);
#endif
	fclose(compress);
	ldclose(oldfile);
	return(SUCCESS);
}
