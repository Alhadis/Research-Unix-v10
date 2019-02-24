/* UNIX HEADER */
#include	<stdio.h>

/* COMMON SGS HEADERS */
#include	"filehdr.h"
#include	"ldfcn.h"

/* STRIP HEADER */
#include	"defs.h"


    /*  buildtmp(filename, filehead)
     *
     *  Directs the building of the stripped temporary file.
     *  Copies the optional header, section headers, section contents, and
     *  relocation or symbol table information (depending on lflag, xflag or
     *  bflag) to the temporary file.
     *
     *  calls:
     *      - cpyopthdr( ) to copy the optional header
     *      - cpyscnhdrs( ) to copy the section headers (update them too)
     *      - cpyscns( ) to copy the section contents
     *
     *		- if lflag is ON
     *			- cpyreloc( ) to copy relocation information
     *			- copysyms( ) to copy all symbols (update them too)
     *		- if sflag is ON
     *			- cpyexts( ) to copy external and static symbols and
     *				fix up the line number entries
     *			- newreloc( ) to copy and fix up relocation entries
     *			- cpylnums( ) to copy and fix up the line number entries
     *		- if xflag is ON
     *			- cpyexts( ) to copy external and static symbols
     *		- if bflag is ON
     *			- cpyexts( ) to copy external, static, .bb, .eb, .bf,
     *			  .ef symbols
     *		- if rflag is ON
     *			- if local symbols are already stripped
     *				- cpyreloc( ) to copy relocation information
     *				- cpysyms to copy all symbols
     *			- else
     *				- cpyexts( ) to copy external and static symbols
     *				- newreloc( ) to copy and fix up relocation entries
     *
     *      - error(file, message, level) if anything goes wrong
     *
     *  buildtmp( ) keeps track of the new location of the symbol table in a
     *  partially stripped (lflag, xflag, bflag) file
     *
     *  returns SUCCESS or FAILURE
     */


int
buildtmp(filename, filehead)

char	*filename;
FILHDR	*filehead;

{
    /* STRIP FUNCTIONS CALLED */
    extern int		cpyopthdr( ),
			cpyscnhdrs( ),
			copysyms( );
    extern long		copyscns( ),
			cpyreloc( ),
			cpylnums( ),
			newreloc( ),
			copyexts( );
    extern		error( );

    /* EXTERNAL VARIABLES USED */
    extern LDFILE	*ldptr;
    extern FILE		*strp2;
    extern FILE		*strp1;
    extern char		tmpnam1[ ];
    extern int		lflag,
			xflag,
			bflag,
			sflag,
			rflag;

    if (filehead->f_opthdr != 0) {
	if (cpyopthdr( ) != SUCCESS) {
	    error(filename, "cannot copy optional header", 2);
	    return(FAILURE);
	}
    }

    if (cpyscnhdrs( ) != SUCCESS) {
	error(filename, "cannot copy section headers", 2);
	return(FAILURE);
    }

    if ((filehead->f_symptr = copyscns( )) == ERROR) {
	error(filename, "cannot copy sections", 2);
	return(FAILURE);
    }

	if ( lflag ) {
		if ((filehead->f_flags & F_RELFLG) == 0) {
			if ((filehead->f_symptr = cpyreloc(filehead->f_symptr)) == ERROR) {
		    		error(filename, "cannot copy relocation information", 2);
		    		return(FAILURE);
			}
		}
		if (copysyms( ) != SUCCESS) {
			error( filename, "cannot copy symbol table", 2 );
			return( FAILURE );
		}

	} else if ( sflag) {
		if ((filehead->f_nsyms = copyexts( filename )) == ERROR) {
			error( filename, "cannot copy external symbols", 2);
			return( FAILURE );
		}
		if ( !(filehead->f_flags & F_RELFLG) && ((filehead->f_symptr = newreloc( filehead->f_symptr )) == ERROR)) {
			error( filename, "cannot copy relocation entries", 2 );
			return( FAILURE );
		}
		if ( !(filehead->f_flags & F_LNNO) && ((filehead->f_symptr = cpylnums( filehead->f_symptr )) == ERROR)) {
			error( filename, "cannot copy line numbers", 2 );
			return( FAILURE );
		}

	} else if (xflag || bflag) {
		if ((filehead->f_nsyms = copyexts(filename)) == ERROR) {
			error(filename, "cannot copy external symbols", 3);
			return(FAILURE);
		}

	} else if (rflag ) {
		if (filehead->f_flags & F_LSYMS) {
			if ((filehead->f_symptr = cpyreloc( filehead->f_symptr )) == ERROR ) {
				error( filename, "cannot copy relocation information", 2);
				return( FAILURE );
			}
			if (copysyms( ) != SUCCESS) {
				error( filename, "cannot copy symbol table", 2);
				return( FAILURE );
			}
		} else {
			if ((filehead->f_nsyms = copyexts(filename)) == ERROR) {
				error(filename,"cannot copy external symbols", 3);
				return(FAILURE);
			}
			if((filehead->f_flags & F_RELFLG) == 0) {
				if ((filehead->f_symptr = newreloc(filehead->f_symptr)) == ERROR) {
					error(filename, "cannot copy relocation information",2); 
					return(FAILURE);
				}
			}
		}

	} else {
		filehead->f_symptr = 0L;
		filehead->f_nsyms = 0L;
	}

    return(SUCCESS);
}
/*
 *	@(#)buildtmp.c	1.5 11/4/83;
 */
