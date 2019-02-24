static char ID[] = "@(#) specsyms.c: 1.2 3/2/83";

#include <stdio.h>

#include "system.h"
#include "structs.h"
#include "sgsmacros.h"
#include "extrns.h"
set_spec_syms()
{

/*
 * For non-relocatable link edits, set up the special symbols.
 *
 *	1. _TORIGIN : segment register value for the REGION containing
 *			text
 *	   _DORIGIN : segment register value for the REGION containing
 *			data
 *	   _TVORIG  : segment register value for the REGION containing
 *			the tv
 *	
 *	2. _ETEXT : next available address after the end of the output
 *			section _TEXT
 *	   _EDATA : next available address after the end of the output
 *			section _DATA
 *	   _END   : next available address after the end of the output
 *			section _BSS
 *
 *	3. _SORIGIN : first address at or following _END which is 
 *			aligned to a 16-byte boundary
 *
 *	4. _TV : special symbol with auxiliary entry giving
 *		 tvlength and tvrange.
 */

	register OUTSECT *osptr;
	register long base;
	SYMTAB	*sym;
	AUXENT aux;

	if ( ! aflag )
		return;


	osptr = (OUTSECT *) outsclst.head;
	while (osptr) {
		if( equal(osptr->oshdr.s_name,_TEXT, 8))
			creatsym(_ETEXT,
				osptr->oshdr.s_paddr + osptr->oshdr.s_size);
		else if( equal(osptr->oshdr.s_name,_DATA, 8))
			creatsym(_EDATA,
				osptr->oshdr.s_paddr + osptr->oshdr.s_size);
		else if( equal(osptr->oshdr.s_name,_BSS, 8)) {
			base = osptr->oshdr.s_paddr + osptr->oshdr.s_size;
			creatsym(_END, base);
			}
#if TRVEC
		else if( equal(osptr->oshdr.s_name,_TV,8) ) {
			creatsym(_TV, ((tvspec.tvbndadr == -1L) ?
					0L : tvspec.tvbndadr));
			zero(&aux, AUXESZ);
			aux.x_tv.x_tvlen = tvspec.tvlength;
			aux.x_tv.x_tvran[0] = tvspec.tvrange[0];
			aux.x_tv.x_tvran[1] = tvspec.tvrange[1];
			aux.x_tv.x_tvfill = -1L;
			sym = findsym(_TV);
			putaux( makeaux(sym, &aux, 0), 1 );
			putsym(sym, 1);
			}
#endif
		osptr = osptr->osnext;
		}
}
