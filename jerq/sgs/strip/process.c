/* UNIX HEADER */
#include	<stdio.h>
#include 	<ar.h>

/* COMMON SGS HEADERS */
#include	"filehdr.h"
#include	"ldfcn.h"

/* SGS SPECIFIC HEADER */

#include	"paths.h"
#include	"sgs.h"
/* STRIP HEADER */
#include	"defs.h"

/* EXTERNAL VARIABLES DEFINED */
FILE	*strp1;
FILE	*strp2;
FILE	*stripout;
FILHDR	filehead;
#if AR16WR
FILE	*tempfil, *readtmp;
#endif


	/*  process(filename)
	 *
	 *  directs the stripping of the named object file
	 *  checks the object file header to determine that it can be stripped
	 *  opens a temporary file (tmpnam1) and then builds the stripped file in it
	 *  copies the temporary stripped file back to its original place (filename)
	 *
	 *	the flags in the file header are set to reflect the new state
	 *	of the file:
	 *	if all flags are OFF there will be no symbols or line numbers
	 *	if lflag is ON then there will be no line numbers
	 *	if sflag is ON then there will be no local symbols
	 *	if rflag or xflag is ON then there will be no local symbols 
	 *			or line numbers
	 *	if bflag is ON then there will be no local symbols except
	 *			.bb, .eb, .bf, .ef
	 *	if fflag if ON then there will be no local symbols or line numbers
	 *			and no relocation except from a section of type COPY
	 *
	 *  defines:
	 *	  - strp1 = fopen(tmpnam1, "w")  for building temporary
	 *		   = fopen(tmpnam1, "r") for copying temporary to original
	 * 	  - strp2 = fopen(tmpnam2, "w") for building temporary
	 *	  - stripout = fopen(filename "w") for copying temporary to original
	 *
	 *  calls:
	 *	  - checkflags(filename, &filehead) to check object file flags
	 *	  - buildtmp(filename, &filehead) to build the temporary file
	 *        - copytmp(filename, &filehead) to copy the temporary to the original
	 *	  - error(filename, message, level) when an error occurs; level
	 *		  indicates the severity of the error and the current strip state
	 *
	 *  returns SUCCESS or FAILURE
	 */


int
process(filename)

char	*filename;

{
	/* UNIX FUNCTIONS CALLED */
	extern	FILE	*fopen( );
	extern		fclose( ),
			exit( );
	extern	int	fseek( );
	extern	long	ftell( );

	/* COMMON OBJECT FILE ACCESS ROUTINES CALLED */
	extern	int	ldaclose( );

	/* STRIP FUNCTIONS CALLED */
	extern	int	checkflags( ),
			buildtmp( ),
			copyarh( ),
			copytmp( );
	extern		error( );

	/* EXTERNAL VARIABLES USED */
	extern	LDFILE	*ldptr;
	extern	char	tmpnam1[ ],
			tmpnam2[ ],
			tmpnam3[ ],
			tmpnam4[ ];
	extern	FILE	*strp1,
			*strp2,
			*stripout;
#if AR16WR
	extern	FILE	*tempfil,
			*readtmp,
#endif
	extern	FILHDR	filehead;
	extern	int	rflag,
			sflag;
	extern	int	fflag;
	extern	int	lflag;
	extern	ARCHDR	arhead;
	extern	int	aflag;
	extern	long	xmore;
	extern	int	mag;
#if FLEXNAMES
	extern	long	string_size;
#endif


	if (ldfhread(ldptr, &filehead) != SUCCESS)
	{
		error(filename, "cannot read file header", 1);
		return(FAILURE);
	}
	else
	{
		if ( ! ISMAGIC(filehead.f_magic) )
		{
			error(filename, "not the correct magic number", 1);
			return(FAILURE);
		}
  	}


	if (checkflags(filename, &filehead) != SUCCESS)
		return(FAILURE);

	if ((strp1 = fopen(tmpnam1, "w")) == NULL)
	{
		error(filename, "cannot open temporary file", 1);
		exit(FATAL);
	}
	if ((strp2 = fopen(tmpnam2, "w")) == NULL)
	{
		error(filename, "cannot open temporary file", 1);
		exit(FATAL);
	}

#if AR16WR
	if ( rflag || sflag )
	{
		if ((tempfil = fopen(tmpnam3, "w")) == NULL)
		{
			error(filename, "cannot open temporary file", 1);
			exit(FATAL);
		}
		if ((readtmp = fopen(tmpnam3, "r")) == NULL)
		{
			error(filename, "cannot open temporary file", 1);
			exit(FATAL);
		}
	}
#endif

	if (buildtmp(filename, &filehead) == SUCCESS)
	{
		xmore = ldclose(ldptr);
		fclose(strp1);
		fclose(strp2);

#if AR16WR
		if ( rflag || sflag )
		{
			fclose(tempfil);
			fclose(readtmp);
		}
#else
		freetempent( );
#endif

		if (aflag)
		{
			aflag = 0;
			if ((stripout = fopen(tmpnam4, "w")) == NULL)
			{
				error(filename, "cannot write cannot strip", 0);
				return(FAILURE);
			}
		}
		else if (TYPE(ldptr) != ARTYPE)
			if ((stripout = fopen( filename, "w" )) == NULL)
			{
				error(filename, "cannot write, cannot strip",5);
				return( FAILURE );
			}


		if ((strp1 = fopen(tmpnam1, "r")) == NULL)
		{
			error(filename, "cannot open temporary file", 5);
			exit(FATAL);
		}
		if ((strp2 = fopen(tmpnam2, "r")) == NULL)
		{
			error(filename, "cannot open temporary file", 5);
			exit(FATAL);
		}

		if (TYPE(ldptr) == ARTYPE)
		{
			fseek(strp1,0L,2);
			fseek(strp2,0L,2);
			arhead.ar_size = ftell(strp1) + ftell(strp2) + sizeof(FILHDR);
#if FLEXNAMES
			if (string_size > 0L)
				arhead.ar_size += string_size;
#endif
			arhead.ar_size += (arhead.ar_size & 1);
			fseek(strp1,0L,0);
			fseek(strp2,0L,0);
			if (copyarh(filename, &arhead) != SUCCESS)
			{
				fprintf(stderr, "can't copy archive header back");
			}
		}

		/* set the new file header flags appropriately */
		if (!lflag)
			filehead.f_flags |= F_LSYMS;
		if (!sflag)
			filehead.f_flags |= F_LNNO;
#if !UNIX
		if (fflag)
			filehead.f_flags |= F_RELFLG;
#endif

		if (copytmp(filename, &filehead) == SUCCESS)
		{
		 	if (xmore == SUCCESS )
				fclose(stripout);
			fclose(strp1);
			fclose(strp2);
		}
	}

	return(SUCCESS);
}


int
copyarh(filename, arhead)

 char 		*filename;
 ARCHDR		*arhead;


{

/*	UNIX FUNCTIONS CALLED */

	extern	int	fwrite( );
	extern		sputl( );

/*	STRIP FUNCTIONS CALLED */

	extern  resetsig( ),
		error( );

/*	EXTERNAL VARIABLES USED */

	extern	LDFILE *ldptr;

	extern  FILE   *stripout;

#ifdef PORT5AR
/*	LOCAL VARIABLES USED */

	struct arf_hdr		arfbuf;

	strncpy(arfbuf.arf_name,arhead->ar_name,sizeof(arfbuf.arf_name));
	sputl(arhead->ar_date,arfbuf.arf_date);
	sputl((long)arhead->ar_uid,arfbuf.arf_uid);
	sputl((long)arhead->ar_gid,arfbuf.arf_gid);
	sputl(arhead->ar_mode,arfbuf.arf_mode);
	sputl(arhead->ar_size,arfbuf.arf_size);
	if (fwrite( &arfbuf, sizeof( struct arf_hdr ), 1, stripout ) != 1 )
#else
#ifdef PORTAR
	struct	ar_hdr	arbuf;
	char		*p;

	sprintf( arbuf.ar_name, "%-16s", arhead->ar_name );
	p = arbuf.ar_name + sizeof( arbuf.ar_name ) - 1;
	while ( *--p == ' ' );
	*++p = '/';

	sprintf( arbuf.ar_date, "%-12ld", arhead->ar_date );
	sprintf( arbuf.ar_uid, "%-6u", arhead->ar_uid );
	sprintf( arbuf.ar_gid, "%-6u", arhead->ar_gid );
	sprintf( arbuf.ar_mode, "%-8o", arhead->ar_mode );
	sprintf( arbuf.ar_size, "%-10ld", arhead->ar_size );
	strncpy( arbuf.ar_fmag, ARFMAG, sizeof( arbuf.ar_fmag ) );
	if (fwrite( &arbuf, sizeof( struct ar_hdr ), 1, stripout ) != 1)
#else
	if (fwrite(&arhead, sizeof(ARCHDR), 1, stripout) != 1)
#endif
#endif
	{
		error(filename, "cannot recreate archive header", 5);
		resetsig( );
		return(FAILURE);
	}
	else
	{
		return(SUCCESS);
	}
}
/*
 *	@(#)process.c	1.9 11/17/83;
 */
