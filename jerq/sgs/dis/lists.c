/*	@(#)lists.c	1.6 11/11/83      */

#include	<stdio.h>
#include	<ar.h>
#include	"dis.h"
#include	"filehdr.h"
#include	"ldfcn.h"
#include	"sgs.h"
#include	"scnhdr.h"
#include	"linenum.h"
#include	"syms.h"
#include	"structs.h"
#if OLIST
#include	"olist_defs.h"
#else
#define	TOOL	"dis"
#endif

/*
 *	build_sections()
 *
 *	create the list of sections to be disassembled
 */

SCNLIST	*
build_sections()
{
	extern	char	*calloc();

	extern	LDFILE	*f_ptr;
	extern	FILHDR	filhdr;
	extern	int	nsecs;
	extern	char	*namedsec[];
	extern	int	namedtype[];
	extern	char	*fname;
	extern	int	trace;

	SCNLIST		*sclist;
	SCNLIST		*sclisttail;
	SCNLIST		*sectp;
	SCNHDR		scnhdr;
	int		sect;
	int		i;

	/* read all the section headers in the file.  If the section
	 * is one of the named sections, add it to the list.  If
	 * there were no named sections, and the section is a text
	 * section, add it to the list
	 */

	sclisttail = sclist = NULL;
	if (nsecs >= 0)
	{
		for ( i = 0; i <= nsecs; i++ )
		{
			FSEEK( f_ptr, FILHSZ + filhdr.f_opthdr, BEGINNING );
			for ( sect = 1; sect <= filhdr.f_nscns; sect++ )
			{
				FREAD( &scnhdr, SCNHSZ, 1, f_ptr );
				if (strncmp( namedsec[i], scnhdr.s_name, 8 ) == 0)
					break;
			}
			if (sect > filhdr.f_nscns)
			{
				fprintf( stderr, "%s%s: %s: %s: cannot find section header\n",
					SGS, TOOL, fname, namedsec[i] );
				continue;
			}
			if (trace > 0)
				printf( "\nsection name is {%s}\n", scnhdr.s_name );
			if ((scnhdr.s_scnptr == 0)
				|| (scnhdr.s_flags & STYP_DSECT)
				|| (scnhdr.s_flags & STYP_NOLOAD))
			{
				fprintf(stderr,"%s%s: %s: %.8s: not a loaded section\n",
					SGS, TOOL, fname, scnhdr.s_name );
				continue;
			}

			if ((sectp = (SCNLIST *) calloc( 1, sizeof(SCNLIST))) == NULL)
				fatal( "memory allocation failure" );
			sectp->shdr = scnhdr;
			sectp->scnum = sect;
			sectp->stype = namedtype[i];
			if (sclisttail)
				sclisttail->snext = sectp;
			sclisttail = sectp;
			if (sclist == NULL)
				sclist = sectp;
		}
	}
	else
		for ( sect = 1; sect <= filhdr.f_nscns; sect++ )
		{
			if (ldshread (f_ptr, sect, &scnhdr ) == FAILURE)
				fatal( "can't read section header %d\n" );
			if (trace > 0)
				printf( "\nsection name is {%s}\n", scnhdr.s_name );
	
			if ((strcmp( ".text", scnhdr.s_name, NCPS) != 0)
				&& (scnhdr.s_lnnoptr == 0)
				&& !(scnhdr.s_flags & STYP_TEXT))
				continue;
	
#if OLIST
			if ((strcmp( scnhdr.s_name, ".plb" ) == 0)
				|| (strcmp( scnhdr.s_name, ".ecd" ) == 0))
				continue;
#endif

			if ((scnhdr.s_scnptr == 0)
				|| (scnhdr.s_flags & STYP_DSECT)
				|| (scnhdr.s_flags & STYP_NOLOAD))
			{
				fprintf(stderr,"%s%s: %s: %.8s: not a loaded section\n",
					SGS, TOOL, fname, scnhdr.s_name );
				continue;
			}

			if ((sectp = (SCNLIST *) calloc( 1, sizeof(SCNLIST))) == NULL)
				fatal( "memory allocation failure" );
			sectp->shdr = scnhdr;
			sectp->scnum = sect;
			sectp->stype = TEXT;
			if (sclisttail)
				sclisttail->snext = sectp;
			sclisttail = sectp;
			if (sclist == NULL)
				sclist = sectp;
		}

	return( sclist );
}



/*
 *	section_free()
 *
 *	free the space used by the list of section headers
 */

section_free( sclist )
SCNLIST	*sclist;
{
	SCNLIST	*sectp;
	SCNLIST	*stemp;
	FUNCLIST	*funcp;
	FUNCLIST	*ftemp;

	if (sclist == NULL)
		return;

	sectp = sclist;
	while ( sectp )
	{
		stemp = sectp;
		funcp = sectp->funcs;
		sectp = sectp->snext;
		free( stemp );

		while ( funcp )
		{
			ftemp = funcp;
			funcp = funcp->nextfunc;
			free( ftemp->funcnm );
			free( ftemp );
		}
	}
}

/*
 *	build_funcs()
 *
 *	for each section in the list for section headers,
 *	make a list of the functions in that section
 */

#if !AR16WR || OLIST
build_funcs( sclist )
SCNLIST	*sclist;
{
	extern	char	*calloc();
	extern	char	*ldgetname();

	extern	LDFILE	*symb;
	extern	FILHDR	filhdr;

	SCNLIST		*sectp;
	SYMENT		symbol;
	AUXENT		axent;
	FUNCLIST	*func;
	char		*func_name;
	long		i;

	for ( i = 0; i < filhdr.f_nsyms; i++)
	{
		if (ldtbread( symb, i, &symbol ) == FAILURE )
			fatal( "cannot read symbol table" );

		for (sectp = sclist; sectp; sectp = sectp->snext)
			if (sectp->scnum == symbol.n_scnum)
				break;

		if (ISFCN( symbol.n_type ) || ((symbol.n_sclass==C_EXT ||
		    symbol.n_sclass == C_STAT) && 
		    (strncmp(sectp->shdr.s_name,".text",8) == 0) &&
		    (strncmp(symbol.n_name,".text",8) != 0)))
		{
			if ((func = (FUNCLIST *) calloc( 1, sizeof(FUNCLIST))) == NULL)
				fatal( "memory allocation failure");
			if ((func_name = ldgetname( symb, &symbol )) == NULL)
				fatal( "can't read function name");
			if ((func->funcnm = calloc(1, strlen( func_name ) + 1)) == NULL)
				fatal( "memory allocation failure" );
			strcpy( func->funcnm, func_name );
			func->faddr = symbol.n_value;
			func->fcnindex = i;
#if OLIST
			i++;
			if (ldtbread( symb, i, &axent ) == FAILURE )
				fatal( "cannot read symbol table" );
			func->fend = func->faddr + axent.x_sym.x_misc.x_fsize;
#else
			i += symbol.n_numaux;
#endif
			add_func( func, sclist, symbol.n_scnum );
		}

		else
			i += symbol.n_numaux;
	}

#if OLIST
	for ( sectp = sclist; sectp; sectp = sectp->snext )
		for ( func = sectp->funcs; func; func = func->nextfunc )
			if (func->nextfunc)
			{
				if (func->fend < func->nextfunc->faddr)
					func->fend = func->nextfunc->faddr;
			}
			else if (func->fend < (sectp->shdr.s_paddr + sectp->shdr.s_size))
				func->fend = sectp->shdr.s_paddr + sectp->shdr.s_size;
#endif
}

/*
 *	add_func()
 *
 *	add func to the list of functions associated with the section
 *	given by sect
 */

add_func( func, sclist, sect )
FUNCLIST	*func;
SCNLIST		*sclist;
short		sect;
{
	extern	char	*fname;

	static short	last_sect = 0;
	static FUNCLIST	*last_func = NULL;
	static char	*last_file = NULL;

	SCNLIST		*sectp;
	FUNCLIST	*funcp;
	FUNCLIST	*backp;
	static int	elist=1;

	/*
	 * if this function follows the last function added to the list,
	 * the addition can be done quickly
	 */
	if (elist && (last_sect == sect) && last_func && (last_func->faddr < func->faddr)
		&& last_file && !strcmp( fname, last_file ))
	{
		funcp = last_func->nextfunc;
			func->nextfunc = funcp;
			funcp = func;
			last_func = func;
			elist = 1;
	}
	else
	{
		/* find the corresponding section pointer */
		for ( sectp = sclist; sectp; sectp = sectp->snext )
		{
			if (sectp->scnum == sect)
				break;
		}

		if (sectp)
		{
			/* keep the list of functions ordered by address */
			if ((sectp->funcs == NULL)
				|| (sectp->funcs->faddr > func->faddr))
			{
				func->nextfunc = sectp->funcs;
				sectp->funcs = func;
				if (sectp->funcs == NULL) elist=1;
				else elist=0;
			}
			else
			{
				backp = sectp->funcs;
				funcp = sectp->funcs->nextfunc;
				for ( ; funcp; funcp = funcp->nextfunc)
				{
					if (func->faddr < funcp->faddr)
					{
						func->nextfunc = funcp;
						backp->nextfunc = func;
						break;
					}
					backp = funcp;
				}
				if (funcp == NULL)
					backp->nextfunc = func;
				elist = 0;
			}

			last_func = func;
			last_sect = sect;
			last_file = fname;
		}

		else
			free( func );
	}
}
#endif
