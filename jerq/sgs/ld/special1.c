static char ID[] = "@(#) special1.c: 1.1 1/7/82";

#include <stdio.h>

#include "system.h"
#include "structs.h"
#include "extrns.h"



#if TRVEC
void
chktvorg(org, tvbndadr)

long	org, *tvbndadr;
{
	/*
	 * check user-supplied .tv origin for legality
	 * if illegal, side-effect tvspec.tvbndadr
	 *  and issue warning message
	 */

			if( (org & 0xf) != 0 )
				yyerror("tv origin (%10.0lx) must be a multiple of 16", org);
			*tvbndadr = (org + 0xfL) & ~0xfL;

}
#endif

void
specflags(flgname, argptr)
char *flgname;
char **argptr;
{

	/*
	 * process special flag specifications for m32ld
	 * these flags have fallen through switch of argname in ld00.c
	 */

	switch ( *flgname ) {

		case 'n':
			break;
		default:
			yyerror("unknown flag: %s", flgname);

		}
}
